#!/bin/bash
# KEKCC LSF job wrapper.
# Submit with arguments via the corresponding .sh driver (see README).

#BSUB -q s
#BSUB -n 1
#BSUB -J "ecal_calib"
#BSUB -cwd /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run
#BSUB -o /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run/output/calib.%J.out
#BSUB -e /gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run/output/calib.%J.err

script_dir="/gpfs/group/ilc/users/murata/data_beamtest/analysis/ECAL_Analysis_LCIO/run"
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
# bash calib.sh

source "${script_dir}/global_config" || exit $?

file_name=$1
pede_factor=$2
inter_factor=$3
mip_factor=$4
calib_o=$5

echo "Calibration ${5}"
echo " ... ... "
#${bin_dir}/calib ${decode_o}/${file_name} ${pedestal_o_2}/${file_name} ${inter_factor} ${mip_factor} ${calib_o}/${prefix}.root #> ${log_dir}/${prefix}.log
"${bin_dir}/calib" "$1" "$2" "$3" "$4" "$5"
RET=$?
echo "    Output dir ===> ${5} "

echo "=============================="


echo "Exit status: $RET"
echo "================================================================"
END_DATE=`date`
echo "End of run is $END_DATE"
exit "$RET"
