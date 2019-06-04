#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include "TSystem.h"
#include "TInterpreter.h"
#include "TFile.h"
#include "TList.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "converter.h"

converter::converter(OptionParser* options_){
  options = options_;
  configure();
}
void converter::configure(){
  filename = options->inputfiles();
  string cindir = options->inputDir();
  for (string& fname:filename) fname.insert(0, cindir);

  string coutput = options->outputDir();
  coutput.append(options->outputFilename());

  cout << "converter::configure -> Creating file " << coutput << endl;
  foutput = new TFile(coutput.c_str(), "recreate");
  foutput->cd();
  char TREE_NAME[] = "SelectedTree";
  tree = new HVVTree(TREE_NAME, TREE_NAME);
  tree->setOptions(options);
}
void converter::finalizeRun(){
  cout << "converter::finalizeRun -> Number of recorded events: " << tree->getTree()->GetEntries() << endl;
  tree->writeTree(foutput);
  delete tree;
  foutput->Close();
  options=nullptr;
  filename.clear();
}

