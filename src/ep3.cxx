
#include "ep3.h"
#include "router.h"
#include "network.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <tr1/functional>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::for_each;
using std::tr1::mem_fn;

namespace ep3 {

typedef void (Router::*Bootstrap) ();

const static Bootstrap bootstrap_list[] = {
  &Router::start_up,
  &Router::linkstate_begin,
  &Router::distvector_begin
};

static vector<Router>     routers;
static Network            network;
static vector<Bootstrap>  bootstraps(bootstrap_list,
                                     bootstrap_list+sizeof(bootstrap_list));

void create_network (const std::string& topology_file) {
  size_t router_num = network.load_topology(topology_file);
  cout << "## Number of routers in the network: " << router_num << endl;
  for (unsigned i = 0; i < router_num; ++i)
    routers.push_back(Router(&network, i));
}

void simulation_step (const Bootstrap& bootstrap_method) {
  for_each(routers.begin(), routers.end(), mem_fn(bootstrap_method));
  while (network.pending_msgs()) {
    Network::Packet packet = network.next_msg();
    routers[packet.id_receiver].receive_msg(packet.id_sender, packet.msg);
  }
}

void find_routes () {
  cout << "## Simulating routers' startup message exchange" << endl;
  for_each(bootstraps.begin(), bootstraps.end(), simulation_step);
}

void run_prompt () {

}

} // namespace ep3

