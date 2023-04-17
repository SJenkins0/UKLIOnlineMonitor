#!/bin/sh

source /home/jmcelwee/monitoring/conv_env.sh

nnn=0825
nrun=85
nsub=""

HOMEDIR=/home/jmcelwee/monitoring/
DATADIR=/disk02/usr6/jmcelwee/monitoringData
DATALOC=$DATADIR/$nnn$nrun


if [[ -d $DATALOC/zbs ]]; then
    echo -e "\e[34;1m[INFO]\e[0m $DATALOC/zbs/ exists. Selecting events."
    for ifile in $DATALOC/zbs/rfm_*
    do
	echo -e "\e[34;1m[INFO]\e[0m Selecting events from $ifile"
	./../selcalib/selcalib_local $ifile
    done
    echo -e "\e[34;1m[INFO]\e[0m Selection complete! You can now run the extraction scripts."
fi
