#!/bin/bash

#AlesyaKarpenko
#03/2016
#Task 1




#Рисуем доску
printDesc()
{
	echo "${d[0]}${d[1]}${d[2]}"
	echo "${d[3]}${d[4]}${d[5]}"
	echo "${d[6]}${d[7]}${d[8]}"
}

#Проверяем координаты на корректность
check()
{
	ch=1
	if ! [ "$x" = "0" ]
	then if ! [ "$x" = "1" ]
		 then if ! [ "$x" = "2" ]
			  then echo 'Error! Enter x from zero to two'
			  ch=0
			  fi
		 fi
	fi
	if ! [ "$y" = "0" ]
	then if ! [ "$y" = "1" ]
		 then if ! [ "$y" = "2" ]
			  then echo 'Error! Enter y from zero to two'
			  ch=0
			  fi
		 fi
	fi
	i=`expr 3 \* $y + $x`
	if ! [ "${d[i]}" = " " ]
		then echo 'This point has already been completed'
		ch=0
	fi
}

#Заполняем нужную клетку значением
filling()
{
	ind=`expr 3 \* $y + $x`
	if [ "$step" = "1" ]
		then 
		d[$ind]=$figure1
	fi
	if [ "$step" = "2" ]
		then d[$ind]=$figure2
	fi
}

#Проверяем на выигрыш (wn отвечает за результат игры: ==0 - игра продолжается; =1 - есть победитель; =2 - ничья)
win()
{
	wn=0 
	if ! [ "${d[0]}" = " " ]; then
	if ! [ "${d[1]}" = " " ]; then
	if ! [ "${d[2]}" = " " ]; then
	if ! [ "${d[3]}" = " " ]; then
	if ! [ "${d[4]}" = " " ]; then
	if ! [ "${d[5]}" = " " ]; then
	if ! [ "${d[6]}" = " " ]; then
	if ! [ "${d[7]}" = " " ]; then
	if ! [ "${d[8]}" = " " ]; then
	wn=2                               
	fi fi fi fi fi fi fi fi fi
	for j in 0 3 6
	do 
		m=`expr $j + 1`
		n=`expr $j + 2`
		if [ "${d[$j]}" = "${d[$m]}" ]
		then 
		  if [ "${d[$m]}" = "${d[$n]}" ]
			then if [ "${d[$m]}" = "O" ]
					then
				    wn=1
				 fi
				if [ "${d[$m]}" = "X" ]
					then wn=1
				fi
		  fi
		fi
	done
	for j in 0 1 2
	do
		m=`expr $j + 3`
		n=`expr $j + 6`
		if [ "${d[$j]}" = "${d[$m]}" ]
			then
		    if [ "${d[$m]}" = "${d[$n]}" ]
				then if [ "${d[$m]}" = "O" ]
						then
						wn=1
					fi
				if [ "${d[$m]}" = "X" ]
					then wn=1
				fi
			fi
		fi
	done
	if [ "${d[0]}" = "${d[4]}" ]
	then if [ "${d[8]}" = "${d[4]}" ]
			then if [ "${d[4]}" = "O" ]
					then
					wn=1
				fi
			if [ "${d[4]}" = "X" ]
				then wn=1
			fi
		 fi
	fi
	if [ "${d[2]}" = "${d[4]}" ]
	then if [ "${d[6]}" = "${d[4]}" ]
			 then if [ "${d[4]}" = "O" ]
					  then
					  wn=1
				  fi
			 if [ "${d[4]}" = "X" ]
				then wn=1
			 fi
		 fi
	fi
}
	

if [ "$1" = "-help" ]
	then echo 'Run the program with -init'
	echo 'Enter coordinates from zero to two'
	exit 0
fi

#Файл нужен для того, чтобы понимать, чей ход (если 0, то ходит первый игрок, его фигура - крестики; если 1 - второй игрок, нолики)
if [ "$1" = "-init" ]
	then 
	if test f.txt
		then rm f.txt
	fi
	touch f.txt
	echo "0">f.txt
	if ! test l
		then mknod l p
	fi
	exit 0
fi

#Основной код программы

if ! test f.txt
	then echo 'Start program with -init'
	exit 0
fi
if ! test l
	then echo 'Start program with -init'
	exit 0
fi

a=`cat f.txt`
if  [ "$a" = "0" ]
	then 
	step=1
	figure1='X'
	figure2='O'
	echo "1">f.txt
else
	if [ "$a" = "1" ]
		then 
		step=2
		figure1='O'
		figure2='X'
		echo "0">f.txt
	else
		exit 0
	fi 
fi

d[0]=' '
d[1]=' '
d[2]=' '
d[3]=' '
d[4]=' '
d[5]=' '
d[6]=' '
d[7]=' '
d[8]=' '
d[9]=' '

while (true)
do
	clear
	#wn=0
	if [ "$step" = "1" ]
		then
		echo 'Lets go'
		printDesc
		read x y
		check
		if [ "$ch" = "0" ]
			then sleep 2
			continue
		fi
		str="$x$y"
		echo "$str">l
		filling
		step=2
		win
		if [ "$wn" = "1" ]
			then
			clear
			echo 'You are win!'
			printDesc
			exit 0
		fi
	else
		echo 'Waiting'
		printDesc
		str=`cat l`
		x="${str:0:1}"
		y="${str:1:1}"
		filling
		step=1
		win
		if [ "$wn" = "1" ]
			then
			clear
			echo 'You are looser!'
			printDesc
			exit 0
		fi
	fi
	if [ "$wn" = "2" ]
		then 
		clear
		echo 'Friendship wins!'
		printDesc
		exit 0
	fi
done