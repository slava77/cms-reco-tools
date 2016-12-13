#!/bin/bash

log=$1
s=$2
#grep -m 1 "^Begin processing the ${fN}[a-z]" <(tail -f ${fLog}) >& /dev/null
grep -m 1 "starting: processing event" <(tail -f ${log}) >& /dev/null
while (true); do
    sleep $s
    dEx=`date '+%Y.%m.%d-%H.%M.%S'`
    touch touch
    echo "poked at ${dEx}"
    sleep 60
    [ -f ig.mp ] || touch ig.mp
    mv ig.mp ig.mp.${dEx}
    gzip ig.mp.${dEx}
done
