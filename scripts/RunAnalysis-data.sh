for X in `seq 45873 1 46884`; do
	echo "run #${X}..."
	if [ -e ${RUNS_DATA}/run_${X} ]; then
		echo "...present"
		if [ -e $PWD/../Sorting/Data_run_${X}.root ]; then
			echo "...has been analyzed"
		else

			STAT=`qstat`
			sleep 2

			if [ `echo ${STAT}|grep "R \|Q "|wc -l` -lt 60 ]; then
				if [ `echo ${STAT}|grep ${X} |wc -l` -lt 1 ]; then
					echo "task is not running"
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
					sleep 2

					echo "... analysis STARTED!!!"
				else
					echo "... analysis is already running"
				fi
			else
				echo "We have already enough jobs running for this reaction"
				exit 0
			fi
		fi
	else
		echo "...not present"
	fi
done
echo "FINISHED"

