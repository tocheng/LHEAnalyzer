#include "../interface/HVVTree.h"


void HVVTree::bookAllBranches(bool doSetAddress){
  bookBranch("MC_weight", BranchTypes::bFloat, doSetAddress);

  if (options->processGenInfo()){
    bookBranch("genFinalState", BranchTypes::bInt, doSetAddress);

    bookBranch("GenMotherMass", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenMotherPt", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenMotherPz", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenMotherPhi", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenMotherId", BranchTypes::bVectorInt, doSetAddress);

    bookBranch("GenHMass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenHPt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenHPz", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenHPhi", BranchTypes::bFloat, doSetAddress);

    bookBranch("GenZ1Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZ1Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZ1Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZ1Eta", BranchTypes::bFloat, doSetAddress);

    bookBranch("GenZ2Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZ2Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZ2Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZ2Eta", BranchTypes::bFloat, doSetAddress);

    bookBranch("GenZaMass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZaPt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZaPhi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZaEta", BranchTypes::bFloat, doSetAddress);

    bookBranch("GenZbMass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZbPt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZbPhi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenZbEta", BranchTypes::bFloat, doSetAddress);

    bookBranch("GenAssociatedParticleMass", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedParticlePt", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedParticleEta", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedParticlePhi", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedParticleId", BranchTypes::bVectorInt, doSetAddress);
    bookBranch("GenDijetMass", BranchTypes::bFloat, doSetAddress);

    bookBranch("GenNAssociatedVs", BranchTypes::bInt, doSetAddress);
    bookBranch("GenAssociatedVMass", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedVPt", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedVEta", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedVPhi", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("GenAssociatedVId", BranchTypes::bVectorInt, doSetAddress);
    bookBranch("GenAssociatedV_Particle1Index", BranchTypes::bVectorInt, doSetAddress);
    bookBranch("GenAssociatedV_Particle2Index", BranchTypes::bVectorInt, doSetAddress);

    bookBranch("GenhelcosthetaZ1", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenhelcosthetaZ2", BranchTypes::bFloat, doSetAddress);
    bookBranch("Genhelphi", BranchTypes::bFloat, doSetAddress);
    bookBranch("Gencosthetastar", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenphistarZ1", BranchTypes::bFloat, doSetAddress);

    bookBranch("GenLep1Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep2Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep3Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep4Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep1Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep2Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep3Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep4Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep1Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep2Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep3Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep4Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep1Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep2Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep3Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep4Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("GenLep1Id", BranchTypes::bInt, doSetAddress);
    bookBranch("GenLep2Id", BranchTypes::bInt, doSetAddress);
    bookBranch("GenLep3Id", BranchTypes::bInt, doSetAddress);
    bookBranch("GenLep4Id", BranchTypes::bInt, doSetAddress);
  }

  if (options->processRecoInfo()){
    bookBranch("isSelected", BranchTypes::bInt, doSetAddress);

    bookBranch("ZZMass", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZZPt", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZZPz", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZZPhi", BranchTypes::bFloat, doSetAddress);

    bookBranch("Z1Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("Z1Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("Z1Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("Z1Eta", BranchTypes::bFloat, doSetAddress);

    bookBranch("Z2Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("Z2Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("Z2Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("Z2Eta", BranchTypes::bFloat, doSetAddress);

    bookBranch("ZaMass", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZaPt", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZaPhi", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZaEta", BranchTypes::bFloat, doSetAddress);

    bookBranch("ZbMass", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZbPt", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZbPhi", BranchTypes::bFloat, doSetAddress);
    bookBranch("ZbEta", BranchTypes::bFloat, doSetAddress);

    bookBranch("AssociatedParticleMass", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedParticlePt", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedParticleEta", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedParticlePhi", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedParticleId", BranchTypes::bVectorInt, doSetAddress);
    bookBranch("DijetMass", BranchTypes::bFloat, doSetAddress);

    bookBranch("NAssociatedVs", BranchTypes::bInt, doSetAddress);
    bookBranch("AssociatedVMass", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedVPt", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedVEta", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedVPhi", BranchTypes::bVectorDouble, doSetAddress);
    bookBranch("AssociatedVId", BranchTypes::bVectorInt, doSetAddress);
    bookBranch("AssociatedV_Particle1Index", BranchTypes::bVectorInt, doSetAddress);
    bookBranch("AssociatedV_Particle2Index", BranchTypes::bVectorInt, doSetAddress);

    bookBranch("helcosthetaZ1", BranchTypes::bFloat, doSetAddress);
    bookBranch("helcosthetaZ2", BranchTypes::bFloat, doSetAddress);
    bookBranch("helphi", BranchTypes::bFloat, doSetAddress);
    bookBranch("costhetastar", BranchTypes::bFloat, doSetAddress);
    bookBranch("phistarZ1", BranchTypes::bFloat, doSetAddress);

    bookBranch("Lep1Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep2Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep3Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep4Mass", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep1Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep2Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep3Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep4Pt", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep1Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep2Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep3Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep4Eta", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep1Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep2Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep3Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep4Phi", BranchTypes::bFloat, doSetAddress);
    bookBranch("Lep1Id", BranchTypes::bInt, doSetAddress);
    bookBranch("Lep2Id", BranchTypes::bInt, doSetAddress);
    bookBranch("Lep3Id", BranchTypes::bInt, doSetAddress);
    bookBranch("Lep4Id", BranchTypes::bInt, doSetAddress);
  }

  actuateBranches(doSetAddress);
}

void HVVTree::fillMotherInfo(Particle* mother){
  if (options->processGenInfo() && mother!=0){
    setVal("GenMotherMass", mother->m());
    setVal("GenMotherPt", mother->pt());
    setVal("GenMotherPz", mother->z());
    setVal("GenMotherPhi", mother->phi());
    setVal("GenMotherId", mother->id);
  }
}


void HVVTree::fillCandidate(ZZCandidate* pH, bool isGen){
  if ((!options->processGenInfo() && isGen) || (!options->processRecoInfo() && !isGen)) return;

  string varname;
  string strcore = "ZZ";
  if (isGen) strcore = "GenH";

  varname = strcore + "Mass"; setVal(varname, (pH!=0 ? pH->m() : 0.));
  varname = strcore + "Pt"; setVal(varname, (pH!=0 ? pH->pt() : 0));
  varname = strcore + "Pz"; setVal(varname, (pH!=0 ? pH->z() : 0));
  varname = strcore + "Phi"; setVal(varname, (pH!=0 ? pH->phi() : 0));

  fillCandidateDaughters(pH, isGen);
  fillDaughterProducts(pH, isGen);
  fillAssociatedInfo(pH, isGen);
  fillDecayAngles(pH, isGen);
//  fillProductionAngles(pH, isGen);
}
void HVVTree::fillCandidateDaughters(ZZCandidate* pH, bool isGen){
  string varname;
  string strcore;

  Particle* pV1=(pH!=0 ? pH->getSortedV(0) : 0);
  Particle* pV2=(pH!=0 ? pH->getSortedV(1) : 0);

  strcore = "Z1";
  if (isGen) strcore.insert(0, "Gen");
  varname = strcore + "Mass"; setVal(varname, (pV1!=0 ? pV1->m() : 0));
  varname = strcore + "Pt"; setVal(varname, (pV1!=0 ? pV1->pt() : 0));
  varname = strcore + "Eta"; setVal(varname, (pV1!=0 ? pV1->eta() : 0));
  varname = strcore + "Phi"; setVal(varname, (pV1!=0 ? pV1->phi() : 0));
  strcore = "Z2";
  if (isGen) strcore.insert(0, "Gen");
  varname = strcore + "Mass"; setVal(varname, (pV2!=0 ? pV2->m() : 0));
  varname = strcore + "Pt"; setVal(varname, (pV2!=0 ? pV2->pt() : 0));
  varname = strcore + "Eta"; setVal(varname, (pV2!=0 ? pV2->eta() : 0));
  varname = strcore + "Phi"; setVal(varname, (pV2!=0 ? pV2->phi() : 0));

  TLorentzVector nullVector(0, 0, 0, 0);
  TLorentzVector pZ1alt = (pH!=0 ? pH->getAlternativeVMomentum(0) : nullVector);
  TLorentzVector pZ2alt = (pH!=0 ? pH->getAlternativeVMomentum(1) : nullVector);

  strcore = "Za";
  if (isGen) strcore.insert(0, "Gen");
  varname = strcore + "Mass"; setVal(varname, (pH!=0 ? pZ1alt.M() : 0));
  varname = strcore + "Pt"; setVal(varname, (pH!=0 ? pZ1alt.Pt() : 0));
  varname = strcore + "Eta"; setVal(varname, (pH!=0 ? pZ1alt.Eta() : 0));
  varname = strcore + "Phi"; setVal(varname, (pH!=0 ? pZ1alt.Phi() : 0));
  strcore = "Zb";
  if (isGen) strcore.insert(0, "Gen");
  varname = strcore + "Mass"; setVal(varname, (pH!=0 ? pZ2alt.M() : 0));
  varname = strcore + "Pt"; setVal(varname, (pH!=0 ? pZ2alt.Pt() : 0));
  varname = strcore + "Eta"; setVal(varname, (pH!=0 ? pZ2alt.Eta() : 0));
  varname = strcore + "Phi"; setVal(varname, (pH!=0 ? pZ2alt.Phi() : 0));
}
void HVVTree::fillDaughterProducts(ZZCandidate* pH, bool isGen){
  string varname;
  string strcore = "Lep";
  if (isGen) strcore.insert(0, "Gen");

  for (int v=0; v<2; v++){
    for (int d=0; d<2; d++){
      int iLep = 2*v+d+1;
      char cILep[2];
      sprintf(cILep, "%i", iLep);
      string strILep = string(cILep);

      Particle* lepton = (pH!=0 ? pH->getSortedV(v)->getDaughter(d) : 0);

      varname = strcore + strILep + "Mass"; setVal(varname, (lepton!=0 ? lepton->m() : 0));
      varname = strcore + strILep + "Pt"; setVal(varname, (lepton!=0 ? lepton->pt() : 0));
      varname = strcore + strILep + "Eta"; setVal(varname, (lepton!=0 ? lepton->eta() : 0));
      varname = strcore + strILep + "Phi"; setVal(varname, (lepton!=0 ? lepton->phi() : 0));
      varname = strcore + strILep + "Id"; setVal(varname, (lepton!=0 ? lepton->id : 0));
    }
  }

  if (isGen){
    Int_t genFinalState=-1;
    if (pH!=0){
      if (PDGHelpers::isAZBoson(pH->getSortedV(0)->id) && PDGHelpers::isAZBoson(pH->getSortedV(1)->id)){
        // 4l
        if (std::abs(pH->getSortedV(0)->getDaughter(0)->id)==13 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==13) genFinalState=0; // 4mu
        else if (std::abs(pH->getSortedV(0)->getDaughter(0)->id)==11 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==11) genFinalState=1; // 4e
        else if ((std::abs(pH->getSortedV(0)->getDaughter(0)->id)==11 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==13) || (std::abs(pH->getSortedV(0)->getDaughter(0)->id)==13 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==11)) genFinalState=2; // 2e2mu
        else if (std::abs(pH->getSortedV(0)->getDaughter(0)->id)==15 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==15) genFinalState=3; // 4tau
        else if ((std::abs(pH->getSortedV(0)->getDaughter(0)->id)==15 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==13) || (std::abs(pH->getSortedV(0)->getDaughter(0)->id)==13 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==15)) genFinalState=4; // 2mu2tau
        else if ((std::abs(pH->getSortedV(0)->getDaughter(0)->id)==11 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==15) || (std::abs(pH->getSortedV(0)->getDaughter(0)->id)==15 && std::abs(pH->getSortedV(1)->getDaughter(0)->id)==11)) genFinalState=5; // 2e2tau
        // 4nu, 4q
        else if (std::abs(pH->getSortedV(0)->getDaughter(0)->id)==std::abs(pH->getSortedV(1)->getDaughter(0)->id)) genFinalState=0;
        else genFinalState=2;
      }
      else genFinalState=2; // WW has no interference
    }
    setVal("genFinalState", genFinalState);
  }
}
void HVVTree::fillAssociatedInfo(ZZCandidate* pH, bool isGen){
  vector<Particle*> AssociatedParticle;
  vector<Particle*> tmpAssociatedParticle;
  Float_t DijetMass=-1;

  Int_t NAssociatedVs=0;
  vector<Particle*> AssociatedV;
  vector<int> AssociatedV_Particle1Index;
  vector<int> AssociatedV_Particle2Index;

  if (pH!=0){
    for (int aa=0; aa<pH->getNAssociatedJets(); aa++){
      Particle* apart = pH->getAssociatedJet(aa);
      tmpAssociatedParticle.push_back(apart);
    }
    for (int aa=0; aa<pH->getNAssociatedLeptons(); aa++){
      Particle* apart = pH->getAssociatedLepton(aa);
      tmpAssociatedParticle.push_back(apart);
    }
    for (int aa=0; aa<pH->getNAssociatedNeutrinos(); aa++){
      Particle* apart = pH->getAssociatedNeutrino(aa);
      tmpAssociatedParticle.push_back(apart);
    }
  }

  while (tmpAssociatedParticle.size()>0){ // Re-sort all associated particles by leading pT (categories are individually soreted, but mixing categories loses this sorting)
    Particle* tmpPart=0;
    int pos=0;
    for (int el=0; el<tmpAssociatedParticle.size(); el++){
      if (tmpPart==0){
        tmpPart = tmpAssociatedParticle.at(el); pos=el;
      }
      else if (tmpPart->pt()<tmpAssociatedParticle.at(el)->pt()){
        tmpPart = tmpAssociatedParticle.at(el); pos=el;
      }// Safer to do in two steps
    }
    AssociatedParticle.push_back(tmpPart);
    tmpAssociatedParticle.erase(tmpAssociatedParticle.begin()+pos);
  }

  NAssociatedVs = (pH!=0 ? pH->getNSortedVs()-2 : 0);
  for (int av=2; av<NAssociatedVs+2; av++){
    Particle* pAV = pH->getSortedV(av);
    AssociatedV.push_back(pAV);
    Particle* avd1 = pAV->getDaughter(0);
    Particle* avd2 = pAV->getDaughter(1);

    for (int aa=0; aa<AssociatedParticle.size(); aa++){
      if (avd1==AssociatedParticle.at(aa)) AssociatedV_Particle1Index.push_back(aa);
      else if (avd2==AssociatedParticle.at(aa)) AssociatedV_Particle2Index.push_back(aa);
    }
  }

  string varname;
  string strcore;

  if (pH->getNAssociatedJets()>1){
    DijetMass = (pH->getAssociatedJet(0)->p4+pH->getAssociatedJet(1)->p4).M();
    varname = "DijetMass";
    if (isGen) varname.insert(0, "Gen");
    setVal(varname, DijetMass);
  }

  varname = "NAssociatedVs";
  if (isGen) varname.insert(0, "Gen");
  setVal(varname, NAssociatedVs);

  strcore = "AssociatedParticle";
  if (isGen) strcore.insert(0, "Gen");
  for (int aa=0; aa<AssociatedParticle.size(); aa++){
    varname = strcore + "Mass"; setVal(varname, AssociatedParticle.at(aa)->m());
    varname = strcore + "Pt"; setVal(varname, AssociatedParticle.at(aa)->pt());
    varname = strcore + "Eta"; setVal(varname, AssociatedParticle.at(aa)->eta());
    varname = strcore + "Phi"; setVal(varname, AssociatedParticle.at(aa)->phi());
    varname = strcore + "Id"; setVal(varname, AssociatedParticle.at(aa)->id);
  }

  strcore = "AssociatedV";
  if (isGen) strcore.insert(0, "Gen");
  for (int aa=0; aa<NAssociatedVs; aa++){
    varname = strcore + "Mass"; setVal(varname, AssociatedV.at(aa)->m());
    varname = strcore + "Pt"; setVal(varname, AssociatedV.at(aa)->pt());
    varname = strcore + "Eta"; setVal(varname, AssociatedV.at(aa)->eta());
    varname = strcore + "Phi"; setVal(varname, AssociatedV.at(aa)->phi());
    varname = strcore + "Id"; setVal(varname, AssociatedV.at(aa)->id);
    varname = strcore + "_Particle1Index"; setVal(varname, AssociatedV_Particle1Index.at(aa));
    varname = strcore + "_Particle2Index"; setVal(varname, AssociatedV_Particle2Index.at(aa));
  }
}

void HVVTree::fillDecayAngles(ZZCandidate* pH, bool isGen){
  Float_t helcosthetaZ1=0, helcosthetaZ2=0, helphi=0, costhetastar=0, phistarZ1=0;

  mela::applyLeptonMassCorrection(options->doRemoveLepMasses());
  if (pH!=0) mela::computeAngles(
    pH->getSortedV(0)->getDaughter(0)->p4, pH->getSortedV(0)->getDaughter(0)->id,
    pH->getSortedV(0)->getDaughter(1)->p4, pH->getSortedV(0)->getDaughter(1)->id,
    pH->getSortedV(1)->getDaughter(0)->p4, pH->getSortedV(1)->getDaughter(0)->id,
    pH->getSortedV(1)->getDaughter(1)->p4, pH->getSortedV(1)->getDaughter(1)->id,
    costhetastar, helcosthetaZ1, helcosthetaZ2, helphi, phistarZ1
    );
  // Protect against NaN
  if (!(costhetastar==costhetastar)) costhetastar=0;
  if (!(helcosthetaZ1==helcosthetaZ1)) helcosthetaZ1=0;
  if (!(helcosthetaZ2==helcosthetaZ2)) helcosthetaZ2=0;
  if (!(helphi==helphi)) helphi=0;
  if (!(phistarZ1==phistarZ1)) phistarZ1=0;

  string varname;
  varname = "helcosthetaZ1"; if (isGen) varname.insert(0, "Gen"); setVal(varname, helcosthetaZ1);
  varname = "helcosthetaZ2"; if (isGen) varname.insert(0, "Gen"); setVal(varname, helcosthetaZ2);
  varname = "helphi"; if (isGen) varname.insert(0, "Gen"); setVal(varname, helphi);
  varname = "costhetastar"; if (isGen) varname.insert(0, "Gen"); setVal(varname, costhetastar);
  varname = "phistarZ1"; if (isGen) varname.insert(0, "Gen"); setVal(varname, phistarZ1);
}
//  void HVVTree::fillProductionAngles(Particle* pH, bool isGen=false);

void HVVTree::fillEventVariables(Float_t weight, Int_t passSelection){
  setVal("MC_weight", weight);
  if (options->processRecoInfo()) setVal("isSelected", passSelection);
}

