#ifndef MTLREADER_H
#define MTLREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <Classes/Material.h>

class MTLReader {
	enum MTLTypes {
		INVALID,
		NEW_MATERIAL,
		MAP_KD,
		KA,
		KS,
		KD,
		NS
	};

	typedef std::map<std::string, MTLTypes> MTLTypeMap;

	MTLTypeMap mtlTypeMap = {
		{"newmtl", MTLTypes::NEW_MATERIAL},
		{"map_Kd", MTLTypes::MAP_KD},
		{"Ka", MTLTypes::KA},
		{"Ks", MTLTypes::KS},
		{"Kd", MTLTypes::KD},
		{"Ns", MTLTypes::NS}
	};

	private:
		int lineCount, currentMaterialIndex;
		std::vector<Material*> materials;
	
	public:
		MTLReader();
		
		std::vector<Material*> Read(std::string filename);
};

#endif