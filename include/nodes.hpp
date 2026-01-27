#pragma once

#include <map>
#include <utility>
#include <functional>
#include <algorithm>
#include <memory>
#include <optional>

#include "types.hpp"
#include "helpers.hpp"
#include "package.hpp"
#include "storage_types.hpp"

enum ReceiverType {REC_WORKER, REC_STOREHOUSE};

class IPackageReceiver{
    
    public:
        virtual void receive_package(Package&& p) = 0;
        virtual ElementID get_id() const = 0;

        virtual IPackageStockpile::const_iterator begin() const = 0;
        virtual IPackageStockpile::const_iterator end() const = 0;
        virtual IPackageStockpile::const_iterator cbegin() const = 0;
        virtual IPackageStockpile::const_iterator cend() const = 0;

        virtual ReceiverType get_receiver_type() const = 0;

        // virtual ~IPackageReceiver() = default;
};


class ReceiverPreferences{

     public:
        using preferences_t = std::map<IPackageReceiver*, double>;
        using const_iterator = preferences_t::const_iterator;

        ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}
        void add_receiver(IPackageReceiver* r);
        void remove_receiver(IPackageReceiver* r);
        IPackageReceiver* choose_receiver();

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
        // PackageSender() = default;
        // PackageSender(PackageSender&& p) = default;
        void send_package();
        const std::optional<Package>& get_sending_buffer() const { return buffer_; }
        ReceiverPreferences receiver_preferences_;


    protected:
        void push_package(Package&& p);
        std::optional<Package> buffer_ = std::nullopt;
};

class Ramp : public PackageSender{

    public:
        Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
        void deliver_goods(Time t);
        TimeOffset get_delivery_interval() const { return di_; }
        ElementID get_id() const { return id_; }
    
    private:
        ElementID id_;
        TimeOffset di_;
};
 
class Worker : public PackageSender, public IPackageReceiver{

    public:
        Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : 
        id_(id), pd_(pd), q_(std::move(q)) {}
        void do_work(Time t);
        TimeOffset get_processing_duration() const { return pd_; }
        Time get_package_processing_start_time() const { return startTime_; }
        const std::optional<Package>& get_processing_buffer() const { return processing_buffer_; }

        void receive_package(Package&& p) override { q_ -> push(std::move(p)); }
        ElementID get_id() const override { return id_; }

        IPackageStockpile::const_iterator begin() const override { return q_ -> begin(); }
        IPackageStockpile::const_iterator end() const override { return q_ -> end(); }
        IPackageStockpile::const_iterator cbegin() const override { return q_ -> cbegin(); }
        IPackageStockpile::const_iterator cend() const override { return q_ -> cend(); }

        ReceiverType get_receiver_type() const override { return REC_WORKER; }
        PackageQueueType get_queue_type() const { return q_->get_queue_type(); }

    private:
        Time startTime_ = 0;
        ElementID id_;
        TimeOffset pd_;
        std::unique_ptr<IPackageQueue> q_;
        std::optional<Package>  processing_buffer_ = std::nullopt;
};

class Storehouse : public IPackageReceiver{
    
    public:
        Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) 
        : id_(id), d_(std::move(d)) {}


        void receive_package(Package&& p) override { d_ -> push(std::move(p)); }
        ElementID get_id() const override { return id_; }

        IPackageStockpile::const_iterator begin() const override { return d_ -> begin(); }
        IPackageStockpile::const_iterator end() const override { return d_ -> end(); }
        IPackageStockpile::const_iterator cbegin() const override { return d_ -> cbegin(); }
        IPackageStockpile::const_iterator cend() const override { return d_ -> cend(); }

        ReceiverType get_receiver_type() const override { return REC_STOREHOUSE; }

        // ~Storehouse() override = default;
        // Storehouse(Storehouse&&) = default;

    private:
        ElementID id_;
        std::unique_ptr<IPackageStockpile> d_;
};
