#include "main_server.h"


//dns service已经恢复当前modid/cmdid对应的全部的主机集合
void deal_recv_route(const char *data, uint32_t len, int msgid, net_connection *conn, void *user_data)
{
    lars::GetRouteResponse rsp;
    rsp.ParseFromArray(data, len);

    int modid = rsp.modid();
    int cmdid = rsp.cmdid();

    int index = (modid+cmdid) % 3;

    //将 modid/cmdid交给一个route_lb来处理，将rsp中的hostinfo集合加入到对应route_lb中 
    r_lb[index]->update_host(modid, cmdid, rsp);
}
//如果队列中有数据，所触发的一个回调业务
void new_dns_request(event_loop *loop, int fd, void *args)
{
    tcp_client *client = (tcp_client*)args;


}
