# ECE474Hw2

The following changes will allow the usage of cmake on a Linux system.
Tutorial on how to use Cmake(Linux Only):
https://www.youtube.com/watch?v=elycr5wi9_g

Two scripts should handle all cmake steps, so we can focus more on the code
main.cpp(inside the src folder).

install_dependecies script installs the dependecies needed to run cmake and edit those files.
create_executable script creates a build folder, compiles, and generates dbgen executable from main.cpp

If you are running on a Windows, you could in theory run the scripts through the WSL command window which Microsft offers. Look with the Window's search bar. 

My suggestion:
  Making an object for each variables which holds: name, type, size
  Contain all those objects in a vector
