/**
 * @file DataSample.h
 * @brief DataSample Definition
 * @date Jul 3, 2012
 * @author Jacobo Blanco (jayb88@cern.ch)
 */

// Includes
#include "TString.h"
#include "TH1D.h"
#include "TFile.h"
#include <map>

#ifndef DATASAMPLE_H_
#define DATASAMPLE_H_

/**
 * @class DataSample
 * @brief A wrapper for TFiles that provide extra information and access to yields.
 *
 * A DataSample object loads histograms from a TFile associated with a specific
 * data sample, be it MC or data. It allows access to yields and uncertainties
 * associated with the various regions of the MET vs Iso plane (named A, B, C and D).
 * Various output options are available for debugging purposes such a full tabulated
 * printout of all yields in all regions for all jet bins at pretag and tagged level
 */
class DataSample {

public:

	/**
	 * Main constructor of DataSample
	 *
	 * Creates a DataSample object based on the sample name. Note that the
	 * sample name MUST match the file naming scheme.
	 *
	 * @param sample_name_
	 *   Name of sample, has to match naming scheme
	 */
	DataSample(TString sample_name_);

	/**
	 * Destructor
	 */
	virtual ~DataSample();

	/**
	 * Get name of sample
	 *
	 * @return
	 *   Sample name
	 */
	TString GetSampleName() const {
		return sample_name;
	}

	/**
	 * Set sample name
	 *
	 * @param sampleName
	 *   Sample name
	 */
	void SetSampleName(TString sampleName) {
		sample_name = sampleName;
	}

	/**
	 * Get sample path
	 *
	 * @return
	 *   Sample path
	 */
	TString GetSamplePath() const {
		return sample_path;
	}

	/**
	 * Set sample path
	 *
	 * @param samplePath
	 *   Sample path
	 */
	void SetSamplePath(TString samplePath) {
		sample_path = samplePath;
	}

	/**
	 * Opens the file corresponding to sample based on sample name
	 */
	void init(void);

	/**
	 * Returns a vector of histograms from the file based on parameters.
	 *
	 * Extract histograms from the TFile object and saves them into a histogram
	 * The histograms extracted can be set by:
	 *
	 * @param mode
	 *   Pretag or tagged
	 * @param br_sys
	 *   Specific BR variation name
	 *
	 * @return
	 *   Vector of histograms
	 */
	std::vector<TH1D*> GetHistos(TString mode, TString br_sys);

	/**
	 * Returns yield for a specific sample
	 *
	 * This function returns the yield associated with a specific sample.
	 * The yield to return is determined by:
	 *
	 * @param mode
	 *   Pretag for the pretag sample or tagged for the tagged sample
	 * @param region
	 *   Region index to extract
	 * @param jet_bin
	 *   Jet bin to consider
	 * @param is_inclusive
	 *   Run inclusive or not
	 * @param br_sys
	 *   Branching ratio systematic to extract yield for
	 *
	 * @return
	 *   Yield of sample
	 */
	const double GetYield(TString mode, int region, int jet_bin,
			bool is_inclusive, TString br_sys);

	/**
	 * Returns error for a specific sample
	 *
	 * This function returns the error associated with a specific sample.
	 * The error to return is determined by:
	 *
	 * @param mode
	 *   Pretag for the pretag sample or tagged for the tagged sample
	 * @param region
	 *   Region index to extract
	 * @param jet_bin
	 *   Jet bin to consider
	 * @param is_inclusive
	 *   Run inclusive or not
	 * @param br_sys
	 *   Branching ratio systematic to extract yield for
	 *
	 * @return
	 *   Error on yield of sample
	 */
	const double GetYieldError(TString mode, int region, int jet_bin,
			bool is_inclusive, TString br_sys);

	/**
	 * Returns the yield as a percentage of Data yield
	 *
	 * @param mode
	 *   Pretag for the pretag sample or tagged for the tagged sample
	 * @param region
	 *   Region index to extract
	 * @param jet_bin
	 *   Jet bin to consider
	 * @param is_inclusive
	 *   Run inclusive or not
	 *
	 * @return
	 *   Contamination percentage of sample
	 */
	const double GetContamination(TString mode, int region, int jet_bin,
			bool is_inclusive);

	/**
	 * Returns the percentage error associated with the relative yield (to Data)
	 *
	 * @param mode
	 *   Pretag for the pretag sample or tagged for the tagged sample
	 * @param region
	 *   Region index to extract
	 * @param jet_bin
	 *   Jet bin to consider
	 * @param is_inclusive
	 *   Run inclusive or not
	 *
	 * @return
	 *   Error associated with the contamination
	 */
	const double GetContaminationError(TString mode, int region, int jet_bin,
			bool is_inclusive);

	/**
	 * Prints out a table of yields associated with this sample
	 */
	void GetYields(void);

	/**
	 * Prints out a table of contaminations (Sample/Data) associated with this sample
	 */
	void GetContaminations(void);

private:

	/**
	 * Returns the yield associated with a region and a sample for the DATA sample
	 *
	 * @param mode
	 *   Pretag for the pretag sample or tagged for the tagged sample
	 * @param region
	 *   Region index to extract
	 * @param jet_bin
	 *   Jet bin to consider
	 * @param is_inclusive
	 *   Run inclusive or not
	 *
	 * @return
	 *   Yield in DATA
	 */
	double GetDataYield(TString mode, int region, int jet_bin,
			bool is_inclusive);

	/**
	 * Returns the error associated with a region and a sample for the DATA sample
	 *
	 * @param mode
	 *   Pretag for the pretag sample or tagged for the tagged sample
	 * @param region
	 *   Region index to extract
	 * @param jet_bin
	 *   Jet bin to consider
	 * @param is_inclusive
	 *   Run inclusive or not
	 *
	 * @return
	 *   Error in DATA
	 */
	double GetDataYieldError(TString mode, int region, int jet_bin,
			bool is_inclusive);

	// Sample properties
	TString sample_name; /**< Sample name */
	TString sample_path; /**< Sample path */
	TString sample_full_name; /**< Sample full name */

	// Data members
	TFile* file; /**< Pointer to dataset file*/

	// Histogram collection
	typedef std::map<TString, TH1D*> HistoDatabase; /**< Type histogram container */
	HistoDatabase *histo_database; /**< Histogram container object */
};

#endif /* DATASAMPLE_H_ */
