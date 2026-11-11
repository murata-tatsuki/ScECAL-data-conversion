# KEKCCでの利用について

このディレクトリは旧2022年専用workflowです。以下の旧サーバー向け手順は
現在のKEKCC用手順ではありません。通常の解析は ../run/README と
../run/global_config を参照してください。

現在の2022年rawデータは
/gpfs/group/ilc/users/murata/data_beamtest/ECAL_data/data/2022/sps/{e+,pi+,mu+}/
の粒子ディレクトリ直下にあり、このworkflowが想定するenergy階層はありません。
また、auto_decode / hl_decode は現在のbin/とMakefileの生成対象に含まれません。
shiftのコード・旧ログ・控えは旧環境の記録として保持しています。

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

