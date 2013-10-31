# Auto generated configuration file
# using: 
# Revision: 1.138 
# Source: /cvs/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,L1Reco,RECO:reconstructionCosmics,ALCA:MuAlCalIsolatedMu+RpcCalHLT --relval 25000,100 --datatier RECO --eventcontent RECO --conditions FrontierConditions_GlobalTag,CRAFT0831X_V1::All --scenario cosmics --no_exec --data
import FWCore.ParameterSet.Config as cms

process = cms.Process('PICK')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')

import sys
import os
import string

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(string.atoi(os.environ['N_EVENTS']))
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(os.environ['INPUT_FILE']),
        skipEvents = cms.untracked.uint32(string.atoi(os.environ['SKIP_EVENTS'])),
                            eventsToProcess = cms.untracked.VEventRange(os.environ['EVENTLIST']) 
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(os.environ['OUTPUT_FILE'])
)

process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.out_step)
