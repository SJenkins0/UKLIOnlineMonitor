#!/bin/csh -f

source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
source /home/jmcelwee/software/root6/root/bin/thisroot.csh

set nrun = $1
#set nrun = 082746
set nsub = `printf "%06d" $2`
echo $nsub
#set nsub = 000001
#set mode = $3
set mode = run

set RUNDIR = /disk02/usr6/jmcelwee/monitoringData/rundir/
set DATADIR = /disk02/usr6/jmcelwee/monitoringData/data/
set ODIR = /disk02/usr6/jmcelwee/monitoringData/extDat/$nrun
set EXEC = ./extraction/monitor


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
    
    $EXEC -f $COLDIR/uklc$i.$nrun.$nsub.root -c -i B$i -o $COLFILE
    $EXEC -f $DIFDIR/ukld$i.$nrun.$nsub.root -d -i B$i -o $DIFFILE

    if ($nsub == 000001) then
	cp $COLFILE $TCOLFILE && rm $COLFILE
	cp $DIFFILE $TDIFFILE && rm $DIFFILE
    else
	cat $COLFILE >> $TCOLFILE && rm $COLFILE
	cat $DIFFILE >> $TDIFFILE && rm $DIFFILE
    endif
    
    @ i++
end




