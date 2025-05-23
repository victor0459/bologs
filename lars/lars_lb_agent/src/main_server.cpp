#include "main_server.h"

thread_queue<lars::ReportStatusRequest>* report_queue = NULL;
thread_queue<lars::GetRouteRequest>* dns_queue = NULL;


//每个Agent UDP server   会对应一个 Route_lb
route_lb *r_lb[3];

struct load_balance_config lb_config;


void init_lb_agent() 
{
    //设置配置文件路径(只能执行一次)
    config_file::setPath("./conf/lb_agent.ini");

    lb_config.probe_num = config_file::instance()->GetNumber("loadbalance", "probe_num", 10);
    lb_config.init_succ_cnt = config_file::instance()->GetNumber("loadbalance", "init_succ_cnt", 180);
    lb_config.init_err_cnt = config_file::instance()->GetNumber("loadbalance", "init_err_cnt", 5);
    lb_config.err_rate = config_file::instance()->GetFloat("loadbalance", "err_rate", 0.1);
    lb_config.succ_rate = config_file::instance()->GetFloat("loadbalance", "succ_rate", 0.95);
    lb_config.contin_succ_limit = config_file::instance()->GetNumber("loadbalance", "contin_succ_limit", 10);
    lb_config.contin_err_limit = config_file::instance()->GetNumber("loadbalance", "contin_err_limit", 10);
    lb_config.idle_timeout = config_file::instance()->GetNumber("loadbalance", "idle_timeout", 15);
    lb_config.overload_timeout = config_file::instance()->GetNumber("loadbalance", "overload_timeout", 15);
    lb_config.update_timeout = config_file::instance()->GetNumber("loadbalance", "update_timeout", 15);


    for (int i = 0; i < 3 ; i++) {
        int id = i + 1; //route_lb 的id是从 1 开始计数的
        r_lb[i] = new route_lb(id);
        if (r_lb[i] == NULL) {
            fprintf(stderr, "create route lb  id = %d error\n", id);
            exit(1);
        }
    }

    //TODO 加载本地ip load_local_host t:11-11
    
}

int main(int argc, char **argv)
{
    init_lb_agent();

    //1 启动udp server服务器(3个 udp server) 用来接收(业务层-开发者-api层)发来的消息
    start_UDP_servers();
    
    //2 启动 lars_reporter client 线程
    report_queue = new thread_queue<lars::ReportStatusRequest>();
    if (report_queue == NULL) {
        fprintf(stderr, "create report queue error\n");
        exit(1);
    }
    start_report_client();

    
    //3 启动 lars_dns client 线程
    dns_queue = new thread_queue<lars::GetRouteRequest>();
    if (dns_queue == NULL) {
        fprintf(stderr, "create dns queue error\n");
        exit(1);
    }
    start_dns_client();

    
    
    //4 主线程应该是阻塞状态
    while (1) {
        sleep(10);
    }

    return 0;
}
