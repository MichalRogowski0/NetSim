#include <iostream>
#include <algorithm>
#include "reports.hpp"
#include "nodes.hpp"

void generate_structure_report(const Factory& f, std::ostream& os){
    os << "Workers: " << std::endl;

    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it){
            os << "Rampa " << it -> get_id() << "interval " << it -> get_delivery_interval();
            const auto& pref = it -> receiver_preferences_.get_preferences();

            if (pref.empty()) {
                os << "empty" << std::endl;
            }

            if(auto pref_it = pref.cbegin(); pref_it != pref.cend(); ++pref_it){
                os << "Receiver:: "<< pref_it -> first -> get_id() << std::endl;
            }
    }
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os << "Tura :" << t << std::endl;

    for(auto it = f.worker_cbegin(); it != f.workers_cend(); ++it){
        
        const auto& worker_buffer = it -> get_processing_buffer();
        if (worker_buffer.has_valuse()){
            os << "Processing: " << worker_buffer -> get_id() << std::endl;
        } else {
            os << "Processing: none" << std::endl;
        }
    }
}