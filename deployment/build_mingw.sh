#!/bin/bash

upload_server=$1

projectdir=`git rev-parse --show-toplevel`
if [ -z "$projectdir" ]; then
	echo "No .git directory found"
	exit 1
fi

outputdir="$projectdir/deployment/out"
cd "$projectdir"
mkdir -p "$outputdir"

version=`grep 'GAME_VERSION=".*"' -o CMakeLists.txt | sed -r "s/GAME_VERSION=\"(.*)\"/\1/"`

build() {

	bits=$1

	# get mingw prefix
	if [ $bits -eq "32" ]; then
		arch=i686-w64-mingw32
	else
		arch=x86_64-w64-mingw32
	fi

	# run cmake
	builddir="$projectdir/build/mingw$bits"
	mkdir -p "$builddir"
	cd "$builddir"
	cmake -DCMAKE_TOOLCHAIN_FILE=../../cmake/mingw${bits}.cmake ../../

	# build
	make -j`nproc`

	if [ $? -ne 0 ]; then
		echo "failed $builddir"
		exit
	fi

	cd "$projectdir"

	cp /usr/$arch/bin/{libbz2-1.dll,libfreetype-6.dll,libgcc_*.dll,libstdc++-6.dll,libwinpthread-1.dll,SDL2.dll,SDL2_image.dll,SDL2_ttf.dll,SDL2_mixer.dll,libvorbisfile-3.dll,libvorbis-0.dll,libogg-0.dll,libpng16-16.dll,zlib1.dll} working/

	gitver=`git log --oneline | wc -l`
	mv bin/Release/openflap.exe working/
	cp README working/
	echo "openflap.exe 123" > working/static_seed.bat
	chmod +x working/*.bat

	final_name=openflap-${version}r${gitver}-win${bits}
	archive=$final_name.zip
	ln -s working "${final_name}"
	zip -r $archive "${final_name}"
	rm "${final_name}"

	rm working/openflap.exe
	rm working/*.dll
	rm working/README
	rm working/*.bat

	if [ -n "$upload_server" ]; then
		scp $archive $upload_server:web/files/
	fi

	mv $archive "$outputdir"
}

if [ -n "$upload_server" ]; then
	ssh $upload_server rm -f web/files/openflap*.zip
fi

rm -f "$outputdir"/openflap*.zip

build 32
build 64
