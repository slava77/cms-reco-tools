echo "started QCD wf 38.0"
runTheMatrix.py -l 38.0 --useInput all --command="-n 1000" >& run38.0_1k.log &
echo "started TTbar PU wf 202.0"
runTheMatrix.py -l 202.0 --useInput all --command="-n 200 --customise Validation/Performance/TimeMemoryInfo.py" >& run202.0_200.log &
echo "started TTbar PU 25ns  wf  25202.0"
runTheMatrix.py -l 25202.0 --useInput all --command=" -n 100 --customise Validation/Performance/TimeMemoryInfo.py" >& run25202.0_100.log &
echo "started Mu10 wf 20.0"
runTheMatrix.py -l 20.0 --useInput all --command="-n 2000" >& run20.0_2k.log &
echo "started Ele35 wf 17.0"
runTheMatrix.py -l 17.0 --useInput all --command="-n 2000" >& run17.0_2k.log &
echo "started Gamma35 wf 19.0"
runTheMatrix.py -l 19.0 --useInput all --command="-n 2000" >& run19.0_2k.log &
echo "started QCD 3TeV wf 1313.0"
runTheMatrix.py -l 1313.0 --useInput all --command="-n 500" >& run1313.0_500.log &
if [ "x${1}" == "xALL" ]; then 
    echo "started Gamma10 wf 18.0"
    runTheMatrix.py -l 18.0 --useInput all --command="-n 2000" >& run18.0_2k.log &
    echo "started Mu1000 wf 22.0"
    runTheMatrix.py -l 22.0 --useInput all --command="-n 2000" >& run22.0_2k.log &
    echo "started Ele1000 wf 16.0"
    runTheMatrix.py -l 16.0 --useInput all --command="-n 2000" >& run16.0_2k.log &
fi