#!/bin/bash
#SBATCH --partition=intel-128
#SBATCH --job-name=PaScal_job
#SBATCH --output=PaScal_job%j.out
#SBATCH --error=PaScal_job%j.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=32
#SBATCH --time=0-0:10

pascalanalyzer -g -t man \
    -c 1,2,4,8,16 \
    -i 256,512,1024,2048,4096 \
    -o multm_main_g.json \
    ./multm_main.out
