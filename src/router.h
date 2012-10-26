
#ifndef EP3_ROUTER_H_
#define EP3_ROUTER_H_

#include <string>
#include <istream>
#include <list>
#include <vector>
#include <queue>
#include <tr1/unordered_map>

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
    void acknowledge_hello (unsigned id_sender, std::istream& args);
    void acknowledge_neighbor (unsigned id_sender, std::istream& args);
    void respond_linkstate (unsigned id_sender, std::istream& args);
    void receive_linkstate (unsigned id_sender, std::istream& args);
    // Métodos que calculam rotas
    double linkstate_route (unsigned id_target, std::vector<unsigned>& route) const;
    double distvector_route (unsigned id_target, std::vector<unsigned>& route) const;
  private:
    struct Neighbor {
      unsigned  id;
      double    delay;
    };
    typedef std::list<Neighbor> LinkState;
    Network*                                      network_;
    unsigned                                      id_;
    std::tr1::unordered_map<unsigned, LinkState>  linkstates_;
    std::queue<unsigned>                          pending_linkstates_;
    void send_linkstate_reqs (LinkState& targets) const;
};

} // namespace ep3

#endif

