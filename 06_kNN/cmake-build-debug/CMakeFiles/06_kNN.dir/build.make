# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Workspace\C++\ArtificialInteligence2018\06_kNN

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Workspace\C++\ArtificialInteligence2018\06_kNN\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/06_kNN.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/06_kNN.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/06_kNN.dir/flags.make

CMakeFiles/06_kNN.dir/kNN.cpp.obj: CMakeFiles/06_kNN.dir/flags.make
CMakeFiles/06_kNN.dir/kNN.cpp.obj: ../kNN.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Workspace\C++\ArtificialInteligence2018\06_kNN\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/06_kNN.dir/kNN.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\06_kNN.dir\kNN.cpp.obj -c D:\Workspace\C++\ArtificialInteligence2018\06_kNN\kNN.cpp

CMakeFiles/06_kNN.dir/kNN.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/06_kNN.dir/kNN.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Workspace\C++\ArtificialInteligence2018\06_kNN\kNN.cpp > CMakeFiles\06_kNN.dir\kNN.cpp.i

CMakeFiles/06_kNN.dir/kNN.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/06_kNN.dir/kNN.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Workspace\C++\ArtificialInteligence2018\06_kNN\kNN.cpp -o CMakeFiles\06_kNN.dir\kNN.cpp.s

# Object files for target 06_kNN
06_kNN_OBJECTS = \
"CMakeFiles/06_kNN.dir/kNN.cpp.obj"

# External object files for target 06_kNN
06_kNN_EXTERNAL_OBJECTS =

06_kNN.exe: CMakeFiles/06_kNN.dir/kNN.cpp.obj
06_kNN.exe: CMakeFiles/06_kNN.dir/build.make
06_kNN.exe: CMakeFiles/06_kNN.dir/linklibs.rsp
06_kNN.exe: CMakeFiles/06_kNN.dir/objects1.rsp
06_kNN.exe: CMakeFiles/06_kNN.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Workspace\C++\ArtificialInteligence2018\06_kNN\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 06_kNN.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\06_kNN.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/06_kNN.dir/build: 06_kNN.exe

.PHONY : CMakeFiles/06_kNN.dir/build

CMakeFiles/06_kNN.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\06_kNN.dir\cmake_clean.cmake
.PHONY : CMakeFiles/06_kNN.dir/clean

CMakeFiles/06_kNN.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Workspace\C++\ArtificialInteligence2018\06_kNN D:\Workspace\C++\ArtificialInteligence2018\06_kNN D:\Workspace\C++\ArtificialInteligence2018\06_kNN\cmake-build-debug D:\Workspace\C++\ArtificialInteligence2018\06_kNN\cmake-build-debug D:\Workspace\C++\ArtificialInteligence2018\06_kNN\cmake-build-debug\CMakeFiles\06_kNN.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/06_kNN.dir/depend

