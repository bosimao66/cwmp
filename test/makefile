# 定义DIRS变量，列出所有的目录
DIRS := bsp/clk \
        bsp/delay \
        bsp/led \
        bsp/key \
        bsp/gpio \
        bsp/buzzer \
        bspp/a \
        look/bsp \
		bsp \
        main

# 利用wildcard函数，列出所有符合条件的目录
# 这里使用filter函数，匹配以"bsp/"开头的子项，且其下一个字符为"/"或没有下一个字符
DIR :=
#$(foreach dir,$(DIRS),$(ifeq $(bsp,$(dir)),DIR += $(dir)))
#$(foreach dir,$(DIRS),$(if $(findstring bsp,$(dir)), $(eval DIR += $(dir))))
#DIR := $(filter bsp/%,$(DIRS))
DIR := $(filter bsp,$(DIRS))

# 打印DIR变量的值

all:
	echo $(DIR)

	
	
	ACS_SRCDIRS 			:=$(filter acs/%,$(SRCDIRS)) $(filter acs,$(SRCDIRS))