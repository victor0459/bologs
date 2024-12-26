#include "load_balance.h"
#include "main_server.h"

int load_balance::pull()
{
    //封装一个dns 请求包
    lars::GetRouteRequest req;
    req.set_modid(_modid);
    req.set_cmdid(_cmdid);
    
    //将这个包 发送 dns_queue即可
    dns_queue->send(req);

    status = PULLING;
    printf("modid %d, cmdid %d pulling from dns service!\n", _modid, _cmdid);

    return 0;
}
//根据Dns service远程返回的主机结果，更新自己的host_map表
void load_balance::update(lars::GetRouteResponse &rsp)
{
    long current_time = time(NULL);

    std::set<uint64_t> remote_hosts;
    std::set<uint64_t> need_delete;

    //1 确保dns service 返回的结果有host信息 
    assert(rsp.host_size() != 0);

    //2. 插入新增的host信息到_host_map中
    for (int i = 0; i < rsp.host_size(); i ++) {
        const lars::HostInfo &h = rsp.host(i);

        //得到主机的ip+port key值
        uint64_t key = ((uint64_t)h.ip() << 32) + h.port();

        //将远程的主机加入到remote_hosts集合中
        remote_hosts.insert(key);

        if (_host_map.find(key) == _host_map.end()) {
            //如果当前的_host_map找不到当下的key，说明是新增的
            host_info *hi = new host_info(h.ip(), h.port(), lb_config.init_succ_cnt);
            if (hi == NULL) {
                fprintf(stderr, "new host_info error\n");
                exit(1);
            }
            _host_map[key] = hi;

            //将新增的host信息加入到 空闲列表中
            _idle_list.push_back(hi);
        }
    }
    
    //3. 遍历本地map和远程dns返回的主机集合，得到需要被删除的主机
    for (host_map_it it = _host_map.begin(); it != _host_map.end(); it ++)    {
       if (remote_hosts.find(it->first) == remote_hosts.end())  {
           //代表当前主机，没有在远程的返回的主机中存在，说明当前主机已经被修改或者删除
           need_delete.insert(it->first);
       }
    }

    //4 删除主机
    for (std::set<uint64_t>::iterator it = need_delete.begin(); it != need_delete.end(); it++) {
        uint64_t key = *it;

        host_info *hi = _host_map[key];

        if (hi->overload == true) {
            //从过载列表中删除
            _overload_list.remove(hi);
        }
        else {
            //从空闲列表中删除
            _idle_list.remove(hi);
        }

        delete hi;
    }

    //更新最后的update时间
    last_update_time = current_time;
    //重置为NEW状态
    status = NEW;

    printf("update hosts from Dns Service modid %d, cmdid %d\n", _modid, _cmdid);
}
//从idle_list或者overload_list中得到一个host节点信息
void get_host_from_list(lars::GetHostResponse &rsp, host_list &l)
{
    //从list中选择第一个节点
    host_info *host = l.front();

    //将取出的host节点信息添加给rsp的hostInfo字段
    lars::HostInfo *hip = rsp.mutable_host();
    hip->set_ip(host->ip);
    hip->set_port(host->port);

    l.pop_front();
    l.push_back(host);
}
