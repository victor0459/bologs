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

