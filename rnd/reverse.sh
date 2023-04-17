#!/bin/sh
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.sh
source /home/jmcelwee/software/root6/root/bin/thisroot.sh
EXE=./../extraction/monitor
IDIR=/disk02/usr6/jmcelwee/monitoringData/forLauren
ODIR=/disk02/usr6/jmcelwee/monitoringData/forLauren2
for file in `ls $IDIR`; do
    tac $file > $ODIR/no.txt
done
