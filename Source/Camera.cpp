#include "Camera.hpp"
#include <cmath>

Camera::Camera(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXVECTOR3 worldUp, const Frustrum& viewFrustrum)
	: mPosition(position), mDirection(direction), mSpeed(0.05f), mWorldUp(worldUp)
{
	D3DXVec3Normalize(&mDirection, &mDirection);
	CreateProjectionMatrix(viewFrustrum);
}

void Camera::Update(const InputState& prevInput, const InputState& currInput)
{
	if(currInput.Keyboard.keyIsPressed['A'])
		MoveLeft();
	if(currInput.Keyboard.keyIsPressed['D'])
		MoveRight();
	if(currInput.Keyboard.keyIsPressed['W'])
		MoveForward();
	if(currInput.Keyboard.keyIsPressed['S'])
		MoveBack();

	if(currInput.Keyboard.keyIsPressed['J'])
		TurnHorizontal(0.001f);
	if(currInput.Keyboard.keyIsPressed['L'])
		TurnHorizontal(-0.001f);
	if(currInput.Keyboard.keyIsPressed['I'])
		TurnVertical(0.001f);
	if(currInput.Keyboard.keyIsPressed['K'])
		TurnVertical(-0.001f);
}

// Move camera backwards
void Camera::MoveBack()
{
	D3DXVECTOR3 forward;
	
	forward = mDirection - (D3DXVec3Dot(&mDirection, &mWorldUp) * mWorldUp);
	D3DXVec3Normalize(&forward, &forward);
	mPosition -= forward * mSpeed;
}

// Move camera forwards: walk
void Camera::MoveForward()
{
	D3DXVECTOR3 forward;

	forward = mDirection - (D3DXVec3Dot(&mDirection, &mWorldUp) * mWorldUp);
	D3DXVec3Normalize(&forward, &forward);
	mPosition += forward * mSpeed;
}

// Move camera to the left: strafe
void Camera::MoveLeft()
{
	D3DXVECTOR3 right = GetRight();
	mPosition -= right * mSpeed;
}

// Move camera to the right: strafe
void Camera::MoveRight()
{
	D3DXVECTOR3 right = GetRight();
	mPosition += right * mSpeed;
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
D3DXMATRIX Camera::GetViewMatrix() const
{
	D3DXVECTOR3 right, up;
	D3DXMATRIX view;

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

// Return the projection matrix for the camera
const D3DXMATRIX& Camera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

// Create the projection matrix for the camera
void Camera::CreateProjectionMatrix(const Frustrum& viewFrustrum)
{
	ZeroMemory(&mProjectionMatrix, sizeof(mProjectionMatrix));
	
	float scaleY, scaleX, length;
	scaleY = 1.0 / std::tan(viewFrustrum.fovY * 0.5);
	scaleX = scaleY / viewFrustrum.aspectRatio;
	length = viewFrustrum.farDistance - viewFrustrum.nearDistance;

	mProjectionMatrix.m[0][0] = scaleX;
	mProjectionMatrix.m[1][1] = scaleY;
	mProjectionMatrix.m[2][2] = viewFrustrum.farDistance / length;
	mProjectionMatrix.m[2][3] = 1;
	mProjectionMatrix.m[3][2] = (-viewFrustrum.nearDistance * viewFrustrum.farDistance) / length;
}

// Get the camera's right vector (camera x-axis)
D3DXVECTOR3 Camera::GetRight() const
{
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &mWorldUp, &mDirection);
	D3DXVec3Normalize(&right, &right);

	return right;
}

// Get the cameras current position in the world
const D3DXVECTOR3& Camera::GetPos() const
{
	return mPosition;
}

void Camera::SetHeight(float height)
{
	mPosition.y = height;
}