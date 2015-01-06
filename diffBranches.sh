#!/bin/bash
or=$1
nw=$2
pPat="RECO"
[ "x$3" != "x" ] && pPat=$3
grep _"[.]*${pPat}" $or | while read -r ob ou uc; do grep -w $ob $nw >& /dev/null || echo gone: $ob $uc; done
grep _"[.]*${pPat}" $nw | while read -r ob ou uc; do grep -w $ob $or >& /dev/null || echo  new: $ob $uc; done

