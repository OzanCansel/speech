rm -rf build
mkdir build
cd build
if [[ "$OSTYPE" == "msys" ]]; then
	cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release .. 
else
	cmake -DCMAKE_BUILD_TYPE=Release ..
fi
cmake --build .
