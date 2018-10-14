#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>

class Material {
	private:
		std::string materialID, textureFilename;
		glm::vec3 ka, kd, ks;
		float ns;

	public:
		Material();
		Material(std::string materialID);

		void SetMaterialID(std::string materialID);
		std::string GetMaterialID();

		void SetTextureFilename(std::string textureFilename);
		std::string GetTextureFilename();

		void SetKA(float red, float green, float blue);
		glm::vec3 GetKA();

		void SetKS(float red, float green, float blue);
		glm::vec3 GetKS();

		void SetKD(float red, float green, float blue);
		glm::vec3 GetKD();

		void SetNS(float ns);
		float GetNS();
};