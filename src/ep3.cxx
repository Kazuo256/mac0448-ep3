
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

// Trata uma linha de comando do prompt do programa. O retorno indica se o
// prompt deve continuar ou não. Se for falso, o prompt deve terminar.
static bool handle_command (stringstream& command);

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
    string line;
    getline(cin, line);
    stringstream command(line);
    if (!handle_command(command)) return;
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

static bool check_id (unsigned id) {
  if (id >= routers.size()) {
    cout << "## No router with ID " << id << "." << endl;
    return false;
  }
  return true;
}

static bool check_args (const stringstream& command) {
  if (command.eof()) {
    cout << "## Missing arguments." << endl;
    return false;
  }
  return true;
}

typedef double (Router::*RoutingMethod) (unsigned, vector<unsigned>&) const;

static bool handle_command (stringstream& command) {
  // Um monte de código feio...
  // Começa verificando a primeira palavra do comando
  string cmd_name;
  command >> cmd_name;
  if (cmd_name == "quit") return false; // Interrompe o prompt
  // Detecta qual algoritmo de roteamento solicitado
  RoutingMethod method = NULL;
  if (cmd_name == "ee") method = &Router::linkstate_route;
  if (cmd_name == "vd") method = &Router::distvector_route;
  if (method) {
    // Sabendo o algoritmo, processamos os argumentos
    // Primeiro vem os IDs de origem e destino
    unsigned id_origin, id_destiny;
    if (!check_args(command)) return true;
    command >> id_origin;
    if (!check_id(id_origin)) return true;
    if (!check_args(command)) return true;
    command >> id_destiny;
    if (!check_id(id_destiny)) return true;
    // Depois a métrica desejada
    string metric;
    if (!check_args(command)) return true;
    command >> metric;
    // Enfim fazemos o roteador calcular a rota
    cout << "## Finding route..." << endl;
    vector<unsigned> route;
    double total_delay = (routers[id_origin].*method) (id_destiny, route);
    // E exibimos a rota para o usuário
    for (vector<unsigned>::iterator it = route.begin(); it != route.end(); ++it)
      cout << *it << " ";
    cout << "(";
    if (metric == "h")
      cout << route.size() << " hops";
    else if (metric == "a")
      cout << total_delay << " milisegundos";
    cout << ")" << endl;
  }
  else if (!cmd_name.empty())
    cout << "## Unknown comman '" << cmd_name << "'." << endl;
  return true;
}

} // namespace ep3

