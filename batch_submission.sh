#!/bin/bash
#SBATCH -A Scalable-Supercomput
#SBATCH -J jobname      			# job name
#SBATCH -o jobname.%j   			# name of the output and error file
#SBATCH -N 1                			# total number of nodes requested
#SBATCH -n 20                			# total number of tasks requested
#SBATCH -p normal             		# queue name normal or development
#SBATCH -t 00:00:05         			# expected maximum runtime (hh:mm:ss)

date

export OMP_NUM_THREADS=16
export OMP_PROC_BIND=[core|socket]
export OMP_PROC_BIND=[close|spread]

ibrun ./asgn6 1000000 100

date
