#!/bin/bash
baseA=$1
baseB=$2
diffN=$3
inList=$4
#noMiniAOD=$5

function waitForProcesses {
    pidList=${pidList}" "${!}
    export pidList
    echo $pidList
    nRunning=`ps -p $pidList | grep -c "^[ ]*[1-9]"`
    while ((nRunning > 10 )); do
        nRunning=`ps -p $pidList | grep -c "^[ ]*[1-9]"`
        #echo $nRunning "still above 5 -> sleep 10 "
        sleep 10
    done
    cd ${cWD}
    echo $pidList > ${llistF}
}


cWD=`pwd`
export pidList=""
llistF=lastlist_${diffN}.txt
echo Start processing at `date`
grep root ${inList} | grep -v "#" | while read -r dsN fN procN comm; do 
    #[ ! -f "${baseA}/${fN}" ] && echo Missing ${baseA}/${fN} && continue
    if [ -f "${baseA}/${fN}" ]; then
      extN=all_${diffN}_${dsN}
      mkdir -p ${extN}
      cd ${cWD}/${extN}
      cp ~/tools/validate.C ./
      echo "Will run on ${fN} in ${cWD}/${extN}"
      echo "Now in `pwd`"
      #g++ -shared -o validate.so validate.C `root-config --cflags ` -fPIC
      echo -e "gSystem->Load(\"libFWCoreFWLite.so\");\n FWLiteEnabler::enable();\n 
      .x validate.C+(\"${extN}\", \"${baseA}/${fN}\", \"${baseB}/${fN}\", \"${procN}\");\n .qqqqqq" | root -l -b >& ${extN}.log &
      waitForProcesses
    fi
    IFS='.' read -r -a splitFile <<< "$fN"
    mFN="${splitFile[0]}_inMINIAOD.root"
    if [ ! -f "${baseA}/${mFN}" ]; then
        mFN="${splitFile[0]}_inMINIAODSIM.root"
    fi
    #if [ -f "${baseA}/${mFN}" ] && [ ! $noMiniAOD ]; then 
    if [ -f "${baseA}/${mFN}" ]; then
        echo $mFN
        extmN=all_mini_${diffN}_${dsN}
        mkdir -p ${extmN}
        cd ${cWD}/${extmN}
        cp ~/tools/validate.C ./
        echo "Will run on ${mFN} in ${cWD}/${extmN}"
        echo "Now in `pwd`"
        echo -e "gSystem->Load(\"libFWCoreFWLite.so\");\n FWLiteEnabler::enable();\n 
        .x validate.C+(\"${extmN}\", \"${baseA}/${mFN}\", \"${baseB}/${mFN}\", \"${mProcN}\");\n .qqqqqq" | root -l -b >& ${extmN}.log &
        waitForProcesses
    fi
    #manually set the make flags, not needed in most cases
    #gSystem->SetMakeSharedLib(\"cd \$BuildDir ;g++  -c \$Opt -pipe -m64 -Wshadow -Wall -W -Woverloaded-virtual -fPIC -std=c++11 -Wno-deprecated-declarations -DG__MAXSTRUCT=36000 -DG__MAXTYPEDEF=36000 -DG__LONGLINE=4096 -pthread \$IncludePath \$SourceFiles ; g++ \$ObjectFiles -shared -Wl,-soname,\$LibName.so -m64 -Wl,--hash-style=gnu -O2  \$LinkedLibs -o \$SharedLib\");\n 
    #cout<< gSystem->GetMakeSharedLib()<<endl ;\n

done
allPids=`cat ${llistF}`
nRunning=1
while (( nRunning > 0 )); do
    nRunning=`ps -p $allPids | grep -c "^[ ]*[1-9]"`
    #echo $nRunning
    sleep 10
done
echo done at `date`
