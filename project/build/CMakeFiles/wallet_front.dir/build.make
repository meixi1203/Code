# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/e/wallet_front/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/wallet_front/project/build

# Include any dependencies generated for this target.
include CMakeFiles/wallet_front.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wallet_front.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wallet_front.dir/flags.make

CMakeFiles/wallet_front.dir/main.cpp.o: CMakeFiles/wallet_front.dir/flags.make
CMakeFiles/wallet_front.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/wallet_front/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wallet_front.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wallet_front.dir/main.cpp.o -c /mnt/e/wallet_front/project/main.cpp

CMakeFiles/wallet_front.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wallet_front.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/wallet_front/project/main.cpp > CMakeFiles/wallet_front.dir/main.cpp.i

CMakeFiles/wallet_front.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wallet_front.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/wallet_front/project/main.cpp -o CMakeFiles/wallet_front.dir/main.cpp.s

CMakeFiles/wallet_front.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/wallet_front.dir/main.cpp.o.requires

CMakeFiles/wallet_front.dir/main.cpp.o.provides: CMakeFiles/wallet_front.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/wallet_front.dir/build.make CMakeFiles/wallet_front.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/wallet_front.dir/main.cpp.o.provides

CMakeFiles/wallet_front.dir/main.cpp.o.provides.build: CMakeFiles/wallet_front.dir/main.cpp.o


CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o: CMakeFiles/wallet_front.dir/flags.make
CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o: ../communication/front_engine.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/wallet_front/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o -c /mnt/e/wallet_front/project/communication/front_engine.pb.cc

CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/wallet_front/project/communication/front_engine.pb.cc > CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.i

CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/wallet_front/project/communication/front_engine.pb.cc -o CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.s

CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.requires:

.PHONY : CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.requires

CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.provides: CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.requires
	$(MAKE) -f CMakeFiles/wallet_front.dir/build.make CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.provides.build
.PHONY : CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.provides

CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.provides.build: CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o


# Object files for target wallet_front
wallet_front_OBJECTS = \
"CMakeFiles/wallet_front.dir/main.cpp.o" \
"CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o"

# External object files for target wallet_front
wallet_front_EXTERNAL_OBJECTS =

bin/wallet_front: CMakeFiles/wallet_front.dir/main.cpp.o
bin/wallet_front: CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o
bin/wallet_front: CMakeFiles/wallet_front.dir/build.make
bin/wallet_front: lib/liblog.a
bin/wallet_front: lib/libsocket.a
bin/wallet_front: lib/libcommunication.a
bin/wallet_front: CMakeFiles/wallet_front.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/wallet_front/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/wallet_front"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wallet_front.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wallet_front.dir/build: bin/wallet_front

.PHONY : CMakeFiles/wallet_front.dir/build

CMakeFiles/wallet_front.dir/requires: CMakeFiles/wallet_front.dir/main.cpp.o.requires
CMakeFiles/wallet_front.dir/requires: CMakeFiles/wallet_front.dir/communication/front_engine.pb.cc.o.requires

.PHONY : CMakeFiles/wallet_front.dir/requires

CMakeFiles/wallet_front.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wallet_front.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wallet_front.dir/clean

CMakeFiles/wallet_front.dir/depend:
	cd /mnt/e/wallet_front/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/wallet_front/project /mnt/e/wallet_front/project /mnt/e/wallet_front/project/build /mnt/e/wallet_front/project/build /mnt/e/wallet_front/project/build/CMakeFiles/wallet_front.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wallet_front.dir/depend

