#!/bin/sh
#########################
# generate bash in batch
#WORKDIR="/cefs/higgs/wangjx/ScECAL/Diagnose/shift"

for EnergyDir in "10GeV" "20GeV" "30GeV" "40GeV" "50GeV" "60GeV" "80GeV" "100GeV" "120GeV"
do
	cp allrun_pi.sh ./batch/allrun_pi_"$EnergyDir".sh
	sed -i "19c if [ \$ENERGYDIR = '${EnergyDir}' ]" ./batch/allrun_pi_"$EnergyDir".sh
	sed -i "19s/^/\t\t\t\t/g" ./batch/allrun_pi_"$EnergyDir".sh
done

	#sed -i "%s/global_config/..\/global_config/g" ./batch/allrun_e_"$EnergyDir".sh
	#sed -i "%s/hl_decode/..\/global_config/g" ./batch/allrun_e_"$EnergyDir".sh
