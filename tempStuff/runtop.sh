#!/bin/csh
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
#source /usr/local/sklib_gcc4.8.5/skofl_19a/env.csh
#set mode = tst
set mode = run
set nrun = $1
set nnn  = `echo $nrun | cut -c 1-4`

echo $nrun

set WDIR = /home/jmcelwee/monitoring/tempStuff
set SUBDIR = /disk02/usr6/jmcelwee/monitoringData/rundir/subList/
set IDIR = /disk02/calib3/auto/top_diffuser/
cd $WDIR

echo $IDIR
#set IDIR = /disk02/usr6/pmehta/adrian_Gen_MC/082462

set i = 1
set count = 0

if ( -f $IDIR/top_diffuser.$nrun ) then
./convtop.sh $nrun
./extractiontop.csh $nrun
endif
