# CBuild
A simplistic build system inspired by Makefile for compiled languages that include other files from include directories and from local directories.

Read more at https://zufalige-daten.github.io/CBuild.html

command 'cbuild':
	Ensure file 'cbuild.make' is in the directory.

'':
	In a source file with isolated single quotes within those quotes the project name is specified.
[attribute] = '':
	Sets [attribute] to the contents of the following single quotes (eg "object_dir = 'obj'")

attribute(s):
	project_root:
		The root directory for the project (./ for the current directory);
	binary_out:
		The output binary file.
	source_dir:
		The source directory.
	object_dir:
		The directory containing the emitted object files.
	include_dir:
		The directory containing the include (header files) that are accessed by the source files.
	object_ext:
		The file extension for the object files emmited by the compilers during the build process (do not precede with '.' only the text of the file extension eg "object_ext = 'o'").
	object_main:
		The object file selected to be placed first in the linking process.
	linker_command:
		Command to be executed when linking (place '{input}' in place of an input file would be specified and also place '{output}' in place of when an output file would be specified).
	{}:
		Within these a new 'profile' is created that cbuild accesses as a different source file type.

profile specific attributes:
	include_type:
		A list (seperated by any character) containing any number of the three keywords: 'lib' 'local' and 'root' which tell cbuild the types of include file/header file including are supported by this profile's language (as different programming languages have different methods of including files).
	include_lib:
		(Only valid when 'lib' is specified in include_types) specifies the format taken by the include string in the programming language in the profile (the code for including a header file from the local include directory except replacing '%' with '%%' and the area where the header file name would go with '%s' eg '%%include "%s"' is seen as '%include "include file.h"')
	include_local:
		(Only valid when 'local' is specified in include_types) above except for local relative includes.
	include_root:
		(Only valid when 'root' is specified in include_types) above except for root relative includes.
	source_ext:
		The file extension for the source files for the programming language covered by this profile (do not precede with '.' and only must contain the text of the extension eg "source_ext = 'asm'").
	compiler_command:
		The command for the compilation of the active profile's loaded source files (replace the input file in the command with '{input}', the output file (must be an object file so adjust your compiler arguments accordingly) with '{output}', and the position in which the include directory is usually specified in programming languages supporting 'lib' would be replaced with '{include}' in the config file).

include types:
	'lib':
		Specifies that the language specified in the profile supports including from include files (header files) relative to the project's local include directory (specified in include_dir).
	'local':
		Specifies that the language specified in the profile supports including from include files (header files) relative to the source directory (specified in source_dir).
	'root':
		Specified that the language specified in the profile supports including from include files (header files) relative to the root of the project (specified in project_root).

Note: all attributes and structures specified above can be placed in any order seen fit by the configurator.

----------(info)----------
cbuild Only compiles a source code file into an object file if either the source code file or any of the files it includes as headers/library files inside of the library directory 'include_dir' directory have changed since the building of that object file.

cbuild Only links the objects together into an output binary if any of the objects had to be recompiled.
