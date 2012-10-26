
#include "router.h"
#include "network.h"

#include <iostream>
#include <utility>
#include <tr1/unordered_map>

using std::list;
using std::vector;
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
  make_pair("HELLO", &Router::acknowledge_hello),
  make_pair("ACK_HELLO", &Router::acknowledge_neighbor),
  make_pair("REQ_LINKSTATE", &Router::respond_linkstate),
};

const static pair<string, MsgHandler> *const handler_end =
  handler_list+sizeof(handler_list)/sizeof(pair<string,MsgHandler>);

const static unordered_map<string, MsgHandler> handlers(handler_list,
                                                        handler_end);

// Métodos básicos.

void Router::receive_msg (unsigned id_sender, const string& msg) {
  stringstream tokens(msg);
  string header;
  tokens >> header;
  unordered_map<string, MsgHandler>::const_iterator it = handlers.find(header);
  if (it != handlers.end())
    (this->*(it->second))(id_sender, tokens);
}

// Métodos de bootstrap

const static string sep = " ";

void Router::start_up () {
  linkstates_[id_] = LinkState();
  network_->local_broadcast(id_, "HELLO");
}

void Router::linkstate_begin () {
  LinkState &neighbors = linkstates_[id_];
  for (list<Neighbor>::iterator it = neighbors.begin();
       it != neighbors.end(); ++it) {
    stringstream msg;
    msg << "REQ_LINKSTATE" << sep << id_ << sep << it->id;
    network_->send(id_, it->id, msg.str());
  }
  //---
  //stringstream msg;
  //msg << "REQ_LINKSTATE" << sep << id_;
  //for (list<Neighbor>::iterator it = linkstates_[id_].begin();
  //     it != linkstates_[id_].end(); ++it)
  //  msg << sep << it->id << ":" << it->delay;
  //network_->local_broadcast(id_, msg.str());
}

void Router::distvector_begin () {
  //network_->local_broadcast(id_, "HELLO");
}

// Métodos que tratam mensagens

void Router::acknowledge_hello (unsigned id_sender, istream& tokens) {
  network_->send(id_, id_sender, "ACK_HELLO");
}

void Router::acknowledge_neighbor (unsigned id_sender, istream& tokens) {
  Neighbor neighbor = { id_sender, network_->get_delay(id_, id_sender) };
  linkstates_[id_].push_back(neighbor);
  cout << "[ROUTER " << id_ << "] Acknowledges neighbor " << id_sender << "."
       << endl;
}

void Router::respond_linkstate (unsigned id_sender, istream& tokens) {

}

// Métodos que calculam rotas

double Router::linkstate_route (unsigned id_target, vector<unsigned>& route) const {
  route.push_back(id_);
  return 0.0;
}

double Router::distvector_route (unsigned id_target, vector<unsigned>& route) const {
  route.push_back(id_);
  return 0.0;
}

} // namespace ep3

