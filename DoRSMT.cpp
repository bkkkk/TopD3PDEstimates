/*
 * DoRSMT.cpp
 *
 *  Created on: Jul 10, 2012
 *      Author: jayb88
 */

#include "DoRSMT.h"
#include <iostream>
#include "math.h"
#include "AbcdBase.h"
#include <iomanip>

DoRSMT::DoRSMT(int jet_bin = 3, bool is_inclusive = true, int sys_mode = 1) :
		jet_bin_(jet_bin), //
		is_inclusive_(is_inclusive), //
		sys_mode_(sys_mode) //
{
	list_of_samples.push_back("dataAllEgamma");
	list_of_samples.push_back("ttbar");
	list_of_samples.push_back("WJetsScaled");
	list_of_samples.push_back("Zjets");
	list_of_samples.push_back("singleTop");
	list_of_samples.push_back("diBoson");

	this->init();
}

DoRSMT::~DoRSMT() {
	ReaderCollection::iterator iter = reader_collection_pretag.begin();
	ReaderCollection::iterator iter_end = reader_collection_pretag.end();

	for (; iter != iter_end; iter++) {
		delete iter->second;
	}

	ReaderCollection::iterator itertwo = reader_collection_tag.begin();
	ReaderCollection::iterator itertwo_end = reader_collection_tag.end();

	for (; itertwo != itertwo_end; itertwo++) {
		delete itertwo->second;
	}
}

void DoRSMT::init() {

	// Loop over the name of samples and create a map of Reader Objects
	for (unsigned int sample_index = 0; sample_index != list_of_samples.size();
			sample_index++) {
		TString samplename = list_of_samples.at(sample_index);

		reader_collection_pretag[samplename] = new ABCDReader(
				new DataSample(samplename), "pretag", jet_bin_, is_inclusive_,
				sys_mode_);
		reader_collection_tag[samplename] = new ABCDReader(
				new DataSample(samplename), "tag", jet_bin_, is_inclusive_,
				sys_mode_);
	}
	return;
} // End init

void DoRSMT::PrintRsmtTable() {

	double r_smt_A = 100 * this->GetRsmt(AbcdBase::A);
	double r_smt_B = 100 * this->GetRsmt(AbcdBase::B);
	double r_smt_C = 100 * this->GetRsmt(AbcdBase::C);
	double r_smt_wgt = 100 * this->GetRsmtWgt();

	double r_smt_A_err_stat = 100 * this->GetRsmtStatError(AbcdBase::A);
	double r_smt_B_err_stat = 100 * this->GetRsmtStatError(AbcdBase::B);
	double r_smt_C_err_stat = 100 * this->GetRsmtStatError(AbcdBase::C);
	double r_smt_wgt_err_stat = 100 * this->GetRsmtWgtStatErr();

	double r_smt_A_err_sys = 100 * this->GetRsmtSystError(AbcdBase::A);
	double r_smt_B_err_sys = 100 * this->GetRsmtSystError(AbcdBase::B);
	double r_smt_C_err_sys = 100 * this->GetRsmtSystError(AbcdBase::C);
	double r_smt_wgt_err_sys = this->GetRsmtWgtSystErr() * r_smt_wgt;

	std::cout << std::setprecision(4);
	std::cout << "| " << this->GetLabel() << " | " << r_smt_A << AbcdBase::pm
			<< r_smt_A_err_stat << "(stat)" << AbcdBase::pm << r_smt_A_err_sys
			<< "(syst) | " << r_smt_B << AbcdBase::pm << r_smt_B_err_stat
			<< "(stat)" << AbcdBase::pm << r_smt_B_err_sys << "(syst) | "
			<< r_smt_C << AbcdBase::pm << r_smt_C_err_stat << "(stat)"
			<< AbcdBase::pm << r_smt_C_err_sys << "(syst) | " << r_smt_wgt
			<< AbcdBase::pm << r_smt_wgt_err_stat << AbcdBase::pm
			<< r_smt_wgt_err_sys << "(syst) | " << std::endl;
	return;
}

// Prints out the qcd estimate with stat error
void DoRSMT::PrintEstimateTable(TString mode) {

	// Getting integrals for regions in Data plot
#ifdef DEBUG
	std::cout << "DoRSMT::PrintEstimateTable - Getting Tag Estimate"
	<< std::endl;
#endif
	double nD_estimate, nD_error, nD_error_syst = 0.;

	if (mode.Contains("pretag")) {
		nD_estimate = this->GetPretagEstimate();
		nD_error = nD_estimate * 0.5;
	} else {
		nD_estimate = this->GetTagEstimate();
		nD_error = this->GetTagEstimateStatError();
		nD_error_syst = this->GetTagEstimateSystError();
	}

	// Tidying up
	std::cout << std::setprecision(1);
	std::cout << "| " << this->GetLabel() << " (" << mode << ") | "
			<< std::fixed << nD_estimate << AbcdBase::pm << nD_error << "(stat)"
			<< AbcdBase::pm << nD_error_syst << "(syst) | " << std::endl;

	return;
}
/*-----*/

// Get Data Yield
double DoRSMT::GetDataRegionYield(TString mode, int region) {
	double yield = 0.;
	if (mode.Contains("pretag")) {
		yield = reader_collection_pretag["dataAllEgamma"]->GetRegionYield(
				region);
	} else {
		yield = reader_collection_tag["dataAllEgamma"]->GetRegionYield(region);
	}
#ifdef DEBUG
	std::cout << "DoRSMT::GetDataRegionYield - " << mode << " yield (" << region
	<< "): " << yield << std::endl;
#endif
	return yield;
}
/*-----*/

// Get Corrections
double DoRSMT::GetCorrection(TString mode, int region) {

	ReaderCollection temp_collection = this->GetCollection(mode);
	double correction = 0.;

#ifdef DEBUG
	std::cout << "--| DoRSMT::Getting corrections for region: " << region
	<< std::endl;
#endif

	ReaderCollection::iterator iter = temp_collection.begin();
	ReaderCollection::iterator iter_end = temp_collection.end();

	for (; iter != iter_end; iter++) {
		if (iter->first.Contains("dataAllEgamma") == 0) {

#ifdef DEBUG
			std::cout << "--| DoRSMT::Current Correction (" << iter->first
			<< "): " << iter->second->GetRegionYield(region)
			<< std::endl;
#endif

			correction += iter->second->GetRegionYield(region);
		}
	}

	return correction;
}
/*-----*/

// Returns corrected yield for region
double DoRSMT::GetCorrectedRegionYield(TString mode, int region) {
	double yield = this->GetDataRegionYield(mode, region);
	double correction = this->GetCorrection(mode, region);
	return yield - correction;
}
/*-----*/

// Returns Region Error
double DoRSMT::GetRegionError(TString mode, int region) {
	ReaderCollection temp_collection = this->GetCollection(mode);
	double sumError = 0.;
	double regError = 0.;

	ReaderCollection::iterator iter = temp_collection.begin();
	ReaderCollection::iterator iter_end = temp_collection.end();

	for (; iter != iter_end; iter++) {
		regError = iter->second->GetRegionError(region);
		sumError += regError * regError;
	}
	return sqrt(sumError);
}
/*-----*/

//
// RSMT SECTION
//
// Returns Rsmt for region
double DoRSMT::GetRsmt(int region) {
	return this->GetCorrectedRegionYield("tag", region)
			/ this->GetCorrectedRegionYield("pretag", region);
}
/*-----*/

// Returns Rsmt Error in region
double DoRSMT::GetRsmtStatError(int region) {
	double r_smt = this->GetRsmt(region);
	double pretag_yield = this->GetCorrectedRegionYield("pretag", region);
	return sqrt(r_smt * (1 - r_smt) / pretag_yield);
}
/*-----*/

// Returns Rsmt Syst Error in region
double DoRSMT::GetRsmtSystError(int region) {
	DoRSMT* up = new DoRSMT(jet_bin_, is_inclusive_, 2);
	DoRSMT* down = new DoRSMT(jet_bin_, is_inclusive_, 0);
	double rsmt_up = up->GetRsmt(region);
	double rsmt_down = down->GetRsmt(region);
	double nominal = this->GetRsmt(region);

	double error = std::max(fabs(rsmt_up - nominal), fabs(rsmt_down - nominal));

	delete up;
	delete down;

	return error;
}
/*-----*/

//
// RSMTWGT SECTION
//
// Returns Weighted Rsmt
double DoRSMT::GetRsmtWgt() {

	double r_smt_A = this->GetRsmt(AbcdBase::A);
	double r_smt_B = this->GetRsmt(AbcdBase::B);
	double r_smt_C = this->GetRsmt(AbcdBase::C);

	return (r_smt_A + r_smt_B + r_smt_C) / 3;
}
/*-----*/

// Get stat error on RsmtWGT
double DoRSMT::GetRsmtWgtStatErr() {
	return sqrt(this->GetSumOfErrors());
} // End of GetRsmtWgtStatErr
/*-----*/

// Get syst error on RsmtWGT
double DoRSMT::GetRsmtWgtSystErr() {

	double r_smt_A = this->GetRsmt(AbcdBase::A);
	double r_smt_B = this->GetRsmt(AbcdBase::B);
	double r_smt_C = this->GetRsmt(AbcdBase::C);

	std::vector<double> regions;
	regions.push_back(r_smt_A);
	regions.push_back(r_smt_B);
	regions.push_back(r_smt_C);

	std::vector<double>::iterator smallest = std::min_element(regions.begin(), regions.end());
	std::vector<double>::iterator largest = std::max_element(regions.begin(), regions.end());

	double max_syst = (*largest - *smallest) / *smallest;
	return (max_syst / 2);
} // End of GetRsmtWgtStatErr
/*-----*/

// Returns sum of errors
double DoRSMT::GetSumOfErrors() {
	double r_smt_err_A = this->GetRsmtStatError(AbcdBase::A);
	double r_smt_err_B = this->GetRsmtStatError(AbcdBase::B);
	double r_smt_err_C = this->GetRsmtStatError(AbcdBase::C);

	return (1 / 9) * (r_smt_err_A * r_smt_err_A) + (r_smt_err_B * r_smt_err_B)
			+ (r_smt_err_C * r_smt_err_C);
}
/*-----*/

// Returns the estimate based on the pretag * rsmt
double DoRSMT::GetTagEstimate(void) {

	double pre_estimate = this->GetPretagEstimate();
	double rsmt_wgt = this->GetRsmtWgt();

#ifdef DEBUG
	std::cout << "DoRSMT::GetTagEstimate - Pretag Est: " << pre_estimate
	<< std::endl;
	std::cout << "DoRSMT::GetTagEstimate - Rsmt Wgt: " << rsmt_wgt << std::endl;
#endif

	return pre_estimate * rsmt_wgt;
} // End of GetTagEstimate
/*-----*/

// Assumes a 50% uncertainty
double DoRSMT::GetTagEstimateStatError(void) {
	double rsmt_bit = this->GetRsmtWgtStatErr() / this->GetRsmtWgt();
	double estimate = this->GetTagEstimate();

	return estimate * rsmt_bit;
} //
/*-----*/

// TODO - Fill this in.
double DoRSMT::GetTagEstimateSystError(void) {

	double r_smt_wgt_err = this->GetRsmtWgtSystErr();

	double pretag_estimate = this->GetPretagEstimate();
	double pretag_estimate_err = pretag_estimate / 2;
	double pretag_estimate_bit = pretag_estimate_err / pretag_estimate;

	double tag_estimate = this->GetTagEstimate();

	return tag_estimate
			* sqrt(pretag_estimate_bit * pretag_estimate_bit
							+ r_smt_wgt_err * r_smt_wgt_err);
} //

/*-----*/

//
TString DoRSMT::GetLabel() {
	TString suffix = "";
	if (is_inclusive_ != 0)
		suffix = "inc ";

// 3 jet inc (pretag)
	TString label = Form("%i jet %s", jet_bin_, suffix.Data());

	return label;
} //

ReaderCollection& DoRSMT::GetCollection(TString mode) {
	if (mode.Contains("pretag")) {
		return reader_collection_pretag;
	} else {
		return reader_collection_tag;
	}
} //

// Get pretag Estimates
double DoRSMT::GetPretagEstimate() {

	int array_bin = jet_bin_ - 1;

	double estimates[] = { 110678, 29323, 7924, 2377, 3434 };
	double estimate = 0.;
	if (is_inclusive_ != 1) {
		estimate = estimates[array_bin];
	} else {
		if (jet_bin_ == 3) {
			estimate = estimates[2] + estimates[4];
		} else if (jet_bin_ == 4) {
			estimate = estimates[4];
		}
	}
	return estimate;
}
/*-----*/
