
#include "ep3.h"
#include "router.h"
#include "network.h"

#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

namespace ep3 {

static vector<Router> routers;
static Network        network;

void create_network (const std::string& topology_file) {
  size_t router_num = network.load_topology(topology_file);
  cout << "Number of routers in the network: " << router_num << endl;
  for (unsigned i = 0; i < router_num; ++i)
    routers.push_back(Router(&network, i));
}

void find_routes () {
  
}

void run_prompt () {

}

} // namespace ep3

