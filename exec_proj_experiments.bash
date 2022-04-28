#!/bin/bash

#SBATCH --job-name=exec_final_proj
#SBATCH --output=experiment.%j.log
#SBATCH --partition=reservation
#SBATCH --reservation=EECE5640
#SBATCH --mem=8Gb
#SBATCH --time=03:00:00
#SBATCH --nodes=1
#SBATCH --ntasks=28

REPEAT=20

module load cmake/3.18.1
module load gcc/11.1.0
module load go/1.17.4

echo "---SETTING UP FOLDERS---"
PROJ_DIR=`pwd`
RUN_DIR=${PROJ_DIR}/experiment_run_${SLURM_JOB_ID}
mkdir $RUN_DIR
cd $RUN_DIR

GO_DIR=${RUN_DIR}/build-go
CPP_DIR=${RUN_DIR}/build-cpp
C_DIR=${RUN_DIR}/build-c
RUST_DIR=${RUN_DIR}/build-rust
mkdir $GO_DIR
mkdir $CPP_DIR
mkdir $C_DIR
mkdir $RUST_DIR

echo "---BUILDING EXECUTABLES---"
cd ${PROJ_DIR}/fp-go
go build -o ${GO_DIR}/matmul tools/matmul/main.go &
go build -o ${GO_DIR}/fft tools/fft/main.go &
go build -o ${RUN_DIR}/genmat tools/genmat/main.go &
go build -o ${RUN_DIR}/genvec tools/genvec/main.go &

cd $CPP_DIR
{ cmake -DCMAKE_BUILD_TYPE=Release -S${PROJ_DIR}/fp-cpp; cmake --build . -- -j 4; } &

cd $C_DIR
{ cmake -DCMAKE_BUILD_TYPE=Release -S${PROJ_DIR}/fp-c; cmake --build . -- -j 4; } &

cd ${PROJ_DIR}/fp-rust
{ cargo build --release --target-dir $RUST_DIR && cd $RUST_DIR/release && cp matmul .. && cp fft ..; } &

wait

echo "---GENERATING MATRICES---"
cd $RUN_DIR
mat_Ms=(512 2000 4096 800)
mat_Ks=(512 2000 4096 900)
mat_Ns=(512 2000 4096 700)
mat_dirs=()

for i in ${!mat_Ms[@]}; do
  mat_dirs+=( mat_${mat_Ms[i]}x${mat_Ks[i]}x${mat_Ns[i]} )
  mkdir ${mat_dirs[i]}
  ./genmat ${mat_Ms[i]} ${mat_Ks[i]} ${mat_dirs[i]}/A &
  ./genmat ${mat_Ks[i]} ${mat_Ns[i]} ${mat_dirs[i]}/B &
done

echo "---GENERATING VECTORS---"
cd $RUN_DIR
vec_Ls=($((1<<16)) $((1<<20)) $((1<<25)) $((1<<28)))
vec_dirs=()

for i in ${!vec_Ls[@]}; do
  vec_dirs+=( vec_${vec_Ls[i]} )
  mkdir ${vec_dirs[i]}
  ./genvec ${vec_Ls[i]} ${vec_dirs[i]}/X &
done

wait

langs=(go cpp c rust)
res_file=${RUN_DIR}/results.log

echo "---RUNNING MATMUL---"
echo "---RUNNING MATMUL---" >> ${res_file}

for l in ${langs[@]}; do
  echo " ~~timing ${l}~~"
  echo " ~~timing ${l}~~" >> ${res_file}
  for d in ${mat_dirs[@]}; do
    echo "  ~timing ${d}~" >> ${res_file}
    fn=${RUN_DIR}/time.${l}.${d}
    cd ${RUN_DIR}/${d}
    for n in {1..${REPEAT}}; do
      ${RUN_DIR}/build-${l}/matmul >> ${fn}
    done
    imp_avg=`grep "Import"   ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    mul_avg=`grep "Multiply" ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    exp_avg=`grep "Export"   ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    tot_avg=`grep "Total"    ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    echo "    Import   Average: ${imp_avg} us" >> ${res_file}
    echo "    Multiply Average: ${mul_avg} us" >> ${res_file}
    echo "    Export   Average: ${exp_avg} us" >> ${res_file}
    echo "    Total    Average: ${tot_avg} us" >> ${res_file}
  done
done

echo "---RUNNING FFT---"
echo "---RUNNING FFT---" >> ${res_file}

for l in ${langs[@]}; do
  echo " ~~timing ${l}~~"
  echo " ~~timing ${l}~~" >> ${res_file}
  for d in ${vec_dirs[@]}; do
    echo "  ~timing ${d}~" >> ${res_file}
    fn=${RUN_DIR}/time.${l}.${d}
    cd ${RUN_DIR}/${d}
    for n in {1..${REPEAT}}; do
      ${RUN_DIR}/build-${l}/fft >> ${fn}
    done
    imp_avg=`grep "Import" ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    fft_avg=`grep "FFT"    ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    exp_avg=`grep "Export" ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    tot_avg=`grep "Total"  ${fn} | awk '{ sum += $2; n++ } END { if (n > 0) print sum/n }'`
    echo "    Import   Average: ${imp_avg} us" >> ${res_file}
    echo "    FFT      Average: ${fft_avg} us" >> ${res_file}
    echo "    Export   Average: ${exp_avg} us" >> ${res_file}
    echo "    Total    Average: ${tot_avg} us" >> ${res_file}
  done
done

echo "---COMPLETE---"
