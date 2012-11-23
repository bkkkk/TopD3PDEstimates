#ifndef ABCDBASE_H_
#define ABCDBASE_H_

/**
 * @file    AbcdBase.h
 * @brief   AbcdBase Definition
 * @author 	Jacobo Blanco (jayb88@cern.ch)
 * @date 	October 2012
 */

// Includes
#include <string>
#include <map>
#include <iostream>
#include "TRoot.h"

/**
 * @class AbcdBase
 *
 * @brief Base class which allows access to various useful tools and information.
 *
 * Contains output tools, labels for different variables, enums to identify regions
 * in the ABCD plane, dataset types and table formats. Common string labels used
 * for result output
 */
class AbcdBase {

public:

	static AbcdBase* baseInst; 			/**< Static instance of AbcdBase. */
	static std::string labelMet; 		/**< Histogram label for Missing Energy. */
	static std::string labelEtcone20; 	/**< Histogram label for etcone20. */
	static std::string labelIsolation; 	/**< Histogram label for isolation */
	static TString pm;					/**< Uncertainty symbol. */
	static TString pm_twiki; 			/**< Uncertainty symbol in latex format */

	/**
	 * Pretag/Tagged
	 * Indexes for pretag and tagged selection
	 */
	typedef enum {
		Pretag,	/**< Pretag index */
		Tagged	/**< Tagged index */
	} EModeType;

	/**
	 * ABCD Regions
	 * Contains region indexes
	 */
	typedef enum {
		A = 1, B = 2, C = 3, D = 4
	} ERegionEnum;

	/**
	 * Table formats
	 * Contains the indexes for table styles
	 */
	typedef enum {
		Terminal, 		/**< Terminal Style */
		Tabbed, 		/**< Tabbed Styles */
		Central, 		/**< Central Value Only */
		CONF, 			/**< CONF Note Style */
		Twiki			/**< Twiki Style */
	} ETableFormat;

	/**
	 * Error formats
	 * Index for error format
	 */
	typedef enum {
		Breakdown, 	/**< Print out uncertainties separately */
		Total		/** Print out total uncertainty */
	} EErrorFormat;

	/**
	 * Type of variable
	 * Index for variable recognition
	 */
	typedef enum {
		MET = 1, 		/**< MET dimension of ABCD Plane*/
		ETCONE20 = 2 	/**< Etcone20 dimension of ABCD Plane*/
	} EDimensions;

	/**
	 * List of datasets
	 * Contains indexes of usable datasets
	 */
	typedef enum {
		dataAllEgamma, 	/**< Data */
		ttbarSig, 		/**< Ttbar MC */
		WjetsScaled, 	/**< W+jets MC */
		Zjets, 			/**< Z+jets MC */
		singleTop,		/**< SingleTop MC */
		diBoson			/**< Diboson MC */
	} ESampleSets;

	/**
	 * List of BR systematics
	 */
	typedef enum {
		B_MU_UP = 11,			/**< B->Mu Up Variation */
		B_MU_DOWN = 12,			/**< B->Mu Down Variation */
		B_TAU_MU_UP = 21,		/**< B->Tau Up Variation */
		B_TAU_MU_DOWN = 22,		/**< B->Mu Down Variation */
		B_D_MUPLUS_UP = 31,		/**< B->Mu+ Up Variation */
		B_D_MUPLUS_DOWN = 32,	/**< B->Mu+ Down Variation */
		B_D_MUMINUS_UP = 41,	/**< B->Mu- Up Variation */
		B_D_MUMINUS_DOWN = 42,	/**< B->Mu- Down Variation */
		C_MU_UP = 51,			/**< c->Mu Up Variation */
		C_MU_DOWN = 52			/**< c->Mu Down Variation */
	} EBrVariations;

	/**
	 * Basic constructor
	 */
	AbcdBase() : status(0) { }

	/**
	 * Basic destructor
	 */
	virtual ~AbcdBase() { }

	/**
	 * Checks if file is not dead, exits on file dead
	 *
	 * @param file
	 *   A TFile object to be tested
	 */
	void isDeadFile(TFile* file);

	/**
	 * Outputs a line of dashes
	 */
	void drawLine(void);

	/**
	 * Print line of table
	 *
	 * @param label
	 *   A string with the label of the row
	 * @param central
	 *   Double of central value of measurement
	 * @param error
	 *   A vector of uncertainties
	 * @param table_format
	 *   Formatting style for output. See ETableFormat for options.
	 * @param error_format
	 *   Formatting style for error. See EErrorFormat for options.
	 * @see ETableFormat
	 * @see EErrorFormat
	 */
	void printTableLine(TString label, double central,
			std::vector<double>& error, ETableFormat table_format, EErrorFormat error_format);

private:

	/**
	 * Initialises tools
	 *
	 * @return
	 *   Return currently not used
	 */
	int init();

	/**
	 * Return string of errors with correct formatting
	 *
	 * @param errors
	 *   A vector with the uncertainty components including
	 *   statistical, systematic and others
	 * @param table_format
	 *   Formatting style to be used. See ETableFormat for options.
	 *  @param error_format
	 *   Style of output for uncertainty. See EErrorFormat for options.
	 *
	 * @see ETableFormat
	 * @see EErrorFormat
	 *
	 * @return
	 *   TString with formatted string with the label, central value and uncertainties
	 */
	TString getErrorString(std::vector<double>& errors, ETableFormat table_format, EErrorFormat error_format);

	// Status of object, currently NOT used
	int status;

	// Used by ROOT
	ClassDef(AbcdBase, 1)
};

#endif /* ABCDBASE_H_ */
