#! /bin/bash

function INT_cleanup()
{
    kill `jobs -pr`
    exit
}

function run_sim()
{
	WORKDIR=$TESTDIR/$1/$2/
	mkdir -p $WORKDIR
	cp $CONF $COORDS $WORKDIR/

	cd $WORKDIR
	mkdir -p logs flows mobility outputFiles store_cap
	
	echo RAND_INIT = $RANDOM > included.cfg
	$GENBIN $1 $SIM_TIME 1 $2 1 2>err1.log > run1.log;

	echo RAND_INIT = $RANDOM > included.cfg;
	$BIN $1 $SIM_TIME 2>err2.log > run2.log;
	
	echo RAND_INIT = $RANDOM > included.cfg;
	$BIN $1 $SIM_TIME 2>err3.log > run3.log
}	

# Don't want to leave any simulations running.
trap INT_cleanup INT

PROJDIR=$PWD	# Set to the absolute path of the working directory.
TESTDIR=$PROJDIR/tests
BIN=$PROJDIR/bin/lo3mac
GENBIN=$PROJDIR/bin/gen_lo3mac
CONF=lo3sim.cfg
COORDS=coordinateFile.txt

ERR_RATES=(0.01 0.02 0.05)
SIM_TIME=2
CALL_TIME=1
CALL_RATES=(0.33 0.5 1.0)
STR_TIME=1

# Use the no. of logical CPU cores available to limit no. of
# active jobs.
NJOBS=`lscpu | grep '^CPU(s):' | grep -Eo '[0-9]+'`

# Make sure both binaries are up-o-date, even if called by make.
make norm
make gen

# Uncomment IncludeCfg line in base config.
sed -i '/.*IncludeCfg.*/s/^[ \t]*#//' $CONF

mkdir -p $TESTDIR

for e in ${ERR_RATES[@]}
do
	for c in ${CALL_RATES[@]}
	do
		echo $e $c
		run_sim $e $c &
	done
done

wait
