#!/bin/bash
# KEKCC LSF job wrapper.
# Array template: replace the sample workload before analysis.
# Example: bsub -J "ecal_array[1-21]%10" < execute_analyzer_array.sl

#BSUB -q s
#BSUB -n 1
#BSUB -J "ecal_mc_array[1]"
#BSUB -cwd /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run_simulation
#BSUB -o /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run_simulation/output/array.%J_%I.out
#BSUB -e /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run_simulation/output/array.%J_%I.err

script_dir="/gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run_simulation"
cd -- "$script_dir" || exit 1

MY_HOST=`hostname`
MY_DATE=`date`
#
# Disable core dump. (Remove this line if you want to create core files)
#
ulimit -c 0

echo "Running on $MY_HOST at $MY_DATE"
echo "================================================================"

#
# Print envirionment
#
#echo "Running environment":
#env | sort
#echo "================================================================"

#
# Replace the start run number and configuration file by yours
#
START_RUNNUM=1
TASK_INDEX=${LSB_JOBINDEX:-1}
RUNNUM=$(( START_RUNNUM + TASK_INDEX - 1 ))
#./meganalyzer -i example/bartenderMode.xml -r $RUNNUM -b
echo "Process ID: $TASK_INDEX"
sleep 90
RET=$?
echo "Process ID end: $TASK_INDEX"

echo "Exit status: $RET"
echo "================================================================"
END_DATE=`date`
echo "End of run is $END_DATE ($SECONDS sec)"
exit "$RET"
