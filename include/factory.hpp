#include <vector>
#include <algorithm>
#include <stdexcept>
#include "nodes.hpp"


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
    NodeCollection(): container_() {};

    void add(Node &&node);

    void remove_by_id(int id);

    iterator find_by_id(ElementID id);

    const_iterator find_by_id(ElementID id) const;
private:
    container_t container_ ;
};

class Factory {
public:
    Factory(): ramp_collection_(), worker_collection_(), storehouse_collection_() {};
    void add_ramp(Ramp &&);

    void remove_ramp(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);

    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;

    NodeCollection<Ramp>::const_iterator ramp_cbegin();

    NodeCollection<Ramp>::const_iterator ramp_cend();

    void add_worker(Worker &&);

    void remove_worker(ElementID id);

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);

    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;

    NodeCollection<Worker>::const_iterator worker_cbegin();

    NodeCollection<Worker>::const_iterator worker_cend();

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

private:
    template<typename Node>
    void remove_receiver(NodeCollection<Node> &collection, ElementID id);
    NodeCollection<Ramp> ramp_collection_;
    NodeCollection<Worker> worker_collection_;
    NodeCollection<Storehouse> storehouse_collection_;

};
