all: game
	echo "all"

game: init
	icpx -fsycl -v -std=c++17 -I./include/ -I./include/LLGL/  -o game ./src/main.cpp -L./lib/bin/ -lLLGL

init: ./lib/bin/libLLGL.so ./lib/tinygltf ./include/Moujassam-Math
	echo "init"

./include/Moujassam-Math:
	cd include && git clone git@github.com:Moujassam/Moujassam-Math.git
./lib/bin/libLLGL.so: lib/LLGL
	cd ./lib/LLGL/ &&  ./BuildLinux.sh
	mkdir -p ./lib/bin
	cp ./lib/LLGL/build_linux/build/libLLGL* ./lib/bin/
	cp ./lib/LLGL/build_linux/build/libLLGL* ./
	mkdir -p ./include
	cp -r ./lib/LLGL/include/LLGL ./include/
	cp -r ./lib/LLGL/build_linux/GaussianLib/include/* ./include/

lib/LLGL:
	mkdir -p ./lib
	cd ./lib/ && curl --output LLGL.zip -L https://github.com/LukasBanana/LLGL/archive/refs/heads/master.zip && unzip LLGL.zip && rm LLGL.zip && mv LLGL-master LLGL

lib/tinygltf:
	cd ./lib && curl --output tinygltf.zip -L https://github.com/syoyo/tinygltf/archive/refs/heads/release.zip && unzip tinygltf.zip && rm tinygltf.zip && mv tinygltf-release tinygltf && cd ../include && mkdir -p tinygltf && cp ../lib/tinygltf/*.h ./tinygltf