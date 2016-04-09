#ifdef _def_melatools_
#include <ZZMatrixElement/MELA/interface/ScalarPdfFactory.h>
#else
#include "../include/ScalarPdfFactory.h"
#endif


ScalarPdfFactory::ScalarPdfFactory(RooSpin::modelMeasurables measurables_, bool acceptance_, RooSpin::VdecayType V1decay_, RooSpin::VdecayType V2decay_) :
parameterization(0),
pmf_applied(false),
acceptance(acceptance_),
V1decay(V1decay_),
V2decay(V2decay_)
{
  initMeasurables(measurables_);
  initMassPole();
  initVdecayParams();
  initGVals();
}
ScalarPdfFactory::ScalarPdfFactory(RooSpin::modelMeasurables measurables_, double gRatio_[4][8], double gZGsRatio_[4][1], double gGsGsRatio_[3][1], bool pmf_applied_, bool acceptance_, RooSpin::VdecayType V1decay_, RooSpin::VdecayType V2decay_) :
parameterization(1),
pmf_applied(pmf_applied_),
acceptance(acceptance_),
V1decay(V1decay_),
V2decay(V2decay_)
{
  for (int v=0; v<4; v++){
    for (int k=0; k<8; k++){
      gRatio[v][k] = gRatio_[v][k];
      if (k==0){
        gZGsRatio[v][k] = gZGsRatio_[v][k];
        if (v>0) gGsGsRatio[v-1][k] = gGsGsRatio_[v-1][k];
      }
    }
  }
  initMeasurables(measurables_);
  initMassPole();
  initVdecayParams();
  initGVals();
}

ScalarPdfFactory::~ScalarPdfFactory(){
  destroyGVals();
  destroyVdecayParams();
  destroyMassPole();
}
void ScalarPdfFactory::initMeasurables(RooSpin::modelMeasurables measurables_){
  measurables.h1 = (RooAbsReal*)measurables_.h1;
  measurables.h2 = (RooAbsReal*)measurables_.h2;
  measurables.Phi = (RooAbsReal*)measurables_.Phi;
  measurables.m1 = (RooAbsReal*)measurables_.m1;
  measurables.m2 = (RooAbsReal*)measurables_.m2;
  measurables.m12 = (RooAbsReal*)measurables_.m12;
  measurables.hs = (RooAbsReal*)measurables_.hs;
  measurables.Phi1 = (RooAbsReal*)measurables_.Phi1;
  measurables.Y = (RooAbsReal*)measurables_.Y;
}
void ScalarPdfFactory::initMassPole(){
  parameters.mX = new RooRealVar("mX", "mX", (measurables.m12)->getVal());
  parameters.gamX = new RooRealVar("gamX", "gamX", 0);
}
void ScalarPdfFactory::initVdecayParams(){
  if ((((int)V1decay)>0 && ((int)V2decay)<0) || (((int)V1decay)<0 && ((int)V2decay)>0)) cerr << "ScalarPdfFactory::initVdecayParams: V1 and V2 decays are inconsistent!" << endl;
  if (V1decay==RooSpin::kVdecayType_GammaOnshell){
    parameters.mV = new RooRealVar("mV", "mV", 0);
    parameters.gamV = new RooRealVar("gamV", "gamV", 0);
  }
  else if (V1decay==RooSpin::kVdecayType_Wany){
    parameters.mV = new RooRealVar("mV", "mV", 80.399);
    parameters.gamV = new RooRealVar("gamV", "gamV", 2.085);
  }
  else{
    parameters.mV = new RooRealVar("mV", "mV", 91.1876);
    parameters.gamV = new RooRealVar("gamV", "gamV", 2.4952);
  }
  parameters.R1Val = new RooRealVar("R1Val", "R1Val", getRValue(V1decay));
  parameters.R2Val = new RooRealVar("R2Val", "R2Val", getRValue(V2decay));
 
  ((RooRealVar*)parameters.mV)->removeRange();
  ((RooRealVar*)parameters.gamV)->removeRange();
  ((RooRealVar*)parameters.R1Val)->removeRange();
  ((RooRealVar*)parameters.R2Val)->removeRange();
}
void ScalarPdfFactory::resetVdecay(RooSpin::VdecayType V1decay_, RooSpin::VdecayType V2decay_){
  if ((((int)V1decay)>0 && ((int)V2decay)<0) || (((int)V1decay)<0 && ((int)V2decay)>0)) cerr << "ScalarPdfFactory::resetVdecay: V1 and V2 decays are inconsistent!" << endl;

  V1decay=V1decay_;
  V2decay=V2decay_;
  PDF_base->setDecayModes(V1decay, V2decay);

  bool is_mvconst = ((RooRealVar*)parameters.mV)->isConstant();
  bool is_gamvconst = ((RooRealVar*)parameters.gamV)->isConstant();
  bool is_r1const = ((RooRealVar*)parameters.R1Val)->isConstant();
  bool is_r2const = ((RooRealVar*)parameters.R2Val)->isConstant();
  ((RooRealVar*)parameters.mV)->setConstant(false);
  ((RooRealVar*)parameters.gamV)->setConstant(false);
  ((RooRealVar*)parameters.R1Val)->setConstant(false);
  ((RooRealVar*)parameters.R2Val)->setConstant(false);

  if (V1decay_==RooSpin::kVdecayType_GammaOnshell){
    ((RooRealVar*)parameters.mV)->setVal(0);
    ((RooRealVar*)parameters.gamV)->setVal(0);
  }
  else if (V1decay_==RooSpin::kVdecayType_Wany){
    ((RooRealVar*)parameters.mV)->setVal(80.399);
    ((RooRealVar*)parameters.gamV)->setVal(2.085);
  }
  else{
    ((RooRealVar*)parameters.mV)->setVal(91.1876);
    ((RooRealVar*)parameters.gamV)->setVal(2.4952);
  }

  ((RooRealVar*)parameters.R1Val)->setVal(getRValue(V1decay));
  ((RooRealVar*)parameters.R2Val)->setVal(getRValue(V2decay));

  ((RooRealVar*)parameters.mV)->setConstant(is_mvconst);
  ((RooRealVar*)parameters.gamV)->setConstant(is_gamvconst);
  ((RooRealVar*)parameters.R1Val)->setConstant(is_r1const);
  ((RooRealVar*)parameters.R2Val)->setConstant(is_r2const);
}
double ScalarPdfFactory::getRValue(RooSpin::VdecayType Vdecay){
  double atomicT3 = 0.5;
  double atomicCharge = 1.;
  double sin2t = 0.23119;

  // gV = T3 - 2*Qf*sintW**2
  double gV_up = atomicT3 - 2.*(2.*atomicCharge/3.)*sin2t;
  double gV_dn = -atomicT3 - 2.*(-atomicCharge/3.)*sin2t;
  double gV_l = -atomicT3 - 2.*(-atomicCharge)*sin2t;
  double gV_nu = atomicT3;

  // gA = T3
  double gA_up = atomicT3;
  double gA_dn = -atomicT3;
  double gA_l = -atomicT3;
  double gA_nu = atomicT3;

  double Rval;
  switch (Vdecay){
  case RooSpin::kVdecayType_Zud:
    Rval = (2.*2.*gV_up*gA_up+3.*2.*gV_dn*gA_dn)/(2.*(pow(gV_up, 2)+pow(gA_up, 2))+3.*(pow(gV_dn, 2)+pow(gA_dn, 2))); // Z->uu+dd avg.
    break;
  case RooSpin::kVdecayType_Zdd:
    Rval = (2.*gV_dn*gA_dn)/(pow(gV_dn, 2)+pow(gA_dn, 2)); // Z->dd
    break;
  case RooSpin::kVdecayType_Zuu:
    Rval = (2.*gV_up*gA_up)/(pow(gV_up, 2)+pow(gA_up, 2)); // Z->uu
    break;
  case RooSpin::kVdecayType_Znn:
    Rval = (2.*gV_nu*gA_nu)/(pow(gV_nu, 2)+pow(gA_nu, 2)); // Z->nunu
    break;
  case RooSpin::kVdecayType_Zll:
    Rval = (2.*gV_l*gA_l)/(pow(gV_l, 2)+pow(gA_l, 2)); // Z->ll
    break;
  case RooSpin::kVdecayType_Wany:
    Rval = 1; // W
    break;
  default:
    Rval = 0; // gamma
    break;
  }
  return Rval;
}

void ScalarPdfFactory::initFractionsPhases(){
  for (int v=0; v<4; v++){
    for (int k=0; k<8; k++){
      gRatioVal[v][k] = new RooRealVar(Form("g%i_%iMixVal", v+1, k), Form("g%i_%iMixVal", v+1, k), gRatio[v][k]);

      // ZG, GG
      if (k==0){
        // Special case for ZG ghzgs1_prime2!!!
        if (v==0) gZGsRatioVal[v][k] = new RooRealVar(Form("gzgs%i_%iMixVal", v+1, k+2), Form("gzgs%i_%iMixVal", v+1, k+2), gZGsRatio[v][k]);
        else gZGsRatioVal[v][k] = new RooRealVar(Form("gzgs%i_%iMixVal", v+1, k), Form("gzgs%i_%iMixVal", v+1, k), gZGsRatio[v][k]);
        if (v>0) gGsGsRatioVal[v-1][k] = new RooRealVar(Form("ggsgs%i_%iMixVal", v+1, k), Form("ggsgs%i_%iMixVal", v+1, k), gGsGsRatio[v-1][k]);
      }

    }
  }

  RooArgList gBareFracList;
  for (int v=0; v<8; v++){
    TString strcore;
    TString strapp;
    if (v>1) strapp.Prepend(Form("%i", v));
    if (v>0) strapp.Prepend("_prime");
    TString strappPhase = strapp;
    strapp.Append("Frac");
    strappPhase.Append("Phase");

    double firstFracVal = 0;
    double phaseBound = TMath::Pi();
    if (pmf_applied){
      firstFracVal = -1;
      phaseBound = 0;
    }

    if (v>0){
      strcore = "g1";
      strcore.Append(strapp);
      if (gRatio[0][v]!=0) g1Frac[v-1] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
      else g1Frac[v-1] = new RooRealVar(strcore, strcore, 0);
      gBareFracList.add(*(g1Frac[v-1]));

      if (v==2){ // ghzgs_prime2
        strcore = "gzgs1";
        strcore.Append(strapp);
        if (gZGsRatioVal[0][v-2]!=0) gzgs1Frac[v-2] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
        else gzgs1Frac[v-2] = new RooRealVar(strcore, strcore, 0);
        gBareFracList.add(*(gzgs1Frac[v-2]));
      }
    }

    strcore = "g2";
    strcore.Append(strapp);
    if (gRatio[1][v]!=0) g2Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
    else g2Frac[v] = new RooRealVar(strcore, strcore, 0);
    gBareFracList.add(*(g2Frac[v]));
    if (v==0){ // ghzgs2 and ghgsgs2
      strcore = "gzgs2";
      strcore.Append(strapp);
      if (gZGsRatioVal[1][v]!=0) gzgs2Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
      else gzgs2Frac[v] = new RooRealVar(strcore, strcore, 0);
      gBareFracList.add(*(gzgs2Frac[v]));
      strcore = "ggsgs2";
      strcore.Append(strapp);
      if (gGsGsRatioVal[0][v]!=0) ggsgs2Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
      else ggsgs2Frac[v] = new RooRealVar(strcore, strcore, 0);
      gBareFracList.add(*(ggsgs2Frac[v]));
    }

    strcore = "g3";
    strcore.Append(strapp);
    if (gRatio[2][v]!=0) g3Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
    else g3Frac[v] = new RooRealVar(strcore, strcore, 0);
    gBareFracList.add(*(g3Frac[v]));
    if (v==0){ // ghzgs3 and ghgsgs3
      strcore = "gzgs3";
      strcore.Append(strapp);
      if (gZGsRatioVal[2][v]!=0) gzgs3Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
      else gzgs3Frac[v] = new RooRealVar(strcore, strcore, 0);
      gBareFracList.add(*(gzgs3Frac[v]));
      strcore = "ggsgs3";
      strcore.Append(strapp);
      if (gGsGsRatioVal[1][v]!=0) ggsgs3Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
      else ggsgs3Frac[v] = new RooRealVar(strcore, strcore, 0);
      gBareFracList.add(*(ggsgs3Frac[v]));
    }

    strcore = "g4";
    strcore.Append(strapp);
    if (gRatio[3][v]!=0) g4Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
    else g4Frac[v] = new RooRealVar(strcore, strcore, 0);
    gBareFracList.add(*(g4Frac[v]));
    if (v==0){ // ghzgs4 and ghgsgs4
      strcore = "gzgs4";
      strcore.Append(strapp);
      if (gZGsRatioVal[3][v]!=0) gzgs4Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
      else gzgs4Frac[v] = new RooRealVar(strcore, strcore, 0);
      gBareFracList.add(*(gzgs4Frac[v]));
      strcore = "ggsgs4";
      strcore.Append(strapp);
      if (gGsGsRatioVal[2][v]!=0) ggsgs4Frac[v] = new RooRealVar(strcore, strcore, 0, firstFracVal, 1);
      else ggsgs4Frac[v] = new RooRealVar(strcore, strcore, 0);
      gBareFracList.add(*(ggsgs4Frac[v]));
    }

    if (v>0){
      strcore = "g1";
      strcore.Append(strappPhase);
      if (gRatio[0][v]!=0) g1Phase[v-1] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
      else g1Phase[v-1] = new RooRealVar(strcore, strcore, 0);

      if (v==2){ // ghzgs_prime2
        strcore = "gzgs1";
        strcore.Append(strappPhase);
        if (gZGsRatioVal[0][v-2]!=0) gzgs1Phase[v-2] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
        else gzgs1Phase[v-2] = new RooRealVar(strcore, strcore, 0);
      }
    }

    strcore = "g2";
    strcore.Append(strappPhase);
    if (gRatio[1][v]!=0) g2Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
    else g2Phase[v] = new RooRealVar(strcore, strcore, 0);
    if (v==0){ // ghzgs2 and ghgsgs2
      strcore = "gzgs2";
      strcore.Append(strappPhase);
      if (gZGsRatioVal[1][v]!=0) gzgs2Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
      else gzgs2Phase[v] = new RooRealVar(strcore, strcore, 0);
      strcore = "ggsgs2";
      strcore.Append(strappPhase);
      if (gGsGsRatioVal[0][v]!=0) ggsgs2Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
      else ggsgs2Phase[v] = new RooRealVar(strcore, strcore, 0);
    }

    strcore = "g3";
    strcore.Append(strappPhase);
    if (gRatio[2][v]!=0) g3Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
    else g3Phase[v] = new RooRealVar(strcore, strcore, 0);
    if (v==0){ // ghzgs3 and ghgsgs3
      strcore = "gzgs3";
      strcore.Append(strappPhase);
      if (gZGsRatioVal[2][v]!=0) gzgs3Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
      else gzgs3Phase[v] = new RooRealVar(strcore, strcore, 0);
      strcore = "ggsgs3";
      strcore.Append(strappPhase);
      if (gGsGsRatioVal[1][v]!=0) ggsgs3Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
      else ggsgs3Phase[v] = new RooRealVar(strcore, strcore, 0);
    }

    strcore = "g4";
    strcore.Append(strappPhase);
    if (gRatio[3][v]!=0) g4Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
    else g4Phase[v] = new RooRealVar(strcore, strcore, 0);
    if (v==0){ // ghzgs4 and ghgsgs4
      strcore = "gzgs4";
      strcore.Append(strappPhase);
      if (gZGsRatioVal[3][v]!=0) gzgs4Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
      else gzgs4Phase[v] = new RooRealVar(strcore, strcore, 0);
      strcore = "ggsgs4";
      strcore.Append(strappPhase);
      if (gGsGsRatioVal[2][v]!=0) ggsgs4Phase[v] = new RooRealVar(strcore, strcore, 0, -phaseBound, phaseBound);
      else ggsgs4Phase[v] = new RooRealVar(strcore, strcore, 0);
    }
  }
  TString sumFormula = "(";
  for (int gg=0; gg<gBareFracList.getSize(); gg++){
    TString bareFormula = "abs(@";
    bareFormula.Append(Form("%i", gg));
    bareFormula.Append(")");
    if (gg!=(gBareFracList.getSize()-1)) bareFormula.Append("+");
    sumFormula.Append(bareFormula);
  }
  sumFormula.Append(")");
  TString gFracSumName = "sum_gjAbsFrac";
  gFracSum = new RooFormulaVar(gFracSumName, gFracSumName, sumFormula.Data(), gBareFracList);
  for (int v=0; v<8; v++){
    TString strcore;
    TString strapp;
    if (v>1) strapp.Prepend(Form("%i", v));
    if (v>0) strapp.Prepend("_prime");
    strapp.Append("InterpFrac");

    if (v>0){
      strcore = "g1";
      strcore.Append(strapp);
      RooArgList tmpArg1(*(g1Frac[v-1]), *gFracSum);
      g1FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg1);

      if (v==2){ // ghzgs_prime2
        strcore = "gzgs1";
        strcore.Append(strapp);
        RooArgList tmpArg_zgs1(*(gzgs1Frac[v-2]), *gFracSum);
        gzgs1FracInterp[v-2] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg_zgs1);
      }
    }
    else{
      strcore = "g1";
      strcore.Append(strapp);
      RooArgList tmpGArg(*gFracSum);
      g1FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@0>1 ? 0. : abs(1.-@0))", tmpGArg);
    }

    strcore = "g2";
    strcore.Append(strapp);
    RooArgList tmpArg2(*(g2Frac[v]), *gFracSum);
    g2FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg2);
    if (v==0){ // ghzgs2 and ghgsgs2
      strcore = "gzgs2";
      strcore.Append(strapp);
      RooArgList tmpArg_zgs2(*(gzgs2Frac[v]), *gFracSum);
      gzgs2FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg_zgs2);

      strcore = "ggsgs2";
      strcore.Append(strapp);
      RooArgList tmpArg_gsgs2(*(ggsgs2Frac[v]), *gFracSum);
      ggsgs2FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg_gsgs2);
    }

    strcore = "g3";
    strcore.Append(strapp);
    RooArgList tmpArg3(*(g3Frac[v]), *gFracSum);
    g3FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg3);
    if (v==0){ // ghzgs3 and ghgsgs3
      strcore = "gzgs3";
      strcore.Append(strapp);
      RooArgList tmpArg_zgs3(*(gzgs3Frac[v]), *gFracSum);
      gzgs3FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg_zgs3);

      strcore = "ggsgs3";
      strcore.Append(strapp);
      RooArgList tmpArg_gsgs3(*(ggsgs3Frac[v]), *gFracSum);
      ggsgs3FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg_gsgs3);
    }

    strcore = "g4";
    strcore.Append(strapp);
    RooArgList tmpArg4(*(g4Frac[v]), *gFracSum);
    g4FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg4);
    if (v==0){ // ghzgs4 and ghgsgs4
      strcore = "gzgs4";
      strcore.Append(strapp);
      RooArgList tmpArg_zgs4(*(gzgs4Frac[v]), *gFracSum);
      gzgs4FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg_zgs4);

      strcore = "ggsgs4";
      strcore.Append(strapp);
      RooArgList tmpArg_gsgs4(*(ggsgs4Frac[v]), *gFracSum);
      ggsgs4FracInterp[v] = new RooFormulaVar(strcore, strcore, "(@1>1 ? 0. : @0)", tmpArg_gsgs4);
    }
  }
  for (int v=0; v<8; v++){
    for (int im=0; im<2; im++){
      TString strcore;
      TString strapp = "Val";
      if (im==1) strapp.Append("Im");
      if (v>1) strapp.Prepend(Form("%i", v));
      if (v>0) strapp.Prepend("_prime");

      strcore = "g1";
      strcore.Append(strapp);
      RooArgList tmpArg1(*(gRatioVal[0][v]), *(g1FracInterp[v]));
      TString strFormulag1 = "@0*sqrt(@1)";
      if (v>0){
        tmpArg1.add(*(g1Phase[v-1]));
        if (im==0) strFormulag1.Append("*cos(@2)");
        else strFormulag1.Append("*sin(@2)");
      }
      RooFormulaVar* g1Val = new RooFormulaVar(strcore, strcore, strFormulag1, tmpArg1);
      couplings.g1List[v][im] = (RooAbsReal*)g1Val;

      strcore = "g2";
      strcore.Append(strapp);
      RooArgList tmpArg2(*(gRatioVal[1][v]), *(g2FracInterp[v]), *(g2Phase[v]));
      TString strFormulag2 = "@0*sqrt(@1)";
      if (im==0) strFormulag2.Append("*cos(@2)");
      else strFormulag2.Append("*sin(@2)");
      RooFormulaVar* g2Val = new RooFormulaVar(strcore, strcore, strFormulag2, tmpArg2);
      couplings.g2List[v][im] = (RooAbsReal*)g2Val;

      strcore = "g3";
      strcore.Append(strapp);
      RooArgList tmpArg3(*(gRatioVal[2][v]), *(g3FracInterp[v]), *(g3Phase[v]));
      TString strFormulag3 = "@0*sqrt(@1)";
      if (im==0) strFormulag3.Append("*cos(@2)");
      else strFormulag3.Append("*sin(@2)");
      RooFormulaVar* g3Val = new RooFormulaVar(strcore, strcore, strFormulag3, tmpArg3);
      couplings.g3List[v][im] = (RooAbsReal*)g3Val;

      strcore = "g4";
      strcore.Append(strapp);
      RooArgList tmpArg4(*(gRatioVal[3][v]), *(g4FracInterp[v]), *(g4Phase[v]));
      TString strFormulag4 = "@0*sqrt(@1)";
      if (im==0) strFormulag4.Append("*cos(@2)");
      else strFormulag4.Append("*sin(@2)");
      RooFormulaVar* g4Val = new RooFormulaVar(strcore, strcore, strFormulag4, tmpArg4);
      couplings.g4List[v][im] = (RooAbsReal*)g4Val;

      // Z+gamma*
      if (v==2){
        strcore = "gzgs1";
        strcore.Append(strapp);
        RooArgList tmpArg_zgs1(*(gZGsRatioVal[0][v-2]), *(gzgs1FracInterp[v-2]), *(gzgs1Phase[v-2]));
        TString strFormulagzgs1 = "@0*sqrt(@1)";
        if (im==0) strFormulagzgs1.Append("*cos(@2)");
        else strFormulagzgs1.Append("*sin(@2)");
        RooFormulaVar* gzgs1Val = new RooFormulaVar(strcore, strcore, strFormulagzgs1, tmpArg_zgs1);
        couplings.gzgs1List[v-2][im] = (RooAbsReal*)gzgs1Val;
      }
      if (v==0){
        strcore = "gzgs2";
        strcore.Append(strapp);
        RooArgList tmpArg_zgs2(*(gZGsRatioVal[1][v]), *(gzgs2FracInterp[v]), *(gzgs2Phase[v]));
        TString strFormulagzgs2 = "@0*sqrt(@1)";
        if (im==0) strFormulagzgs2.Append("*cos(@2)");
        else strFormulagzgs2.Append("*sin(@2)");
        RooFormulaVar* gzgs2Val = new RooFormulaVar(strcore, strcore, strFormulagzgs2, tmpArg_zgs2);
        couplings.gzgs2List[v][im] = (RooAbsReal*)gzgs2Val;

        strcore = "gzgs3";
        strcore.Append(strapp);
        RooArgList tmpArg_zgs3(*(gZGsRatioVal[1][v]), *(gzgs3FracInterp[v]), *(gzgs3Phase[v]));
        TString strFormulagzgs3 = "@0*sqrt(@1)";
        if (im==0) strFormulagzgs3.Append("*cos(@2)");
        else strFormulagzgs3.Append("*sin(@2)");
        RooFormulaVar* gzgs3Val = new RooFormulaVar(strcore, strcore, strFormulagzgs3, tmpArg_zgs3);
        couplings.gzgs3List[v][im] = (RooAbsReal*)gzgs3Val;

        strcore = "gzgs4";
        strcore.Append(strapp);
        RooArgList tmpArg_zgs4(*(gZGsRatioVal[1][v]), *(gzgs4FracInterp[v]), *(gzgs4Phase[v]));
        TString strFormulagzgs4 = "@0*sqrt(@1)";
        if (im==0) strFormulagzgs4.Append("*cos(@2)");
        else strFormulagzgs4.Append("*sin(@2)");
        RooFormulaVar* gzgs4Val = new RooFormulaVar(strcore, strcore, strFormulagzgs4, tmpArg_zgs4);
        couplings.gzgs4List[v][im] = (RooAbsReal*)gzgs4Val;
      }

      if (v==0){
        strcore = "ggsgs2";
        strcore.Append(strapp);
        RooArgList tmpArg_gsgs2(*(gGsGsRatioVal[0][v]), *(ggsgs2FracInterp[v]), *(ggsgs2Phase[v]));
        TString strFormulaggsgs2 = "@0*sqrt(@1)";
        if (im==0) strFormulaggsgs2.Append("*cos(@2)");
        else strFormulaggsgs2.Append("*sin(@2)");
        RooFormulaVar* ggsgs2Val = new RooFormulaVar(strcore, strcore, strFormulaggsgs2, tmpArg_gsgs2);
        couplings.ggsgs2List[v][im] = (RooAbsReal*)ggsgs2Val;

        strcore = "ggsgs3";
        strcore.Append(strapp);
        RooArgList tmpArg_gsgs3(*(gGsGsRatioVal[1][v]), *(ggsgs3FracInterp[v]), *(ggsgs3Phase[v]));
        TString strFormulaggsgs3 = "@0*sqrt(@1)";
        if (im==0) strFormulaggsgs3.Append("*cos(@2)");
        else strFormulaggsgs3.Append("*sin(@2)");
        RooFormulaVar* ggsgs3Val = new RooFormulaVar(strcore, strcore, strFormulaggsgs3, tmpArg_gsgs3);
        couplings.ggsgs3List[v][im] = (RooAbsReal*)ggsgs3Val;

        strcore = "ggsgs4";
        strcore.Append(strapp);
        RooArgList tmpArg_gsgs4(*(gGsGsRatioVal[2][v]), *(ggsgs4FracInterp[v]), *(ggsgs4Phase[v]));
        TString strFormulaggsgs4 = "@0*sqrt(@1)";
        if (im==0) strFormulaggsgs4.Append("*cos(@2)");
        else strFormulaggsgs4.Append("*sin(@2)");
        RooFormulaVar* ggsgs4Val = new RooFormulaVar(strcore, strcore, strFormulaggsgs4, tmpArg_gsgs4);
        couplings.ggsgs4List[v][im] = (RooAbsReal*)ggsgs4Val;
      }

    }
  }
}
void ScalarPdfFactory::initGVals(){
  couplings.Lambda = new RooRealVar("Lambda", "Lambda", 1000.);
  couplings.Lambda_z1 = new RooRealVar("Lambda_z1", "Lambda_z1", 10000.);
  couplings.Lambda_z2 = new RooRealVar("Lambda_z2", "Lambda_z2", 10000.);
  couplings.Lambda_z3 = new RooRealVar("Lambda_z3", "Lambda_z3", 10000.);
  couplings.Lambda_z4 = new RooRealVar("Lambda_z4", "Lambda_z4", 10000.);
  couplings.Lambda_Q = new RooRealVar("Lambda_Q", "Lambda_Q", 10000.);
  couplings.Lambda_zgs1 = new RooRealVar("Lambda_zgs1", "Lambda_zgs1", 10000.);

  for (int v=0; v<3; v++){
    TString strcore;
    double initval = 100.;

    strcore = "Lambda_z1";
    strcore.Append(Form("%i", (v!=3 ? v+1 : 0)));
    couplings.Lambda_z1qsq[v] = new RooRealVar(strcore, strcore, initval, 0., 1e15);
    ((RooRealVar*)couplings.Lambda_z1qsq[v])->removeMax();
    strcore = "Lambda_z2";
    strcore.Append(Form("%i", (v!=3 ? v+1 : 0)));
    couplings.Lambda_z2qsq[v] = new RooRealVar(strcore, strcore, initval, 0., 1e15);
    ((RooRealVar*)couplings.Lambda_z2qsq[v])->removeMax();
    strcore = "Lambda_z3";
    strcore.Append(Form("%i", (v!=3 ? v+1 : 0)));
    couplings.Lambda_z3qsq[v] = new RooRealVar(strcore, strcore, initval, 0., 1e15);
    ((RooRealVar*)couplings.Lambda_z3qsq[v])->removeMax();
    strcore = "Lambda_z4";
    strcore.Append(Form("%i", (v!=3 ? v+1 : 0)));
    couplings.Lambda_z4qsq[v] = new RooRealVar(strcore, strcore, initval, 0., 1e15);
    ((RooRealVar*)couplings.Lambda_z4qsq[v])->removeMax();

    initval=0;
    strcore = "cz_q";
    strcore.Append(Form("%i%s", (v!=3 ? v+1 : 12),"sq"));
    couplings.cLambda_qsq[v] = new RooRealVar(strcore, strcore, initval, -1., 1.);
  }

  if (parameterization==0){
    for (int v=0; v<8; v++){
      for (int im=0; im<2; im++){
        TString strcore;
        double initval = 0;
        TString strapp = "Val";
        if (im==1) strapp.Append("Im");
        if (v>1) strapp.Prepend(Form("%i", v));
        if (v>0) strapp.Prepend("_prime");

        strcore = "g1";
        strcore.Append(strapp);
        if (v==0 && im==0) initval = 1;
        RooRealVar* g1Val = new RooRealVar(strcore, strcore, initval, -1e15, 1e15);
        g1Val->removeMin();
        g1Val->removeMax();
        couplings.g1List[v][im] = (RooAbsReal*)g1Val;

        strcore = "g2";
        strcore.Append(strapp);
        RooRealVar* g2Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
        g2Val->removeMin();
        g2Val->removeMax();
        couplings.g2List[v][im] = (RooAbsReal*)g2Val;

        strcore = "g3";
        strcore.Append(strapp);
        RooRealVar* g3Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
        g3Val->removeMin();
        g3Val->removeMax();
        couplings.g3List[v][im] = (RooAbsReal*)g3Val;

        strcore = "g4";
        strcore.Append(strapp);
        RooRealVar* g4Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
        g4Val->removeMin();
        g4Val->removeMax();
        couplings.g4List[v][im] = (RooAbsReal*)g4Val;

        if (v==2){
          strcore = "gzgs1";
          strcore.Append(strapp);
          RooRealVar* gzgs1Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
          gzgs1Val->removeMin();
          gzgs1Val->removeMax();
          couplings.gzgs1List[v-2][im] = (RooAbsReal*)gzgs1Val;
        }
        if(v==0){
          strcore = "gzgs2";
          strcore.Append(strapp);
          RooRealVar* gzgs2Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
          gzgs2Val->removeMin();
          gzgs2Val->removeMax();
          couplings.gzgs2List[v][im] = (RooAbsReal*)gzgs2Val;

          strcore = "gzgs3";
          strcore.Append(strapp);
          RooRealVar* gzgs3Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
          gzgs3Val->removeMin();
          gzgs3Val->removeMax();
          couplings.gzgs3List[v][im] = (RooAbsReal*)gzgs3Val;

          strcore = "gzgs4";
          strcore.Append(strapp);
          RooRealVar* gzgs4Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
          gzgs4Val->removeMin();
          gzgs4Val->removeMax();
          couplings.gzgs4List[v][im] = (RooAbsReal*)gzgs4Val;

          strcore = "ggsgs2";
          strcore.Append(strapp);
          RooRealVar* ggsgs2Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
          ggsgs2Val->removeMin();
          ggsgs2Val->removeMax();
          couplings.ggsgs2List[v][im] = (RooAbsReal*)ggsgs2Val;

          strcore = "ggsgs3";
          strcore.Append(strapp);
          RooRealVar* ggsgs3Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
          ggsgs3Val->removeMin();
          ggsgs3Val->removeMax();
          couplings.ggsgs3List[v][im] = (RooAbsReal*)ggsgs3Val;

          strcore = "ggsgs4";
          strcore.Append(strapp);
          RooRealVar* ggsgs4Val = new RooRealVar(strcore, strcore, 0, -1e15, 1e15);
          ggsgs4Val->removeMin();
          ggsgs4Val->removeMax();
          couplings.ggsgs4List[v][im] = (RooAbsReal*)ggsgs4Val;
        }
      }
    }
  }
  else initFractionsPhases();
}

void ScalarPdfFactory::destroyMassPole(){
  delete parameters.mX;
  delete parameters.gamX;
}
void ScalarPdfFactory::destroyVdecayParams(){
  delete parameters.R1Val;
  delete parameters.R2Val;
  delete parameters.mV;
  delete parameters.gamV;
}
void ScalarPdfFactory::destroyFractionsPhases(){
  for (int v=0; v<8; v++){
    delete g1FracInterp[v];
    delete g2FracInterp[v];
    delete g3FracInterp[v];
    delete g4FracInterp[v];
    if (v==2){
      delete gzgs1FracInterp[v-2];
    }
    if (v==0){
      delete gzgs2FracInterp[v];
      delete gzgs3FracInterp[v];
      delete gzgs4FracInterp[v];
      delete ggsgs2FracInterp[v];
      delete ggsgs3FracInterp[v];
      delete ggsgs4FracInterp[v];
    }
  }
  delete gFracSum;
  for (int v=0; v<8; v++){
    delete g2Frac[v];
    delete g3Frac[v];
    delete g4Frac[v];
    delete g2Phase[v];
    delete g3Phase[v];
    delete g4Phase[v];
    if (v>0){
      delete g1Frac[v-1];
      delete g1Phase[v-1];
    }

    if (v==2){
      delete gzgs1Frac[v-2];
      delete gzgs1Phase[v-2];
    }
    if (v==0){
      delete gzgs2Frac[v];
      delete gzgs3Frac[v];
      delete gzgs4Frac[v];
      delete ggsgs2Frac[v];
      delete ggsgs3Frac[v];
      delete ggsgs4Frac[v];
      delete gzgs2Phase[v];
      delete gzgs3Phase[v];
      delete gzgs4Phase[v];
      delete ggsgs2Phase[v];
      delete ggsgs3Phase[v];
      delete ggsgs4Phase[v];
    }
  }
  for (int gg=0; gg<4; gg++){
    for (int v=0; v<8; v++){
      delete gRatioVal[gg][v];
      if (v==0){
        delete gZGsRatioVal[gg][v];
        if (gg>0) delete gGsGsRatioVal[gg-1][v];
      }
    }
  }
}
void ScalarPdfFactory::destroyGVals(){
  for (int v=0; v<8; v++){
    for (int im=0; im<2; im++){
      delete couplings.g1List[v][im];
      delete couplings.g2List[v][im];
      delete couplings.g3List[v][im];
      delete couplings.g4List[v][im];

      if (v==2){
        delete couplings.gzgs1List[v-2][im];
      }
      if (v==0){
        delete couplings.gzgs2List[v][im];
        delete couplings.gzgs3List[v][im];
        delete couplings.gzgs4List[v][im];
        delete couplings.ggsgs2List[v][im];
        delete couplings.ggsgs3List[v][im];
        delete couplings.ggsgs4List[v][im];
      }
    }
  }
  if (parameterization!=0) destroyFractionsPhases();
  for (int v=0; v<3; v++){
    delete couplings.Lambda_z1qsq[v];
    delete couplings.Lambda_z2qsq[v];
    delete couplings.Lambda_z3qsq[v];
    delete couplings.Lambda_z4qsq[v];
    delete couplings.cLambda_qsq[v];
  }
  delete couplings.Lambda;
  delete couplings.Lambda_zgs1;
  delete couplings.Lambda_z1;
  delete couplings.Lambda_z2;
  delete couplings.Lambda_z3;
  delete couplings.Lambda_z4;
  delete couplings.Lambda_Q;
}

void ScalarPdfFactory::addHypothesis(int ig, int ilam, double iphase, double altparam_fracval){
  if ((ig==4 && ilam!=2) || (ig>4 && ilam!=0)){ cerr << "Invalid ZG/GG g" << ig << "_prime" << ilam << endl; return; }
  if (ig>=11 || ig<0){ cerr << "Invalid g" << ig << endl; return; }
  if (ilam>=8 || ilam<0){ cerr << "Out-of-range g" << ig << "_prime" << ilam << endl; return; }

  if (parameterization==0){
    // Good guesses of c-constants
    Double_t initval=1.;
    if (ilam>0){
      if (ig==0){ // g1_dyn
        if (ilam>=1 && ilam<=3) initval = pow(couplings.Lambda_z1->getVal()/parameters.mV->getVal(), 2);
        else if (ilam>=5 && ilam<=7) initval = pow(couplings.Lambda_z1->getVal()/parameters.mV->getVal(), 4);
        else if (ilam==4) initval = pow(couplings.Lambda_Q->getVal()/parameters.mX->getVal(), 2);
      }
      else if (ig==1){ // g2_dyn
        if (ilam>=1 && ilam<=3) initval = pow(couplings.Lambda_z2->getVal()/parameters.mV->getVal(), 2);
        else if (ilam>=5 && ilam<=7) initval = pow(couplings.Lambda_z2->getVal()/parameters.mV->getVal(), 4);
        else if (ilam==4) initval = pow(couplings.Lambda_Q->getVal()/parameters.mX->getVal(), 2);
      }
      else if (ig==2){ // g3_dyn
        if (ilam>=1 && ilam<=3) initval = pow(couplings.Lambda_z3->getVal()/parameters.mV->getVal(), 2);
        else if (ilam>=5 && ilam<=7) initval = pow(couplings.Lambda_z3->getVal()/parameters.mV->getVal(), 4);
        else if (ilam==4) initval = pow(couplings.Lambda_Q->getVal()/parameters.mX->getVal(), 2);
      }
      else if (ig==3){ // g4_dyn
        if (ilam>=1 && ilam<=3) initval = pow(couplings.Lambda_z4->getVal()/parameters.mV->getVal(), 2);
        else if (ilam>=5 && ilam<=7) initval = pow(couplings.Lambda_z4->getVal()/parameters.mV->getVal(), 4);
        else if (ilam==4) initval = pow(couplings.Lambda_Q->getVal()/parameters.mX->getVal(), 2);
      }
      else if (ig==4){ // gzgs1_dyn
        if (ilam>=1 && ilam<=3) initval = pow(couplings.Lambda_zgs1->getVal()/parameters.mV->getVal(), 2);
        else if (ilam>=5 && ilam<=7) initval = pow(couplings.Lambda_zgs1->getVal()/parameters.mV->getVal(), 4);
        else if (ilam==4) initval = pow(couplings.Lambda_Q->getVal()/parameters.mX->getVal(), 2);
      }
    }
    if (ig==2 || ig==6) initval *= fabs(pow(couplings.Lambda->getVal(), 2)/(pow(parameters.mX->getVal(), 2) - pow(parameters.mV->getVal(), 2)));
    if (ig==9) initval *= pow(couplings.Lambda->getVal()/parameters.mX->getVal(), 2);

    if (ig==0){
      ((RooRealVar*)couplings.g1List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.g1List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==1){
      ((RooRealVar*)couplings.g2List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.g2List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==2){
      ((RooRealVar*)couplings.g3List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.g3List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==3){
      ((RooRealVar*)couplings.g4List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.g4List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==4){
      ((RooRealVar*)couplings.gzgs1List[ilam-2][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.gzgs1List[ilam-2][1])->setVal(initval*sin(iphase));
    }
    else if (ig==5){
      ((RooRealVar*)couplings.gzgs2List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.gzgs2List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==6){
      ((RooRealVar*)couplings.gzgs3List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.gzgs3List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==7){
      ((RooRealVar*)couplings.gzgs4List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.gzgs4List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==8){
      ((RooRealVar*)couplings.ggsgs2List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.ggsgs2List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==9){
      ((RooRealVar*)couplings.ggsgs3List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.ggsgs3List[ilam][1])->setVal(initval*sin(iphase));
    }
    else if (ig==10){
      ((RooRealVar*)couplings.ggsgs4List[ilam][0])->setVal(initval*cos(iphase));
      ((RooRealVar*)couplings.ggsgs4List[ilam][1])->setVal(initval*sin(iphase));
    }
  }
  else{
    if (ig==0 && ilam==0){ cerr << "Cannot set fa1! Try to set everything else." << endl; return; }
    else if (ig>4 && ilam!=0){ cerr << "Cannot set fa1 for the g_primes of ZG or GG! Try to set everything else." << endl; return; }
    else if (ig==4 && ilam!=2){ cerr << "Cannot set fa1 for the g_primes of ZG or GG! Try to set everything else." << endl; return; }
    else{
      if (ig==0){
        g1Frac[ilam-1]->setVal(altparam_fracval);
        g1Phase[ilam-1]->setVal(iphase);
      }
      else if (ig==1){
        g2Frac[ilam]->setVal(altparam_fracval);
        g2Phase[ilam]->setVal(iphase);
      }
      else if (ig==2){
        g3Frac[ilam]->setVal(altparam_fracval);
        g3Phase[ilam]->setVal(iphase);
      }
      else if (ig==3){
        g4Frac[ilam]->setVal(altparam_fracval);
        g4Phase[ilam]->setVal(iphase);
      }
      else if (ig==4){
        gzgs1Frac[ilam-2]->setVal(altparam_fracval);
        gzgs1Phase[ilam-2]->setVal(iphase);
      }
      else if (ig==5){
        gzgs2Frac[ilam]->setVal(altparam_fracval);
        gzgs2Phase[ilam]->setVal(iphase);
      }
      else if (ig==6){
        gzgs3Frac[ilam]->setVal(altparam_fracval);
        gzgs3Phase[ilam]->setVal(iphase);
      }
      else if (ig==7){
        gzgs4Frac[ilam]->setVal(altparam_fracval);
        gzgs4Phase[ilam]->setVal(iphase);
      }
      else if (ig==8){
        ggsgs2Frac[ilam]->setVal(altparam_fracval);
        ggsgs2Phase[ilam]->setVal(iphase);
      }
      else if (ig==9){
        ggsgs3Frac[ilam]->setVal(altparam_fracval);
        ggsgs3Phase[ilam]->setVal(iphase);
      }
      else if (ig==10){
        ggsgs4Frac[ilam]->setVal(altparam_fracval);
        ggsgs4Phase[ilam]->setVal(iphase);
      }
    }
  }
}
void ScalarPdfFactory::resetHypotheses(){
  for (int ilam=0; ilam<8; ilam++){
    if (parameterization==0){
      for (int im=0; im<2; im++){
        ((RooRealVar*)couplings.g1List[ilam][im])->setVal(0.); // This is different from setting the fractions!
        ((RooRealVar*)couplings.g2List[ilam][im])->setVal(0.);
        ((RooRealVar*)couplings.g3List[ilam][im])->setVal(0.);
        ((RooRealVar*)couplings.g4List[ilam][im])->setVal(0.);

        if (ilam==2){
          ((RooRealVar*)couplings.gzgs1List[ilam-2][im])->setVal(0.);
        }
        if (ilam==0){
          ((RooRealVar*)couplings.gzgs2List[ilam][im])->setVal(0.);
          ((RooRealVar*)couplings.gzgs3List[ilam][im])->setVal(0.);
          ((RooRealVar*)couplings.gzgs4List[ilam][im])->setVal(0.);
          ((RooRealVar*)couplings.ggsgs2List[ilam][im])->setVal(0.);
          ((RooRealVar*)couplings.ggsgs3List[ilam][im])->setVal(0.);
          ((RooRealVar*)couplings.ggsgs4List[ilam][im])->setVal(0.);
        }
      }
    }
    else{
      if (ilam>0){
        g1Frac[ilam-1]->setVal(0.);
        g1Phase[ilam-1]->setVal(0.);

        if (ilam==2){
          gzgs1Frac[ilam-2]->setVal(0.);
          gzgs1Phase[ilam-2]->setVal(0.);
        }
      }
      else{
        g2Frac[ilam]->setVal(0.);
        g2Phase[ilam]->setVal(0.);
        g3Frac[ilam]->setVal(0.);
        g3Phase[ilam]->setVal(0.);
        g4Frac[ilam]->setVal(0.);
        g4Phase[ilam]->setVal(0.);

        if (ilam==0){
          gzgs2Frac[ilam]->setVal(0.);
          gzgs2Phase[ilam]->setVal(0.);
          gzgs3Frac[ilam]->setVal(0.);
          gzgs3Phase[ilam]->setVal(0.);
          gzgs4Frac[ilam]->setVal(0.);
          gzgs4Phase[ilam]->setVal(0.);
          ggsgs2Frac[ilam]->setVal(0.);
          ggsgs2Phase[ilam]->setVal(0.);
          ggsgs3Frac[ilam]->setVal(0.);
          ggsgs3Phase[ilam]->setVal(0.);
          ggsgs4Frac[ilam]->setVal(0.);
          ggsgs4Phase[ilam]->setVal(0.);
        }
      }
    }
  }
}


