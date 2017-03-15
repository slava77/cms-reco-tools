#!/bin/bash
echo "started QCD wf 10071.0"
runTheMatrix.py -l 10071.0 -w upgrade --useInput all --command="-n 600 --customise Validation/Performance/TimeMemoryInfo.py" >& run10071.0_600.log &
echo "started TTbar PU 25ns  wf 10224.0"
runTheMatrix.py -l 10224.0 -w upgrade --useInput all --command=" -n 100 --customise Validation/Performance/TimeMemoryInfo.py" >& run10224.0_100.log &
echo "started QCD 3TeV wf 10059.0"
runTheMatrix.py -l 10059.0 -w upgrade --useInput all --command="-n 400 --customise Validation/Performance/TimeMemoryInfo.py" >& run10059.0_400.log &
if [ "x${1}" == "xALL" ]; then 
    echo "started 2 e/2 gamma/2 mu guns: 10002.0,10003.0,10004.0,10005.0,10007.0,10009.0"
    runTheMatrix.py -l 10002.0,10003.0,10004.0,10005.0,10007.0,10009.0 -w upgrade --useInput all --command="-n 1000" -j 12 >& runGuns.log &
fi
