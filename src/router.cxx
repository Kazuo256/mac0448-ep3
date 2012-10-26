
#include "router.h"
#include "network.h"

#include <iostream>
#include <utility>
#include <stack>
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
using std::tr1::unordered_set;
using std::stack;

namespace ep3 {

typedef void (Router::*MsgHandler) (unsigned, istream&);

const static pair<string, MsgHandler> handler_list[] = {
  make_pair("HELLO", &Router::acknowledge_hello),
  make_pair("ACK_HELLO", &Router::acknowledge_neighbor),
  make_pair("REQ_LINKSTATE", &Router::respond_linkstate),
  make_pair("ANSWER_LINKSTATE", &Router::receive_linkstate),
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
    stringstream request;
    request << "REQ_LINKSTATE" << sep << id_ << sep << it->id;
    network_->send(id_, it->id, request.str());
    pending_linkstates_.insert(it->id);
  }
}

void Router::distvector_begin () {
  //network_->local_broadcast(id_, "HELLO");
}

// Métodos que tratam mensagens

void Router::acknowledge_hello (unsigned id_sender, istream& args) {
  network_->send(id_, id_sender, "ACK_HELLO");
}

void Router::acknowledge_neighbor (unsigned id_sender, istream& args) {
  Neighbor neighbor = { id_sender, network_->get_delay(id_, id_sender) };
  linkstates_[id_].push_back(neighbor);
  cout << "[ROUTER " << id_ << "] Acknowledges neighbor " << id_sender << "."
       << endl;
}

void Router::respond_linkstate (unsigned id_sender, istream& args) {
  unsigned id_origin, id_destiny;
  args >> id_origin >> id_destiny;
  if (id_destiny == id_) {
    stringstream answer;
    answer << "ANSWER_LINKSTATE" << sep << id_ << sep << id_origin;
    stack<unsigned> path;
    path.push(id_origin);
    while (!args.eof()) {
      unsigned id;
      args >> id;
      path.push(id);
    }
    unsigned next = path.top();
    path.pop();
    while (path.size() > 1) {
      answer << sep << path.top();
      path.pop();
    }
    if (!path.empty())
      answer << sep << "|";
    LinkState &neighbors = linkstates_[id_];
    for (list<Neighbor>::iterator it = neighbors.begin();
         it != neighbors.end(); ++it)
      answer << sep << it->id << ":" << it->delay;
    network_->send(id_, next, answer.str());
  } else {
    stringstream            request;
    unordered_set<unsigned> invalid;
    invalid.insert(id_origin);
    invalid.insert(id_sender);
    request << "REQ_LINKSTATE" << sep << id_origin << sep << id_destiny;
    cout << "[ROUTER " << id_ << "] Repassing packet from " << id_sender << endl;
    while (!args.eof()) {
      unsigned id;
      args >> id;
      invalid.insert(id);
      request << sep << id;
    }
    request << sep << id_;
    LinkState &neighbors = linkstates_[id_];
    for (list<Neighbor>::iterator it = neighbors.begin();
         it != neighbors.end(); ++it)
      if (it->id == id_destiny) {
        network_->send(id_, id_destiny, request.str());
        return;
      }
    for (list<Neighbor>::iterator it = neighbors.begin();
         it != neighbors.end(); ++it)
      if (invalid.count(it->id) == 0)
        network_->send(id_, it->id, request.str());
  }
}

void Router::receive_linkstate (unsigned id_sender, istream& args) {
  unsigned id_origin, id_destiny;
  args >> id_origin >> id_destiny;
  if (id_destiny == id_ && linkstates_.count(id_origin) == 0) {
    pending_linkstates_.erase(id_origin);
    LinkState neighbors;
    // Lê os vizinhos
    while (!args.eof()) {
      string neighbor_data;
      args >> neighbor_data;
      size_t    div = neighbor_data.find(":");
      unsigned  id;
      double    delay;
      stringstream(neighbor_data.substr(0,div)) >> id;
      stringstream(neighbor_data.substr(div+1)) >> delay;
      cout  << "[ROUTER " << id_ << "] Received linkstate from " << id_origin
            << ": neighbor " << id << " delay " << delay << endl;
      Neighbor neighbor = { id, delay };
      neighbors.push_back(neighbor);
    }
    // Manda mais requisições para os que não conhece ainda
    for (LinkState::iterator it = neighbors.begin();
         it != neighbors.end(); ++it)
      if (linkstates_.count(it->id) == 0 && pending_linkstates_.count(it->id) == 0) {
        stringstream request;
        request << "REQ_LINKSTATE" << sep << id_ << sep << it->id;
        network_->local_broadcast(id_, request.str());
        pending_linkstates_.insert(it->id);
      }
    linkstates_[id_origin] = neighbors;
  } else {
    
  }
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

