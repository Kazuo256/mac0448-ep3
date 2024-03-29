
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
#include <tr1/functional>

namespace ep3 {

class Network;

class Router {
  public:
    Router (Network* network, unsigned id) :
      network_(network), id_(id), lastcost_(0.0) {}
    //== Métodos básicos ==//
    unsigned id () const { return id_; }
    void receive_msg (unsigned id_sender, const std::string& msg);
    //== Métodos de bootstrap ==//
    void start_up ();
    void linkstate_begin ();
    void distvector_begin ();
    //== Métodos que tratam mensagens ==//
    void acknowledge_hello (unsigned id_sender, std::stringstream& args);
    void acknowledge_neighbor (unsigned id_sender, std::stringstream& args);
    void respond_linkstate (unsigned id_sender, std::stringstream& args);
    void receive_linkstate (unsigned id_sender, std::stringstream& args);
    void receive_distvector (unsigned id_sender, std::stringstream& args);
    void route_ms (unsigned id_sender, std::stringstream& args);
    void route_hop (unsigned id_sender, std::stringstream& args);
    //== Métodos para calcular rotas ==//
    // Usados para estado de enlace:
    double linkstate_route_ms (unsigned id_target, std::vector<unsigned>& route);
    double linkstate_route_hop (unsigned id_target, std::vector<unsigned>& route);
    double delay (unsigned origin, unsigned destiny);
    bool comp_ms (unsigned id_1, unsigned id_2) const;
    bool comp_hop (unsigned id_1, unsigned id_2) const;
    // Usados para vetor de distâncias:
    void distvector_route_ms (unsigned id_target);
    void distvector_route_hop (unsigned id_target);
    double distvector_extract_route (std::vector<unsigned>& route);
    //== Informações de debug ==//
    void dump_linkstate_table () const;
  private:
    Network*                                      network_;
    unsigned                                      id_;
    std::tr1::unordered_map<unsigned, double>     neighbors_;
    //== Informações de estado de enlace ==//
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
    //== Informações de vetor de distância ==//
    struct Dist {
      double    delay;
      size_t    hops;
      double get_delay () const { return delay; }
      unsigned get_hops () const { return hops; }
    };
    typedef std::tr1::unordered_map<unsigned, Dist>   DistVector;
    typedef std::tr1::function<double (const Dist&)>  Metric;
    std::tr1::unordered_map<unsigned, DistVector> distvectors_;
    std::vector<unsigned>                         lastroute_;
    double                                        lastcost_;
    // Envia o vetor de distâncias para todos os vizinhos
    void send_distvector ();
    // Lida com requisição de roteamento
    void dv_handle_route (std::stringstream& args, Metric metric,
                          const std::string& metric_name);
    // Segue a rota do algoritmo de vetor de distâncias
    void dv_follow_route (unsigned id_target, double cost,
                          const std::string& path, Metric metric,
                          const std::string& metric_name);
    // Encontra próximo passo da rota
    unsigned dv_next_step (unsigned id_target, Metric metric);
    //== Outros ==//
    // Método para formatar a saída do roteador.
    std::ostream& output () const {
      return std::cout << "[ROUTER " << id_ << "] ";
    }
};

} // namespace ep3

#endif

