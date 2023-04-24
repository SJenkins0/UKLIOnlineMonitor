#!/bin/csh -f

source /usr/local/sklib_gcc8/skofl_r30333/env.csh
source /usr/local/sklib_gcc8/root_v6.22.06_python3.6/bin/thisroot.csh

set nrun = $1
set nsub = `printf "%06d" $2`
echo $nsub
set mode = run

set RUNDIR = /disk2/disk03/calib4/usr/ukli/monitoring/rundir/
set DATADIR = /disk2/disk03/calib4/usr/ukli/monitoring/data/
set ODIR = /disk2/disk03/calib4/usr/ukli/monitoring/extDat/$nrun
set EXEC = /home/calib/uk_inj/backLog/top_diff/top_diff


if ( ! -d $ODIR ) then
    mkdir $ODIR
endif



set TOPDIR = $DATADIR/top_dif/$nrun
set TOPFILE = $ODIR/top_dif_$nrun\_$nsub.dat
set TTOPFILE = $ODIR/top_dif_$nrun.dat

    
$EXEC -f $TOPDIR/uktd.$nrun.$nsub.root  -o $TOPFILE

if ($nsub == 000001) then
    cp $TOPFILE $TTOPFILE && rm $TOPFILE
else
    cat $TOPFILE >> $TTOPFILE && rm $TOPFILE
endif
    




