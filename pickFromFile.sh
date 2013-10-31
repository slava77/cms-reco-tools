pf=${1}
ds=${2}
oPfx=${3}
[ "x${oPfx}" == "x" ] && oPfx=events
[ ! -f "${pf}" ] && echo No input file found && exit 0
cat ${pf} | grep ^[1-9] | while read -r rn lm ev; do 
    fl=`dbsql find file where dataset=${ds} and run=${rn} and lumi=${lm} | grep store`
    export INPUT_FILE=$fl
    export OUTPUT_FILE=${oPfx}.${rn}.${lm}.${ev}.root
    export EVENTLIST="${rn}:${ev}"
    export N_EVENTS=-1
    export SKIP_EVENTS=0
    cmsRun ~/tools/pickFromFile.py >& ${oPfx}.${rn}.${lm}.${ev}.log
done

