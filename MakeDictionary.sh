#!/bin/bash

echo Making Dictionary
rootcint -f qcdEstimationDict.C -c AbcdBase.h DataSample.h DoABCD.h ABCDReader.h DoRSMT.h RootLinkDef.h
echo "Done! :-)"