#!/bin/bash

while [ X$# != X0 ] ; do
    case ${1} in
        -rb) shift; relBase=$1; shift;;
        -te) shift; testExt=$1; teOpt="-te ${testExt}"; shift;;
        -o) shift;
	    oOpt=${1}
            case ${1} in
                eos) toEos=1; echo will write to eos; shift;;
                cmsdev02) toDev2=1; echo will write to dev02; dHost=cmsdev02; shift;;
                *) shift; echo "unknown destination (can have eos or cmsdev02)"; exit 12;;
            esac
            ;;
	-sMatrix) doSMatrix=1; shift;;
	-run2) doRun2=1; shift;;
	-phase2) doPhase2=1; shift;;
	-extended) doExtended=1; shift;;
        -dry) shift; j0="-j 0";;
        *) shift; echo unknown option ${1} ;;
    esac
done


[ "x${oOpt}" == "x" ] && echo "dest not specified: default to cmsdev02" && oOpt=cmsdev02

if [ "x${relBase}" == "x" ]; then
    maybeBase=`echo $CMSSW_BASE | tr '/' '\n' | grep CMSSW_ | tail -1`
    [ "x${maybeBase}" == "x" ] && echo "CMSSW_BASE is not known: exit " && exit 13
    relBase=${maybeBase}
    echo "relBase not specified, default to "${relBase}
fi

subSMatrix()
{
    for wf in `source ~/tools/runMatrix.sh "" showOnly | tr ',' ' ' `; do
	bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf ${wf} -iAll -o ${oOpt} ${j0}
    done
}

subRun2()
{
    for wf in 134.702 134.703 134.705 134.902 134.903 134.905; do 
	bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf ${wf} -iAll -wfCMD " -n 200 --customise Validation/Performance/TimeMemoryInfo.py " -o ${oOpt} ${j0}
    done
}

subPhase2()
{
    for wf in 11021.0 11200.0 10600.0; do 
	bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf ${wf} -wUp -wfCMD " -n 100 --customise Validation/Performance/TimeMemoryInfo.py " -o ${oOpt} ${j0}
    done
    for wf in 11024.0; do 
	bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf ${wf} -wfCMD " --customise Validation/Performance/TimeMemoryInfo.py " -o ${oOpt} ${j0}
    done
}

subExtended()
{
    for wf in 38.0 1338.0; do
	bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf ${wf} -iAll -wfCMD " -n 600 " -o ${oOpt} ${j0}
    done
    for wf in 16.0 17.0 18.0 19.0 20.0 22.0 1316.0 1317.0 1318.0 1319.0 1320.0 1322.0; do
	bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf ${wf} -iAll -wfCMD " -n 1000 " -o ${oOpt} ${j0}
    done
    bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf 1313.0 -iAll -wfCMD " -n 400 " -o ${oOpt} ${j0}
    bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf 202.0 -iAll -wfCMD " -n 200 --customise Validation/Performance/TimeMemoryInfo.py" -o ${oOpt} ${j0}
    bsub -q 8nh -R "type==SLC6_64" ~/tools/lsfOneMatrix.sh -rb ${relBase} ${teOpt} -wf 25202.0 -iAll -wfCMD " -n 70 --customise Validation/Performance/TimeMemoryInfo.py" -o ${oOpt} ${j0}
}

[ "x${doSMatrix}" != "x" ] && ( echo doSMatrix ; subSMatrix )
[ "x${doRun2}" != "x" ] && ( echo doRun2 ; subRun2 )
[ "x${doPhase2}" != "x" ] && ( echo doPhase2 ; subPhase2 )
[ "x${doExtended}" != "x" ] && ( echo doExtended ; subExtended )
