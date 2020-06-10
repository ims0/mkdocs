if [[ $1 =~ "global" ]];then
	mkdocs serve -a 0.0.0.0:8000
fi

mkdocs serve 
