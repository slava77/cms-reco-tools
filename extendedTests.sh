#!/bin/bash
echo "started QCD wf 38.0 and 1338"
runTheMatrix.py -l 38.0,1338.0 --useInput all --command="-n 600 --customise Validation/Performance/TimeMemoryInfo.py --lazy_download" >& run38.0_600.log &
echo "started TTbar PU wf 202.0"
runTheMatrix.py -l 202.0 --useInput all --command="-n 200 --customise Validation/Performance/TimeMemoryInfo.py --lazy_download" >& run202.0_200.log &
echo "started TTbar PU 25ns  wf  25202.0"
runTheMatrix.py -l 25202.0 --useInput all --command=" -n 70 --customise Validation/Performance/TimeMemoryInfo.py --lazy_download" >& run25202.0_70.log &
echo "started QCD 3TeV wf 1313.0"
runTheMatrix.py -l 1313.0 --useInput all --command="-n 400 --customise Validation/Performance/TimeMemoryInfo.py --lazy_download" >& run1313.0_400.log &
if [ "x${1}" == "xALL" ]; then 
    echo "started e/mu/gamma guns: 16.0,17.0,18.0,19.0,20.0,22.0,1316.0,1317.0,1318.0,1319.0,1320.0,1322.0"
    runTheMatrix.py -l 16.0,17.0,18.0,19.0,20.0,22.0,1316.0,1317.0,1318.0,1319.0,1320.0,1322.0 --useInput all --command="-n 1000 --lazy_download" -j 12 >& runGuns.log &

fi
