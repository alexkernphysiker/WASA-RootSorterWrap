for X in `seq 45873 1 46884`; do
	echo "run #${X}..."
	if [ -e ${RUNS_DATA}/run_${X} ]; then
		echo "...present"
		if [ -e $PWD/../Sorting/Data_run_${X}.root ]; then
			echo "...has been analyzed"
		else
			echo "...starting..."
			scriptname="run_${X}.sh"
			rm -f ${scriptname}
			echo "#!/bin/bash" >> ${scriptname}
			echo "cd $PWD/../Sorting" >> ${scriptname}
			echo "./main Data -local -fin cluster:${RUNS_DATA}/run_${X} -r ${X} -n Data_run_${X} -lf run_${X}.log -abort " >> ${scriptname}
			echo >> ${scriptname}
			echo "rm -f $PWD/${scriptname}" >> ${scriptname}
			chmod u+x ${scriptname}
			./${scriptname}
		fi
	else
		echo "...not present"
	fi
done
echo "FINISHED"

