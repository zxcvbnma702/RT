# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Library/Projects/Vulkan/RT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Library/Projects/Vulkan/RT/build

# Include any dependencies generated for this target.
include CMakeFiles/RT.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RT.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RT.dir/flags.make

CMakeFiles/RT.dir/main.cpp.o: CMakeFiles/RT.dir/flags.make
CMakeFiles/RT.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Library/Projects/Vulkan/RT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RT.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RT.dir/main.cpp.o -c /Library/Projects/Vulkan/RT/main.cpp

CMakeFiles/RT.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RT.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Library/Projects/Vulkan/RT/main.cpp > CMakeFiles/RT.dir/main.cpp.i

CMakeFiles/RT.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RT.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Library/Projects/Vulkan/RT/main.cpp -o CMakeFiles/RT.dir/main.cpp.s

# Object files for target RT
RT_OBJECTS = \
"CMakeFiles/RT.dir/main.cpp.o"

# External object files for target RT
RT_EXTERNAL_OBJECTS =

RT: CMakeFiles/RT.dir/main.cpp.o
RT: CMakeFiles/RT.dir/build.make
RT: CMakeFiles/RT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Library/Projects/Vulkan/RT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable RT"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RT.dir/build: RT

.PHONY : CMakeFiles/RT.dir/build

CMakeFiles/RT.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RT.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RT.dir/clean

CMakeFiles/RT.dir/depend:
	cd /Library/Projects/Vulkan/RT/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Library/Projects/Vulkan/RT /Library/Projects/Vulkan/RT /Library/Projects/Vulkan/RT/build /Library/Projects/Vulkan/RT/build /Library/Projects/Vulkan/RT/build/CMakeFiles/RT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RT.dir/depend

