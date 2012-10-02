/**
 * @file    AbcdBase.cpp
 * @brief   AbcdBase Implementation
 * @author 	Jacobo Blanco (jayb88@cern.ch)
 * @date 	October 2012
 */

// Includes
#include "AbcdBase.h"
#include "TFile.h"
#include <map>
#include <string>

// ================================================================

// Used by ROOT
ClassImp(AbcdBase)

// ================================================================

AbcdBase* AbcdBase::baseInst = new AbcdBase();

std::string AbcdBase::labelEtcone20 = "E_{\rm T}^{cone20} [GeV]";

std::string AbcdBase::labelMet = "E_{\rm T}^{\rm miss} [GeV]";

std::string AbcdBase::labelIsolation = "Isolation";

TString AbcdBase::pm = "$\\pm$";

TString AbcdBase::pm_twiki = "<latex>\\pm</latex>";

// ================================================================

void AbcdBase::drawLine() {

	std::cout << "----------------------------------------------------"
			<< std::endl;
}

// ================================================================

void AbcdBase::isDeadFile(TFile* file) {

	if (file->IsZombie()) {
		std::cout << "Sample file NOT found" << std::endl;
		exit(-1);
	}
	return;
}

// ================================================================

void AbcdBase::printTableLine(TString label, double central,
		std::vector<double>& error, ETableFormat table_format,
		EErrorFormat error_format) {

	// Get string of errors formatted correctly
	TString s_error = this->getErrorString(error, table_format, error_format);

	// Output properly formatted line
	switch (table_format) {
	case Terminal:
	case Tabbed:
		std::cout << label << "\t" << std::fixed << central << s_error
				<< std::endl;
		break;
	case Central:
		std::cout << label << "\t" << std::fixed << central << std::endl;
		break;
	case CONF:
		std::cout << std::fixed << central << s_error << " & ";
		break;
	default:
		std::cout << label << " " << std::fixed << central << s_error
				<< std::endl;
		break;
	}

	return;
}

// ================================================================

TString AbcdBase::getErrorString(std::vector<double>& error,
		ETableFormat table_format, EErrorFormat error_format) {

	TString s_error = "";
	TString delimiter = "";
	double total_error = 0.;

	if (error_format == Total) {
		total_error = error.at(0);
	}

	// Pick correct uncertainty sign
	switch (table_format) {
	case Terminal:
		delimiter = "+-";
		break;
	case Tabbed:
		delimiter = "\t";
		break;
	case CONF:
		delimiter = AbcdBase::pm;
		break;
	case Twiki:
		delimiter = AbcdBase::pm_twiki;
		break;
	}

	// Output error string correctly formatted
	std::vector<double>::iterator iter = error.begin();
	std::vector<double>::iterator iter_end = error.end();
	if (error_format == Breakdown) {
		for (; iter != iter_end; ++iter) {
			s_error.Append(delimiter);
			s_error.Append(*iter);
		} // End for looping over errors
	} else {
		s_error.Append(delimiter);
		s_error.Append(total_error);
	} // End of error output

	return s_error;
}

// ================================================================
