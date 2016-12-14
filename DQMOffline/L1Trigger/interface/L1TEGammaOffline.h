#ifndef L1TEGammaOffline_H
#define L1TEGammaOffline_H

//Framework
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//event
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

//DQM
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//Candidate handling
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

// Electron
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

// PFMET
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

// Vertex utilities
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// CaloJets
#include "DataFormats/JetReco/interface/CaloJet.h"

// Calo MET
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

// Conversions
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"

// stage2 collections:
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"

class L1TEGammaOffline: public DQMEDAnalyzer {

public:

  L1TEGammaOffline(const edm::ParameterSet& ps);
  virtual ~L1TEGammaOffline();

protected:

  void dqmBeginRun(edm::Run const &, edm::EventSetup const &) override;
  void bookHistograms(DQMStore::IBooker &, edm::Run const &, edm::EventSetup const &) override;
  void analyze(edm::Event const& e, edm::EventSetup const& eSetup);
  void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
  void endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& eSetup);
  void endRun(edm::Run const& run, edm::EventSetup const& eSetup);

private:
  bool passesSelection(edm::Handle<reco::GsfElectronCollection> const& electrons) const;
  bool passesLooseEleId(reco::GsfElectron const& electron) const;
  bool passesMediumEleId(reco::GsfElectron const& electron) const;
  //histos booking function
  void bookElectronHistos(DQMStore::IBooker &);
  void bookPhotonHistos(DQMStore::IBooker &);

  //other functions
  double Distance(const reco::Candidate & c1, const reco::Candidate & c2);
  double DistancePhi(const reco::Candidate & c1, const reco::Candidate & c2);
  double calcDeltaPhi(double phi1, double phi2);

  void fillElectrons(edm::Event const& e, const unsigned int nVertex);
  void fillPhotons(edm::Event const& e, const unsigned int nVertex);
  bool findTagAndProbePair(edm::Handle<reco::GsfElectronCollection> const& electrons);

  //private variables
  math::XYZPoint PVPoint_;

  //variables from config file
  edm::EDGetTokenT<reco::GsfElectronCollection> theGsfElectronCollection_;
  edm::EDGetTokenT<std::vector<reco::Photon> > thePhotonCollection_;
  edm::EDGetTokenT<reco::VertexCollection> thePVCollection_;
  edm::EDGetTokenT<reco::BeamSpot> theBSCollection_;
  edm::EDGetTokenT<trigger::TriggerEvent> triggerEvent_;
  edm::EDGetTokenT<edm::TriggerResults> triggerResults_;
  edm::InputTag triggerFilter_;
  std::string triggerPath_;
  std::string histFolder_;
  std::string efficiencyFolder_;

  edm::EDGetTokenT<l1t::EGammaBxCollection> stage2CaloLayer2EGammaToken_;

  std::vector<double> electronEfficiencyThresholds_;

  std::vector<double> electronEfficiencyBins_;

  reco::GsfElectron tagElectron_;
  reco::GsfElectron probeElectron_;

  // TODO: add turn-on cuts (vectors of doubles)
  // Histograms
  MonitorElement* h_nVertex_;

  // electron reco vs L1
  MonitorElement* h_L1EGammaETvsElectronET_EB_;
  MonitorElement* h_L1EGammaETvsElectronET_EE_;
  MonitorElement* h_L1EGammaETvsElectronET_EB_EE_;

  MonitorElement* h_L1EGammaPhivsElectronPhi_EB_;
  MonitorElement* h_L1EGammaPhivsElectronPhi_EE_;
  MonitorElement* h_L1EGammaPhivsElectronPhi_EB_EE_;

  MonitorElement* h_L1EGammaEtavsElectronEta_;

  // electron resolutions
  MonitorElement* h_resolutionElectronET_EB_;
  MonitorElement* h_resolutionElectronET_EE_;
  MonitorElement* h_resolutionElectronET_EB_EE_;

  MonitorElement* h_resolutionElectronPhi_EB_;
  MonitorElement* h_resolutionElectronPhi_EE_;
  MonitorElement* h_resolutionElectronPhi_EB_EE_;

  MonitorElement* h_resolutionElectronEta_;

  // electron turn-ons
  std::map<double, MonitorElement*> h_efficiencyElectronET_EB_pass_;
  std::map<double, MonitorElement*> h_efficiencyElectronET_EE_pass_;
  std::map<double, MonitorElement*> h_efficiencyElectronET_EB_EE_pass_;

  // we could drop the map here, but L1TEfficiency_Harvesting expects
  // identical names except for the suffix
  std::map<double, MonitorElement*> h_efficiencyElectronET_EB_total_;
  std::map<double, MonitorElement*> h_efficiencyElectronET_EE_total_;
  std::map<double, MonitorElement*> h_efficiencyElectronET_EB_EE_total_;

};

#endif
