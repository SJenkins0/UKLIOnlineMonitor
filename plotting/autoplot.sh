#!/bin/csh
source /home/calib/.cshrc
source /usr/local/sklib_gcc8/root_v6.22.06_python3.6/bin/thisroot.csh

#old dir was /disk02/calib3
set RUNDIR=/disk03/calib4/usr/ukli/monitoring/plotting
set CRUNDIR=/disk03/calib4/usr/ukli/monitoring/data/col/b1
set DATADIR=/disk03/calib4/usr/ukli/monitoring/extDat
set PLOTDIR=/home/calib/uk_inj/monitoring/plotting
set WEBDIR=/home/calib/public_html/water_scattering/monitoring/uk_gdload
set exec=$PLOTDIR/plotting
set execMon=$PLOTDIR/monPlot
set execTop=$PLOTDIR/topDiff
set execNoise=$PLOTDIR/noise
set var=`date +'%m%d%y'`


#Get the list of currently running jobs
pjstat --filter usr=calib --filter "jnam=ukli*" --choose jnam > $RUNDIR/plottinglist.txt
set files=`cat $RUNDIR/plottinglist.txt`
set prefix="ukli_"

yes | cp $PLOTDIR/col_spotQ_day.png $RUNDIR/old_plots/col_spotQ_day_$var.png
yes | cp $PLOTDIR/col_totQ_day.png $RUNDIR/old_plots/col_totQ_day_$var.png
yes | cp $PLOTDIR/dif_spotQ_day.png $RUNDIR/old_plots/dif_spotQ_day_$var.png
yes | cp $PLOTDIR/dif_totQ_day.png $RUNDIR/old_plots/dif_totQ_day_$var.png
yes | cp $PLOTDIR/col_monQ_day.png $RUNDIR/old_plots/col_monQ_day_$var.png
yes | cp $PLOTDIR/dif_monQ_day.png $RUNDIR/old_plots/dif_monQ_day_$var.png
yes | cp $PLOTDIR/col_normQ_day.png $RUNDIR/old_plots/col_normQ_day_$var.png
yes | cp $PLOTDIR/dif_normQ_day.png $RUNDIR/old_plots/dif_normQ_day_$var.png
yes | cp $PLOTDIR/col_normSpotQ_day.png $RUNDIR/old_plots/col_normSpotQ_day_$var.png
yes | cp $PLOTDIR/dif_normSpotQ_day.png $RUNDIR/old_plots/dif_normSpotQ_day_$var.png


set inj=1
while ($inj <= 5 )
    cat $DATADIR/col_B$inj.dat > $RUNDIR/col_B$inj.dat
    cat $DATADIR/dif_B$inj.dat > $RUNDIR/dif_B$inj.dat
    set i=2
    while ( $i <= $#files )
	set runNow=`echo $files[$i] | sed "s/^$prefix/0/"`
	cat $DATADIR/$runNow/col_${runNow}_B$inj.dat >> $RUNDIR/col_B$inj.dat
	cat $DATADIR/$runNow/dif_${runNow}_B$inj.dat >> $RUNDIR/dif_B$inj.dat
	@ i++
    end
    yes | cp $RUNDIR/col_B$inj.dat $WEBDIR/text/
    yes | cp $RUNDIR/dif_B$inj.dat $WEBDIR/text/
    @ inj++
end

cat $DATADIR/top_dif.dat > $RUNDIR/top_dif.dat
set i=2
    while ( $i <= $#files )
    set runNow=`echo $files[$i] | sed "s/^$prefix/0/"`
    cat $DATADIR/$runNow/top_dif_$runNow.dat >> $RUNDIR/top_dif.dat
    @ i++
end
yes | cp $RUNDIR/top_dif.dat $WEBDIR/text/


$exec > & $RUNDIR/logn.txt
$exec -d > & $RUNDIR/logd.txt
$exec -c > & $RUNDIR/logc.txt
#$execMon -d > $RUNDIR/log.txt
#$execMon -c > $RUNDIR/log.txt
$execTop > & $RUNDIR/logt.txt

#move files to webpage dirs
yes | cp $PLOTDIR/*.png $WEBDIR/plots/
