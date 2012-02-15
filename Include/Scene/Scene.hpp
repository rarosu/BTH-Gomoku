#ifndef SCENE_HPP
#define SCENE_HPP

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Effect.hpp"
#include "Grid.hpp"
#include "InputManager.hpp"
#include "Marker.hpp"
#include "Console.hpp"
#include "Globals.hpp"

/**
	The scene will render a view of the grid and is able to determine
	what cell the mouse is hovering over
*/
class Scene : public MouseListener
{
public:
	Scene(ID3D10Device* device, InputSubscription* inputSubscription);
	~Scene() throw();

	/**
		Create a texture of the grid from the model
	*/
	void Update(const Logic::Grid* grid, const Camera& camera, const InputState& currentInput);

	/**
		Render the grid, through the given camera
	*/
	void Draw(const Camera& camera);


	virtual void MouseButtonPressed(int index, const InputState& currentState);
	virtual void MouseButtonReleased(int index, const InputState& currentState);
	virtual void MouseWheelMoved(short delta, const InputState& currentState);
private:
	struct GridVertex
	{
		D3DXVECTOR3				position;
		D3DXCOLOR				color;
	};

	ID3D10Device*				mDevice;
	InputSubscription*			mInputSubscription;

	Effect*						mEffect;
	Buffer*						mVertexBuffer;

	/**
		Methods for creating the buffer- and effect objects, for rendering.
	*/
	void CreateBuffer();
	void CreateEffect();

	/**
		Given the position of the mouse and the orientation of the camera,
		this method will return the cell the mouse is hovering over.
	*/
	D3DXVECTOR2 PickCell(int mouseX, int mouseY, const Camera& camera) const;

	/**
		Get the rectangle which completely encompasses the visible area
		on the grid.
	*/
	RECT GetVisibleRectangle(const Camera& camera) const;
};

#endif