PWD := $(shell pwd)
obj-m += chardev.o 

all:
	make -C /usr/lib/modules/4.18.7-200.fc28.x86_64/build/ M=$(PWD) modules

clean:
	make -C /usr/lib/modules/4.18.7-200.fc28.x86_64/build/ M=$(PWD) clean