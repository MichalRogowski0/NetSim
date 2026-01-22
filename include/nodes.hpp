#pragma once

#include <map>
#include <utility>
#include <functional>
#include <algorithm>
#include <memory>
#include <optional>

#include "types.hpp"


class ReciverPreferencess{

     public:
        using preferences_t = std::map<IPackageReciver*, double>;
        using const_iterator = preferences_t::const_iterator;

        ReciverPreferencess(PropabilityGenerator pg = probability_generator){} 
        void add_reciver(IPackageReciver*);
        void remove_reciver(IPackageReciver*);
        IPackageReciever* choose_reciver();
        preferences_t& get_preferences() const;
    
    private:
        preferences_t preferences_;
        ProbabilityGenerator pg_;

};