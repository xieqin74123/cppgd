#ifndef GDATA_CLASS_HPP_
#include "gdata_class.hpp"
#endif

void Gdata::error(int error_num) const {
    switch (error_num) {
        case 1:
            throw runtime_error("Error G1: molecule ID out of range!\n");
            break;
        case 2:
            throw runtime_error("Error G2: molecule not found!\n");
            break;
        case 3:
            throw runtime_error("Error G3: dataset has no structure data!\n");
            break;
        case 4:
            throw runtime_error("Error G4: dataset has no charge data!\n");
            break;
        case 5:
            throw runtime_error("Error G5: dataset has no topology data!\n");
            break;
        case 6:
            throw runtime_error(
                "Error G6: dataset has no dipole moment data!\n");
            break;
        case 7:
            throw runtime_error(
                "Error G7: this molecule has no charge type definded or \
                                    multiple charge type found\n");
            break;
        default:
            throw invalid_argument(
                "Error G0: invalid error argument received!\n");
            break;
    }
}

Gdata::Gdata() {}

Gdata::Gdata(string name) { this->gd_name = name; }

void Gdata::clear() {
    this->molecule_list.clear();
    this->gd_name = "";
}

Gdata::~Gdata() { this->clear(); }

void Gdata::add_molecule(const Molecule& new_molecule) {
    this->molecule_list.push_back(new_molecule);
}

void Gdata::print() const {
    cout << "summary of Gdata class: " << endl;
    cout << "data number: " << this->get_molecule_num() << endl;
    cout << "max_atom: " << this->get_max_atom() << endl;
    cout << "contained data: ";
    if (this->structure_exist()) cout << "structure ";
    if (this->charge_exist()) cout << "charge ";
    if (this->topo_exist()) cout << "topology ";
    if (this->dipole_exist()) cout << "dipole_moment ";
    cout << endl;
    if (this->charge_exist())
        cout << "charge type: " << this->get_charge_type() << endl;
}

void Gdata::print_molecule(const unsigned long id) const {
    if (!(id < molecule_list.size())) {
        error(1);
    }
    molecule_list[id].print_all();
}

void Gdata::print_molecule(const string name) const {
    bool match = false;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        if (this->molecule_list[i].get_name() == name) {
            this->print_molecule(i);
            match = true;
        }
    }
    if (!match) {
        this->error(2);
    }
}

unsigned long Gdata::get_molecule_num() const {
    return this->molecule_list.size();
}

unsigned long Gdata::get_max_atom() const {
    unsigned long max_atom = 0;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        if (molecule_list[i].get_atom_num() > max_atom) {
            max_atom = molecule_list[i].get_atom_num();
        }
    }
    return max_atom;
}

const vector<vector<short int>> Gdata::get_atom_info() const {
    vector<vector<short int>> output;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(this->molecule_list[i].get_atom_info());
    }
    return output;
}

const vector<vector<vector<float>>> Gdata::get_structure() const {
    if (!this->structure_exist()) {
        this->error(3);
    }
    vector<vector<vector<float>>> output;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(this->molecule_list[i].get_structure());
    }
    return output;
}

const vector<vector<float>> Gdata::get_charge() const {
    if (!this->charge_exist()) {
        this->error(4);
    }
    vector<vector<float>> output;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(this->molecule_list[i].get_charge());
    }
    return output;
}

const vector<string> Gdata::get_name() const {
    vector<string> output;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(this->molecule_list[i].get_name());
    }
    return output;
}

const vector<vector<vector<short int>>> Gdata::get_topology() const {
    if (!this->topo_exist()) {
        this->error(5);
    }

    vector<vector<vector<short int>>> output;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(this->molecule_list[i].get_topology());
    }
    return output;
}

const vector<vector<vector<bool>>> Gdata::get_adjacency() const {
    if (!this->topo_exist()) {
        this->error(5);
    }

    vector<vector<vector<bool>>> output;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(this->molecule_list[i].get_adjacency());
    }
    return output;
}

const vector<Topology> Gdata::get_topology_class() const {
    vector<Topology> output;
    for (unsigned int i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(molecule_list[i].get_topology_class());
    }
    return output;
}

const vector<vector<float>> Gdata::get_dipole() const {
    if (!this->dipole_exist()) {
        this->error(6);
    }

    vector<vector<float>> output;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        output.push_back(this->molecule_list[i].get_dipole());
    }
    return output;
}

const string Gdata::get_gd_name() const { return this->gd_name; }

void Gdata::search_atom(const short atom_id) const {
    vector<vector<short int>> atom_info = this->get_atom_info();
    unsigned long count = 0;
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        for (unsigned long j = 0; j < this->get_max_atom(); j++) {
            if (atom_info[i][j] == atom_id) {
                cout << "atom found in Molecule " << i << " Atom " << j << endl;
                count += 1;
            }
        }
    }
    cout << count << " atom match" << endl;
}

bool Gdata::structure_exist() const {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        if (!molecule_list[i].structure_exist()) {
            return false;
        }
    }
    if (this->get_molecule_num() == 0) {
        return false;
    }
    return true;
}

bool Gdata::charge_exist() const {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        if (!molecule_list[i].charge_exist()) {
            return false;
        }
    }
    if (this->get_molecule_num() == 0) {
        return false;
    }
    return true;
}

bool Gdata::topo_exist() const {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        if (!molecule_list[i].topo_exist()) {
            return false;
        }
    }
    if (this->get_molecule_num() == 0) {
        return false;
    }
    return true;
}

bool Gdata::dipole_exist() const {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        if (!molecule_list[i].dipole_exist()) {
            return false;
        }
    }
    if (this->get_molecule_num() == 0) {
        return false;
    }
    return true;
}

const string Gdata::get_charge_type() const {
    if (!this->charge_exist()) {
        this->error(4);
    }
    string charge_type = molecule_list[0].get_charge_type();
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        if (molecule_list[i].get_charge_type() != charge_type) {
            this->error(7);
        }
    }
    return charge_type;
}

void Gdata::del_structure() {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        molecule_list[i].del_structure();
    }
}

void Gdata::del_charge() {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        molecule_list[i].del_charge();
    }
}

void Gdata::del_dipole() {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        molecule_list[i].del_dipole();
    }
}

void Gdata::del_topology() {
    for (unsigned long i = 0; i < this->get_molecule_num(); i++) {
        molecule_list[i].del_topology();
    }
}

bool Gdata::is_empty() const { return this->get_molecule_num() == 0; }

void Gdata::set_gd_name(string new_name) { this->gd_name = new_name; }

const Gdata Gdata::operator+(const Gdata& gd) const {
    Gdata gd_output(*this);

    unsigned long molecule_num = gd.get_molecule_num();
    for (int i = 0; i < molecule_num; i++) {
        gd_output.add_molecule(gd[i]);
    }

    return gd_output;
}

void Gdata::operator+=(const Gdata& gd) {
    unsigned long molecule_num = gd.get_molecule_num();
    for (int i = 0; i < molecule_num; i++) {
        this->add_molecule(gd[i]);
    }
}

bool Gdata::operator==(const Gdata& gd) const {
    bool name_check = (this->gd_name == gd.gd_name);
    bool molecule_list_check = (this->molecule_list == gd.molecule_list);

    return name_check && molecule_list_check;
}

const Molecule Gdata::operator[](unsigned long id) const {
    return this->molecule_list[id];
}