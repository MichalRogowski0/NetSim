#include <iostream>
#include <algorithm>
#include "reports.hpp"
#include "nodes.hpp"

void generate_structure_report(const Factory& f, std::ostream& os){

    os << "Loading ramps: " << std::endl;
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); ++it){
        os << "Rampa " << it -> get_id() << " interval " << it -> get_delivery_interval();

        const auto& pref = it -> receiver_preferences_.get_preferences();
        os << "Receivers: "; 

        if (pref.empty()) {
            os << "empty" << std::endl;
        } else {
            for (auto pref_it = pref.cbegin(); pref_it != pref.cend(); ++pref_it){
                os << "Receiver: "<< pref_it -> first -> get_id() << std::endl;
            }
        }
    }
    os << "Workers: " << std::endl;
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it){
        os << "Worker: " << it -> get_id() << " processing duration " << it -> get_processing_duration();

        const auto& pref = it -> receiver_preferences_.get_preferences();
        os << " Receivers: ";

        if (pref.empty()) {
            os << "empty" << std::endl;
        } else {
            for (auto pref_it = pref.cbegin(); pref_it != pref.cend(); ++pref_it){
                os << "Receiver: "<< pref_it -> first -> get_id() << std::endl;
            }
        }
    }

    os << "Storehouses: " << std::endl;
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) {
        os << "  Storehouse: " << it->get_id() << std::endl;
    }

}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t) {

    os << "Turn: " << t << std::endl;
    
    os << "Workers state:" << std::endl;
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        os << "Worker " << it->get_id() << ":" << std::endl;
        const auto& worker_buffer = it->get_processing_buffer();
        if (worker_buffer.has_value()) {
            os << "  Processing: " << worker_buffer->get_id() << std::endl;
        } else {
            os << "  Processing: none" << std::endl;
        }
        os << "  Queue: ";
        if (it->cbegin() == it->cend()) {
            os << "empty";
        } else {
            for (auto q_it = it->cbegin(); q_it != it->cend(); ++q_it) {
                os << q_it->get_id() << " ";
            }
        }
        os << std::endl;
    }

    os << "Storehouses state:" << std::endl;
    for (auto st = f.storehouse_cbegin(); st != f.storehouse_cend(); ++st) {
        os << "Storehouse " << st->get_id() << " stock: ";
        if (st->cbegin() == st->cend()) {
            os << "empty";
        } else {
            for (auto p_it = st->cbegin(); p_it != st->cend(); ++p_it) {
                os << p_it->get_id() << " ";
            }
        }
        os << std::endl;
    }
}