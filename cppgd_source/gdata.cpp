#include <dirent.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#ifndef SUP_FUINCTIONS_HPP_
#include "../sup_source/sup_functions.hpp"
#endif
#ifndef GDATA_CLASS_HPP_
#include "classes/gdata_class.hpp"
#endif
#ifndef DICTIONARY_HPP_
#include "dictionary.hpp"
#endif
#ifndef NPY_HPP_
#include "../sup_source/npy.hpp"
#endif

using namespace std;

// 字典
static Dictionary dictionary;

namespace gdata {

// 保存为xyz格式
void save_as_xyz(string path, const Gdata& data) {
    cout << "saving..." << endl;

    // 检查 "/"
    if (path[path.size() - 1] != '/') {
        path.push_back('/');
    }

    for (unsigned long molecule = 0; molecule < data.get_molecule_num();
         molecule++) {
        string name_temp;
        if (data[molecule].get_name() != "") {
            name_temp = data[molecule].get_name() + ".xyz";
        } else {
            name_temp = to_string(molecule) + ".xyz";
        }

        ofstream output_temp;
        output_temp.open(path + name_temp, fstream::out);
        output_temp.setf(ios::fixed, ios::floatfield);
        output_temp << data[molecule].get_atom_num() << endl << endl;

        for (unsigned long atom = 0; atom < data[molecule].get_atom_num();
             atom++) {
            output_temp << dictionary.element_dict(
                data[molecule][atom].get_atom_info());
            for (int i = 0; i < 3; i++) {
                output_temp << " " << data[molecule][atom].get_position()[i];
            }
            output_temp << endl;
        }

        output_temp.close();
    }

    cout << "done!       " << endl;
}

// 保存数据为npy格式
void save_as_npy(string path, Gdata& data, bool save_config = true) {
    // unify vector shape

    cout << "saving..." << endl;

    // 检查 "/"
    if (path[path.size() - 1] != '/') {
        path.push_back('/');
    }

    string name_temp;

    // 保存原子信息
    {
        name_temp = path + "atom_info.npy";
        auto data_temp = data.get_atom_info();
        array<long unsigned, 2> shape;
        shape[0] = data.get_molecule_num();
        shape[1] = data.get_max_atom();
        data_temp = sup::vec_pad<short>(data_temp);
        npy::SaveArrayAsNumpy(name_temp, false, shape.size(), shape.data(),
                              sup::vec_flat<short>(data_temp));
    }

    // 保存结构
    if (data.structure_exist()) {
        name_temp = path + "structure.npy";
        {
            auto data_temp = data.get_structure();
            array<long unsigned, 3> shape;
            shape[0] = data.get_molecule_num();
            shape[1] = data.get_max_atom();
            shape[2] = 3;
            data_temp = sup::vec_pad<float>(data_temp);
            npy::SaveArrayAsNumpy(name_temp, false, shape.size(), shape.data(),
                                  sup::vec_flat<float>(data_temp));
        }
    }

    // 保存电荷
    if (data.charge_exist()) {
        name_temp = path + "charge.npy";
        {
            auto data_temp = data.get_charge();
            array<long unsigned, 2> shape;
            shape[0] = data.get_molecule_num();
            shape[1] = data.get_max_atom();
            data_temp = sup::vec_pad<float>(data_temp);
            npy::SaveArrayAsNumpy(name_temp, false, shape.size(), shape.data(),
                                  sup::vec_flat<float>(data_temp));
        }
    }
    // 保存名字
    name_temp = path + "name.txt";
    {
        ofstream namefile;
        auto data_temp = data.get_name();

        namefile.open(name_temp);

        if (!namefile.good()) {
            throw runtime_error("fail to create file!\n");
        }

        for (unsigned long i = 0; i < data_temp.size(); i++) {
            namefile << data_temp[i] << endl;
        }

        namefile.close();
    }

    // 保存拓扑信息
    if (data.topo_exist()) {
        name_temp = path + "topology.npy";
        {
            auto data_temp = data.get_topology();
            array<long unsigned, 3> shape;
            shape[0] = data.get_molecule_num();
            shape[1] = data.get_max_atom();
            shape[2] = data.get_max_atom();
            data_temp = sup::vec_pad<short>(data_temp);
            npy::SaveArrayAsNumpy(name_temp, false, shape.size(), shape.data(),
                                  sup::vec_flat<short int>(data_temp));
        }
    }

    // 保存偶极矩信息
    if (data.dipole_exist()) {
        name_temp = path + "dipole.npy";
        {
            auto data_temp = data.get_dipole();
            array<long unsigned, 2> shape;
            shape[0] = data.get_molecule_num();
            shape[1] = 3;
            data_temp = sup::vec_pad<float>(data_temp);
            npy::SaveArrayAsNumpy(name_temp, false, shape.size(), shape.data(),
                                  sup::vec_flat<float>(data_temp));
        }
    }

    // 保存配置文件
    if (save_config) {
        name_temp = path + "config.conf";
        {
            ofstream conffile;
            conffile.open(name_temp);
            if (!conffile.good()) {
                throw runtime_error("fail to create file!\n");
            }
            /* 配置文件结构：[data_num, max_atom, charge_type] */
            conffile << data.get_molecule_num() << endl
                     << data.get_max_atom() << endl
                     << data.get_charge_type() << endl;

            conffile.close();
        }
    }

    cout << "done!       " << endl;
}

// 读取chargemol output 文件
const Gdata read_chargemol_file(const string path) {
    ifstream in_file;
    in_file.open(path);
    if (!in_file.good()) {
        cerr << "fail to open " << path << endl;
        throw runtime_error("fail to open file!\n");
    }

    string word;
    string line;
    Gdata gd_output;

    Atom* atom = NULL;
    Molecule* molecule = NULL;

    unsigned long long start_mark = 0;

    while (!in_file.eof()) {
        getline(in_file, line);

        if (line.find("Multipole analysis for each of the expansion sites.") !=
            string::npos) {
            start_mark = in_file.tellg();
        }
    }

    in_file.clear();
    in_file.seekg(start_mark, ios::beg);

    molecule = new Molecule(sup::get_real_name(path));
    if (molecule == NULL) {
        throw runtime_error("fail to allocate memory for new molecule!\n");
    }

    // 跳过两行
    getline(in_file, line);
    getline(in_file, line);
    while (!in_file.eof()) {
        getline(in_file, line);

        if (line == "  ") {
            break;
            ;
        }

        stringstream ss(line);
        // 跳过序数
        ss >> word;
        // 读取原子序数
        ss >> word;
        atom = new Atom(stoi(word));
        if (atom == NULL) {
            throw runtime_error("fail to allocate memory for new atom!\n");
        }
        // 读取坐标
        array<float, 3> position;
        // x
        ss >> word;
        position[0] = stof(word);
        // y
        ss >> word;
        position[1] = stof(word);
        // z
        ss >> word;
        position[2] = stof(word);

        atom->add_position(position);

        // 读取电荷
        ss >> word;
        atom->add_charge(stof(word), "DDEC");

        molecule->add_atom(*atom);
        delete atom;
        atom = NULL;
    }
    gd_output.add_molecule(*molecule);

    delete molecule;
    molecule = NULL;
    return gd_output;
}

// 读取多个chargemol output文件
const Gdata read_chargemol_file(const vector<string> path) {
    Gdata gd_output;
    cout << "start reading chargemol output files ..." << endl;
    const int file_num = path.size();

    for (int i = 0; i < file_num; i++) {
        string path_temp = path[i];

        cout << i + 1 << " out of " << file_num << "\r";

        Gdata gd_temp = read_chargemol_file(path_temp);

        gd_output += gd_temp;
    }

    cout << "done!                                            " << endl;
    return gd_output;
}

// 读取chargemol output 文件夹
const Gdata read_chargemol_dir(string path) {
    vector<string> file_list;

    // check "/"
    if (path[path.size() - 1] != '/') {
        path.push_back('/');
    }

    DIR* dir = opendir(path.c_str());
    struct dirent* ent;

    if (dir == NULL) {
        throw runtime_error("fail to open directory!");
    }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            file_list.push_back(path + ent->d_name);
        }
    }

    Gdata gd_output = read_chargemol_file(file_list);

    return gd_output;
}

// 读取mol2文件
const Gdata read_mol2_file(const string path) {
    ifstream in_file;
    in_file.open(path);
    if (!in_file.good()) {
        cerr << "fail to open " << path << endl;
        throw runtime_error("fail to open file!\n");
    }

    string word;
    string line;

    Gdata gd_output;

    Atom* atom = NULL;
    Molecule* molecule = NULL;

    // 跳过错误用
    bool skip_molecule = false;

    // 读取原子用
    float charge = 0;
    short atom_type = 0;
    int dot_pos = 0;

    // 读取分子用
    unsigned long atom_num = 0;
    unsigned long bond_num = 0;

    // 读取键连接用
    unsigned long atom_1 = 0;
    unsigned long atom_2 = 0;
    short bond = 0;

    // 读取文档长度
    in_file.seekg(0, ios::end);
    // double file_len = in_file.tellg();
    unsigned long long cur_pos = 0;
    in_file.seekg(0, ios::beg);

    while (!in_file.eof()) {
        getline(in_file, line);

        // 找到分子开头
        if (line.find("@<TRIPOS>MOLECULE") != string::npos) {
            cur_pos = in_file.tellg();

            // 找到分子名称，并分配分子内存
            getline(in_file, line);
            molecule = new Molecule(sup::get_real_name(line));
            if (molecule == NULL) {
                throw runtime_error(
                    "fail to allocate memory for new molecule!\n");
            }

            // 找到原子数键链接数并转换为int
            in_file >> word;
            atom_num = stoul(word);
            in_file >> word;
            bond_num = stoul(word);

            // 跳过数行直至原子开始
            while (!in_file.eof()) {
                getline(in_file, line);
                if (line.find("@<TRIPOS>ATOM") != string::npos) {
                    break;
                }
            }

            // 读取原子
            for (int i = 0; i < atom_num; i++) {
                // 跳过前2个数据
                getline(in_file, line);
                stringstream ss(line);

                ss >> word;
                ss >> word;

                array<float, 3> position = {0, 0, 0};

                // 读取坐标
                ss >> word;  // x
                position[0] = stof(word);
                ss >> word;  // y
                position[1] = stof(word);
                ss >> word;  // z
                position[2] = stof(word);

                // 读取原子种类并建立Atom类
                ss >> word;

                dot_pos = word.find(".");
                if (dot_pos == string::npos) {
                    atom_type = dictionary.element_dict(word);
                } else {
                    atom_type = dictionary.element_dict(
                        sup::get_in_range(word, 0, dot_pos));
                }

                // atom不匹配
                if (atom_type == -1) {
                    skip_molecule = true;
                    break;
                }

                atom = new Atom(atom_type);
                if (atom == NULL) {
                    throw runtime_error(
                        "fail to allocate memory for new atom!\n");
                }

                // 添加坐标
                atom->add_position(position);

                // 跳到最后一个读取电荷
                while (ss >> word)
                    ;
                if (!sup::str_is_number(word)) {
                    skip_molecule = true;
                    cout << "invalid charge received!" << endl;

                    // 释放原子
                    if (atom != NULL) {
                        atom->clear();
                        delete atom;
                        atom = NULL;
                    }

                    break;
                }
                charge = stof(word);
                atom->add_charge(charge, "Unknown");

                // 添加进分子
                molecule->add_atom(*atom);

                // 释放原子
                if (atom != NULL) {
                    atom->clear();
                    delete atom;
                    atom = NULL;
                }
            }

            // 检查并跳过错误数据
            if (!skip_molecule) {
                // 跳过数行直至键链接开头
                while (!in_file.eof()) {
                    getline(in_file, line);
                    if (line.find("@<TRIPOS>BOND") != string::npos) {
                        break;
                    }
                }

                // 读取键链接
                for (unsigned long i = 0; i < bond_num; i++) {
                    // 跳过序数
                    in_file >> word;
                    // 读取原子链接
                    in_file >> word;
                    atom_1 = stoi(word) - 1;
                    in_file >> word;
                    atom_2 = stoi(word) - 1;
                    in_file >> word;
                    bond = dictionary.bond_dict(word);
                    if (bond == -1) {
                        skip_molecule = true;
                        break;
                    }

                    // 添加键链接
                    if (!skip_molecule) {
                        molecule->add_bond(atom_1, atom_2, bond);
                    }
                }
                gd_output.add_molecule(*molecule);
            }
            if (skip_molecule) {
                skip_molecule = false;
                cout << "warning: error found in molecule "
                     << molecule->get_name() << ". skipped! ";
                cout << "position: " << cur_pos << endl;
            }

            if (atom != NULL) {
                atom->clear();
                delete atom;
                atom = NULL;
            }

            if (molecule != NULL) {
                molecule->clear();
                delete molecule;
                molecule = NULL;
            }
        }

        // cur_pos = in_file.tellg();
    }

    if (atom != NULL) {
        delete atom;
        atom = NULL;
    }

    if (molecule != NULL) {
        delete molecule;
        molecule = NULL;
    }
    in_file.close();

    return gd_output;
}

// 读取多个.mol2文件
const Gdata read_mol2_file(const vector<string> path) {
    Gdata gd_output;
    cout << "start reading mol2 files ..." << endl;
    const int file_num = path.size();

    for (int i = 0; i < file_num; i++) {
        string path_temp = path[i];

        cout << i + 1 << " out of " << file_num << "\r";

        Gdata gd_temp = read_mol2_file(path_temp);

        gd_output += gd_temp;
    }

    cout << "done!                                            " << endl;
    return gd_output;
}

// 读取mol2文件夹
const Gdata read_mol2_dir(string path) {
    vector<string> file_list;

    // check "/"
    if (path[path.size() - 1] != '/') {
        path.push_back('/');
    }

    DIR* dir = opendir(path.c_str());
    struct dirent* ent;

    if (dir == NULL) {
        throw runtime_error("fail to open directory!");
    }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            file_list.push_back(path + ent->d_name);
        }
    }

    Gdata gd_output = read_mol2_file(file_list);

    return gd_output;
}

// 读取Gaussian log文件
const Gdata read_gaussian_log_file(const string path,
                                   const string charge_type) {
    Gdata gd_output;
    ifstream in_file;

    in_file.open(path);
    if (!in_file.good()) {
        cerr << "fail to open " << path << endl;
        throw runtime_error("fail to open file!\n");
    }

    unsigned long long mulliken_start = 0;
    unsigned long long hirshfeld_start = 0;
    unsigned long long structure_start = 0;

    string line, word;

    // 定位数据位置
    while (!in_file.eof()) {
        getline(in_file, line);
        if (line.find("Mulliken charges:") != string::npos ||
            line.find("Mulliken charges and spin densities:") != string::npos) {
            // 跳过一行
            getline(in_file, line);
            mulliken_start = in_file.tellg();
        } else if (line.find("Hirshfeld charges, spin densities, dipoles, and "
                             "CM5 charges") != string::npos) {
            // 跳过一行
            getline(in_file, line);
            hirshfeld_start = in_file.tellg();
        } else if (line.find("Standard orientation:") != string::npos) {
            // 跳过4行
            getline(in_file, line);
            getline(in_file, line);
            getline(in_file, line);
            getline(in_file, line);
            structure_start = in_file.tellg();
        }
    }
    // 数据读取

    Molecule molecule(sup::get_real_name(path));
    Atom* atom;

    unsigned long long structure_cur_pos = 0;
    unsigned long long charge_cur_pos = 0;

    short int atom_info = 0;
    array<float, 3> position = {0, 0, 0};
    float charge = 0;

    // 移动指针到structure起始位置
    in_file.clear();
    in_file.seekg(structure_start, ios::beg);

    if (charge_type != "") {
        if (charge_type == "Mulliken") {
            charge_cur_pos = mulliken_start;
        } else if (charge_type == "Hirshfeld" || charge_type == "CM5") {
            charge_cur_pos = hirshfeld_start;
        } else {
            cout << "Unsupported charge type, charge will be assigned as 0"
                 << endl;
        }
    }

    while (!in_file.eof()) {
        getline(in_file, line);

        if (line.find("---------------") != string::npos) {
            break;
        }

        stringstream ss(line);
        ss >> word;

        // 原子序数
        ss >> word;
        atom_info = stoi(word);

        ss >> word;

        // x
        ss >> word;
        position[0] = stof(word);
        // y
        ss >> word;
        position[1] = stof(word);
        // z
        ss >> word;
        position[2] = stof(word);

        atom = new Atom(atom_info);
        if (atom == NULL) {
            throw runtime_error("fail to allocate memory for new atom!\n");
        }

        atom->add_position(position);

        // 读取电荷
        if (charge_type != "") {
            // 记录位置并移动到电荷读取位
            structure_cur_pos = in_file.tellg();
            in_file.seekg(charge_cur_pos, ios::beg);

            if (charge_type == "Mulliken") {
                getline(in_file, line);
                stringstream cs(line);

                cs >> word;
                cs >> word;

                // 电荷
                cs >> word;
                charge = stof(word);
            }

            else if (charge_type == "Hirshfeld") {
                getline(in_file, line);
                stringstream cs(line);
                cs >> word;
                cs >> word;

                // 电荷
                cs >> word;
                charge = stof(word);
            }

            else if (charge_type == "CM5") {
                getline(in_file, line);
                stringstream cs(line);
                while (cs >> word)
                    ;
                charge = stof(word);
            }

            // unknown charge type, assigned 0
            else {
                charge = 0;
            }

            atom->add_charge(charge, charge_type);

            // 记录位置并移动到结构读取位
            charge_cur_pos = in_file.tellg();
            in_file.seekg(structure_cur_pos, ios::beg);
        }

        molecule.add_atom(*atom);
        // set position to 0,0,0
        fill_n(position.begin(), 3, 0);
        delete atom;
        atom = NULL;
    }

    in_file.close();

    gd_output.add_molecule(molecule);

    return gd_output;
}

const Gdata read_gaussian_log_file(const vector<string> path_list,
                                   const string charge_type) {
    Gdata gd_output;
    Gdata gd_temp;

    cout << "start reading gaussian log files ..." << endl;

    unsigned long file_num = path_list.size();

    for (unsigned long i = 0; i < file_num; i++) {
        gd_temp = read_gaussian_log_file(path_list[i], charge_type);
        cout << i + 1 << " out of " << file_num << "\r";
        gd_output += gd_temp;
    }

    cout << "done!                                             " << endl;
    return gd_output;
}

const Gdata read_gaussian_log_dir(string path, const string charge_type) {
    vector<string> file_list;

    // check "/"
    if (path[path.size() - 1] != '/') {
        path.push_back('/');
    }

    DIR* dir = opendir(path.c_str());
    struct dirent* ent;

    if (dir == NULL) {
        throw runtime_error("fail to open directory!\n");
    }

    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type == DT_REG) {
            file_list.push_back(path + ent->d_name);
        }
    }

    Gdata gd_output = read_gaussian_log_file(file_list, charge_type);

    return gd_output;
}

const Gdata merge(const Gdata& gd1, const Gdata& gd2) {
    Gdata gd_output;

    auto gd1_name = gd1.get_name();
    auto gd2_name = gd2.get_name();

    cout << "start merging ..." << endl;
    cout << "building match table ... \r";

    // 生成对照表
    vector<unsigned long> match_table;

    for (unsigned long i = 0; i < gd1.get_molecule_num(); i++) {
        auto it = find(gd2_name.begin(), gd2_name.end(), gd1_name[i]);
        if (it != gd2_name.end()) {
            match_table.push_back(distance(gd2_name.begin(), it));
        } else
            match_table.push_back(-1);
    }

    // 生成剩余表
    vector<unsigned long> rest_table;

    for (unsigned long i = 0; i < gd2_name.size(); i++) {
        if (find(match_table.begin(), match_table.end(), i) ==
            match_table.end()) {
            rest_table.push_back(i);
        }
    }

    // 合并
    cout << "merging class ...               \r";
    for (unsigned long i1 = 0; i1 < match_table.size(); i1++) {
        auto i2 = match_table[i1];
        double table_size = match_table.size();

        if (i2 == -1) {
            gd_output.add_molecule(gd1[i1]);
            cout << "merging class ... " << setprecision(2) << fixed
                 << i1 / table_size * 100 << "%\r";
            continue;
        }

        Molecule molecule(gd1_name[i1]);

        for (unsigned long j = 0; j < gd1[i1].get_atom_num(); j++) {
            Atom atom(gd1[i1][j].get_atom_info());

            // 合并position
            if (gd1.structure_exist() ^ gd2.structure_exist()) {
                if (gd1.structure_exist())
                    atom.add_position(gd1[i1][j].get_position());
                else
                    atom.add_position(gd2[i2][j].get_position());
            } else if (gd1.structure_exist()) {
                if (gd1[i1][j].get_position() == gd2[i2][j].get_position())
                    atom.add_position(gd1[i1][j].get_position());
                else {
                    cerr << "error found in molecule " << gd1[i1].get_name()
                         << " atom " << j << endl;
                    throw runtime_error("position value not match!\n");
                }
            }

            // 合并charge
            if (gd1.charge_exist() ^ gd2.charge_exist()) {
                if (gd1.charge_exist())
                    atom.add_charge(gd1[i1][j].get_charge(),
                                    gd1[i1][j].get_charge_type());
                else
                    atom.add_charge(gd2[i2][j].get_charge(),
                                    gd2[i2][j].get_charge_type());
            } else if (gd1.charge_exist()) {
                if (gd1[i1][j].get_charge() == gd2[i2][j].get_charge()) {
                    if (gd1[i1][j].get_charge_type() ==
                        gd2[i2][j].get_charge_type())
                        atom.add_charge(gd1[i1][j].get_charge(),
                                        gd1[i1][j].get_charge_type());
                    else {
                        cerr << "error found in molecule " << gd1[i1].get_name()
                             << " atom " << j << endl;
                        throw runtime_error("charge type not match!\n");
                    }
                } else {
                    cerr << "error found in molecule " << gd1[i1].get_name()
                         << " atom " << j << endl;
                    throw runtime_error("charge value not match!\n");
                }
            }

            molecule.add_atom(atom);
        }

        // 合并topology/bond
        if (gd1.topo_exist() ^ gd2.topo_exist()) {
            if (gd1.topo_exist()) {
                molecule.add_bond(gd1[i1].get_bond_list());
            } else {
                molecule.add_bond(gd2[i2].get_bond_list());
            }
        } else if (gd1.topo_exist()) {
            if (gd1[i1].get_bond_list() == gd2[i2].get_bond_list()) {
                molecule.add_bond(gd1[i1].get_bond_list());
            } else {
                cerr << "error found in molecule " << gd1[i1].get_name()
                     << endl;
                throw runtime_error("bond list not match!");
            }
        }

        // 合并dipole
        if (gd1.dipole_exist() ^ gd2.dipole_exist()) {
            if (gd1.dipole_exist())
                molecule.add_dipole(gd1[i1].get_dipole());
            else
                molecule.add_dipole(gd2[i2].get_dipole());
        } else if (gd1.dipole_exist()) {
            if (gd1[i1].get_dipole() == gd2[i2].get_dipole())
                molecule.add_dipole(gd1[i1].get_dipole());
            else {
                cerr << "error found in molecule " << gd1[i1].get_name()
                     << endl;
                throw runtime_error("dipole not match!");
            }
        }

        gd_output.add_molecule(molecule);

        cout << "merging class ... " << setprecision(2) << fixed
             << i1 / table_size * 100 << "%\r";
    }

    // 添加gd2剩余元素
    for (unsigned long i = 0; i < rest_table.size(); i++) {
        gd_output.add_molecule(gd2[rest_table[i]]);
    }

    cout << "done!                                            " << endl;
    return gd_output;
}

}  // namespace gdata
