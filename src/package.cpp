#include "package.hpp"
#include <stdexcept>

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package() {
    if (freed_IDs.empty()) {
        if (assigned_IDs.empty()) {
            ID_ = 1;
        } else {
            ID_ = *assigned_IDs.rbegin() + 1;
        }
    } else {
        auto it = freed_IDs.begin();
        ID_ = *it;
        freed_IDs.erase(it);
    }
    assigned_IDs.insert(ID_);
}

Package::Package(ElementID ID) {
    if (ID <= 0) {
        throw std::invalid_argument("ID nie może być mniejsze lub równe 0");
    }

    if (assigned_IDs.find(ID) != assigned_IDs.end()) {
        throw std::logic_error("ID nie mogą być takie same");
    }

    ID_ = ID;
    freed_IDs.erase(ID_);
    assigned_IDs.insert(ID_);
}

Package::Package(Package&& other) noexcept {
    ID_ = other.ID_;
    other.ID_ = 0;
}

Package& Package::operator=(Package&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (ID_ != 0) {
        assigned_IDs.erase(ID_);
        freed_IDs.insert(ID_);
    }

    ID_ = other.ID_;
    other.ID_ = 0;

    return *this;
}

Package::~Package() {
    if (ID_ != 0) {
        assigned_IDs.erase(ID_);
        freed_IDs.insert(ID_);
    }
}