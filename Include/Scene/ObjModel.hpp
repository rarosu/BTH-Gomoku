#ifndef OBJMODEL_HPP
#define OBJMODEL_HPP

#include "WinInclude.hpp"
#include <fstream>
#include <vector>
#include <string>

#include "Buffer.hpp"
#include "Effect.hpp"
#include "GameTime.hpp"
#include "Camera.hpp"

struct Vertex
{
	D3DXVECTOR3			Position;
	D3DXVECTOR3			Normal;
	//D3DXVECTOR2			UV;
};

class ObjModel
{
public:
	ObjModel(ID3D10Device* device, std::string filename, D3DXCOLOR color);
	~ObjModel() throw();
	void Draw(const Camera& camera, D3DXVECTOR3 drawPosition);

private:
	ID3D10Device*				mDevice;
	VertexBuffer*				mVertexBuffer;
	Effect*						mEffect;
	std::vector<Vertex>			mVertices;
	D3DXMATRIX*					mMatrixWorld;

	bool Load(std::string filename);
	void UpdateWorldMatrix(D3DXVECTOR3 position);
	void CreateBuffers();
	void CreateEffect();
};
#endif