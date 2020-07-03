//
// Created by Jelle Spijker on 6/27/20.
//
#include <gtest/gtest.h>

#include <string_view>
#include <iostream>

#include "GHermeneus/Machine.h"


using namespace std::literals;
TEST(MachineTestSuite, ParseGCode)
{
    const auto GCode
    {
        "T0\n" // Tool 0
        "M82 ;absolute extrusion mode\n" // set extrusion mode to absolute
        "G92 E0\n" // set the extrusion axis to position 0
        "G0 F15000 X100 Y75.75 Z2\n" // Move to 100, 75.75, 2 with a feedrate of 15000
        "G1 F1500 E-6.5\n" // retract filament 6.5 mm with a feedrate of 1500
        "G1 F600 Z0.27\n" // move tool to a height of 2.27 with a feedrate of 600
        "G1 F1500 E0\n"  // move filament to 0 with a feedrate of 1500
        "G1 F1200 X110 Y100 E0.0500\n" // move tool to 110, 100, 0.27 and extrude to 0.05 off filament with a feedrate of 1500
        "G1 Y110.33 E0.0655\n" // move tool to 110, 110.33, 0.27 and extrude to 0.0655 off filament with a feedrate of 1500
    };
    const auto gcode = std::string_view {GCode};

    auto machine = GHermeneus::Machine();
    machine << gcode;
    std::cout << machine;
}