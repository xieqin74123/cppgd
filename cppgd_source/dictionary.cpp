#ifndef DICTIONARY_HPP_
#include "dictionary.hpp"
#endif

Dictionary::Dictionary() {
    sym_element_map = {{"Ghost", 0}, {"H", 1},  {"C", 6},  {"N", 7},
                       {"O", 8},     {"F", 9},  {"P", 15}, {"S", 16},
                       {"Cl", 17},   {"Br", 35}};
    num_element_map = {{0, "Ghost"}, {1, "H"},  {6, "C"},  {7, "N"},
                       {8, "O"},     {9, "F"},  {15, "P"}, {16, "S"},
                       {17, "Cl"},   {35, "Br"}};
    atm_dict = {{"Ghost", 0},  {"H", 1.0080}, {"C", 12.011}, {"N", 14.007},
                {"O", 15.999}, {"F", 18.998}, {"P", 30.974}, {"S", 32.06},
                {"Cl", 35.45}, {"Br", 79.904}};
    bd_dict = {{"1", 1}, {"2", 2}, {"3", 3}, {"am", 4}, {"ar", 5}, {"du", 6}};
    el_dict = {
        {0, 0},      // Ghost atom
        {1, 2.20},   // H
        {5, 2.04},   // B
        {6, 2.55},   // C
        {7, 3.04},   // N
        {8, 3.44},   // O
        {9, 3.98},   // F
        {14, 1.90},  // Si
        {15, 2.19},  // P
        {16, 2.58},  // S
        {17, 3.16},  // Cl
        {35, 2.96},  // Br
        {53, 2.66}   // I
    };
}

int Dictionary::element_dict(std::string sym) {
    /***********************************************/
    /* Element Dictionary str2int                  */
    /* Args:                                       */
    /*     sym: input atomic symbol. type <string> */
    /* Returns:                                    */
    /*     atomic_number: type <int>               */
    /***********************************************/

    int atomic_number = 0;

    std::map<std::string, int>::iterator iter = sym_element_map.find(sym);

    if (iter == sym_element_map.end()) {
        std::cout << "warning: element not match! ";
        std::cout << "receive: " << sym << std::endl;
        return -1;
    }

    atomic_number = iter->second;

    return atomic_number;
}

std::string Dictionary::element_dict(int at_num) {
    /***********************************************/
    /* Element Dictionary int2str                  */
    /* Args:                                       */
    /*     at_num: input atomic number. type <int> */
    /* Returns:                                    */
    /*     atomic_symbol: type <string>            */
    /***********************************************/

    std::string atomic_symbol = "";

    std::map<int, std::string>::iterator iter = num_element_map.find(at_num);

    if (iter == num_element_map.end()) {
        std::cout << "warning: element not match! ";
        std::cout << "receive: " << at_num << std::endl;
        return " ";
    }

    atomic_symbol = iter->second;

    return atomic_symbol;
}

float Dictionary::mass_dict(std::string sym) {
    /***********************************************/
    /* Atomic Mass Dictionary                      */
    /* Args:                                       */
    /*     sym: input atomic symbol. type <string> */
    /* Returns:                                    */
    /*     atomic_weight: type <float>             */
    /***********************************************/

    float atomic_weight = 0;

    std::map<std::string, float>::iterator iter = atm_dict.find(sym);

    if (iter == atm_dict.end()) {
        std::cout << "warning: element not match! ";
        std::cout << "receive: " << sym << std::endl;
        return -1;
    }

    atomic_weight = iter->second;

    return atomic_weight;
}

int Dictionary::bond_dict(std::string bond_str) {
    /***************************************************/
    /* Bond Type Dictionary                            */
    /* Args:                                           */
    /*     bond_str: input bond type. type <string>    */
    /* Returns:                                        */
    /*     bond_int: representation of bond type <int> */
    /***************************************************/

    int bond_int = 0;

    std::map<std::string, int>::iterator iter = bd_dict.find(bond_str);

    if (iter == bd_dict.end()) {
        std::cout << "warning: bond type not match! ";
        std::cout << "receive: " << bond_str << std::endl;
        return -1;
    }

    bond_int = iter->second;

    return bond_int;
}

float Dictionary::eleneg_dict(int atom) {
    std::map<int, float>::iterator iter = this->el_dict.find(atom);
    if (iter == el_dict.end()) {
        std::cout
            << "warning: atom not found in electornegativity dictionary! ";
        std::cout << "receive: " << atom << std::endl;
        return -1;
    }

    return iter->second;
}