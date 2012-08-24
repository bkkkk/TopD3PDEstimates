/*
 * DataSample.cpp
 *
 *  Created on: Jul 3, 2012
 *      Author: jayb88
 */

#include "DataSample.h"
#include <iostream>
#include "math.h"
// #define DEBUG

DataSample::DataSample(TString sample_name_) :
		sample_name(sample_name_),
		file(NULL),
		histo_database(NULL){
	
#ifdef DEBUG 
		std::cout << "DataSample::Constructor - Debug Point 1" << std::endl;
#endif 

	this->init();
}

DataSample::~DataSample() {
#ifdef DEBUG
	std::cout << "---| DataSample::~DataSample Calling Destructor" << std::endl;
#endif

	file->Close();
	delete file;
	sample_name = "";
	sample_path = "";
	sample_full_name = "";
}

void DataSample::init() {

#ifdef DEBUG 
		std::cout << "DataSample::init() - Debug Point 1" << std::endl;
#endif 

	this->SetSamplePath(Form("./TopD3PDHistos_%s_el.root", sample_name.Data()));
	file = new TFile(sample_path);
}

std::vector<TH1D*> DataSample::GetHistos(TString mode, TString sys_br) {
	std::vector<TH1D*> histos;

	TString suffix = "h_njet";
	TString regions[] = { "A", "B", "C", "D" };
	TString sys_br_full = "";

	// If not TTbar ignore BR syst else make name
	if(sys_br == "") {
		sys_br_full = "";
	} else {
		sys_br_full = "_wgt_" + sys_br;
	}
	
	for (int region_idx = 0; region_idx != 4; region_idx++) {
		TString region_label = regions[region_idx];
		
#ifdef DEBUG 
		std::cout << "DataSample::GetHistos - Debug Point Histogram Name: " << 
					Form("%s_%s_%s_el%s", suffix.Data(), mode.Data(),
						  region_label.Data(), sys_br_full.Data()) << std::endl;
#endif 
		histos.push_back(
				(TH1D*) file->Get(
						Form("%s_%s_%s_el%s", suffix.Data(), mode.Data(),
								region_label.Data(), sys_br_full.Data())));
	}
	return histos;
}

const double DataSample::GetYield(TString mode, int region, int jet_bin,
		bool is_inclusive, TString br_sys = "") {

#ifdef DEBUG 
		std::cout << "DataSample::GetYield - Debug Point 1" << std::endl;
#endif 
	int histo_bin = (jet_bin + 1);
	double bin_content;

	TH1D* histo_to_consider = this->GetHistos(mode, br_sys).at(region);

	if (is_inclusive != 0) {
		bin_content = histo_to_consider->Integral(histo_bin, 19);
	} else {
		bin_content = histo_to_consider->GetBinContent(histo_bin);
	} // End if is Inclusive
	
#ifdef DEBUG 
	std::cout << "DataSample::GetYield - Debug Point 2" << std::endl;
#endif 

	return bin_content;
} // End GetYield

const double DataSample::GetYieldError(TString mode, int region, int jet_bin,
		bool is_inclusive, TString br_sys = "") {
	int histo_bin = (jet_bin + 1);
	double bin_error = 0;
	double bin_content = 0;

	if (is_inclusive != 0) {
		bin_content = this->GetHistos(mode, br_sys).at(region)->IntegralAndError(
				histo_bin, 19, bin_error);
	} else {
		bin_error = this->GetHistos(mode, br_sys).at(region)->GetBinError(histo_bin);
	} // End if Is Inclusive

	return bin_error;
} // End GetYieldError

void DataSample::GetYields() {

	// Loop over modes
	TString modes[] = { "pretag", "tag" };

	TString br_sys = "";

	// Labels
	std::cout << "| *Jet-bin* | *A* | *B* | *C* | *D* |" << std::endl;

	// Simple loop over pretag and tag
	for (int mode_idx = 0; mode_idx != 2; mode_idx++) {
		TString mode = modes[mode_idx];

		// Loop over jet bins, 5 and 6 are for inc 3 an 4
		for (int jet_bin = 1; jet_bin != 7; jet_bin++) {

			int jet_bin_actual = jet_bin;
			bool inc = false;
			TString inc_label = "";

			// To Do inclusive
			if (jet_bin == 5 || jet_bin == 6) {
				inc = true;
				inc_label = "inc ";
				if (jet_bin == 5) {
					jet_bin_actual = 3;
				} else {
					jet_bin_actual = 4;
				}
			} else {
				inc_label = "";
				inc = false;
			} // End if jet_bin

			TString label = Form("%s(%s)", inc_label.Data(), mode.Data());
			// Jet bin label
			std::cout << "| " << jet_bin_actual << " " << label << " | ";

			// Regions selector
			for (int region = 0; region != 4; region++) {
				std::cout
						<< Form("%4.2f",
								this->GetYield(mode, region, jet_bin_actual,
										inc, br_sys)) << " | ";
			}
			std::cout << std::endl;
		}
	}
	return;
}

const double DataSample::GetContamination(TString mode, int region, int jet_bin,
		bool is_inclusive) {

	return 100 * this->GetYield(mode, region, jet_bin, is_inclusive, TString(""))
			/ this->GetDataYield(mode, region, jet_bin, is_inclusive);

}

const double DataSample::GetContaminationError(TString mode, int region,
		int jet_bin, bool is_inclusive) {

	double yield = this->GetYield(mode, region, jet_bin, is_inclusive, TString(""));
	double data = this->GetDataYield(mode, region, jet_bin, is_inclusive);
	double yield_error = this->GetYieldError(mode, region, jet_bin,
			is_inclusive, TString(""));
	double data_error = this->GetDataYieldError(mode, region, jet_bin,
			is_inclusive);

	double yield_sigma = yield_error / yield;
	double data_sigma = data_error / data;
	double cont = 100 * yield / data;

	return cont * sqrt((yield_sigma*yield_sigma)  + (data_sigma*data_sigma));
} // End GetContaminationError

void DataSample::GetContaminations() {

	// Loop over modes
	TString modes[] = { "pretag", "tag" };

	// Labels
	std::cout << "| *Jet-bin* | *A+-(stat) (%)* | *B+-(stat) (%)* | *C+-(stat) (%)* | *D+-(stat) (%)* |"
			<< std::endl;

	// Simple loop over pretag and tag
	for (int mode_idx = 0; mode_idx != 2; mode_idx++) {
		TString mode = modes[mode_idx];

		// Loop over jet bins, 5 and 6 are for inc 3 an 4
		for (int jet_bin = 1; jet_bin != 7; jet_bin++) {

			int jet_bin_actual = jet_bin;
			bool inc = false;
			TString inc_label = "";

			// To Do inclusive
			if (jet_bin == 5 || jet_bin == 6) {
				inc = true;
				inc_label = "inc ";
				if (jet_bin == 5) {
					jet_bin_actual = 3;
				} else if (jet_bin == 6) {
					jet_bin_actual = 4;
				}
			} else {
				inc_label = "";
				inc = false;
			} // End if jet_bin

			TString label = Form("%s(%s)", inc_label.Data(), mode.Data());
			// Jet bin label
			std::cout << "| " << jet_bin_actual << " " << label << " | ";

			// Regions selector
			for (int region = 0; region != 4; region++) {
				std::cout
						<< Form("%4.2f+-%4.2f",
								this->GetContamination(mode, region,
										jet_bin_actual, inc),
								this->GetContaminationError(mode, region,
										jet_bin_actual, inc)) << " | ";
			}
			std::cout << std::endl;
		}
	}
	return;
}

double DataSample::GetDataYield(TString mode, int region, int jet_bin,
		bool is_inclusive) {
	DataSample* data = new DataSample("dataAllEgamma");
	double yield_data = data->GetYield(mode, region, jet_bin, is_inclusive, TString(""));
	delete data;
	return yield_data;
}

double DataSample::GetDataYieldError(TString mode, int region, int jet_bin,
		bool is_inclusive) {
	DataSample* data = new DataSample("dataAllEgamma");
	double yield_data = data->GetYieldError(mode, region, jet_bin, is_inclusive, TString(""));
	delete data;
	return yield_data;
}
