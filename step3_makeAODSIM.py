# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:run2_mc -n -1 --eventcontent RECOSIM,AODSIM -s RAW2DIGI,L1Reco,RECO,EI --datatier GEN-SIM-RECO,AODSIM --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --customise Validation/Performance/TimeMemoryInfo.py --magField 38T_PostLS1 --filein file:/tas/slava77/reltest/performance/CMSSW_7_2_0_pre5-orig-gcc481/AVE_20_BX_25ns/step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('mkAODSIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:step3.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.19 $'),
    annotation = cms.untracked.string('step3 nevts:-1'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = process.AODSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:step3_inAODSIM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('AODSIM')
    )
)

# Additional output definition

# Path and EndPath definitions
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.AODSIMoutput_step)


