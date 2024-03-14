



#ifndef _POOL_2H
#define _POOL_2H




//使用者
#define POOL_USER_NAME_MAX_LEN 16
typedef struct __pool_user{
    char name[POOL_USER_NAME_MAX_LEN];
    //int id;   //数组里的序号当做id号
    char en;    //使用标记
}pool_user_t;

//统计结果，内存池的总量，内存池使用占比，内存池剩余字节数
//分配块的个数，各个使用者的使用字节数及其占比（用id号标识）
#define POOL_USER_MAX_NUM 256   //用户的id号 1~256
typedef struct __pool_stat{
    int totle;
    int used;
    int usedRate;    //千分之占比，这里不用浮点型，注意计算方式
    int residue;    //剩余
    int residueRate;    //剩余占比
    int blockCnt;   
    int userUsed[POOL_USER_MAX_NUM];
    int userUsedRate[POOL_USER_MAX_NUM];
    //char userEn[POOL_USER_MAX_NUM];
}pool_stat_t;




void pool_init();
pool_user_t *get_pool_user_handle();
void pool_show();
int pool_appy_user_id(char *name);
void *pool_user_malloc(int id, int size);
void pool_user_free(void *handle);


void pool_test();





#endif



