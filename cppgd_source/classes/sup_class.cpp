#ifndef GDATA_CLASS_HPP_
#include "gdata_class.hpp"
#endif

// class Position
bool Position::operator==(const Position& position) const {
    bool x_check = (this->x == position.x);
    bool y_check = (this->y == position.y);
    bool z_check = (this->z == position.z);
    bool set_check = (this->set == position.set);

    return x_check && y_check && z_check && set_check;
}

// class Dipole
bool Dipole::operator==(const Dipole& dipole) const {
    bool x_check = (this->x == dipole.x);
    bool y_check = (this->y == dipole.y);
    bool z_check = (this->z == dipole.z);
    bool set_check = (this->set == dipole.set);

    return x_check && y_check && z_check && set_check;
}

// class Charge
bool Charge::operator==(const Charge& charge) const {
    bool charge_check = (this->charge == charge.charge);
    bool charge_type_check = (this->charge_type == charge.charge_type);
    bool set_check = (this->set == charge.set);

    return charge_check && charge_type_check && set_check;
}

// class Bond
Bond::Bond(unsigned long atom1, unsigned long atom2, short order) {
    this->atom1 = atom1;
    this->atom2 = atom2;
    this->order = order;
}

Bond::Bond() {}

void Bond::add_connection(unsigned long atom1, unsigned long atom2) {
    this->atom1 = atom1;
    this->atom2 = atom2;
}

void Bond::add_order(short order) { this->order = order; }

array<unsigned long, 2> Bond::get_connection() const {
    array<unsigned long, 2> output;
    output[0] = this->atom1;
    output[1] = this->atom2;
    return output;
}

const short Bond::get_order() const { return this->order; }

bool Bond::operator==(const Bond& bond) const {
    bool atom1_check =
        (this->atom1 == bond.atom1) || (this->atom1 == bond.atom2);
    bool atom2_check =
        (this->atom2 == bond.atom1) || (this->atom2 == bond.atom2);
    bool order_check = (this->order == bond.order);

    return atom1_check && atom2_check && order_check;
}
