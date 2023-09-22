#ifndef GRAPH_HPP_
#include "graph.hpp"
#endif


void Graph::error (int error_num) const {
    switch (error_num) {
        case 1: throw runtime_error("Error GR1: input graph is not square!\n"); break;
        default: throw invalid_argument("Error GR0: invalid error argument received!\n"); break;
    }
    
}

void Graph::init (unsigned long size) {
    this->graph.clear();
    vector<short> fill_d2;
    for (unsigned long i=0; i<size; i++) {
        fill_d2.push_back(0);
    }

    for (unsigned long j=0; j<size; j++) {
        this->graph.push_back(fill_d2);
    }
}

Graph::Graph (unsigned long size) {
    this->init(size);
}

Graph::Graph (vector< vector<short> > graph) {
    unsigned long size = graph.size();
    for (unsigned long i=0; i<size; i++) {
        if (graph[i].size()!=size) {
            this->error(1);
        }
    }
    this->graph = graph;
}

Graph::Graph () {
    this->init(0);
}

void Graph::clear () {
    this->init(this->size());
}

void Graph::reset (unsigned long size) {
    this->init(size);
}

void Graph::add (const unsigned long node1, const unsigned long node2) {
    this->add(node1, node2, 1);
}

void Graph::add (const unsigned long node1, const unsigned long node2, const short edge) {
    this->graph[node1][node2] = edge;
    this->graph[node2][node1] = edge;
}

short Graph::get (const unsigned long node1, const unsigned long node2) const {
    return this->graph[node1][node2];
}

const unsigned long Graph::size() const {
    return this->graph.size();
}

const vector< vector<short> > Graph::get_raw_data () const {
    return this->graph;
}

const unsigned long Graph::get_connection_num () const {
    unsigned long long num = 0;
    for (unsigned long i=0; i<this->size(); i++) {
        for (unsigned long j=i; j<this->size(); j++) {
            if (this->get(i, j) == 0) {
                continue;
            }
            num++;
        }
    }
    return num;
}

void Graph::print () const {
    for(unsigned long i=0; i<this->size(); i++) {
        for(unsigned long j=0; j<this->size(); j++) {
            cout << this->get(i, j) << " ";
        }
        cout << endl;
    }
}

vector<short> Graph::operator[] (const unsigned long id) const {
    return this->graph[id];
}