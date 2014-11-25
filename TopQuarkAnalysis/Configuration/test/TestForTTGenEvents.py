import FWCore.ParameterSet.Config as cms

import FWCore.ParameterSet.VarParsing as VarParsing

import sys

import os



process = cms.Process("Analyzer")





## enabling unscheduled mode for modules

process.options = cms.untracked.PSet(

    wantSummary = cms.untracked.bool(True),

    allowUnscheduled = cms.untracked.bool(True),

)



options = VarParsing.VarParsing ('standard')

options.register('runOnPythia8', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "decide to run on Pythia8 or Pythia6")
options.register('useRun2Code', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "decide to use Run1 or Run2 code")



# Get and parse the command line arguments

if( hasattr(sys, "argv") ):

    for args in sys.argv :

        arg = args.split(',')

        for val in arg:

            val = val.split('=')

            if(len(val)==2):

                setattr(options,val[0], val[1])



if options.runOnPythia8:

    print 'Running on Pythia8'

else:

    print 'Running on Pythia6'



## configure message logger

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.MessageLogger.cerr.threshold = 'INFO'

process.MessageLogger.cerr.FwkReport.reportEvery = 100



## define input

if options.runOnPythia8:

    process.source = cms.Source("PoolSource",

       fileNames = cms.untracked.vstring(    

        'file://TopQuarkAnalysis/TTBar_pythia8.root',

       ),

       skipEvents = cms.untracked.uint32(0)

 )



else:

    process.source = cms.Source("PoolSource",

       fileNames = cms.untracked.vstring(

        'file://TopQuarkAnalysis/TTBar_pythia6.root',

       ),

       skipEvents = cms.untracked.uint32(0)

    )



## define maximal number of events to loop over

process.maxEvents = cms.untracked.PSet(

    input = cms.untracked.int32(1000)

)





####################################################################



genParticleCollection = ''

genParticleCollection = 'genParticles'





process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")

process.initSubset.src = genParticleCollection

process.decaySubset.src = genParticleCollection

process.decaySubset.fillMode = "kME" # Status3, use kStable for Status2
runMode = "Run2"
if options.useRun2Code:
    runMode = "Run2"
else:
    runMode = "Run1"
process.decaySubset.runMode = runMode

#process.load("TopAnalysis.TopFilter.filters.GeneratorTopFilter_cfi")

#process.generatorTopFilter.rejectNonBottomDecaysOfTops = False

#

#process.generatorTopFilter.invert_selection = True

#process.generatorTopFilter.channels = ["none"] # Empty array would use some defaults

process.ttGenEventSequence = cms.Sequence(process.makeGenEvt)





## module to store raw output from the processed modules into the ROOT file
output_file = 'output.root'
if options.runOnPythia8:
    output_file = 'Pythia8_' + runMode + '.root'
else:
    output_file = 'Pythia6_' + runMode + '.root'

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(output_file),
    outputCommands = cms.untracked.vstring('drop *', 'keep *_*_*_Analyzer')
)

process.outpath = cms.EndPath(process.out)
