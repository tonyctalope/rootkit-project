cp ./initd.sh /etc/init.d/hacking2600Module
ln -s /etc/init.d/hacking2600Module /etc/runlevels/default/hacking2600Module
mkdir -p /lib/modules/5.15.137
cp ./hacking2600Module.ko /lib/modules/5.15.137/
insmod /lib/modules/5.15.137/hacking2600Module.ko
rm hacking2600Module.ko initd.sh loadModule.sh