#include <vector>
#include <algorithm>
#include <stdexcept>
#include "nodes.hpp"
#include "factory.hpp"


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
        [id](Node& n) { return n.get_id() == id; }  // use reference
    );
}


template<typename Node>
typename NodeCollection<Node>::const_iterator NodeCollection<Node>::find_by_id(ElementID id) const {
    return std::find_if(
        this->container_.cbegin(),
        this->container_.cend(),
        [id](const Node& n) { return n.get_id() == id; }  // const reference
    );
}



void Factory::add_ramp(Ramp&& ramp) {
    this->ramp_collection_.add(std::move(ramp));
}


void Factory::remove_ramp(ElementID id) {
    this->ramp_collection_.remove_by_id(id);

}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id){
        return this->ramp_collection_.find_by_id(id);
    }


NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const {
    return this->ramp_collection_.find_by_id(id);
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() {
    return ramp_collection_.cbegin();

}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() {
    return ramp_collection_.cend();
}

void Factory::add_worker(Worker && worker) {
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

NodeCollection<Worker>::const_iterator Factory::worker_cbegin() {
    return worker_collection_.cbegin();
}

NodeCollection<Worker>::const_iterator Factory::worker_cend() {
    return worker_collection_.cend();
}

void Factory::add_storehouse(Storehouse && storehouse) {
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

bool Factory::is_consistent(void) const {
}

void Factory::do_deliveries(Time) {
}

void Factory::do_package_passing(void) {
}

void Factory::do_work(Time) {
}


template<typename Node>
void Factory::remove_receiver(NodeCollection<Node> &collection, ElementID id) {
}
