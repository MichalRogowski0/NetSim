

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
#include <fstream>

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
    auto it = worker_collection_.find_by_id(id);
    if (it == worker_collection_.end()) return; // worker not found

    Worker* node = &(*it);

    // Remove this worker from all other workers' preferences
    for (auto& worker : worker_collection_) {
        if (&worker != node) {
            worker.receiver_preferences_.remove_receiver(node);
        }
    }

    // Remove this worker from all ramps' preferences
    for (auto& ramp : ramp_collection_) {
        ramp.receiver_preferences_.remove_receiver(node);
    }

    // Finally remove the worker from the collection
    worker_collection_.remove_by_id(id);

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
       Storehouse* node = &(*storehouse_collection_.find_by_id(id));
    std::for_each(worker_collection_.begin(), worker_collection_.end(), [&node](Worker& ramp) {
        ramp.receiver_preferences_.remove_receiver(node);
    });

    std::for_each(worker_collection_.begin(), worker_collection_.end(), [&node](Worker& worker) {
        worker.receiver_preferences_.remove_receiver(node);
    });
    storehouse_collection_.remove_by_id(id);
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

std::vector<std::string> split(const std::string &str, char delimiter) {
    std::stringstream parameter_stream(str);
    std::string part;
    std::vector<std::string> result;

    while (std::getline(parameter_stream, part, delimiter)) {
        result.push_back(part);
    }

    return result;
}

ParsedLineData parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(line);

    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    tokens = split(line, ' ');

    ParsedLineData parsed_line_data;
    bool element_selected = false;

    std::map<std::string, ElementType> str_to_type{
        {"LOADING_RAMP", RAMP},
        {"WORKER", WORKER},
        {"STOREHOUSE", STOREHOUSE},
        {"LINK", LINK}
    };

    for (const auto &current_token: tokens) {
        if (!element_selected) {
            parsed_line_data.element_type = str_to_type[current_token];
            element_selected = true;
        } else {
            std::istringstream param_stream(current_token);

            auto kv_pair = split(current_token, '=');
            parsed_line_data.parameters[kv_pair[0]] = kv_pair[1];
        }
    }
    return parsed_line_data;
}

bool is_whitespace(const std::string &s) {
    return s.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
}


void parse_link(Factory &factory, std::string src, std::string dest) {
    auto src_type_id = split(src, '-');
    auto dest_type_id = split(dest, '-');

    std::map<std::string, ElementType> type_str_to_enum{
        {"ramp", RAMP},
        {"worker", WORKER},
        {"store", STOREHOUSE}
    };

    ElementType src_type = type_str_to_enum[src_type_id[0]];
    ElementID src_id = std::stoi(src_type_id[1]);
    ElementType dest_type = type_str_to_enum[dest_type_id[0]];
    ElementID dest_id = std::stoi(dest_type_id[1]);

    IPackageReceiver *receiver;

    switch (dest_type) {
        case STOREHOUSE:
            receiver = &*factory.find_storehouse_by_id(dest_id);
            break;
        case WORKER:
            receiver = &*factory.find_worker_by_id(dest_id);
            break;
        default:
            throw;
    }

    switch (src_type) {
        case RAMP:
            factory.find_ramp_by_id(src_id)->receiver_preferences_.add_receiver(receiver);
            break;
        case WORKER:
            factory.find_worker_by_id(src_id)->receiver_preferences_.add_receiver(receiver);
            break;
        default:
            throw;
    }
}

Factory load_factory_structure(std::istream &is) {
    Factory factory;
    std::string line;

    while (std::getline(is, line)) {
        if (line.empty() || is_whitespace(line) || line[0] == ';') continue;

        auto [element_type, parameters] = parse_line(line);

        std::map<std::string, PackageQueueType> str_to_qt{
            {"LIFO", PackageQueueType::LIFO},
            {"FIFO", PackageQueueType::FIFO},
        };

        switch (element_type) {
            case RAMP: {
                // LOADING_RAMP id=<ramp-id> delivery-interval=<delivery-interval>
                ElementID id = std::stoi(parameters.at("id"));
                TimeOffset di = std::stoi(parameters.at("delivery-interval"));

                factory.add_ramp(std::move(Ramp(id, di)));
            }
            break;
            case WORKER: {
                // WORKER id=<worker-id> processing-time=<processing-time> queue-type=<queue-type>
                ElementID id = std::stoi(parameters.at("id"));
                TimeOffset pd = std::stoi(parameters.at("processing-time"));
                std::string qt = parameters.at("queue-type");

                std::unique_ptr<PackageQueue> q = std::make_unique<PackageQueue>(str_to_qt.at(qt));
                factory.add_worker(std::move(Worker(id, pd, std::move(q))));
            }
            break;
            case STOREHOUSE: {
                // STOREHOUSE id=<storehouse-id>
                ElementID id = std::stoi(parameters.at("id"));
                std::unique_ptr<PackageQueue> d = std::make_unique<PackageQueue>(str_to_qt["FIFO"]);
                factory.add_storehouse(std::move(Storehouse(id, std::move(d))));
            }
            break;
            case LINK: {
                // LINK src=<node-type>-<node-id> dest=<node-type>-<node-id>
                std::string src = parameters.at("src");
                std::string dest = parameters.at("dest");

                parse_link(factory, src, dest);
            }
            break;
        }
    }
    return factory;
}

void save_factory_structure(const Factory &factory, std::ostream &os) {
    for (auto r = factory.ramp_cbegin(); r != factory.ramp_cend(); ++r) {
        os
                << "LOADING_RAMP id=" << r->get_id()
                << " delivery-interval=" << r->get_delivery_interval() <<
                std::endl;
    }

    for (auto w = factory.worker_cbegin(); w != factory.worker_cend(); ++w) {
        std::string queue_type;

        switch ((w->get_queue()->get_queue_type())) {
            case (PackageQueueType::FIFO):
                queue_type = std::string("FIFO");
                break;
            case (PackageQueueType::LIFO):
                queue_type = std::string("LIFO");
                break;
            default:
                queue_type = std::string("UNKNOWN");
        }
        os
                << "WORKER id=" << w->get_id()
                << " processing-time=" << w->get_processing_duration()
                << " queue-type=" << queue_type << std::endl;
    }

    for (auto s = factory.storehouse_cbegin(); s != factory.storehouse_cend(); ++s) {
        os << "STOREHOUSE id=" << s->get_id() << std::endl;
    }

    for (auto r = factory.ramp_cbegin(); r != factory.ramp_cend(); ++r) {
        for (auto [receiver, _] : r->receiver_preferences_.get_preferences()) {
            os << "LINK src=ramp-" << r->get_id() << " dest=";

            switch (receiver->get_receiver_type()) {
                case REC_WORKER:
                    os << "worker-";
                break;
                case REC_STOREHOUSE:
                    os << "store-";
                break;
            }

            os << receiver->get_id() << std::endl;
        }
    }

    for (auto w = factory.worker_cbegin(); w != factory.worker_cend(); ++w) {
        for (auto [receiver, _] : w->receiver_preferences_.get_preferences()) {
            os << "LINK src=worker-" << w->get_id() << " dest=";

            switch (receiver->get_receiver_type()) {
                case REC_WORKER:
                    os << "worker-";
                break;
                case REC_STOREHOUSE:
                    os << "store-";
                break;
            }

            os << receiver->get_id() << std::endl;
        }
    }
}



bool Factory::has_reachable_storehouse(const PackageSender* sender,
                                       std::map<const PackageSender*, NodeColor>& node_colors) const {
    if (node_colors[sender] == NodeColor::VERIFIED) return true;
    if (node_colors[sender] == NodeColor::VISITED) return false; // cycle

    const auto& prefs = sender->receiver_preferences_.get_preferences();
    if (prefs.empty()) return false; // missing link → inconsistent

    node_colors[sender] = NodeColor::VISITED;

    for (const auto& [receiver, _] : prefs) {
        if (receiver->get_receiver_type() == ReceiverType::REC_STOREHOUSE) {
            node_colors[sender] = NodeColor::VERIFIED;
            return true;
        } else if (receiver->get_receiver_type() == ReceiverType::REC_WORKER) {
            const Worker* w = dynamic_cast<const Worker*>(receiver);
            const PackageSender* sendrecv_ptr = dynamic_cast<const PackageSender*>(w);
            if (!sendrecv_ptr) continue;

            if (has_reachable_storehouse(sendrecv_ptr, node_colors)) {
                node_colors[sender] = NodeColor::VERIFIED;
                return true;
            }
        }
    }

    return false; // no path found
}

bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> node_colors;

    for (const auto& ramp : ramp_collection_) node_colors[&ramp] = NodeColor::UNVISITED;
    for (const auto& worker : worker_collection_) node_colors[&worker] = NodeColor::UNVISITED;

    for (const auto& ramp : ramp_collection_) {
        if (!has_reachable_storehouse(&ramp, node_colors)) return false;
    }

    return true;
}
