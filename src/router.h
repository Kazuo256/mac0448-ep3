
#ifndef EP3_ROUTER_H_
#define EP3_ROUTER_H_

#include <string>

namespace ep3 {

class Network;

class Router {
  public:
    Router (Network* network, unsigned id) :
      network_(network), id_(id) {}
    unsigned id () const { return id_; }
    void receive_msg (unsigned id_sender, const std::string& msg);
    void start_up ();
  private:
    Network* network_;
    unsigned id_;
};

} // namespace ep3

#endif

