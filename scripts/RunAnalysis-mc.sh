for X in `seq 1 1 50`; do
    scriptname="run_mc-$X.sh"
    rm -f ${scriptname}
    echo "#!/bin/bash" >> ${scriptname}
    echo "cd $PWD/../Sorting" >> ${scriptname}
    echo "./main MC_$1 -mode mc -fin file:$WMC_DATA/$1-$X.wmc.data -n MC$1-$X -abort" >> ${scriptname}
    echo >> ${scriptname}
    echo "rm -f $PWD/${scriptname}" >> ${scriptname}
    chmod u+x ${scriptname}
    ./${scriptname}
done

