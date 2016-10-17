import FWCore.ParameterSet.Config as cms

process = cms.Process('RECODQM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load(
    'Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EDMtoMEAtRunEnd_cff')
process.load(
    'Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# load DQM
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")

# my analyzer
process.load('DQMOffline.L1Trigger.L1TJetMetAnalyzer_Offline_cfi')

# messaging
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet(
    input=cms.untracked.int32(10000)
)

with open('fileList.local') as f:
    lines = f.readlines()

# Input source
process.source = cms.Source("PoolSource",
                            secondaryFileNames=cms.untracked.vstring(),
                            fileNames=cms.untracked.vstring(lines)
                            )


process.DQMoutput = cms.OutputModule("DQMRootOutputModule",
                                     fileName=cms.untracked.string("OUT_step1.root"))

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')  # for MC


# Path and EndPath definitions
process.dqmoffline_step = cms.Path(process.l1tJetMetAnalyzer)
process.dqmsave_step = cms.Path(process.DQMSaver)
process.dqmSaver.workflow = '/L1T/JetMet/DQM'
process.DQMoutput_step = cms.EndPath(process.DQMoutput)


# Schedule definition
process.schedule = cms.Schedule(
    process.dqmoffline_step,
    process.DQMoutput_step,
    process.dqmsave_step
)
