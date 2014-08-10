#!/bin/sh

## configure USB storage

# check if USB stick is connected

# check if stick is mounted

# create ext4 file system on stick
mkfs.ext4 /dev/sda1
/sbin/block mount

# copy file system 
tar -C /overlay -cvf - . | tar -C /mnt/share -xf -
echo "1" > "/mnt/share/qaul/config/storage"

# configure overlay mount
uci set fstab.share.enabled=0
uci set fstab.overlay.enabled=1
uci commit

# restart device
#reboot

exit 0
