FROM alpine

RUN mkdir /my-rootfs
RUN apk add openrc util-linux build-base
RUN ln -s agetty /etc/init.d/agetty.ttyS0
RUN echo ttyS0 > /etc/securetty
RUN rc-update add agetty.ttyS0 default && \
    rc-update add root default

RUN echo -e "root\nroot" | passwd root
RUN adduser -D test && \
    echo "test:test" | chpasswd

RUN rc-update add devfs boot && \
    rc-update add procfs boot && \
    rc-update add sysfs boot
RUN for d in bin etc lib root sbin usr; do tar c "/$d" | tar x -C /my-rootfs; done
RUN for dir in dev proc run sys var; do mkdir /my-rootfs/${dir}; done