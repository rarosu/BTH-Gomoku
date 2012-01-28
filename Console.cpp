#include "Console.hpp"

const char*	Console::C_FILENAME = "Console.fx";
const int Console::C_NUM_VERTICES = 4;

Console::Console(ID3D10Device* device, float screenWidth, float screenHeight)
{
	mDevice = device;

	D3DXVECTOR2 vertices[C_NUM_VERTICES];
	vertices[0] = D3DXVECTOR2(0, 0);
	vertices[1] = D3DXVECTOR2(screenWidth, 0);
	vertices[2] = D3DXVECTOR2(0, screenHeight / 2);
	vertices[3] = D3DXVECTOR2(screenWidth, screenHeight / 2);

	mVertexBuffer = new Buffer();
	BufferInformation bufferDesc;

	bufferDesc.type = VertexBuffer;
	bufferDesc.usage = Buffer_Default;
	bufferDesc.numberOfElements = C_NUM_VERTICES;
	bufferDesc.firstElementPointer = vertices;
	bufferDesc.elementSize = sizeof(D3DXVECTOR2);

	mVertexBuffer->Initialize(mDevice, bufferDesc);

	CreateEffect();
	CreateVertexLayout();
}

// Compile and create the shader/effect
HRESULT Console::CreateEffect()
{
	HRESULT result = S_OK;								// Variable that stores the result of the functions
	UINT shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;	// Shader flags
	ID3D10Blob* errors = NULL;							// Variable to store error messages from functions
	ID3D10Blob* effect = NULL;							// Variable to store compiled (but not created) effect
	
	// Compile shader, if failed - show error message and return
	result = D3DX10CompileFromFileA(C_FILENAME,		// Path and name of the effect file to compile
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
						C_FILENAME,					// Name of the effect file
						NULL,						// Shader macros: none needed
						NULL,						// Include interface: not needed 
						"fx_4_0",					// String specifying shader model (or shader profile)
						NULL,						// Shader constants, HLSL compile options						
						NULL,						// Effect constants, Effect compile options
						mDevice,					// Pointer to the direct3D device that will use resources
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
HRESULT Console::CreateVertexLayout()
{
	// Create an array describing each of the elements of the vertex that are inputs to the vertex shader.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] = 
	{
		{ "POSITION",					// Semantic name, must be same as the vertex shader input semantic name
		  0,							// Semantic index, if one semantic name exists for more than one element
		  DXGI_FORMAT_R32G32_FLOAT,		// Format of the element, R32G32_FLOAT is a 32-bit 2D float vector
		  0,							// Input slot, of the 0-15 slots, through wich to send vertex data
		  0,							// AlignedByteOffset, bytes from start of the vertex to this component
		  D3D10_INPUT_PER_VERTEX_DATA,	// Input data class for this input slot
		  0 }							// 0 when slot input data class is D3D10_INPUT_PER_VERTEX_DATA
	};

		D3D10_PASS_DESC passDesc;
		HRESULT result;

		// Get the effect technique from the effect, and save the descritption of the first pass
		mTechnique = mEffect->GetTechniqueByName("DrawTechnique");
		mTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

		// Create the input layout and save it, if failed - show an error message
		result = mDevice->CreateInputLayout(
					vertexDesc,						// Description of input structure - array of element descriptions
					1,								// Number of elements in the input structure description
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

void Console::Draw()
{
	mVertexBuffer->MakeActive();

	mDevice->IASetInputLayout(mVertexLayout);
	mDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3D10_TECHNIQUE_DESC techDesc;
	mTechnique->GetDesc(&techDesc);
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTechnique->GetPassByIndex(p)->Apply(0);
		mDevice->Draw(C_NUM_VERTICES, 0);
	}
}