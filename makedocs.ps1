
$BuildDirectory = ".\out\build\x64-Debug\"

# Build the documentation targets
cmake.exe --build $BuildDirectory --target Doxygen_utility && cmake.exe --build $BuildDirectory --target Sphinx_utility

# Run the documentation
& "$BuildDirectory\docs\Sphinx\index.html"