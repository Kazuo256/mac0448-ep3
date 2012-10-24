
#include "router.h"

#include <iostream>

using std::string;
using std::cout;

namespace ep3 {

void Router::receive_msg (unsigned id_sender, const string& packet) {
  cout << "[ROUTER " << id_ << "] Message from " << id_sender << ":\n"
       << "\t\"" << packet << "\"\n";
}

} // namespace ep3

