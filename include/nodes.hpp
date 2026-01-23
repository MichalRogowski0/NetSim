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
        virtual void receive_package(Package&& p) = 0;
        virtual ElementID get_id() const = 0;
        virtual IPackageStockpile::const_iterator begin() const = 0;
        virtual IPackageStockpile::const_iterator end() const = 0;
        virtual IPackageStockpile::const_iterator cbegin() const = 0;
        virtual IPackageStockpile::const_iterator cend() const = 0;

        virtual ~IPackageReceiver() = default;
};

class ReciverPreferences{

     public:
        using preferences_t = std::map<IPackageReceiver*, double>;
        using const_iterator = preferences_t::const_iterator;

        ReciverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}
        void add_reciver(IPackageReceiver* r);
        void remove_reciver(IPackageReceiver* r);
        IPackageReceiver* choose_reciver();

        const_iterator begin() const { return preferences_.begin(); }
        const_iterator end() const { return preferences_.end(); }
        const_iterator cbegin() const { return preferences_.cbegin(); }
        const_iterator cend() const { return preferences_.cend(); }

        const preferences_t& get_preferences() const { return preferences_; }
    
    private:
        preferences_t preferences_;
        ProbabilityGenerator pg_;

};

class PackageSender{

    public:
        PackageSender(PackageSender&&) = default;
        void send_package();
        std::optional<Package>& get_sending_buffer() const;
        ReciverPreferences reciever_preferences_;


    protected:
        void push_package(Package&& p);
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
 
class Worker : public PackageSender, IPackageReceiver{

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

class Storehouse : public IPackageReceiver{
    
    public:
        Storehouse(ElementID id, std::unique_ptr<IPackStockpile> d) : id_(id), d_(d) {}

    private:
        ElementID id_;
        std::unique_ptr<IPackStockpile> d_;
};
