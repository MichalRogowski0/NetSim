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
