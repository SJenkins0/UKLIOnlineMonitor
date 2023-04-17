#!/bin/csh
source /home/calib/.cshrc
source /usr/local/sklib_gcc8/root_v6.22.06_python3.6/bin/thisroot.csh

CRUNDIR=/disk02/usr6/jmcelwee/monitoringData/data/col/b1
RUNDIR=/disk02/usr6/jmcelwee/monitoringData/plotting/
DATADIR=/disk02/usr6/jmcelwee/monitoringData/extDat
PLOTDIR=/home/jmcelwee/monitoring/plotting
exec=$PLOTDIR/plotting
execMon=$PLOTDIR/monPlot
execTop=$PLOTDIR/topDiff
var=`date +'%m%d%y'`

ls $CRUNDIR > $RUNDIR/plottinglist.txt
runNow=`tail -1 $RUNDIR/plottinglist.txt`
runs=`tail -2 $RUNDIR/plottinglist.txt`

cp $PLOTDIR/col_spotQ_day.png $RUNDIR/old_plots/col_spotQ_day_$var.png
cp $PLOTDIR/dif_spotQ_day.png $RUNDIR/old_plots/dif_spotQ_day_$var.png
cp $PLOTDIR/col_totQ_day.png $RUNDIR/old_plots/col_totQ_day_$var.png
cp $PLOTDIR/dif_totQ_day.png $RUNDIR/old_plots/dif_totQ_day_$var.png
cp $PLOTDIR/col_monQ_day.png $RUNDIR/old_plots/col_monQ_day_$var.png
cp $PLOTDIR/dif_monQ_day.png $RUNDIR/old_plots/dif_monQ_day_$var.png
cp $PLOTDIR/dif_monQ_day.png $RUNDIR/old_plots/col_normQ_day_$var.png
cp $PLOTDIR/dif_monQ_day.png $RUNDIR/old_plots/dif_normQ_day_$var.png 

for inj in {1..5}; do
    cat $DATADIR/col_B$inj.dat > $RUNDIR/col_B$inj.dat
    cat $DATADIR/dif_B$inj.dat > $RUNDIR/dif_B$inj.dat
    cat $DATADIR/$runNow/col_$runNow\_B$inj.dat >> $RUNDIR/col_B$inj.dat
    cat $DATADIR/$runNow/dif_$runNow\_B$inj.dat >> $RUNDIR/dif_B$inj.dat
done

cat $DATADIR/top_diffuser.dat > $RUNDIR/top_diffuser.dat
cat $DATADIR/$runNow/top_diffuser_$runNow.dat >> $RUNDIR/top_diffuser.dat





$exec -d > $RUNDIR/log.txt 2>&1
$exec -c > $RUNDIR/log.txt 2>&1
$execMon -d > $RUNDIR/log.txt 2>&1
$execMon -c > $RUNDIR/log.txt 2>&1
$execTop > $RUNDIR/log.txt 2>&1
