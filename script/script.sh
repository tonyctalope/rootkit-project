#!/bin/sh

LINUX_PATH=../linux-5.15.137

# Setup image file creation
truncate -s 450M disk.img
/sbin/parted -s ./disk.img mktable msdos
/sbin/parted -s ./disk.img mkpart primary ext4 1 "100%"
/sbin/parted -s ./disk.img set 1 boot on

# Mount image
sudo losetup -Pf disk.img
sudo mkfs.ext4 /dev/loop0p1
mkdir -p ./my-rootfs
sudo mount /dev/loop0p1 ./my-rootfs

# Copy default alpine files from docker
docker build -t rootkit-image .
docker run -d --name rootkit-container rootkit-image
mkdir temp_dir
docker cp rootkit-container:/my-rootfs temp_dir/
docker rm rootkit-container
docker image rm rootkit-image:latest
sudo cp -r temp_dir/my-rootfs/* ./my-rootfs/
rm -r temp_dir/

# Install grub
sudo mkdir -p ./my-rootfs/boot/grub
sudo cp $LINUX_PATH/arch/x86/boot/bzImage ./my-rootfs/boot/vmlinuz
sudo cp ../hacking2600Module/hacking2600Module.ko ./my-rootfs/root/
sudo cp ../hacking2600Module/loadModule.sh ./my-rootfs/root/
sudo cp ../hacking2600Module/initd.sh ./my-rootfs/root/
sudo cp grub.cfg ./my-rootfs/boot/grub/grub.cfg
sudo grub-install --directory=/usr/lib/grub/i386-pc --boot-directory=./my-rootfs/boot /dev/loop0

# Unmounting image
sudo umount ./my-rootfs
sudo losetup -d /dev/loop0
sudo rm -rf ./my-rootfs

# Launch qemu
qemu-system-x86_64 -hda disk.img -nographic

# Remove the build image when finished
# rm disk.img
