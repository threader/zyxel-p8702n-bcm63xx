#!/bin/sh

case "$1" in
	start)
		# BUILD_GUI_DEBUG

		mkdir /var/etc
		mkdir /var/etc/ppp
		mkdir /var/etc/ppp/chat
		cp /etc/pwdog/* /var/etc
		cp /etc/ppp/* /var/etc/ppp
		cp /etc/ppp/chat/* /var/etc/ppp/chat
		# automount
		#mount -t tmpfs mdev /dev/mdev
		#echo /sbin/mdev > /proc/sys/kernel/hotplug
		#mdev -s 
		# wwan data
		mkdir /var/etc
		cp -r /etc/wwan /var/etc
		#REPLACE_STRING_IF_BUILD_NORWAY_CUSTOMIZATION_1
		#REPLACE_STRING_IF_BUILD_NORWAY_CUSTOMIZATION_2
		#REPLACE_STRING_IF_BUILD_NORWAY_CUSTOMIZATION_3
		#REPLACE_STRING_IF_BUILD_NORWAY_CUSTOMIZATION_4
		# copy data-model
		if [ -e /etc/cms-data-model-merged.xml ]; then
		echo "cpoy cms-data-model.xml for reference..."
		cp -f /etc/cms-data-model-merged.xml /var/
		fi
		exit 0
		;;

	stop)
		echo "Unconfig..."
		exit 1
		;;

	*)
		echo "$0: unrecognized option $1"
		exit 1
		;;

esac

