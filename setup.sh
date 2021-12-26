
rm -rf build
mkdir build &&
cd build &&
cmake .. &&
cmake --build .
./cpplox ../demo
cd ..
