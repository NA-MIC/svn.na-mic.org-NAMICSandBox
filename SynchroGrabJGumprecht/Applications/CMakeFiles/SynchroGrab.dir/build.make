# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /home/jgumprec/mrrobot/eclipse/workspace/SlicerTrunk/Slicer3-lib/CMake-build/bin/cmake

# The command to remove a file.
RM = /home/jgumprec/mrrobot/eclipse/workspace/SlicerTrunk/Slicer3-lib/CMake-build/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /projects/mrrobot/gumprecht/eclipse/workspace/SlicerTrunk/Slicer3-lib/CMake-build/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab

# Include any dependencies generated for this target.
include Applications/CMakeFiles/SynchroGrab.dir/depend.make

# Include the progress variables for this target.
include Applications/CMakeFiles/SynchroGrab.dir/progress.make

# Include the compile flags for this target's objects.
include Applications/CMakeFiles/SynchroGrab.dir/flags.make

Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o: Applications/CMakeFiles/SynchroGrab.dir/flags.make
Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o: Applications/SynchroGrab.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o"
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && /usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SynchroGrab.dir/SynchroGrab.o -c /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications/SynchroGrab.cxx

Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SynchroGrab.dir/SynchroGrab.i"
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications/SynchroGrab.cxx > CMakeFiles/SynchroGrab.dir/SynchroGrab.i

Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SynchroGrab.dir/SynchroGrab.s"
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications/SynchroGrab.cxx -o CMakeFiles/SynchroGrab.dir/SynchroGrab.s

Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.requires:
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.requires

Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.provides: Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.requires
	$(MAKE) -f Applications/CMakeFiles/SynchroGrab.dir/build.make Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.provides.build
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.provides

Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.provides.build: Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.provides.build

Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o: Applications/CMakeFiles/SynchroGrab.dir/flags.make
Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o: Applications/vtkSynchroGrabPipeline.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o"
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && /usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o -c /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications/vtkSynchroGrabPipeline.cxx

Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.i"
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications/vtkSynchroGrabPipeline.cxx > CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.i

Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.s"
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications/vtkSynchroGrabPipeline.cxx -o CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.s

Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.requires:
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.requires

Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.provides: Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.requires
	$(MAKE) -f Applications/CMakeFiles/SynchroGrab.dir/build.make Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.provides.build
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.provides

Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.provides.build: Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.provides.build

# Object files for target SynchroGrab
SynchroGrab_OBJECTS = \
"CMakeFiles/SynchroGrab.dir/SynchroGrab.o" \
"CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o"

# External object files for target SynchroGrab
SynchroGrab_EXTERNAL_OBJECTS =

bin/SynchroGrab: Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o
bin/SynchroGrab: Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o
bin/SynchroGrab: bin/libOpenIGTLink.a
bin/SynchroGrab: bin/libndicapi.a
bin/SynchroGrab: bin/libigtlutil.a
bin/SynchroGrab: Applications/CMakeFiles/SynchroGrab.dir/build.make
bin/SynchroGrab: Applications/CMakeFiles/SynchroGrab.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/SynchroGrab"
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SynchroGrab.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Applications/CMakeFiles/SynchroGrab.dir/build: bin/SynchroGrab
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/build

Applications/CMakeFiles/SynchroGrab.dir/requires: Applications/CMakeFiles/SynchroGrab.dir/SynchroGrab.o.requires
Applications/CMakeFiles/SynchroGrab.dir/requires: Applications/CMakeFiles/SynchroGrab.dir/vtkSynchroGrabPipeline.o.requires
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/requires

Applications/CMakeFiles/SynchroGrab.dir/clean:
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications && $(CMAKE_COMMAND) -P CMakeFiles/SynchroGrab.dir/cmake_clean.cmake
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/clean

Applications/CMakeFiles/SynchroGrab.dir/depend:
	cd /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications /home/jgumprec/mrrobot/eclipse/workspace/SynchroGrab/Applications/CMakeFiles/SynchroGrab.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Applications/CMakeFiles/SynchroGrab.dir/depend

