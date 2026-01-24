#include "storage_types.hpp"
#include <stdexcept>
#include <utility>

PackageQueue::PackageQueue(PackageQueueType type) : type_(type) {}

void PackageQueue::push(Package&& p) {
    stockpile_.emplace_back(std::move(p));
}

bool PackageQueue::empty() const {
    return stockpile_.empty();
}

IPackageStockpile::size_type PackageQueue::size() const {
    return stockpile_.size();
}

IPackageStockpile::const_iterator PackageQueue::begin() const {
    return stockpile_.begin();
}

IPackageStockpile::const_iterator PackageQueue::end() const {
    return stockpile_.end();
}

IPackageStockpile::const_iterator PackageQueue::cbegin() const {
    return stockpile_.cbegin();
}

IPackageStockpile::const_iterator PackageQueue::cend() const {
    return stockpile_.cend();
}

Package PackageQueue::pop() {
    if (stockpile_.empty()) {
        throw std::logic_error("Składowisko (stockpile) puste");
    }

    switch(type_) {
        case PackageQueueType::FIFO : {
            Package p = std::move(stockpile_.front());
            stockpile_.pop_front();
            return p;
        }
        case PackageQueueType::LIFO : {
            Package p = std::move(stockpile_.back());
            stockpile_.pop_back();
            return p;
        }
        default:
            throw std::logic_error("Nieznany typ kolejki");
    }
}

PackageQueueType PackageQueue::get_queue_type() const {
    return type_;
}