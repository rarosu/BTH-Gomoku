#ifndef SCENE_HPP
#define SCENE_HPP

#include "Globals.hpp"
#include "Buffer.hpp"
#include "Effect.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Marker.hpp"
#include "Viewport.hpp"

/**
	The scene will render a view of the grid and is able to determine
	what cell the mouse is hovering over
*/
class Scene
{
public:
	Scene(ID3D10Device* device);
	~Scene() throw();

	/**
		Create a texture of the grid from the model
	*/
	void Update(const Logic::Grid& grid, const Camera& camera, const Viewport& viewport, const InputState& currentInput);

	/**
		Render the grid, through the given camera
	*/
	void Draw(const Camera& camera);
private:
	static const int C_GRID_WIDTH;
	static const int C_GRID_HEIGHT;
	static const int C_CELL_SIZE;
	static const float C_BORDER_SIZE;

	struct GridVertex
	{
		D3DXVECTOR3				position;
	};

	ID3D10Device*				mDevice;
	Effect*						mEffect;
	VertexBuffer*				mVertexBuffer;

	Logic::Cell					mHoveredCell;

	/**
		Methods for creating the buffer- and effect objects, for rendering.
	*/
	void CreateBuffer();
	void CreateEffect();

	/**
		Given the position of the mouse and the orientation of the camera,
		this method will return the cell the mouse is hovering over.
	*/
	Logic::Cell PickCell(const Viewport& viewport, int mouseX, int mouseY, const Camera& camera) const;
};

#endif