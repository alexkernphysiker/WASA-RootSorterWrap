for X in `seq 45873 1 46884`; do
	if [ -f ${RUNS_DATA}/run_${X} ]; then
		if [ ! -f $PWD/../Sorting/Data_run_${X}.root ]; then
			if [ `qstat|grep "R \|Q "|wc -l` -lt 60 ]; then
				sleep 2
				if [ `qstat|grep ${X} |wc -l` -lt 1 ]; then
					sleep 2
					scriptname="run_${X}.sh"
					rm -f ${scriptname}
					echo "#!/bin/bash" >> ${scriptname}
					echo "#PBS -N DATA_${X}" >> ${scriptname}
					echo "#PBS -l walltime=48:00:00" >> ${scriptname}
					echo >> ${scriptname}
					echo "cd $PWD/../Sorting" >> ${scriptname}
					echo "./main Data -fin cluster:${RUNS_DATA}/run_${X} -r ${X} -n Data_run_${X} -lf run_${X}.log -abort " >> ${scriptname}
					echo >> ${scriptname}
					echo "rm -f $PWD/${scriptname}" >> ${scriptname}
					chmod u+x ${scriptname}
					qsub ${scriptname}
					echo "${X} STARTED!!!"
					sleep 2
				else
					echo "${X} is already running"
				fi
			else
				echo "We have already enough jobs running for this reaction"
				exit 0
			fi
		else
			echo "${X} is finished already"
		fi
	else
		echo "${X} not present"
	fi
done
qstat

