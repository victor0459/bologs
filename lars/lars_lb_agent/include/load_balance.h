#pragma once
#include "host_info.h"
#include "lars.pb.h"
#include <ext/hash_map>
#include <list>

//ip/port 作为主键 host信息集合 map
typedef __gnu_cxx::hash_map<uint64_t, host_info*> host_map; //key--> uint64(ip+port), value-->host_info
typedef __gnu_cxx::hash_map<uint64_t, host_info*>::iterator host_map_it;


typedef std::list<host_info*> host_list;
typedef std::list<host_info*>::iterator host_list_it;


