#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TH1D.h>
#include <TClonesArray.h>           // ROOT array class
#include <TBenchmark.h>             // class to track macro running statistics
#include <TLorentzVector.h>
#include <TMath.h>                  // ROOT math library
#include <TRandom3.h>
#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <sstream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <TChain.h>

#include "CaltechTriboson/Utils/Event.hh"
#include "CaltechTriboson/Utils/Obj.hh"
#include "CaltechTriboson/Utils/Lep.hh"
#include "CaltechTriboson/Utils/Jet.hh"

#include "CaltechTriboson/Utils/LeptonID.hh"

#endif

void Selection2(TString infile="/eos/cms/store/group/phys_susy/razor/run2/Run2RazorNtupleV3.15/MC_Summer16/RunIISpring16/v5/sixie/WWZJetsTo4L2Nu_4f_TuneCUETP8M1_13TeV_aMCatNLOFxFx_pythia8/Run2RazorNtuplerV3p15_ToCERN_MC_Summer16_25ns_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2_v5_v1/170815_180614/0000/razorNtuple_10.root",
		TString outfile="test.root", 
	       Float_t xsec=1.0, 
	       Float_t lumi=100.0
		  ) {

  int mcPeriod=2016;

  const float EL_MASS = 0.000511;
  const float MU_MASS = 0.105658;
  const float Z_MASS = 91.1876;

  const float LEP_ETA_CUT = 2.4;
  const float LEP_PT_CUT = 10;

  const float JET_ETA_CUT = 3.0;
  const float JET_PT_CUT = 20;

  const float OVLP_R = 0.3;

  TFile *f = new TFile(infile,"read");

  TTree *t = (TTree*) f->Get("ntuples/RazorEvents");

  Bool_t          isData;
  UInt_t          runNum;
  UInt_t          lumiNum;
  UInt_t          eventNum;
  Float_t         fixedGridRhoFastjetAll;

  Int_t           nMuons;
  Float_t         muonE[700];   //[nMuons]
  Float_t         muonPt[700];   //[nMuons]
  Float_t         muonEta[700];   //[nMuons]
  Float_t         muonPhi[700];   //[nMuons]
  Int_t           muonCharge[700];   //[nMuons]
  Float_t         muon_dZ[700]; //[nMuons]
  Bool_t          muonIsLoose[700]; //[nMuons]
  Float_t muon_ip3dSignificance[700]; //[nMuons]
  Float_t         muon_pileupIso[700];   //[nMuons]
  Float_t         muon_chargedIso[700];   //[nMuons]
  Float_t         muon_photonIso[700];   //[nMuons]
  Float_t         muon_neutralHadIso[700]; //[nMuons]

  Int_t           nElectrons;
  Float_t         eleE[700];   //[nElectrons]
  Float_t         elePt[700];   //[nElectrons]
  Float_t         eleEta[700];   //[nElectrons]
  Float_t         elePhi[700];   //[nElectrons]
  Float_t         eleCharge[700];   //[nElectrons]
  Float_t         eleEta_SC[700]; //[nElectrons]
  Float_t         ele_d0[700]; //[nElectrons]
  Float_t         ele_dZ[700]; //[nElectrons]
  Float_t         ele_pileupIso[700];   //[nElectrons]
  Float_t         ele_chargedIso[700];   //[nElectrons]
  Float_t         ele_photonIso[700];   //[nElectrons]
  Float_t         ele_neutralHadIso[700]; //[nElectrons]  
  Float_t         ele_IDMVAGeneralPurpose[700]; //[nElectrons]
  Float_t         ele_IDMVAHZZ[700]; //[nElectrons]
  
  Int_t           nJets;
  Float_t         jetE[900];   //[nJets]
  Float_t         jetPt[900];   //[nJets]
  Float_t         jetEta[900];   //[nJets]
  Float_t         jetPhi[900];   //[nJets]
  Float_t         jetCSV[900];   //[nJets]
  Float_t         jetCISV[900];   //[nJets]
  Bool_t jetPassIDLoose[900]; //[nJets]

  Float_t         metPt;
  Float_t         metPhi;
  Float_t         metPuppiPt;
  Float_t         metPuppiPhi;
  Float_t         metType1Pt;
  Float_t         metType1Phi;

  Float_t         genWeight;
  Int_t           nGenParticle;
  Int_t           gParticleMotherId[4000];   //[nGenParticle]
  Int_t           gParticleMotherIndex[4000];   //[nGenParticle]
  Int_t           gParticleId[4000];   //[nGenParticle]
  Int_t           gParticleStatus[4000];   //[nGenParticle]
  Float_t         gParticleE[4000];   //[nGenParticle]
  Float_t         gParticlePt[4000];   //[nGenParticle]
  Float_t         gParticleEta[4000];   //[nGenParticle]
  Float_t         gParticlePhi[4000]; //[nGenParticle]

  t->SetBranchAddress("isData",	         &isData);
  t->SetBranchAddress("runNum",	       	 &runNum);
  t->SetBranchAddress("lumiNum",	 &lumiNum);
  t->SetBranchAddress("eventNum",	 &eventNum);
  t->SetBranchAddress("fixedGridRhoFastjetAll",	 &fixedGridRhoFastjetAll);

  t->SetBranchAddress("nMuons",	       	 &nMuons);
  t->SetBranchAddress("muonE",      	 &muonE);
  t->SetBranchAddress("muonPt",     	 &muonPt);
  t->SetBranchAddress("muonEta",    	 &muonEta);
  t->SetBranchAddress("muonPhi",    	 &muonPhi);
  t->SetBranchAddress("muonCharge",    	 &muonCharge);
  t->SetBranchAddress("muonIsLoose",   	 &muonIsLoose);
  t->SetBranchAddress("muon_dZ",    	 &muon_dZ);
  t->SetBranchAddress("muon_ip3dSignificance",&muon_ip3dSignificance);
  t->SetBranchAddress("muon_pileupIso",    	 &muon_pileupIso);
  t->SetBranchAddress("muon_chargedIso",    	 &muon_chargedIso);
  t->SetBranchAddress("muon_photonIso",    	 &muon_photonIso);
  t->SetBranchAddress("muon_neutralHadIso",    	 &muon_neutralHadIso);

  t->SetBranchAddress("nElectrons",   	 &nElectrons);
  t->SetBranchAddress("eleE",       	 &eleE);
  t->SetBranchAddress("elePt",      	 &elePt);
  t->SetBranchAddress("eleEta",     	 &eleEta);
  t->SetBranchAddress("elePhi",     	 &elePhi);
  t->SetBranchAddress("eleCharge",  	 &eleCharge);
  t->SetBranchAddress("eleEta_SC",     	 &eleEta_SC);
  t->SetBranchAddress("ele_d0",    	 &ele_d0);
  t->SetBranchAddress("ele_dZ",    	 &ele_dZ);
  t->SetBranchAddress("ele_pileupIso",    	 &ele_pileupIso);
  t->SetBranchAddress("ele_chargedIso",    	 &ele_chargedIso);
  t->SetBranchAddress("ele_photonIso",    	 &ele_photonIso);
  t->SetBranchAddress("ele_neutralHadIso",    	 &ele_neutralHadIso);
  t->SetBranchAddress("ele_IDMVAGeneralPurpose", &ele_IDMVAGeneralPurpose);
  t->SetBranchAddress("ele_IDMVAHZZ",    	 &ele_IDMVAHZZ);

  t->SetBranchAddress("nJets",	       	 &nJets);
  t->SetBranchAddress("jetE",       	 &jetE);
  t->SetBranchAddress("jetPt",      	 &jetPt);
  t->SetBranchAddress("jetEta",     	 &jetEta);
  t->SetBranchAddress("jetPhi",     	 &jetPhi);
  t->SetBranchAddress("jetCSV",     	 &jetCSV);
  t->SetBranchAddress("jetCISV",   	 &jetCISV);
  t->SetBranchAddress("jetPassIDLoose",  &jetPassIDLoose);

  t->SetBranchAddress("metPt",	       	 &metPt);
  t->SetBranchAddress("metPhi",          &metPhi);
  t->SetBranchAddress("metPuppiPt",	 &metPuppiPt);
  t->SetBranchAddress("metPuppiPhi",     &metPuppiPhi);
  t->SetBranchAddress("metType1Pt",	 &metType1Pt);
  t->SetBranchAddress("metType1Phi",     &metType1Phi);

  t->SetBranchAddress("genWeight",         &genWeight);
  t->SetBranchAddress("nGenParticle",         &nGenParticle);
  t->SetBranchAddress("gParticleMotherId",    &gParticleMotherId);
  t->SetBranchAddress("gParticleMotherIndex", &gParticleMotherIndex);
  t->SetBranchAddress("gParticleId",          &gParticleId);
  t->SetBranchAddress("gParticleStatus",      &gParticleStatus);
  t->SetBranchAddress("gParticleE",           &gParticleE);
  t->SetBranchAddress("gParticlePt",          &gParticlePt);
  t->SetBranchAddress("gParticleEta",         &gParticleEta);
  t->SetBranchAddress("gParticlePhi",         &gParticlePhi);

  TFile *outFile = new TFile(outfile,"recreate");
  TTree *outTree = new TTree("wwz","wwz");

  // pileup histogram
  TH1D* puhisto = new TH1D("pileup", "", 50, 0, 50);
  
  //histogram containing total number of processed events (for normalization)
  TH1F *histNPV = new TH1F("NPV", "NPV", 2, -0.5, 1.5);
  TH1F *NEvents = new TH1F("NEvents", "NEvents", 1, 1, 2);
  TH1F *SumWeights = new TH1F("SumWeights", "SumWeights", 1, 0.5, 1.5);
  TH1F *SumScaleWeights = new TH1F("SumScaleWeights", "SumScaleWeights", 6, -0.5, 5.5);
  //TH1F *SumPdfWeights = new TH1F("SumPdfWeights", "SumPdfWeights", NUM_PDF_WEIGHTS, -0.5, NUM_PDF_WEIGHTS-0.5);

  WWZ::Event *evt = new WWZ::Event();

  //TObject::Class()->IgnoreTObjectStreamer();
  WWZ::Event::Class()->IgnoreTObjectStreamer();
  WWZ::Obj::Class()->IgnoreTObjectStreamer();
  WWZ::Lep::Class()->IgnoreTObjectStreamer();
  WWZ::Jet::Class()->IgnoreTObjectStreamer();

  outTree->Branch("evt", &evt);
  
  for (uint i=0; i<t->GetEntries(); i++) {
  //for (uint i=0; i<10; i++) {
    t->GetEntry(i);
    evt->Reset();

    //fill normalization histogram    
    NEvents->SetBinContent( 1, NEvents->GetBinContent(1) + genWeight);
    evt->weight = genWeight;
    SumWeights->Fill(1.0, evt->weight);
  
    //evt->pileupWeight = 1.0;

    // pileup reweighting TODO
    //if( !isData ) {
    //  //Get number of PU interactions
    //  for (int i = 0; i < nBunchXing; i++) {
    //	if (BunchXing[i] == 0) {
    //	  NPU = nPUmean[i];
    //	}
    //  }
    //  puhisto->Fill(NPU);
    //  pileupWeight = helper->getPileupWeight(NPU);
    //  pileupWeightUp = helper->getPileupWeightUp(NPU) / pileupWeight;
    //  pileupWeightDown = helper->getPileupWeightDown(NPU) / pileupWeight;
    //}
    
    // scale and PDF variations TODO
    //if (scaleWeights) {
    //  if ( (*scaleWeights).size() >= 9 ) 
    //	{
    //	  // sf_facScaleUp      = (*scaleWeights)[1]/genWeight;
    //	  // sf_facScaleDown    = (*scaleWeights)[2]/genWeight;
    //	  // sf_renScaleUp      = (*scaleWeights)[3]/genWeight;
    //	  // sf_renScaleDown    = (*scaleWeights)[6]/genWeight;
    //	  // sf_facRenScaleUp   = (*scaleWeights)[4]/genWeight;
    //	  // sf_facRenScaleDown = (*scaleWeights)[8]/genWeight;
    //	      
    //	      
    //	  SumScaleWeights->Fill(0.0, (*scaleWeights)[1]);
    //	  SumScaleWeights->Fill(1.0, (*scaleWeights)[2]);
    //	  SumScaleWeights->Fill(2.0, (*scaleWeights)[3]);
    //	  SumScaleWeights->Fill(3.0, (*scaleWeights)[6]);
    //	  SumScaleWeights->Fill(4.0, (*scaleWeights)[4]);
    //	  SumScaleWeights->Fill(5.0, (*scaleWeights)[8]);
    //	}
    //}

    //if (pdfWeights) {      
    //  sf_pdf.erase( sf_pdf.begin(), sf_pdf.end() );
    //  for ( unsigned int iwgt = 0; iwgt < pdfWeights->size(); ++iwgt ) 
    //	{
    //	  sf_pdf.push_back( pdfWeights->at(iwgt)/genWeight );
    //	  SumPdfWeights->Fill(double(iwgt),(*pdfWeights)[iwgt]);
    //	}
    //}

    //Gen info TODO


    // 
    // OBJECT SELECTION
    //

    std::vector<WWZ::Lep> vLep;

    //muons
    for( int i = 0; i < nMuons; i++ ){
      if (!muLooseID(muonIsLoose[i], muon_ip3dSignificance[i])) continue;
      if (!muLooseIso(muonPt[i], muon_chargedIso[i], muon_photonIso[i], muon_neutralHadIso[i], muon_pileupIso[i])) continue;
  
      if(muonPt[i] < LEP_PT_CUT) continue;
      if(fabs(muonEta[i]) > LEP_ETA_CUT) continue;
  
      //remove overlaps
      bool overlap = false;
      for(auto& lep : vLep){
      	if (deltaR(muonEta[i],muonPhi[i],lep.recV.Eta(),lep.recV.Phi()) < OVLP_R) overlap = true;
      }
      if(overlap) continue;

      WWZ::Lep tLep(muonPt[i], muonEta[i], muonPhi[i], MU_MASS, muon_dZ[i], -13*muonCharge[i]);
      tLep.passLoose = true; 

      vLep.push_back(tLep);
    }

    //electrons
    for( int i = 0; i < nElectrons; i++ ) {
      if (!eleVetoMvaID(elePt[i], eleEta_SC[i], ele_IDMVAGeneralPurpose[i], ele_IDMVAHZZ[i])) continue;
      if (!eleVetoIso(elePt[i], eleEta_SC[i], ele_chargedIso[i], ele_photonIso[i], 
		      ele_neutralHadIso[i], ele_pileupIso[i])) continue;
      if (!( fabs(eleEta[i]<1.5 && fabs(ele_d0[i]) < 0.0564) ||
	     (fabs(eleEta[i]) >= 1.5 && fabs(ele_d0[i]) < 0.222))) continue;

      if(elePt[i] < LEP_PT_CUT) continue;
      if(fabs(eleEta[i]) > LEP_ETA_CUT) continue;
      
      //remove overlaps
      bool overlap = false;
      for(auto& lep : vLep){
      	if (deltaR(eleEta[i],elePhi[i],lep.recV.Eta(),lep.recV.Phi()) < OVLP_R) overlap = true;
      }
      if(overlap) continue;
      
      WWZ::Lep tLep(elePt[i], eleEta[i], elePhi[i], EL_MASS, ele_dZ[i], -13*eleCharge[i]);
      tLep.passLoose = true; 

      vLep.push_back(tLep);
    }

    //find Z candidate
    TLorentzVector ZCandidate;
    
    float minDm = 9999;
    pair<uint, uint> ZCandLepIndex;
    bool foundZ=false;
    
    for (uint i=0; i<vLep.size(); i++) {
      for (uint j=i+1; j<vLep.size(); j++) {
	if (!(vLep[i].fpid == -1*vLep[j].fpid)) continue;
	float tmpM = (vLep[i].recV+vLep[j].recV).M();
	
	if (fabs(tmpM-Z_MASS) < minDm) {
	  minDm = fabs(tmpM-Z_MASS);
	  
	  if (vLep[i].fpid>0) {
	    ZCandLepIndex=pair<int,int>(i,j);
	  }
	  else {
	    ZCandLepIndex=pair<int,int>(j,i);
	  }

	  evt->ZMass = tmpM;
	  evt->ZPt   = (vLep[i].recV+vLep[j].recV).Pt();
	  foundZ= true;
	}
      } 
    }

    if (foundZ) {
      evt->lep1 = vLep[ZCandLepIndex.first];
      evt->lep2 = vLep[ZCandLepIndex.second];      
    }

    // w leptons
    for (uint i=0; i<vLep.size(); i++) {
      if (foundZ && ( i == ZCandLepIndex.first || i == ZCandLepIndex.second )) continue;

      if (vLep[i].recV.Pt() > evt->lep3.recV.Pt()) {
	evt->lep4 = evt->lep3;
	evt->lep3 = vLep[i];
      }
      else if (vLep[i].recV.Pt() > evt->lep4.recV.Pt()) {
	evt->lep4 = vLep[i];
      }
    }

    // jets
    std::vector<WWZ::Jet> jetVector;
    auto ptOrder = [](auto a, auto b) { return a.recV.Pt() > b.recV.Pt(); };
      
    for(int i = 0; i < nJets; i++){

      // remove overlaps
      double dR = -1;
      for(auto& lep : vLep){
      	double thisDR = deltaR(jetEta[i],jetPhi[i],lep.recV.Eta(),lep.recV.Phi());  
      	if(dR < 0 || thisDR < dR) dR = thisDR;
      }
      if(dR > 0 && dR < 0.4) continue; //jet matches a selected lepton

      //jec TODO
      double JEC = 1.0;//JetEnergyCorrectionFactor( jetPt[i], jetEta[i], jetPhi[i], jetE[i],
      //fixedGridRhoAll, jetJetArea[i], runNum,
      //JetCorrectorIOV,JetCorrector);


      if( jetPt[i]*JEC < JET_PT_CUT ) continue;
      if( fabs( jetEta[i] ) >= JET_ETA_CUT ) continue;
      if ( !jetPassIDLoose[i] ) continue;

      WWZ::Jet thisJet(jetPt[i]*JEC, jetEta[i], jetPhi[i], jetE[i]*JEC);
      thisJet.csv = jetCISV[i]; //is btag? TODO
      thisJet.jec=JEC;

      evt->NJet20++;

      if (thisJet.recV.Pt() > 30) {
	evt->NJet30++;
	
	//TODO
	//if (lep3Index >= 0) {
	//  double dRJetToLep3 = RazorAnalyzer::deltaR(thisJet.Eta(),thisJet.Phi(),Leptons[lep3Index].Eta(),Leptons[lep3Index].Phi());
	//  if (dRJetToLep3 < minDRJetToLep3) minDRJetToLep3 = dRJetToLep3;
	//}
	//if (lep4Index >= 0) {
	//  double dRJetToLep4 = RazorAnalyzer::deltaR(thisJet.Eta(),thisJet.Phi(),Leptons[lep4Index].Eta(),Leptons[lep4Index].Phi());
	//  if (dRJetToLep4 < minDRJetToLep4) minDRJetToLep4 = dRJetToLep4;
	//}
      }
      
      if (isData && jetLooseCISV(jetCISV[i], runNum)) {
	thisJet.isBtag=true;
	evt->NBJet20++;
	if (thisJet.recV.Pt()>30) evt->NBJet30++;
      }
      else if (!isData && jetLooseCISV(jetCISV[i], mcPeriod)) {
	thisJet.isBtag=true;
	evt->NBJet20++;
	if (thisJet.recV.Pt()>30) evt->NBJet30++;
      }
      jetVector.push_back(thisJet);
    }

    sort(jetVector.begin(), jetVector.end(), ptOrder);
    if (jetVector.size()>=1) evt->jet1=jetVector[0];
    if (jetVector.size()>=2) evt->jet2=jetVector[1];
    if (jetVector.size()>=3) evt->jet3=jetVector[2];
    if (jetVector.size()>=4) evt->jet4=jetVector[3];


    // event variables
    double PFMetCustomType1CorrectedX = metType1Pt*cos(metType1Phi);
    double PFMetCustomType1CorrectedY = metType1Pt*sin(metType1Phi);
    TLorentzVector PFMETCustomType1Corrected; 
    PFMETCustomType1Corrected.SetPxPyPzE(PFMetCustomType1CorrectedX, PFMetCustomType1CorrectedY, 0, 
					 sqrt( pow(PFMetCustomType1CorrectedX,2) + pow(PFMetCustomType1CorrectedY,2)));      
    TLorentzVector MyMET = PFMETCustomType1Corrected; //This is the MET that will be used below.
    evt->MET = MyMET.Pt();
    evt->METPhi = MyMET.Phi();
    evt->METPuppiPt = metPuppiPt;
    evt->METPuppiPhi = metPuppiPhi;

    //PTZeta TODO
    //if (foundZ) { 
    //  TVector3 lep1, lep2, metv3, zeta;
    //  
    //  metv3.SetPtEtaPhi(MET, 0., METPhi);
    //  lep1.SetPtEtaPhi(Leptons[ZCandidateLeptonIndex.first].Pt(), 0,  Leptons[ZCandidateLeptonIndex.first].Phi());
    //  lep2.SetPtEtaPhi(Leptons[ZCandidateLeptonIndex.second].Pt(), 0,  Leptons[ZCandidateLeptonIndex.second].Phi());
    //  
    //  zeta = lep1*lep2.Mag() + lep2*lep1.Mag(); // find bisector
    //  
    //  TVector3 sum = lep1 + lep2 + metv3;
    //  TVector3 sum_vis = lep1 + lep2;
    //  
    //  pt_zeta = sum.Dot(zeta.Unit());
    //  pt_zeta_vis = sum_vis.Dot(zeta.Unit());
    //}
    //  
    //TLorentzVector DileptonWW;
    //if (lep3Index >= 0) {
    //  lep3MT = sqrt(2*lep3Pt*MyMET.Pt()*( 1.0 - cos(  Leptons[lep3Index].DeltaPhi(MyMET) ) ) ); 
    //  if (lep4Index >= 0) {
    //	lep4MT = sqrt(2*lep4Pt*MyMET.Pt()*( 1.0 - cos(  Leptons[lep4Index].DeltaPhi(MyMET) ) ) ); 
    //	DileptonWW = Leptons[lep3Index] + Leptons[lep4Index];  
    //	lep34MT = sqrt(2*DileptonWW.Pt()*MyMET.Pt()*( 1.0 - cos(  DileptonWW.DeltaPhi(MyMET) ) ) ); 
    //  }
    //}

    //angular variables TODO


    //trigger weights
    //evt->triggerEffWeight = 1.0;
    //evt->triggerEffSFWeight = 1.0;
    
    //filter
    //if (!(lep1.pt > LEP_PT_CUT && lep2.pt > LEP_PT_CUT && 
    //	  lep3.pt > LEP_PT_CUT && lep4.pt > LEP_PT_CUT)) continue;

    outTree->Fill();
  }
  
  outFile->Write();
  //NEvents->Write();
  //SumWeights->Write();
  //SumScaleWeights->Write();
  ////SumPdfWeights->Write();
  //histNPV->Write();
  //puhisto->Write();
  outFile->Close();


}
