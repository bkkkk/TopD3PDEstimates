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
std::string AbcdBase::labelEtcone20 = "E_{T}^{cone20} [GeV]";
std::string AbcdBase::labelMet = "E_{T}^{miss} [GeV]";
std::string AbcdBase::labelIsolation = "Isolation";
TString AbcdBase::pm = "$\\pm$";

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
