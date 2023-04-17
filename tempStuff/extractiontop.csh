#!/bin/csh -f

source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
source /home/jmcelwee/software/root6/root/bin/thisroot.csh

set nrun = $1
#set nrun = 082746
#set nsub = 000001
#set mode = $3
set mode = run

set RUNDIR = /disk02/usr6/jmcelwee/monitoringData/rundir/
set DATADIR = /disk02/usr6/jmcelwee/monitoringData/data/
set ODIR = /disk02/usr6/jmcelwee/monitoringData/extDat/$nrun
#set EXEC = ./extraction/monitor
set EXEC = ./main


if ( ! -d $ODIR ) then
    mkdir $ODIR
endif



set TOPDIR = $DATADIR/top_diffuser/$nrun
set TOPFILE = $ODIR/top_diffuser_$nrun.dat
set TTOPFILE = $ODIR/top_diffuser_$nrun.dat

    
$EXEC -f $TOPDIR/top_diffuser.$nrun.root  -o $TOPFILE
    




