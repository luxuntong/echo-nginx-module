#!/bin/sh
modPath=$(pwd)
cd $1
echo $modPath
./configure --add-module=$modPath
if make
then
make install
fi
rm /tmp/echoLog
killall -9 nginx;/usr/local/nginx/sbin/nginx
