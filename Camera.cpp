#include "Camera.hpp"

Camera::Camera(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXVECTOR3 worldUp)
	: mPosition(position), mDirection(direction), mSpeed(0.15f), mWorldUp(worldUp)
{
	D3DXVec3Normalize(&mDirection, &mDirection);
}

// Move camera backwards: walk
void Camera::MoveBack(bool running)
{
	D3DXVECTOR3 forward;
	float runFactor = 1.0f;
	if(running)
		runFactor = 2.0f;
	
	forward = mDirection - (D3DXVec3Dot(&mDirection, &mWorldUp) * mWorldUp);
	D3DXVec3Normalize(&forward, &forward);
	mPosition -= forward * mSpeed * runFactor;
}

// Move camera forwards: walk
void Camera::MoveForward(bool running)
{
	D3DXVECTOR3 forward;
	float runFactor = 1.0f;
	if(running)
		runFactor = 2.0f;

	forward = mDirection - (D3DXVec3Dot(&mDirection, &mWorldUp) * mWorldUp);
	D3DXVec3Normalize(&forward, &forward);
	mPosition += forward * mSpeed * runFactor;
}

// Move camera to the left: strafe
void Camera::MoveLeft(bool running)
{
	float runFactor = 1.0f;
	if(running)
		runFactor = 2.0f;

	D3DXVECTOR3 right = GetRight();
	mPosition -= right * mSpeed * runFactor;
}

// Move camera to the right: strafe
void Camera::MoveRight(bool running)
{
	float runFactor = 1.0f;
	if(running)
		runFactor = 2.0f;

	D3DXVECTOR3 right = GetRight();
	mPosition += right * mSpeed* runFactor;
}

// Tilt the camera horisontally: look left/right
void Camera::TurnHorizontal(float angle)
{
	D3DXVECTOR3 up;
	D3DXVec3Cross(&up, &mDirection, &GetRight());
	D3DXMATRIX rotation;

	D3DXMatrixRotationAxis(&rotation, &up, angle);
	D3DXVec3TransformCoord(&mDirection, &mDirection, &rotation);
	D3DXVec3Normalize(&mDirection, &mDirection);
}

// Tilt the camera vertically: look up/down
void Camera::TurnVertical(float angle)
{
	D3DXVECTOR3 right = GetRight();
	D3DXMATRIX rotation;

	D3DXMatrixRotationAxis(&rotation, &right, angle);
	D3DXVec3TransformCoord(&mDirection, &mDirection, &rotation);
	D3DXVec3Normalize(&mDirection, &mDirection);
}

// Create and return the view matrix for the camera
D3DXMATRIX Camera::GetViewMatrix()
{
	D3DXVECTOR3 right, up;
	D3DXMATRIX view;

	D3DXVec3Normalize(&mDirection, &mDirection);
	right = GetRight();
	D3DXVec3Cross(&up, &mDirection, &right);
	
	view.m[0][0] = right.x;
	view.m[1][0] = right.y;
	view.m[2][0] = right.z;

	view.m[0][1] = up.x;
	view.m[1][1] = up.y;
	view.m[2][1] = up.z;

	view.m[0][2] = mDirection.x;
	view.m[1][2] = mDirection.y;
	view.m[2][2] = mDirection.z;

	view.m[0][3] = 0;
	view.m[1][3] = 0;
	view.m[2][3] = 0;
	view.m[3][3] = 1;

	view.m[3][0] = -D3DXVec3Dot(&mPosition, &right);
	view.m[3][1] = -D3DXVec3Dot(&mPosition, &up);
	view.m[3][2] = -D3DXVec3Dot(&mPosition, &mDirection);

	return view;
}

// Get the camera's right vector (camera x-axis)
D3DXVECTOR3 Camera::GetRight()
{
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &mWorldUp, &mDirection);
	D3DXVec3Normalize(&right, &right);

	return right;
}

// Get the cameras current position in the world
D3DXVECTOR3 Camera::GetPos()
{
	return mPosition;
}

void Camera::SetHeight(float height)
{
	mPosition.y = height;
}