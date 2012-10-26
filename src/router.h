
#ifndef EP3_ROUTER_H_
#define EP3_ROUTER_H_

#include <string>
#include <istream>
#include <list>
#include <vector>

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
    void acknowledge_hello (unsigned id_sender, std::istream& msg);
    void acknowledge_neighbor (unsigned id_sender, std::istream& msg);
    // Métodos que calculam rotas
    double linkstate_route (unsigned id_target, std::vector<unsigned>& route) const;
    double distvector_route (unsigned id_target, std::vector<unsigned>& route) const;
  private:
    struct Neighbor {
      unsigned  id;
      double    delay;
    };
    Network*            network_;
    unsigned            id_;
    std::list<Neighbor> neighbors_;
};

} // namespace ep3

#endif

