/*
 * AbcdBase.cpp
 *
 *  Created on: Dec 9, 2011
 *      Author: jayb88
 */

#include "AbcdBase.h"
#include "TFile.h"
#include <map>
#include <string>

ClassImp(AbcdBase)

AbcdBase* AbcdBase::baseInst = new AbcdBase();
std::string AbcdBase::labelEtcone20 = "E_{\rm T}^{cone20} [GeV]";
std::string AbcdBase::labelMet = "E_{\rm T}^{\rm miss} [GeV]";
std::string AbcdBase::labelIsolation = "Isolation";
TString AbcdBase::pm = "$\\pm$";
TString AbcdBase::pm_twiki = "<latex>\\pm</latex>";

void AbcdBase::drawLine() {
	std::cout << "----------------------------------------------------"
			<< std::endl;
}

// Extra functions for testing validity of files
void AbcdBase::isDeadFile(TFile* file) {
	if (file->IsZombie()) {
		std::cout << "Sample file NOT found" << std::endl;
		exit(-1);
	}
	return;
}

void AbcdBase::printTableLine(TString label, double central,
		std::vector<double>& error, TString mode) {

	TString s_error = this->getErrorString(error, mode);

	if (mode.Contains("Terminal") || mode.Contains("Tabbed")) {
		std::cout << label << "\t" << std::fixed << central << s_error << std::endl;
	} else if (mode.Contains("Central")) {
		std::cout << label << "\t" << std::fixed << central << std::endl;
	} else if (mode.Contains("CONF")) {
		std::cout << std::fixed << central << s_error << " & ";
	} else {
		std::cout << label << " " << std::fixed << central << s_error
				<< std::endl;
	}
	return;
}

TString AbcdBase::getErrorString(std::vector<double>& error, TString mode) {

	TString s_error = "";
	TString delimiter = "";
	double total_error = 0.;

	if (!(mode.Contains("Full"))) {
		total_error = error.at(0);
	}
	std::vector<double>::iterator iter = error.begin();
	std::vector<double>::iterator iter_end = error.end();

	if (mode.Contains("Terminal")) {
		delimiter = "+-";
	} else if (mode.Contains("Tabbed")) {
		delimiter = "\t";
	} else if (mode.Contains("CONF")) {
		delimiter = AbcdBase::pm;
	} else if (mode.Contains("Twiki")) {
		delimiter = AbcdBase::pm_twiki;
	}

	// Full BreakDown Mode
	if (mode.Contains("Full")) {
		for (; iter != iter_end; ++iter) {
			s_error.Append(delimiter);
			s_error.Append(*iter);
		}
	} else {
		s_error.Append(delimiter);
		s_error.Append(total_error);
	} // End if Full Mode

	return s_error;
}
