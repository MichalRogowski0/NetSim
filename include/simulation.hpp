#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "types.hpp"
#include "factory.hpp"
#include <set>
#include <functional>

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf);

class SpecificTurnsReportNotifier {
public:
    explicit SpecificTurnsReportNotifier(std::set<Time> turns);
    bool should_generate_report(Time t) const;

private:
    std::set<Time> turns_;
};

class IntervalReportNotifier {
public:
    explicit IntervalReportNotifier(TimeOffset interval);
    bool should_generate_report(Time t) const;

private:
    TimeOffset interval_;
};

#endif //SIMULATION_HPP