#!/sbin/openrc-run

# Provide a description for the service
description="Load my custom kernel module"

# Specify dependencies (if any)
depend() {
    # The module may depend on local filesystems being mounted (if it reads from the filesystem)
    # and the network being down (if it should load before the network comes up).
    need localmount
    after modules
    keyword -prefix -lxc -docker
}

# The start function is called when the service is started
start() {
    ebegin "Loading my custom kernel module"
    # Replace 'mymodule.ko' with the path to your module
    insmod /lib/modules/5.15.137/hacking2600Module.ko
    eend $?
}