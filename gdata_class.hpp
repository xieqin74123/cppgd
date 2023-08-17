#include <iostream>
#include <vector>

using namespace std;

class Atom {
    private:
        vector<float> position;
        short int atom_info = 0;
        float charge = 0;
        string charge_type = "";
        bool charge_set = false;
        bool position_set = false;
    public:
        // 构造函数
        Atom (const short int atom_info) {
            this->atom_info = atom_info;
        }

        Atom () {

        }

        // 析构函数
        ~Atom () {
            this->clear();
        }

        // 更改原子类型
        void change_atom (const short int atom_into) {
            this->atom_info = atom_info;
        }

        // 添加坐标
        void add_position (const vector<float> position) {
            if (position.size() != 3) {
                throw invalid_argument("position coordinate is not 3 dimensional!");
            }
            this->position = position;
            this->position_set = true;
        }

        // 添加电荷
        void add_charge (const float charge, const string charge_type) {
            this->charge = charge;
            this->charge_set = true;
            this->charge_type = charge_type;
        }

        // 更改电荷类型
        void change_charge_type (const string new_charge_type) {
            this->charge_type = new_charge_type;
        }

        // 获取电荷类型
        const string get_charge_type () const {
            return this->charge_type;
        }

        // 获取坐标
        const vector<float> get_position () const {
            if (this->position_set == false) {
                throw runtime_error("no position data exist!");
            }
            return this->position;
        }

        // 获取原子类型
        short int get_atom_info () const {
            return this->atom_info;
        }

        // 获取电荷
        float get_charge () const {
            if (this->charge_set == false) {
                throw runtime_error("no charge assigned to this atom!");
            }
            return this->charge;
        }

        // 获取坐标存在
        bool structure_exist () const {
            return this->position_set;
        }

        // 获取电荷存在
        bool charge_exist () const {
            return this->charge_set;
        }

        // 删除坐标
        void del_position () {
            this->position.clear();
            this->position_set = false;
        }

        // 删除电荷
        void del_charge () {
            this->charge = 0;
            this->charge_type = "";
            this->charge_set = false;
        }

        // 删除所有
        void clear () {
            this->del_position();
            this->del_charge();
            this->atom_info = 0;
        }

};

class Topology {
    private:
        vector< vector<short int> > topology;
        unsigned long atom_num;

    public:
        // 构造函数
        Topology () {
            
        }

        // 初始化拓扑结构
        void init_topology (const unsigned long atom_num) {
            this->atom_num = atom_num;

            vector<short int> empty(this->atom_num, 0);

            for(unsigned long i=0; i<this->atom_num; i++) {
                this->topology.push_back(empty);
            }
        }

        // 清空拓扑信息
        void clear () {
            for (unsigned long i=0; i<this->atom_num; i++) {
                this->topology[i].clear();
            }
        }

        // 添加拓扑信息
        void add (const unsigned long atom_1, const unsigned long atom_2, const short int bond_type) {

            this->topology[atom_1][atom_2] = bond_type;
            this->topology[atom_2][atom_1] = bond_type;
        }

        // 打印拓扑信息
        void print () const {
            for(unsigned long i=0; i<this->atom_num; i++) {

                for(unsigned long j=0; j<this->atom_num; j++) {

                    cout << this->topology[i][j] << " ";

                }
                cout << endl;
            }
        }

        // 获取键链接信息
        short get_bond (unsigned long atom_1, unsigned long atom_2) const {
            return this->topology[atom_1][atom_2];
        }

        // 获取原子数
        const unsigned long get_atom_num () const {
            return this->atom_num;
        }

        // 获取原始信息
        const vector< vector<short int> > get_raw_data () const {
            return this->topology;
        }

        // 获取键连接数量
        const unsigned long get_bond_num () const {
            unsigned long bond_num = 0;
            for (unsigned long i=0; i<this->atom_num; i++) {
                for (unsigned long j=0; j<this->atom_num; j++) {
                    if (this->topology[i][j]!=0) bond_num += 1;
                }
            }
            return bond_num / 2;
        }

};


class Molecule {
    private:

        unsigned long atom_num = 0;
        unsigned long bond_num = 0;

        vector<Atom> data;
        string name = "";
        Topology topology;
        vector<float> dipole;
        
        bool charge_set = false;
        bool structure_set = false;
        bool topo_set = false;
        bool dipole_set = false;
        string charge_type = "";

    public:

        // 构造函数
        Molecule (string name) {
            this->name = name;
        }

        // 析构函数
        ~Molecule () {
            this->clear();
        }

        // 数据自检
        bool check_update () {
            unsigned long structure_num = 0;
            unsigned long charge_num = 0;
            vector<string> atom_charge_type;

            for(unsigned long i=0; i<this->atom_num; i++) {
                if (data[i].structure_exist() == true) { // 获取结构信息
                    structure_num += 1;
                }
                if (data[i].charge_exist() == true) {   // 获取电荷信息
                    charge_num += 1;
                    atom_charge_type.push_back(data[i].get_charge_type());
                }
            }
            // 检查数量，无数据设置有效
            if(structure_num!=this->atom_num && structure_num!=0) {
                cout << "structure number does not match, self check fail!\n";
                return false;
            }
            else if (structure_num != 0) {
                this->structure_set = true;
            }


            if(charge_num!=this->atom_num && charge_num!=0) {
                cout << "charge number does not match, self check fail!\n";
                return false;
            }
            else if (charge_num != 0) {
                this->charge_set = true;
                string temp_s = atom_charge_type[0];
                for (unsigned long i=0; i<atom_charge_type.size(); i++) {     // 检查电荷类型一致性
                    if (atom_charge_type[i]!=temp_s && atom_charge_type[i]!="") {
                        cout <<  "charge type not match, self check fail!\n";
                        return false;
                    }
                }
                this->charge_type = temp_s;
            }
            
            return true;
        }

        // 添加原子
        void add_atom (const Atom &new_atom) {
            if (this->topo_set == true) {
                throw runtime_error("topology has been set, cannot add atom at this time!");
            }
            this->data.push_back(new_atom);
            this->atom_num += 1;
        }

        // 添加偶极矩
        void add_dipole (const vector<float> &dipole) {
            if (dipole.size() != 3) {
                throw runtime_error("dipole moment is not 3 dimensional!");
            }
            this->dipole = dipole;
            this->dipole_set = true;
        }

        // 添加拓扑信息
        void add_bond (const unsigned long atom_1, const unsigned long atom_2, const short bond_type) {
            if(this->topo_set == false) {
                this->topology.init_topology(this->atom_num);
                this->topo_set = true;
            }
            this->topology.add(atom_1, atom_2, bond_type);
            this->bond_num += 1;
        }

        // 整体替换拓扑信息
        void replace_topo (const Topology &topology) {
            if (topology.get_atom_num() != this->atom_num) throw runtime_error("shape of input topology does not match! replace fail.\n");
            this->del_topology();
            this->topology = topology;
            this->topo_set = true;
            this->bond_num = this->topology.get_bond_num();
        }

        // 清空拓扑信息
        void del_topology () {
            if(this->topo_set) {
                this->topology.clear();
                this->topo_set = false;
                this->bond_num = 0;
            }
        }

        // 打印拓扑信息
        void print_topo () const {
            if(this->topo_set == false) {
                throw runtime_error("no topology set up yet!");
            }
            cout << "Topology matrix:" << endl;
            this->topology.print();
        }

        // 打印原子信息
        void print_atom () const {
            cout << this->atom_num << " atoms in this molecule" << endl;
            cout << "Atom list:" << endl;
            for(unsigned long i=0; i<atom_num; i++) {
                cout << data[i].get_atom_info() << " ";
            }
            cout << endl;
        }

        // 打印结构信息
        void print_structure () const {
            cout << "Cartesian coordinate:" << endl;
            vector<float> position;
            for(unsigned long i=0; i<this->atom_num; i++) {
                position = data[i].get_position();
                cout << data[i].get_atom_info() << " ";

                for (unsigned long j=0; j<3; j++) {
                    cout << position[j] << " ";
                }
                cout << endl;
            }
        }

        // 打印偶极矩信息
        void print_dipole () const {
            cout << "Dipole moment:" << endl;

            for (int i=0; i<3; i++) {
                cout << this->dipole[i] << "\t";
            }
            cout << endl;
        }

        // 打印电荷
        void print_charge () const {
            cout << "Charge:" << endl;
            cout << "type: " << this->charge_type << endl;
            for(unsigned long i=0; i<this->atom_num; i++) {
                cout << data[i].get_atom_info() << "\t" << data[i].get_charge() << endl;
            }
        }

        // 打印所有信息
        void print_all () const {
            cout << this->name << endl;
            this->print_atom();
            if(this->structure_set == true) {
                this->print_structure();
            }
            if (this->charge_set == true) {
                this->print_charge();
            }
            if(this->topo_set == true) {
                this->print_topo();
            }
            if(this->dipole_set == true) {
                this->print_dipole();
            }
        }

        // 获取原子数
        unsigned long get_atom_num () const {
            return this->atom_num;
        }

        // 获取原子（重载[]）
        const Atom operator[] (unsigned long id) const {
            return this->data[id];
        }

        // 删除电荷信息
        void del_charge () {
            for (int i=0; i<this->atom_num; i++) {
                data[i].del_charge();
            }
            this->charge_set = false;
            this->charge_type = "";
        }

        // 删除坐标信息
        void del_structure () {
            for (int i=0; i<this->atom_num; i++) {
                data[i].del_position();
            }
            this->structure_set = false;
        }

        // 删除偶极矩
        void del_dipole () {
            dipole.clear();
            this->dipole_set = false;
        }
    

        // 清空分子
        void clear () {
            // 清空原子
            for (unsigned long i=0; i<this->atom_num; i++) {
                this->data[i].clear();
            }
            this->data.clear();
            this->atom_num = 0;

            // 清空名字
            this->name = "";

            //清空拓扑信息
            this->del_topology();

            //清空偶极矩信息
            this->dipole.clear();

            //清空留存信息
            this->charge_set = false;
            this->structure_set = false;
            this->topo_set = false;
            this->dipole_set = false;
        }

        // 添加空原子填充
        void pad (const unsigned long target_num) {
            Atom ghost(0);

            if(charge_set) {
                ghost.add_charge(0, "");
            }
            if(structure_set) {
                vector<float> null_position(3, 0);
                ghost.add_position(null_position);
            }

            // 统一topology
            if(topo_set) {
                Topology newtopo;

                newtopo.init_topology(target_num);
                for(unsigned long i=0; i<this->atom_num; i++) {
                    for(unsigned long j=0; j<this->atom_num; j++) {
                        newtopo.add(i, j, this->topology.get_bond(i, j));
                    }
                }

                this->topology.clear();
                this->topology = newtopo;
            }

            // 统一data
            for(unsigned long i=this->atom_num; i<target_num; i++) {
                data.push_back(ghost);
            }
            this->atom_num = target_num;
        }

        // 获取原子信息
        const vector<short int> get_atom_info () const {
            vector<short int> output;
            for(unsigned long i=0; i<this->atom_num; i++) {
                output.push_back(this->data[i].get_atom_info());
            }
            return output;
        }

        // 获取结构信息
        const vector< vector<float> > get_structure () const {

            vector< vector<float> > output;
            vector<float> pos_temp;
            vector<float> output_temp;

            for(unsigned long i=0; i<this->atom_num; i++) {

                pos_temp = this->data[i].get_position();

                for(int j=0; j<3; j++) {
                    output_temp.push_back(pos_temp[j]);
                }

                output.push_back(output_temp);

                pos_temp.clear();
                output_temp.clear();
            }
            return output;
        }

        // 获取电荷信息
        const vector<float> get_charge () const {

            vector<float> output;
            for(unsigned long i=0; i<this->atom_num; i++) {
                output.push_back(this->data[i].get_charge());
            }
            return output;
        }

        // 获取名字
        const string get_name () const {
            return this->name;
        }

        // 获取拓扑信息
        const vector< vector<short int> > get_topology () const {
            return this->topology.get_raw_data();
        }

        const Topology get_topology_class() const {
            return this->topology;
        }

        // 获取偶极矩
        const vector<float> get_dipole () const {
            return this->dipole;
        }

        // 检查结构存在
        bool structure_exist () const {
            return this->structure_set;
        }
        // 检查电荷存在
        bool charge_exist () const {
            return this->charge_set;
        }
        // 检查拓扑数据存在
        bool topo_exist () const {
            return this->topo_set;
        }
        // 检查偶极矩存在
        bool dipole_exist () const {
            return this->dipole_set;
        }

        // 获取电荷类型
        const string get_charge_type () const {
            return this->charge_type;
        }

};

class Gdata {
    private:
        string gd_name;

        vector<Molecule> data;
        unsigned long data_num = 0;
        unsigned long max_atom = 0;
        
        bool topo_set = false;
        bool charge_set = false;
        bool structure_set = false;
        bool dipole_set = false;
        string charge_type = "";

        bool unified = false;

    public:
        // 构造函数
        Gdata () {
        }

        Gdata (string name) {
            this->gd_name = name;
        }

        void clear() {
            data.clear();
            this->data_num = 0;
            this->max_atom = 0;
            this->topo_set = false;
            this->charge_set = false;
            this->structure_set = false;
            this->dipole_set = false;
            this->charge_type = "";
            this->unified = false;
        }

        ~Gdata () {
            this->clear();
        }

        // 检查并更新
        bool check_update () {
            unsigned long max_atom = 0;
            unsigned long structure_num = 0;
            unsigned long charge_num = 0;
            unsigned long topo_num = 0;
            unsigned long dipole_num = 0;
            vector<string> atom_charge_type;

            for(unsigned long i=0; i<this->data_num; i++) {
                if (data[i].structure_exist() == true) { // 获取结构信息
                    structure_num += 1;
                }
                if (data[i].charge_exist() == true) {   // 获取电荷信息
                    charge_num += 1;
                    atom_charge_type.push_back(data[i].get_charge_type());
                }
                if (data[i].topo_exist() == true) {
                    topo_num += 1;
                }
                if (data[i].dipole_exist() == true) {
                    dipole_num += 1;
                }
                if (data[i].get_atom_num() > max_atom) {
                    max_atom = data[i].get_atom_num();
                }
            }

            // 检查数量，无数据设置有效
            // 结构数据
            if(structure_num!=this->data_num && structure_num!=0) {
                cout << "structure number does not match, self check fail!\n";
                return false;
            }
            else if (structure_num != 0) {
                this->structure_set = true;
            }
            // 电荷数据
            if(charge_num!=this->data_num && charge_num!=0) {
                cout << "charge number does not match, self check fail!\n";
                return false;
            }
            else if (charge_num != 0) {
                this->charge_set = true;
                string temp_s = atom_charge_type[0];
                for (unsigned long i=0; i<atom_charge_type.size(); i++) {     // 检查电荷类型一致性
                    if (atom_charge_type[i]!=temp_s && atom_charge_type[i]!="") {
                        cout <<  "charge type not match, self check fail!\n";
                        return false;
                    }
                }
                this->charge_type = temp_s;
            }
            // 拓扑数据
            if (topo_num!=this->data_num && topo_num!=0) {
                cout << "topo number does not match, self check fail\n";
                return false;
            }
            else if (topo_num != 0) {
                this->topo_set = true;
            }
            // 偶极矩数据
            if (dipole_num!=this->data_num && dipole_num!=0) {
                cout << "dipole number does not match, self check fail\n";
                return false;
            }
            else if (dipole_num != 0) {
                this->dipole_set = true;
            }

            this->max_atom = max_atom;
            return true;
        }

        // 添加分子
        void add_molecule (const Molecule &new_molecule) {
            this->data.push_back(new_molecule);
            data_num += 1;
            if (new_molecule.get_atom_num()>this->max_atom) {
                max_atom = new_molecule.get_atom_num();
            }
            this->unified = false;
        }

        // 打印信息
        void print () const {
            cout << "summary of Gdata class: " << endl;
            cout << "data number: " << this->data_num << endl;
            cout << "max_atom: " << this->max_atom << endl;
            cout << "contained data: ";
            if (structure_set) cout << "structure ";
            if (charge_set) cout << "charge ";
            if (topo_set) cout << "topology ";
            if (dipole_set) cout << "dipole_moment ";
            cout << endl;
            if (charge_set) cout << "charge type: " << charge_type << endl;
        }

        // 打印指定分子信息
        void print_molecule (const unsigned long id) const {
            data[id].print_all();
        }

        void print_molecule (const string name) const {
            bool match = false;
            for (unsigned long i=0; i<this->data_num; i++) {
                if (this->data[i].get_name() == name) {
                    this->print_molecule(i);
                    match = true;
                }
            }
            if (!match) {
                cout << "No molecule named " << name << endl;
            }
        }

        // 获取数据数量
        unsigned long get_data_num () const {
            return this->data_num;
        }

        // 获取max_atom
        unsigned long get_max_atom () const {
            return this->max_atom;
        }

        // 统一尺寸
        void unify () {
            cout << "unifying data shape..." << endl;
            double data_num = this->data_num;
            for(unsigned long i=0; i<this->data_num; i++) {
                this->data[i].pad(this->max_atom);
                cout << setprecision(2) << fixed << i/data_num*100 << "%\r";
            }
            cout << "          \r";
            this->unified = true;
        }

        // 检查统一
        bool check_unified () const {
            return this->unified;
        }


        // 获取原子类型
        const vector< vector<short int> > get_atom_info () const {

            vector< vector<short int> > output;
            for(unsigned long i=0; i<this->data_num; i++) {
                output.push_back(this->data[i].get_atom_info());
            }

            return output;
        }

        // 获取xyz格式结构信息
        const vector< vector< vector<float> > > get_structure () const {

            vector< vector< vector<float> > > output;

            for(unsigned long i=0; i<this->data_num; i++) {
                output.push_back(this->data[i].get_structure());
            }

            return output;
        }

        // 获取电荷信息
        const vector< vector<float> > get_charge () const {

            vector< vector<float> > output;

            for(unsigned long i=0; i<this->data_num; i++) {
                output.push_back(this->data[i].get_charge());
            }

            return output;
        }

        // 获取名字信息
        const vector<string> get_name () const {

            vector<string> output;

            for(unsigned long i=0; i<this->data_num; i++) {
                output.push_back(this->data[i].get_name());
            }

            return output;
        }

        // 获取拓扑信息
        const vector< vector< vector<short int> > > get_topology () const {

            vector< vector< vector<short int> > > output;
 
            for(unsigned long i=0; i<this->data_num; i++) {
                output.push_back(this->data[i].get_topology());
            }

            return output;
        }

        // 获取偶极矩信息
        const vector< vector<float> > get_dipole () const {

            vector< vector<float> > output;

            for(unsigned long i=0; i<this->data_num; i++) {
                output.push_back(this->data[i].get_dipole());
            }

            return output;
        }

        // 获取数据集名字
        const string get_gd_name () const {
            return this->gd_name;
        }

        // 搜索数据
        void search_atom (const short atom_id) {
            vector< vector<short int> > atom_info = this->get_atom_info();
            unsigned long count = 0;
            for(unsigned long i=0; i<this->data_num; i++) {
                for(unsigned long j=0; j<this->max_atom; j++) {
                    if(atom_info[i][j] == atom_id) {
                        cout << "atom found in Molecule " << i << " Atom " << j << endl;
                        count += 1;
                    }
                }
            }
            cout << count << " atom match" << endl;
        }

        // 获取结构存在
        bool structure_exist () const {
            return this->structure_set;
        }
        // 获取电荷存在
        bool charge_exist () const {
            return this->charge_set;
        }
        //获取拓扑存在
        bool topo_exist () const {
            return this->topo_set;
        }
        // 获取偶极矩存在
        bool dipole_exist () const {
            return this->dipole_set;
        }
        // 获取电荷类型
        const string get_charge_type () const {
            return this->charge_type;
        }

        // 删除结构信息
        void del_structure () {
            for (int i=0; i<this->data_num; i++) {
                data[i].del_structure();
            }
            this->structure_set = false;
        }
        // 删除电荷信息
        void del_charge () {
            for (int i=0; i<this->data_num; i++) {
                data[i].del_charge();
            }
            this->charge_set = false;
            this->charge_type = "";
        }
        // 删除拓扑信息
        void del_topology () {
            for (int i=0; i<this->data_num; i++) {
                data[i].del_topology();
            }
            this->topo_set = false;
        }
        // 删除偶极矩信息
        void del_dipole () {
            for (int i=0; i<this->data_num; i++) {
                data[i].del_dipole();
            }
            this->dipole_set = false;
        }

        // 数据为空检查
        bool is_empty () {
            return this->data_num==0;
        }

        // 设置名字
        void set_gd_name (string new_name) {
            this->gd_name = new_name;
        }

        // 重载+为拼接
        const Gdata operator+ (const Gdata& gd) const {

            Gdata gd_output(*this);

            unsigned long data_num = gd.get_data_num();
            for(int i=0; i<data_num; i++) {
                gd_output.add_molecule(gd[i]);
            }

            gd_output.check_update();
            return gd_output;
        }
        // 重载+=为自拼接
        void operator+= (const Gdata& gd) {
            
            unsigned long data_num = gd.get_data_num();
            for(int i=0; i<data_num; i++) {
                this->add_molecule(gd[i]);
            }

            this->check_update();
        }

        // 重载==为判断
        bool operator== (const Gdata& gd) const {

            // 检查data num
            if (this->data_num != gd.data_num) {
                return false;
            }

            // 检查数据max atom
            if (this->max_atom != gd.max_atom) {
                return false;
            }

            // 检查结构数据
            if (this->structure_exist() ^ gd.structure_exist()) return false;
            else if (this->structure_exist()) {
                if (this->get_structure() != gd.get_structure()) return false;
            }

            // 检查电荷数据
            if (this->charge_exist() ^ gd.charge_exist()) return false;
            else if (this->charge_exist()) {
                if (this->get_charge() != gd.get_charge()) return false;
            }

            // 检查拓扑数据
            if (this->topo_exist() ^ gd.topo_exist()) return false;
            else if (this->topo_exist()) {
                if (this->get_topology() != gd.get_topology()) return false;
            }

            // 检查偶极矩数据
            if (this->dipole_exist() ^ gd.dipole_exist()) return false;
            else if (this->dipole_exist()) {
                if (this->get_dipole() != gd.get_dipole()) return false;
            }

            return true;
        }

        // 重载[]
        const Molecule operator[](unsigned long id) const {
            return this->data[id];
        }
};
