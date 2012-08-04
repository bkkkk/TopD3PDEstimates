/*
 * doABCD.h
 * Drives METvsIso, DataSample and sampleAggregator classes
 * to produce results using the ABCD method.
 *
 * Author: Jayb88
 *
 */

#ifndef DOABCD_H_
#define DOABCD_H_

#include <map>
#include "ABCDReader.h"
#include "DataSample.h"

typedef std::map<TString, ABCDReader*> ReaderCollection;

class DoABCD {

private:
	ReaderCollection reader_collection;
	std::vector<std::string> listOfSamples;

	TString mode_;
	bool doInclusive_;
	int jet_bin_;
	int sysMode_;
	TString br_sys_;

	void init(void);

public:
	DoABCD(TString mode = "tag", bool doInclusive_ = false, int jet_bin = 3, int sysMode = 1, TString br_sys = "");
	virtual ~DoABCD();
	void printNdEstimateTable(TString table_mode);

	double getDataRegionYield(int region);
	double getCorrection(int region);

	double getCorrectedRegionYield(int region);
	double getRegionError(int region);

	double getNdEstimate(void);
	double getNdError(void);
	double getNdSystError(void);

	TString getLabel(void);

	ClassDef(DoABCD,1)
};

#endif /* DOABCD_H_ */
