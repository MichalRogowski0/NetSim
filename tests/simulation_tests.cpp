//
// Created by Kamil on 28.01.2026.
//
#include <gtest/gtest.h>

#include "simulation.hpp"
#include "factory.hpp"
#include "nodes.hpp"

TEST(SimulationNotifiersTest, SpecificTurnsReportNotifier_Works) {
    SpecificTurnsReportNotifier n({1, 3, 7});

    EXPECT_TRUE(n.should_generate_report(1));
    EXPECT_FALSE(n.should_generate_report(2));
    EXPECT_TRUE(n.should_generate_report(3));
    EXPECT_FALSE(n.should_generate_report(4));
    EXPECT_FALSE(n.should_generate_report(6));
    EXPECT_TRUE(n.should_generate_report(7));
    EXPECT_FALSE(n.should_generate_report(8));
}

TEST(SimulationNotifiersTest, IntervalReportNotifier_Works) {
    IntervalReportNotifier n(2);

    EXPECT_TRUE(n.should_generate_report(1));
    EXPECT_FALSE(n.should_generate_report(2));
    EXPECT_TRUE(n.should_generate_report(3));
    EXPECT_FALSE(n.should_generate_report(4));
    EXPECT_TRUE(n.should_generate_report(5));
}

TEST(SimulationTest, Simulate_ThrowsWhenFactoryNetworkIsInconsistent) {
    Factory f;

    f.add_ramp(Ramp(1, 1));

    EXPECT_THROW(
            simulate(f, 1, [](Factory&, Time) {}),
            std::logic_error
    );
}

TEST(SimulationTest, Simulate_CallsReporterEveryTurn) {
    Factory f;

    f.add_ramp(Ramp(1, 1));
    f.add_storehouse(Storehouse(1));

    auto r_it = f.find_ramp_by_id(1);
    ASSERT_NE(r_it, f.ramp_cend());

    auto s_it = f.find_storehouse_by_id(1);
    ASSERT_NE(s_it, f.storehouse_cend());

    r_it->receiver_preferences_.add_receiver(&(*s_it));

    std::vector<Time> called_times;
    simulate(f, 5, [&called_times](Factory&, Time t) {
        called_times.push_back(t);
    });

    ASSERT_EQ(called_times.size(), 5u);
    EXPECT_EQ(called_times[0], 1);
    EXPECT_EQ(called_times[1], 2);
    EXPECT_EQ(called_times[2], 3);
    EXPECT_EQ(called_times[3], 4);
    EXPECT_EQ(called_times[4], 5);
}

TEST(SimulationTest, Simulate_PerformsTurnsAndMovesPackagesToStorehouse) {
    Factory f;

    f.add_ramp(Ramp(1, 1));
    f.add_storehouse(Storehouse(1));

    auto r_it = f.find_ramp_by_id(1);
    ASSERT_NE(r_it, f.ramp_cend());

    auto s_it = f.find_storehouse_by_id(1);
    ASSERT_NE(s_it, f.storehouse_cend());

    r_it->receiver_preferences_.add_receiver(&(*s_it));

    simulate(f, 4, [](Factory&, Time) {});

    auto begin = s_it->begin();
    auto end = s_it->end();
    const auto count = static_cast<int>(std::distance(begin, end));

    EXPECT_EQ(count, 4);
}