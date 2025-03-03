// Copyright (c) open_iA contributors
// SPDX-License-Identifier: GPL-3.0-or-later
#include <QMeasureCalculation.h>
#include <ImageHistogram.h>

#include <sstream>

std::string fullMeasureName(std::string measure, int peakNr)
{
	std::ostringstream oss;
	oss << "Peak " << peakNr << " " << measure;
	return oss.str();
}

std::map<std::string, double> QMeasureCalculation::computeOrigQ(
	float* fImage, const int* dim, const double* range, int HistogramBins, int NumberPeaks, bool AnalyzePeak = false)
{
	// some "magic numbers"
	unsigned int dgauss_size_BINscale = 24;
	unsigned int gauss_size_P2Pscale = 24;
	double threshold_x = -0.1;
	double threshold_y = 2;  // one single voxel is no valid class


	//float* fImage = static_cast < float*>(img->GetScalarPointer());
	cImageHistogram curHist;
	curHist.CreateHist(fImage, dim[0], dim[1], dim[2], HistogramBins, static_cast<float>(range[0]), static_cast<float>(range[1]), false, 0, 0);
	/*unsigned int Peaks_fnd = */ curHist.DetectPeaksValleys(
		NumberPeaks, dgauss_size_BINscale, gauss_size_P2Pscale, threshold_x, threshold_y, false);

	// Calculate histogram quality measures Q using the valley thresholds to separate classes
	std::vector<int> thresholds_IDX = curHist.GetValleyThreshold_IDX();
	std::vector<float> thresholds = curHist.GetValleyThreshold();
	std::vector<ClassMeasure> classMeasures;
	double Q0 = (thresholds_IDX.size() == 0) ? 0.0 : curHist.CalcQ(thresholds_IDX, classMeasures, 0);
	double Q1 = (thresholds_IDX.size() == 0) ? 0.0 : curHist.CalcQ(thresholds_IDX, classMeasures, 1);


	std::map<std::string, double> results;

	results.insert({"Q (orig, equ 0)", Q0});
	results.insert({"Q (orig, equ 1)", Q1});
	if (AnalyzePeak)
	{

		int classNr = 0;
		for (auto c : classMeasures)
		{
			results.insert({fullMeasureName("Mean", classNr), c.mean});
			results.insert({fullMeasureName("Sigma",classNr), c.sigma});
			results.insert({fullMeasureName("Probability",classNr), c.probability});
			results.insert({fullMeasureName("Min",classNr), c.LowerThreshold});
			results.insert({fullMeasureName("Max",classNr), c.UpperThreshold});
			results.insert({fullMeasureName("Usage",classNr), c.UsedForQ});
			++classNr;
		}
	}


	return results;
}
