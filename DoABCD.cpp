/**
 * @file 	DoABCD.cpp
 * @brief 	DoABCD
 * @author 	Jacobo Blanco (jayb88@cern.ch)
 * @date 	Oct. 2011
 */

// Includes
#include <TFile.h>
#include <TH2D.h>
#include <TString.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include "AbcdBase.h"
#include "DataSample.h"
#include "ABCDReader.h"
#include "DoABCD.h"

// Needed by ROOT
ClassImp(DoABCD)

// ============================================================================

DoABCD::DoABCD(TString mode, bool doInclusive, int jet_bin, int sysMode,
		TString br_sys) :
		mode_(mode), doInclusive_(doInclusive), //
		jet_bin_(jet_bin), sysMode_(sysMode), //
		br_sys_(br_sys) {

	listOfSamples.push_back("dataAllEgamma");
	listOfSamples.push_back("ttbarSig");
	listOfSamples.push_back("WJetsScaled");
	listOfSamples.push_back("Zjets");
	listOfSamples.push_back("singleTop");
	listOfSamples.push_back("diBoson");

	this->init();
}

// ============================================================================

DoABCD::~DoABCD() {
#ifdef DEBUG
	std::cout << "---| DoABCD::~DoABCD Calling Destructor" << std::endl;
#endif
	ReaderCollection::iterator iter = reader_collection.begin();
	ReaderCollection::iterator iter_end = reader_collection.end();

	for (; iter != iter_end; iter++) {
		delete iter->second;
	}
}

// ============================================================================

void DoABCD::init(void) {

	// Loop over the name of samples and create a map of Reader Objects
	for (unsigned int sample_index = 0; sample_index != listOfSamples.size();
			sample_index++) {
		TString samplename = listOfSamples.at(sample_index);
#ifdef DEBUG
		std::cout << "DoABCD::init() - Debug Loading Sample: " << samplename << std::endl;
#endif
		reader_collection[samplename] = new ABCDReader(
				new DataSample(samplename), mode_, jet_bin_, doInclusive_,
				sysMode_, br_sys_);
	}
	return;
}

// ============================================================================

void DoABCD::printNdEstimateTable(TString table_mode) {
	// Getting integrals for regions in Data plot
	double nDEstimate = this->getNdEstimate();
	double nDError = this->getNdError();
	double nDSystErr = this->getNdSystError();
	double nD_tot_err = sqrt((nDError * nDError) + (nDSystErr * nDSystErr));

	// Tidying up
	std::cout << std::setprecision(1);
	if (table_mode.Contains("nice")) {
		std::cout << this->getLabel() << " " << std::fixed << nDEstimate
				<< AbcdBase::pm << nD_tot_err << std::endl;
	} else if (table_mode.Contains("central")) {
		std::cout << this->getLabel() << "\t" << std::fixed << nDEstimate
				<< std::endl;
	} else if (table_mode.Contains("table")) {
		std::cout << this->getLabel() << "\t" << std::fixed << nDEstimate
				<< "\t" << nD_tot_err << "\t" << std::endl;
	} else {
		std::cout << this->getLabel() << " " << std::fixed << nDEstimate
				<< AbcdBase::pm << nDError << AbcdBase::pm << nDSystErr
				<< std::endl;
	}
	return;
}

// ============================================================================

double DoABCD::getCorrection(int region) {
	double correction = 0.;

#ifdef DEBUG
	std::cout << "--| DoABCD::Getting corrections for region: " << region << std::endl;
#endif

	ReaderCollection::iterator iter = reader_collection.begin();
	ReaderCollection::iterator iter_end = reader_collection.end();

	for (; iter != iter_end; iter++) {
		if (iter->first.Contains("dataAllEgamma") == 0) {
#ifdef DEBUG
			std::cout << "--| DoABCD::Current Correction (" << iter->first
			<< "): " << iter->second->GetRegionYield(region)
			<< std::endl;
#endif
			correction += iter->second->GetRegionYield(region);
		}
	}
	return correction;
}

// ============================================================================

double DoABCD::getDataRegionYield(int region) {
	double yield = 0.;
	yield = reader_collection["dataAllEgamma"]->GetRegionYield(region);

#ifdef DEBUG
	std::cout << "--| DoABCD::Data Yield: " << yield << std::endl;
#endif

	return yield;
}

// ============================================================================

double DoABCD::getNdEstimate() {

	double nA_corr = this->getCorrectedRegionYield(AbcdBase::A);
	double nB_corr = this->getCorrectedRegionYield(AbcdBase::B);
	double nC_corr = this->getCorrectedRegionYield(AbcdBase::C);

	return nB_corr * nC_corr / nA_corr;
}

// ============================================================================

double DoABCD::getNdError() {
	double nDEstimate = getNdEstimate();

	double nA_ = this->getDataRegionYield(AbcdBase::A);
	double nB_ = this->getDataRegionYield(AbcdBase::B);
	double nC_ = this->getDataRegionYield(AbcdBase::C);

	double totalErrorA = this->getRegionError(AbcdBase::A);
	double totalErrorB = this->getRegionError(AbcdBase::B);
	double totalErrorC = this->getRegionError(AbcdBase::C);

	double correctionA = this->getCorrection(AbcdBase::A);
	double correctionB = this->getCorrection(AbcdBase::B);
	double correctionC = this->getCorrection(AbcdBase::C);

	double bPortion = totalErrorB / ((nB_ - correctionB) * (nB_ - correctionB));
	double cPortion = totalErrorC / ((nC_ - correctionC) * (nC_ - correctionC));
	double aPortion = totalErrorA / ((nA_ - correctionA) * (nA_ - correctionA));
	double errorNd = nDEstimate * sqrt(bPortion + cPortion + aPortion);

	return errorNd;
}

// ============================================================================

double DoABCD::getNdSystError() {
	DoABCD* up = new DoABCD(mode_, doInclusive_, jet_bin_, 2, br_sys_);
	DoABCD* down = new DoABCD(mode_, doInclusive_, jet_bin_, 0, br_sys_);

	double up_est = up->getNdEstimate();
	double down_est = down->getNdEstimate();
	double nominal = this->getNdEstimate();

	double error = std::max(fabs(up_est - nominal), fabs(down_est - nominal));

	delete up;
	delete down;

	return error;

}

// ============================================================================

double DoABCD::getCorrectedRegionYield(int region) {
	double yield = this->getDataRegionYield(region);
	double corr = this->getCorrection(region);
	return (yield - corr);
}

// ============================================================================

double DoABCD::getRegionError(int region) {
	double sumError = 0.;
	double regError = 0.;
	ReaderCollection::iterator iter = reader_collection.begin();
	ReaderCollection::iterator iter_end = reader_collection.end();

	for (; iter != iter_end; iter++) {
		regError = iter->second->GetRegionError(region);
		sumError += regError * regError;
	}

	return sqrt(sumError);
}

// ============================================================================

TString DoABCD::getLabel() {
	TString suffix = "";
	if (doInclusive_ != 0)
		suffix = "inc ";

	// 3 jet inc (pretag)
	TString label = Form("%i jet %s(%s)", jet_bin_, suffix.Data(),
			mode_.Data());

	return label;
}

// ============================================================================
