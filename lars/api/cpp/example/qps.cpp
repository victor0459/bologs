
#include "lars_api.h"
#include <iostream>

struct ID {
    int t_id;// 线程ID
    int modid;
    int cmdid;
};

void usage()
{
    printf("usage : ./qps [thread_num]\n");
}

void *test_qps(void *args) 
{
   
    return NULL;
}

//开发者业务模块
int main(int argc, char **argv)
{
    if (argc != 2) {
        usage();
        return 1;
    }

    int cnt = atoi(argv[1]);

    ID *ids = new ID[cnt];
    pthread_t *tids = new pthread_t[cnt];

    for (int i = 0; i < cnt; i ++) {
        ids[i].t_id = i;
        ids[i].modid = i + 1;
        ids[i].cmdid = 1;

        pthread_create(&tids[i], NULL, test_qps, &ids[i]);
    }

    for (int i = 0 ; i < cnt; i ++) {
        pthread_join(tids[i], NULL);
    }



    return 0;
}
