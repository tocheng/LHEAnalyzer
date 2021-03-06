#!/bin/env python

import sys
import imp
import copy
import os
import filecmp
import shutil
import pickle
import math
import pprint
import subprocess
from datetime import date
from argparse import ArgumentParser as OptionParser
from CMSDataTools.AnalysisTree.TranslateStringBetweenPythonAndShell import *


class BatchManager:
   def __init__(self):
      # define options and arguments ====================================
      self.parser = OptionParser()

      self.parser.add_argument("--input", type=str, help="File containing input commands")
      self.parser.add_argument("--output", type=str, help="Output file with the list of commands")
      self.parser.add_argument("--add_range", type=str, action="append", help="Wildcards to replace")
      self.parser.add_argument("--add_string_list", type=str, action="append", help="Wildcards to replace (string type)")
      self.parser.add_argument("--reset_linenumber", action="store_true", default=False, help="Reset line number indicator <LINENUMBER> for each line in the input file")

      self.opt = self.parser.parse_args()

      optchecks=[
         "input",
         "output"
      ]
      for theOpt in optchecks:
         if not hasattr(self.opt, theOpt) or getattr(self.opt, theOpt) is None:
            sys.exit("Need to set --{} option".format(theOpt))

      if not os.path.isfile(self.opt.input):
         sys.exit("List {} does not exist. Exiting...".format(self.opt.input))

      self.rangelist = {}
      self.strlist = {}
      if self.opt.add_range is None:
         self.opt.add_range = []
      for rng in self.opt.add_range:
         rnglist = rng.split(':')
         if len(rnglist)!=2:
            raise RuntimeError("{} could not be split properly.".format(rng))

         if "<ALLINPUTS>" == rnglist[0]:
            raise RuntimeError("Forbidden to specify <ALLINPUTS> as a wildcard range key.")
         elif "<LINENUMBER>" == rnglist[0]:
            raise RuntimeError("Forbidden to specify <LINENUMBER> as a wildcard range key.")

         rnglist[0] = rnglist[0].replace('<','')
         rnglist[0] = rnglist[0].replace('>','')
         rnglist[1] = rnglist[1].split(',')
         if len(rnglist[1])==2:
            rnglist[1][0]=int(rnglist[1][0])
            rnglist[1][1]=int(rnglist[1][1])+1
         elif len(rnglist[1])==1:
            rnglist[1][0]=str(rnglist[1][0])
         elif len(rnglist[1])>2:
            raise RuntimeError("{} could not be split properly.".format(rnglist[1]))
         self.rangelist[rnglist[0]]=rnglist[1]

      if self.opt.add_string_list is None:
         self.opt.add_string_list = []
      for rng in self.opt.add_string_list:
         rnglist = rng.split(':')
         if "<ALLINPUTS>" == rnglist[0]:
            raise RuntimeError("Forbidden to specify <ALLINPUTS> as a wildcard range key.")
         elif "<LINENUMBER>" == rnglist[0]:
            raise RuntimeError("Forbidden to specify <LINENUMBER> as a wildcard range key.")

         rnglist[0] = rnglist[0].replace('<','')
         rnglist[0] = rnglist[0].replace('>','')
         rnglist[1] = rnglist[1].split(',')
         self.strlist[rnglist[0]]=rnglist[1]


      self.run()


   def run(self):
      currentdir = os.getcwd()
      finput = open(self.opt.input,'r')
      foutput = open(self.opt.output, 'w')

      iline=0
      for fline in finput:
         if self.opt.reset_linenumber: # Reset the line number if the option dictates so
            iline=0
         fline = fline.lstrip()
         if fline.startswith('#'):
            continue

         #print "Analyzing line '{}'".format(fline)

         fline = " ".join(fline.split()); fline += '\n'
         wlinelist = [ fline ] # Start with a single line
         for key,val in self.strlist.iteritems():
            skey = '<'+key+'>'
            wlinelisttmp = []
            for tmpline in wlinelist:
               if skey in tmpline:
                  for sval in val:
                     addline=tmpline.replace(skey,sval)
                     wlinelisttmp.append(addline)
               else:
                  wlinelisttmp.append(tmpline)
            wlinelist = wlinelisttmp
         for key,val in self.rangelist.iteritems():
            skey = '<'+key+'>'
            wlinelisttmp = []
            for tmpline in wlinelist:
               if skey in tmpline:
                  if len(val)==1:
                     addline=tmpline.replace(skey,val[0])
                     wlinelisttmp.append(addline)
                  else: # Length==2
                     for ival in range(val[0],val[1]):
                        addline=tmpline.replace('<'+key+'>',str(ival))
                        wlinelisttmp.append(addline)
            wlinelist = wlinelisttmp

         # After replacing all keys, look for the special key <ALLINPUTS> in each line in order to create a line for each input file
         wlinelist_allinputs = []
         for wline in wlinelist:
            #print "Checking line {}".format(wline)
            if "<ALLINPUTS>" in wline:
               lineargs = wline.split()
               indir="./"
               fileLevel=0
               flist = []
               for linearg in lineargs:
                  if "indir=" in linearg:
                     indir = linearg.replace("indir=","")
                  elif "fileLevel=" in linearg:
                     fileLevel = int(linearg.replace("fileLevel=",""))
               if os.path.exists(indir):
                  fext=".root"
                  if fileLevel == 0:
                     fext=".lhe"
                  flist = [ff for ff in os.listdir(indir) if (os.path.isfile(os.path.join(indir, ff)) and fext in ff and "grid" not in ff.lower())]
               elif fileLevel == 1:
                  flist = [ ff.replace("root://cms-xrd-global.cern.ch//","") for ff in subprocess.check_output(['createDatasetFileList.py', '--dataset={}'.format(indir), '--method=dbs', '--options=prepend_cms_xrd']).split('\n') if ff ]
                  wline = wline.replace("indir={}".format(indir),"indir={}".format("root://cms-xrd-global.cern.ch//"))

               if len(flist) == 0:
                  print "Warning: No files were found for input '{}'!".format(indir)
               for ff in flist:
                  wlinelist_allinputs.append(wline.replace("<ALLINPUTS>", ff))
            else:
               wlinelist_allinputs.append(wline)

         for wline in wlinelist_allinputs:
            if "<LINENUMBER>" in wline:
               wline = wline.replace("<LINENUMBER>",str(iline))
            foutput.write(wline)
            iline = iline+1

      foutput.close()
      finput.close()



if __name__ == '__main__':
   batchManager = BatchManager()
