# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.p9WanbUdMo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.p9WanbUdMo/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/POS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/POS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/POS.dir/flags.make

CMakeFiles/POS.dir/main.c.o: CMakeFiles/POS.dir/flags.make
CMakeFiles/POS.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.p9WanbUdMo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/POS.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/POS.dir/main.c.o -c /tmp/tmp.p9WanbUdMo/main.c

CMakeFiles/POS.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/POS.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.p9WanbUdMo/main.c > CMakeFiles/POS.dir/main.c.i

CMakeFiles/POS.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/POS.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.p9WanbUdMo/main.c -o CMakeFiles/POS.dir/main.c.s

CMakeFiles/POS.dir/terminal_display.c.o: CMakeFiles/POS.dir/flags.make
CMakeFiles/POS.dir/terminal_display.c.o: ../terminal_display.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.p9WanbUdMo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/POS.dir/terminal_display.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/POS.dir/terminal_display.c.o -c /tmp/tmp.p9WanbUdMo/terminal_display.c

CMakeFiles/POS.dir/terminal_display.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/POS.dir/terminal_display.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.p9WanbUdMo/terminal_display.c > CMakeFiles/POS.dir/terminal_display.c.i

CMakeFiles/POS.dir/terminal_display.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/POS.dir/terminal_display.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.p9WanbUdMo/terminal_display.c -o CMakeFiles/POS.dir/terminal_display.c.s

CMakeFiles/POS.dir/client.c.o: CMakeFiles/POS.dir/flags.make
CMakeFiles/POS.dir/client.c.o: ../client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.p9WanbUdMo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/POS.dir/client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/POS.dir/client.c.o -c /tmp/tmp.p9WanbUdMo/client.c

CMakeFiles/POS.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/POS.dir/client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.p9WanbUdMo/client.c > CMakeFiles/POS.dir/client.c.i

CMakeFiles/POS.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/POS.dir/client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.p9WanbUdMo/client.c -o CMakeFiles/POS.dir/client.c.s

CMakeFiles/POS.dir/server.c.o: CMakeFiles/POS.dir/flags.make
CMakeFiles/POS.dir/server.c.o: ../server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.p9WanbUdMo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/POS.dir/server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/POS.dir/server.c.o -c /tmp/tmp.p9WanbUdMo/server.c

CMakeFiles/POS.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/POS.dir/server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.p9WanbUdMo/server.c > CMakeFiles/POS.dir/server.c.i

CMakeFiles/POS.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/POS.dir/server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.p9WanbUdMo/server.c -o CMakeFiles/POS.dir/server.c.s

CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.o: CMakeFiles/POS.dir/flags.make
CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.o: ../server/pos_sockets/active_socket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.p9WanbUdMo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.o -c /tmp/tmp.p9WanbUdMo/server/pos_sockets/active_socket.c

CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.p9WanbUdMo/server/pos_sockets/active_socket.c > CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.i

CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.p9WanbUdMo/server/pos_sockets/active_socket.c -o CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.s

# Object files for target POS
POS_OBJECTS = \
"CMakeFiles/POS.dir/main.c.o" \
"CMakeFiles/POS.dir/terminal_display.c.o" \
"CMakeFiles/POS.dir/client.c.o" \
"CMakeFiles/POS.dir/server.c.o" \
"CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.o"

# External object files for target POS
POS_EXTERNAL_OBJECTS =

POS: CMakeFiles/POS.dir/main.c.o
POS: CMakeFiles/POS.dir/terminal_display.c.o
POS: CMakeFiles/POS.dir/client.c.o
POS: CMakeFiles/POS.dir/server.c.o
POS: CMakeFiles/POS.dir/server/pos_sockets/active_socket.c.o
POS: CMakeFiles/POS.dir/build.make
POS: CMakeFiles/POS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.p9WanbUdMo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable POS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/POS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/POS.dir/build: POS

.PHONY : CMakeFiles/POS.dir/build

CMakeFiles/POS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/POS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/POS.dir/clean

CMakeFiles/POS.dir/depend:
	cd /tmp/tmp.p9WanbUdMo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.p9WanbUdMo /tmp/tmp.p9WanbUdMo /tmp/tmp.p9WanbUdMo/cmake-build-debug /tmp/tmp.p9WanbUdMo/cmake-build-debug /tmp/tmp.p9WanbUdMo/cmake-build-debug/CMakeFiles/POS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/POS.dir/depend

