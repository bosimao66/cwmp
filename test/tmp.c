/***************************************************************
Copyright © bosimao. All rights reserved.
文件名	: 	 pool2.c
作者	   : bosimao
版本	   : V1.0
描述	   : 内存池管理。
其他	   : 无
网址 	   : github
日志	   : 2024年3月14日
***************************************************************/




//管理动态内存分配
#include "pool.h"
#include <stdio.h>
#include <stdlib.h>

//键值对
typedef struct __pool_key_value{
    void *key;
    unsigned int value;
    unsigned int userid;  //用于标识请求者id
    unsigned char en;
}pool_key_value_t;


//请求者
#define USER_NAME_MAX_LEN 8
typedef struct __pool_user{
    unsigned int id;  //用于标识请求者id
    char name[USER_NAME_MAX_LEN];
    int mallocSize;
    int freeSize;
    unsigned char en;
}pool_user_t;



#define KEY_VALUE_MAX_SIZE 1024
#define user_MAX_SIZE 256
#define MALLOC_TOTLE_MAX_BYTE 1024*1024*4   //最大的分配量，4M大小，在每一次分配之前，都要计算是否溢出
typedef struct pool_manager_t{
    unsigned int mallocCnt;
    unsigned int freeCnt;
    pool_key_value_t keyVal[KEY_VALUE_MAX_SIZE];
    pool_user_t user[user_MAX_SIZE];
    unsigned char keyValOverFlow;     //分配次数是否溢出的标志
   // unsigned char mallocOverFlow;     //pool内存的使用是否溢出

    int (*get_malloc_size)(void);
}pool_manager_t;

static pool_manager_t poolLocalManager; //本地管理



/*
 * @函数名		        : pool_key_value_in
 * @描述		        : 内存池键值对新增。
 * @参数 - key	    : 键值对的“键”。
 * @参数 - value	    : 键值对的“值”。
 * @返回 			    : 无
 */
static void pool_key_value_in(void *key, int value)
{
    int i;
    for(i = 0; i < KEY_VALUE_MAX_SIZE; i++)
    {
        if(poolLocalManager.keyVal[i].en == 0)
        {
            poolLocalManager.keyVal[i].en = 1;
            poolLocalManager.keyVal[i].key = key;
            poolLocalManager.keyVal[i].value = value;
            return;
        }    
    }
    poolLocalManager.keyValOverFlow = 1;
}

/*
 * @函数名		        : pool_key_value_out
 * @描述		        : 内存池键值对删除。
 * @参数 - key	    : 键值对的“键”。
 * @返回 			    : 无
 */
static void pool_key_value_out(void *key)
{
    int i;
    for(i = 0; i < KEY_VALUE_MAX_SIZE; i++)
    {
        if(poolLocalManager.keyVal[i].en == 1 && poolLocalManager.keyVal[i].key == key)
        {
            poolLocalManager.keyVal[i].en = 0;
            poolLocalManager.keyValOverFlow = 0;
            return;
        }    
    }
}

/*
 * @函数名		        : pool_get_malloc_size
 * @描述		        : 获取内存已经分配完的字节数。
 * @返回 			    : 内存已经分配完的字节数
 */
static int pool_get_malloc_size(void)
{
    int i, size;
    size = 0;
    for(i = 0; i < KEY_VALUE_MAX_SIZE; i++)
    {
        if(poolLocalManager.keyVal[i].en == 1 )
            size += poolLocalManager.keyVal[i].value;
    }
    return size;
}

/*
 * @函数名		        : pool_malloc
 * @描述		        : 内存池内存的动态分配。
 * @参数 - size	    : 要分配的字节数。
 * @返回 			    : 分配得到的内存的指针
 */
void *pool_malloc(int size)
{
    if(pool_get_malloc_size() + size >= MALLOC_TOTLE_MAX_BYTE)
    {
        //printf("pool malloc over flow!\n");
        return NULL;
    }
    void *header = malloc(size);
    if(header == NULL)
    {
        //printf("pool malloc faild!\n");
        return NULL;
    }  
    poolLocalManager.mallocCnt++;
    pool_key_value_in(header, size);
    return header;
}


/*
 * @函数名		        : pool_free
 * @描述		        : 内存池的内存释放。
 * @参数 - size	    : 要释放的内存指针。
 * @返回 			    : 无
 */
void pool_free(void *header)
{
    poolLocalManager.freeCnt++;
    if(header != NULL)
    {
        pool_key_value_out(header);
        free(header);
     
    }    
}

/*
 * @函数名		        : pool_init
 * @描述		        : 内存池的初始化。
 * @返回 			    : 无
 */
void pool_init(void)
{
    int i;
    poolLocalManager.mallocCnt = 0;
    poolLocalManager.freeCnt = 0;
    for(i = 0; i < KEY_VALUE_MAX_SIZE; i++)
        poolLocalManager.keyVal[i].en = 0;
    poolLocalManager.keyValOverFlow = 0;
    poolLocalManager.get_malloc_size = pool_get_malloc_size;
}

/*
 * @函数名		        : pool_stat_show
 * @描述		        : 内存池的状态查询。
 * @参数	buf         : 状态内容存储字符串。
 * @参数size	        : 字符串大小。
 * @返回 			    : 空
 */
void pool_stat_show(char *buf, int size)
{
    if(buf == NULL)return;
    if(size <= 0)return;
 
    snprintf(buf, size, "==================pool stat================\n");
    snprintf(buf, size, "mallocCnt=%d\n"
            "MALLOC_TOTLE_MAX_BYTE:%d\n"
            "freeCnt=%d\n"
            "mallocSize=%d byte\n"
            "KEY_VALUE_MAX_SIZE:%d\n"
            "keyValueOverFlow=%d\n\n",
            
            poolLocalManager.mallocCnt,
            MALLOC_TOTLE_MAX_BYTE,
            poolLocalManager.freeCnt,
            poolLocalManager.get_malloc_size(),
            KEY_VALUE_MAX_SIZE,
            poolLocalManager.keyValOverFlow);  
    snprintf(buf, size, "==================== end ==================\n"); 
}





//=======================================
//新增用户内容，主要是更清晰的了解“谁”使用内存池


//初始化
void pool_user_init(void)
{
    int i;
    pool_user_t *user = poolLocalManager.user;
    for(i = 0; i < user_MAX_SIZE; i++)
   {
       memset(user[i].name, 0, sizeof(user[i].name));
       user[i].en = 0;
       user[i].id = 0;
       user[i].mallocSize = 0;
       user[i].freeSize = 0;
   }

}

//请求编号
int pool_request_user_num(char *name)
{
    int i;
    pool_user_t *users = poolLocalManager.user;
    for(i = 0; i < user_MAX_SIZE; i++)
   {
        if(users[i].en == 0)
        {
            users[i].en = 1;
            if(name == NULL)
            {
                strncpy(users[i].name, USER_NAME_MAX_LEN, "unknow");
            }
            else
            {
                strncpy(users[i].name, USER_NAME_MAX_LEN, name);
            }
            return i;
        }    
   }
   return  users[i].en - 1;
}

//和user有关的malloc接口
void *pool_user_malloc(int usrid, int size)
{
    pool_user_t *user = poolLocalManager.user[usrid];
    
    void *ret = pool_malloc(size);
    if(ret != NULL)
    {
        user.mallocSize += size;
    }
    return ret;
}

//和user有关的free接口
void pool_user_free()
{
    pool_user_t *user = poolLocalManager.user[usrid];
    
    void *ret = pool_malloc(size);
    if(ret != NULL)
    {
        user.mallocSize += size;
    }
    return ret;
}






















