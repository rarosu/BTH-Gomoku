#ifndef SCENE_HPP
#define SCENE_HPP

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Logic/Grid.hpp"

/**
	The scene will render a view of the grid and is able to determine
	what cell the mouse is hovering over
*/
class Scene
{
public:
	Scene(ID3D10Device* device);

	/**
		Create a texture of the grid from the model

		Needs only be called when the grid has been updated
	*/
	void Update(const Logic::Grid* grid);

	/**
		Render the grid, through the given camera
	*/
	void Render(const Camera* camera);
private:
	ID3D10Device* mDevice;
};

#endif