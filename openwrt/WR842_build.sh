#!/bin/sh

## build openwrt image for the following routers:
## TL-WR842ND, TL-WR842N


# check if build directory already exists

# extract archive

# change into build directory
cd OpenWrt-ImageBuilder-ar71xx_generic-for-linux-x86_64

# build image
# maybe needed: uhttpd uhttpd-mod-lua liblua lua libuci-lua libubus-lua \
make image PROFILE=TLWR842 FILES=../WR84X_qaul.net \
    PACKAGES="olsrd kmod-ipip ip \
    uhttpd uhttpd-mod-lua libuci-lua \
    tinc \
    kmod-usb-storage block-mount kmod-fs-ext4 \
    kmod-usb-serial kmod-usb-serial-option kmod-usb-serial-wwan kmod-usb-acm usb-modeswitch \
    openssh-sftp-server horst"

cd ../

# copy the image into the top directory
cp OpenWrt-ImageBuilder-ar71xx_generic-for-linux-i486/bin/ar71xx/openwrt-ar71xx-generic-tl-wr842n-v2-squashfs-factory.bin ./
