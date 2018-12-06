#include <Classes/OBJWriter.h>

OBJWriter::OBJWriter(std::vector<float> *internalCurvePoints, std::vector<float> *externalCurvePoints) {
	this -> internalCurvePoints = internalCurvePoints;
	this -> externalCurvePoints = externalCurvePoints;
}

void OBJWriter::WriteOBJ() {
	CreateOBJFile();
	
	WriteInternalCurve();
	WriteExternalCurve();
	
	WriteTextureCoordinates();
	WriteVertexNormals();

	WriteFaces();

	WriteMTL();
}

int OBJWriter::GetInternalPointsCount() {
	return this -> internalCurvePoints -> size();
}

int OBJWriter::GetExternalPointsCount() {
	return this -> externalCurvePoints -> size();
}

int OBJWriter::GetTotalPointsCount() {
	return (
		this -> GetInternalPointsCount() + this -> GetExternalPointsCount()
	);
}

void OBJWriter::CreateOBJFile() {
	this -> objFile.open(FILENAME, std::ofstream::out | std::ofstream::trunc);

	this -> objFile << "mtllib " << "pista.mtl" << "\n" << std::endl;

	this -> objFile.close();
}

void OBJWriter::WriteInternalCurve() {
	this -> objFile.open(FILENAME, std::ios_base::app);

	std::cout << GetInternalPointsCount() << std::endl;
	
	for (int i = 0; i < this -> GetInternalPointsCount(); i += 3) {
		this -> objFile << "v " <<
			this -> internalCurvePoints -> at(i) << " " <<
			this -> internalCurvePoints -> at(i + 2) * 100 << " " <<
			this -> internalCurvePoints -> at(i + 1) << std::endl;
	}
	
	this -> objFile.close();
}

void OBJWriter::WriteExternalCurve() {
	this -> objFile.open(FILENAME, std::ios_base::app);

	std::cout << GetExternalPointsCount() << std::endl;

	for (int i = 0; i < this -> GetExternalPointsCount(); i += 3) {
		this -> objFile << "v " <<
			this -> externalCurvePoints -> at(i) << " " <<
			this -> externalCurvePoints -> at(i + 2) * 100 << " " <<
			this -> externalCurvePoints -> at(i + 1) << std::endl;
	}
	
	this -> objFile.close();
}

void OBJWriter::WriteTextureCoordinates() {
	this -> objFile.open(FILENAME, std::ios_base::app);
	
	this -> objFile << std::endl;
	this -> objFile << "vt " << "0.0" << " " << "0.0" << std::endl;
	this -> objFile << "vt " << "1.0" << " " << "0.0" << std::endl;
	this -> objFile << "vt " << "0.0" << " " << "1.0" << std::endl;
	this -> objFile << "vt " << "1.0" << " " << "1.0" << std::endl;
	this -> objFile << std::endl;
	
	this -> objFile.close();
}

void OBJWriter::WriteVertexNormals() {
	this -> objFile.open(FILENAME, std::ios_base::app);

	this -> objFile << "vn " << "0.0" << " " << "1.0" << " " << "0.0" << std::endl;

	this -> objFile.close();
}

void OBJWriter::WriteFaces() {
	this -> objFile.open(FILENAME, std::ios_base::app);

	this -> objFile << "g " << "pista" << "\n" << std::endl;
	this -> objFile << "usemtl pista" << "\n" << std::endl;

	for (int i = 1; i <= this -> GetInternalPointsCount() / 3 - 1; i++) {
		this -> objFile <<
			"f " <<
			i << "/" << 2 << "/" << 1 << " " <<
			i + 1 << "/" << 4 << "/" << 1 << " " <<
			i + (this -> GetInternalPointsCount() / 3) << "/" << 3 << "/" << 1 << 
			std::endl;

		this -> objFile <<
			"f " <<
			i + (this -> GetInternalPointsCount() / 3) << "/" << 3 << "/" << 1 << " " <<
			i + 1 << "/" << 1 << "/" << 1 << " " <<
			i + 1 + (this -> GetInternalPointsCount() / 3) << "/" << 2 << "/" << 1 <<
			std::endl;
	}

	this -> objFile.close();
}

void OBJWriter::WriteMTL() {
	std::ofstream mtlFile;
	
	mtlFile.open("pista.mtl", std::ofstream::out | std::ofstream::trunc);

	mtlFile << "newmtl pista" << std::endl;
	mtlFile << "map_Kd pista.jpg" << std::endl;
	mtlFile << "Ka 0.5 0.5 0.5" << std::endl;
	mtlFile << "Kd 1.0 1.0 1.0" << std::endl;
	mtlFile << "Ks 1.0 1.0 1.0" << std::endl;
	mtlFile << "Ns 10" << std::endl;

	mtlFile.close();
}