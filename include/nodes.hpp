#pragma once

#include <map>
#include <utility>
#include <functional>
#include <algorithm>
#include <memory>
#include <optional>

#include "types.hpp"
#include "helpers.hpp"


class IPackageReceiver{
    
    public:
        void recieve_package(Package&& p);
        ElementID get_id() const;
};

class ReciverPreferences{

     public:
        using preferences_t = std::map<IPackageReceiver*, double>;
        using const_iterator = preferences_t::const_iterator;

        ReciverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}
        void add_reciver(IPackageReceiver* r);
        void remove_reciver(IPackageReceiver* r);
        IPackageReceiver* choose_reciver();
        preferences_t& get_preferences() const;
    
    private:
        preferences_t preferences_;
        ProbabilityGenerator pg_;

};

class PackageSender{

    public:
        void send_package();
        std::optional<Package>& get_sending_buffer() const;

    protected:
        void push_package(Package&& p);

    private:
        ReciverPreferences reciever_preferences_;

};

class Ramp : public PackageSender{

    public:
        Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
        void deliver_goods(Time t);
        TimeOffset get_delivery_goods() const;
        ElementID get_id() const;
    
    private:
        ElementID id_;
        TimeOffset di_;
};
 
class Worker : public PackageSender{

    public:
        Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : 
        id_(id), pd_(pd), q_(std::move(q)) {}
        void do_work(Time t);
        TimeOffset get_processing_duration() const;
        Time get_package_processing_start_time() const;

    private:
        ElementID id_;
        TimeOffset pd_;
        std::unique_ptr<IPackageQueue> q_;

};
