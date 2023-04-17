#!/bin/csh
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh

set nrun = $1
set opts1 = "col"
set opts2 = "dif"
set opts3 = "mon"
set op1 = "c"
set op2 = "d"
set op3 = "m"



set pos = 1
set inj = 1
while ($pos <= 5)

      echo "pos = " $pos

      while ($inj <= 3)

	    if ($inj == 1) then
	       set dir1 = /disk02/usr6/jmcelwee/monitoringData/data/$opts1/b$pos/$nrun
	       set ofile1 = ukl$op1$pos.$nrun.root
	       set dir = /disk02/usr6/jmcelwee/monitoringData/rundir
	       set n = 0

	       rm -rf $dir/outputFileTemp_$op1$pos.$nrun.root

	       foreach file (`ls -f $dir1/*.root`)
		    echo $file
		    if ($n == 0) then
			cp $file $dir/outputFileTemp_$op1$pos.$nrun.root
		    else
			hadd -f $dir1/$ofile1 $file $dir/outputFileTemp_$op1$pos.$nrun.root
			echo $file
			rm -rf $dir/outputFileTemp_$op1$pos.$nrun.root
			cp $dir1/$ofile1 /$dir/outputFileTemp_$op1$pos.$nrun.root
		    endif # n
		    @ n++
	       end # file
	       echo $dir1/$ofile1

	    else if ($inj == 2) then
		set dir2 = /disk02/usr6/jmcelwee/monitoringData/data/$opts2/b$pos/$nrun
		set ofile2 = ukl$op2$pos.$nrun.root
		set dir = /disk02/usr6/jmcelwee/monitoringData/rundir
		set n = 0

		rm -rf $dir/outputFileTemp_$op2$pos.$nrun.root

		foreach file (`ls -f $dir2/*.root`)
		    echo $file
		    if ($n == 0) then
			cp $file $dir/outputFileTemp_$op2$pos.$nrun.root
		    else
			hadd -f $dir2/$ofile2 $file $dir/outputFileTemp_$op2$pos.$nrun.root
			echo $file
			rm -rf $dir/outputFileTemp_$op2$pos.$nrun.root
			cp $dir2/$ofile2 /$dir/outputFileTemp_$op2$pos.$nrun.root
		    endif # n
		    @ n++
		end # file
	        echo $dir2/$ofile2

	    else
		set dir3 = /disk02/usr6/jmcelwee/monitoringData/data/$opts3/$nrun
		set ofile3 = ukl$op3.$nrun.root
		set dir = /disk02/usr6/jmcelwee/monitoringData/rundir
		set n = 0

		rm -rf $dir/outputFileTemp_$op3.$nrun.root

		foreach file (`ls -f $dir3/*.root`)
		    echo $file
		    if ($n == 0) then
			cp $file $dir/outputFileTemp_$op3.$nrun.root
		    else
			hadd -f $dir3/$ofile3 $file $dir/outputFileTemp_$op3.$nrun.root
			echo $file
			rm -rf $dir/outputFileTemp_$op3.$nrun.root
			cp $dir3/$ofile3 /$dir/outputFileTemp_$op3.$nrun.root
		    endif # n
		    @ n++
		end # file
		echo $dir3/$ofile3
	    endif # inj
	    @ inj++
      end # inj
      set inj=1
      @ pos++
end # pos

rm -rf $dir/outputFileTemp_*.root
