#include "simulation.hpp"
#include <stdexcept>
#include <utility>

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf) {
    if (!f.is_consistent()) {
        throw std::logic_error("Sieć fabryki jest niespójna");
    }

    for (Time t = 1; t <= d; ++t) {

        f.do_deliveries(t);
        f.do_package_passing();
        f.do_work(t);

        rf(f, t);
    }
}

SpecificTurnsReportNotifier::SpecificTurnsReportNotifier(std::set<Time> turns)
    : turns_(std::move(turns)) {}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) const {
    return turns_.find(t) != turns_.end();
}

IntervalReportNotifier::IntervalReportNotifier(TimeOffset to)
    : to_(to) {
    if (to_ <= 0) {
        throw std::logic_error("Interwał musi być większy od 0");
    }
}

bool IntervalReportNotifier::should_generate_report(Time t) const {
    return (t - 1) % to_ == 0;
}