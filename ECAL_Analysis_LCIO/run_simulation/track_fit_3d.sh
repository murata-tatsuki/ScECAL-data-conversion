#！/bin/sh
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "${script_dir}/global_config"
mkdir -p "${script_dir}/output" || exit $?

#work_dir='/mnt2/debug/cheyuzhi/ecal_ana'
#data_dir='/mnt2/debug/cheyuzhi/data'
layer=8
# this layer is not used in the code
#
##----------- INPUT -----------
#file_tag=221018
## input_file="${data_dir}/analyseCode/MIPCalib/calibration/${file_tag}"
#input_file="${data_dir}/analyseCode/RawtoRoot/${file_tag}"
#
##----------- OUTPUT -----------
#track_file="${data_dir}/analyseCode/TrackFit/${file_tag}/"


# Strip Split
stripsplit_o=${stripSprit_path}

#track Fit
trackFit_o=${trackFit_path3D}

#------ Create Directores -----
mkdir -p ${trackFit_o}

for dat_name in $(ls ${stripsplit_o})
do
  file_name=`basename $dat_name`
  prefix=${file_name%.root}
  
  if [[ -e ${trackFit_o}/${prefix}.root ]]
  then
    echo "${trackFit_o}/${prefix} has been decoded, skip ..."
    continue
  fi

  mkdir -p ${trackFit_o}/graph

  echo "Track Fitting with SSA ${prefix}"
  # echo " ... ... "
  # ${bin_dir}/track_fit_3d ${stripsplit_o}/${dat_name} "${trackFit_o}/" ${layer}
  bsub -q "${LSF_QUEUE:-s}" -n 1 -cwd "${script_dir}" -J track_fit_3d -o "${script_dir}/output/track_fit_3d.%J.out" -e "${script_dir}/output/track_fit_3d.%J.err" bash "${script_dir}/execute.sl" "${bin_dir}/track_fit_3d" "${stripsplit_o}/${dat_name}" "${trackFit_o}/" "${layer}" || exit $?
  # echo "    Output dir ===> ${trackFit_o}/${prefix}.root "

  # echo "=============================="

#  ./bin/track_fit ${input_file}/${dat_name} ${track_file} ${layer}
done

