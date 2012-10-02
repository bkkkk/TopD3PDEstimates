/**
 * @file ABCDReader.cpp
 * @brief ABCDReader Implementation
 * @date Jul 10, 2012
 * @author Jacobo Blanco (jayb88@cern.ch)
 */

// Includes
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
#include <string>

// Needed by ROOT
ClassImp(ABCDReader)

// ============================================================

ABCDReader::ABCDReader(DataSample* sample, TString mode, int jet_bin,
		bool is_inclusive, int sys_mode, TString br_sys) :
		sample_(sample), //
		nA_(0.), nB_(0.), nC_(0.), nD_(0.), //
		nAError_(0.), nBError_(0.), nCError_(0.), nDError_(0.), //
		isZombie_(0), //
		mode_(mode), jet_bin_(jet_bin), //
		is_inclusive_(is_inclusive), sys_mode_(sys_mode), //
		br_sys_(br_sys) {

	// Get all regional yields and uncertainties
	this->setRegionIntegralsAndErrors();
}
 
// ============================================================

ABCDReader::~ABCDReader() {

	// Clear everything
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

	// Delete DataSample object
	delete sample_;
}

// ============================================================

void ABCDReader::setRegionIntegralsAndErrors() {

	// Get yields for each region from sample file
	//
	// @todo Implement enum from AbcdBase
	nA_ = this->GetYieldFromSample(0);
	nB_ = this->GetYieldFromSample(1);
	nC_ = this->GetYieldFromSample(2);
	nD_ = this->GetYieldFromSample(3);

	// Get statistical error for each region
	//
	// @todo Implement enum from AbcdBase
	nAError_ = this->GetYieldErrorFromSample(0);
	nBError_ = this->GetYieldErrorFromSample(1);
	nCError_ = this->GetYieldErrorFromSample(2);
	nDError_ = this->GetYieldErrorFromSample(3);

	return;
}

// ============================================================

const double ABCDReader::GetNdEstimate() {
	return (nB_ * nC_) / (nA_);
}

// ============================================================

double ABCDReader::GetRegionYield(int region) {

	double regionValue = 0;

	// Select correct region yield
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

	// Adjust the yield by factor from systematic uncertainty
	regionValue *= this->GetSystFactor();

	return regionValue;
}

// ============================================================

double ABCDReader::GetRegionError(int region) {
	double regionError = 0;

	// Select correct region error
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

// ============================================================

const double ABCDReader::GetYieldFromSample(int region) {

	// Return the yield for given region for current sample
	return sample_->GetYield(mode_, region, jet_bin_, is_inclusive_, br_sys_);
}

// ============================================================

const double ABCDReader::GetYieldErrorFromSample(int region) {

	// Return the error for the given region for current sample
	return sample_->GetYieldError(mode_, region, jet_bin_, is_inclusive_, br_sys_);
}

// ============================================================

double ABCDReader::GetSystFactor() {

	// Initial variables
	double factor = 1.;
	double error = 0.;

	// Debugging block
#ifdef DEBUG
	std::cout << "ABCDReader::GetSystFactor - Systematic: " << sys_mode_
			<< std::endl;
	std::cout << "ABCDReader::GetSystFactor - Sample Name: "
			<< sample_->GetSampleName() << std::endl;
#endif
	// ttbar and W+jets variation for systematic uncertainty
	// currently using 15% for ttbar and 25% for W+jets
	if (sample_->GetSampleName().Contains("ttbar"))
		error = 0.15;
	else if (sample_->GetSampleName().Contains("WJetsScaled"))
		error = 0.25;


	// Calculate the factor to rescale sample by
	if (sys_mode_ == 0) {
		factor -= error;
	} else if (sys_mode_ == 2) {
		factor += error;
	} else {
		factor = 1.;
	}

	return factor;
}

// ============================================================

