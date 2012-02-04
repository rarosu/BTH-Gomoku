#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <D3DX10.h>
#include "InputManager.hpp"

struct Frustrum
{
	float		nearDistance;
	float		farDistance;
	float		fovY;
	float		aspectRatio;
};

class Camera
{
public:
	Camera(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXVECTOR3 worldUp, const Frustrum& viewFrustrum);
	void Update(const InputState& prevInput, const InputState& currInput);
	D3DXMATRIX GetViewMatrix() const;
	const D3DXMATRIX& GetProjectionMatrix() const;
	void CreateProjectionMatrix(const Frustrum& viewFrustrum);
	const D3DXVECTOR3& GetPos() const;
	void SetHeight(float height);

private:
	D3DXVECTOR3				mPosition;
	D3DXVECTOR3				mDirection;
	D3DXVECTOR3				mWorldUp;
	D3DXMATRIX				mProjectionMatrix;
	const float				mSpeed;

	D3DXVECTOR3 GetRight() const;
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBack();
	void TurnHorizontal(float angle);
	void TurnVertical(float angle);
};
#endif
