# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/immix.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/immix.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/immix.dir/flags.make

CMakeFiles/immix.dir/immix_allocator.c.o: CMakeFiles/immix.dir/flags.make
CMakeFiles/immix.dir/immix_allocator.c.o: ../immix_allocator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/immix.dir/immix_allocator.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/immix.dir/immix_allocator.c.o   -c /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/immix_allocator.c

CMakeFiles/immix.dir/immix_allocator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/immix.dir/immix_allocator.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/immix_allocator.c > CMakeFiles/immix.dir/immix_allocator.c.i

CMakeFiles/immix.dir/immix_allocator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/immix.dir/immix_allocator.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/immix_allocator.c -o CMakeFiles/immix.dir/immix_allocator.c.s

CMakeFiles/immix.dir/immix_allocator.c.o.requires:

.PHONY : CMakeFiles/immix.dir/immix_allocator.c.o.requires

CMakeFiles/immix.dir/immix_allocator.c.o.provides: CMakeFiles/immix.dir/immix_allocator.c.o.requires
	$(MAKE) -f CMakeFiles/immix.dir/build.make CMakeFiles/immix.dir/immix_allocator.c.o.provides.build
.PHONY : CMakeFiles/immix.dir/immix_allocator.c.o.provides

CMakeFiles/immix.dir/immix_allocator.c.o.provides.build: CMakeFiles/immix.dir/immix_allocator.c.o


CMakeFiles/immix.dir/utility/address_utils.c.o: CMakeFiles/immix.dir/flags.make
CMakeFiles/immix.dir/utility/address_utils.c.o: ../utility/address_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/immix.dir/utility/address_utils.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/immix.dir/utility/address_utils.c.o   -c /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/address_utils.c

CMakeFiles/immix.dir/utility/address_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/immix.dir/utility/address_utils.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/address_utils.c > CMakeFiles/immix.dir/utility/address_utils.c.i

CMakeFiles/immix.dir/utility/address_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/immix.dir/utility/address_utils.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/address_utils.c -o CMakeFiles/immix.dir/utility/address_utils.c.s

CMakeFiles/immix.dir/utility/address_utils.c.o.requires:

.PHONY : CMakeFiles/immix.dir/utility/address_utils.c.o.requires

CMakeFiles/immix.dir/utility/address_utils.c.o.provides: CMakeFiles/immix.dir/utility/address_utils.c.o.requires
	$(MAKE) -f CMakeFiles/immix.dir/build.make CMakeFiles/immix.dir/utility/address_utils.c.o.provides.build
.PHONY : CMakeFiles/immix.dir/utility/address_utils.c.o.provides

CMakeFiles/immix.dir/utility/address_utils.c.o.provides.build: CMakeFiles/immix.dir/utility/address_utils.c.o


CMakeFiles/immix.dir/allocator.c.o: CMakeFiles/immix.dir/flags.make
CMakeFiles/immix.dir/allocator.c.o: ../allocator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/immix.dir/allocator.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/immix.dir/allocator.c.o   -c /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/allocator.c

CMakeFiles/immix.dir/allocator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/immix.dir/allocator.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/allocator.c > CMakeFiles/immix.dir/allocator.c.i

CMakeFiles/immix.dir/allocator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/immix.dir/allocator.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/allocator.c -o CMakeFiles/immix.dir/allocator.c.s

CMakeFiles/immix.dir/allocator.c.o.requires:

.PHONY : CMakeFiles/immix.dir/allocator.c.o.requires

CMakeFiles/immix.dir/allocator.c.o.provides: CMakeFiles/immix.dir/allocator.c.o.requires
	$(MAKE) -f CMakeFiles/immix.dir/build.make CMakeFiles/immix.dir/allocator.c.o.provides.build
.PHONY : CMakeFiles/immix.dir/allocator.c.o.provides

CMakeFiles/immix.dir/allocator.c.o.provides.build: CMakeFiles/immix.dir/allocator.c.o


CMakeFiles/immix.dir/utility/block.c.o: CMakeFiles/immix.dir/flags.make
CMakeFiles/immix.dir/utility/block.c.o: ../utility/block.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/immix.dir/utility/block.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/immix.dir/utility/block.c.o   -c /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/block.c

CMakeFiles/immix.dir/utility/block.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/immix.dir/utility/block.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/block.c > CMakeFiles/immix.dir/utility/block.c.i

CMakeFiles/immix.dir/utility/block.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/immix.dir/utility/block.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/block.c -o CMakeFiles/immix.dir/utility/block.c.s

CMakeFiles/immix.dir/utility/block.c.o.requires:

.PHONY : CMakeFiles/immix.dir/utility/block.c.o.requires

CMakeFiles/immix.dir/utility/block.c.o.provides: CMakeFiles/immix.dir/utility/block.c.o.requires
	$(MAKE) -f CMakeFiles/immix.dir/build.make CMakeFiles/immix.dir/utility/block.c.o.provides.build
.PHONY : CMakeFiles/immix.dir/utility/block.c.o.provides

CMakeFiles/immix.dir/utility/block.c.o.provides.build: CMakeFiles/immix.dir/utility/block.c.o


CMakeFiles/immix.dir/Immix_space.c.o: CMakeFiles/immix.dir/flags.make
CMakeFiles/immix.dir/Immix_space.c.o: ../Immix_space.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/immix.dir/Immix_space.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/immix.dir/Immix_space.c.o   -c /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/Immix_space.c

CMakeFiles/immix.dir/Immix_space.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/immix.dir/Immix_space.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/Immix_space.c > CMakeFiles/immix.dir/Immix_space.c.i

CMakeFiles/immix.dir/Immix_space.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/immix.dir/Immix_space.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/Immix_space.c -o CMakeFiles/immix.dir/Immix_space.c.s

CMakeFiles/immix.dir/Immix_space.c.o.requires:

.PHONY : CMakeFiles/immix.dir/Immix_space.c.o.requires

CMakeFiles/immix.dir/Immix_space.c.o.provides: CMakeFiles/immix.dir/Immix_space.c.o.requires
	$(MAKE) -f CMakeFiles/immix.dir/build.make CMakeFiles/immix.dir/Immix_space.c.o.provides.build
.PHONY : CMakeFiles/immix.dir/Immix_space.c.o.provides

CMakeFiles/immix.dir/Immix_space.c.o.provides.build: CMakeFiles/immix.dir/Immix_space.c.o


CMakeFiles/immix.dir/utility/line.c.o: CMakeFiles/immix.dir/flags.make
CMakeFiles/immix.dir/utility/line.c.o: ../utility/line.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/immix.dir/utility/line.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/immix.dir/utility/line.c.o   -c /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/line.c

CMakeFiles/immix.dir/utility/line.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/immix.dir/utility/line.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/line.c > CMakeFiles/immix.dir/utility/line.c.i

CMakeFiles/immix.dir/utility/line.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/immix.dir/utility/line.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/line.c -o CMakeFiles/immix.dir/utility/line.c.s

CMakeFiles/immix.dir/utility/line.c.o.requires:

.PHONY : CMakeFiles/immix.dir/utility/line.c.o.requires

CMakeFiles/immix.dir/utility/line.c.o.provides: CMakeFiles/immix.dir/utility/line.c.o.requires
	$(MAKE) -f CMakeFiles/immix.dir/build.make CMakeFiles/immix.dir/utility/line.c.o.provides.build
.PHONY : CMakeFiles/immix.dir/utility/line.c.o.provides

CMakeFiles/immix.dir/utility/line.c.o.provides.build: CMakeFiles/immix.dir/utility/line.c.o


CMakeFiles/immix.dir/utility/chunk.c.o: CMakeFiles/immix.dir/flags.make
CMakeFiles/immix.dir/utility/chunk.c.o: ../utility/chunk.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/immix.dir/utility/chunk.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/immix.dir/utility/chunk.c.o   -c /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/chunk.c

CMakeFiles/immix.dir/utility/chunk.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/immix.dir/utility/chunk.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/chunk.c > CMakeFiles/immix.dir/utility/chunk.c.i

CMakeFiles/immix.dir/utility/chunk.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/immix.dir/utility/chunk.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/utility/chunk.c -o CMakeFiles/immix.dir/utility/chunk.c.s

CMakeFiles/immix.dir/utility/chunk.c.o.requires:

.PHONY : CMakeFiles/immix.dir/utility/chunk.c.o.requires

CMakeFiles/immix.dir/utility/chunk.c.o.provides: CMakeFiles/immix.dir/utility/chunk.c.o.requires
	$(MAKE) -f CMakeFiles/immix.dir/build.make CMakeFiles/immix.dir/utility/chunk.c.o.provides.build
.PHONY : CMakeFiles/immix.dir/utility/chunk.c.o.provides

CMakeFiles/immix.dir/utility/chunk.c.o.provides.build: CMakeFiles/immix.dir/utility/chunk.c.o


# Object files for target immix
immix_OBJECTS = \
"CMakeFiles/immix.dir/immix_allocator.c.o" \
"CMakeFiles/immix.dir/utility/address_utils.c.o" \
"CMakeFiles/immix.dir/allocator.c.o" \
"CMakeFiles/immix.dir/utility/block.c.o" \
"CMakeFiles/immix.dir/Immix_space.c.o" \
"CMakeFiles/immix.dir/utility/line.c.o" \
"CMakeFiles/immix.dir/utility/chunk.c.o"

# External object files for target immix
immix_EXTERNAL_OBJECTS =

immix: CMakeFiles/immix.dir/immix_allocator.c.o
immix: CMakeFiles/immix.dir/utility/address_utils.c.o
immix: CMakeFiles/immix.dir/allocator.c.o
immix: CMakeFiles/immix.dir/utility/block.c.o
immix: CMakeFiles/immix.dir/Immix_space.c.o
immix: CMakeFiles/immix.dir/utility/line.c.o
immix: CMakeFiles/immix.dir/utility/chunk.c.o
immix: CMakeFiles/immix.dir/build.make
immix: CMakeFiles/immix.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable immix"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/immix.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/immix.dir/build: immix

.PHONY : CMakeFiles/immix.dir/build

CMakeFiles/immix.dir/requires: CMakeFiles/immix.dir/immix_allocator.c.o.requires
CMakeFiles/immix.dir/requires: CMakeFiles/immix.dir/utility/address_utils.c.o.requires
CMakeFiles/immix.dir/requires: CMakeFiles/immix.dir/allocator.c.o.requires
CMakeFiles/immix.dir/requires: CMakeFiles/immix.dir/utility/block.c.o.requires
CMakeFiles/immix.dir/requires: CMakeFiles/immix.dir/Immix_space.c.o.requires
CMakeFiles/immix.dir/requires: CMakeFiles/immix.dir/utility/line.c.o.requires
CMakeFiles/immix.dir/requires: CMakeFiles/immix.dir/utility/chunk.c.o.requires

.PHONY : CMakeFiles/immix.dir/requires

CMakeFiles/immix.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/immix.dir/cmake_clean.cmake
.PHONY : CMakeFiles/immix.dir/clean

CMakeFiles/immix.dir/depend:
	cd /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug /Users/lukaskellenberger/Developer/scala-native/nativelib/src/main/resources/gc/immix/cmake-build-debug/CMakeFiles/immix.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/immix.dir/depend

