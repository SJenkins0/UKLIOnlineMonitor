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
set EXEC = /home/calib/uk_inj/backLog/scripts/monitor

if ( ! -d $ODIR ) then
    mkdir $ODIR
endif

set i = 1
while ($i <= 5)    
    set COLDIR = $DATADIR/col/b$i/$nrun
    set DIFDIR = $DATADIR/dif/b$i/$nrun
    set DIFFILE = $ODIR/dif_$nrun\_$nsub\_B$i.dat
    set COLFILE = $ODIR/col_$nrun\_$nsub\_B$i.dat
    set TCOLFILE = $ODIR/col_$nrun\_B$i.dat
    set TDIFFILE = $ODIR/dif_$nrun\_B$i.dat
    
    $EXEC -f $COLDIR/uklc$i.$nrun.$nsub.root -c -i B$i -o $COLFILE >& /dev/null
    $EXEC -f $DIFDIR/ukld$i.$nrun.$nsub.root -d -i B$i -o $DIFFILE >& /dev/null

    if ($nsub == 000001) then
	cp $COLFILE $TCOLFILE && rm $COLFILE
	cp $DIFFILE $TDIFFILE && rm $DIFFILE
    else
	cat $COLFILE >> $TCOLFILE && rm $COLFILE
	cat $DIFFILE >> $TDIFFILE && rm $DIFFILE
    endif
    
    @ i++
end




