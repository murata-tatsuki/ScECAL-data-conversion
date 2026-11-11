#!/bin/bash
# KEKCC LSF job wrapper.
# Create output/ before submission, then: bsub < execute_analyzer_decode.sl

#BSUB -q s
#BSUB -n 1
#BSUB -J "ecal_mc_decode"
#BSUB -cwd /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run_simulation
#BSUB -o /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run_simulation/output/decode.%J.out
#BSUB -e /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run_simulation/output/decode.%J.err

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
# Replace configuration file by yours
#
#./meganalyzer -i example/bartenderMode.xml -r 1 -b
bash "${script_dir}/decode.sh"

RET=$?
echo "Exit status: $RET"
echo "================================================================"
END_DATE=`date`
echo "End of run is $END_DATE"
exit "$RET"
