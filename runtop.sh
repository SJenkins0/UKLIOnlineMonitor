#!/bin/csh
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
#source /usr/local/sklib_gcc4.8.5/skofl_19a/env.csh
#set mode = tst
set mode = run
set nrun = $1
set nnn  = `echo $nrun | cut -c 1-4`

echo $nrun

set WDIR = /home/jmcelwee/monitoring/
set SUBDIR = /disk02/usr6/jmcelwee/monitoringData/rundir/subList/
set IDIR = /disk02/calib3/realtime/top_diffuser/$nrun

echo $IDIR
#set IDIR = /disk02/usr6/pmehta/adrian_Gen_MC/082462

set i = 1
set count = 0
set SRUNFILE = $SUBDIR/subtoplist_$nrun.txt
rm $SRUNFILE
ls $IDIR > $SRUNFILE
set imax = `wc -l < $SRUNFILE`

if ( -d $IDIR ) then
    while ($i < 1500)
      set nsub = `printf "%06d" $i`
      set fname = $IDIR/top_diffuser.$nrun.$nsub
      echo $fname
      rm $SRUNFILE
      ls $IDIR > $SRUNFILE
      set imax = `wc -l < $SRUNFILE`

      if ($count == 15) then
        echo "Correcting old file $fname"
        ./convtop.sh $nrun $i
	./extractiontop.csh $nrun $i
      	break
      endif

      if ($i < $imax) then
	set count = 0
	if ( -e $fname ) then
      	    echo "Correcting old file $fname"
#	    /disk02/usr6/apritcha/InjectorTests/July2019/selectdata/conv.sh $mode $1 $i
	    ./convtop.sh $nrun $i
	    ./extractiontop.csh $nrun $i
#	    /disk02/usr6/lanthony/UKInjectorTests/selcalib/conv.sh $mode $1 $i
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
source $WDIR/merge_roottop.csh $nrun
