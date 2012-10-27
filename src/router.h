
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
    struct Neighbor {
      unsigned  id;
      double    delay;
    };
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
    // Métodos que calculam rotas
    double linkstate_route (unsigned id_target, std::vector<unsigned>& route);
    double distvector_route (unsigned id_target, std::vector<unsigned>& route);
    // Informações de debug
    void dump_linkstate_table () const;
    bool operator < (const Neighbor& rhs) const;
  private:
    typedef std::list<Neighbor> LinkState;
    Network*                                      network_;
    unsigned                                      id_;
    std::tr1::unordered_map<unsigned, LinkState>  linkstates_;
    std::tr1::unordered_set<unsigned>             pending_linkstates_;
    std::vector<unsigned>                         ls_route_;
    std::vector<double>                           ls_cost_;
    std::ostream& output () const {
      return std::cout << "[ROUTER " << id_ << "] ";
    }
};

} // namespace ep3

#endif

