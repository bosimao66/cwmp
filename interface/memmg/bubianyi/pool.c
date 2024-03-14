




//管理动态内存分配
#include "pool.h"
#include <stdio.h>
#include <stdlib.h>

//键值对
typedef struct pool_key_value_t{
    void *key;
    unsigned int value;
    unsigned char en;
}pool_key_value_t;

#define KEY_VALUE_MAX_SIZE 1024
typedef struct pool_manager_t{
    unsigned int mallocCnt;
    unsigned int freeCnt;
    pool_key_value_t keyVal[KEY_VALUE_MAX_SIZE];
    unsigned char keyValOverFlow;     //分配次数是否溢出的标志

    int (*get_malloc_size)(void);
}pool_manager_t;

pool_manager_t poolLocalManager;




int pool_get_malloc_size(void);


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

void pool_stat_show(void)
{
    printf("==================pool stat================\n");
    printf("mallocCnt=%d\n"
            "freeCnt=%d\n"
            "mallocSize=%d byte\n"
            "KEY_VALUE_MAX_SIZE:%d\n"
            "keyValueOverFlow=%d\n\n",
            
            poolLocalManager.mallocCnt,
            poolLocalManager.freeCnt,
            poolLocalManager.get_malloc_size(),
            KEY_VALUE_MAX_SIZE,
            poolLocalManager.keyValOverFlow);  
    printf("==================== end ==================\n");          
}

void pool_key_value_in(void *key, int value)
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

void pool_key_value_out(void *key)
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

int pool_get_malloc_size(void)
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

void *pool_malloc(int size)
{
    void *header = malloc(size);
    poolLocalManager.mallocCnt++;
    pool_key_value_in(header, size);
    return header;
}

void pool_free(void *header)
{
    poolLocalManager.freeCnt++;
    if(header != NULL)
    {
        pool_key_value_out(header);
        free(header);
     
    }    
}










