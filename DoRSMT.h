/**
 * @file DoRSMT.h
 * @brief DoRSMT Definition
 * @date Jul 10, 2012
 * @author Jacobo Blanco (jayb88@cern.ch)
 */

#ifndef DORSMT_H_
#define DORSMT_H_

// Includes
#include "ABCDReader.h"
#include <map>

/**
 * @class DoRSMT
 * @brief Does the MatrixMethod based analysis by calculating a tagging rate
 *
 *
 */
class DoRSMT {

private:

	ReaderCollection reader_collection_pretag; /**< Container of Pretag ABCDReader objects */
	ReaderCollection reader_collection_tag; /**< Container of Tagged ABCDReader objects */

	std::vector<std::string> list_of_samples; /**< List of samples */

	int jet_bin_; /**< Jet bin */
	bool is_inclusive_; /**< Is inclusive sample */
	int sys_mode_; /**< Systematic variation mode */
	TString br_sys_mode_; /**< BR variation mode */

	/**
	 * Fills the ReaderCollection with pretag and tagged samples
	 */
	void init(void);

	/**
	 * Returns the pretag estimate from a table. These are provided by the Top Fakes
	 * group
	 *
	 * @return
	 *   Pretag estimate
	 */
	double GetPretagEstimate(void);

	/**
	 * Returns the label for the sample. Used for table output.
	 *
	 * @return
	 *   Label for tabulation
	 */
	TString GetLabel(void);

	/**
	 * Returns the collection of ABCDReader objects for a given mode
	 *
	 * @param mode
	 *   pretag or tag
	 */
	ReaderCollection& GetCollection(TString mode);

	/**
	 * Returns the sum of errors
	 *
	 * @return
	 *   Sum of errors
	 */
	double GetSumOfErrors(void);

public:

	/**
	 * Main constructor builds an Rsmt object given sample parameters
	 *
	 * The DoRSMT object setups all the analysis objects needed to obtain the tagging
	 * rates for a specific sample given a few parameters:
	 *
	 * @param jet_bin
	 *   Jet bin
	 * @param is_inclusive
	 *   TRUE for inclusive, FALSE for exclusive
	 * @param sys_mode
	 *   Systematic variation: UP (2), NONE (1), DOWN (0)
	 * @param br_sys_mode
	 *   BR systematic
	 */
	DoRSMT(int jet_bin = 3, bool is_inclusive = true, int sys_mode = 1,
			TString br_sys_mode = "");

	/**
	 * Destructor
	 */
	virtual ~DoRSMT();

	/**
	 * Print out estimate table
	 *
	 * @param mode
	 *   pretag or tag
	 * @param table_mode
	 *   Table format
	 *
	 * @todo change table_mode to enum from AbcdBase
	 */
	void PrintEstimateTable(TString mode, TString table_mode);

	/**
	 * Print out table of tagging rate
	 *
	 * @param table_mode
	 *   Format of table
	 *
	 * @todo change table_mode to enum from AbcdBase
	 */
	void PrintRsmtTable(TString table_mode);

	/**
	 * Returns Data region yield
	 *
	 * @param mode
	 *   pretag or tab
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Data region yield
	 *
	 * @todo Implement enum for region index
	 */
	double GetDataRegionYield(TString mode, int region);

	/**
	 * Returns correction (sum of MC) for the region
	 *
	 * @param mode
	 *   pretag or tag
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Correction for region
	 *
	 * @todo Implement enum for region index
	 */
	double GetCorrection(TString mode, int region);

	/**
	 * Returns corrected region yield
	 *
	 * @param mode
	 *   pretag or tag
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Corrected region yield
	 *
	 * @todo Implement enum for region index
	 */
	double GetCorrectedRegionYield(TString mode, int region);

	/**
	 * Returns error for region
	 *
	 * @param mode
	 *   pretag or tag
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Region error
	 *
	 * @todo Implement enum for region index
	 */
	double GetRegionError(TString mode, int region);

	/**
	 * Returns tagging-rate for region
	 *
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Region tagging-rate
	 *
	 * @todo Implement enum for region index
	 */
	double GetRsmt(int region);

	/**
	 * Returns statistical error on tagging-rate for region
	 *
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Statistical error on tagging-rate
	 *
	 * @todo Implement enum for region index
	 */
	double GetRsmtStatError(int region);

	/**
	 * Returns systematics error on tagging-rate for region
	 *
	 * @param region
	 *   Region index
	 *
	 * @return
	 *   Systematics error on tagging-rate
	 *
	 * @todo Implement enum for region index
	 */
	double GetRsmtSystError(int region);

	/**
	 * Returns the tagging-rate average
	 *
	 * @return
	 *   Average of tagging-rate
	 */
	double GetRsmtWgt(void);

	/**
	 * Returns the statistical uncertainty on tagging-rate average
	 *
	 * @return
	 *   Statistical uncertainty on average of tagging-rate
	 */
	double GetRsmtWgtStatErr(void);

	/**
	 * Returns the systematic uncertainty on tagging-rate average
	 *
	 * @return
	 *   Systematic uncertainty on average of tagging-rate
	 */
	double GetRsmtWgtSystErr(void);

	/**
	 * Returns the tag estimate of multijet
	 *
	 * The estimate is calculated by multiplying the pretag estimate by the
	 * tagging-rate.
	 *
	 * @return
	 *   Tagged estimate
	 */
	double GetTagEstimate(void);

	/**
	 * Returns the systematic uncertainty on the tag estimate of multijet background
	 *
	 * The estimate is calculated by multiplying the pretag estimate by the
	 * tagging-rate.
	 *
	 * @return
	 *   Systematic uncertainty on tagged estimate
	 */
	double GetTagEstimateSystError(void);

	/**
	 * Returns the statistical uncertainty on the tag estimate of multijet background
	 *
	 * The estimate is calculated by multiplying the pretag estimate by the
	 * tagging-rate.
	 *
	 * @return
	 *   Statistical uncertainty on tagged estimate
	 */
	double GetTagEstimateStatError(void);
};

#endif /* DORSMT_H_ */
