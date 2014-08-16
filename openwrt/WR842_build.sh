#!/bin/sh

## build openwrt image for the following routers:
## TL-WR842ND, TL-WR842N


# prepare files to include
rm -R tmp_qaul.net
cp -R qaul.net tmp_qaul.net
cp -R WR842/* tmp_qaul.net/

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
make image PROFILE=TLWR842 FILES=../tmp_qaul.net \
    PACKAGES="olsrd olsrd-mod-dyn-gw kmod-ipip ip \
    uhttpd libpam empty \
	libubus-lua liblua lua libuci-lua \
    luci-ssl libpolarssl px5g \
    tinc \
    kmod-usb-storage block-mount kmod-fs-ext4 \
    comgt kmod-usb-serial kmod-usb-serial-option kmod-usb-serial-wwan kmod-usb-acm kmod-usb-net usb-modeswitch \
    openssh-sftp-server horst"

cd ../

# copy the image into the top directory
cp ./OpenWrt-ImageBuilder-ar71xx_generic-for-linux-x86_64/bin/ar71xx/openwrt-ar71xx-generic-tl-wr842n-v2-squashfs-factory.bin ./
