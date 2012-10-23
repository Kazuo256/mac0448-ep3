
#ifndef EP3_NETWORK_H_
#define EP3_NETWORK_H_

#include "router.h"

#include <string>
#include <vector>

namespace ep3 {

class Network {
  public:
    Network () {}
    void load_topology (const std::string& topology_file);
  private:
    typedef std::vector< std::vector<double> > Topology;
    std::vector<Router> routers_;
    Topology            topology_;
};

} // namespace ep3

#endif

