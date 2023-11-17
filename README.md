# stamen

Static menu generator written in C++20


## Description

The goal of the project is experimentation.I have been toying with flexible
menus for a while and I've noticed a lot of boilerplate code emerging. That's
why I decided to automate the task by generating menus on the fly. I aim for
maximum flexibility while maintaining minimalism.


## Getting Started

### Dependencies

* CMake 3.25.2 or latter
* Compiler with C++20 support


### Installing

* Clone the repo
* Make a build folder and cd into it
* Run `cmake -DCMAKE_BUILD_TYPE=Release <path to cloned repo>`


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

Menu entity creates a new panel, and each subsequent menu item is added as an
option to the panel, until new panel is created.

Panel code is an unique reference to the panel, whilst item code can be a
reference to another panel or any other function (from now on referred to as
`free function`).


### Usage

Please reference demo folder for relevant usage example.

There are a few things needed before you begin.

* Everything is contained in the `stamen namespace`
* Panel and item codes must be one word. In addition they must be valid C++
function names if static menu is to be build correctly.
* Each free function must have `int name(void)` signature as prescribed by
`Menu::callback_f`.
* You must set a value of the static variable `const Menu::display_f
Menu::display` to specify function used for displaying the menu. You can start
by using a build in `stamen::builtinDisplay`, just make sure to link stamen library
while building your project.


#### Dynamic menu

In dynamic mode, configuration file is read every time the program is run. In
order to invoke the menu you need to add the following code to your C++
program:

```
// shorthand
using stamen::Menu;

// read the configuration
Menu::read("path to config");

// register free functions
Menu::insert("reference name", some_function);
...

// start the menu on specific panel
Menu::start("panel code");

// print tree-like menu structure
// starting from a specific panel
// for debugging purposes
Menu::print("panel code");
```

#### Static menu

After writing a configuration file, run `./bin/generate <config file>` which
will create source file and include file in the current directory with the name
as the configuration file but with extensions `.cpp` and `.h` respectively.

Include file will contain declarations for all of the menu functions inside
`stamen` namespace. You should include this file in your code.

Source file contains definitions for the menu functions. It also includes
`shared.h` file which should contain declarations for all of the free functions
you have specified in the configuration. Generated source file should be
compiled with the rest of your code. You can call any function to display the
menu starting from that specific pane.


#### Custom display function

Please refer to the implementation of `stamen::builtinDisplay` to get a general
idea of the direction.

A display function should have `int name(const std::string&, const
Menu::item_t[], std::size_t)` signature as prescribed by `Menu::display_f`. To
get information about the specific item use `getPrompt()` and `getCallback()`
member functions.

After prompting user to select one of the items all you have to do is call
`operator()` on selected item to invoke the next panel or free function. The
return type of int is intended to be used as a measure how many panels back
should be backtracked after a free function terminates, but you can use in any
way you see fit.


## Version History

* 1.0
    * Initial Release


## License

This project is licensed under the MIT License - see the LICENSE.md file for details
