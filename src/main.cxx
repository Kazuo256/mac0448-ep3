
#include "ep3.h"

#include <cstdlib>
#include <iostream>

using std::cout;

int main (int argc, char** argv) { 
  // Verifica argumentos
  if (argc != 2) {
    cout << "Uso:\n\t" << argv[0] << " <arquivo_de_topologia>\n";
    return EXIT_FAILURE;
  }
  // Executa lógica do programa
  ep3::create_network(argv[1]);
  ep3::find_routes();
  ep3::run_prompt(argv[0]);
  // Execução bem sucedida
  return EXIT_SUCCESS;
}

