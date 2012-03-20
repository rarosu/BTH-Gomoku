#ifndef SCENE_HPP
#define SCENE_HPP

#include "Globals.hpp"
#include "Component.hpp"
#include "ComponentGroup.hpp"
#include "Buffer.hpp"
#include "Effect.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Marker.hpp"
#include "Viewport.hpp"
#include "GameFont.hpp"		// DEBUG
#include <fstream>

/**
	The scene will render a view of the grid and is able to determine
	what cell the mouse is hovering over
*/
class Scene : public Components::Component
{
public:
	Scene(ID3D10Device* device, Components::ComponentGroup* ownerGroup, float aspectRatio, const Logic::Grid* grid, unsigned int playerCount);
	~Scene() throw();

	enum HighlightType { Hint, Warning, General, Mine };

	void HandleKeyPress(const InputState& currentInput, const GameTime& gameTime);

	// Methods inherited from Component
	void Draw();
	void LostFocus();
	void GotFocus();
	std::string GetName(); // DEBUG

	void KeyPressed(int code, const InputState& currentState);
	void KeyReleased(int code, const InputState& currentState);
	void MouseWheelMoved(short delta, const InputState& currentState);

	/**
		Create a texture of the grid from the model
	*/
	void Update(const Logic::Grid& grid, const InputState& currentInput, const InputState& previousInput, const GameTime& gameTime);

	/**
		When the frustum's dimensions have been resized, call this
		method.
	*/
	void ResizeFrustum(float aspectRatio);

	/**
		Given the position of the mouse and the orientation of the camera,
		this method will return the cell the mouse is hovering over.
	*/
	Logic::Cell PickCell(int mouseX, int mouseY) const;

	/**
		Highlight a given cell with a specific highlight type
	*/
	void HighlightCell(Logic::Cell cell, HighlightType type);

	/**
		Make the camera look at a specific cell.
	*/
	void LookAtCell(const Logic::Cell& cell);
protected:
	// Methods inherited from Component
	void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState) {}

private:
	typedef std::map<HighlightType, Logic::Cell> HighlightMap;
	static const D3DXCOLOR C_MARKER_COLORS[];
	static const Marker::MarkerType C_MARKER_TYPES[];

	static const int C_GRID_WIDTH;
	static const int C_GRID_HEIGHT;
	static const int C_CELL_SIZE;
	static const float C_BORDER_SIZE;

	struct GridVertex
	{
		D3DXVECTOR3				position;
		D3DXVECTOR2				cellUV;
		D3DXVECTOR2				boardUV;
	};

	struct CellFace
	{
		GridVertex mVertices[6];
	};

	

	ID3D10Device*				mDevice;
	Effect*						mEffect;
	VertexBuffer*				mVertexBuffer;

	Frustum						mFrustum;
	Camera*						mCamera;
	std::vector<Marker*>		mMarkers;
	Logic::Cell					mHoveredCell;
	HighlightMap				mHighlightedCells;
	const Logic::Grid*			mGrid;

	D3DXMATRIX					mModelMatrix;

	/**
		Methods for creating the buffer- and effect objects, for rendering.
	*/
	void CreateBuffer();
	void CreateEffect();

	// Methods inherited by Component
	void MouseEntered() {}
	void MouseExited() {}
	void MousePressed(int buttonIndex, const InputState& currentState) {}
	void MouseReleased(int buttonIndex, const InputState& currentState) {}
};

#endif