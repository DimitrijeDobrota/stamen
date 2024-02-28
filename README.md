# stamen

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


## Getting Started

### Dependencies

* CMake 3.25.2 or latter
* Compiler with C++20 support


### Installing

* Clone the repo
* Make a build folder and cd into it
* Run `cmake -DCMAKE_BUILD_TYPE=Release <path to cloned repo>`
* Run `make`
* If desired run `make install` in order to install the library and generator program


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

There are a few things needed before you begin.

* All types and functions with prefix stamen_ are also available in namespace
stamen:: in C++ for easier use.
* Panel and item codes must be one word. In addition they must be valid C/C++
function names if static menu is to be build correctly.
* Each free function must have `int name(int)` signature as prescribed by
`stamen_callback_f`. Passed int doesn't need to serve any specific purpose,
it is intended to detonate the position of an item in the previous panel
which is essential for dynamic menu implementation.
* You must set a value of the static variable `const stamen_display_f
stamen_display` to specify function used for displaying the menu. You can start
by using a build in `stamen_builtin_display`, just make sure to link stamen
library while building your project.


#### Static menu

After writing a configuration file, run `./bin/generate <config file>` which
will create source file and include file in the current directory with the name
as the configuration file but with extensions `.cpp` and `.hpp` respectively.
You can create  files with extensions `.c` and `.h` by appending C to previous
command

Include file will contain declarations for all of the menu functions. You
should include this file in your code.

Source file contains definitions for the menu functions. It also includes
`shared.h` file which should contain declarations for all of the free functions
you have specified in the configuration.

Generated source file should be compiled with the rest of your code.

You can call any function to display the menu starting from that specific pane.


#### Custom display function

Please refer to the implementation of `stamen_builtin_display` to get a general
idea of the direction.

A display function should have `int name(const char*, const item_t[], int)`
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

const stamen_display_f stamen_display = stamen_builtin_display;

// read the configuration
stamen_read("path to config");

// register free functions
stamen_insert("free function code", some_free_function);
...

// start the menu on specific panel
stamen_dynamic("panel code");
```

For C++ there is a namespaced version of the functions:
```
#include <stamen.h>

const stamen::display_f stamen_display = stamen::builtin_display;

// read the configuration
stamen::read("path to config");

// register free functions
stamen::insert("free function code", some_free_function);
...

// start the menu on specific panel
stamen::dynamic("panel code");
```

`

## Version History

* 1.0
    * Initial Release


## License

This project is licensed under the MIT License - see the LICENSE.md file for details

