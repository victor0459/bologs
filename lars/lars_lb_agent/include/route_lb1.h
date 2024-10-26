#pragma once
#include "load_balance.h"


//key-->modid/cmdid  value--> load_balance
typedef __gnu_cxx::hash_map<uint64_t, load_balance*> route_map;
typedef __gnu_cxx::hash_map<uint64_t, load_balance*>::iterator route_map_it;

class route_lb
{
public:
    route_lb(int id);

    //agent 获取一个host主机 将返回的主机结果存放在rsp中
    int get_host(int modid, int cmdid, lars::GetHostResponse &rsp);

private:
    route_map _route_lb_map;  //当前route_lb模块所管理的loadbalance集合
    pthread_mutex_t _mutex;
    int _id;                  //当前route_lb 的编号 和 udp server是一一对应的
};