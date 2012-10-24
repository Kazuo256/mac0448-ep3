
#include "ep3.h"
#include "router.h"
#include "network.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <tr1/functional>

using std::vector;
using std::string;
using std::stringstream;
using std::getline;
using std::cout;
using std::cin;
using std::endl;
using std::for_each;
using std::tr1::mem_fn;

namespace ep3 {

// Representa um método bootstrap dos roteadores.
typedef void (Router::*Bootstrap) ();

// Lista de métodos bootstrap.
const static Bootstrap bootstrap_list[] = {
  &Router::start_up,
  &Router::linkstate_begin,
  &Router::distvector_begin
};

// Variável auxiliar que aponta para o final da lista acima.
const static Bootstrap *const bootstrap_end =
  bootstrap_list+sizeof(bootstrap_list)/sizeof(Bootstrap);

// Os roteadores.
static vector<Router>     routers;
// A rede.
static Network            network;
// Os métodos de bootstrap para os algoritmos de roteamento.
static vector<Bootstrap>  bootstraps(bootstrap_list,
                                     bootstrap_end);

// Executa um passo da simulação de troca de mensagens, usando o método de
// bootstrap indicado.
static void simulation_step (const Bootstrap& bootstrap_method);

void create_network (const std::string& topology_file) {
  size_t router_num = network.load_topology(topology_file);
  cout << "## Number of routers in the network: " << router_num << endl;
  for (unsigned i = 0; i < router_num; ++i)
    routers.push_back(Router(&network, i));
}

void find_routes () {
  cout << "## Simulating routers' startup message exchange" << endl;
  for_each(bootstraps.begin(), bootstraps.end(), simulation_step);
}

void run_prompt (const string& progname) {
  cout << "## Inicando prompt..." << endl;
  while (cin.good()) {
    cout << progname << "$ ";
    cout.flush();
    string input;
    getline(cin, input);
    stringstream line(input);
    line >> input;
    if (input == "quit") return;
  }
  cout << endl;
}

static void simulation_step (const Bootstrap& bootstrap_method) {
  for_each(routers.begin(), routers.end(), mem_fn(bootstrap_method));
  // Uma vez executados o métodos de bootstrap, simula a troca de mensagens até
  // não haver mais mensagens enviadas entre os roteadores
  while (network.pending_msgs()) {
    Network::Packet packet = network.next_msg();
    routers[packet.id_receiver].receive_msg(packet.id_sender, packet.msg);
  }
}

} // namespace ep3

