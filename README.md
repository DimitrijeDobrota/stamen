# Stamen

Static menu generator written in C++20


## Description

This project allows for a creation of static menus, to be used in C or C++, on
the fly. The stamen library acts as a framework for linking and keeping track
of the menus, whilst allowing a custom display function to be used for maximum
flexibility. In dynamic mode configuration file can be read on the fly without
recompilation.

The main goal of this project is experimentation. I have been toying with menus
for a while and I've noticed a lot of boilerplate code emerging.  That's why I
decided to automate the task by generating menus on the fly.

The main advantage of stamen is two modes it can operate in:
1) Static mode, where the menus are pregenerated and linked together with zero cost.
2) Dynamic mode, where menus are generated on the fly (possibly in runtime) without recompilation.


## Getting Started

### Dependencies

* CMake 3.25.2 or latter
* Compiler with C++20 support
* [Poafloc Library](https://github.com/DimitrijeDobrota/poafloc)


### Installing

* Clone the repo
* Make a build folder and cd into it
* Run `cmake <path to cloned repo>` to set up the build scripts
* Run `make` to build the project
* Run `cmake --build . --target install` to install the project


### Configuration

In order to generate the desired menu, a configuration file needs to be
written. Here is an example:
```
+ menu_main Main Menu
- submenu_1 Enter Submenu 1
- submenu_2 Enter Submenu 2
- finish Quit

+ submenu_1 Submenu 1
- operation1 Operation 1
- operation2 Operation 2

+ submenu_2 Submenu 2
- operation1 Operation 1
- operation2 Operation 2
```

Configuration file consists of 2 types of entities: panels and items. Empty
lines are ignored.

Panel is detonated by `+` sign at the start of the line and consists of two
parts: code(one word) and title(rest of the line).

Item entity is detonated by `-` sign at the start of the line and consists of
two parts: code(one word) and prompt(rest of the line).

Panel entity creates a new panel, and each subsequent menu item is added as an
option to the panel, until new panel is created.

Panel code is an unique reference to the panel, whilst item code can be a
reference to another panel or any other function (from now on referred to as
`free function`).


### Usage

> Please reference demo folder for relevant usage example.

> Refer to demo/CMakeLists.txt to see how to integrate stamen into build system

There are a few things needed before you begin.

* All types and functions with prefixes `stamen_` and `stamen_menu_` are also
available in namespaces `stamen::` and `stamen::menu::` in C++ for easier use.
* Panel and item codes must be one word. In addition they must be valid C/C++
function names if static menu is to be build correctly.
* Each free function must have `int name(int);` signature as prescribed by
`stamen_callback_f`. Passed int it is intended to detonate the position of an
item in the previous panel which is essential for dynamic menu implementation,
but it is not required to be used like that.


#### Static menu

> Please refer to `stamen-generate --help` for list of all options

After writing a configuration file, run `stamen-generate <config file>` which
will create source file and include file in the current directory with the name
as the configuration file but with extensions `.cpp` and `.hpp` respectively.
You can create  files with extensions `.c` and `.h` by appending adding `--c`
flag to the command line arguments.

Include file will contain declarations for all of the menu functions. You
should include this file in your code.

Source file contains definitions for the menu functions. It also includes
`shared.h` file which should contain declarations for all of the free functions
you have specified in the configuration. The name of the file with free functions
can be changed with `--header NAME` flag;

Custom display function to be used can be set with `-d FUNC` flag. Specified
function will be forward-declared according to `stamen_display_f`, so you
don't have to worry about doing it yourself.

Generated source file should be compiled with the rest of your code. If 
`stamen_builtin_display` is not used, there is no need to link with the stamen library.

You can call any function to display the menu starting from that specific pane.


#### Custom display function

Please refer to the implementation of `stamen_builtin_display` to get a general
idea of the direction.

A display function should have `int name(const char*, const stamen_item_t[], int)`
signature as prescribed by `stamen_display_f`.

After prompting user to select one of the items all you have to do is call a
function pointed to by the callback field on selected item to invoke the next
panel or free function. The return type of int is intended to be used as a
measure how many panels back should be backtracked after a free function
terminates, but you can use in any way you see fit.


#### Dynamic menu

In dynamic mode, configuration file is read every time the program is run. In
order to invoke the menu you need to add the following snippet to your C
program:

```
#include <stamen.h>

// read the configuration
stamen_menu_read("path to config");

// register free functions
stamen_menu_insert("free function code", some_free_function);
...

// start the menu on specific panel
stamen_menu_dynamic("panel code", display_function);
```

For C++ there is a namespaced version of the functions:
```
#include <stamen.h>

// read the configuration
stamen::menu::read("path to config");

// register free functions
stamen::menu::insert("free function code", some_free_function);
...

// start the menu on specific panel
stamen::menu::dynamic("panel code", display_function);
```

For the dynamic mode the work, you program needs to be linked with the stamen
library with C++ compiler as the last step, regardless whether the program was
written in C or C++.


## Version History

- 1.1
    * Separate C and C++ interfaces
    * Separate dynamic mode into menu namespace
    * Improve functionality of code generation
    * Get rid of unnecessary moving parts (stamen_display, ...)
    * Clean up implementation

- 1.0
    * Initial Release


## License

This project is licensed under the MIT License - see the LICENSE.md file for details

