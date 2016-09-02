mod=$1
sList=`runTheMatrix.py -s -n | grep ^[1-9] | grep -v workflows | cut -d" " -f1 | tr  '\n' ','|sed -e 's/,$//'`
sList=`echo 101.0, 4.22, 4.37, 4.38, 4.39, 4.17, 4.29, 4.53, 4.77, ${sList}, 200.0, 201.0, 205.1, 1102.0, 1103.0, 40.51, 140.51, 140.53, 50101.0, 50202.0, 11325.0, 10021.0, 10024.0`
if [ "x${2}" != "x" -a "x${2}" == "xshowOnly" ]; then
    echo ${sList}
else 
    runTheMatrix.py -j 7 -l "${sList}" --useInput all "${mod}"
fi
