#!/bin/sh

## configure USB storage

# set flag
echo "{\"state\":1}" > "/www/qaul/storage_config"

# check if USB stick is connected

# check if stick is mounted

# unmount if mounted
umount /dev/sda1

# create ext4 file system on stick
##mkfs.ext4 /dev/sda1
# use busybox mkfs.ext2 to save disk space
mkfs.ext2 /dev/sda1
/sbin/block mount

# copy file system 
tar -C /overlay -cvf - . | tar -C /mnt/share -xf -
echo "1" > "/mnt/share/qaul/config/storage"

# configure overlay mount
uci set fstab.share.enabled=0
uci set fstab.overlay.enabled=1
uci commit

# remove flag
rm "/www/qaul/storage_config"

# restart device
reboot

exit 0
