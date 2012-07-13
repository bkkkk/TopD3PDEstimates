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

public:

	static AbcdBase* baseInst;

	static std::string labelMet;
	static std::string labelEtcone20;
	static std::string labelIsolation;

	AbcdBase() {
	}
	virtual ~AbcdBase() {
	}
	void isDeadFile(TFile* file);
	void drawLine(void);

	typedef enum {
		A = 1, B = 2, C = 3, D = 4
	} RegionEnum;
	typedef enum {
		MET = 1, ETCONE20 = 2
	} DimensionEnum;

	static TString pm;


ClassDef(AbcdBase, 1)
};

#endif /* ABCDBASE_H_ */
