#include <math.h>
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TString.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TFile.h>
#include <TLine.h>
#include <iomanip>
#include <vector>

#include "DataSample.h"
#include "ABCDReader.h"
#include "AbcdBase.h"
#include <string>;
// #define DEBUG

ClassImp(ABCDReader)

ABCDReader::ABCDReader(DataSample* sample, TString mode, int jet_bin,
		bool is_inclusive, int sys_mode, TString br_sys) :
		sample_(sample), //
		nA_(0.), nB_(0.), nC_(0.), nD_(0.), //
		nAError_(0.), nBError_(0.), nCError_(0.), nDError_(0.), //
		isZombie_(0), //
		mode_(mode), jet_bin_(jet_bin), //
		is_inclusive_(is_inclusive), sys_mode_(sys_mode), //
		br_sys_(br_sys)
{
#ifdef DEBUG 
		std::cout << "ABCDReader::Constructor - Debug Point 1" << std::endl;
#endif 
	this->setRegionIntegralsAndErrors();
}
 
ABCDReader::~ABCDReader() {

	nA_ = 0.;
	nB_ = 0.;
	nC_ = 0.;
	nD_ = 0.;
	nAError_ = 0.;
	nBError_ = 0.;
	nCError_ = 0.;
	nDError_ = 0.;
	isZombie_ = 1;
	mode_ = "";
	delete sample_;
}

void ABCDReader::setRegionIntegralsAndErrors() {
	nA_ = this->GetYieldFromSample(0);
	nB_ = this->GetYieldFromSample(1);
	nC_ = this->GetYieldFromSample(2);
	nD_ = this->GetYieldFromSample(3);

	nAError_ = this->GetYieldErrorFromSample(0);
	nBError_ = this->GetYieldErrorFromSample(1);
	nCError_ = this->GetYieldErrorFromSample(2);
	nDError_ = this->GetYieldErrorFromSample(3);

	return;
}

// Returns nD estimate for this ABCDReader object
const double ABCDReader::GetNdEstimate() {
	return (nB_ * nC_) / (nA_);
}
/*----------------------------------------------*/

// returns the region integral
double ABCDReader::GetRegionYield(int region) {

	double regionValue = 0;

	switch (region) {
	case (AbcdBase::A):
		regionValue = nA_;
		break;
	case (AbcdBase::B):
		regionValue = nB_;
		break;
	case (AbcdBase::C):
		regionValue = nC_;
		break;
	case (AbcdBase::D):
		regionValue = nD_;
		break;
	}

	regionValue *= this->GetSystFactor();

	return regionValue;
}

// returns the integral error in
double ABCDReader::GetRegionError(int region) {
	double regionError = 0;

	switch (region) {
	case (AbcdBase::A):
		regionError = nAError_;
		break;
	case (AbcdBase::B):
		regionError = nBError_;
		break;
	case (AbcdBase::C):
		regionError = nCError_;
		break;
	case (AbcdBase::D):
		regionError = nDError_;
		break;
	}

	return regionError;
}

// Grabs Yields from histogram
const double ABCDReader::GetYieldFromSample(int region) {
	return sample_->GetYield(mode_, region, jet_bin_, is_inclusive_, br_sys_);
} // End of GetYieldFromSample

const double ABCDReader::GetYieldErrorFromSample(int region) {
	return sample_->GetYieldError(mode_, region, jet_bin_, is_inclusive_, br_sys_);
} // End of GetYieldErrorFromSample

double ABCDReader::GetSystFactor() {
	double factor = 1.;
	double error = 0.;
#ifdef DEBUG
	std::cout << "ABCDReader::GetSystFactor - Systematic: " << sys_mode_
			<< std::endl;
	std::cout << "ABCDReader::GetSystFactor - Sample Name: "
			<< sample_->GetSampleName() << std::endl;
#endif
	if (sample_->GetSampleName().Contains("ttbar"))
		error = 0.15;
	else if (sample_->GetSampleName().Contains("WJetsScaled"))
		error = 0.25;

	if (sys_mode_ == 0) {
		factor -= error;
	} else if (sys_mode_ == 2) {
		factor += error;
	} else {
		factor = 1.;
	} // end if sysmode

	return factor;
}
