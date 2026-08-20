#！/bin/sh
source ./global_config

#------I/O dircetories ----
# HL Ratio
hl_ratio_i=${decode_path}
hl_ratio_o=${hl_ratio_path}

#------Create dircetories----
mkdir -p ${hl_ratio_o}

#------HL ratio ------
prefix=${file_name%.dat}

echo "Running HL ratio ${prefix}"
echo "... ..."
${bin_dir}/hl_ratio ${hl_ratio_i}/${prefix}.root ${hl_ratio_o}/${prefix}.root
echo "		Output dir ===> ${hl_ratio_o}/${prefix}.root"
echo "=================="

# path="$PWD"
# datafile=20201216_20210308
# inputfile="/mnt2/ScECAL_CR/results/trackFit/"
# caliFile="/mnt2/ScECAL_CR/results/calibration/"
# DIR_IN=${inputfile}${datafile}".root"
# DIR_OUT1=${caliFile}
# 
# ./main ${DIR_IN} ${DIR_OUT1}
# echo "all over"
