
#include "router.h"
#include "network.h"

#include <iostream>
#include <utility>
#include <tr1/unordered_map>

using std::list;
using std::string;
using std::istream;
using std::stringstream;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;
using std::tr1::unordered_map;

namespace ep3 {

typedef void (Router::*MsgHandler) (unsigned, istream&);

const static pair<string, MsgHandler> handler_list[] = {
  make_pair("hello", &Router::acknowledge_neighbor),
};

const static pair<string, MsgHandler> *const handler_end =
  handler_list+sizeof(handler_list)/sizeof(pair<string,MsgHandler>);

const static unordered_map<string, MsgHandler> handlers(handler_list,
                                                        handler_end);

void Router::receive_msg (unsigned id_sender, const string& msg) {
  stringstream tokens(msg);
  string header;
  tokens >> header;
  unordered_map<string, MsgHandler>::const_iterator it = handlers.find(header);
  if (it != handlers.end())
    (this->*(it->second))(id_sender, tokens);
}

void Router::start_up () {
  network_->local_broadcast(id_, "hello");
}

const static string sep = " ";

void Router::linkstate_begin () {
  stringstream msg;
  msg << "linkstate" << sep << id_;
  for (list<Neighbor>::iterator it = neighbors_.begin();
       it != neighbors_.end(); ++it)
    msg << sep << it->id << ":" << it->delay;
  network_->local_broadcast(id_, msg.str());
}

void Router::distvector_begin () {
  //network_->local_broadcast(id_, "hello");
}

void Router::acknowledge_neighbor (unsigned id_sender, istream& tokens) {
  Neighbor neighbor = { id_sender, network_->get_delay(id_, id_sender) };
  neighbors_.push_back(neighbor);
  cout << "[ROUTER " << id_ << "] Acknowledges neighbor " << id_sender << "."
       << endl;
}

} // namespace ep3

