#pragma once
#include "lars_reactor.h"
#include "lars.pb.h"
#include <pthread.h>
#include "route_lb.h"

void start_UDP_servers(void);
void start_dns_client(void);
void start_report_client(void);

extern thread_queue<lars::ReportStatusRequest>* report_queue;
extern thread_queue<lars::GetRouteRequest>* dns_queue;
extern route_lb * r_lb[3];

struct load_balance_config
{
    //经过若干次请求host节点后， 试探1次overload的过载节点
    int probe_num;    
    //初始化一个host_info主机访问的成功次数，防止刚启动少量失败就认为过载
    int init_succ_cnt;
    int init_err_cnt;
    //当idle节点失败率高于此值，节点变为overload状态
    float err_rate;
};

extern struct load_balance_config lb_config;
