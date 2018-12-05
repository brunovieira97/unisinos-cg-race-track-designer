#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <GLM/glm.hpp>

class OBJWriter {
	private:
		const std::string FILENAME = "curve.obj";
		std::ofstream objFile;
		std::vector<float> *internalCurvePoints, *externalCurvePoints;

		void CreateOBJFile();
		void WriteInternalCurve();
		void WriteExternalCurve();
		void WriteTextureCoordinates();
		void WriteVertexNormals();
		void WriteFaces();
		void WriteMTL();

		int GetInternalPointsCount();
		int GetExternalPointsCount();
		int GetTotalPointsCount();

		void AddFaces(int index, int externalCurveLength, int faces, int n);
		void AddNormalizedExternalCurve(glm::vec3 normal_abac, glm::vec3 normal_dbdc);
		
	public:
		OBJWriter(std::vector<float> *internalCurvePoints, std::vector<float> *externalCurvePoints);

		void WriteOBJ();
};