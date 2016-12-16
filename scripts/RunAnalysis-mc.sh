for X in `seq 1 1 20`; do
    scriptname="run_mc-$X.sh"
    rm -f ${scriptname}
    echo "#!/bin/bash" >> ${scriptname}
    echo "#PBS -N MC_$1-$X" >> ${scriptname}
    echo "#PBS -l walltime=48:00:00" >> ${scriptname}
    echo >> ${scriptname}
    echo "cd $PWD/../Sorting" >> ${scriptname}
    echo "./main MC_$1-$X -mode mc -fin file:$WMC_DATA/$1-$X.wmc.data -n MC$1-$X -abort" >> ${scriptname}
    echo >> ${scriptname}
    echo "rm -f $PWD/${scriptname}" >> ${scriptname}
    chmod u+x ${scriptname}
    qsub ${scriptname}
    echo "${scriptname} generated and executed"
done
qstat
