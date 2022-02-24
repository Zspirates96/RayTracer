To compile the code:

cd path/to/RayTracer
mkdir build
cd build
cmake ..
cmake --build .


This compiles both the raytracer and the example programs, executables of which can be found in build/ folder.

To run the examples, do:

1. for the json parsing example:

./jsonexample ../examples/example.json

2. for the vector library example:

./vectorexample

3. for the raytracer provided:

./raytracer ../examples/example.json <path_to_output_img>/testout.ppm
