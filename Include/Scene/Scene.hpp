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

struct DebugRayInput
{
	int mScreenWidth;
	int mScreenHeight;
	Marker* mMarker;
	Console* mConsole;
	Camera* mCamera;
	float mAspectRatio;
};

/**
	The scene will render a view of the grid and is able to determine
	what cell the mouse is hovering over
*/
class Scene : public MouseListener
{
public:
	Scene(ID3D10Device* device, InputSubscription* inputManager);
	~Scene();

	/**
		Create a texture of the grid from the model

		Needs only be called when the grid has been updated
	*/
	void Update(const Logic::Grid* grid, const Camera& camera, const InputState& currentInput, DebugRayInput debugRayInput);

	/**
		Render the grid, through the given camera
	*/
	void Draw();


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
	InputSubscription*			mInputManager;

	Effect*						mEffect;
	Buffer*						mVertexBuffer;

	DebugRayInput				mDebugRayInput;

	void CreateBuffer();
	void CreateEffect();
	D3DXVECTOR2 PickCell(const InputState& currentState);
	RECT GetVisibleGrid(const Camera& camera) const;
};

#endif