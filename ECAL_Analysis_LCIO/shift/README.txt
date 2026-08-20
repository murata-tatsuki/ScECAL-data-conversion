# Decode, Calibration and Diagnose Packages for CEPC ECAL Beam Test Data

Yuzhi, CHE
Jiaxuan, WANG

## Manual for users in Shift
##############################################################################
Step 1: Decode

- Enter the running directroy:	
##	cd /mnt2/ScECAL/Diagnose/shift

- Open global_config: 
##	vim global_config

- Specify the 'date_tag' in file `global_config`: 
##	change particle type

- Specify the 'filename' in file `global_config`: 
##	change file name

- Run decode.sh:	
##	sh decode.sh	

Result :	In the result directroy:  
##	/mnt2/ScECAL/Result/decode/

##############################################################################
Step 2: Calibrate

- Enter the running directroy:  cd /mnt2/ScECAL/Diagnose/run (Same Folder)

- Run calib.sh:		sh calib.sh

Result :
- Enter the result directroy:  
##	cd /mnt2/ScECAL/Result/calib/e+(or other particles)

- Record number of event

- Record energy deposition and hit layer profile

##############################################################################

