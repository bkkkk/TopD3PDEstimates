/*
 * DoRSMT.h
 *
 *  Created on: Jul 10, 2012
 *      Author: jayb88
 */

#ifndef DORSMT_H_
#define DORSMT_H_

#include "ABCDReader.h"
#include <map>

typedef std::map<TString, ABCDReader*> ReaderCollection;

class DoRSMT {

private:
	ReaderCollection reader_collection_pretag;
	ReaderCollection reader_collection_tag;

	std::vector<std::string> list_of_samples;

	int jet_bin_;
	bool is_inclusive_;
	int sys_mode_;
	TString br_sys_mode_;

	void init(void);
	double GetPretagEstimate(void);
	TString GetLabel(void);
	ReaderCollection& GetCollection(TString mode);

	double GetSumOfErrors();

public:
	DoRSMT(int jet_bin = 3, bool is_inclusive = true, int sys_mode = 1, TString br_sys_mode = "" );
	virtual ~DoRSMT();

	void PrintEstimateTable(TString mode, TString table_mode);
	void PrintRsmtTable(TString table_mode);

	// Region yields correction section
	double GetDataRegionYield(TString mode, int region);
	double GetCorrection(TString mode, int region);
	double GetCorrectedRegionYield(TString mode, int region);
	double GetRegionError(TString mode, int region);

	// RsmtRegion section
	double GetRsmt(int region);
	double GetRsmtStatError(int region);
	double GetRsmtSystError(int region);

	// RsmtWgt Section
	double GetRsmtWgt(void);
	double GetRsmtWgtStatErr(void);
	double GetRsmtWgtSystErr(void);

	// Rsmt Estimate Section
	double GetTagEstimate(void);
	double GetTagEstimateSystError(void);
	double GetTagEstimateStatError(void);
};

#endif /* DORSMT_H_ */
