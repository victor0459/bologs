#include "main_server.h"

void report_cb(const char *data, uint32_t len, int msgid, net_connection *conn, void *user_data)
{
    lars::ReportRequest req;
    req.ParseFromArray(data, len);

    route_lb *route_lb_p = (route_lb*)user_data;
    route_lb_p->report_host(req);
}
void get_host_cb(const char *data, uint32_t len, int msgid, net_connection *conn, void *user_data)
{
    lars::GetHostRequest req;
    req.ParseFromArray(data, len);
    int modid = req.modid();
    int cmdid = req.cmdid();
    //设置回复的消息
    lars::GetHostResponse rsp;
    rsp.set_seq(req.seq());
    rsp.set_modid(modid);
    rsp.set_cmdid(cmdid);
    //通过 route_lb 获取一个可用host 添加到rsp中
    route_lb *route_lb_p = (route_lb*)user_data;
    route_lb_p->get_host(modid, cmdid, rsp);
    //将 rsp 发送回给 api 
    std::string responseString;
    rsp.SerializeToString(&responseString);
    conn->send_message(responseString.c_str(), responseString.size(), lars::ID_GetHostResponse);
}