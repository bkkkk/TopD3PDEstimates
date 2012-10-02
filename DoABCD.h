/**
 * @file 	DoABCD.h
 * @brief 	DoABCD
 * @author 	Jacobo Blanco (jayb88@cern.ch)
 * @date 	Oct. 2011
 */

// Includes
#include <map>
#include "ABCDReader.h"
#include "DataSample.h"

#ifndef DOABCD_H_
#define DOABCD_H_

/**
 * @class DoABCD
 * @brief Does the ABCD analysis.
 *
 * This class creates a set of ABCDReader objects to calculate the estimate
 * of the multijet background after subtracting all MC contributions.
 */
class DoABCD {

private:

	ReaderCollection reader_collection;		/**< A collection ABCDReader */
	std::vector<std::string> listOfSamples; /**< List of samples */

	TString mode_;		/**< Pretag/Tag */
	bool doInclusive_;  /**< Do inclusive or not */
	int jet_bin_;		/**< Jet bin */
	int sysMode_;		/**< Systematic mode up or down */
	TString br_sys_;	/**< BR systematic */

	/**
	 * Initialises ABCDReader collection
	 */
	void init(void);

public:

	/**
	 * Main constructor
	 *
	 * @param mode
	 *   Pretag/Tagged
	 * @param doInclusive_
	 *   TRUE for inclusive analysis, FALSE for exclusive
	 * @param jet_bin
	 *   Jet bin to consider
	 * @param sysMode
	 *   Systematic variation UP/DOWN
	 * @param br_sys
	 *   BR variation to consider
	 */
	DoABCD(TString mode = "tag", bool doInclusive_ = false, int jet_bin = 3, int sysMode = 1, TString br_sys = "");

	/**
	 * Destructor
	 */
	virtual ~DoABCD();

	/**
	 * Print a table of estimate for region D
	 *
	 * @param table_mode
	 *   Table style
	 * @todo Change to enum from AbcdBase
	 */
	void printNdEstimateTable(TString table_mode);

	/**
	 * Returns data region yield
	 *
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Data Region Yield
	 */
	double getDataRegionYield(int region);

	/**
	 * Returns the correction (MC yield)
	 *
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Sum of MC contribution for region
	 */
	double getCorrection(int region);

	/**
	 * Returns the corrected (Data-MC) yield in region
	 *
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Corrected region yield
	 */
	double getCorrectedRegionYield(int region);

	/**
	 * Returns the error associated with region
	 *
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Error in region
	 */
	double getRegionError(int region);

	/**
	 *  Returns the corrected estimate of region D multijet
	 *
	 *  @return
	 *    Corrected estimate
	 */
	double getNdEstimate(void);

	/**
	 *  Returns error on estimate of region D multijet
	 *
	 *  @return
	 *    Error on estimate
	 */
	double getNdError(void);

	/**
	 *  Returns the systematic uncertainty associated with the estimate of region D
	 *
	 *  @return
	 *    Systematic uncertainty on D
	 */
	double getNdSystError(void);

	/**
	 * Get label for table output
	 *
	 * @todo Consider moving to AbcdBase
	 *
	 * @return
	 *   String with label
	 */
	TString getLabel(void);

	// Needed by ROOT
	ClassDef(DoABCD,1)
};

#endif /* DOABCD_H_ */
