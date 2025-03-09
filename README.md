# stamen

Static menu generator written in C++20

## Description

This project is a code generation tool that takes care of the menus.  The
stamen acts as a framework for linking and keeping track of the menus, whilst
allowing a custom display function to be used for maximum flexibility.  In
dynamic mode configuration file can be read on the fly without recompilation.

The main goal of this project is experimentation. I have been toying with menus
for a while and I've noticed a lot of boilerplate code emerging.  That's why I
decided to automate the task by generating menus on the fly.

The main advantage of stamen is two modes it can operate in:
1) Static mode, where the menus are pregenerated and linked together with zero cost.
2) Dynamic mode, where menus are generated on the fly (possibly in runtime) without recompilation.


## Dependencies

* CMake 3.14 or latter
* Compiler with C++20 support (tested: clang 19.1.7, gcc 14.2.1)


## Building and installing

See the [`BUILDING`](BUILDING.md) document.


## Usage

> Please reference example folder for relevant usage examples.

> Refer to example/CMakeLists.txt to see how to integrate stamen into build system

There are a few things needed before you begin.

* Panel and item codes must be one word. In addition they must be valid C++
function names if static menu is to be build correctly.
* Each free function must have `int name(std::size_t);` signature as prescribed by
`stamen::callback_f`. Passed value is intended to detonate the position of an
item in the previous panel which is essential for dynamic menu implementation,
but it is not required to be used like that.

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


### Static menu

> Please refer to `stamen --help` for list of all options

After writing a configuration file, run `stamen <config file>` which
will create source file and include file in the current directory with the name
as the configuration file but with extensions `.cpp` and `.hpp` respectively.

Include file will contain declarations for all of the menu functions, as well
as the class to be used in this specific menu instance. You should include this
file in your code.

Generated source file should be compiled with the rest of your code, as it
contains definitions for the menu functions.

User should give a definition of `menu_t::visit(const menu_t& menu)`, that
actually renders the menu end prompts the user for action.

You can call any function to display the menu starting from that specific panel.


### Custom display function

Please refer to the implementation in `example/static.cpp` to get a general
idea of the direction.

After prompting user to select one of the items all you have to do is call a
function pointed to by the callback field on selected item to invoke the next
panel or free function. The return type of int is intended to be used as a
measure how many panels back should be backtracked after a free function
terminates, but you can use in any way you see fit.


### Dynamic menu

In dynamic mode, configuration file is read every time the program is run. In
order to invoke the menu you need to add the following snippet:

```
#include "stamen/stamen.hpp"

// read the configuration
stamen::Stamen inst(ifs);

// register free functions
inst.insert("free function code", some_free_function);
...

// start the menu on specific panel
inst.dynamic("menu_main", display);
...
```

For the dynamic mode the work, you program needs to be linked with the stamen
library with C++ compiler as the last step.


## Version History

- 1.3
    * Drop C support (it was fun while it lasted)
    * Use [`Cemplate`](https://github.com/DimitrijeDobrota/cemplate) for code generation
    * No need to include stamen headers as everything is self contained
    * Cleanup the code with better implementation

- 1.2
    * Modernize CMake project
    * Modernize codebase

- 1.1
    * Separate C and C++ interfaces
    * Separate dynamic mode into menu namespace
    * Improve functionality of code generation
    * Get rid of unnecessary moving parts (stamen_display, ...)
    * Clean up implementation

- 1.0
    * Initial Release


## Contributing

See the [`CONTRIBUTING`](CONTRIBUTING.md) document.


## License

This project is licensed under the MIT License - see the [`LICENSE`](LICENSE.md) file for details


