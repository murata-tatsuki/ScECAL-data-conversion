#!/bin/sh
##################

source /cvmfs/sft.cern.ch/lcg/views/LCG_101/x86_64-centos7-gcc11-opt/setup.sh
DATADIR="/cefs/higgs/wangjx/BeamData/ECAL/Particle/Combined"

##################
#cd ${DATADIR}
echo "execute all run file..."
function allrun(){
	for PARTICLEDIR in `ls $1`
	do 	
		if [ $PARTICLEDIR = 'e+' ]
		then
			echo " ******* NOW particledir is ${PARTICLEDIR} *******"
			for ENERGYDIR in `ls $1"/"$PARTICLEDIR`
			do
				#if [ "$ENERGYDIR" != 'BadRun' -a "$ENERGYDIR" != 'threshold_scan' ]
				if [ $ENERGYDIR = '80GeV' ]
				then
					echo "____________ NOW energydir is ${ENERGYDIR} ____________"
					for FILENAME in `ls $1"/"$PARTICLEDIR"/"$ENERGYDIR`
					do
						FileNo=$((${FILENAME:8:3}))
						#if [ $FileNo -le 297 -a $FileNo -ge 226 -a $FileNo -ne 236 -a $FileNo -ne 237 -a $FileNo -ne 238 -a $FileNo -ne 239 -a $FileNo -ne 240 -a $FileNo -ne 241  -a $FileNo -ne 242 -a $FileNo -ne 243 -a $FileNo -ne 263 ] 
						#if [ $FileNo -le 225 -a $FileNo -ge 222 ] 
						if [ $FileNo -le 297 ] 
						then
							echo " ====== NOW file is ${FILENAME} ======"
							echo " modifying the global_config ..."
							sed -i "9c particle_tag=$PARTICLEDIR" global_config
							sed -i "10c energy_tag=$ENERGYDIR" global_config
							sed -i "11c file_name=$FILENAME" global_config
							sh hl_decode.sh
							#sh calib.sh
							echo " ====== NOW ${FILENAME} finished ====="
						fi
					done
				fi
			done
		fi
	done
}

allrun ${DATADIR}
##################

#cd ${DATADIR}
#function getdir(){
#	for element in `ls $1`
#	do
#		if [ "$element" != 'BadRun' ] && [ "$element" != 'threshold_scan' ]
#		then
#			dir_or_file=$1"/"$element
#			if [ -d $dir_or_file ]
#			then
#				getdir $dir_or_file
#			else
#				echo $dir_or_file
#			fi
#		fi
#	done
#}
#
#getdir ${DATADIR}


