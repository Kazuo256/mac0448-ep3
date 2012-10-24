
#ifndef EP3_H_
#define EP3_H_

#include <string>

namespace ep3 {

void create_network (const std::string& topology_file);

void find_routes ();

void run_prompt (const std::string& progname);

} // namespace ep3

#endif

