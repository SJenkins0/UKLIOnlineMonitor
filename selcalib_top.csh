#!/bin/csh -f
#

#source /home/jmcelwee/.bashrc
source /home/jmcelwee/.cshrc
source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
set maxcpu = 60
#set nrun = 82586
#set val=$2
#set indir=/disk01/calib2/auto/water/2017GainCorrectedData
#set outdir=/disk01/calib2/auto/water/2017GainCorrectedHbooks
#set outdir=/disk01/usr5/lanthony/auto_wt_sctt/mkhbook-4.2/testsfornqs
#set workdir=/disk01/usr5/lanthony/auto_wt_sctt/mkhbook-4.2
set logdir=/disk02/usr6/jmcelwee/monitoringData/rundir/logs
set rundir=/disk02/usr6/jmcelwee/monitoringData/rundir
set workdir=/home/jmcelwee/monitoring/
#set file1 = $workdir/sk5test.txt
#set file1 = $workdir/082742.txt
cd $workdir


set IDIR = /disk02/calib3/realtime/top_diffuser
ls $IDIR > $workdir/topDiffList.txt
touch $workdir/oldTopList.txt
#sed 's/^0//' $workdir/oldTopList.txt 
awk '{gsub("^0*","")}' $workdir/oldTopList.txt


diff $workdir/oldTopList.txt $workdir/topDiffList.txt | grep '> ' | sed 's/> //' > $workdir/newtoplistTEMP.txt
sed 's/^.//' $workdir/newtoplistTEMP.txt > $workdir/newtoplist.txt
awk '{gsub("op_diffuser.", "");print}' newtoplist.txt
#sed 's/op_diffuser.//' $workdir/newtoplist.txt
rm $workdir/newtoplistTEMP.txt $workdir/oldTopList.txt
cp $workdir/topDiffList.txt $workdir/oldTopList.txt
rm $workdir/topDiffList.txt 
set file1 = $workdir/newtoplist.txt

set i = 1
set list = `less $file1`
set imax = $#list


while($i <= $imax)
set nrun = `printf $list[$i]`
set nrun = `printf "%06d" $nrun`
echo "i = " $i ", nrun = " $nrun

set njobs=`qstat calib | grep jmcelwee | wc -l`

if ( -d $IDIR/$nrun ) then 
    echo $IDIR/$nrun

    if ( ! -f $rundir/topdiffuser_$nrun.sh ) then

	cat > $rundir/topdiffuser_$nrun.sh <<EOF
#!/bin/csh -f 
	
$workdir/runtop.sh $nrun

EOF

	set njobs=`qstat calib | grep jmcelwee | wc -l`
	while ($njobs >= $maxcpu)
	echo 'njobs used = ' $njobs
	sleep 1
	unset njobs
	set njobs=`qstat calib | grep jmcelwee | wc -l`
	end
	chmod u+x $rundir/topdiffuser_$nrun.sh
#	qsub -q calib -o $logdir/topdiffuser_$nrun.log -e $logdir/topdiffuser_$nrun.err $rundir/topdiffuser_$nrun.sh
	sleep 10
	
    endif

endif

@ i++


end

rm $workdir/newtoplist.txt
