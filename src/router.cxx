
#include "router.h"
#include "network.h"

#include <iostream>

using std::string;
using std::cout;

namespace ep3 {

void Router::receive_msg (unsigned id_sender, const string& msg) {
  cout << "[ROUTER " << id_ << "] Message from " << id_sender << ":\n"
       << "\t\"" << msg << "\"\n";
}

void Router::start_up () {
  network_->local_broadcast(id_, "hello");
}

} // namespace ep3

