#!/bin/csh -f
#
# Script to run UKLI selcalib monitoring
# S. Jenkins 31/5/22
#
# Last updated 24/4/23


setenv PATH /usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
source /home/calib/.cshrc
#source /usr/local/sklib_gcc8/cshenv_gcc8_skofl_21b+atmpd_21b
source /usr/local/sklib_gcc8/skofl_r30333/env.csh

source /home/calib/uk_inj/monitoring/selcalibbatch_check.csh

sleep 5m
source /home/calib/uk_inj/monitoring/plotting/autoplot.sh

#clean up log files that are over 1 week old
find /disk1/disk02/calib3/usr/ukli/monitoring/rundir/logs/* -mtime +7 -exec rm {} \;

