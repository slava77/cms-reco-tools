#!/bin/bash

##########################################
#
# example submission 
#
# bsub -q 8nh -R "type==SLC6_64" ~/lsfOneMatrix.sh -rb CMSSW_ABCDE -te pass-1 -wf 10024.0  -wfCMD " --customise AAOO.py" -o cmsdev02
#
##########################################

shopt -s expand_aliases

iAll=""
j0=""
wUp=""

toEos=0
toDev2=0

while [ X$# != X0 ] ; do
    case ${1} in
	-rb) shift; relBase=$1; shift;;
	-te) shift; testExt=$1; shift;;
	-wf)  shift; wf=$1; shift;;
	-iAll) shift; iAll="-i all";;
	-wUp) shift; wUp="-w upgrade";;
	-wfCMD) shift; wfCMD="${1}"; shift;;
	-o) shift; 
	    case ${1} in
		eos) toEos=1; echo will write to eos; shift;;
		cmsdev02) toDev2=1; echo will write to dev02; dHost=cmsdev02; shift;;
		*) shift; echo "unknown destination (can have eos or cmsdev02)";;
	    esac
	    ;;
	-dry) shift; j0="-j 0";;
	*) shift; echo unknown option ${1} ;;
    esac
done

echo relBase ${relBase}
echo testExt ${testExt}
echo wf ${wf}
echo iAll ${iAll}
echo wUp ${wUp}
echo wfCMD "${wfCMD}"
echo j0 ${j0}

##########################################
# edit these to specialize
##########################################
# release base directory, the $CMSSW_BASE will be ${relDir}/${relBase}
relDir=/afs/cern.ch/work/U/USER/reltest
# eos output directory base: actual output will go to ${eosBase}/${relBase}/[${testExt}/] if eos is chosen
eosBase=/eos/cms/store/user/USER/reltest
# local node directory; cmsdev02 is currently hardcoded as the only option other than eos; edit dHost above to change
dev2base=/build/USER/reltest/lsf
##########################################

wDir=`pwd`
echo "Started from ${wDir} on "`uname -a`" at "`date`
cat /proc/cpuinfo  | tail -30
relDir=${relDir}/${relBase}/src

[ -d "${relDir}" ] || (echo relDir ${relDir} is missing; exit 15 )
cd ${relDir}
eval `scramv1 ru -sh` || (echo failed to cmsenv in ${relDir}; exit 16 )

echo back to ${wDir}
cd ${wDir}

if [ "x${toEos}" == "x1" ]; then
    source /afs/cern.ch/project/eos/installation/cms/etc/setup.sh
    
    testOut=${eosBase}/${relBase}

    eos ls ${testOut} >& /dev/null || ( echo make ${testOut}; eos mkdir ${testOut} )
    eos ls ${testOut} >& /dev/null || ( echo failed to get ${testOut}; exit 17 )
    
    if [ "x${testExt}" != "x" ]; then
	testOut=${testOut}/${testExt}
	eos ls ${testOut} >& /dev/null || ( echo make ${testOut}; eos mkdir ${testOut} )
    fi
fi

if [ "x${toDev2}" == "x1" ]; then
    testOut=${dev2base}/${relBase}
    ssh ${dHost} " [ -d ${testOut} ] || mkdir -p ${testOut} "
    if [ "x${testExt}" != "x" ]; then
	testOut=${testOut}/${testExt}
	ssh ${dHost} " [ -d ${testOut} ] || mkdir -p ${testOut} "
    fi
fi

echo start matrix wf ${wf}
if [ "x${wfCMD}" == "x" ]; then
    runTheMatrix.py -l ${wf} ${iAll} ${wUp} ${j0} >& run_${wf}.log
else
    runTheMatrix.py -l ${wf} ${iAll} ${wUp} ${j0} --command="${wfCMD}" >& run_${wf}.log
fi

echo done with runTheMatrix.py at `date`
ls -ltrh 
ls -ltrh ${wf}_*/

if [ "x${toEos}" == "x1" ]; then
    eos cp run_${wf}.log ${testOut}/run_${wf}.log
fi
if [ "x${toDev2}" == "x1" ]; then
    rsync -avz run_${wf}.log ${dHost}:${testOut}/run_${wf}.log
fi

oDir=`echo ${wf}_*`
if [ -d "${oDir}" ]; then
    lOdir=${PWD}/${oDir}
    if [ "x${toEos}" == "x1" ]; then
	eOdir=${testOut}/${oDir}
	echo "done running: output is in ${oDir} of size " `du -cksh ${oDir}`" to be copied to ${eOdir}"
	eos ls ${eOdir} >& /dev/null || ( echo make ${eOdir}; eos mkdir ${eOdir} )
	eos cp -r ${lOdir}/ ${eOdir}/
    fi
    if [ "x${toDev2}" == "x1" ]; then
	rsync -avz ${lOdir}/ ${dHost}:${testOut}/${oDir}/
    fi
fi

echo all done at `date`

exit 0
