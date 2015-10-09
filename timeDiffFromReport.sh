#!/bin/bash

ol=$1

sl=$2

if [ ! -f "${ol}" ]; then
    echo "Couldn't file input log file ${ol}"
    exit 1
fi

if [ ! -f "${sl}" ]; then
    echo "Couldn't file input log file ${sl}"
    exit 1
fi

nSkip=$3
if [ "x${nSkip}" == "x" ]; then
    nSkip=1
fi
export nSkip

tRelDiff=$4
if [ "x${tRelDiff}" == "x" ]; then
    tRelDiff=0.05
fi
export tRelDiff

tDiff=$5
if [ "x${tDiff}" == "x" ]; then
    tDiff=0.005
fi
export tDiff


ot=${ol}.times
st=${sl}.times

grep -P "^TimeReport( [ ]*[0-9.]{1,}){6}[ ]*[^ ]*$" ${ol} |  awk '{print $8" "$2}' >  ${ot}
grep -P "^TimeReport( [ ]*[0-9.]{1,}){6}[ ]*[^ ]*$" ${sl} |  awk '{print $8" "$2}' >  ${st}

#based on per-event timing
otm=${ol}.timesm
stm=${sl}.timesm
grep "^TimeModule[^$]*"  ${ol} | awk 'BEGIN{nSkip=ENVIRON["nSkip"]}{if(cn[$4]>nSkip){sum[$4]+=$6;} cn[$4]++;}END{for (m in sum){print m" "sum[m]/(cn[m]-nSkip)" "cn[m]-nSkip}}' > ${otm}
grep "^TimeModule[^$]*"  ${sl} | awk 'BEGIN{nSkip=ENVIRON["nSkip"]}{if(cn[$4]>nSkip){sum[$4]+=$6;} cn[$4]++;}END{for (m in sum){print m" "sum[m]/(cn[m]-nSkip)" "cn[m]-nSkip}}' > ${stm}

no=`grep [a-z] ${ot} | wc -l`
ns=`grep [a-z] ${st} | wc -l`

nom=`grep [a-z] ${otm} | wc -l`
nsm=`grep [a-z] ${stm} | wc -l`

if [ "${no}" == "0" -o "${ns}" == "0" ]; then
    echo "Couldn't parse time report using CPU and wall-clock format: trying Wall-clock only"

    grep -P "^TimeReport( [ ]*[0-9.]{1,}){3}[ ]*[^ ]*$" ${ol} |  awk '{print $5" "$2}' >  ${ot}
    grep -P "^TimeReport( [ ]*[0-9.]{1,}){3}[ ]*[^ ]*$" ${sl} |  awk '{print $5" "$2}' >  ${st}
    no=`grep [a-z] ${ot} | wc -l`
    ns=`grep [a-z] ${st} | wc -l`
    if [ "${no}" == "0" -o "${ns}" == "0" ]; then
	echo "Couldn't parse time report"
	exit 1
    fi
fi

grep [a-zA-Z] ${ot} ${st} | tr ':' ' ' | sed -e "s?^${st}?st?g;s?^${ot}?ot?g"| awk -f ~/tools/timeDiffFromReport.awk 

if [ "${nom}" != "0" -a  "${nsm}" != "0" ]; then 
    echo 
    echo "The same excluding the first ${nSkip} events"
    grep [a-zA-Z] ${otm} ${stm} | tr ':' ' ' | sed -e "s?^${stm}?st?g;s?^${otm}?ot?g"| awk -f ~/tools/timeDiffFromReport.awk 
fi
