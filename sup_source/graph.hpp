#include <array>
#include <iostream>
#include <vector>

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

using namespace std;

class Graph {
   private:
    vector<vector<short> > graph;

    // error function
    void error(int error_num) const;
    void init(unsigned long size);

   public:
    // construct
    explicit Graph(unsigned long size);
    Graph(vector<vector<short> > graph);
    Graph();

    // clear
    void clear();

    // reset
    void reset(unsigned long size);

    // add connection
    void add(const unsigned long node1, const unsigned long node2);
    void add(const unsigned long node1, const unsigned long node2,
             const short edge);

    // get connection
    short get(unsigned long node1, unsigned long node2) const;

    // get size
    const unsigned long size() const;

    // get raw data
    const vector<vector<short> > get_raw_data() const;

    // get connection number
    const unsigned long get_connection_num() const;

    // print
    void print() const;

    // overload []
    vector<short> operator[](const unsigned long id) const;
};

#endif