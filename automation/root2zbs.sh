#!/bin/sh

source /home/jmcelwee/monitoring/conv_env.sh

nnn=0825
nrun=85
nsub=""

DATADIR=/disk02/usr6/jmcelwee/monitoringData
DATALOC=$DATADIR/$nnn$nrun
RUNLOC=/disk02/data7/sk5/run/$nnn/$nnn$nrun/


if [[ ! -d $DATALOC ]]; then
    mkdir -p $DATALOC/raw
    echo -e "\e[34;1m[INFO]\e[0m Creating directory: $DATALOC/raw"
    echo -e "\e[34;1m[INFO]\e[0m Copying data from $RUNLOC"
    cp -v $RUNLOC/rfm_run$nnn${nrun}.*.root $DATALOC/raw
fi


if [[ ! -d $DATALOC/zbs ]]; then
    mkdir -p $DATALOC/zbs
    echo -e "\e[34;1m[INFO]\e[0m Creating directory: $DATALOC/zbs"
    echo -e "\e[34;1m[INFO]\e[0m Extracting from raw data to: $DATALOC/zbs"
    for ifile in $DATALOC/raw/*.root
    do
	ofile=${ifile%.root}
	ofile=${ofile#$DATALOC/raw/}
	echo -e "\e[34;1m[INFO]\e[0m Extracting to file $ofile"
	root2zbs $ifile $DATALOC/zbs/$ofile
    done
    echo -e "\e[34;1m[INFO]\e[0m Extraction complete! You can find the data in: $DATALOC/zbs"
fi
