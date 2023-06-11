#!/bin/bash

# srun -A cs475-575 -p classmpitest --pty bash runtest.bash

module load openmpi
mpiexec -np 16 ./mpitest
echo "--------------------- Trying Again ---------------------"
mpiexec -mca btl self,tcp -np 16 ./mpitest
