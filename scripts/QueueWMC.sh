for X in `seq 1 1 100`; do
    if [ -e $PLUTO_OUTPUT/$1-$X.root ]; then
	echo "number $X"
	if [ -e $WMC_DATA/$1-$X.wmc.data ];then
	    echo "was already done"
	else
	    scriptname="$PWD/run_wmc-$1-$X.sh"
	    dirname="$PWD/wmc-$1-$X"
	    mkdir ${dirname}
	    cp $PWD/WMC/* ${dirname}
	    rm -f ${scriptname}
	    echo "#!/bin/bash" >> ${scriptname}
	    echo "#PBS -N WMC_$1-$X" >> ${scriptname}
	    echo "#PBS -l walltime=48:00:00" >> ${scriptname}
	    echo >> ${scriptname}
	    echo "cd ${dirname}" >> ${scriptname}
	    echo "./wmc.sh $PLUTO_OUTPUT/$1-$X.root $WMC_DATA/$1-$X.wmc.data > /dev/null" >> ${scriptname}
	    echo >> ${scriptname}
	    echo "rm -f ${scriptname}" >> ${scriptname}
	    echo "rm -rf ${dirname}" >> ${scriptname}
	    chmod u+x ${scriptname}
	    qsub ${scriptname}
	    echo "${scriptname} generated and executed"
	fi
    fi
done
qstat
