/*
 * AbcdBase.h
 *
 *  Created on: Dec 9, 2011
 *      Author: jayb88
 */

#ifndef ABCDBASE_H_
#define ABCDBASE_H_

#include <string>
#include <map>
#include <iostream>
#include "TRoot.h"

class AbcdBase {

private:
	int init();
	int status;
	TString getErrorString(std::vector<double>& errors, TString mode);

public:

	static AbcdBase* baseInst;

	static std::string labelMet;
	static std::string labelEtcone20;
	static std::string labelIsolation;

	AbcdBase() : status(0) {
	}
	virtual ~AbcdBase() {
	}
	void isDeadFile(TFile* file);
	void drawLine(void);
	void printTableLine(TString label, double central, std::vector<double>& error, TString mode = "nice");

	typedef enum {
		A = 1, B = 2, C = 3, D = 4
	} RegionEnum;
	typedef enum {
		Nice, Central, Syst, Stat, Latex, Twiki, NONE
	} TableEnum;
	typedef enum {
		MET = 1, ETCONE20 = 2
	} DimensionEnum;
	typedef enum {
		dataAllEgamma, ttbarSig, WjetsScaled, Zjets, singleTop, diBoson
	} SampleEnum;

	static TString pm;
	static TString pm_twiki;


ClassDef(AbcdBase, 1)
};

#endif /* ABCDBASE_H_ */
