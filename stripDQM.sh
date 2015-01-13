f=$1

pattRemove="AAABBBCCCDDD"
#"AlcaBeamMonitor\|Btag/Run\|1/HLT/Run\|ry/DiMuonH\|mmary/Isolation\|mmary/MuonKinVsEta\|RecoMuon_MuonAssoc_Sta\|1/Pixel/Run\|1/RPC/Run\|1/SiStrip/Run\|cutsRecoThird_\|cutsRecoSecond_\|cutsRecoZero_\|cutsRecoSixth_\|cutsRecoFifth_\|cutsRecoFirst_\|cutsRecoFourth_\|cutsReco_"
[ "x${2}" != "x" ] && pattRemove="${pattRemove}\|$2"
fExt=`echo $f | tr '.' '\n' | tail -1`
fB=`echo ${f} | sed -e "s?.${fExt}??g"`
if [ "${fExt}" != "ps" ]; then
    echo Need a ps extension
    exit 1
fi
range=`grep "\%Page\|Run [0-9]\{1,6\}/" $f  | sed -e 's?[^(]*\((/DQM[^)]*)\) [^$]*?\1?g;s/$^(//g' |\
 awk '/^%%Page/{pg=$3}/^\(\/DQM/{nm=$0; print pg" "nm}' |\
 grep -v "${pattRemove}" |\
 awk 'BEGIN{lx=0;n=0}{if(n==0){f=$1;lx=f}nx=$1;n++; if(nx-lx>1){printf f"-"lx",";f=nx;} lx=nx}END{print f"-"lx}'`
if [ "x${range}" != "x" ]; then
    echo will strip ${range} from ${f}
    psselect -p${range} $f ${fB}_strip.${fExt} 2> /dev/null
fi
