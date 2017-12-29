#!/bin/bash
echo "started QCD wf 10871.0"
runTheMatrix.py -l 10871.0 -w upgrade --useInput all --command="-n 600 --customise Validation/Performance/TimeMemoryInfo.py --lazy_download" >& run10871.0_600.log &
echo "started TTbar PU 25ns  wf 11024.0"
runTheMatrix.py -l 11024.0 -w upgrade --useInput all --command=" -n 100 --customise Validation/Performance/TimeMemoryInfo.py --lazy_download" >& run11024.0_100.log &
echo "started QCD 3TeV wf 10859.0"
runTheMatrix.py -l 10859.0 -w upgrade --useInput all --command="-n 400 --customise Validation/Performance/TimeMemoryInfo.py --lazy_download" >& run10859.0_400.log &
if [ "x${1}" == "xALL" ]; then 
    echo "started 2 e/2 gamma/2 mu guns: 10802.0,10803.0,10804.0,10805.0,10807.0,10809.0"
    runTheMatrix.py -l 10802.0,10803.0,10804.0,10805.0,10807.0,10809.0 -w upgrade --useInput all --command="-n 1000 --lazy_download" -j 12 >& runGuns2018.log &
fi
