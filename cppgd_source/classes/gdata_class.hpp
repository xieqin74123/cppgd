#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#ifndef GRAPH_HPP_
#include "../../sup_source/graph.hpp"
#endif

#ifndef GDATA_CLASS_HPP_
#define GDATA_CLASS_HPP_

using namespace std;

class Position {
   public:
    float x = 0;
    float y = 0;
    float z = 0;
    bool set = false;

    bool operator==(const Position& position) const;
};

class Charge {
   public:
    float charge = 0;
    string charge_type = "";
    bool set = false;

    bool operator==(const Charge& charge) const;
};

class Dipole {
   public:
    float x = 0;
    float y = 0;
    float z = 0;
    bool set = false;

    bool operator==(const Dipole& dipole) const;
};

class Atom {
   private:
    Position position;
    short int atom_info = 0;
    Charge charge;

    void error(int error_num) const;

   public:
    // 构造函数
    explicit Atom(const short int atom_info);
    Atom();

    // 析构函数
    ~Atom();

    // 更改原子类型
    void change_atom(const short int atom_into);

    // 添加坐标
    void add_position(const array<float, 3>& position);
    void add_position(const vector<float>& position);
    void add_position(const float x, const float y, const float z);

    // 添加电荷
    void add_charge(const float charge, const string charge_type);
    void add_charge(const Charge charge);

    // 更改电荷类型
    void change_charge_type(const string new_charge_type);

    // 获取电荷类型
    const string get_charge_type() const;

    // 获取坐标
    const vector<float> get_position() const;

    // 获取原子类型
    short get_atom_info() const;

    // 获取电荷
    float get_charge() const;

    // 获取坐标存在
    bool position_exist() const;

    // 获取电荷存在
    bool charge_exist() const;

    // 删除坐标
    void del_position();

    // 删除电荷
    void del_charge();

    // 删除所有
    void clear();

    // overload ==
    bool operator==(const Atom& atom) const;
};

class Bond {
   private:
    unsigned long atom1 = 0;
    unsigned long atom2 = 0;
    short order = 0;

   public:
    // 构造函数
    Bond(unsigned long atom1, unsigned long atom2, short order);
    Bond();

    // 添加连接
    void add_connection(unsigned long atom1, unsigned long atom2);

    // 添加连接种类
    void add_order(short order);

    // 获取连接
    array<unsigned long, 2> get_connection() const;

    // 获取连接种类
    const short get_order() const;

    // overload ==
    bool operator==(const Bond& bond) const;
};

class Topology : public Graph {
   public:
    bool set = false;

    // constructor
    using Graph::Graph;
};

class Molecule {
   private:
    vector<Atom> atom_list;
    vector<Bond> bond_list;
    string name = "";
    Dipole dipole;

    // error output
    void error(int error_num) const;

    // generate topology
    const Topology generate_topology() const;

   public:
    // 构造函数
    explicit Molecule(string name);

    // 析构函数
    ~Molecule();

    // 添加原子
    void add_atom(const Atom& new_atom);

    // 添加偶极矩
    void add_dipole(const Dipole& dipole);
    void add_dipole(const array<float, 3>& dipole);
    void add_dipole(const vector<float>& dipole);
    void add_dipole(const float x, const float y, const float z);

    // 添加键信息
    void add_bond(const unsigned long atom1, const unsigned long atom2,
                  const short order);
    void add_bond(const Bond& bond);
    void add_bond(const vector<Bond>& bond_list);

    // 打印原子信息
    void print_atom() const;

    // 打印结构信息
    void print_structure() const;

    // 打印偶极矩信息
    void print_dipole() const;

    // 打印电荷
    void print_charge() const;

    // print topology
    void print_topology() const;

    // 打印所有信息
    void print_all() const;

    // 获取原子数
    unsigned long get_atom_num() const;

    // 获取键数量
    unsigned long get_bond_num() const;

    // 获取原子（重载[]）
    const Atom operator[](unsigned long id) const;

    // 删除电荷信息
    void del_charge();

    // 删除坐标信息
    void del_structure();

    // 删除偶极矩
    void del_dipole();

    // delete bond/topology
    void del_bond();
    void del_topology();

    // 清空分子
    void clear();

    // 获取原子信息
    const vector<short int> get_atom_info() const;

    // 获取结构信息
    const vector<vector<float>> get_structure() const;

    // 获取电荷信息
    const vector<float> get_charge() const;

    // 获取名字
    const string get_name() const;

    // 获取偶极矩
    const vector<float> get_dipole() const;

    // get bond list
    const vector<Bond> get_bond_list() const;

    // get bond
    const Bond get_bond(unsigned long atom1, unsigned long atom2) const;

    // get bond order
    const short get_bond_order(unsigned long atom1, unsigned long atom2) const;
    const short get_bond_order(array<unsigned long, 2> connection) const;

    // 检查结构存在
    bool structure_exist() const;

    // 检查电荷存在
    bool charge_exist() const;

    // 检查偶极矩存在
    bool dipole_exist() const;

    // check bond/topology exist
    bool bond_exist() const;
    bool topo_exist() const;

    // 获取电荷类型
    const string get_charge_type() const;

    // get topology
    const vector<vector<short>> get_topology() const;

    // get adjacency
    const vector<vector<bool>> get_adjacency() const;

    // get topology class
    const Topology get_topology_class() const;

    // overload ==
    bool operator==(const Molecule& molecule) const;
};

class Gdata {
   private:
    string gd_name = "";
    vector<Molecule> molecule_list;

    void error(int error_num) const;

   public:
    // 构造函数
    Gdata();
    explicit Gdata(string name);

    ~Gdata();

    void clear();

    // 添加分子
    void add_molecule(const Molecule& new_molecule);

    // 打印信息
    void print() const;

    // 打印指定分子信息
    void print_molecule(const unsigned long id) const;
    void print_molecule(const string name) const;

    // 获取数据数量
    unsigned long get_molecule_num() const;

    // 获取max_atom
    unsigned long get_max_atom() const;

    // 获取原子类型
    const vector<vector<short int>> get_atom_info() const;

    // 获取xyz格式结构信息
    const vector<vector<vector<float>>> get_structure() const;

    // 获取电荷信息
    const vector<vector<float>> get_charge() const;

    // 获取名字信息
    const vector<string> get_name() const;

    // 获取拓扑信息
    const vector<vector<vector<short int>>> get_topology() const;

    // get adjacency
    const vector<vector<vector<bool>>> get_adjacency() const;

    // get topology class
    const vector<Topology> get_topology_class() const;

    // 获取偶极矩信息
    const vector<vector<float>> get_dipole() const;

    // 获取数据集名字
    const string get_gd_name() const;

    // 搜索数据
    void search_atom(const short atom_id) const;

    // 获取结构存在
    bool structure_exist() const;

    // 获取电荷存在
    bool charge_exist() const;

    // 获取拓扑存在
    bool topo_exist() const;

    // 获取偶极矩存在
    bool dipole_exist() const;

    // 获取电荷类型
    const string get_charge_type() const;

    // 删除结构信息
    void del_structure();

    // 删除电荷信息
    void del_charge();

    // 删除偶极矩信息
    void del_dipole();

    // delete topology
    void del_topology();

    // 数据为空检查
    bool is_empty() const;

    // 设置名字
    void set_gd_name(string new_name);

    // 重载+为拼接
    const Gdata operator+(const Gdata& gd) const;

    // 重载+=为自拼接
    void operator+=(const Gdata& gd);

    // 重载==为判断
    bool operator==(const Gdata& gd) const;

    // 重载[]
    const Molecule operator[](unsigned long id) const;
};

#endif