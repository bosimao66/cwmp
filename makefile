






#这里编译器为gcc

MKAE_CMP_NAME		?= 

CC					:= $(MKAE_CMP_NAME)gcc
LD					:= $(MKAE_CMP_NAME)ld 
OBJCOPY				:= $(MKAE_CMP_NAME)objcopy
OBJDUMP				:= $(MKAE_CMP_NAME)objdump

#目标

TARGET				?= testapp

#所有的源文件路径

SRCDIRS				:= test \
					   test/acp 
					   
					   
#更多的头文件路径，思路：把所有的源文件路径当做头文件路径，然后加上额外的头文件路径

MORE_INC_DIRS		:= test/inc 
INCDIRS = $(MORE_INC_DIRS) $(SRCDIRS)

#函数patsubst，$(patsubst <pattern>,<replacement>,<text>)
#在每一个头文件路径的前面加上-I，用于编译时，对头文件的寻找
					  
INCDIRS_PRO			:= $(patsubst %, -I %, $(INCDIRS))


#函数dir：从文件名中提取目录部分
#函数foreach：$(foreach <var>, <list>,<text>)，从单词列表list中，以此取出单词赋值给变量var，然后放到text中
#函数wildcard：(类比%的作用）对每一项进行操作

CFILES				:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
#SFILES				:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))


#中间的目标文件存放点

OBJ_DIR				?= object

#函数 notdir ：路径+文件组合，把路径去掉
#替换方式？test.c 如何变为 test.o，用  test.c:.c=.o ??

CFILES_NOTDIR		:= $(notdir $(CFILES))
#SFILES_NOTDIR		:= $(notdir $(SFILES))
COBJS				:= $(patsubst %, $(OBJ_DIR)/%, $(CFILES_NOTDIR:.c=.o))
#SOBJS				:= $(patsubst %, $(OBJ_DIR)/%, $(SFILES_NOTDIR:.S=.o))
OBJS				:= $(COBJS) $(SOBJS)


#VPATH 声明所有的源文件存在的路径

VPATH				:= $(SRCDIRS)


#线程

#PTHREAD				:=-lpthread


#编译

$(TARGET).out : $(OBJS)
	$(CC) -o $@ $^
	$(CC) -o $(TARGET).elf $^ 
#	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).dis

$(COBJS) : $(OBJ_DIR)/%.o : %.c
	$(CC) -Wall -c $(INCDIRS_PRO) -o $@ $< $(PTHREAD)

#$(SOBJS) : $(OBJ_DIR)/%.o : %.S
#	$(CC) -Wall -nostdlib -c -O2 $(INCDIRS_PRO) -o $@ $<	


.PHONY : clean
clean:
	rm $(TARGET).out $(TARGET).elf $(TARGET).dis $(COBJS) $(SOBJS)



























