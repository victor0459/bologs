#include "lars_reactor.h"
#include "dns_route.h"
#include "subcribe.h"
#include "lars.pb.h"


tcp_server *server;

typedef hash_set<uint64_t> client_sub_mod_list; //Agent客户端已经订阅的mod模块集合

//处理agent发送Route信息获取的业务
void get_route(const char *data, uint32_t len, int msgid, net_connection *conn, void *user_data)
{
    //1 解析proto文件
    lars::GetRouteRequest req;
    req.ParseFromArray(data, len);
    
    //2 得到 modID 和 cmdID
    int modid, cmdid;
    modid = req.modid();
    cmdid = req.cmdid();


    //2.5 如果之前没有订阅过modid/cmdid, 则订阅
    uint64_t mod = (((uint64_t)modid) << 32) + cmdid;
    client_sub_mod_list *sub_list = (client_sub_mod_list*)conn->param;
    if (sub_list == NULL) {
        fprintf(stderr, "sub_list = NULL\n");
    }
    else if (sub_list->find(mod) == sub_list->end()) {
        //说明当前mod是没有被订阅的(首次请求当前mod), 需要订阅
        sub_list->insert(mod);

        //订阅当前mod
        SubcribeList::instance()->subcribe(mod, conn->get_fd());
        printf("fd %d subscribe modid = %d, cmdid = %d\n", conn->get_fd(), modid, cmdid);
    }
    
    //3 通过modid/cmdid 获取host信息 从 _data_pointer所指向map中
    host_set hosts = Route::instance()->get_hosts(modid, cmdid);
    
    //4 打包一个新的response protobuf数据
    lars::GetRouteResponse rsp;
    rsp.set_modid(modid);
    rsp.set_cmdid(cmdid);

    for (host_set_it it = hosts.begin(); it != hosts.end(); it++) {
        //it就是set中的一个元素 ip+port 64位一个整形键值对
        uint64_t ip_port = *it;

        lars::HostInfo host;
        host.set_ip((uint32_t)(ip_port>>32));
        host.set_port((uint32_t)(ip_port));

        //将host添加到rsp 对象中
        rsp.add_host()->CopyFrom(host);
    }
    
    //5 发送给对方
    std::string responseString;
    rsp.SerializeToString(&responseString);
    conn->send_message(responseString.c_str(), responseString.size(), lars::ID_GetRouteResponse);
}

