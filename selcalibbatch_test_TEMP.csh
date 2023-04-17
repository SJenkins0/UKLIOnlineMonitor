#!/bin/csh -f
#

#source /home/jmcelwee/.bashrc
source /home/jmcelwee/.cshrc
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
set maxcpu = 30
#set nrun = 82586
#set val=$2
#set indir=/disk01/calib2/auto/water/2017GainCorrectedData
#set outdir=/disk01/calib2/auto/water/2017GainCorrectedHbooks
#set outdir=/disk01/usr5/lanthony/auto_wt_sctt/mkhbook-4.2/testsfornqs
#set workdir=/disk01/usr5/lanthony/auto_wt_sctt/mkhbook-4.2
set logdir=/disk02/usr6/jmcelwee/monitoringData/rundir/logs
set rundir=/disk02/usr6/jmcelwee/monitoringData/rundir
set workdir=/home/jmcelwee/monitoring/tempStuff/
#set file1 = $workdir/sk5test.txt
#set file1 = $workdir/082742.txt
cd $workdir

set IDIR = /disk02/data7/sk5/run/
ls $IDIR > $workdir/currentrunlist.txt

set toplist = `tail $workdir/currentrunlist.txt`
#echo $toplist
awk '$1~/^[0]/' $workdir/currentrunlist.txt > $workdir/inlist.txt
set nnn = 0825
rm $workdir/inlist.txt $workdir/currentrunlist.txt

set IDIR2 = /disk02/data7/sk5/run/$nnn
touch $workdir/oldlist.txt
ls $IDIR2 > $workdir/runlistTEMP.txt
diff $workdir/oldlist.txt $workdir/runlistTEMP.txt | grep '> ' | sed 's/> //' > $workdir/newlistTEMP.txt
sed 's/^.//' $workdir/newlistTEMP.txt > $workdir/newlist.txt
rm $workdir/oldlist.txt $workdir/newlistTEMP.txt
cp $workdir/runlistTEMP.txt $workdir/oldlist.txt
rm $workdir/runlistTEMP.txt 
set file1 = $workdir/newlist.txt



set i = 1
set list = `less $file1`
set imax = $#list
#echo $imax
#echo "first element =" ${list[$i]}
#echo "last element =" ${list[$imax]}

while($i <= $imax)
set nrun = `printf $list[$i]`
echo "i = " $i ", nrun = " $nrun

cat > $rundir/ukli_$nrun.sh <<EOF

#!/bin/csh -f
#

$workdir/runmultiple_run_TEMP.sh $nrun

EOF

set njobs=`qstat calib | grep jmcelwee | wc -l`
while ($njobs >= $maxcpu)
echo 'njobs used = ' $njobs
sleep 1
unset njobs
set njobs=`qstat calib | grep jmcelwee | wc -l`
end
chmod u+x $rundir/ukli_$nrun.sh
qsub -q calib -o $logdir/ukli_$nrun.log -e $logdir/ukli_$nrun.err $rundir/ukli_$nrun.sh
#sleep 10
    


@ i++


end

rm $workdir/newlist.txt
