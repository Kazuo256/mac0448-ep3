
#ifndef EP3_ROUTER_H_
#define EP3_ROUTER_H_

#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

namespace ep3 {

class Network;

class Router {
  public:
    Router (Network* network, unsigned id) :
      network_(network), id_(id) {}
    // Métodos básicos.
    unsigned id () const { return id_; }
    void receive_msg (unsigned id_sender, const std::string& msg);
    // Métodos de bootstrap
    void start_up ();
    void linkstate_begin ();
    void distvector_begin ();
    // Métodos que tratam mensagens
    void acknowledge_hello (unsigned id_sender, std::stringstream& args);
    void acknowledge_neighbor (unsigned id_sender, std::stringstream& args);
    void respond_linkstate (unsigned id_sender, std::stringstream& args);
    void receive_linkstate (unsigned id_sender, std::stringstream& args);
    void receive_distvector (unsigned id_sender, std::stringstream& args);
    // Métodos que calculam rotas
    double linkstate_route_ms (unsigned id_target, std::vector<unsigned>& route);
    double linkstate_route_hop (unsigned id_target, std::vector<unsigned>& route);
    double distvector_route_ms (unsigned id_target, std::vector<unsigned>& route);
    double distvector_route_hop (unsigned id_target, std::vector<unsigned>& route);
    // Informações de debug
    void dump_linkstate_table () const;
    double delay (unsigned origin, unsigned destiny);
    bool comp_ms (unsigned id_1, unsigned id_2) const;
    bool comp_hop (unsigned id_1, unsigned id_2) const;
  private:
    Network*                                      network_;
    unsigned                                      id_;
    std::tr1::unordered_map<unsigned, double>     neighbors_;
    // Informações de estado de enlace
    struct Neighbor {
      unsigned  id;
      double    delay;
    };
    typedef std::list<Neighbor> LinkState;
    std::tr1::unordered_map<unsigned, LinkState>  linkstates_;
    std::tr1::unordered_set<unsigned>             pending_linkstates_;
    std::vector<unsigned>                         ls_route_ms_;
    std::vector<double>                           ls_cost_ms_;
    std::vector<unsigned>                         ls_route_hop_;
    std::vector<double>                           ls_cost_hop_;
    // Informações de vetor de distância
    struct Dist {
      double    delay;
      size_t    hops;
    };
    typedef std::tr1::unordered_map<unsigned, Dist> DistVector;
    std::tr1::unordered_map<unsigned, DistVector> distvectors_;
    // Envia o vetor de distâncias para todos os vizinhos.
    void send_distvector ();
    // Método para formatar a saída do roteador.
    std::ostream& output () const {
      return std::cout << "[ROUTER " << id_ << "] ";
    }
};

} // namespace ep3

#endif

