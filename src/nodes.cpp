#include "nodes.hpp"

void ReceiverPreferences::add_Receiver(IPackageReceiver* r){
    preferences_[r] = 0.0;
    double eq_propability = 1.0/preferences_.size();

    for(auto& [Receiver, propability] : preferences_){
        propability = eq_propability;
    }
}

void ReceiverPreferences::remove_Receiver(IPackageReceiver* r){
    preferences_.erase(r);

    if (preferences_.empty()){
        return;
    }

    double eq_propability = 1.0/preferences_.size();

    for(auto& [Receiver, propability] : preferences_){
        propability = eq_propability;
    }
}

IPackageReceiver* ReceiverPreferences::choose_Receiver(){
    double p= pg_();
    double sum = 0.0;

    for(auto& [Receiver, propability] : preferences_){
        sum += propability;

        if (p <= sum){
            return Receiver;
        }
    }
    return nullptr;
}

 void PackageSender::send_package(){
    if(!bufor_){
        return;
    }
    IPackageReceiver* reciever = receiver_preferences_.choose_Receiver();
   
    if(reciever){
        reciever ->receive_package(std::move(bufor_.value()));
        bufor_ = std::nullopt;
    }
 }

 void PackageSender::push_package(Package&& p){
    bufor_ = std::move(p);
 }

 void Ramp::deliver_goods(Time t){

    if( (t -1) % di_ == 0){
        Package p;
        push_package(std::move(p));
    }
 }

 void Worker::do_work(Time t){
    if(!processing_bufor_.has_value() &&  !q_->empty()){
        processing_bufor_ = q_ -> pop();
        startTime_ = t;
    }
    if (processing_bufor_.has_value()){
        if ( t >= startTime_ + pd_ - 1){
            push_package(std::move(processing_bufor_.value()));
            processing_bufor_.reset();
            startTime_ = 0;
        }
    }
 }