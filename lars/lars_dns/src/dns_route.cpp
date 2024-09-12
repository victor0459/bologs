#include "dns_route.h"
#include "subcribe.h"
#include "config_file.h"
#include <stdio.h>
#include <string.h>


using namespace std;

//单例初始化
Route *Route::_instance = NULL;
pthread_once_t Route::_once = PTHREAD_ONCE_INIT;

Route::Route():_version(0) {
    printf("Route init\n");
    memset(_sql, 0, 1000);

    //初始化锁
    pthread_rwlock_init(&_map_lock, NULL);

    //初始化 map
    _data_pointer = new route_map(); //RouteDataMap(A)
    _temp_pointer = new route_map(); //RouteDataMap(B)

    //链接数据库
    this->connect_db();


    //将数据库中的RouteData的数据加载到_data_pointer中
    this->build_maps(); 
}

//加载当前版本
//return 0 成功 version是没有改变的
//return 1 成功 version有改变
//return -1 失败
int Route::load_version()
{
    memset(_sql, 0, 1000);
    snprintf(_sql, 1000,  "SELECT version from RouteVersion WHERE id = 1;");

    int ret = mysql_real_query(&_db_conn, _sql, strlen(_sql));
    if (ret != 0) {
        fprintf(stderr, "select RouteVersion error %s\n", mysql_error(&_db_conn));
        exit(1);
    }

    //获得一个结果集合
    MYSQL_RES *result = mysql_store_result(&_db_conn);

    //得到行数
    long line_num = mysql_num_rows(result);
    if (line_num == 0) {
        fprintf(stderr, "No version in table Routeversion: %s\n", mysql_error(&_db_conn));
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    //得到最新的version
    long new_version = atol(row[0]);

    if (new_version == this->_version) {
        return 0;
    }

    this->_version = new_version;

    printf("now route version is %ld\n", this->_version);
    

    mysql_free_result(result);

    return 1;
}


//加载RouteChange 得到修改的modid/cmdid
//放在vector中 
void Route::load_changes(std::vector<uint64_t>& change_list)
{
    memset(_sql, 0, 1000);

    //得去最新的修改版本数据
    snprintf(_sql, 1000, "SELECT modid,cmdid FROM RouteChange WHERE version >= %ld;", this->_version);

    int ret = mysql_real_query(&_db_conn, _sql, strlen(_sql));
    if (ret != 0) {
        fprintf(stderr, "select RouteChange error %s\n", mysql_error(&_db_conn));
        exit(1);
    }


    //获得一个结果集合
    MYSQL_RES *result = mysql_store_result(&_db_conn);

    //得到行数
    long line_num = mysql_num_rows(result);
    if (line_num == 0) {
        fprintf(stderr, "no Change in RouteChange: %s\n", mysql_error(&_db_conn));
        return ;
    }

    MYSQL_ROW row; 
    for (long i = 0; i < line_num; i ++) {
        row = mysql_fetch_row(result);
        int modid = atoi(row[0]);
        int cmdid = atoi(row[1]);

        uint64_t mod = (((uint64_t)modid) << 32) + cmdid;
        change_list.push_back(mod);
    }


    mysql_free_result(result);

    return ;
}

void Route::build_maps() 
{
    int ret = 0;
    memset(_sql, 0, 1000);

    //查询RouteData数据库 
    //"Select * from RouteData;"
    snprintf(_sql, 1000, "SELECT * FROM RouteData;");
    ret = mysql_real_query(&_db_conn, _sql, strlen(_sql));
    if (ret != 0) {
        fprintf(stderr, "select RouteData error %s\n", mysql_error(&_db_conn));
        exit(1);
    }
    
    //获得一个结果集合
    MYSQL_RES *result = mysql_store_result(&_db_conn);

    //得到行数
    long line_num = mysql_num_rows(result);
    
    //遍历分析集合中的元素， 加入_data_pointer中(MapA中)
    MYSQL_ROW row;
    for (int i = 0; i < line_num; i++) {
        //处理一行的数据
        row = mysql_fetch_row(result);
        int modID = atoi(row[1]);
        int cmdID = atoi(row[2]);
        unsigned int ip = atoi(row[3]);
        int port = atoi(row[4]);

        printf("modid = %d, cmdid = %d, ip = %u, port = %d\n", modID, cmdID, ip, port);

        //将读到数据加入map中
        //组装一个map的key
        uint64_t key = ((uint64_t)modID << 32) + cmdID;
        uint64_t value = ((uint64_t)ip << 32) + port;

        //插入到RouteDataMap_A中
        (*_data_pointer)[key].insert(value);
    }

    mysql_free_result(result);
}

//将RouteData表中的数据加载到_temp_pointer
void Route::load_route_data()
{
    int ret = 0;
    memset(_sql, 0, 1000);

    //清空temp_pointer所指向的临时表
    _temp_pointer->clear();

    //查询RouteData数据库 
    //"Select * from RouteData;"
    snprintf(_sql, 1000, "SELECT * FROM RouteData;");
    ret = mysql_real_query(&_db_conn, _sql, strlen(_sql));
    if (ret != 0) {
        fprintf(stderr, "select RouteData error %s\n", mysql_error(&_db_conn));
        exit(1);
    }
    
    //获得一个结果集合
    MYSQL_RES *result = mysql_store_result(&_db_conn);

    //得到行数
    long line_num = mysql_num_rows(result);
    
    //遍历分析集合中的元素， 加入_data_pointer中(MapA中)
    MYSQL_ROW row;
    for (int i = 0; i < line_num; i++) {
        //处理一行的数据
        row = mysql_fetch_row(result);
        int modID = atoi(row[1]);
        int cmdID = atoi(row[2]);
        unsigned int ip = atoi(row[3]);
        int port = atoi(row[4]);

        //将读到数据加入map中
        //组装一个map的key
        uint64_t key = ((uint64_t)modID << 32) + cmdID;
        uint64_t value = ((uint64_t)ip << 32) + port;

        //插入到RouteDataMap_B中
        (*_temp_pointer)[key].insert(value);
    }

    mysql_free_result(result);
}

//链接数据库的方法
void Route::connect_db()
{
    //加载mysql的配置参数
    string db_host = config_file::instance()->GetString("mysql", "db_host", "127.0.0.1");
    short db_port = config_file::instance()->GetNumber("mysql", "db_port", 3306);
    string db_user = config_file::instance()->GetString("mysql", "db_user", "root");
    string db_passwd = config_file::instance()->GetString("mysql", "db_passwd", "aceld");
    string db_name = config_file::instance()->GetString("mysql", "db_name", "lars_dns");

    //初始化mysql链接
    mysql_init(&_db_conn);

    //设置一个超时定期重连
    mysql_options(&_db_conn, MYSQL_OPT_CONNECT_TIMEOUT, "30");
    //开启mysql断开链接自动重连机制
    my_bool reconnect = 1;
    mysql_options(&_db_conn, MYSQL_OPT_RECONNECT, &reconnect);

    //链接数据库
    if (!mysql_real_connect(&_db_conn, db_host.c_str(), db_user.c_str(), db_passwd.c_str(), db_name.c_str(), db_port, NULL, 0)) {
        fprintf(stderr, "Failed to connect mysql\n");
        exit(1);
    }

    printf("connect db succ!\n");
}


