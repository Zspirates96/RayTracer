To compile the code:

cd path/to/RayTracer

mkdir build

cd build

cmake ..

cmake --build .

This compiles both the raytracer and the example programs, executables of which can be found in build/debug folder.

To run the example, do:

./debug/raytracer ../examples/example.json <path_to_output_img>/testout.ppm

For bvh to work, replace RayTracer.cpp with RayTracer_bvh.cpp.

Then, rename RayTracer_bvh.cpp as RayTracer.cpp

An example image with pointlight on the right and arealight on the left:

without depth of field:
![alt text](https://github.com/Zspirates96/RayTracer/blob/main/final_no_dof.jpg?raw=true)

with depth of field (1 sampling):
![alt text](https://github.com/Zspirates96/RayTracer/blob/main/final_dof.jpg?raw=true)
