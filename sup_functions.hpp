#include <vector>

// 数据展开
template <class dtype>
vector<dtype> vec_flat(vector< vector<dtype> > in_data) {

    vector<dtype> output;

    typename vector< vector<dtype> >::iterator it1;
    typename vector<dtype>::iterator it2;
    
    for(it1=in_data.begin(); it1!=in_data.end(); it1++) {
        for(it2=it1->begin(); it2!=it1->end(); it2++) {
            output.push_back(*it2);
        }
    }

    return output;

}

template <class dtype>
vector<dtype> vec_flat(vector< vector< vector<dtype> > > in_data) {

    vector<dtype> output;

    typename vector< vector< vector<dtype> > >::iterator it1;
    typename vector< vector<dtype> >::iterator it2;
    typename vector<dtype>::iterator it3;
    
    for(it1=in_data.begin(); it1!=in_data.end(); it1++) {
        for(it2=it1->begin(); it2!=it1->end(); it2++) {
            for(it3=it2->begin(); it3!=it2->end(); it3++) {
                output.push_back(*it3);
            }
        }
    }

    return output;

}

// 从字符串中截取段
string get_in_range (const string str, const int begin, const int end) {
    string output;
    for(int i=begin; i<end; i++) {
        output.push_back(str[i]);
    }
    return output;
}

// 判断字符串是否为数字
bool str_is_number (const string str) {
    const int len = str.size();
    for(int i=0; i<len; i++) {
        char temp = str[i];
        if((temp>='0' && temp<='9') || temp=='.' || temp=='-') {
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

// 获取文件名
string get_real_name(string path) {
    unsigned int beg = 0;
    unsigned int end = path.size();

    for (int i=0; i<path.size(); i++) {
        if (path[i] == '/' || path[i] == '\\') {
            beg = i + 1;
        }
        else if (path[i] == '.') {
            end = i;
        }
    }

    return get_in_range(path, beg, end);
}