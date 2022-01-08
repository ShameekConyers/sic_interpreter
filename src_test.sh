mkdir build
cd build &&
cmake .. -DCMAKE_CXX_COMPILER=clang++ -GNinja &&
cmake --build . &&
ctest --output-on-failure
cd ..
