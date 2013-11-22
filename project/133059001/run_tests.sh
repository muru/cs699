#! /bin/bash

SUCCESS_MSG="Run successful for"
FAIL_MSG="Run failed for"
PID=$$

function INT_cleanup()
{
    kill `jobs -pr`
	pkill lo3mac
	pkill parse_output.sh
    exit
}

function single_sim()
{
	SEED=$RANDOM
	echo RAND_INIT = $SEED > included.cfg

	if $1 $2 $SIM_TIME $3 $CALL_DURATION $STORED_VOICE_DURATION 2>err.log >run.log
	then
		echo $SUCCESS_MSG $1 $2 $3 $SEED 
	else
		echo $FAIL_MSG $1 $2 $3 $SEED
	fi
	
	$PROJDIR/parse_output.sh $TESTDIR/$PID-overall.txt $2 $3 >> $TESTDIR/$PID-flows.txt
}

function run_sim()
{
	WORKDIR=$TESTDIR/$1/$2
	mkdir -p $WORKDIR
	cp $CONF $COORDS $WORKDIR/

	cd $WORKDIR
	ARCHDIR=$PID-$1-$2
	mkdir -p $DATADIRS $ARCHDIR
		
	single_sim $GENBIN $1 $2 
	mkdir -p $ARCHDIR/run0
	cp -R $DATADIRS err.log run.log recordFlow.txt networkGraph.txt included.cfg $ARCHDIR/run0
	
	for i in `seq 1 1`
	do
		single_sim $BIN $1 $2
		mkdir -p $ARCHDIR/run$i
		cp -R $DATADIRS err.log run.log recordFlow.txt networkGraph.txt included.cfg $ARCHDIR/run$i
	done
	echo "tar -czf $ARCHIVES/$PID-$1-$2.tgz $ARCHDIR"
	cd $TESTDIR
	tar -czf $ARCHIVES/$PID-$1-$2.tgz $1/$2/$ARCHDIR

#	rm -rf $DATADIRS err.log run.log recordFlow.txt networkGraph.txt included.cfg
}	

# Don't want to leave any simulations running.
trap INT_cleanup INT

# Set to the absolute path of the working directory.
PROJDIR=~/cs699/project/133059001
TESTDIR=$PROJDIR/tests
ARCHIVES=$PROJDIR/archives
BIN=$PROJDIR/bin/lo3mac
GENBIN=$PROJDIR/bin/gen_lo3mac
DATADIRS="logs flows mobility outputFiles store_cap"
CONF=lo3sim.cfg
COORDS=coordinateFile.txt

mkdir -p $ARCHIVES

# Make sure both binaries are up-to-date,
# even if called by make.
make norm
make gen

ERR_RATES=(0.01 0.02 0.05)
SIM_TIME=2
CALL_DURATION=1
CALL_RATES=(0.3333333333 0.5 1.0)
STORED_VOICE_DURATION=1

# Use the no. of logical CPU cores available to limit
# no. of active jobs.
NJOBS=`lscpu | grep '^CPU(s):' | grep -Eo '[0-9]+'`
NJOBS=$(( NJOBS * 3 / 4))
echo $NJOBS

# Uncomment IncludeCfg line in base config.
sed -i '/.*IncludeCfg.*/s/^[ \t]*##*//' $CONF

echo '"Error Rate" "Call Rate" "Flow ID" Hops Latency "Avg Packet Loss" "Avg Jitter" "Median Jitter"' > $TESTDIR/$PID-flows.txt
echo '"Error Rate" "Call Rate" "Total Flows" "Accepted Flows" "Dropped Flows"' > $TESTDIR/$PID-overall.txt

for e in ${ERR_RATES[@]}
do
	for c in ${CALL_RATES[@]}
	do
		run_sim $e $c &
	done
done

wait

cp $TESTDIR/$PID* $ARCHIVES/
