#include "nodes.hpp"

void ReciverPreferences::add_reciver(IPackageReceiver* r){
    preferences_[r] = 0.0;
    double eq_propability = 1.0/preferences_.size();

    for(auto& [receiver, propability] : preferences_){
        propability = eq_propability;
    }
}

void ReciverPreferences::remove_reciver(IPackageReceiver* r){
    preferences_.erase(r);

    if (preferences_.empty()){
        return;
    }

    double eq_propability = 1.0/preferences_.size();

    for(auto& [receiver, propability] : preferences_){
        propability = eq_propability;
    }
}

IPackageReceiver* ReciverPreferences::choose_reciver(){
    double p= pg_();
    double sum = 0.0;

    for(auto& [receiver, propability] : preferences_){
        sum += propability;

        if (p <= sum){
            return receiver;
        }
    }
}

 void PackageSender::send_package(){
    if(!buffor_){
        return;
    }
    IPackageReceiver* reciever = receiver_preferences_.choose_reciver();
   
    if(reciever){
        reciever ->receive_package(std::move(buffor_.value()));
    }
    buffor_ = std::nullopt;

 }

 void PackageSender::push_package(Package&& p){
    buffor_ = std::move(p);
 }

 void Ramp::deliver_goods(Time t){

    if(id_ % (t - 1) == 0){
        Package p;
        push_package(std::move(p));
    }
 }