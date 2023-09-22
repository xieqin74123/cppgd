#include <stdlib.h>

#include <iostream>
#include <vector>

#ifndef GDATA_HPP_
#include "gdata.hpp"
#endif
#ifndef TUI_HPP_
#include "tui.hpp"
#endif
#ifndef SUP_FUNCTIONS_HPP_
#include "../sup_source/sup_functions.hpp"
#endif

string cppgd_version = "1.2";

using namespace std;

void show_help() {
    string help_info;

    help_info =
        "\n\
    cppgd: machine learning oriented Gaussian output file converter.\n\
    version " +
        cppgd_version +
        "\n\
    \n\
    Usage: cppgd -i<input format> [<input option>] <input path> -o<output format> <output path> [options]\n\
    \n\
    You can also directly run this programme without any argument to entre advanced mode\n\
    \n\
    Options:\n\
    --help \t show this help\n\
    \n\
    Supported input format:\n\
    mol2 \t mol2 format file \n\
    glog \t gaussian output log. Can specific charge type with --mulliken (default), --hirshfeld or --cm5\n\
    cmol \t chargemol output file\n\
    \n\
    Supported output format:\n\
    xyz \t xyz coordinate file\n\
    npy \t python3 numpy data series files\n\
    \n\
";

    cout << help_info;
}

const int quick_operate(const int argc, const char *argv[]) {
    Gdata data;

    for (int i = 1; i < argc; i++) {
        // 帮助
        if (string(argv[i]) == "--help") {
            show_help();
            return 0;
        }

        // mol2输入
        else if (string(argv[i]) == "-imol2") {
            data = gdata::read_mol2_dir(string(argv[i + 1]));
            i++;
        }
        // log输入
        else if (string(argv[i]) == "-iglog") {
            string charge_type;
            if (string(argv[i + 1]) == "--hirshfeld") {
                charge_type = "Hirshfeld";
                i++;
            } else if (string(argv[i + 1]) == "--mulliken") {
                charge_type = "Mulliken";
                i++;
            } else if (string(argv[i + 1]) == "--cm5") {
                charge_type = "CM5";
                i++;
            } else {
                charge_type = "Mulliken";
            }
            data =
                gdata::read_gaussian_log_dir(string(argv[i + 1]), charge_type);
            i++;
        }
        // chargemol 输入
        else if (string(argv[i]) == "-icmol") {
            data = gdata::read_chargemol_dir(string(argv[i + 1]));
            i++;
        }

        // xyz输出
        else if (string(argv[i]) == "-oxyz") {
            if (data.is_empty()) {
                cout << "No data to save! Use --help to see usage." << endl;
                return -1;
            }

            gdata::save_as_xyz(string(argv[i + 1]), data);
            i++;
        }

        // npy输出
        else if (string(argv[i]) == "-onpy") {
            if (data.is_empty()) {
                cout << "No data to save! Use --help to see usage." << endl;
                return -1;
            }

            gdata::save_as_npy(string(argv[i + 1]), data);
            i++;
        }

        // 其他情况
        else {
            cout << "Argument is not valid! Use --help to see usage." << endl;
            return -1;
        }
    }

    return 0;
}

const int main_title() {
    vector<Gdata> gdata_list;
    return main_title(&gdata_list);
}

const int main_title(vector<Gdata> *gdata_list) {
    while (1) {
        print_main_title(gdata_list);
        int cmd = user_cmd();
        if (cmd == 0) {  // Exit
            break;
        } else if (cmd == 1) {  // Merge
            auto merged_gd = merge_title(gdata_list);
            if (!merged_gd.is_empty()) {
                merged_gd.set_gd_name("merged");
                gdata_list->push_back(merged_gd);
            }
        } else if (cmd == 2) {  // add new
            Gdata new_gd("new_dataset");
            gdata_list->push_back(new_gd);
        } else if (cmd >= 10 &&
                   cmd < gdata_list->size() + 10) {  // entre dataset title
            int ds_index = cmd - 10;
            int dataset_return = dataset_title(&((*gdata_list)[ds_index]));
            if (dataset_return == -1)
                gdata_list->erase(gdata_list->begin() + ds_index);
        }

        else {
            cout << "Illegal input!" << endl;
            press_to_continue();
        }
    }
    return 0;
}

void print_main_title(const vector<Gdata> *gdata_list) {
    system("clear");
    cout << "Welcome to cppgd!" << endl;
    cout << "OPERATION:" << endl;
    cout << 0 << "\t - \t"
         << "Exit" << endl;
    cout << 1 << "\t - \t"
         << "Merge" << endl;
    cout << 2 << "\t - \t"
         << "New Dataset" << endl;
    cout << "SELECT DATASET:" << endl;
    for (int i = 10; i < gdata_list->size() + 10; i++)
        cout << i << "\t - \t" << (*gdata_list)[i - 10].get_gd_name() << endl;
}

const int dataset_title(Gdata *gd) {
    while (1) {
        print_dataset_title(gd);
        int cmd = user_cmd();
        if (cmd == 0) {  // Back
            return 0;
        } else if (cmd == 1) {  // rename
            string new_name = user_str("Set New Name");
            gd->set_gd_name(new_name);
        }

        /* INPUT SECTION */
        else if (cmd == 10) {  // mol2 input
            cout << "0 \t - \t Cancel" << endl;
            cout << "1 \t - \t From Dir" << endl;
            cout << "2 \t - \t From File" << endl;
            cmd = user_cmd();
            string gd_name_temp = gd->get_gd_name();
            if (cmd == 0)
                continue;         // Cancel
            else if (cmd == 1) {  // read mol2 dir
                string path = user_str("Input Dir Path");
                gd->clear();
                *gd = gdata::read_mol2_dir(path);
                gd->set_gd_name(gd_name_temp);
            } else if (cmd == 2) {  // read mol2 file
                string path = user_str("Input File Path");
                gd->clear();
                *gd = gdata::read_mol2_file(path);
                gd->set_gd_name(gd_name_temp);
            } else {
                cout << "Illegal Input!" << endl;
                press_to_continue();
            }
        }

        else if (cmd == 11) {  // Gaussian log input
            cout << "0 \t - \t Cancel" << endl;
            cout << "1 \t - \t From Dir" << endl;
            cout << "2 \t - \t From File" << endl;
            cmd = user_cmd();
            string gd_name_temp = gd->get_gd_name();
            if (cmd == 0)
                continue;         // Cancel
            else if (cmd == 1) {  // read log dir
                string path = user_str("Input Dir Path");
                string chargetype =
                    user_str("Input Charge Type (Mulliken, Hirshfeld, CM5)");
                if (chargetype == "Mulliken" || chargetype == "mulliken") {
                    gd->clear();
                    *gd = gdata::read_gaussian_log_dir(path, "Mulliken");
                    gd->set_gd_name(gd_name_temp);
                } else if (chargetype == "Hirshfeld" ||
                           chargetype == "hirshfeld") {
                    gd->clear();
                    *gd = gdata::read_gaussian_log_dir(path, "Hirshfeld");
                    gd->set_gd_name(gd_name_temp);
                } else if (chargetype == "CM5" || chargetype == "cm5") {
                    gd->clear();
                    *gd = gdata::read_gaussian_log_dir(path, "CM5");
                    gd->set_gd_name(gd_name_temp);
                } else {
                    cout << "Charge Type Not Supported!" << endl;
                    press_to_continue();
                }

            } else if (cmd == 2) {  // read log file
                string path = user_str("Input File Path");
                string chargetype =
                    user_str("Input Charge Type (Mulliken, Hirshfeld, CM5)");
                if (chargetype == "Mulliken" || chargetype == "mulliken") {
                    gd->clear();
                    *gd = gdata::read_gaussian_log_file(path, "Mulliken");
                    gd->set_gd_name(gd_name_temp);
                } else if (chargetype == "Hirshfeld" ||
                           chargetype == "hirshfeld") {
                    gd->clear();
                    *gd = gdata::read_gaussian_log_file(path, "Hirshfeld");
                    gd->set_gd_name(gd_name_temp);
                } else if (chargetype == "CM5" || chargetype == "cm5") {
                    gd->clear();
                    *gd = gdata::read_gaussian_log_file(path, "CM5");
                    gd->set_gd_name(gd_name_temp);
                } else {
                    cout << "Charge Type Not Supported!" << endl;
                    press_to_continue();
                }

            } else {
                cout << "Illegal Input!" << endl;
                press_to_continue();
            }
        }

        else if (cmd == 12) {  // read chargemol output
            cout << "0 \t - \t Cancel" << endl;
            cout << "1 \t - \t From Dir" << endl;
            cout << "2 \t - \t From File" << endl;
            cmd = user_cmd();
            string gd_name_temp = gd->get_gd_name();
            if (cmd == 0)
                continue;         // Cancel
            else if (cmd == 1) {  // read mol2 dir
                string path = user_str("Input Dir Path");
                gd->clear();
                *gd = gdata::read_chargemol_dir(path);
                gd->set_gd_name(gd_name_temp);
            } else if (cmd == 2) {  // read mol2 file
                string path = user_str("Input File Path");
                gd->clear();
                *gd = gdata::read_chargemol_file(path);
                gd->set_gd_name(gd_name_temp);
            } else {
                cout << "Illegal Input!" << endl;
                press_to_continue();
            }
        }

        /* OUTPUT SECTION */
        else if (cmd == 20) {  // output xyz
            string path = user_str("Output path");
            gdata::save_as_xyz(path, *gd);
        } else if (cmd == 21) {  // output npy
            string path = user_str("Output path");
            gdata::save_as_npy(path, *gd);
        }

        /* DELETE SECTION */
        else if (cmd == 100) {  // delete dataset
            gd->clear();
            return -1;
        } else if (cmd == 101) {  // delete structure
            gd->del_structure();
        } else if (cmd == 102) {  // delete charge
            gd->del_charge();
        } else if (cmd == 103) {  // delete topology
            gd->del_topology();
        } else if (cmd == 104) {  // delete dipole
            gd->del_dipole();
        }

        /* OTHER */
        else {
            cout << "Illegal Input!" << endl;
            press_to_continue();
        }
    }
}

void print_dataset_title(const Gdata *gd) {
    system("clear");
    cout << "===== Operation Manu of Dataset " << gd->get_gd_name()
         << " =====" << endl;
    cout << "INFORMATION:" << endl;
    cout << "Number of Molecule: \t" << gd->get_molecule_num() << endl;
    cout << "Availiable Data:" << endl;
    if (gd->structure_exist()) cout << "structure ";
    if (gd->charge_exist()) cout << "charge(" << gd->get_charge_type() << ") ";
    if (gd->topo_exist()) cout << "topology ";
    if (gd->dipole_exist()) cout << "dipole_moment ";
    cout << endl;
    cout << "General OPERATIONS" << endl;
    cout << "0 \t - \t Back" << endl;
    cout << "1 \t - \t Rename" << endl;
    cout << "INPUT OPERATIONS:" << endl;
    cout << "10 \t - \t From mol2" << endl;
    cout << "11 \t - \t From Gaussian log" << endl;
    cout << "12 \t - \t From Chargemol output" << endl;
    cout << "OUTPUT OPERATIONS:" << endl;
    cout << "20 \t - \t To xyz" << endl;
    cout << "21 \t - \t To npy" << endl;
    cout << "DELETE OPERATIONS:" << endl;
    cout << "100 \t - \t Delete Dataset" << endl;
    cout << "101 \t - \t Delete Structure" << endl;
    cout << "102 \t - \t Delete Charge" << endl;
    cout << "103 \t - \t Delete Topology" << endl;
    cout << "104 \t - \t Delete Dipole" << endl;
}

Gdata merge_title(vector<Gdata> *gdata_list) {
    string gd1_name = "None";
    string gd2_name = "None";
    const Gdata *gd1 = NULL;
    const Gdata *gd2 = NULL;
    while (1) {
        print_merge_title(gd1_name, gd2_name);
        int cmd = user_cmd();
        if (cmd == 0) {  // back
            Gdata gd_return;
            return gd_return;
        } else if (cmd == 1) {  // choose gd1
            cout << "Select Dataset 1 from List: " << endl;
            for (int i = 0; i < gdata_list->size(); i++)
                cout << i << " \t - \t " << (*gdata_list)[i].get_gd_name()
                     << endl;
            cmd = user_cmd();
            if (cmd >= 0 && cmd < gdata_list->size()) {
                gd1 = &(*gdata_list)[cmd];
                gd1_name = gd1->get_gd_name();
            } else {
                cout << "Illegal Input!" << endl;
                press_to_continue();
            }
        } else if (cmd == 2) {  // choose gd2
            cout << "Select Dataset 1 from List: " << endl;
            for (int i = 0; i < gdata_list->size(); i++)
                cout << i << " \t - \t " << (*gdata_list)[i].get_gd_name()
                     << endl;
            cmd = user_cmd();
            if (cmd >= 0 && cmd < gdata_list->size()) {
                gd2 = &(*gdata_list)[cmd];
                gd2_name = gd2->get_gd_name();
            } else {
                cout << "Illegal Input!" << endl;
                press_to_continue();
            }
        }

        else if (cmd == 3) {  // start merge
            return gdata::merge(*gd1, *gd2);
        }

        else {
            cout << "Illegal Input!" << endl;
            press_to_continue();
        }
    }
}

void print_merge_title(const string gd1_name, const string gd2_name) {
    system("clear");
    cout << "===== Merge Menu =====" << endl;
    cout << "0 \t 0 \t Back" << endl;
    cout << "1 \t - \t Chosen Dataset 1: " << gd1_name << endl;
    cout << "2 \t - \t Chosen Dataset 2: " << gd2_name << endl;
    cout << "3 \t - \t Confirm Merge" << endl;
}

const string user_str(const string info = "") {
    string user_input;
    cout << info << ": ";
    cin >> user_input;
    return user_input;
}

void press_to_continue() {
    string user_input;
    cout << "Press anykey to continue" << endl;
    getchar();
}

const int user_cmd() {
    string user_input;
    cout << "Input Command: ";
    cin >> user_input;
    if (sup::str_is_number(user_input))
        return stoll(user_input);
    else
        return -1;
}