
# rm -rf build
mkdir build
cd build &&
cmake .. -DCMAKE_CXX_COMPILER=clang++ &&
cmake --build . &&
./cpplox ../demo
cd ..
