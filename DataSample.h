/*
 * DataSample.h
 *
 *  Created on: Jul 3, 2012
 *      Author: jayb88
 */

#include "TString.h"
#include "TH1D.h"
#include "TFile.h"
#include <map>

#ifndef DATASAMPLE_H_
#define DATASAMPLE_H_

class DataSample {
public:
	DataSample(TString sample_name_);
	virtual ~DataSample();

	TString GetSampleName() const {
		return sample_name;
	}

	void SetSampleName(TString sampleName) {
		sample_name = sampleName;
	}

	TString GetSamplePath() const {
		return sample_path;
	}

	void SetSamplePath(TString samplePath) {
		sample_path = samplePath;
	}

	void init(void);

	std::vector<TH1D*> GetHistos(TString mode);

	const double GetYield(TString mode, int region, int jet_bin, bool is_inclusive);
	const double GetYieldError(TString mode, int region, int jet_bin, bool is_inclusive);
	const double GetContamination(TString mode, int region, int jet_bin, bool is_inclusive);
	const double GetContaminationError(TString mode, int region, int jet_bin, bool is_inclusive);

	void GetYields(void);
	void GetContaminations(void);

private:

	double GetDataYield(TString mode, int region, int jet_bin,
			bool is_inclusive);
	double GetDataYieldError(TString mode, int region, int jet_bin,
			bool is_inclusive);

	typedef std::map< TString, TH1D* > HistoDatabase;
	HistoDatabase *histo_database;

	TString sample_name;
	TString sample_path;
	TString sample_full_name;
	TFile* file;

};

#endif /* DATASAMPLE_H_ */
