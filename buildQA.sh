#!/bin/bash

BUILD_LOG=abuild.log
echo "grep for errors: in ${BUILD_LOG}"
grep -iw "warning\|error" ${BUILD_LOG} | grep -v "Warning in <TUnixSystem::SetDisplay>: DISPLAY not set" \
  ||    echo "==> No errors/warnings"

echo "check dup dicts"
duplicateReflexLibrarySearch.py --dir ./ --dup
echo "check wrong dict locations"
duplicateReflexLibrarySearch.py --dir ./ --lostDefs
echo "check dup plugins"
duplicateReflexLibrarySearch.py  --edmFile $CMSSW_BASE/lib/$SCRAM_ARCH/.edmplugincache --edmPD
echo done `date` 
