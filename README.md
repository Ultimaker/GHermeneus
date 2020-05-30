# GHermeneus

Is intended as a fast GCode interpreter.

GCode is read from a stream where each command updates the state of the machine (3D printer), these states are then
funneled to an output stream, such as a numpy array. Which can later be used in statistical analysis, which allows
us to see the difference between Cura settings, versions, materials or even different slicers.

It could also be used to generate the date for the Cura preview, allowing this data to be run on multiple processors,
and freeing the GIL (Global Interpreter Lock).

## Work-In-Progess

This library is currently very much a work in progress and subject to heavy changes.

## State-Space Representation of the dynamics

A machine can have multiple tools, each tool is represented by a state vector. This vector const of the position, velocity
extrusion speed, temperature etc.

<a href="https://www.codecogs.com/eqnedit.php?latex=\vec{x}_k&space;=&space;[x,&space;y,&space;z,&space;\dot{x},&space;\dot{y},&space;\dot{z},&space;e,&space;\dot{e},&space;T_t,&space;T_b,&space;t]" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\vec{x}_k&space;=&space;[x,&space;y,&space;z,&space;\dot{x},&space;\dot{y},&space;\dot{z},&space;e,&space;\dot{e},&space;T_t,&space;T_b,&space;t]" title="\vec{x}_k = [x, y, z, \dot{x}, \dot{y}, \dot{z}, e, \dot{e}, T_t, T_b, t]" /></a>

 - x = coordinates on x-axis
 - y = coordinates on x-axis
 - z = coordinates on x-axis
 - x_dot = velocity on x-axis
 - y_dot = velocity on x-axis
 - z_dot = velocity on x-axis
 - e = position on the extrusion axis
 - e_dot = velocity on the extrusion axis
 - T_t = Temperature of the tool
 - T_b = Temperature of the bed (maybe this should be placed in an other type)
 - t = time at given GCode command

Each GCode command represents a control input <a href="https://www.codecogs.com/eqnedit.php?latex=\vec{u}_k" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\vec{u}_k" title="\vec{u}_k" /></a> on the tools system, coupled with a State transition matrix
<a href="https://www.codecogs.com/eqnedit.php?latex=\mathbf{F}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\mathbf{F}" title="\mathbf{F}" /></a>, allows us to transform state vector to time <a href="https://www.codecogs.com/eqnedit.php?latex=k&space;&plus;&space;1" target="_blank"><img src="https://latex.codecogs.com/gif.latex?k&space;&plus;&space;1" title="k + 1" /></a>

## GCode processing

The GCode is read from the stream and only handled as std::string_views, this eliminates unwanted copying of strings.
Since we don't need to manipulate the text data.

https://www.modernescpp.com/index.php/c-17-avoid-copying-with-std-string-view

Gcode string_views are then processed using the [ranges-v3](https://github.com/ericniebler/range-v3) library, which allows for lazy execution. Each line is split
in a command and comment. The command is then split in to words. Resulting in there basic command, such as G0, G1, M140
etc. then the parameters are checked, such as X100, Y20, S65 etc. This will result in a certain characteristic which is
mapped against a known set of control vector and state transition matrix, from which a type is deduced, eq an [Eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)
Matrix with certain dimensions. This type is instantiated with the parameters and placed in the machine que. Where the
state transitions are processed using std::async algorithms and consumed by the output stream.

https://www.modernescpp.com/index.php/multithreading-in-c-17-and-c-20


## Build

This library makes heavy use of C++17 feautures.

This library is also intended as an investigation in the usability of [Conan](https://conan.io/) for Ultimaker C++ packages.
Check the latest [Conan docs](https://docs.conan.io/en/latest/) for usage and build instructions. When you're developing on Clion it is advised to use [Conan-plugin](https://blog.jetbrains.com/clion/2019/05/getting-started-with-the-conan-clion-plugin/).
