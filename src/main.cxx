
#include "network.h"

#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;
using ep3::Network;

int main (int argc, char** argv) { 
  if (argc != 2) {
    cout << "Uso:\n\t" << argv[0] << " <arquivo_de_topologia>\n";
    return EXIT_FAILURE;
  }
  Network network;
  size_t router_num = network.load_topology(argv[1]);
  cout << "Number of routers in the network: " << router_num << endl;
  return EXIT_SUCCESS;
}

