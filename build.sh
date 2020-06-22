if [[ $1 =~ "g" ]];then
	mkdocs serve -a 0.0.0.0:8000
else
	mkdocs serve 
fi

