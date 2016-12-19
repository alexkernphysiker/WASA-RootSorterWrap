for X in `seq 1 1 100`; do
    if [ -e $WMC_DATA/$1-$X.wmc.data ]; then
	echo "number $X"
	if [ -e $PWD/../Sorting/MC$1-$X.root ];then
	    echo "was already done"
	else
	    scriptname="run_mc-$X.sh"
	    rm -f ${scriptname}
	    echo "#!/bin/bash" >> ${scriptname}
	    echo "#PBS -N MC_$1-$X" >> ${scriptname}
	    echo "#PBS -l walltime=48:00:00" >> ${scriptname}
	    echo >> ${scriptname}
	    echo "cd $PWD/../Sorting" >> ${scriptname}
	    echo "./main MC_$1 -mode mc -fin file:$WMC_DATA/$1-$X.wmc.data -n MC$1-$X -abort" >> ${scriptname}
	    echo >> ${scriptname}
	    echo "rm -f $PWD/${scriptname}" >> ${scriptname}
	    chmod u+x ${scriptname}
	    qsub ${scriptname}
	    echo "${scriptname} generated and executed"
	fi
    fi
done
qstat
