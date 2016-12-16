for X in `seq 1 1 20`; do
    scriptname="run_wmc-$1-$X.sh"
    rm -f ${scriptname}
    echo "#!/bin/bash" >> ${scriptname}
    echo "cd $PWD/WMC" >> ${scriptname}
    echo "./wmc.sh $PLUTO_OUTPUT/$1-$X.root $WMC_DATA/$1-$X.wmc.data" >> ${scriptname}
    echo >> ${scriptname}
    echo "rm -f $PWD/${scriptname}" >> ${scriptname}
    chmod u+x ${scriptname}
    ./${scriptname}
done
