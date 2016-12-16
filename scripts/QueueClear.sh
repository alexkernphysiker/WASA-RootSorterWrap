#!/bin/bash
for X in `qstat | grep "R \|Q " |cut -c1-7`; do echo $X; qdel $X; sleep 2; done
rm run*.sh
rm *.o* *.e*
rm ../Sorting/*.log ../Sorting/*.root

