/**
 * @file ABCDReader.h
 * @brief ABCDReader Definition
 * @date Jul 10, 2012
 * @author Jacobo Blanco (jayb88@cern.ch)
 */

#ifndef ABCDREADER_H_
#define ABCDREADER_H_

// Includes
#include <vector>
#include "TString.h"
#include "DataSample.h"

/**
 * @class ABCDReader
 * @brief A type of wrapper that gives you easier access to ABCD variables
 *
 * The ABCD reader allows access to the yields in each control and their errors.
 * DataSample objects are handed over to the ABCDReader object and in return one can obtain
 * the yields and uncertainties associated with each region of the ABCD plane defined in the
 * DataSample class
 */
class ABCDReader {

private:

	// DataSample file
	DataSample* sample_;

	// Regions
	// Region A Yield
	double nA_;
	// Region B Yield
	double nB_;
	// Region C Yield
	double nC_;
	// Region D Yield
	double nD_;

	// Errors
	// Region A Error
	double nAError_;

	// Region A Error
	double nBError_;

	// Region A Error
	double nCError_;

	// Region A Error
	double nDError_;

	// Is file zombie
	int isZombie_;

	// Pretag or tagged sample
	TString mode_;

	// Jet bin for sample
	int jet_bin_;
	bool is_inclusive_;
	int sys_mode_;
	TString br_sys_;

	/**
	 * Extracts the yields and errors from each region and stores as members
	 *
	 * This function extracts the yields of each region in the ABCD plane, based on the
	 * datasample used and saves them as data members. Only runs once.
	 */
	void setRegionIntegralsAndErrors();

	/**
	 * Extract the yield for the region requested from the DataSample object
	 *
	 * Returns the number of events in the region requested by calling on DataSample::getYield.
	 * The variable is stored as a member variable for faster re-extraction. This is called by
	 * setRegionIntegralsAndErrors when the object is constructed. Only runs per region.
	 *
	 * @param region
	 *   The index of the region desired
	 * @todo - use enums from AbcdBase
	 *
	 * @return
	 *   A constant which is the yield in region
	 */
	const double GetYieldFromSample(int region);

	/**
	 * Extract the uncertainty for the region requested from the DataSample object
	 *
	 * Returns the statistical uncertainty in the region requested by calling on DataSample::getError.
	 * The variable is stored as a member variable for faster re-extraction. This is called by
	 * setRegionIntegralsAndErrors when the object is constructed. Only runs per region.
	 *
	 * @param region
	 *   The index of the region desired
	 * @todo - use enums from AbcdBase
	 *
	 * @return
	 *   The uncertainty associated with the region
	 */
	const double GetYieldErrorFromSample(int region);

	/**
	 * Returns the scaling factor used when obtaining systematic uncertainties
	 *
	 * This function returns the scale factor by which the sample is scaled
	 * for the purpose of calculating systematic uncertainty. Since MC samples
	 * are used to correct the yield in all control regions for both ABCD and
	 * MatrixMethod, it is important to take into consideration the uncertainty
	 * on the theoretical cross-section for tt-bar and the normalization of W+jets
	 * which is obtained with a data-driven estimate. Conservative values for
	 * these uncertainties are used. These are 25% and 15% for W+jets and tt-bar
     * respectively. Note that all parameters needed to figure out the correct
     * scaling are provided when constructing this object. This function is used
     * internally only.
     *
     * @return
     *   Scale factor to reweight sample by for systematics calculation
	 *
	 */
	double GetSystFactor(void);

public:

	/**
	 * Normal constructor for ABCDReader object
	 *
	 * Creates the ABCDReader structure based on the sample provides.
	 * The definition of the sub-sample used is determine by:
	 *
	 * @param sample
	 *   Object of type DataSample from which to extract historgrams.
	 * @param mode
	 *   Extract tagged or pretag samples. Set to tagged or pretag.
	 * @param jet_bin
	 *   Sets the Jet bin to yields to extract. Options are 1, 2, 3 or 4 jets.
	 * @param is_inclusive
	 *   TRUE to extract inclusive or FALSE for exclusive yields.
	 * @param sys_mode
	 *   Sets the scaling on sample -> 0 (Down), 1 (None), 2 (Up).
	 * @param br_sys
	 *   A string which determines which BR variation to apply.
	 *
	 * @todo Implement an enumerator for br_sys.
	 * @todo Implement an enumerator for sys_mode.
	 * @todo Implement an enumerator for mode.
	 */
	ABCDReader(DataSample* sample, TString mode, int jet_bin, bool is_inclusive,
			int sys_mode, TString br_sys);

	/**
	 * Destructor
	 */
	virtual ~ABCDReader(void);

	/**
	 * Returns the regional yield.
	 *
	 * @param region
	 *   Region to consider.
	 *
	 * @return
	 *   Region yield.
	 */
	double GetRegionYield(int region);

	/**
	 * Returns the regional error.
	 *
	 * @param region
	 *   Region to consider.
	 *
	 * @return
	 *   Region statistical uncertainty.
	 */
	double GetRegionError(int region);

	/**
	 * Returns the estimate of region D (no subtraction correction).
	 *
	 * @return
	 *   Estimate of region D yield.
	 */
	const double GetNdEstimate(void);

	// Needed by root
	ClassDef(ABCDReader,1)
};

/**
 * Container of ABCDReader objects
 */
typedef std::map<TString, ABCDReader*> ReaderCollection;

#endif /* ABCDReader_H_ */
