/**
* Copyright (c) 2020 Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <ctime>
#include <cmath>
#include <filesystem>

#include "qformat.hpp"
#include "activation.hpp"

namespace fs = std::filesystem; // Must use C++ 17

typedef enum
{
    tanhActivation,
    sigmoidActivation,
    expActivation,
    logActivation,
    endActivation
} activation_e;

typedef struct
{
    size_t startYear;
    size_t endYear;
} copyrightSpan_t;

static std::string folderPathString;
static char const* const activationTags[] = {"TINYMIND_USE_TANH_", "TINYMIND_USE_SIGMOID_", "TINYMIND_USE_EXP_", "TINYMIND_USE_LOG_"};
static const uint8_t tables[] = {8, 16, 32, 64};

using namespace std;

static const string lutFileName = "lookupTables.cpp";
static const string selectorPath[] = {"tanh.hpp", "sigmoid.hpp", "exp.hpp", "log.hpp"};
static const string valuesPathPrefix[] = {"tanhValues", "sigmoidValues", "expValues", "logValues"};
static const string structPrefixes[] = {"Tanh", "Sigmoid", "Exp", "Log"};
static const string includePrefix[] = {"tanh", "sigmoid", "exp", "log"};

static_assert((sizeof(selectorPath) / sizeof(string)) == endActivation, "Invalid selector file path array size.");
static_assert((sizeof(valuesPathPrefix) / sizeof(string)) == endActivation, "Invalid path prefix array size.");
static_assert((sizeof(structPrefixes) / sizeof(string)) == endActivation, "Invalid struct prefix array size.");
static_assert((sizeof(includePrefix) / sizeof(string)) == endActivation, "Invalid include prefix array size.");

static double sigmoid(const double x)
{
    const double exponential = std::exp(x);
    const double result = (exponential / (exponential + static_cast<double>(1.0)));
    
    return result;
}

static copyrightSpan_t getCurrentCopyright(const string& path)
{
    copyrightSpan_t span = {0,0};
    ifstream file;
	char line[256] = {'\0'};
    char startYear[5] = {0};
    char endYear[5] = {0};
    size_t len;
    size_t pos;
    int yearIndex = 0;

    for(int i = 0;i < 10;++i)
    {
        file.open(path);
        if(file.is_open())
        {
            break;
        }
    }

    if(!file.is_open())
    {
        return span;
    }

    for(int i = 0;i < 4;++i)
    {
        file.getline(&line[0], 256); // Assumes the Copyright tag is in the 4th line
    }

    len = strlen(&line[0]);

    for(pos = 0;pos < len;++pos)
    {
        if(isdigit(line[pos]))
        {
            startYear[yearIndex] = line[pos];
            ++yearIndex;
            if(yearIndex == 4)
            {
                break;
            }
        }
    }

    span.startYear = atoi(&startYear[0]);
    yearIndex = 0;
    ++pos;

    for(;pos < len;++pos)
    {
        if(isdigit(line[pos]))
        {
            endYear[yearIndex] = line[pos];
            ++yearIndex;
            if(yearIndex == 4)
            {
                break;
            }
        }
    }

    span.endYear = atoi(&endYear[0]);
    if(span.endYear <= span.startYear)
    {
        span.endYear = 0;
    }

    return span;
}

static void writeFileCopyrightAndLicense(const string& path, const copyrightSpan_t& span)
{
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);
    const size_t currentYear = (1900 + timePtr->tm_year);

    ofstream outFile(path);
    outFile << "/**" << endl;
    if((span.startYear == 0) && (span.endYear == 0))
    {
        outFile << "* Copyright " << currentYear << " Intel Corporation All Rights Reserved." << endl;
    }
    else if((span.startYear != 0) && (span.endYear == 0))
    {
        if(currentYear == span.startYear)
        {
            outFile << "* Copyright " << currentYear << " Intel Corporation All Rights Reserved." << endl;
        }
        else
        {
            outFile << "* Copyright " << span.startYear << "-" << currentYear << " Intel Corporation All Rights Reserved." << endl;
        }
    }
    else if((span.startYear != 0) && (span.endYear != 0))
    {
        outFile << "* Copyright " << span.startYear << "-" << currentYear << " Intel Corporation All Rights Reserved." << endl;
    }
    outFile << "*" << endl;
    outFile << "* Permission is hereby granted, free of charge, to any person obtaining a copy" << endl;
    outFile << "* of this software and associated documentation files (the \"Software\"), to deal" << endl;
    outFile << "* in the Software without restriction, including without limitation the rights" << endl;
    outFile << "* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell" << endl;
    outFile << "* copies of the Software, and to permit persons to whom the Software is" << endl;
    outFile << "* furnished to do so, subject to the following conditions:" << endl;
    outFile << "*" << endl;
    outFile << "* The above copyright notice and this permission notice shall be included in all" << endl;
    outFile << "* copies or substantial portions of the Software." << endl;
    outFile << "*" << endl;
    outFile << "* THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR" << endl;
    outFile << "* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY," << endl;
    outFile << "* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE" << endl;
    outFile << "* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER" << endl;
    outFile << "* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM," << endl;
    outFile << "* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE" << endl;
    outFile << "* SOFTWARE." << endl;
    outFile << "*/" << endl;
    outFile << endl;
    outFile.flush();
}

static void writeNamespaceBegin(const string& path)
{
    ofstream outFile(path, ofstream::app);

    outFile << "namespace tinymind {" << endl;
}

static void writeNamespaceEnd(const string& path)
{
    ofstream outFile(path, ofstream::app);

    outFile << "}" << endl;
}

static void writeActivationFileHeader(const string& path)
{
    copyrightSpan_t span = getCurrentCopyright(path);

    writeFileCopyrightAndLicense(path, span);

    ofstream outFile(path, ofstream::app);
    outFile << "#pragma once" << endl << endl;
}

static void writeFileHeader(const string& path)
{
    copyrightSpan_t span = getCurrentCopyright(path);

    writeFileCopyrightAndLicense(path, span);

    ofstream outFile(path, ofstream::app);
    outFile << "#pragma once" << endl << endl;
    outFile << "#include \"activation.hpp\"" << endl << endl;
}

static void writeTableHeader(const string& path, char const* const preprocessorTag, const int totalBits, const int fixedBits, const int fracBits, const activation_e activationType)
{
    const string spaces("    ");
    ofstream outFile;
    outFile.open(path, ofstream::app);
    outFile << spaces << "#if (defined(" << activationTags[activationType] << fixedBits << "_" << fracBits << "))" << endl;
    outFile << spaces << "struct " << structPrefixes[activationType] << "ValuesTableQ" << fixedBits << "_" << fracBits << endl;
    outFile << spaces << "{" << endl;
    outFile << spaces << "    static const uint" << totalBits << "_t values[NUMBER_OF_ACTIVATION_TABLE_VALUES];" << endl;
    outFile << spaces << "};" << endl;
    outFile << spaces << "#endif // "  << "(defined(" << preprocessorTag << fixedBits << "_" << fracBits << "))" << endl;
    outFile.close();
}

static void writeTableFooter(const string& path, char const* const preprocessorTag, uint64_t fixedBits, uint64_t fracBits)
{
    ofstream outFile;
    outFile.open(path, ofstream::app);
    outFile << "};" << endl;
    outFile << "#endif // "  << "(defined(" << preprocessorTag << fixedBits << "_" << fracBits << "))" << endl << endl;
}

static void writeLutFileHeader(const string& path)
{
    copyrightSpan_t span = getCurrentCopyright(path);

    writeFileCopyrightAndLicense(path, span);

    ofstream outFile(path, ofstream::app);
    outFile << "#include <cstdint>" << endl << endl;
    
    outFile << "#include \"activation.hpp\"" << endl;
    for(int i = 0; i < endActivation;++i)
    {
        for(size_t value : tables)
        {
            outFile << "#include \"" << includePrefix[i] << "Values" << value << "Bit.hpp\"" << endl;
        }
    }
    outFile << endl;
    outFile.close();
}

static void writeTableSelectorTemplate(const string& path, const activation_e activationType)
{
    const string spaces("    ");
    ofstream outFile;
    outFile.open(path, ofstream::app);
    outFile << spaces << "template<unsigned FixedBits, unsigned FracBits, bool IsSigned>\n";
    outFile << spaces << "struct " << structPrefixes[activationType] << "TableValueSize\n";
    outFile << spaces << "{\n";
    outFile << spaces << "};\n\n";
}

static void writeTableSelectorInclude(const string& path, const int numBits, const activation_e activationType)
{
    ofstream outFile;
    outFile.open(path, ofstream::app);
    outFile << "#include \"" << includePrefix[activationType] << "Values" << numBits << "Bit.hpp\"" << endl;
}

static void writeSelectorMinMaxShift(const string& path)
{
    ofstream outFile;
    outFile.open(path, ofstream::app);
    outFile << "#define NUMBER_OF_ACTIVATION_TABLE_VALUES " << NUMBER_OF_ACTIVATION_TABLE_VALUES << endl;
    outFile << "#define MIN_X_TABLE_VALUE " << MIN_X_TABLE_VALUE << endl;
    outFile << "#define MAX_X_TABLE_VALUE " << MAX_X_TABLE_VALUE << endl;
    outFile << "#define ACTIVATION_DELTA_SHIFT " << ACTIVATION_DELTA_SHIFT << endl;
    outFile << endl;
}

template <typename T>
static void writeTableSelectorCases(const string& path, const int fixedBits, const int fracBits, const activation_e activationType)
{
    const string spaces("    ");
    ofstream outFile;
    outFile.open(path, ofstream::app);
    outFile << spaces << "#if (defined(" << activationTags[activationType] << fixedBits << "_" << fracBits << "))" << endl;
    outFile << spaces << "template<>" << endl;
    outFile << spaces << "struct " << structPrefixes[activationType] << "TableValueSize<"<< fixedBits << ", " << fracBits << ", true>\n";
    outFile << spaces << "{" << endl;
    outFile << spaces << "    typedef " << structPrefixes[activationType] << "ValuesTableQ" << fixedBits << "_" << fracBits << " " << structPrefixes[activationType] << "TableType;\n";
    outFile << spaces << "};" << endl;
    outFile << spaces << "#endif // "  << "(defined(" << activationTags[activationType] << fixedBits << "_" << fracBits << "))" << endl << endl;
}

static void writeTableSelector(string path, const activation_e activationType)
{
    const string spaces("    ");
    ofstream outFile;
    outFile.open(path, ofstream::app);
    outFile << spaces << "template<unsigned FixedBits,unsigned FracBits, bool IsSigned>\n";
    outFile << spaces << "struct " << structPrefixes[activationType] << "ValuesTableSelector\n";
    outFile << spaces << "{" << endl;
    outFile << spaces << "    typedef typename " << structPrefixes[activationType] << "TableValueSize<FixedBits, FracBits, IsSigned>::" << structPrefixes[activationType] <<
                "TableType " << structPrefixes[activationType] << "TableType;\n";
    outFile << spaces << "};" << endl;
}

template <typename T>
static void writeLutValues(string path, const size_t totalBits, uint64_t fixedBits, uint64_t fracBits, const activation_e activationType)
{
    const string spaces("    ");
    const double valueDelta = (static_cast<double>(1.0) / static_cast<double>(1 << ACTIVATION_DELTA_SHIFT));
    const uint64_t one = fixedBits;
    double value = MIN_X_TABLE_VALUE;
    ofstream outFile;
    outFile.open(path, ofstream::app);
    double activate;
    double num;

    outFile << std::dec;
    outFile << spaces << "#if (defined(" << activationTags[activationType] << fixedBits << "_" << fracBits << "))" << endl;
    outFile << spaces << "const uint" << totalBits << "_t " << structPrefixes[activationType] << "ValuesTableQ" << fixedBits << "_" << fracBits << "::values[NUMBER_OF_ACTIVATION_TABLE_VALUES] = {" << endl;

    for (int i = 0; i < NUMBER_OF_ACTIVATION_TABLE_VALUES; ++i)
    {
        if (tanhActivation == activationType)
        {
            activate = tanh(value);
        }
        else if (sigmoidActivation == activationType)
        {
            activate = sigmoid(value);
        }
        else if (expActivation == activationType)
        {
            activate = exp(value);
        }
        else if (logActivation == activationType)
        {
            if(value > 0.0f)
            {
                activate = log(value);
            }
            else
            {
                activate = 0.0;
            }
        }
        else
        {
            assert(false);
        }

        uint64_t leftShift = static_cast<T>(1ULL << fracBits);
        num = activate * leftShift;

        switch (std::numeric_limits<T>::digits)
        {
            case 8:
                outFile << spaces << "            0x" << std::hex << std::uppercase << (int)(uint8_t)num << "," << endl; // cast to uint8_t first for the correct value. cast next to int to write to file correctly as a number not a char.
                break;

            case 16:
                outFile << spaces << "            0x" << std::hex << std::uppercase << (uint16_t)num << "," << endl;
                break;

            case 32:
                outFile << spaces << "            0x" << std::hex << std::uppercase << (uint32_t)num << "," << endl;
                break;

            case 64:
                outFile << spaces << "            0x" << std::hex << std::uppercase << (uint64_t)num << "," << endl;
                break;
            default:
                assert(0);
        }
    
        value += valueDelta;
    }

    outFile << std::dec;
    outFile << spaces << "};" << endl;
    outFile << spaces << "#endif // "  << "(defined(" << activationTags[activationType] << fixedBits << "_" << fracBits << "))" << endl;
}

static void generateLut(const string& path, const activation_e activationType)
{
    for(uint8_t index = 0;index < ((sizeof(tables) / sizeof(tables[0])));++index)
    {
        const uint8_t totalBits = tables[index];

        for (int i = 1; i < totalBits; i++)
        {
            switch (totalBits)
            {
            case 8:
                writeLutValues<uint8_t>(path, totalBits, i, totalBits - i, activationType);
                break;

            case 16:
                writeLutValues<uint16_t>(path, totalBits, i, totalBits - i, activationType);
                break;

            case 32:
                writeLutValues<uint32_t>(path, totalBits, i, totalBits - i, activationType);
                break;

            case 64:
                writeLutValues<uint64_t>(path, totalBits, i, totalBits - i, activationType);
                break;
            default:
                assert(0);
            }
        }
    }
}

static void generateHeader(const activation_e activationType)
{
    fs::path selectorFilePath(folderPathString);

    selectorFilePath /= selectorPath[activationType];

    for(uint8_t index = 0;index < ((sizeof(tables) / sizeof(tables[0])));++index)
    {
        fs::path filePath(folderPathString);
        const uint8_t totalBits = tables[index];
        string fileName = valuesPathPrefix[activationType] + to_string(totalBits);
        
        fileName.append("Bit.hpp");

        filePath /= fileName;

        writeFileHeader(filePath.string());

        writeNamespaceBegin(filePath.string());

        for (int i = 1; i < totalBits; i++)
        {
            writeTableHeader(filePath.string(), activationTags[activationType], totalBits, i, totalBits - i, activationType);
        }

        writeNamespaceEnd(filePath.string());
    }


    writeFileHeader(selectorFilePath.string());

    for(uint8_t index = 0;index < ((sizeof(tables) / sizeof(tables[0])));++index)
    {
        const uint8_t totalBits = tables[index];
        writeTableSelectorInclude(selectorFilePath.string(), totalBits, activationType);
    }

    {
        ofstream outFile(selectorFilePath.string(), ofstream::app);
        outFile << endl;
    }

    writeNamespaceBegin(selectorFilePath.string());

    writeTableSelectorTemplate(selectorFilePath.string(), activationType);

    for(uint8_t index = 0;index < ((sizeof(tables) / sizeof(tables[0])));++index)
    {
        const uint8_t totalBits = tables[index];
        
        for (int i = 1; i < totalBits; i++)
        {
            switch (totalBits)
            {
            case 8:
                writeTableSelectorCases<uint8_t>(selectorFilePath.string(), i, totalBits - i, activationType);
                break;

            case 16:
                writeTableSelectorCases<uint16_t>(selectorFilePath.string(), i, totalBits - i, activationType);
                break;

            case 32:
                writeTableSelectorCases<uint32_t>(selectorFilePath.string(), i, totalBits - i, activationType);
                break;

            case 64:
                writeTableSelectorCases<uint64_t>(selectorFilePath.string(), i, totalBits - i, activationType);
                break;
            default:
                assert(0);
            }
        }
    }

    writeTableSelector(selectorFilePath.string(), activationType);

    writeNamespaceEnd(selectorFilePath.string());
}

int main(const int argc, char *argv[])
{
    if(argc != 2)
    {
        cerr << "Must provide folder path for output files." << endl;
        return -1;
    }

    folderPathString = argv[1];

    fs::path folderPath(folderPathString);

    fs::path activationHeaderPath(folderPath);
    activationHeaderPath /= "activation.hpp";

    writeActivationFileHeader(activationHeaderPath.string());
    writeSelectorMinMaxShift(activationHeaderPath.string());

    generateHeader(sigmoidActivation);
    generateHeader(tanhActivation);
    generateHeader(expActivation);
    generateHeader(logActivation);

    fs::path lutPath(folderPath);
    lutPath /= lutFileName;

    writeLutFileHeader(lutPath.string());

    writeNamespaceBegin(lutPath.string());

    for(int activation = tanhActivation;activation < endActivation;++activation)
    {
        generateLut(lutPath.string(), (activation_e)activation);
    }

    writeNamespaceEnd(lutPath.string());
}

