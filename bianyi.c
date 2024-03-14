interface/memmg/pool2.c: In function ¡®__apply_new_block¡¯:
interface/memmg/pool2.c:108:20: warning: implicit declaration of function ¡®malloc¡¯ [-Wimplicit-function-declaration]
     void *handle = malloc(size);
                    ^
interface/memmg/pool2.c:108:20: warning: incompatible implicit declaration of built-in function ¡®malloc¡¯
interface/memmg/pool2.c:108:20: note: include ¡®<stdlib.h>¡¯ or provide a declaration of ¡®malloc¡¯
interface/memmg/pool2.c: In function ¡®__free_block¡¯:
interface/memmg/pool2.c:142:13: warning: implicit declaration of function ¡®free¡¯ [-Wimplicit-function-declaration]
             free(handle);
             ^
interface/memmg/pool2.c:142:13: warning: incompatible implicit declaration of built-in function ¡®free¡¯
interface/memmg/pool2.c:142:13: note: include ¡®<stdlib.h>¡¯ or provide a declaration of ¡®free¡¯
interface/memmg/pool2.c:144:13: warning: ¡®return¡¯ with no value, in function returning non-void
             return ;
             ^
interface/memmg/pool2.c: At top level:
interface/memmg/pool2.c:81:12: warning: ¡®__get_pool_block_num¡¯ defined but not used [-Wunused-function]
 static int __get_pool_block_num()
            ^
interface/memmg/pool2.c:100:14: warning: ¡®__apply_new_block¡¯ defined but not used [-Wunused-function]
 static void *__apply_new_block(int id, int size)
              ^
interface/memmg/pool2.c:132:14: warning: ¡®__free_block¡¯ defined but not used [-Wunused-function]
 static void *__free_block(void *handle)
              ^
interface/cmdline/cmdline.c:26:18: warning: ¡®cmd_obj_array¡¯ defined but not used [-Wunused-variable]
 static cmd_obj_t cmd_obj_array[CMD_OBJ_MAX_NUM] = {0};
                  ^
interface/memmg/pool2.c: In function ¡®__apply_new_block¡¯:
interface/memmg/pool2.c:108:20: warning: implicit declaration of function ¡®malloc¡¯ [-Wimplicit-function-declaration]
     void *handle = malloc(size);
                    ^
interface/memmg/pool2.c:108:20: warning: incompatible implicit declaration of built-in function ¡®malloc¡¯
interface/memmg/pool2.c:108:20: note: include ¡®<stdlib.h>¡¯ or provide a declaration of ¡®malloc¡¯
interface/memmg/pool2.c: In function ¡®__free_block¡¯:
interface/memmg/pool2.c:142:13: warning: implicit declaration of function ¡®free¡¯ [-Wimplicit-function-declaration]
             free(handle);
             ^
interface/memmg/pool2.c:142:13: warning: incompatible implicit declaration of built-in function ¡®free¡¯
interface/memmg/pool2.c:142:13: note: include ¡®<stdlib.h>¡¯ or provide a declaration of ¡®free¡¯
interface/memmg/pool2.c:144:13: warning: ¡®return¡¯ with no value, in function returning non-void
             return ;
             ^
interface/memmg/pool2.c: At top level:
interface/memmg/pool2.c:81:12: warning: ¡®__get_pool_block_num¡¯ defined but not used [-Wunused-function]
 static int __get_pool_block_num()
            ^
interface/memmg/pool2.c:100:14: warning: ¡®__apply_new_block¡¯ defined but not used [-Wunused-function]
 static void *__apply_new_block(int id, int size)
              ^
interface/memmg/pool2.c:132:14: warning: ¡®__free_block¡¯ defined but not used [-Wunused-function]
 static void *__free_block(void *handle)
              ^
interface/cmdline/cmdline.c:26:18: warning: ¡®cmd_obj_array¡¯ defined but not used [-Wunused-variable]
 static cmd_obj_t cmd_obj_array[CMD_OBJ_MAX_NUM] = {0};
                  ^
