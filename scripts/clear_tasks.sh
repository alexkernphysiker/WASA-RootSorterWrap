#!/bin/bash
for X in `qstat | grep DATA_|cut -c1-7`; do echo $X; qdel $X; sleep 2; done

