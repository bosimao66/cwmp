

#注意：COMMON_SRCDIRS 公共目录及其子目录里的.c文件，如果重名，会生成同名.o文件，发生冲突；
#如果有需要的话，可以优化

#这里编译器为gcc

MKAE_CMP_NAME		?= 

CC					:= $(MKAE_CMP_NAME)gcc
LD					:= $(MKAE_CMP_NAME)ld 
OBJCOPY				:= $(MKAE_CMP_NAME)objcopy
OBJDUMP				:= $(MKAE_CMP_NAME)objdump

#目标

ACS_TARGET			:= acs
CPE_TARGET			:= cpe

#所有的源文件路径

SRCDIRS					:= acs \
						   cpe \
						   acs/cmdline \
						   cpe/cmdline \
						   interface/memmg \
						   interface/log \
						   interface/cmdline 

#区分 acs和 cpe文件目录（注意不要有空格！！！！！）
ACS_DIR_NAME			:=acs
CPE_DIR_NAME			:=cpe


# 思路：acs和cpe的代码不完全相同，所以源文件集合分为三个子集ACS_SRCDIRS、CPE_SRCDIRS、COMMON_SRCDIRS
# 过滤规则……
ACS_SRCDIRS 			:=
CPE_SRCDIRS 			:=
COMMON_SRCDIRS 			:=
TMP						:=

ACS_SRCDIRS 			:=$(filter $(ACS_DIR_NAME)/%,$(SRCDIRS)) $(filter $(ACS_DIR_NAME),$(SRCDIRS))
CPE_SRCDIRS 			:=$(filter $(CPE_DIR_NAME)/%,$(SRCDIRS)) $(filter $(CPE_DIR_NAME),$(SRCDIRS))
TMP			 			:=$(filter-out $(ACS_SRCDIRS),$(SRCDIRS))
COMMON_SRCDIRS 			:=$(filter-out $(CPE_SRCDIRS),$(TMP))

					   
#更多的头文件路径，思路：把所有的源文件路径当做头文件路径，然后加上额外的头文件路径

MORE_INC_DIRS			:=  
INCDIRS = $(MORE_INC_DIRS) $(SRCDIRS)

#函数patsubst，$(patsubst <pattern>,<replacement>,<text>)
#在每一个头文件路径的前面加上-I，用于编译时，对头文件的寻找
					  
INCDIRS_PRO				:= $(patsubst %, -I %, $(INCDIRS))


#函数dir：从文件名中提取目录部分
#函数foreach：$(foreach <var>, <list>,<text>)，从单词列表list中，以此取出单词赋值给变量var，然后放到text中
#函数wildcard：(类比%的作用）对每一项进行操作

ACS_CFILES				:= $(foreach dir, $(ACS_SRCDIRS), $(wildcard $(dir)/*.c))
CPE_CFILES				:= $(foreach dir, $(CPE_SRCDIRS), $(wildcard $(dir)/*.c))
COMMON_CFILES			:= $(foreach dir, $(COMMON_SRCDIRS), $(wildcard $(dir)/*.c))

#中间的目标文件存放点
OBJ_DIR					:= object
ACS_OBJ_DIR				:= $(OBJ_DIR)/acs
CPE_OBJ_DIR				:= $(OBJ_DIR)/cpe
COMMON_OBJ_DIR			:= $(OBJ_DIR)

#目标全名
ACS_TARGET_NAME			:=$(OBJ_DIR)/$(ACS_TARGET)
CPE_TARGET_NAME			:=$(OBJ_DIR)/$(CPE_TARGET)
#ACS_TARGET_NAME		:=$(ACS_TARGET)
#CPE_TARGET_NAME		:=$(CPE_TARGET)



#函数 notdir ：路径+文件组合，把路径去掉
#替换方式？test.c 如何变为 test.o，用  test.c:.c=.o ??

ACS_CFILES_NOTDIR		:= $(notdir $(ACS_CFILES))
ACS_COBJS				:= $(patsubst %, $(ACS_OBJ_DIR)/%, $(ACS_CFILES_NOTDIR:.c=.o))
CPE_CFILES_NOTDIR		:= $(notdir $(CPE_CFILES))
CPE_COBJS				:= $(patsubst %, $(CPE_OBJ_DIR)/%, $(CPE_CFILES_NOTDIR:.c=.o))
COMMON_CFILES_NOTDIR	:= $(notdir $(COMMON_CFILES))
COMMON_COBJS			:= $(patsubst %, $(COMMON_OBJ_DIR)/%, $(COMMON_CFILES_NOTDIR:.c=.o))


#VPATH 声明所有的源文件存在的路径

VPATH				:= $(SRCDIRS)


#线程

#PTHREAD				:=-lpthread


#编译
all:$(ACS_TARGET_NAME).out $(CPE_TARGET_NAME).out 
	@echo ---------------OK!------------------
	
$(ACS_TARGET_NAME).out : $(ACS_COBJS) $(COMMON_COBJS)
	$(CC) -o $@ $^
	$(CC) -o $(ACS_TARGET_NAME).elf $^ 
	$(OBJDUMP) -D $(ACS_TARGET_NAME).elf > $(ACS_TARGET_NAME).dis
	
$(CPE_TARGET_NAME).out : $(CPE_COBJS) $(COMMON_COBJS)
	$(CC) -o $@ $^
	$(CC) -o $(CPE_TARGET_NAME).elf $^ 
	$(OBJDUMP) -D $(CPE_TARGET_NAME).elf > $(CPE_TARGET_NAME).dis
	

$(ACS_COBJS) : $(ACS_OBJ_DIR)/%.o : %.c
	$(CC) -Wall -c $(INCDIRS_PRO) -o $@ $< $(PTHREAD)	
$(CPE_COBJS) : $(CPE_OBJ_DIR)/%.o : %.c
	$(CC) -Wall -c $(INCDIRS_PRO) -o $@ $< $(PTHREAD)
$(COMMON_COBJS) : $(COMMON_OBJ_DIR)/%.o : %.c
	$(CC) -Wall -c $(INCDIRS_PRO) -o $@ $< $(PTHREAD)	

.PHONY : clean
clean:
	rm $(ACS_TARGET_NAME).out $(ACS_TARGET_NAME).elf $(ACS_TARGET_NAME).dis 
	rm $(CPE_TARGET_NAME).out $(CPE_TARGET_NAME).elf $(CPE_TARGET_NAME).dis 
	rm $(ACS_COBJS) $(CPE_COBJS) $(COMMON_COBJS)




