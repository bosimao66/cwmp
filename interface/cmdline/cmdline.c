



/*

以后再说吧
*/

#include "cmdline.h"



#define CMD_OBJ_MAX_NUM 100
typedef struct __cmd_obj{
   unsigned char en;
   
}cmd_obj_t;

typedef struct __cmd_obj_mg{
    unsigned int cnt;
}cmd_obj_mg_t;



static cmd_obj_t cmd_obj_array[CMD_OBJ_MAX_NUM] = {0};
cmd_obj_mg_t cmd_obj_mg = {0};









