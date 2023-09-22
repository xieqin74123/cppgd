#ifndef SUP_FUNCTIONS_HPP_
#include "sup_functions.hpp"
#endif

#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace sup {
// 数据展开
template <class dtype>
const vector<dtype> vec_flat(const vector<vector<dtype>> &in_data) {
    vector<dtype> output;

    typename vector<vector<dtype>>::const_iterator it1;
    typename vector<dtype>::const_iterator it2;

    for (it1 = in_data.begin(); it1 != in_data.end(); it1++) {
        for (it2 = it1->begin(); it2 != it1->end(); it2++) {
            output.push_back(*it2);
        }
    }

    return output;
}
template const vector<short> vec_flat<short>(
    const vector<vector<short>> &in_data);
template const vector<int> vec_flat<int>(const vector<vector<int>> &in_data);
template const vector<long> vec_flat<long>(const vector<vector<long>> &in_data);
template const vector<long long> vec_flat<long long>(
    const vector<vector<long long>> &in_data);
template const vector<float> vec_flat<float>(
    const vector<vector<float>> &in_data);
template const vector<double> vec_flat<double>(
    const vector<vector<double>> &in_data);

template <class dtype>
const vector<dtype> vec_flat(const vector<vector<vector<dtype>>> &in_data) {
    vector<dtype> output;

    typename vector<vector<vector<dtype>>>::const_iterator it1;
    typename vector<vector<dtype>>::const_iterator it2;
    typename vector<dtype>::const_iterator it3;

    for (it1 = in_data.begin(); it1 != in_data.end(); it1++) {
        for (it2 = it1->begin(); it2 != it1->end(); it2++) {
            for (it3 = it2->begin(); it3 != it2->end(); it3++) {
                output.push_back(*it3);
            }
        }
    }

    return output;
}
template const vector<short> vec_flat<short>(
    const vector<vector<vector<short>>> &in_data);
template const vector<int> vec_flat<int>(
    const vector<vector<vector<int>>> &in_data);
template const vector<long> vec_flat<long>(
    const vector<vector<vector<long>>> &in_data);
template const vector<long long> vec_flat<long long>(
    const vector<vector<vector<long long>>> &in_data);
template const vector<float> vec_flat<float>(
    const vector<vector<vector<float>>> &in_data);
template const vector<double> vec_flat<double>(
    const vector<vector<vector<double>>> &in_data);

template <class dtype>
const vector<vector<vector<dtype>>> vec_pad(
    const vector<vector<vector<dtype>>> &in_data) {
    unsigned long max_d1 = 0;
    unsigned long max_d2 = 0;

    typename vector<vector<vector<dtype>>>::const_iterator it1;
    typename vector<vector<dtype>>::const_iterator it2;
    dtype pad_item = 0;

    for (it1 = in_data.begin(); it1 != in_data.end(); it1++) {
        if (it1->size() > max_d1) max_d1 = it1->size();
        for (it2 = it1->begin(); it2 != it1->end(); it2++) {
            if (it2->size() > max_d2) max_d2 = it2->size();
        }
    }

    auto output = in_data;
    typename vector<vector<vector<dtype>>>::iterator it3;
    typename vector<vector<dtype>>::iterator it4;
    vector<dtype> pad_vec_d1;
    for (unsigned long i = 0; i < max_d2; i++) pad_vec_d1.push_back(pad_item);

    for (it3 = output.begin(); it3 != output.end(); it3++) {
        for (it4 = it3->begin(); it4 != it3->end(); it4++) {
            for (unsigned long i = it4->size(); i < max_d2; i++)
                it4->push_back(pad_item);
        }

        for (unsigned long i = it3->size(); i < max_d1; i++)
            it3->push_back(pad_vec_d1);
    }

    return output;
}
template const vector<vector<vector<short>>> vec_pad(
    const vector<vector<vector<short>>> &in_data);
template const vector<vector<vector<float>>> vec_pad(
    const vector<vector<vector<float>>> &in_data);

template <class dtype>
const vector<vector<dtype>> vec_pad(const vector<vector<dtype>> &in_data) {
    unsigned long max_d1 = 0;

    typename vector<vector<dtype>>::const_iterator it1;
    dtype pad_item = 0;

    for (it1 = in_data.begin(); it1 != in_data.end(); it1++) {
        if (it1->size() > max_d1) max_d1 = it1->size();
    }

    auto output = in_data;
    typename vector<vector<dtype>>::iterator it2;

    for (it2 = output.begin(); it2 != output.end(); it2++) {
        for (unsigned long i = it2->size(); i < max_d1; i++)
            it2->push_back(pad_item);
    }

    return output;
}
template const vector<vector<short>> vec_pad(
    const vector<vector<short>> &in_data);
template const vector<vector<float>> vec_pad(
    const vector<vector<float>> &in_data);

// 从字符串中截取段
string get_in_range(const string str, const int begin, const int end) {
    string output;
    for (int i = begin; i < end; i++) {
        output.push_back(str[i]);
    }
    return output;
}

// 判断字符串是否为数字
bool str_is_number(const string str) {
    const int len = str.size();
    for (int i = 0; i < len; i++) {
        char temp = str[i];
        if ((temp >= '0' && temp <= '9') || temp == '.' || temp == '-') {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

// 获取文件名
string get_real_name(string path) {
    unsigned int beg = 0;
    unsigned int end = path.size();

    for (int i = 0; i < path.size(); i++) {
        if (path[i] == '/' || path[i] == '\\') {
            beg = i + 1;
        } else if (path[i] == '.') {
            end = i;
        }
    }

    return get_in_range(path, beg, end);
}

const array<float, 4> get_normalisation_param(
    const vector<float> &data_series) {
    // parameters will be return in the order of: min max ave std
    array<float, 4> output;
    float min_value = data_series[0];
    float max_value = data_series[0];
    float ave_value = 0;
    float std_value = 0;

    for (unsigned long i = 0; i < data_series.size(); i++) {
        if (data_series[i] < min_value) {
            min_value = data_series[i];
        }
        if (data_series[i] > max_value) {
            max_value = data_series[i];
        }
        ave_value += data_series[i];
    }

    ave_value = ave_value / data_series.size();

    for (unsigned long i = 0; i < data_series.size(); i++) {
        std_value += pow((data_series[i] - ave_value), 2);
    }
    std_value = sqrt(std_value / data_series.size());

    // assigned numbers
    output[0] = min_value;
    output[1] = max_value;
    output[2] = ave_value;
    output[3] = std_value;

    return output;
}

const array<float, 4> get_normalisation_param(
    const vector<vector<float>> &data_series) {
    auto data_series_flatten = vec_flat(data_series);
    return get_normalisation_param(data_series_flatten);
}

const vector<vector<float>> normalisation(
    const vector<vector<float>> &data_series, const string style) {
    auto param = get_normalisation_param(data_series);

    return normalisation(data_series, param, style);
}

const vector<vector<float>> normalisation(
    const vector<vector<float>> &data_series, const array<float, 4> param,
    const string style) {
    vector<vector<float>> output;
    if (style == "std") {
        // x'_i = frac{x_i - \bar{x}}{\std{x}}
        float ave_value = param[2];
        float std_value = param[3];

        for (unsigned long molecule = 0; molecule < data_series.size();
             molecule++) {
            vector<float> molecule_output;
            for (unsigned long atom = 0; atom < data_series[molecule].size();
                 atom++) {
                molecule_output.push_back(
                    (data_series[molecule][atom] - ave_value) / std_value);
            }
            output.push_back(molecule_output);
        }
    }

    else if (style == "minmax") {
        // x'_i = frac{x_i - \min{x}}{\max{x} - \min{x}}
        float min_value = param[0];
        float max_value = param[1];

        float minmax = max_value - min_value;

        for (unsigned long molecule = 0; molecule < data_series.size();
             molecule++) {
            vector<float> molecule_output;
            for (unsigned long atom = 0; atom < data_series[molecule].size();
                 atom++) {
                molecule_output.push_back(
                    (data_series[molecule][atom] - min_value) / minmax);
            }
            output.push_back(molecule_output);
        }
    }

    else {
        throw runtime_error(
            "Normalisation error: unknown normalisation method!\n");
    }
    return output;
}

const string get_pwd() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    return currentPath;
}

}  // namespace sup