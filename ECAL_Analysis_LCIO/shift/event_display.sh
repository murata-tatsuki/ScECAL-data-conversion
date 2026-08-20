#! /bin/bash
source ./global_config

#------ I/O directories for each processor -------
# Decode: Raw to ROOT
decode_i=${raw_path}
decode_o=${decode_path}

# Pedestal
pedestal_i=${decode_path}
pedestal_o_1=${ped_hist}
pedestal_o_2=${ped_fit}

# MIP Fit
mip_i=${decode_path}
mip_o_1=${mip_hist}
mip_o_2=${mip_fit}

# Calibration
calib_o=${calib_path}

# Event Display
event_display_i=${calib_path}
event_display_o=${event_display_path}

#------ Create Directores -----
mkdir -p ${event_display_o}

prefix=${file_name%.dat}
prefix=${prefix%.root}

echo "Event Display ${prefix}"
echo " ... ... "

if  [[-e {event_display_o}/${prefix}.root ]]
then 
	rm {event_display_o}/${prefix}.root
	echo "...delete the exist file and make a new one"
fi

for ((i=10;i<13;i++))
do
	trigger=${i}
	${bin_dir}/event_display ${event_display_i}/${prefix}.root ${event_display_o}/${prefix}.root ${trigger}
done 

echo "    Output dir ===> ${event_display_o}/${prefix}.root "

echo "=============================="

# if [[ -e ${decode_o}/${date_tag}.root ]]
# then
# 	rm ${decode_o}/${date_tag}.root
# 	hadd ${decode_o}/${date_tag}.root ${decode_o}/*
# 	rm -i ${decode_o}/*_*.root
# else
# 	hadd ${decode_o}/${date_tag}.root ${decode_o}/*
# 	rm -i ${decode_o}/*_*.root
# fi
