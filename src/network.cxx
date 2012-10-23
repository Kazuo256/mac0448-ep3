
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

void Network::load_topology (const string& topology_file) {
  size_t   router_num = 0, count = 0;
  ifstream file(topology_file.c_str(), ifstream::in);

  topology_.clear();
  do {
    string line;
    getline(file, line);
    stringstream stream(line, stringstream::in);
    topology_.push_back(vector<double>());
    while(!stream.eof()) {
      double delay;
      stream >> delay;
      topology_.back().push_back(delay);
    }
    router_num = max(router_num, topology_.back().size());
    count++;
  } while (count < router_num);
}

} // namespce ep3

