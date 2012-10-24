
#ifndef EP3_NETWORK_H_
#define EP3_NETWORK_H_

#include <string>
#include <vector>
#include <queue>

namespace ep3 {

class Network {
  public:
    struct Packet {
      Packet (unsigned id_sender, unsigned id_receiver, const std::string& msg)
        : id_sender_(id_sender), id_receiver_(id_receiver), msg_(msg) {}
      unsigned    id_sender_,
                  id_receiver_;
      std::string msg_;
    };
    Network () {}
    size_t load_topology (const std::string& topology_file);
    double get_delay (unsigned id_sender, unsigned id_receiver) const;
    void local_broadcast (unsigned id_sender, const std::string& msg);
    void send (unsigned id_sender, unsigned id_receiver,
               const std::string& msg);
    bool pending_msgs () const { return !packets_.empty(); }
    Packet next_msg ();
  private:
    typedef std::vector< std::vector<double> > Topology;
    Topology            topology_;
    std::queue<Packet>  packets_;
};

} // namespace ep3

#endif

