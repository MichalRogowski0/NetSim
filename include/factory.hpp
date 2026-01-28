#pragma once

#include <vector>
#include "nodes.hpp"
enum class NodeColor { UNVISITED, VISITED, VERIFIED };

template<typename Node>
class NodeCollection {
public:
    using container_t = std::vector<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return container_.begin(); }
    iterator end() { return container_.end(); }

    const_iterator begin() const { return container_.begin(); }
    const_iterator end() const { return container_.end(); }

    const_iterator cbegin() const { return container_.cbegin(); }
    const_iterator cend() const { return container_.cend(); }

    NodeCollection(): container_() {
    };

    void add(Node &&node);

    void remove_by_id(int id);

    iterator find_by_id(ElementID id);

    const_iterator find_by_id(ElementID id) const;

private:
    container_t container_;
};

class Factory {
public:
    void add_ramp(Ramp &&);

    void remove_ramp(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;

    NodeCollection<Ramp>::const_iterator ramp_cbegin() const;

    NodeCollection<Ramp>::const_iterator ramp_cend() const;

    void add_worker(Worker &&);

    void remove_worker(ElementID id);

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);

    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;

    NodeCollection<Worker>::const_iterator worker_cbegin() const;

    NodeCollection<Worker>::const_iterator worker_cend() const;

    void add_storehouse(Storehouse &&);

    void remove_storehouse(ElementID id);

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);

    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const;

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const;

    NodeCollection<Storehouse>::const_iterator storehouse_cend() const;

    bool is_consistent(void) const;

    void do_deliveries(Time);

    void do_package_passing(void);

    void do_work(Time);
    bool has_reachable_storehouse(const PackageSender *sender, std::map<const PackageSender*, NodeColor> &node_colors) const;




private:
    template<typename Node>
    void remove_receiver(NodeCollection<Node> &collection, ElementID id);

    NodeCollection<Ramp> ramp_collection_;
    NodeCollection<Worker> worker_collection_;
    NodeCollection<Storehouse> storehouse_collection_;
};

Factory load_factory_structure(std::istream&);

void save_factory_structure(const Factory& factory, std::ostream& os);


template<class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    // Find the node to remove
    auto iter = collection.find_by_id(id);
    if (iter == collection.end()) return;  // node not found

    IPackageReceiver* receiver_ptr = dynamic_cast<IPackageReceiver*>(&(*iter));
    if (!receiver_ptr) return;  // safety check

    // Remove receiver from all ramps
    for (auto& ramp : ramp_collection_) {
        ramp.receiver_preferences_.remove_receiver(receiver_ptr);
    }

    // Remove receiver from all workers
    for (auto& worker : worker_collection_) {
        worker.receiver_preferences_.remove_receiver(receiver_ptr);
    }

    // Finally remove from the collection itself
    collection.remove_by_id(id);
}