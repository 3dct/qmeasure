// Copyright (c) open_iA contributors
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <map>
#include <string>


class QMeasureCalculation
{

public:

static std::map<std::string, double> computeOrigQ(
		float* fImage, const int* dim, const double* range, int HistogramBins, int NumberPeaks, bool AnalyzePeak);
};
