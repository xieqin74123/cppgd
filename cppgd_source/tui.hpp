#include <stdlib.h>

#include <iostream>
#include <vector>

#ifndef TUI_HPP_
#define TUI_HPP_

using namespace std;

class Gdata;

void show_help();
const int quick_operate(const int argc, const char *argv[]);

const string user_str(const string);
const int user_cmd();
void press_to_continue();

const int main_title();
const int main_title(vector<Gdata> *gdata_list);
void print_main_title(const vector<Gdata> *);

const int dataset_title(Gdata *);
void print_dataset_title(const Gdata *);

Gdata merge_title(vector<Gdata> *);
void print_merge_title(const string, const string);

#endif
