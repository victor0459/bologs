#pragma once
#include "load_balance.h"


//key-->modid/cmdid  value--> load_balance
typedef __gnu_cxx::hash_map<uint64_t, load_balance*> route_map;
typedef __gnu_cxx::hash_map<uint64_t, load_balance*>::iterator route_map_it;


