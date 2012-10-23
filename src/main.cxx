
#include "network.h"

#include <cstdlib>
#include <cstdio>

using std::printf;
using ep3::Network;

int main (int argc, char** argv) { 
  if (argc != 2) {
    printf("Uso:\n\t%s <arquivo_de_topologia>\n", argv[0]);
    return EXIT_FAILURE;
  }
  Network network;
  network.load_topology(argv[1]);
  return EXIT_SUCCESS;
}

