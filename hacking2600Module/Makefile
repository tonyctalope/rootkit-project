obj-m += hacking2600Module.o

all:
	make -C ../linux-5.15.137/ M=$(PWD) modules

clean:
	make -C ../linux-5.15.137/ M=$(PWD) clean
