#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "factory.hpp"
#include "package.hpp"


template<typename Node>
void NodeCollection<Node>::add(Node &&node) {
    this->container_.push_back(std::move(node));
}

template<typename Node>
void NodeCollection<Node>::remove_by_id(int id) {
    iterator it = this->find_by_id(id);
    if (it != this->container_.end()) {
        this->container_.erase(it);
    }
}

template<typename Node>
typename NodeCollection<Node>::iterator NodeCollection<Node>::find_by_id(ElementID id) {
    return std::find_if(
        this->container_.begin(),
        this->container_.end(),
        [id](Node &n) { return n.get_id() == id; }
    );
}


template<typename Node>
typename NodeCollection<Node>::const_iterator NodeCollection<Node>::find_by_id(ElementID id) const {
    return std::find_if(
        this->container_.cbegin(),
        this->container_.cend(),
        [id](const Node &n) { return n.get_id() == id; }
    );
}


void Factory::add_ramp(Ramp &&ramp) {
    this->ramp_collection_.add(std::move(ramp));
}


void Factory::remove_ramp(ElementID id) {
    this->ramp_collection_.remove_by_id(id);
}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {
    return this->ramp_collection_.find_by_id(id);
}


NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const {
    return this->ramp_collection_.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() const {
    return ramp_collection_.cbegin();
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() const {
    return ramp_collection_.cend();
}

void Factory::add_worker(Worker &&worker) {
    this->worker_collection_.add(std::move(worker));
}

void Factory::remove_worker(ElementID id) {
    this->worker_collection_.remove_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) {
    return this->worker_collection_.find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const {
    return this->worker_collection_.find_by_id(id);
}

NodeCollection<Worker>::const_iterator Factory::worker_cbegin() const {
    return worker_collection_.cbegin();
}

NodeCollection<Worker>::const_iterator Factory::worker_cend() const {
    return worker_collection_.cend();
}

void Factory::add_storehouse(Storehouse &&storehouse) {
    this->storehouse_collection_.add(std::move(storehouse));
}

void Factory::remove_storehouse(ElementID id) {
    this->storehouse_collection_.remove_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) {
    return this->storehouse_collection_.find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const {
    return this->storehouse_collection_.find_by_id(id);
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() const {
    return storehouse_collection_.cbegin();
}

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() const {
    return storehouse_collection_.cend();
}

bool Factory::is_consistent() const {
    for (const auto &ramp: ramp_collection_) {
        std::set<const PackageSender *> visited;

        if (!has_reachable_storehouse(&ramp, visited)) {
            throw std::logic_error(
                "Sieć niespójna: z rampy ID=" + std::to_string(ramp.get_id()) +
                " nie ma ścieżki prowadzącej do magazynu."
            );
        }
    }

    return true;
}


void Factory::do_deliveries(Time time) {
    for (auto &ramp: ramp_collection_) {
        ramp.deliver_goods(time);
    }
}

void Factory::do_package_passing(void) {
    for (auto &ramp: ramp_collection_) {
        ramp.send_package();
    }

    for (auto &worker: worker_collection_) {
        worker.send_package();
    }
}

void Factory::do_work(Time time) {
    for (auto &worker: worker_collection_) {
        worker.do_work(time);
    }
}


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

bool Factory::has_reachable_storehouse(const PackageSender *sender, std::set<const PackageSender *> &visited) const {
    if(sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Nadawca nie posiada żadnego połączenia wyjściowego.");
    }

    visited.insert(sender);

    for (const auto& [receiver, prob] : sender->receiver_preferences_.get_preferences()) {

        if (receiver->get_receiver_type() == ReceiverType::REC_STOREHOUSE) {
            return true;
        }

        if (receiver->get_receiver_type() == ReceiverType::REC_WORKER) {
            const Worker* w = dynamic_cast<const Worker*>(receiver);
            if (w == sender) {
                continue;
            }

            if (visited.find(w) != visited.end()) {
                continue;
            }

            if (has_reachable_storehouse(w, visited)) {
                return true;
            }
        }
    }

    return false;
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
