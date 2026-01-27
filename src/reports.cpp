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
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

enum ElementType { RAMP, WORKER, STOREHOUSE, LINK };

struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(line);

    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    ParsedLineData parsed_line_data;
    bool element_selected = false;

    for (const auto &current_token: tokens) {
        if (!element_selected) {
            if (current_token == "RAMP") {
                parsed_line_data.element_type = RAMP;
            } else if (current_token == "WORKER") {
                parsed_line_data.element_type = WORKER;
            } else if (current_token == "STOREHOUSE") {
                parsed_line_data.element_type = STOREHOUSE;
            } else if (current_token == "LINK") {
                parsed_line_data.element_type = LINK;
            } else {
                throw std::runtime_error("Invalid element type: " + current_token);
            }
            element_selected = true;
        } else {
            std::istringstream param_stream(current_token);
            std::string key, value;

            if (std::getline(param_stream, key, '=')) {
                if (std::getline(param_stream, value)) {
                    parsed_line_data.parameters[key] = value;
                }
            }
        }
    }
    return parsed_line_data;
}

int main() {
    try {
        ParsedLineData data = parse_line("WORKER id=5 speed=10");
        std::cout << "Type: " << data.element_type << "\n";
        for (auto const &[k, v]: data.parameters) {
            std::cout << k << " is " << v << "\n";
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

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