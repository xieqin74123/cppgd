#include <dirent.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#ifndef GDATA_CLASS_HPP_
#include "classes/gdata_class.hpp"
#endif

#ifndef GDATA_HPP_
#define GDATA_HPP_

using namespace std;

namespace gdata {
void save_as_xyz(string path, const Gdata &data);
void save_as_npy(string path, Gdata &data, bool save_config = true);
const Gdata read_chargemol_file(const string path);
const Gdata read_chargemol_file(const vector<string> path);
const Gdata read_chargemol_dir(string path);
const Gdata read_mol2_file(const string path);
const Gdata read_mol2_file(const vector<string> path);
const Gdata read_mol2_dir(string path);
const Gdata read_gaussian_log_file(const string path, const string charge_type);
const Gdata read_gaussian_log_file(const vector<string> path_list,
                                   const string charge_type);
const Gdata read_gaussian_log_dir(string path, const string charge_type);
const Gdata merge(const Gdata &gd1, const Gdata &gd2);
}  // namespace gdata

#endif