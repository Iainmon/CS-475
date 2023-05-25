#!/bin/bash
#SBATCH -J MonteCarlo
#SBATCH -A cs475-575
#SBATCH -p classgpufinal
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o montecarlo.out
#SBATCH -e montecarlo.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=moncrief@oregonstate.edu

# srun -A cs475-575 -p classgputest --pty bash run.bash
# srun -A cs475-575 -p classgpufinal --pty bash run.bash

./run.bash

