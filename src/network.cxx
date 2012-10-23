
#include "network.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using std::max;
using std::vector;
using std::string;
using std::getline;
using std::ifstream;
using std::stringstream;

using std::cout;
using std::endl;

namespace ep3 {

size_t Network::load_topology (const string& topology_file) {
  size_t   node_num = 0, count = 0;
  ifstream file(topology_file.c_str(), ifstream::in);

  topology_.clear();
  do {
    string line;
    getline(file, line);
    stringstream stream(line, stringstream::in);
    topology_.push_back(vector<double>());
    while(!stream.eof()) {
      double cost;
      stream >> cost;
      topology_.back().push_back(cost);
    }
    node_num = max(node_num, topology_.back().size());
    count++;
  } while (count < node_num);
  return node_num;
}

double Network::get_delay (unsigned id_sender, unsigned id_receiver) const {
  return topology_[id_sender][id_receiver];
}

} // namespce ep3

