#include "../include/TensorPdfFactory_HVV.h"

TensorPdfFactory_HVV::TensorPdfFactory_HVV(RooSpinTwo::modelMeasurables measurables_, int V1decay_, int V2decay_) :
TensorPdfFactory(measurables_, V1decay_, V2decay_)
{
  //measurables.Y=0;
  makeParamsConst(true);
  initPDF();
}

TensorPdfFactory_HVV::~TensorPdfFactory_HVV(){
  destroyPDF();
}

void TensorPdfFactory_HVV::makeParamsConst(bool yesNo){
  parameters.Lambda->setConstant(true);

  parameters.mX->setConstant(yesNo);
  parameters.gamX->setConstant(yesNo);
  parameters.mV->setConstant(yesNo);
  parameters.gamV->setConstant(yesNo);
  parameters.R1Val->setConstant(yesNo);
  parameters.R2Val->setConstant(yesNo);
}

void TensorPdfFactory_HVV::initPDF(){
  PDF = new RooSpinTwo_7DComplex_HVV(
    "PDF", "PDF",
    measurables,
    parameters,
    V1decay, V2decay
    );
  PDF_base = (RooSpinTwo*)PDF;
}



