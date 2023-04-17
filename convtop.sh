#!/bin/csh

# A test program for UK injector works. J.K Jan.23, 2018

source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh
#source /usr/local/sklib_gcc4.8.5/skofl_19a/env.csh

set nrun = $1
set nsub = `printf "%06d" $2`
set nnn  = `echo $nrun | cut -c 1-4`

#set ODIR = /disk02/usr6/pmehta/adrian_Gen_MC/selectdata/data/
#set ODIR = /disk02/usr6/pmehta/adrian_Gen_MC/selectdata/data/
#set ODIR = /disk02/usr6/pmehta/neil_help/data/
#set ODIR = /disk02/usr6/mccauley/selcalib/data
set ODIR = /disk02/usr6/jmcelwee/monitoringData/data

#set IDIR = /disk02/data7/sk5/$mode/$nnn/$nrun
#set IDIR = /disk01/data6/sk4/$mode/$nnn/$nrun
set IDIR = /disk02/calib3/realtime/top_diffuser/$nrun


set ifile = $IDIR/top_diffuser.$nrun.$nsub
set outdirTOP = $ODIR/top_diffuser/$nrun

if ( ! -d $outdirTOP ) then
    mkdir $outdirTOP
endif

cp $ifile $outdirTOP

set exec = /home/jmcelwee/ProcessZBSToRoot/example3c
$exec $outdirTOP/top_diffuser.$nrun.$nsub


rm $outdirTOP/top_diffuser.$nrun.$nsub

rm -rf $outdirTOP/top_diffuser.$nrun.$nsub
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


