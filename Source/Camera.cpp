#include "Camera.hpp"
#include "Globals.hpp"
#include <cmath>

const float	Camera::C_MOVE_SPEED		= 100.0f;
const float	Camera::C_TILTING_SPEED		= 0.001f;
const float	Camera::C_ZOOM_SPEED		= 6.0f;
const float	Camera::C_ZOOM_MIN			= -500.0f;
const float	Camera::C_ZOOM_MAX			= -30.0f;

Camera::Camera(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXVECTOR3 worldUp, const Frustum& viewFrustum)
	: mPosition(position), mDirection(direction), mWorldUp(worldUp), mZoom(-200.0f)
{
	mPosition = mPosition - (D3DXVec3Dot(&mPosition, &mWorldUp) * mWorldUp);
	mDestination = mPosition;

	D3DXVec3Normalize(&mDirection, &mDirection);
	CreateProjectionMatrix(viewFrustum);
}

Camera::~Camera() throw()
{
}

void Camera::Update(const InputState& prevInput, const InputState& currInput, const GameTime& gameTime)
{
	D3DXVECTOR3 displacement = mDestination - mPosition;
	//D3DXVec3Normalize(&displacement, &displacement);

	mPosition += displacement * gameTime.GetTimeSinceLastTick().Seconds;
	
	displacement = mDestination - mPosition;
	if (D3DXVec3Length(&displacement) <= 1.0f)
	{
		mPosition = mDestination;
	}
}

// Move camera backwards
void Camera::MoveBack(const GameTime& gameTime)
{
	D3DXVECTOR3 forward;
	
	forward = mDirection - (D3DXVec3Dot(&mDirection, &mWorldUp) * mWorldUp);
	D3DXVec3Normalize(&forward, &forward);
	mPosition -= forward * C_MOVE_SPEED * (float)gameTime.GetTimeSinceLastTick().Seconds;
}

// Move camera forwards: walk
void Camera::MoveForward(const GameTime& gameTime)
{
	D3DXVECTOR3 forward;

	forward = mDirection - (D3DXVec3Dot(&mDirection, &mWorldUp) * mWorldUp);
	D3DXVec3Normalize(&forward, &forward);
	mPosition += forward * C_MOVE_SPEED * (float)gameTime.GetTimeSinceLastTick().Seconds;
	mDestination = mPosition;
}

// Move camera to the left: strafe
void Camera::MoveLeft(const GameTime& gameTime)
{
	D3DXVECTOR3 right = GetRight();
	mPosition -= right * C_MOVE_SPEED * (float)gameTime.GetTimeSinceLastTick().Seconds;
	mDestination = mPosition;
}

// Move camera to the right: strafe
void Camera::MoveRight(const GameTime& gameTime)
{
	D3DXVECTOR3 right = GetRight();
	mPosition += right * C_MOVE_SPEED * (float)gameTime.GetTimeSinceLastTick().Seconds;
	mDestination = mPosition;
}

// Tilt the camera horisontally: look left/right
void Camera::TurnHorizontal(const GameTime& gameTime, bool turnLeft)
{
	float angle = (float)gameTime.GetTimeSinceLastTick().Milliseconds * C_TILTING_SPEED;
	if(turnLeft)
		angle = -angle;

	D3DXVECTOR3 up;
	D3DXVec3Cross(&up, &mDirection, &GetRight());
	D3DXMATRIX rotation;

	D3DXMatrixRotationAxis(&rotation, &up, angle);
	D3DXVec3TransformCoord(&mDirection, &mDirection, &rotation);
	D3DXVec3Normalize(&mDirection, &mDirection);
}

// Tilt the camera vertically: look up/down
//void Camera::TurnVertical(float angle)
//{
//	D3DXVECTOR3 right = GetRight();
//	D3DXMATRIX rotation;
//
//	D3DXMatrixRotationAxis(&rotation, &right, angle);
//	D3DXVec3TransformCoord(&mDirection, &mDirection, &rotation);
//	D3DXVec3Normalize(&mDirection, &mDirection);
//}

// Create and return the view matrix for the camera
D3DXMATRIX Camera::GetViewMatrix() const
{
	D3DXVECTOR3 right, up, eyePos;
	D3DXMATRIX view;

	eyePos = mPosition + mDirection * mZoom;

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

	view.m[3][0] = -D3DXVec3Dot(&eyePos, &right);
	view.m[3][1] = -D3DXVec3Dot(&eyePos, &up);
	view.m[3][2] = -D3DXVec3Dot(&eyePos, &mDirection);

	return view;
}

// Return the projection matrix for the camera
const D3DXMATRIX& Camera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

// Create the projection matrix for the camera
void Camera::CreateProjectionMatrix(const Frustum& viewFrustum)
{
	ZeroMemory(&mProjectionMatrix, sizeof(mProjectionMatrix));
	
	float scaleY, scaleX, length;
	scaleY = 1.0f / std::tan(viewFrustum.fovY * 0.5f);
	scaleX = scaleY / viewFrustum.aspectRatio;
	length = viewFrustum.farDistance - viewFrustum.nearDistance;

	mProjectionMatrix.m[0][0] = scaleX;
	mProjectionMatrix.m[1][1] = scaleY;
	mProjectionMatrix.m[2][2] = viewFrustum.farDistance / length;
	mProjectionMatrix.m[2][3] = 1;
	mProjectionMatrix.m[3][2] = (-viewFrustum.nearDistance * viewFrustum.farDistance) / length;
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

void Camera::Zoom(short amount)
{
	mZoom += amount * C_ZOOM_SPEED;
	mZoom = Clamp(mZoom, C_ZOOM_MIN, C_ZOOM_MAX);
}

void Camera::SetDestination(const D3DXVECTOR3& destination)
{
	mDestination = destination;
}

/*
void Camera::MouseButtonPressed(int index, const InputState& currentState)
{
}

void Camera::MouseButtonReleased(int index, const InputState& currentState)
{
}

void Camera::MouseWheelMoved(short delta, const InputState& currentState)
{
	mZoom += delta * C_ZOOM_SPEED;
	mZoom = Clamp(mZoom, C_ZOOM_MIN, C_ZOOM_MAX);
}
*/
