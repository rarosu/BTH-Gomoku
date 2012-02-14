#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <D3DX10.h>
#include "Globals.hpp"

class Effect
{
public:
	Effect();
	void Initialize(ID3D10Device* device, char* filename, D3D10_INPUT_ELEMENT_DESC* vertexDesc, int size);
	void MakeActive();
	void ApplyTechniquePass(int index);
	UINT GetNumberOfPasses();

	void SetIntVariable(char* variableName, int value);
	void SetBoolVariable(char* variableName, bool value);
	void SetFloatVariable(char* variableName, float value);
	void SetVectorVariable(char* variableName, D3DXVECTOR4* value);
	void SetMatrixVariable(char* variableName, D3DXMATRIX* matrix);
	void SetResourceVariable(char* variableName, ID3D10ShaderResourceView* resource);

private:
	ID3D10Device*				mDevice;
	ID3D10Effect*				mEffect;
	ID3D10EffectTechnique*		mTechnique;
	ID3D10InputLayout*			mVertexLayout;

	HRESULT CreateEffect(ID3D10Device* device, char* filename);
	HRESULT CreateVertexLayout(D3D10_INPUT_ELEMENT_DESC* vertexDesc, int size);
};
#endif