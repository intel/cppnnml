<html>
<body>
<h1>Tinymind</h1>
<p>Years ago, I began writing this code with the goal of instantiating and running neural networks and machine learning algorithms within embedded systems. I did not have an FPU, GPU, or any vectorized instructions at my disposal so I needed to ensure I could run these algorithms with only a very simple CPU. The end result is this library of C++ templates. The code within the templates are contained within the namespace tinymind.</p>
<p>My inspiration for these libraries is Andrei Alexandrescu's <a href="https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design">Modern C++ Design</a>. This book was my first exposure to the power of C++ templates and <a href="https://en.wikipedia.org/wiki/Template_metaprogramming">template metaprogramming</a>. In the book he describes how to design your code using policy classes as template parameters to customize behavior. I used this idea to make the code within these libraries as small and efficient as possible.</p>
<p>Today, tinymind provides: Feed-forward neural networks, recurrent neural networks, Q format, and Q learning.</p>
<p>See the <a href="https://github.com/intel/cppnnml/wiki">wiki pages</a> and tutorials which document how the code is designed as well as how to use the code within tinymind to instantiate and run neural networks as well as machine learning algorithms on virtually any device which has C++ compiler support.</p>
<p>Dan McLeran</p>
<p>@tinymind_cpp</p>
<p>danmcleran@gmail.com</p>
<p>daniel.mcleran@intel.com</p>
</body>
</html>
