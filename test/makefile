DIRS := bsp/clk \
        bsp/delay \
        bsp/led \
        bsp/key \
        bsp/gpio \
        bsp/buzzer \
        main

# 定义空的DIR变量
DIR :=

# 遍历DIRS中的每个子项，检查是否包含'bsp'字符串，如果不包含则添加到DIR中
$(foreach dir,$(DIRS),$(if $(findstring bsp,$(dir)),,$(eval DIR += $(dir))))

all:
	@echo $(DIR)