#!/bin/sh
#by sunbeamboy
#File Name:fire_daemon.sh
#2020-12-16 10:38:31
#############################

#set the process name
proc_name=fire_service

proc_num()
{
        num=`ps | grep $proc_name | grep -v grep | wc -l`
        return $num
}

sleep 60

echo "begin the daemon..."

while true
do
        proc_num
        number=$?
#if the process does not exist...
        if [ $number -eq 0 ]
        then
                sleep 3
                /etc/init.d/${proc_name} start
        fi
#if there are too many process...
        if [ 2 -le $number ]
        then
                reboot
        fi

        sleep 20

done