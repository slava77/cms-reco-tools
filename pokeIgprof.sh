#!/bin/bash
fLog=${1}
[ "x${fLog}" == "x" ] && echo Need at least one argument && exit 10
[ ! -f "${fLog}" ] && echo ${fLog} file does not exist && exit 10

fN=$2
[ "x${fN}" == "x" ] && echo will use 10 events && fN=10
fTouch=$3
[ "x${fTouch}" == "x" ] && echo will use ${fN}evts && fTouch=${fN}evts

fMv=${4}
[ "x${fMv}" == "x" ] && echo will use  igprof.mp.${fTouch}  && fMv=igprof.mp.${fTouch}

grep -m 1 "^Begin processing the ${fN}[a-z]" <(tail -f ${fLog}) >& /dev/null
touch ${fTouch}
sleep 10
mv igprof.mp ${fMv}
