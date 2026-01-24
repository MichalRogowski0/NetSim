#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "types.hpp"
#include <set>

class Package {
public:
    Package();
    Package(ElementID ID);
    Package(Package&& other) noexcept;
    Package& operator=(Package&& other) noexcept;
    ElementID get_id() const {return ID_;}
    ~Package();

private:
    ElementID ID_;

    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};
#endif //PACKAGE_HPP