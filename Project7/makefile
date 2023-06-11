

proj07: proj07.cpp
	mpic++ -o proj07 proj07.cpp

clean:
	rm proj07

run:
	mpiexec -np 16 ./proj07
	# mpiexec -n 4 ./proj07
	# mpiexec -mca btl self,tcp -np 4 program
	# sbatch submit.bash

mpitest:
	mpic++ mpitest.cpp -o mpitest -lm