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


/*
 * 负载均衡模块
 * 是针对一个(modid/cmdid)下的全部的host节点做负载规则
 * */
class load_balance {
public:
    load_balance(int modid, int cmdid) {
        _modid = modid;
        _cmdid = cmdid;
        _access_cnt = 0;
        status = NEW;
        last_update_time = time(NULL);    
    }

    bool empty() {
        return _host_map.empty();
    }

private:
    int _modid;
    int _cmdid;

    //host_map 当前负载均衡模块所管理的全部主机
    host_map _host_map;
    
    //空闲队列
    host_list _idle_list; 
    
    //过载队列
    host_list _overload_list;

    //当前modid/cmdid的请求次数
    int _access_cnt;

};