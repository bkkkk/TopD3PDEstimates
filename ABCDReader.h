/*
 * METvsIso.h
 * METvsIso class creates a smart wrapper around histograms to make ABCD
 * type analysis easier and extensible.
 *
 *  Created on: Oct 26, 2011
 *      Author: jayb88
 */

#ifndef ABCDREADER_H_
#define ABCDREADER_H_

#include <vector>
#include "TString.h"
#include "DataSample.h"

class ABCDReader {

private:
	DataSample* sample_;
	double nA_;
	double nB_;
	double nC_;
	double nD_;
	double nAError_;
	double nBError_;
	double nCError_;
	double nDError_;
	int isZombie_;
	TString mode_;
	int jet_bin_;
	bool is_inclusive_;
	int sys_mode_;
	TString br_sys_;

	void setRegionIntegralsAndErrors();

	const double GetYieldFromSample(int region);
	const double GetYieldErrorFromSample(int region);
	double GetSystFactor(void);

public:
	ABCDReader(DataSample* sample, TString mode, int jet_bin, bool is_inclusive,
			int sys_mode, TString br_sys);
	virtual ~ABCDReader(void);
	double GetRegionYield(int region);
	double GetRegionError(int region);

	const double GetNdEstimate(void);

ClassDef(ABCDReader,1)

};
#endif /* ABCDReader_H_ */
