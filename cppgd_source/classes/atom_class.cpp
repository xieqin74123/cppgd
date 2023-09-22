#include <array>
#include <iostream>
#include <vector>

#ifndef GDATA_CLASS_HPP_
#include "gdata_class.hpp"
#endif

void Atom::error(int error_num) const {
    switch (error_num) {
        case 1:
            throw runtime_error("Error A1: this atom has no position data!\n");
            break;
        case 2:
            throw runtime_error("Error A2: this atom has no charge data!\n");
            break;
        case 3:
            throw runtime_error(
                "Error A3: added position vector must has size of 3!\n");
            break;
        default:
            throw invalid_argument(
                "Error A0: invalid error argument received!\n");
            break;
    }
}

Atom::Atom(const short int atom_info) { this->atom_info = atom_info; }

Atom::Atom() {}

Atom::~Atom() { this->clear(); }

void Atom::change_atom(const short int atom_into) {
    this->atom_info = atom_info;
}

void Atom::add_position(const array<float, 3> &position) {
    this->add_position(position[0], position[1], position[2]);
}

void Atom::add_position(const vector<float> &position) {
    if (position.size() != 3) {
        this->error(3);
    }
    this->add_position(position[0], position[1], position[2]);
}

void Atom::add_position(const float x, const float y, const float z) {
    this->position.set = true;
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void Atom::add_charge(const float charge, const string charge_type) {
    this->charge.charge = charge;
    this->charge.charge_type = charge_type;
    this->charge.set = true;
}

void Atom::add_charge(const Charge charge) {
    this->charge = charge;
    this->charge.set = true;
}

void Atom::change_charge_type(const string new_charge_type) {
    this->charge.charge_type = new_charge_type;
}

const string Atom::get_charge_type() const { return this->charge.charge_type; }

const vector<float> Atom::get_position() const {
    if (!this->position_exist()) {
        this->error(1);
    }
    vector<float> output;
    output.push_back(this->position.x);
    output.push_back(this->position.y);
    output.push_back(this->position.z);
    return output;
}

short Atom::get_atom_info() const { return this->atom_info; }

float Atom::get_charge() const {
    if (!this->charge_exist()) {
        this->error(2);
    }
    return this->charge.charge;
}

bool Atom::position_exist() const { return this->position.set; }

bool Atom::charge_exist() const { return this->charge.set; }

void Atom::del_position() {
    if (this->position_exist()) {
        this->position.x = 0;
        this->position.y = 0;
        this->position.z = 0;
        this->position.set = false;
    }
}

void Atom::del_charge() {
    if (this->charge_exist()) {
        this->charge.charge = 0;
        this->charge.charge_type = "";
        this->charge.set = false;
    }
}

void Atom::clear() {
    this->del_position();
    this->del_charge();
    this->atom_info = 0;
}

bool Atom::operator==(const Atom &atom) const {
    bool atom_info_check = (this->atom_info == atom.atom_info);
    bool position_check = (this->position == atom.position);
    bool charge_check = (this->charge == atom.charge);

    return atom_info_check && position_check && charge_check;
}