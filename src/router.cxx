
#include "router.h"
#include "network.h"

#include <iostream>
#include <utility>
#include <tr1/unordered_map>

using std::string;
using std::cout;
using std::pair;
using std::make_pair;
using std::tr1::unordered_map;

namespace ep3 {

typedef void (Router::*MsgHandler) (unsigned, const string&);

const static pair<string, MsgHandler> handler_list[] = {
  make_pair("hello", &Router::acknowledge_neighbor),
};

const static pair<string, MsgHandler> *const handler_end =
  handler_list+sizeof(handler_list)/sizeof(pair<string,MsgHandler>);

const static unordered_map<string, MsgHandler> handlers(handler_list,
                                                        handler_end);

void Router::receive_msg (unsigned id_sender, const string& msg) {
  unordered_map<string, MsgHandler>::const_iterator it = handlers.find(msg);
  if (it != handlers.end())
    (this->*(it->second))(id_sender, msg);
}

void Router::start_up () {
  network_->local_broadcast(id_, "hello");
}

void Router::linkstate_begin () {
  network_->local_broadcast(id_, "hello");
}

void Router::distvector_begin () {
  network_->local_broadcast(id_, "hello");
}

void Router::acknowledge_neighbor (unsigned id_sender, const string& msg) {
  cout << "[ROUTER " << id_ << "] Message from " << id_sender << ":\n"
       << "\t\"" << msg << "\"\n";
}

} // namespace ep3

