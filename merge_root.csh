#!/bin/csh
source /usr/local/sklib_gcc8/cshenv_gcc8_skofl_21b+atmpd_21b

set nrun = $1
set opts1 = "col"
set opts2 = "dif"
set opts3 = "mon"
set op1 = "c"
set op2 = "d"
set op3 = "m"


set DATADIR = /disk2/disk03/calib4/usr/ukli/monitoring/extDat
set datCount = 1
while ($datCount <= 5)
    if ( ! -e $DATADIR/col_B$datCount.dat) then
	touch $DATADIR/col_B$datCount.dat
    endif
    if ( ! -e $DATADIR/dif_B$datCount.dat) then
	touch $DATADIR/dif_B$datCount.dat
    endif
    cat $DATADIR/$nrun/col_$nrun\_B$datCount.dat >> $DATADIR/col_B$datCount.dat
    cat $DATADIR/$nrun/dif_$nrun\_B$datCount.dat >> $DATADIR/dif_B$datCount.dat
    @ datCount++
end
if ( ! -e $DATADIR/top_dif.dat) then
    touch $DATADIR/top_dif.dat
endif
cat $DATADIR/$nrun/top_dif_$nrun.dat >> $DATADIR/top_dif.dat


set pos = 1
set inj = 1
while ($pos <= 5)

      echo "pos = " $pos

      while ($inj <= 3)

	    if ($inj == 1) then
	       set dir1 = /disk2/disk03/calib4/usr/ukli/monitoring/data/$opts1/b$pos/$nrun
	       set ofile1 = ukl$op1$pos.$nrun.root
	       set dir = /disk2/disk03/calib4/usr/ukli/monitoring/rundir
	       set n = 0

	       rm -f $dir/outputFileTemp_$op1$pos.$nrun.root

	       foreach file (`ls -f $dir1/*.root`)
		    echo $file
		    if ($n == 0) then
			cp $file $dir/outputFileTemp_$op1$pos.$nrun.root
		    else
			hadd -f $dir1/$ofile1 $file $dir/outputFileTemp_$op1$pos.$nrun.root
			echo $file
			rm -f $dir/outputFileTemp_$op1$pos.$nrun.root
			cp $dir1/$ofile1 /$dir/outputFileTemp_$op1$pos.$nrun.root
		    endif # n
		    @ n++
	       end # file
	       echo $dir1/$ofile1

	    else if ($inj == 2) then
		set dir2 = /disk2/disk03/calib4/usr/ukli/monitoring/data/$opts2/b$pos/$nrun
		set ofile2 = ukl$op2$pos.$nrun.root
		set dir = /disk2/disk03/calib4/usr/ukli/monitoring/rundir
		set n = 0

		rm -f $dir/outputFileTemp_$op2$pos.$nrun.root

		foreach file (`ls -f $dir2/*.root`)
		    echo $file
		    if ($n == 0) then
			cp $file $dir/outputFileTemp_$op2$pos.$nrun.root
		    else
			hadd -f $dir2/$ofile2 $file $dir/outputFileTemp_$op2$pos.$nrun.root
			echo $file
			rm -f $dir/outputFileTemp_$op2$pos.$nrun.root
			cp $dir2/$ofile2 /$dir/outputFileTemp_$op2$pos.$nrun.root
		    endif # n
		    @ n++
		end # file
	        echo $dir2/$ofile2

	    else
		set dir3 = /disk2/disk03/calib4/usr/ukli/monitoring/data/$opts3/$nrun
		set ofile3 = ukl$op3.$nrun.root
		set dir = /disk2/disk03/calib4/usr/ukli/monitoring/rundir
		set n = 0

		rm -f $dir/outputFileTemp_$op3.$nrun.root

		foreach file (`ls -f $dir3/*.root`)
		    echo $file
		    if ($n == 0) then
			cp $file $dir/outputFileTemp_$op3.$nrun.root
		    else
			hadd -f $dir3/$ofile3 $file $dir/outputFileTemp_$op3.$nrun.root
			echo $file
			rm -f $dir/outputFileTemp_$op3.$nrun.root
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

#Do once more for top diffuser
set dir1 = /disk2/disk03/calib4/usr/ukli/monitoring/data/top_dif/$nrun
set ofile1 = uktd.$nrun.root
set dir = /disk2/disk03/calib4/usr/ukli/monitoring/rundir
set n = 0

rm -f $dir/outputFileTemp_td.$nrun.root

foreach file (`ls -f $dir1/*.root`)
	    echo $file
	    if ($n == 0) then
		cp $file $dir/outputFileTemp_td.$nrun.root
	    else
		hadd -f $dir1/$ofile1 $file $dir/outputFileTemp_td.$nrun.root
		echo $file
		rm -f $dir/outputFileTemp_td.$nrun.root
		cp $dir1/$ofile1 /$dir/outputFileTemp_td.$nrun.root
	    endif # n
	    @ n++
end # file
echo $dir1/$ofile1

rm -f $dir/outputFileTemp_*.$nrun.root

set position = 1
while ($position <= 5)
    rm -f /disk2/disk03/calib4/usr/ukli/monitoring/data/col/b$position/$nrun/uklc$position.$nrun.*.root
    rm -f /disk2/disk03/calib4/usr/ukli/monitoring/data/dif/b$position/$nrun/ukld$position.$nrun.*.root

    @ position++
end

rm -f /disk2/disk03/calib4/usr/ukli/monitoring/data/mon/$nrun/uklm.$nrun.*.root
rm -f /disk2/disk03/calib4/usr/ukli/monitoring/data/top_dif/$nrun/uktd.$nrun.*.root
