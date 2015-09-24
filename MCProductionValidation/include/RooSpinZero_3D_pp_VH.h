/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOSPINZERO_3D_PP_VH
#define ROOSPINZERO_3D_PP_VH

#include "RooSpinZero.h"
#include "TLorentzVector.h"
#include "TLorentzRotation.h"

class RooSpinZero_3D_pp_VH : public RooSpinZero {
public:

  Double_t sqrts;

  RooSpinZero_3D_pp_VH(){}
  RooSpinZero_3D_pp_VH(
    const char *name, const char *title,
    modelMeasurables _measurables,
    modelParameters _parameters,
    Double_t _sqrts
    );

  RooSpinZero_3D_pp_VH(const RooSpinZero_3D_pp_VH& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooSpinZero_3D_pp_VH(*this, newname); }
  inline virtual ~RooSpinZero_3D_pp_VH(){}

  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const;

protected:

  Double_t evaluate() const;

  double partonicLuminosity(double mVal, double YVal, double sqrts) const;

};

#endif