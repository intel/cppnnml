<html>
<body>
<h1>Tinymind</h1>
Years ago, I began writing this code with the goal of instantiating and running neural networks and machine learning algorithms within embedded systems. I did not have an FPU, GPU, or any vectorized instructions at my disposal so I needed to ensure I could run these algorithms with only a very simple CPU. The end result is this library of C++ templates. The code within the templates are contained within the namespace tinymind.

My inspiration for these libraries is Andrei Alexandrescu's Modern C++ Design. This book was my first exposure to the power of C++ templates and template metaprogramming. He describes how to design your code using policy classes as template parameters to customize behavior. I used this idea to make the code within these libraries as small and efficient as possible.

Today, tinymind provides: Feed-forward neural networks, recurrent neural networks, Q format, and Q learning.

See the wiki pages and tutorials which document how the code is designed as well as how to use the code within tinymind to instantiate and run neural networks as well as machine learning algorithms on virtually any device which has C++ compiler support.
</body>
</html>
