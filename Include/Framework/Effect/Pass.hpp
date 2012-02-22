#ifndef PASS_HPP
#define PASS_HPP

#include "Global.hpp"
#include <d3d10.h>
#include <string>
#include <vector>

/**
	Specifies a variable in the vertex buffer, and how it is represented in
	the pass.
*/
struct InputLayoutElement
{
public:
	InputLayoutElement(const std::string& name, DXGI_FORMAT format, unsigned int size, unsigned int semanticIndex = 0) :
		mName(name), mFormat(format), mSize(size), mSemanticIndex(semanticIndex) {}

	std::string mName;				// The name of the variable, specified after the : in the effect file
	DXGI_FORMAT mFormat;			// The format of the variable, what size and how it should be interpreted.
	unsigned int mSize;				// The size of the variable (in bytes)
	unsigned int mSemanticIndex;	// In case two variables have the same name, this index will separate them (start at 0).
};

/**
	The container used to specify a layout of a vertex buffer, and its interface
	towards the pass.
*/
typedef std::vector<InputLayoutElement> InputLayoutVector;

/**
	Specifies one pass in one technique. It knows how the vertex buffer should be formatted
	to be able to properly parse it.
*/
class Pass
{
public:
	Pass(ID3D10EffectPass* pass);
	~Pass() throw();

	/**
		Set the layout of the vertex buffer and how it relates to the variables
		for the input of the vertex shader.
	*/
	void SetInputLayout(const InputLayoutVector& inputLayout);

	/**
		Bind the pass to an input slot with the given input layout, and
		then apply it.
	*/
	bool Apply(ID3D10Device* device);

	/**
		Get the name of the pass
	*/
	const std::string& GetName() const { return mName; }
private:
	ID3D10EffectPass* mPass;
	ID3D10InputLayout* mCalculatedLayout;
	std::string mName;

	InputLayoutVector mInputLayout;
	bool mShouldLayoutBeRecalculated;

	bool RecalculateLayout(ID3D10Device* device);
};

#endif