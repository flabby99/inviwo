#!/bin/bash

if ["$1" == ""]
then
	echo "Please enter the path to the relevant qt cmake folder"
else
	export CMAKE_PREFIX_PATH="$1"
	echo $CMAKE_PREFIX_PATH
	mkdir -p inviwo_warping_build
	cd inviwo_warping_build
	# Can use glfw tiny also
	cmake -G "Unix Makefiles" -D IVW_TINY_GLFW_APPLICATION=OFF -D IVW_TINY_QT_APPLICATION=ON ../inviwo
	#8 indicates the number of cores to build with
	make -j4
	echo "Starting Inviwo"
	./bin/inviwo
fi
