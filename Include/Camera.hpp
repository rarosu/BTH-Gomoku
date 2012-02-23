#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "WinInclude.hpp"
#include "InputManager.hpp"
#include "GameTime.hpp"

struct Frustrum
{
	float		nearDistance;
	float		farDistance;
	float		fovY;
	float		aspectRatio;
};

class Camera : public MouseListener
{
public:
	Camera(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXVECTOR3 worldUp, const Frustrum& viewFrustrum, InputSubscription* inputManager);
	~Camera() throw();

	void Update(const InputState& prevInput, const InputState& currInput, const GameTime& gameTime);

	D3DXMATRIX GetViewMatrix() const;
	const D3DXMATRIX& GetProjectionMatrix() const;
	void CreateProjectionMatrix(const Frustrum& viewFrustrum);

	const D3DXVECTOR3& GetPos() const;
	void SetHeight(float height);
	
	void MouseButtonPressed(int index, const InputState& currentState);
	void MouseButtonReleased(int index, const InputState& currentState);
	void MouseWheelMoved(short delta, const InputState& currentState);
private:
	InputSubscription*		mInputManager;

	D3DXVECTOR3				mPosition;
	D3DXVECTOR3				mDirection;
	D3DXVECTOR3				mWorldUp;
	D3DXMATRIX				mProjectionMatrix;

	float					mZoom;
	
	static const float		C_MOVE_SPEED;
	static const float		C_TILTING_SPEED;
	static const float		C_ZOOM_SPEED;
	static const float		C_ZOOM_MIN;
	static const float		C_ZOOM_MAX;

	D3DXVECTOR3 GetRight() const;
	void MoveLeft(const GameTime& gameTime);
	void MoveRight(const GameTime& gameTime);
	void MoveForward(const GameTime& gameTime);
	void MoveBack(const GameTime& gameTime);
	void TurnHorizontal(float angle);
	void TurnVertical(float angle);
};
#endif
