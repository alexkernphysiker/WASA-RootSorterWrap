for X in `seq 1 1 50`; do
    scriptname="run_wmc-$1-$X.sh"
    rm -f ${scriptname}
    echo "#!/bin/bash" >> ${scriptname}
    echo "#PBS -N WMC_$1-$X" >> ${scriptname}
    echo "#PBS -l walltime=48:00:00" >> ${scriptname}
    echo >> ${scriptname}
    echo "cd $PWD/WMC" >> ${scriptname}
    echo "./wmc.sh $PLUTO_OUTPUT/$1-$X.root $WMC_DATA/$1-$X.wmc.data" >> ${scriptname}
    echo >> ${scriptname}
    echo "rm -f $PWD/${scriptname}" >> ${scriptname}
    chmod u+x ${scriptname}
    qsub ${scriptname}
    echo "${scriptname} generated and executed"
    sleep 5
done
qstat
