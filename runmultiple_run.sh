#!/bin/csh

source /usr/local/sklib_gcc8/skofl_r30333/env.csh
#set mode = tst
set SKRUN=7
set mode = run
set nrun = `printf "%06d" $1`
set nnn  = `echo $nrun | cut -c 1-4`

set WDIR = /home/calib/uk_inj/monitoring/
set SUBDIR = /disk2/disk03/calib4/usr/ukli/monitoring/rundir/subList/

#set IDIR = /disk02/data7/sk$SKRUN/$mode/$nnn/$nrun
set IDIR = /disk2/disk03/data8/sk$SKRUN/$mode/$nnn/$nrun

set i = 1
set count = 0
set SRUNFILE = $SUBDIR/subrunlist_$nrun.txt
rm -f $SRUNFILE
ls $IDIR > $SRUNFILE
set imax = `wc -l < $SRUNFILE`

if ( -d $IDIR ) then
    while ($i < 3000)
      set nsub = `printf "%06d" $i`
      set fname = $IDIR/rfm_$mode$nrun.$nsub.root
      echo $fname
      rm -f $SRUNFILE
      ls $IDIR > $SRUNFILE
      set imax = `wc -l < $SRUNFILE`

      if ($count == 3) then #sleeps up to 1m30
        $WDIR/conv.sh $mode $nrun $i
	$WDIR/extraction.csh $nrun $i
	$WDIR/extractiontop.csh $nrun $i
      	break
      endif

      if ($i < $imax) then
	set count = 0
	if ( -e $fname ) then
	    $WDIR/conv.sh $mode $nrun $i
	    $WDIR/extraction.csh $nrun $i
	    $WDIR/extractiontop.csh $nrun $i
	endif  
	@ i++  # increment the subrun number
      else if ($i == $imax) then
	echo "Checking if run is still going..."
	sleep 30 
	@ count++
      endif
    end
else
    echo "Directory not found"
endif

echo -e "\e[1;34m[INFO]\e[0m We have done a thing yaaaay."
source $WDIR/merge_root.csh $nrun
