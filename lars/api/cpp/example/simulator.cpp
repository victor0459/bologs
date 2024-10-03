
#include "lars_api.h"
#include <iostream>

void usage()
{
    printf("usage : ./simulator [modid] [cmdid] [errRate(0-10)] [query cnt(0-999999)]\n");
}

//开发者业务模块
int main(int argc, char **argv)
{

    if (argc < 3) {
        usage();
        return 1;
    }

    int modid = atoi(argv[1]);
    int cmdid = atoi(argv[2]);
    int err_rate = 2;//20%
    int query_cnt = 100;

    if (argc > 3) {
        err_rate = atoi(argv[3]);
    }

    if (argc > 4) {
        query_cnt = atoi(argv[4]);
    }

    //key --> ip,  value--><succ_cnt, err_cnt>
    std::map<std::string, std::pair<int, int>> result;

    int ret = 0;
    std::string ip;
    int port ;
    lars_client api;
    std::cout <<"err_rate = " <<err_rate << std::endl; 

    //1 将modid/cmdid注册(只调用一次)
    ret = api.reg_init(modid, cmdid) ;
    if (ret != 0) {
        std::cout <<"modid " << modid << "cmdid " << cmdid <<" not exist, register error ret = " << ret << std::endl;
    }
   

    return 0;
}
