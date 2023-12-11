#!/bin/sh

LINUX_PATH=../linux-5.15.137

if [ ! -d "$LINUX_PATH" ]; then
    # Clone the specific version of the Linux Kernel
    git clone --depth 1 --branch v5.15.137 https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git "$LINUX_PATH"
    cd "$LINUX_PATH"

    make defconfig

    make -j $(nproc --ignore=2)

    cd ../script

fi

# Setup image file creation
truncate -s 450M disk.img
/sbin/parted -s ./disk.img mktable msdos
/sbin/parted -s ./disk.img mkpart primary ext4 1 "100%"
/sbin/parted -s ./disk.img set 1 boot on

# Mount image
LOOP_DEVICE=$(sudo losetup -Pf --show disk.img)
sudo mkfs.ext4 ${LOOP_DEVICE}p1
mkdir -p ./my-rootfs
sudo mount ${LOOP_DEVICE}p1 ./my-rootfs

# Copy default alpine files from docker
docker build -t rootkit-image .
docker run -d --name rootkit-container rootkit-image
mkdir temp_dir
docker cp rootkit-container:/my-rootfs temp_dir/
docker rm rootkit-container
docker image rm rootkit-image:latest
sudo cp -r temp_dir/my-rootfs/* ./my-rootfs/
sudo rm -r temp_dir/

# Install grub
sudo mkdir -p ./my-rootfs/boot/grub
sudo cp $LINUX_PATH/arch/x86/boot/bzImage ./my-rootfs/boot/vmlinuz
cd ../hacking2600Module
make clean && make all
cd ../script
sudo cp ../hacking2600Module/hacking2600Module.ko ./my-rootfs/root/
sudo cp ../hacking2600Module/loadModule.sh ./my-rootfs/root/
sudo cp ../hacking2600Module/initd.sh ./my-rootfs/root/
sudo cp grub.cfg ./my-rootfs/boot/grub/grub.cfg
sudo grub-install --directory=/usr/lib/grub/i386-pc --boot-directory=./my-rootfs/boot ${LOOP_DEVICE}

# Unmounting image
sudo umount ./my-rootfs
sudo losetup -d ${LOOP_DEVICE}
sudo rm -rf ./my-rootfs

# Launch qemu
qemu-system-x86_64 -hda disk.img -nographic

# Remove the build image when finished
rm disk.img
