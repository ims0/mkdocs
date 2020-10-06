##########################################################
# File Name: build.sh
# Author: ims
# Created Time: 2020年10月06日 星期二 11时00分33秒
##########################################################
#!/bin/bash

if [[ $1 =~ "g" ]];then
    mkdocs serve -a 0.0.0.0:8000
else
    mkdocs serve 
fi

