#ifndef EFFECT_HPP
#define EFFECT_HPP

#include "Globals.hpp"
#include <vector>
#include "Technique.hpp"

class Effect
{
public:
	Effect(ID3D10Device* device, const std::string& filename);
	~Effect();

	/**
		Release the old effect file and load a new one, and enumerate
		all the techniques
	*/
	bool LoadEffectFile(const std::string& filename);

	/**
		Get the number of techniques in this effect
	*/
	unsigned int GetTechniqueCount() const;

	/**
		Get a technique by specifying its index
	*/
	Technique& GetTechniqueByIndex(unsigned int index);
	const Technique& GetTechniqueByIndex(unsigned int index) const;

	/**
		Get a technique by specifying its name. If no technique by the name exists,
		a runtime exception is raised.
	*/
	Technique& GetTechniqueByName(const std::string& name);
	const Technique& GetTechniqueByName(const std::string& name) const;

	/**
		Set global or constant buffer variables
	*/
	void SetVariable(const std::string& variableName, int value);
	void SetVariable(const std::string& variableName, bool value);
	void SetVariable(const std::string& variableName, float value);
	void SetVariable(const std::string& variableName, const D3DXVECTOR4& value);
	void SetVariable(const std::string& variableName, const D3DXVECTOR3& value);
	void SetVariable(const std::string& variableName, const D3DXVECTOR2& value);
	void SetVariable(const std::string& variableName, const D3DXMATRIX& value);
	void SetVariable(const std::string& variableName, ID3D10ShaderResourceView* value);

	/**
		Comparison operator

		Two effects are equal if the effect pointer is equal.
	*/
	bool operator==(const Effect& rhs) const;
private:
	ID3D10Device* mDevice;
	ID3D10Effect* mEffect;
	
	std::vector<Technique> mTechniques;
};

#endif