#!/bin/sh
rm /tmp/echoLog
cd $1
if make
then
make install
fi
killall -9 nginx;/usr/local/nginx/sbin/nginx
curl -i localhost/echo
