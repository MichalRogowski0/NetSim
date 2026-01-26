#pragma once

#include "factory.hpp"
#include <iostream>
#include "types.hpp"

void generate_structure_report(const Factory& f, std::ostream& os);

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

class SpecificTurnsReportNotifier{

};

class IntervalReportNotifier {

};