#!/bin/bash
##########################################################
# File Name: build.sh
# Author: ims
# Created Time: 2020年10月06日 星期二 11时00分33秒
##########################################################

#ubuntu root lack mkdocs resolve
# vi /usr/local/bin/mkdocs
# add follow context
# sys.path.append('/home/username/.local/lib/python3.8/site-packages')

if [[ $1 =~ "g" ]];then
    echo "listen on 80 port, global"
    mkdocs serve -a 0.0.0.0:80
else
    mkdocs serve -a 127.0.0.1:9000
fi

