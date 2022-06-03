#!/bin/sh
#easy setup for	button drv

dtoverlay -d . button.dtbo
insmod button.ko

