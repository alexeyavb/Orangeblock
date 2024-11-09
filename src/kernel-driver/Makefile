obj-m := stm32f411_i2s.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo cp stm32f411_i2s.ko /lib/modules/$(shell uname -r)
	sudo depmod -a
