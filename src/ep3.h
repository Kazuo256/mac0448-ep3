
#ifndef EP3_H_
#define EP3_H_

#include <string>

namespace ep3 {

/// Cria a rede e sua topologia a partir do arquivo passado.
void create_network (const std::string& topology_file);

/// Os roteadores trocam mensagens até montarem suas tabelas de roteamento.
void find_routes ();

/// Roda o prompt para o usuário solicitar rotas.
void run_prompt (const std::string& progname);

} // namespace ep3

#endif

