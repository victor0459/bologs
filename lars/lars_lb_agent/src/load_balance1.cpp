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


