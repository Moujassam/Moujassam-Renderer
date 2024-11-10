all: game
	echo "all"

game: init
	c++  -std=c++2a -I./include/ -I./include/LLGL/  -o game ./src/main.cpp -L./lib/bin/ -lLLGL

game_macos: init_macos
	c++  -std=c++2a -I./include/ -I./include/LLGL/  -o game ./src/main.cpp -L./lib/bin/ -lLLGL

init: ./lib/bin/libLLGL.so ./include/Moujassam-Math
	echo "init"

init_macos: ./lib/bin/libLLGL.dylib ./include/Moujassam-Math
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

./lib/bin/libLLGL.dylib: lib/LLGL
	cd ./lib/LLGL/ &&  ./BuildMacOS.command
	mkdir -p ./lib/bin
	cp ./lib/LLGL/build_macos/build/libLLGL* ./lib/bin/
	cp ./lib/LLGL/build_macos/build/libLLGL* ./
	mkdir -p ./include
	cp -r ./lib/LLGL/include/LLGL ./include/
	cp -r ./lib/LLGL/build_macos/GaussianLib/include/* ./include/

lib/LLGL:
	mkdir -p ./lib
	cd ./lib/ && curl --output LLGL.zip -L https://github.com/LukasBanana/LLGL/archive/refs/heads/master.zip && unzip LLGL.zip && rm LLGL.zip && mv LLGL-master LLGL
