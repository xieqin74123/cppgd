#include <array>
#include <string>
#include <vector>

#ifndef SUP_FUNCTIONS_HPP_
#define SUP_FUNCTIONS_HPP_

using namespace std;
namespace sup {
template <class dtype>
const vector<dtype> vec_flat(const vector<vector<dtype>> &in_data);

template <class dtype>
const vector<dtype> vec_flat(const vector<vector<vector<dtype>>> &in_data);

template <class dtype>
const vector<vector<vector<dtype>>> vec_pad(
    const vector<vector<vector<dtype>>> &in_data);

template <class dtype>
const vector<vector<dtype>> vec_pad(const vector<vector<dtype>> &in_data);

string get_in_range(const string str, const int begin, const int end);

bool str_is_number(const string str);

string get_real_name(string path);

// generate normalisation param
const array<float, 4> get_normalisation_param(const vector<float> &data_series);
const array<float, 4> get_normalisation_param(
    const vector<vector<float>> &data_series);

// normalisation function
const vector<vector<float>> normalisation(
    const vector<vector<float>> &data_series, const array<float, 4> param,
    const string style = "std");
const vector<vector<float>> normalisation(
    const vector<vector<float>> &data_series, const string style = "std");

// filesystem related
const string get_pwd();

}  // namespace sup

#endif