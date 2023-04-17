#!/bin/csh

# A test program for UK injector works. J.K Jan.23, 2018
# Edited by S. Jenkins 30/5/2022
#source /usr/local/sklib_gcc8/cshenv_gcc8_skofl_21b+atmpd_21b
source /usr/local/sklib_gcc8/skofl_r30333/env.csh

set SKRUN=7
set mode = $1
set nrun = $2
set nsub = `printf "%06d" $3`
set nnn  = `echo $nrun | cut -c 1-4`

#set ODIR = /disk02/calib3/usr/ukli/monitoring/data
set ODIR = /disk2/disk03/calib4/usr/ukli/monitoring/data
#set IDIR = /disk02/data7/sk$SKRUN/$mode/$nnn/$nrun
set IDIR = /disk2/disk03/data8/sk$SKRUN/$mode/$nnn/$nrun

set ifile = $IDIR/rfm_$mode$nrun.$nsub.root
set ofile = $ODIR/rfm/rfm_$mode$nrun.$nsub

if (! -f $ifile ) then
echo $ifile " Doesn't Exist. Stop"
exit
endif

if ( -f $ifile ) then
echo $ifile "is processed .."
if (! -e $ofile) then
	/usr/local/sklib_gcc8/skofl_r30333/bin/root2zbs $ifile $ofile
endif

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
set outdiruktd = $ODIR/top_dif/$nrun

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
    mkdir $outdiruktd
    mkdir $ODIR/water/$nrun
    mkdir $ODIR/laser/$nrun
    mkdir $ODIR/xenon/$nrun
    mkdir $ODIR/led/$nrun
endif


if ( -e $ofile ) then
    /home/calib/uk_inj/selcalib_sam/selcalib_local $ofile >& /dev/null


set exec = /home/calib/uk_inj/zbs2root/example3c

$exec $outdirukc1/uklc1.$nrun.$nsub >& /dev/null
$exec $outdirukc2/uklc2.$nrun.$nsub >& /dev/null
$exec $outdirukc3/uklc3.$nrun.$nsub >& /dev/null
$exec $outdirukc4/uklc4.$nrun.$nsub >& /dev/null
$exec $outdirukc5/uklc5.$nrun.$nsub >& /dev/null
$exec $outdirukd1/ukld1.$nrun.$nsub >& /dev/null
$exec $outdirukd2/ukld2.$nrun.$nsub >& /dev/null
$exec $outdirukd3/ukld3.$nrun.$nsub >& /dev/null
$exec $outdirukd4/ukld4.$nrun.$nsub >& /dev/null
$exec $outdirukd5/ukld5.$nrun.$nsub >& /dev/null
$exec $outdirukm/uklm.$nrun.$nsub >& /dev/null
$exec $outdiruktd/uktd.$nrun.$nsub >& /dev/null


rm -f $outdirukc1/uklc1.$nrun.$nsub
rm -f $outdirukc2/uklc2.$nrun.$nsub
rm -f $outdirukc3/uklc3.$nrun.$nsub
rm -f $outdirukc4/uklc4.$nrun.$nsub
rm -f $outdirukc5/uklc5.$nrun.$nsub
rm -f $outdirukd1/ukld1.$nrun.$nsub
rm -f $outdirukd2/ukld2.$nrun.$nsub
rm -f $outdirukd3/ukld3.$nrun.$nsub
rm -f $outdirukd4/ukld4.$nrun.$nsub
rm -f $outdirukd5/ukld5.$nrun.$nsub
rm -f $outdirukm/uklm.$nrun.$nsub
rm -f $outdiruktd/uktd.$nrun.$nsub
rm -rf $ODIR/water/$nrun/wtr*
rm -rf $ODIR/laser/$nrun/lsr*
rm -rf $ODIR/xenon/$nrun/xen*
rm -rf $ODIR/led/$nrun/led*

rm -rf $ofile 


endif
