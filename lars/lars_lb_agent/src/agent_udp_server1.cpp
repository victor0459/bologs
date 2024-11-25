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
    //request
    lars::GetHostRequest req;
    req.ParseFromArray(data, len);
}
