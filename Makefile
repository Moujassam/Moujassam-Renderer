all:
	echo "all"

init: lib/LLGL.tar.gz
	cd lib && tar -xvf LLGL.tar.gz
	cd ./lib/LLGL-Release-v0.03b/ &&  ./BuildLinux.sh
	cp ./lib/LLGL-Release-v0.03b/build_linux/build/libLLGL* ./lib/bin/
	mkdir -p ./include
	cp -r ./lib/LLGL-Release-v0.03b/include/LLGL ./include/

lib/LLGL.tar.gz:
	mkdir -p ./lib
	curl -L -o ./lib/LLGL.tar.gz https://github.com/LukasBanana/LLGL/archive/refs/tags/Release-v0.03b.tar.gz
