

#这里编译器为gcc

MKAE_CMP_NAME		?= 

CC					:= $(MKAE_CMP_NAME)gcc
LD					:= $(MKAE_CMP_NAME)ld 
OBJCOPY				:= $(MKAE_CMP_NAME)objcopy
OBJDUMP				:= $(MKAE_CMP_NAME)objdump


#目标

ACS_TARGET			:= a
CPE_TARGET			:= c

#所有的源文件路径

SRCDIRS				:= acs \
					   cpe \
					   acs/shell \
					   cpe/shell \
					   interface/memmg \
					   interface/log \
					   interface/cmdline \
					   interface/linux \
					   protocol/transport 

#区分 acs和 cpe文件目录（注意不要有空格！！！！！）
ACS_DIR_NAME		:=acs
CPE_DIR_NAME		:=cpe


# 思路：acs和cpe的代码不完全相同，所以源文件集合分为三个子集ACS_SRCDIRS、CPE_SRCDIRS、COMMON_SRCDIRS
# 过滤规则……
ACS_SRCDIRS 		:=
CPE_SRCDIRS 		:=
COMMON_SRCDIRS 		:=
TMP					:=

ACS_SRCDIRS 		:=$(filter $(ACS_DIR_NAME)/%,$(SRCDIRS)) $(filter $(ACS_DIR_NAME),$(SRCDIRS))
CPE_SRCDIRS 		:=$(filter $(CPE_DIR_NAME)/%,$(SRCDIRS)) $(filter $(CPE_DIR_NAME),$(SRCDIRS))
TMP			 		:=$(filter-out $(ACS_SRCDIRS),$(SRCDIRS))
COMMON_SRCDIRS 		:=$(filter-out $(CPE_SRCDIRS),$(TMP))

					   
#更多的头文件路径，思路：把所有的源文件路径当做头文件路径，然后加上额外的头文件路径

MORE_INC_DIRS		:=  
INCDIRS 			:= $(MORE_INC_DIRS) $(SRCDIRS)

#函数patsubst，$(patsubst <pattern>,<replacement>,<text>)
#在每一个头文件路径的前面加上-I，用于编译时，对头文件的寻找
					  
INCDIRS_PRO			:= $(patsubst %, -I %, $(INCDIRS))


#函数dir：从文件名中提取目录部分
#函数foreach：$(foreach <var>, <list>,<text>)，从单词列表list中，以此取出单词赋值给变量var，然后放到text中
#函数wildcard：(类比%的作用）对每一项进行操作

ACS_CFILES			:= $(foreach dir, $(ACS_SRCDIRS), $(wildcard $(dir)/*.c))
CPE_CFILES			:= $(foreach dir, $(CPE_SRCDIRS), $(wildcard $(dir)/*.c))
COMMON_CFILES		:= $(foreach dir, $(COMMON_SRCDIRS), $(wildcard $(dir)/*.c))



#VPATH 声明所有的源文件存在的路径
VPATH				:= $(SRCDIRS)

#线程
PTHREAD			:=-lpthread

#中间的目标文件存放点
OBJ_DIR				:= object


#编译
all:$(OBJ_DIR)/$(ACS_TARGET) $(OBJ_DIR)/$(CPE_TARGET)
	cp $(OBJ_DIR)/$(ACS_TARGET) ./
	cp $(OBJ_DIR)/$(CPE_TARGET) ./
	@echo ---------------OK!------------------

	
$(OBJ_DIR)/$(ACS_TARGET) : $(ACS_CFILES) $(COMMON_CFILES)
	$(CC) -Wall $(INCDIRS_PRO) -o $@ $^ $(PTHREAD)
	
$(OBJ_DIR)/$(CPE_TARGET) : $(CPE_CFILES) $(COMMON_CFILES)
	$(CC) -Wall $(INCDIRS_PRO) -o $@ $^ $(PTHREAD)	

.PHONY : clean
clean:
	rm $(OBJ_DIR)/$(ACS_TARGET) $(OBJ_DIR)/$(CPE_TARGET) $(ACS_TARGET) $(CPE_TARGET) 




