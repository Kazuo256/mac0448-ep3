
#ifndef EP3_ROUTER_H_
#define EP3_ROUTER_H_

namespace ep3 {

class Network;

class Router {
  public:
    Router (Network* network, unsigned id) :
      network_(network), id_(id) {}
    unsigned id () const { return id_; }
  private:
    Network* network_;
    unsigned id_;
};

} // namespace ep3

#endif

