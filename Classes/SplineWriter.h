#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <GLM/glm.hpp>

class SplineWriter {
	private:
		const std::string FILENAME = "spline.txt";
		std::ofstream splineFile;
		std::vector<float> *splineCurvePoints;

		void CreateSplineFile();
		void WriteSplineCurve();

		int GetSplinePointsCount();

	public:
		SplineWriter(std::vector<float> *splineCurvePoints);

		void WriteSpline();
};