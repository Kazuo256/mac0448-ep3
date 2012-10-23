
#ifndef EP3_ROUTER_H_
#define EP3_ROUTER_H_

namespace ep3 {

class Router {
  public:
    Router (unsigned id) : id_(id) {}
    unsigned id () const { return id_; }
  private:
    unsigned id_;
};

} // namespace ep3

#endif

