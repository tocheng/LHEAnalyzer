#ifndef HVVTREE_H
#define HVVTREE_H

#include "BaseTree.h"
#include "OptionParser.h"

class HVVTree : public BaseTree{
public:
  HVVTree():BaseTree(), options(0){}
  HVVTree(string treename) : BaseTree(treename), options(0){}
  HVVTree(string treename, string treetitle) : BaseTree(treename, treetitle), options(0){}
  HVVTree(string treename, TFile* fin) : BaseTree(treename, fin), options(0){}

  void setOptions(OptionParser* options_){ options=options_; }
  vector<string> getMELABranchList()const;

  bool reserveBranch(string branchname, BaseTree::BranchTypes branchtype, bool doSetAddress);
  void bookAllBranches(bool doSetAddress);

  void fillEventVariables(Float_t weight, Int_t passSelection);
  void fillMotherInfo(Particle* mother);

  void fillCandidate(ZZCandidate* pH, bool isGen=false);
  void fillCandidateDaughters(ZZCandidate* pH, bool isGen=false);
  void fillDaughterProducts(ZZCandidate* pH, bool isGen=false);
  void fillAssociatedInfo(ZZCandidate* pH, bool isGen=false);

//  void calculateDecayAngles(TLorentzVector thep4M11, TLorentzVector thep4M12, TLorentzVector thep4M21, TLorentzVector thep4M22, Float_t& costheta1, Float_t& costheta2, Float_t& phi, Float_t& costhetastar, Float_t& phistar1);
  void fillDecayAngles(ZZCandidate* pH, bool isGen=false);
//  void fillProductionAngles(ZZCandidate* pH, bool isGen=false);

  void fillMELAProbabilities(ZZCandidate* pH, bool isGen);

protected:
  void bookPtEtaPhiMassIdBranches(string owner, BaseTree::BranchTypes btype, bool doSetAddress, bool addId, bool usePz, bool isGen);
  void getPtEtaPhiMIdBranches(vector<string>& blist, string owner, bool addId, bool usePz, bool isGen);

  void bookAngularBranches(bool doSetAddress);
  void getAngularBranches(vector<string>& blist, Int_t prodFlag /* 0: Decay, 1: VBF, 2: VH */, bool isGen);

  void bookMELABranches(bool doSetAddress);
  vector<string> constructMELABranchList(bool doSetAddress);
  void setupMELASignalMECases(vector<string>& accumulatedlist, TVar::Production prod, TVar::MatrixElement me, bool isGen, bool isProdME, bool doSetAddress);
  vector<string> getMELASignalMEBranches(TVar::Production prod, TVar::MatrixElement me, vector<string> gList, vector<int> gCountRe, vector<int> gCountIm, bool isGen, bool isProdME, bool doSetAddress);

  OptionParser* options;
  vector<string> melaProbBranches;
};

#endif
