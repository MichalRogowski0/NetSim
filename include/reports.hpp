#pragma once

#include <iostream>
#include <set>
#include "factory.hpp"
#include "types.hpp"

void generate_structure_report(const Factory& f, std::ostream& os);

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

class SpecificTurnsReportNotifier{

    public:
        SpecificTurnsReportNotifier(std::set<Time> Turns) : turns_(std::move(Turns)) {}
        bool generate_report(Time t) const { return turns_.find(t) != turns_.end(); }

    private:
        std::set<Time> turns_;
};
class IntervalReportNotifier {

    public:
        IntervalReportNotifier(TimeOffset Interval) : interval_(Interval) {}
        bool generate_report(Time t) const { return (t - 1) % interval_ == 0; }

    private:
        TimeOffset interval_;
};