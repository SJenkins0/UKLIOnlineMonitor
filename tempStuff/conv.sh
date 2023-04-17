#!/bin/csh

# A test program for UK injector works. J.K Jan.23, 2018

source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
#source /usr/local/sklib_gcc4.8.5/skofl_19a/env.csh

set mode = $1
set nrun = $2
set nsub = `printf "%06d" $3`
set nnn  = `echo $nrun | cut -c 1-4`

#set ODIR = /disk02/usr6/pmehta/adrian_Gen_MC/selectdata/data/
#set ODIR = /disk02/usr6/pmehta/adrian_Gen_MC/selectdata/data/
#set ODIR = /disk02/usr6/pmehta/neil_help/data/
#set ODIR = /disk02/usr6/mccauley/selcalib/data
set ODIR = /disk02/usr6/jmcelwee/monitoringData/data

#set IDIR = /disk02/data7/sk5/$mode/$nnn/$nrun
#set IDIR = /disk01/data6/sk4/$mode/$nnn/$nrun
set IDIR = /disk02/data7/sk5/$mode/$nnn/$nrun


set ifile = $IDIR/rfm_$mode$nrun.$nsub.root
set ofile = $ODIR/rfm/rfm_$mode$nrun.$nsub
#set ofile2 = $ODIR/ofl/ofl_$mode$nrun.$nsub

if (! -f $ifile ) then
echo $ifile " Doesn't Exist. Stop"
exit
endif


if ( -f $ifile ) then
echo $ifile "is processed .."
if (! -e $ofile) then
    /usr/local/sklib_gcc4.8.5/skofl_19b/bin/root2zbs $ifile $ofile
#    /usr/local/sklib_gcc4.8.5/skofl_19a/bin/root2zbs $ifile $ofile
endif
#/usr/local/sklib_gcc4.8.5/skofl_19a/bin/tqreal $ofile $ofile2

set outdirukc1 = $ODIR/col/b1/$nrun
set outdirukc2 = $ODIR/col/b2/$nrun
set outdirukc3 = $ODIR/col/b3/$nrun
set outdirukc4 = $ODIR/col/b4/$nrun
set outdirukc5 = $ODIR/col/b5/$nrun
set outdirukd1 = $ODIR/dif/b1/$nrun
set outdirukd2 = $ODIR/dif/b2/$nrun
set outdirukd3 = $ODIR/dif/b3/$nrun
set outdirukd4 = $ODIR/dif/b4/$nrun
set outdirukd5 = $ODIR/dif/b5/$nrun
set outdirukm = $ODIR/mon/$nrun

if ( ! -d $outdirukc1 ) then
    mkdir $outdirukc1
    mkdir $outdirukc2
    mkdir $outdirukc3
    mkdir $outdirukc4
    mkdir $outdirukc5
    mkdir $outdirukd1
    mkdir $outdirukd2
    mkdir $outdirukd3
    mkdir $outdirukd4
    mkdir $outdirukd5
    mkdir $outdirukm
    mkdir $ODIR/water/$nrun
    mkdir $ODIR/laser/$nrun
    mkdir $ODIR/xenon/$nrun
    mkdir $ODIR/led/$nrun
endif


if ( -e $ofile ) then
    ./selcalib/selcalib_local $ofile
#/disk02/usr6/apritcha/InjectorTests/July2019/selectdata/selcalib/selcalib $ofile | tee log.txt


set exec = /home/jmcelwee/ProcessZBSToRoot/example3c
# foreach -f (`outdirukd1`)
$exec $outdirukc1/uklc1.$nrun.$nsub
$exec $outdirukc2/uklc2.$nrun.$nsub
$exec $outdirukc3/uklc3.$nrun.$nsub
$exec $outdirukc4/uklc4.$nrun.$nsub
$exec $outdirukc5/uklc5.$nrun.$nsub
$exec $outdirukd1/ukld1.$nrun.$nsub
$exec $outdirukd2/ukld2.$nrun.$nsub
$exec $outdirukd3/ukld3.$nrun.$nsub
$exec $outdirukd4/ukld4.$nrun.$nsub
$exec $outdirukd5/ukld5.$nrun.$nsub
$exec $outdirukm/uklm.$nrun.$nsub


rm $outdirukc1/uklc1.$nrun.$nsub
rm $outdirukc2/uklc2.$nrun.$nsub
rm $outdirukc3/uklc3.$nrun.$nsub
rm $outdirukc4/uklc4.$nrun.$nsub
rm $outdirukc5/uklc5.$nrun.$nsub
rm $outdirukd1/ukld1.$nrun.$nsub
rm $outdirukd2/ukld2.$nrun.$nsub
rm $outdirukd3/ukld3.$nrun.$nsub
rm $outdirukd4/ukld4.$nrun.$nsub
rm $outdirukd5/ukld5.$nrun.$nsub
rm $outdirukm/uklm.$nrun.$nsub
rm -rf $ODIR/water/$nrun/wtr*
rm -rf $ODIR/laser/$nrun/lsr*
rm -rf $ODIR/xenon/$nrun/xen*
rm -rf $ODIR/led/$nrun/led*

rm -rf $ofile 
# /home/apritcha/work/ProcessZBSToRoot/example3c $ofile

endif

#echo files
#echo $ifile
#echo $ofile
#echo $ofile2


##
## Final output will be placed at usual place:
## under /disk01/calib2/realtime/water
##


