#!/bin/sh

## set new password
## 
## a new salt ( e.g. \$1\$12345678 ) is generated.
## the new password is encrypted.
## the old password in the shadow file is replaced.
## 
## usage:
##     ./newpassword.sh <NEWPASSWORD>
##     ./newpassword.sh myNewPassword 

# create salt
random=`head -c256 /dev/urandom`
SALT=`echo -n '$random'  | md5sum | sed -e "s/^\(........\).*/\1/"`

# encrypt password
/qaul/bin/qauladmin newpassword "$1" "\$1\$$SALT" | sed -E "s/[\/]/\\\\\//g" > /tmp/newpasswd

# write into shadow file
sed -i "s/^root:[^:]*/root:`cat /tmp/newpasswd`/" /etc/shadow

exit 0
