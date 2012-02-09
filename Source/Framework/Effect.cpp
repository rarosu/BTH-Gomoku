#include "Effect.hpp"

Effect::Effect()
	: mDevice(0), mEffect(0), mTechnique(0), mVertexLayout(0)
{
}

void Effect::Initialize(ID3D10Device* device, char* filename, D3D_PRIMITIVE_TOPOLOGY topology,
	D3D10_INPUT_ELEMENT_DESC* vertexDesc, int size)
{
	mDevice = device;
	mTopology = topology;

	CreateEffect(mDevice, filename);
	CreateVertexLayout(vertexDesc, size);
}

// Compile and create the shader/effect
HRESULT Effect::CreateEffect(ID3D10Device* device, char* filename)
{
	HRESULT result = S_OK;								// Variable that stores the result of the functions
	UINT shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;	// Shader flags
	ID3D10Blob* errors = NULL;							// Variable to store error messages from functions
	ID3D10Blob* effect = NULL;							// Variable to store compiled (but not created) effect
	
	// Compile shader, if failed - show error message and return
	result = D3DX10CompileFromFileA(filename,		// Path and name of the effect file to compile
								   0,				// Shader macros: none needed
								   0,				// Include interface: not needed - no #include in shader
								   "",				// Shader start function, not used when compiling from file
								   "fx_4_0",		// String specifying shader model (or shader profile)
								   shaderFlags,		// Shader compile flags - how to compile the shader	
								   0,				// Effect compile flags - not used when compiling a shader
								   0,				// Thread pump interface: not needed - return only when finished
								   &effect,			// Out: Where to put the compiled effect information (pointer)
								   &errors,			// Out: Where to put the errors, if there are any (pointer)
								   NULL);			// Out: Result not needed, result is gotten from the return value
	
	if(FAILED(result))								// If failed, show error message and return
	{
		if(errors)
		{
			MessageBox(0, (char*)errors->GetBufferPointer(), "ERROR", 0);
			SafeRelease(errors);
		}
		

		return result;
	}

	result = D3DX10CreateEffectFromMemory(
						effect->GetBufferPointer(),	// Pointer to the effect in memory, gotten from the compiled effect
						effect->GetBufferSize(),	// The effect's size in memory, gotten from the compiled effect
						filename,					// Name of the effect file
						NULL,						// Shader macros: none needed
						NULL,						// Include interface: not needed 
						"fx_4_0",					// String specifying shader model (or shader profile)
						NULL,						// Shader constants, HLSL compile options						
						NULL,						// Effect constants, Effect compile options
						device,						// Pointer to the direct3D device that will use resources
						NULL,						// Pointer to effect pool for variable sharing between effects
						NULL,						// Thread pump interface: not needed - return only when finished
						&mEffect,					// Out: where to put the created effect (pointer)
						&errors,					// Out: Where to put the errors, if there are any (pointer)
						NULL);						// Out: Result not needed, result is gotten from the return value

	if(FAILED(result))								// If failed, show error message and return
	{
		MessageBox(0, "Shader creation failed: Console!", "ERROR", 0);
		return result;
	}

	return result;
}

// Build vertex layout
HRESULT Effect::CreateVertexLayout(D3D10_INPUT_ELEMENT_DESC* vertexDesc, int size)
{
	D3D10_PASS_DESC passDesc;
	HRESULT result;

	// Get the effect technique from the effect, and save the descritption of the first pass
	mTechnique = mEffect->GetTechniqueByIndex(0);
	mTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	// Create the input layout and save it, if failed - show an error message
	result = mDevice->CreateInputLayout(
				vertexDesc,						// Description of input structure - array of element descriptions
				size,							// Number of elements in the input structure description
				passDesc.pIAInputSignature,		// Get pointer to the compiled shader
				passDesc.IAInputSignatureSize,	// The size of the compiled shader
				&mVertexLayout);				// Out: where to put the created input layout 

	if(FAILED(result))							// If layer creation fails, show an error message and return
	{
		MessageBox(0, "Input Layout creation failed!", "ERROR", 0);
		return result;
	}

	// Bind the input layout to the 3D device
	mDevice->IASetInputLayout(mVertexLayout);

	return result;
}

void Effect::MakeActive()
{
	mDevice->IASetInputLayout(mVertexLayout);
	mDevice->IASetPrimitiveTopology(mTopology);
}

void Effect::ApplyTechniquePass(int index)
{
	mTechnique->GetPassByIndex(index)->Apply(0);
}

UINT Effect::GetNumberOfPasses()
{
	D3D10_TECHNIQUE_DESC techDesc;
	mTechnique->GetDesc(&techDesc);

	return techDesc.Passes;
}

void Effect::SetIntVariable(char* variableName, int value)
{
	mEffect->GetVariableByName(variableName)->AsScalar()->SetInt(10);
}

void Effect::SetBoolVariable(char* variableName, bool value)
{
	mEffect->GetVariableByName(variableName)->AsScalar()->SetBool(value);
}

void Effect::SetFloatVariable(char* variableName, float value)
{
	mEffect->GetVariableByName(variableName)->AsScalar()->SetFloat(value);
}

void Effect::SetVectorVariable(char* variableName, D3DXVECTOR4* value)
{
	mEffect->GetVariableByName(variableName)->AsVector()->SetFloatVector((float*)value);
}

void Effect::SetMatrixVariable(char* variableName, D3DXMATRIX* matrix)
{
	mEffect->GetVariableByName(variableName)->AsMatrix()->SetMatrix((float*)matrix);
}