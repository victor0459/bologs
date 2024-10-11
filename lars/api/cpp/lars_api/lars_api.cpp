#include "lars_api.h"
//#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include "unistd.h"
#include "lars.pb.h"

//using namespace std;

#define MESSAGE_HAED_LEN 8


lars_client::lars_client()
{
    _seqid = 0;

    //1 初始化服务器地址
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //默认的ip地址是本地，api和agent应该部署在同一台主机上
    inet_aton("127.0.0.1", &servaddr.sin_addr);
    
    //2 创建3个UDP server
    for (int i = 0; i < 3; i ++) {
        _sockfd[i] = socket(AF_INET, SOCK_DGRAM|SOCK_CLOEXEC, IPPROTO_UDP);
        if (_sockfd[i] == -1) {
            perror("socket()");
            exit(1);
        }

        //本连接对应udp server的端口号
        servaddr.sin_port = htons(8888+i);

        //连接
        int ret = connect(_sockfd[i], (const struct sockaddr *)&servaddr, sizeof(servaddr));
        if (ret == -1) {
            perror("connect()\n");
            exit(1);
        }

 //       cout <<"connection agent udp server " << 8888+i << "succ!" <<endl;
    }
}

lars_client::~lars_client()
{
    //关闭3个链接
    for (int i = 0; i < 3 ; i ++) {
        close(_sockfd[i]);
    }
}

//注册一个模块
int lars_client::reg_init(int modid, int cmdid)
{
    route_set route;

    int retry_cnt = 0;

    while (route.empty() && retry_cnt < 3) {

        get_route(modid, cmdid, route);

        if (route.empty() == true) {
            usleep(50000); //等待50ms
        }
        else {
            break;
        }

        ++retry_cnt;
    }


    if (route.empty() == true) {
        return lars::RET_NOEXIST;//3
    }

    return lars::RET_SUCC; //0
}

