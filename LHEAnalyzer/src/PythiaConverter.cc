#include <cassert>
#include "TSystem.h"
#include "TInterpreter.h"
#include "TList.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "PythiaConverter.h"


using namespace std;
using namespace PDGHelpers;


struct PythiaConverter::PythiaIOHandle{
  TTree* tin;

  vectorDouble* geneventinfoweights;
  TBranch* b_geneventinfoweights;

  vectorFloat* GenParticles_FV[4];
  vectorInt* GenParticles_id;
  vectorInt* GenParticles_status;
  TBranch* b_GenParticles_FV[4];
  TBranch* b_GenParticles_id;
  TBranch* b_GenParticles_status;

  vectorFloat* FinalParticles_FV[4];
  vectorInt* FinalParticles_id;
  vectorInt* FinalParticles_status;
  TBranch* b_FinalParticles_FV[4];
  TBranch* b_FinalParticles_id;
  TBranch* b_FinalParticles_status;

  vectorFloat* GenJets_FV[4];
  vectorInt* GenJets_id;
  vectorInt* GenJets_status;
  TBranch* b_GenJets_FV[4];
  TBranch* b_GenJets_id;
  TBranch* b_GenJets_status;

  PythiaIOHandle(TTree* tin_);
  ~PythiaIOHandle(){}
};
PythiaConverter::PythiaIOHandle::PythiaIOHandle(TTree* tin_) : tin(tin_){
  geneventinfoweights=nullptr;
  b_geneventinfoweights=nullptr;

  GenParticles_id=nullptr;
  GenParticles_status=nullptr;
  b_GenParticles_id=nullptr;
  b_GenParticles_status=nullptr;

  FinalParticles_id=nullptr;
  FinalParticles_status=nullptr;
  b_FinalParticles_id=nullptr;
  b_FinalParticles_status=nullptr;

  GenJets_id=nullptr;
  GenJets_status=nullptr;
  b_GenJets_id=nullptr;
  b_GenJets_status=nullptr;

  for (unsigned char ic=0; ic<4; ic++){
    GenParticles_FV[ic]=nullptr;
    b_GenParticles_FV[ic]=nullptr;

    FinalParticles_FV[ic]=nullptr;
    b_FinalParticles_FV[ic]=nullptr;

    GenJets_FV[ic]=nullptr;
    b_GenJets_FV[ic]=nullptr;
  }

  if (tin){
    tin->SetBranchAddress("genWeights", &geneventinfoweights, &b_geneventinfoweights);

    tin->SetBranchAddress("GenParticles_X", GenParticles_FV, b_GenParticles_FV);
    tin->SetBranchAddress("GenParticles_Y", GenParticles_FV+1, b_GenParticles_FV+1);
    tin->SetBranchAddress("GenParticles_Z", GenParticles_FV+2, b_GenParticles_FV+2);
    tin->SetBranchAddress("GenParticles_E", GenParticles_FV+3, b_GenParticles_FV+3);
    tin->SetBranchAddress("GenParticles_id", &GenParticles_id, &b_GenParticles_id);
    tin->SetBranchAddress("GenParticles_status", &GenParticles_status, &b_GenParticles_status);

    tin->SetBranchAddress("FinalParticles_X", FinalParticles_FV, b_FinalParticles_FV);
    tin->SetBranchAddress("FinalParticles_Y", FinalParticles_FV+1, b_FinalParticles_FV+1);
    tin->SetBranchAddress("FinalParticles_Z", FinalParticles_FV+2, b_FinalParticles_FV+2);
    tin->SetBranchAddress("FinalParticles_E", FinalParticles_FV+3, b_FinalParticles_FV+3);
    tin->SetBranchAddress("FinalParticles_id", &FinalParticles_id, &b_FinalParticles_id);
    tin->SetBranchAddress("FinalParticles_status", &FinalParticles_status, &b_FinalParticles_status);

    tin->SetBranchAddress("GenJets_X", GenJets_FV, b_GenJets_FV);
    tin->SetBranchAddress("GenJets_Y", GenJets_FV+1, b_GenJets_FV+1);
    tin->SetBranchAddress("GenJets_Z", GenJets_FV+2, b_GenJets_FV+2);
    tin->SetBranchAddress("GenJets_E", GenJets_FV+3, b_GenJets_FV+3);
    tin->SetBranchAddress("GenJets_id", &GenJets_id, &b_GenJets_id);
    tin->SetBranchAddress("GenJets_status", &GenJets_status, &b_GenJets_status);
  }
  else{
    cerr << "PythiaConverter::PythiaIOHandle::PythiaIOHandle: Input tree is null!" << endl;
    throw std::exception();
  }
}


PythiaConverter::PythiaConverter(OptionParser* options_) : converter(options_){
  configure();
  run();
}

void PythiaConverter::configure(){
  string tmpdir = options->getTempDir();
  string strCmd = "mkdir -p ";
  strCmd.append(tmpdir);
  gSystem->Exec(strCmd.c_str());
}
void PythiaConverter::finalizeRun(){
  string tmpdir = options->getTempDir();
  string strCmdcore = "rm -rf ";
  string strCmd = strCmdcore;
  strCmd.append(tmpdir);
  gSystem->Exec(strCmd.c_str());
}
void PythiaConverter::run(){
  Float_t MC_weight=0;
  Int_t isSelected=0;

  int globalNEvents = 0;
  int maxProcEvents = options->maxEventsToProcess();
  vector < pair<Int_t, Int_t> > eventSkipList = options->getSkippedEvents();

  tree->bookAllBranches(false);

  for (string const& cinput:filename){
    if (globalNEvents>=maxProcEvents && maxProcEvents>=0) break;

    cout << "Processing " << cinput << "..." << endl;
    TFile* fin = nullptr;
    fin = getIntermediateFile(cinput);
    if (!fin) continue;
    if (fin->IsZombie()){
      if (fin->IsOpen()) fin->Close();
      delete fin;
      fin=nullptr;
    }
    else if (!fin->IsOpen()){ // Separate if-statement on purpose
      delete fin;
      fin=nullptr;
    }
    else{
      TTree* tin = (TTree*) fin->Get("TrimmedTree");
      PythiaIOHandle intree_handle(tin);
      foutput->cd();

      int nInputEvents = tin->GetEntries();
      int nProcessed = 0;
      for (int ev=0; ev<nInputEvents; ev++){
        if (globalNEvents>=maxProcEvents && maxProcEvents>=0) break;
        bool doSkipEvent = false;
        for (unsigned int es=0; es<eventSkipList.size(); es++){
          if (
            (eventSkipList.at(es).first<=globalNEvents && eventSkipList.at(es).second>=globalNEvents)
            ||
            (eventSkipList.at(es).first<=globalNEvents && eventSkipList.at(es).second<0)
            )doSkipEvent=true;
        }
        if (doSkipEvent){ globalNEvents++; continue; }

        double weight;
        bool genSuccess=false, smearedSuccess=false;

        // Bookkeeping
        vector<MELAParticle*> genParticleList;
        vector<MELAParticle*> smearedParticleList;
        vector<MELACandidate*> genCandList;
        vector<MELACandidate*> smearedCandList;

        tree->initializeBranches();

        tin->GetEntry(ev);
        readEvent(intree_handle, genParticleList, genSuccess, smearedParticleList, smearedSuccess, weight);

        if (weight!=0.){
          MC_weight = (float) weight;

          MELAEvent genEvent;
          if (genSuccess){
            genEvent.setWeight(weight);
            vector<MELAParticle*> writtenGenCands;
            vector<MELAParticle*> writtenGenTopCands;

            for (MELAParticle* genPart:genParticleList){
              if (isATopQuark(genPart->id)){
                writtenGenTopCands.push_back(genPart);
                if (genPart->genStatus==1) genEvent.addIntermediate(genPart);
              }
              if (isAHiggs(genPart->id)){
                writtenGenCands.push_back(genPart);
                if (options->doGenHZZdecay()==MELAEvent::UndecayedMode && (genPart->genStatus==1 || genPart->genStatus==2)) genEvent.addIntermediate(genPart);
              }
              if (genPart->genStatus==1){
                if (isALepton(genPart->id)) genEvent.addLepton(genPart);
                else if (isANeutrino(genPart->id)) genEvent.addNeutrino(genPart);
                else if (isAPhoton(genPart->id)) genEvent.addPhoton(genPart);
                else if (isAGluon(genPart->id) || isAQuark(genPart->id)) genEvent.addJet(genPart);
              }
              else if (genPart->genStatus==-1){
                genEvent.addMother(genPart);
                tree->fillMotherInfo(genPart);
              }
            }

            genEvent.constructTopCandidates();
            // Disable tops unmatched to a gen. top
            {
              vector<MELATopCandidate_t*> matchedTops;
              for (auto* writtenGenTopCand:writtenGenTopCands){
                MELATopCandidate_t* tmpCand = TopComparators::matchATopToParticle(genEvent, writtenGenTopCand);
                if (tmpCand) matchedTops.push_back(tmpCand);
              }
              for (MELATopCandidate_t* tmpCand:genEvent.getTopCandidates()){
                if (std::find(matchedTops.begin(), matchedTops.end(), tmpCand)==matchedTops.end()) tmpCand->setSelected(false);
              }
            }

            genEvent.constructVVCandidates(options->doGenHZZdecay(), options->genDecayProducts());
            genEvent.addVVCandidateAppendages();
            MELACandidate* genCand=nullptr;
            if (!writtenGenCands.empty()){
              for (auto* writtenGenCand:writtenGenCands){
                MELACandidate* tmpCand = HiggsComparators::matchAHiggsToParticle(genEvent, writtenGenCand);
                if (tmpCand){
                  if (!genCand) genCand = tmpCand;
                  else genCand = HiggsComparators::candComparator(genCand, tmpCand, options->getHiggsCandidateSelectionScheme(true), options->doGenHZZdecay());
                }
              }
            }
            if (!genCand) genCand = HiggsComparators::candidateSelector(genEvent, options->getHiggsCandidateSelectionScheme(true), options->doGenHZZdecay());
            if (genCand) tree->fillCandidate(genCand, true);
            else{
              cout << cinput << " (" << ev << "): No gen. level Higgs candidate was found!" << endl;
              genSuccess=false;
            }
          }

          MELAEvent smearedEvent;
          if (smearedSuccess){
            smearedEvent.setWeight(weight);
            for (MELAParticle* smearedPart:smearedParticleList){
              if (isALepton(smearedPart->id)) smearedEvent.addLepton(smearedPart);
              else if (isANeutrino(smearedPart->id)) smearedEvent.addNeutrino(smearedPart);
              else if (isAPhoton(smearedPart->id)) smearedEvent.addPhoton(smearedPart);
              else if (isAKnownJet(smearedPart->id)){
                smearedPart->id=0; // Wipe id from reco. quark/gluon
                if (!isATopQuark(smearedPart->id)) smearedEvent.addJet(smearedPart);
                else smearedEvent.addIntermediate(smearedPart);
              }
              else if (isAnUnknownJet(smearedPart->id)) smearedEvent.addJet(smearedPart);
              else smearedEvent.addParticle(smearedPart);
            }

            smearedEvent.constructTopCandidates();
            smearedEvent.constructVVCandidates(options->doRecoHZZdecay(), options->recoDecayProducts());
            if (options->recoSelectionMode()==0) smearedEvent.applyParticleSelection();
            smearedEvent.addVVCandidateAppendages();

            MELACandidate* rCand = HiggsComparators::candidateSelector(smearedEvent, options->getHiggsCandidateSelectionScheme(false), options->doRecoHZZdecay());
            if (rCand){
              isSelected=1;
              tree->fillCandidate(rCand, false);
            }
            else{
              isSelected=0;
              smearedSuccess=false;
            }
          }

          tree->fillEventVariables(MC_weight, isSelected);
          tree->fillXsec(options->get_xsec(), options->get_xsecerr());

          if ((smearedSuccess && options->processRecoInfo()) || (genSuccess && options->processGenInfo())){
            tree->record();
            nProcessed++;
          }
        }

        // Bookkeeping
        for (auto*& tmpPart:smearedCandList) delete tmpPart;
        for (auto*& tmpPart:smearedParticleList) delete tmpPart;
        for (auto*& tmpPart:genCandList) delete tmpPart;
        for (auto*& tmpPart:genParticleList) delete tmpPart;

        globalNEvents++;
        if (globalNEvents % 100000 == 0) cout << "Event " << globalNEvents << "..." << endl;
      }
      fin->Close();
      cout << "Processed number of events from the input file (recorded events / sample size observed / cumulative traversed): " << nProcessed << " / " << nInputEvents << " / " << globalNEvents << endl;
    }
  }
  finalizeRun();
}


TFile* PythiaConverter::getIntermediateFile(const std::string& cinput){
  TFile* res = nullptr;
  if (options->pythiaType()>-1){
    string coutput = options->getTempDir();
    stringstream streamCmd;
    streamCmd
      << "trimPythia"
      << " "
      << cinput
      << " "
      << coutput
      << " "
      << options->pythiaType()
      << " "
      << options->jetAlgorithm();
    TString strCmd = streamCmd.str();
    gSystem->Exec(strCmd);
    string strtmp = coutput;
    strtmp.append("pythiaTemp.root");
    res = TFile::Open(strtmp.c_str(), "read");
  }
  else if (options->pythiaType()==-1) res = TFile::Open(cinput.c_str(), "read");

  return res;
}


void PythiaConverter::readEvent(PythiaIOHandle const& iohandle, std::vector<MELAParticle*>& genCollection, bool& genSuccess, std::vector<MELAParticle*>& recoCollection, bool& smearedSuccess, double& eventWeight){
  vectorDouble weights;
  // Gen. particles
  vector<MELAParticle*> mothers; mothers.reserve(2);
  unsigned char mctr=0;
  for (unsigned int a = 0; a < iohandle.GenParticles_id->size(); a++){
    int const& istup = iohandle.GenParticles_status->at(a);
    int const& idup = iohandle.GenParticles_id->at(a);
    TLorentzVector partFourVec(iohandle.GenParticles_FV[0]->at(a), iohandle.GenParticles_FV[1]->at(a), iohandle.GenParticles_FV[2]->at(a), iohandle.GenParticles_FV[3]->at(a));

    MELAParticle* onePart = new MELAParticle(idup, partFourVec);
    onePart->setGenStatus(PDGHelpers::convertPythiaStatus(istup));
    onePart->setLifetime(0);
    genCollection.push_back(onePart);
    if (istup==21 && mctr<2){
      mothers.push_back(onePart);
      mctr++;
    }
  }
  // Assign the mothers
  for (MELAParticle* part:genCollection){
    if (part->genStatus==-1) continue;
    for (MELAParticle* mot:mothers) part->addMother(mot);
  }
  genSuccess=(!genCollection.empty());
  // Reco. particles
  for (unsigned int a = 0; a < iohandle.FinalParticles_id->size(); a++){
    int const& istup = iohandle.FinalParticles_status->at(a);
    int const& idup = iohandle.FinalParticles_id->at(a);
    TLorentzVector partFourVec(iohandle.FinalParticles_FV[0]->at(a), iohandle.FinalParticles_FV[1]->at(a), iohandle.FinalParticles_FV[2]->at(a), iohandle.FinalParticles_FV[3]->at(a));

    MELAParticle* onePart = new MELAParticle(idup, partFourVec);
    if (options->recoSmearingMode()>0){ // Reverse of LHE mode
      MELAParticle* smearedPart = LHEParticleSmear::smearParticle(onePart);
      delete onePart;
      onePart = smearedPart;
    }
    onePart->setGenStatus(PDGHelpers::convertPythiaStatus(istup));
    onePart->setLifetime(0);
    recoCollection.push_back(onePart);
  }
  for (unsigned int a = 0; a < iohandle.GenJets_id->size(); a++){
    int const& istup = iohandle.GenJets_status->at(a);
    int const& idup = iohandle.GenJets_id->at(a);
    TLorentzVector partFourVec(iohandle.GenJets_FV[0]->at(a), iohandle.GenJets_FV[1]->at(a), iohandle.GenJets_FV[2]->at(a), iohandle.GenJets_FV[3]->at(a));

    MELAParticle* onePart = new MELAParticle(idup, partFourVec);
    if (options->recoSmearingMode()>0){ // Reverse of LHE mode
      MELAParticle* smearedPart = LHEParticleSmear::smearParticle(onePart);
      delete onePart;
      onePart = smearedPart;
    }
    onePart->setGenStatus(PDGHelpers::convertPythiaStatus(istup));
    onePart->setLifetime(0);
    recoCollection.push_back(onePart);
  }
  smearedSuccess=(!recoCollection.empty());

  if (iohandle.geneventinfoweights && !iohandle.geneventinfoweights->empty()){
    weights.reserve(iohandle.geneventinfoweights->size());
    for (auto const& w:(*iohandle.geneventinfoweights)) weights.push_back(w);
  }
  else if (!smearedSuccess && !genSuccess) weights.push_back(0);
  else weights.push_back(1.);

  eventWeight = weights.front();
}

