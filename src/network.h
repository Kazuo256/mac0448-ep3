
#ifndef EP3_NETWORK_H_
#define EP3_NETWORK_H_

#include "router.h"

#include <string>
#include <vector>

namespace ep3 {

class Network {
  public:
    Network () {}
    size_t load_topology (const std::string& topology_file);
    double get_delay (unsigned id_sender, unsigned id_receiver) const;
  private:
    typedef std::vector< std::vector<double> > Topology;
    Topology            topology_;
};

} // namespace ep3

#endif

