
#include "ep3.h"
#include "router.h"
#include "network.h"

#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
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
  for (vector<Router>::iterator it = routers.begin();
       it != routers.end(); ++it)
    it->start_up();
  cout << "=== Simulando troca de mensagens ===" << endl;
  while (network.pending_msgs()) {
    Network::Packet packet = network.next_msg();
    routers[packet.id_receiver].receive_msg(packet.id_sender, packet.msg);
  }
}

void run_prompt () {

}

} // namespace ep3

