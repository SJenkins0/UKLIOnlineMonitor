#!/bin/csh
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh

set nrun = $1

set DATADIR = /disk02/usr6/jmcelwee/monitoringData/extDat


if ( ! -e $DATADIR/top_diffuser.dat) then
    touch $DATADIR/top_diffuser.dat
endif
cat $DATADIR/$nrun/top_diffuser_$nrun.dat >> $DATADIR/top_diffuser.dat


set dir1 = /disk02/usr6/jmcelwee/monitoringData/data/top_diffuser/$nrun
set ofile1 = top_diffuser.$nrun.root
set dir = /disk02/usr6/jmcelwee/monitoringData/rundir
set n = 0

rm -rf $dir/outputFileTemp_top_diffuser.$nrun.root

foreach file (`ls -f $dir1/*.root`)
    echo $file
    if ($n == 0) then
	cp $file $dir/outputFileTemp_top_diffuser.$nrun.root
    else
	hadd -f $dir1/$ofile1 $file $dir/outputFileTemp_top_diffuser.$nrun.root
	echo $file
	rm -rf $dir/outputFileTemp_top_diffuser.$nrun.root
	cp $dir1/$ofile1 $dir/outputFileTemp_top_diffuser.$nrun.root
    endif # n
    @ n++
end # file
echo $dir1/$ofile1

      
rm -rf $dir/outputFileTemp_top_diffuser.$nrun.root

rm /disk02/usr6/jmcelwee/monitoringData/data/top_diffuser/$nrun/top_diffuser.$nrun.*.root
