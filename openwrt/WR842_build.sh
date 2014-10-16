#!/bin/sh

## build openwrt image for the following routers:
## TL-WR842ND, TL-WR842N


# prepare files to include
rm -R image_files_tmp
cp -R image_files image_files_tmp
cp -R WR842/* image_files_tmp/

# change into build directory
cd OpenWrt-ImageBuilder-ar71xx_generic-for-linux-x86_64

# build image
#
# http maybe needed: 
# uhttpd uhttpd-mod-lua liblua lua libuci-lua libubus-lua \
# e2fsprogs
# 
# https: 
# maybe further packages needed: libpolarssl libustream-polarssl uhttpd-mod-tls px5g
# if px5g was installed correctly the https keys are automatically built 
# after server restart or router restart
# 
# 3G: 
# comgt 
# kmod-usb-acm    # ACM based modems, such as Nokia phones
# kmod-usb-net    # for tethering and rndis support (new 4G dongles)
# kmod-usb-serial # for dongles with serial support (most common)
# 
# authentication:
# empty           # interactive pseudo shell
# pam 
make image PROFILE=TLWR842 FILES=../image_files_tmp \
    PACKAGES="olsrd olsrd-mod-dyn-gw kmod-ipip ip \
    tcpdump-mini olsrd-mod-txtinfo \
    uhttpd \
    luci-ssl \
    tinc \
    kmod-usb-storage block-mount kmod-fs-ext4 \
    comgt kmod-usb-serial kmod-usb-serial-option kmod-usb-serial-wwan kmod-usb-acm kmod-usb-net usb-modeswitch \
    openssh-sftp-server horst"

cd ../

# copy the image into the top directory
cp ./OpenWrt-ImageBuilder-ar71xx_generic-for-linux-x86_64/bin/ar71xx/openwrt-ar71xx-generic-tl-wr842n-*squashfs-factory.bin ./
