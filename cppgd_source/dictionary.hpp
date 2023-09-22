#include <iostream>
#include <map>

#ifndef DICTIONATY_HPP_
#define DICTIONARY_HPP_

class Dictionary {
   private:
    std::map<std::string, int> sym_element_map;
    std::map<int, std::string> num_element_map;
    std::map<std::string, float> atm_dict;
    std::map<std::string, int> bd_dict;
    std::map<int, float> el_dict;

   public:
    Dictionary();
    // element dict: input string return int
    int element_dict(std::string sym);
    // element dict: input int return string
    std::string element_dict(int at_num);
    // atomic mass dict
    float mass_dict(std::string sym);
    // bond dict
    int bond_dict(std::string bond_str);
    // electronegativity dict
    float eleneg_dict(int atom);
};

#endif