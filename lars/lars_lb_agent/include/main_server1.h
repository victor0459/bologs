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
};

extern struct load_balance_config lb_config;
