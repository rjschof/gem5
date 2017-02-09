#!/bin/bash

bench_home="/home/robert/benchmarks"
bench_home_x86="$bench_home/x86"
bench_home_arm="$bench_home/arm"

benchmarks=(
    "cjpeg"         # 0
    "djpeg"         # 1
    "decode"        # 2
    "encode"        # 3
    "epic"          # 4
    "mipmap"        # 5
    "mpeg2decode"   # 6
    "mpeg2encode"   # 7
    "osdemo"        # 8 
    "pegwit"        # 9 - pegwit encode
    "pegwit"        # 10 - pegwit decode
    "rawcaudio"     # 11
    "rawdaudio"     # 12
    "sha"           # 13
    "susan"         # 14 
    "textgen"       # 15
    "toast"         # 16
    "unepic"        # 17
    "untoast"       # 18
)

benchmark_options=(
    "-dct int -progressive -opt -outfile ${bench_home}/files/jpeg/data/testout.jpeg ${bench_home}/files/jpeg/data/testimg.ppm"
    "-dct int -ppm -outfile ${bench_home}/files/jpeg/data/testout.ppm ${bench_home}/files/jpeg/data/testimg.jpg"
    "-4 -l -f ${bench_home}/files/g721/data/clinton.g721"        
    "-4 -l -f ${bench_home}/files/g721/data/clinton.pcm"
    "-b 25 ${bench_home}/files/epic/data/test_image.pgm"
    ">! ${bench_home}/files/mesa/mipmap"
    "-b ${bench_home}/files/mpeg2/data/mei16v2.m2v -r -f -o0 ${bench_home}/files/mpeg2/data/tmp%d"   
    "${bench_home}/files/mpeg2/data/options.par ${bench_home}/files/mpeg2/data/out.m2v"   
    ">! ${bench_home}/files/mesa/osdemo"        
    "-e ${bench_home}/files/pegwit/data/my.pub ${bench_home}/files/pegwit/data/pgptest.plain ${bench_home}/files/pegwit/data/pegwit.enc < ${bench_home}/files/pegwit/data/encryption_junk > ${bench_home}/files/pegwit/data/pegwitenc.shade"
    "-d ${bench_home}/files/pegwit/data/pegwit.enc ${bench_home}/files/pegwit/data/pegwit.dec < ${bench_home}/files/pegwit/data/my.sec > ${bench_home}/files/pegwit/data/pegwitdec.shade"
    "< ${bench_home}/files/adpcm/data/clinton.pcm > ${bench_home}/files/adpcm/results/out.adpcm"     
    "< ${bench_home}/files/adpcm/data/clinton.adpcm > ${bench_home}/files/adpcm/results/out.pcm"     
    "${bench_home}/files/sha/input_small.asc > ${bench_home}/files/sha/output_small.txt"           
    "${bench_home}/files/susan/input_small.pgm ${bench_home}/files/susan/output_small.edges.pgm -e"         
    ">! ${bench_home}/files/mesa/texgen"
    "-fpl ${bench_home}/files/gsm/data/clinton.pcm"        
    "${bench_home}/files/epic/data/test.image.pgm.E"       
    "-fpl ${bench_home}/files/gsm/data/clinton.pcm.run.gsm"
)

pvf_analysis="yes"

gem5_base="/home/robert/gem5"
gem5_exec_path="${gem5_base}/build/${1^^}/gem5.opt"
config="/home/robert/gem5/configs/pvf_analysis/semode.py"

if [ $1 = "x86" ]; then
    echo "Running benchmark script for x86."
    count=${#benchmarks[@]}
    #for i in "${benchmarks[@]}"
    for (( i=0; i<${count}; i++))
        do
            echo "!!! -------------------- ${benchmarks[$i]} -------------------- !!!"
            $gem5_exec_path $config -c "${bench_home_x86}/${benchmarks[$i]}" --options="${benchmark_options[$i]}" --pvf-analysis=$pvf_analysis --pvf-statsfile="${gem5_base}/${benchmarks[$i]}_pvf_x86.txt" --pvf-instinterval=10000
        done
elif [ $1 = "arm" ]; then
    echo "Running benchmark script for ARM."
    count=${#benchmarks[@]}
    #for i in "${benchmarks[@]}"
    for (( i=0; i<${count}; i++))
        do
            echo "!!! -------------------- ${benchmarks[$i]} -------------------- !!!"
            $gem5_exec_path $config -c "${bench_home_x86}/${benchmarks[$i]}" --options="${benchmark_options[$i]}" --pvf-analysis=$pvf_analysis --pvf-statsfile="${gem5_base}/${benchmarks[$i]}_pvf_arm.txt"
        done    
else 
    echo "Command must be $0 <x86|arm|>"
fi
