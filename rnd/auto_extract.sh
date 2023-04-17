#!/bin/sh
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.sh
source /home/jmcelwee/software/root6/root/bin/thisroot.sh
EXE=./../extraction/monitor
DATADIR=/disk02/usr6/lanthony/UKInjectorTests/data/ukli/root
ODIR=/disk02/usr6/jmcelwee/monitoringData/forLauren
for count in {500..800}; do
    for inj in {1..5}; do
	diffile=$DATADIR/ukld$inj.082$count.root
	colfile=$DATADIR/uklc$inj.082$count.root
	if [[ -e $diffile ]]; then
	    ./../extraction/monitor -f $diffile -i B$inj -d -o $ODIR/dif_082$count\_B$inj.dat
	fi
	if [[ -e $colfile ]]; then
	    ./../extraction/monitor -f $colfile -i B$inj -d -o $ODIR/col_082$count\_B$inj.dat
	fi
    done
done
