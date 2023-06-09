#!/bin/csh -f
#

set SKRUN=7

source /home/calib/.cshrc
source /usr/local/sklib_gcc8/skofl_r30333/env.csh
set maxcpu = 60

#Set up disk paths now that we've had to move calib area to disk2
set DISK2=/disk2/disk03/calib4/usr/ukli

set logdir=${DISK2}/monitoring/rundir/logs
set rundir=${DISK2}/monitoring/rundir
set workdir=/home/calib/uk_inj/monitoring/
cd $workdir

#set IDIR = /disk02/data7/sk$SKRUN/run/  #old directory on disk1
set IDIR = /disk2/disk03/data8/sk$SKRUN/run/

set nnn = 0904

set IDIR2 = /disk2/disk03/data8/sk$SKRUN/run/$nnn
touch $workdir/old_904.txt
ls $IDIR2 > $workdir/runlistTEMP.txt
diff $workdir/old_904.txt $workdir/runlistTEMP.txt | grep '> ' | sed 's/> //' > $workdir/newlistTEMP.txt
sed 's/^.//' $workdir/newlistTEMP.txt > $workdir/newlist.txt
yes | rm $workdir/old_904.txt $workdir/newlistTEMP.txt
cp $workdir/runlistTEMP.txt $workdir/old_904.txt
yes | rm $workdir/runlistTEMP.txt 
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

# Remove '/' - If something is wrong, might be this
# Have fun <3
set runnum=`echo $nrun | rev | cut -c 2- | rev`    

cat > $rundir/ukli_$runnum.sh <<EOF

#!/bin/csh -f
#

$workdir/runmultiple_run.sh $runnum

EOF

set njobs=`pjstat | grep calib | wc -l`
while ($njobs >= $maxcpu)
echo 'njobs used = ' $njobs
sleep 1
unset njobs
set njobs=`pjstat calib | grep calib | wc -l`
end
chmod u+x $rundir/ukli_$runnum.sh
pjsub -o $logdir/ukli_$runnum.log -e $logdir/ukli_$runnum.err -L rscgrp=calib --mail-list s.j.jenkins@liverpool.ac.uk -m e $rundir/ukli_$runnum.sh
sleep 10



@ i++


end

yes | rm $workdir/newlist.txt
