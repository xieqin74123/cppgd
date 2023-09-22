#ifndef GDATA_CLASS_HPP_
#include "gdata_class.hpp"
#endif

void Molecule::error(int error_num) const {
    switch (error_num) {
        case 1:
            throw runtime_error(
                "Error M1: this molecule has no bonding data!\n");
            break;
        case 2:
            throw runtime_error(
                "Error M2: the index of bonded atom is out of range!\n");
            break;
        case 3:
            throw runtime_error(
                "Error M3: cannot find bonded atom in this molecule!\n");
            break;
        case 4:
            throw runtime_error(
                "Error M4: this moelcule has no structure data!\n");
            break;
        case 5:
            throw runtime_error(
                "Error M5: this molecule has no charge data!\n");
            break;
        case 6:
            throw runtime_error(
                "Error M6: this molecule has no dipole moment data!\n");
            break;
        case 7:
            throw runtime_error(
                "Error M7: this molecule has no charge type definded or \
                                    multiple charge type found\n");
            break;
        case 8:
            throw runtime_error(
                "Error M8: added dipole moment vector must has size of 3!\n");
            break;
        case 9:
            throw runtime_error(
                "Error M9: bond with specific connection not found!\n");
            break;
        default:
            throw invalid_argument(
                "Error M0: invalid error argument received!\n");
            break;
    }
}

const Topology Molecule::generate_topology() const {
    if (!this->bond_exist()) {
        this->error(1);
    }
    Topology topology;
    topology.reset(this->get_atom_num());
    for (unsigned long i = 0; i < this->get_bond_num(); i++) {
        auto connection = this->bond_list[i].get_connection();
        unsigned long atom1 = connection[0];
        unsigned long atom2 = connection[1];

        topology.add(atom1, atom2, bond_list[i].get_order());
    }

    return topology;
}

Molecule::Molecule(string name) { this->name = name; }

Molecule::~Molecule() { this->clear(); }

void Molecule::add_atom(const Atom &new_atom) {
    this->atom_list.push_back(new_atom);
}

void Molecule::add_dipole(const Dipole &dipole) {
    this->add_dipole(dipole.x, dipole.y, dipole.z);
}

void Molecule::add_dipole(const array<float, 3> &dipole) {
    this->add_dipole(dipole[0], dipole[1], dipole[2]);
}

void Molecule::add_dipole(const vector<float> &dipole) {
    if (dipole.size() != 3) {
        this->error(8);
    }
    this->add_dipole(dipole[0], dipole[1], dipole[2]);
}

void Molecule::add_dipole(const float x, const float y, const float z) {
    this->dipole.x = x;
    this->dipole.y = y;
    this->dipole.z = z;
    this->dipole.set = true;
}

void Molecule::add_bond(const unsigned long atom1, const unsigned long atom2,
                        const short order) {
    Bond bond(atom1, atom2, order);
    this->add_bond(bond);
}

void Molecule::add_bond(const Bond &bond) {
    if (bond.get_connection()[0] >= this->atom_list.size() ||
        bond.get_connection()[1] >= this->atom_list.size()) {
        this->error(2);
    }
    this->bond_list.push_back(bond);
}

void Molecule::add_bond(const vector<Bond> &bond_list) {
    for (unsigned long i = 0; i < bond_list.size(); i++) {
        this->add_bond(bond_list[i]);
    }
}

void Molecule::print_atom() const {
    cout << this->get_atom_num() << " atoms in this molecule" << endl;
    cout << "Atom list:" << endl;
    for (unsigned long i = 0; i < get_atom_num(); i++) {
        cout << atom_list[i].get_atom_info() << " ";
    }
    cout << endl;
}

void Molecule::print_structure() const {
    if (!this->structure_exist()) {
        this->error(4);
    }

    cout << "Cartesian coordinate:" << endl;
    vector<float> position;
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        position = atom_list[i].get_position();
        cout << atom_list[i].get_atom_info() << " ";

        for (unsigned long j = 0; j < 3; j++) {
            cout << position[j] << " ";
        }
        cout << endl;
    }
}

void Molecule::print_dipole() const {
    if (!this->dipole_exist()) {
        this->error(6);
    }
    cout << "Dipole moment:" << endl;
    cout << this->dipole.x << this->dipole.y << this->dipole.z << "\t";
    cout << endl;
}

void Molecule::print_charge() const {
    if (!this->charge_exist()) {
        this->error(5);
    }
    cout << "Charge:" << endl;
    cout << "type: " << this->get_charge_type() << endl;
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        cout << atom_list[i].get_atom_info() << "\t"
             << atom_list[i].get_charge() << endl;
    }
}

void Molecule::print_topology() const {
    auto topology = this->generate_topology();
    topology.print();
}

void Molecule::print_all() const {
    cout << this->name << endl;
    this->print_atom();
    if (this->structure_exist()) {
        this->print_structure();
    }
    if (this->charge_exist()) {
        this->print_charge();
    }
    if (this->dipole_exist()) {
        this->print_dipole();
    }
    if (this->bond_exist()) {
        this->print_topology();
    }
}

unsigned long Molecule::get_atom_num() const { return this->atom_list.size(); }

const Atom Molecule::operator[](unsigned long id) const {
    return this->atom_list[id];
}

void Molecule::del_charge() {
    for (int i = 0; i < this->get_atom_num(); i++) {
        atom_list[i].del_charge();
    }
}

void Molecule::del_structure() {
    for (int i = 0; i < this->get_atom_num(); i++) {
        atom_list[i].del_position();
    }
}

void Molecule::del_dipole() {
    if (this->dipole_exist()) {
        this->dipole.x = 0;
        this->dipole.y = 0;
        this->dipole.z = 0;
        this->dipole.set = false;
    }
}

void Molecule::del_bond() {
    if (this->bond_exist()) {
        bond_list.clear();
    }
}

void Molecule::del_topology() { this->del_bond(); }

void Molecule::clear() {
    // 清空原子
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        this->atom_list[i].clear();
    }
    this->atom_list.clear();

    // 清空名字
    this->name = "";

    // 清空偶极矩信息
    this->del_dipole();
}

const vector<short int> Molecule::get_atom_info() const {
    vector<short int> output;
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        output.push_back(this->atom_list[i].get_atom_info());
    }
    return output;
}

const vector<vector<float> > Molecule::get_structure() const {
    if (!this->structure_exist()) {
        this->error(4);
    }

    vector<vector<float> > output;
    vector<float> pos_temp;
    vector<float> output_temp;

    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        pos_temp = this->atom_list[i].get_position();

        for (int j = 0; j < 3; j++) {
            output_temp.push_back(pos_temp[j]);
        }

        output.push_back(output_temp);

        fill_n(pos_temp.begin(), 3, 0);
        output_temp.clear();
    }
    return output;
}

const vector<float> Molecule::get_charge() const {
    if (!this->charge_exist()) {
        this->error(5);
    }

    vector<float> output;
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        output.push_back(this->atom_list[i].get_charge());
    }
    return output;
}

const string Molecule::get_name() const { return this->name; }

const vector<float> Molecule::get_dipole() const {
    if (!this->dipole_exist()) {
        this->error(6);
    }
    vector<float> output;
    output.push_back(dipole.x);
    output.push_back(dipole.y);
    output.push_back(dipole.z);
    return output;
}

const vector<Bond> Molecule::get_bond_list() const { return this->bond_list; }

unsigned long Molecule::get_bond_num() const { return this->bond_list.size(); }

const Bond Molecule::get_bond(unsigned long atom1, unsigned long atom2) const {
    array<unsigned long, 2> connection;
    array<unsigned long, 2> connection_rev;
    connection[0] = atom1;
    connection[1] = atom2;
    connection_rev[0] = atom2;
    connection_rev[1] = atom1;

    for (unsigned long i = 0; i < this->get_bond_num(); i++) {
        if (this->bond_list[i].get_connection() == connection ||
            this->bond_list[i].get_connection() == connection_rev)
            return this->bond_list[i];
    }

    Bond bond;
    return bond;
}

const short Molecule::get_bond_order(unsigned long atom1,
                                     unsigned long atom2) const {
    return this->get_bond(atom1, atom2).get_order();
}

const short Molecule::get_bond_order(array<unsigned long, 2> connection) const {
    return this->get_bond(connection[0], connection[1]).get_order();
}

bool Molecule::structure_exist() const {
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        if (!atom_list[i].position_exist()) {
            return false;
        }
    }
    if (this->get_atom_num() == 0) {
        return false;
    }
    return true;
}

bool Molecule::charge_exist() const {
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        if (!atom_list[i].charge_exist()) {
            return false;
        }
    }
    if (this->get_atom_num() == 0) {
        return false;
    }
    if (this->get_atom_num() == 0) {
        return false;
    }
    return true;
}

bool Molecule::dipole_exist() const { return this->dipole.set; }

bool Molecule::bond_exist() const {
    if (this->bond_list.size() == 0) {
        return false;
    }
    return true;
}

bool Molecule::topo_exist() const { return this->bond_exist(); }

const string Molecule::get_charge_type() const {
    if (!this->charge_exist()) {
        this->error(5);
    }
    string charge_type = atom_list[0].get_charge_type();
    for (unsigned long i = 0; i < this->get_atom_num(); i++) {
        if (atom_list[i].get_charge_type() != charge_type) {
            this->error(7);
        }
    }
    return charge_type;
}

const vector<vector<short> > Molecule::get_topology() const {
    auto topology = this->generate_topology();
    return topology.get_raw_data();
}

const vector<vector<bool> > Molecule::get_adjacency() const {
    auto topology = this->generate_topology().get_raw_data();
    vector<vector<bool> > adjacency;
    for (unsigned long i = 0; i < topology.size(); i++) {
        vector<bool> temp_adj_row;
        for (unsigned long j = 0; j < topology[i].size(); j++) {
            if (topology[i][j] == 0) {
                temp_adj_row.push_back(0);
            } else {
                temp_adj_row.push_back(1);
            }
        }
        adjacency.push_back(temp_adj_row);
    }
    return adjacency;
}

const Topology Molecule::get_topology_class() const {
    auto topology = generate_topology();
    return topology;
}

bool Molecule::operator==(const Molecule &molecule) const {
    bool atom_list_check = (this->atom_list == molecule.atom_list);
    bool bond_list_check = (this->bond_list == molecule.bond_list);
    bool name_check = (this->name == molecule.name);
    bool dipole_check = (this->dipole == molecule.dipole);
    return atom_list_check && bond_list_check && name_check && dipole_check;
}