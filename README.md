# CBuild
A simplistic build system inspired by Makefile for compiled languages that include other files similarly to c/c++.

----------'cbuild' (command)----------
Ensure you have a file named 'cbuild.make' in the directory the command is run in.

----------'cbuild.make' (file)----------
'source_dir': The directory containing the source code files.

'include_dir': Any headers or similar files the source code files depend on to function (only ones that would change during the development process/not standard library ones).

'object_dir': The directory containing the compiler outputs as object files before linking.

'object_main': The object file that comes first during the linking process (important for if you are compiling the program to a flat binary and want the entry point to be fixed, not important for anything else).

'output_binary': The file to link all the objects to.

'source_ext': The file extension for the source code files in 'source_dir'.

'object_ext': The file extension for the object files in the directory of 'object_dir'.

'include_line_lib': A line of code specifieng an example line for if the program is including anything from the 'include_dir' directory or any other directories specified by the operating system (anything in '<>' or '""' are seen as the file's name relative to the 'include_dir' directory).

'include_line_local': A line of code specifieng an example line for if the program is including a header file or otherwise into the source code file (anything in '<>' or '""' will be seen as the file's path relative to the 'source_dir' directory).

'compiler_command': A line of shell code that specifies the command to run on all source files to be compiled into object files ('{output}' is placed in place of the object file and '{input}' is placed in place of the source code file).

'linker_command': A line of shell code that specifies the command to run at the end if changes have been made to link all of the object files into the output binary ('{output}' is in place of the output binary and '{input}' is placed in place of all of the object files inside of the object files' directory).

----------(info)----------
cbuild Only compiles a source code file into an object file if either the source code file or any of the files it includes as headers/library files inside of the library directory 'include_dir' directory have changed since the building of that object file.

cbuild Only links the objects together into an output binary if any of the objects had to be recompiled.
