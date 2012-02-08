#ifndef SCENE_HPP
#define SCENE_HPP

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Effect.hpp"
#include "Logic/Grid.hpp"

/**
	The scene will render a view of the grid and is able to determine
	what cell the mouse is hovering over
*/
class Scene
{
public:
	Scene(ID3D10Device* device);
	~Scene();

	/**
		Create a texture of the grid from the model

		Needs only be called when the grid has been updated
	*/
	void Update(const Logic::Grid* grid, const Camera& camera);

	/**
		Render the grid, through the given camera
	*/
	void Draw();

private:
	struct GridVertex
	{
		D3DXVECTOR3				position;
		D3DXCOLOR				color;
	};

	ID3D10Device*				mDevice;
	Effect*						mEffect;
	Buffer*						mVertexBuffer;

	void CreateBuffer();
	void CreateEffect();
};

#endif