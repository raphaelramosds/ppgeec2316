#!/bin/bash
#SBATCH --partition=intel-128
#SBATCH --job-name=pmultm_main_job
#SBATCH --output=pmultm_main_job%j.out
#SBATCH --error=pmultm_main_job%j.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=64
#SBATCH --time=0-0:15

pascalanalyzer -g -t man \
    -c 1,2,4,8,16,32,64 \
    -i 256,512,1024,2048,4096 \
    -o pmultm_main_g.json \
    ./pmultm_main.out
