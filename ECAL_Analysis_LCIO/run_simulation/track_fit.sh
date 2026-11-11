#！/bin/sh

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "${script_dir}/global_config"
mkdir -p "${script_dir}/output" || exit $?

layer=8

input_file=${simpleCalib_path}
# input_file=${calib_path}
trackfile=${trackFit_path}
# trackfile=${data_dir}/${year}/${beam}/trackFit/${date_tag}/${energy}
# trackfile=${data_dir}/${year}/${beam}/trackFit_layer16/${date_tag}/${energy}


#echo $datafile
#./main ${DIR_IN} ${DIR_OUT1} ${layer} 
mkdir -p ${trackfile}

 ##### for file loop
 num=0
for dat_name in $(ls ${input_file})
  do


  prefix=`basename $dat_name`
  # if [[ -e ${trackfile}/${prefix} ]]
  # then
  #   echo "\[${prefix}\] has been decoded, skip ..."
  #   continue
  # fi

  bsub -q "${LSF_QUEUE:-s}" -n 1 -cwd "${script_dir}" -J track_fit -o "${script_dir}/output/track_fit.%J.out" -e "${script_dir}/output/track_fit.%J.err" bash "${script_dir}/execute.sl" "${bin_dir}/track_fit" "${input_file}/${dat_name}" "${trackfile}/" "${layer}" || exit $?

done

