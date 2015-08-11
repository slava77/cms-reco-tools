#!/bin/bash
fLog=${1}
[ "x${fLog}" == "x" ] && echo Need at least one argument && exit 10
[ ! -f "${fLog}" ] && echo ${fLog} file does not exist && exit 10

fN=$2
[ "x${fN}" == "x" ] && echo will use 10 events && fN=10
fTouch=$3
[ "x${fTouch}" == "x" ] && echo will use ${fN}evts && fTouch=${fN}evts

grep -m 1 "^Begin processing ${fN}[a-z]" <(tail -f ${fLog}) >& /dev/null
touch ${fTouch}
sleep 10
mv igprof.mp igprof.mp.${fTouch}
