/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOSPINZERO_5D_VH
#define ROOSPINZERO_5D_VH

#include "RooSpinZero.h"
 
class RooSpinZero_5D_VH : public RooSpinZero {
public:

  RooSpinZero_5D_VH(){}
  RooSpinZero_5D_VH(
    const char *name, const char *title,
    modelMeasurables _measurables,
    modelParameters _parameters
    );

  RooSpinZero_5D_VH(const RooSpinZero_5D_VH& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooSpinZero_5D_VH(*this, newname); }
  inline virtual ~RooSpinZero_5D_VH(){}

  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const;


protected:

  Double_t evaluate() const;

};
 
#endif