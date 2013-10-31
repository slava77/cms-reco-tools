relB=${1}
ext=${2}
relD=${relB}-${ext}
dataDirB=/tas06/disk00/slava77/reltest/
[ "x${3}" != "x" ] && dataDirB=${3}
[ ! -d "${dataDirB}" ] && echo "Directory ${dataDirB} does not exist, default to /tas06 " && dataDirB=/tas06/disk00/slava77/reltest/
dataDir=${dataDirB}/${relD}
mkdir ${dataDir} ${dataDir}_tmp
scramv1 p -n ${relD} CMSSW ${relB}
rm -rf ${relD}/tmp
ln -s ${dataDir}_tmp ${relD}/tmp
# "make.sh is not copied anymore. Time  to update?"
#cp ~slava77/tools/make.sh ${relD}/src
