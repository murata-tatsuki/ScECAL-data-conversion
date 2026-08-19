#！/bin/sh

work_dir='/mnt2/debug/cheyuzhi/ecal_ana'
data_dir='/mnt2/debug/cheyuzhi/data'
layer=8

#----------- INPUT -----------
file_tag=221018
# input_file="${data_dir}/analyseCode/MIPCalib/calibration/${file_tag}"
input_file="${data_dir}/analyseCode/RawtoRoot/${file_tag}"

#----------- OUTPUT -----------
track_file="${data_dir}/analyseCode/TrackFit/${file_tag}/"

if [[ ! -d $track_file ]]
	then
	mkdir -p $track_file
fi

for dat_name in $(ls ${input_file})
do
	dat_name=`basename $dat_name`
	echo $track_file
	echo "./bin/track_fit ${input_file} ${track_file} ${layer}"
	./bin/track_fit ${input_file}/${dat_name} ${track_file} ${layer}
done

