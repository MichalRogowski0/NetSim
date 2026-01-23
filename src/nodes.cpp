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
