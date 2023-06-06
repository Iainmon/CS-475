#!/bin/bash
#SBATCH -J MatrixMult
#SBATCH -A cs475-575
#SBATCH -p classgpufinal
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o data.out
#SBATCH -e data.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=moncrief@oregonstate.edu

# srun -A cs475-575 -p classgputest --pty bash run.bash
# srun -A cs475-575 -p classgpufinal --pty bash run.bash

./run.bash

