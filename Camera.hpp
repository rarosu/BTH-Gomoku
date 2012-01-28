#ifndef CAMERA_H
#define CAMERA_H

#include <D3DX10.h>

class Camera
{
public:
	Camera(D3DXVECTOR3 position, D3DXVECTOR3 direction, D3DXVECTOR3 worldUp);
	void MoveLeft(bool running);
	void MoveRight(bool running);
	void MoveForward(bool running);
	void MoveBack(bool running);
	void TurnHorizontal(float angle);
	void TurnVertical(float angle);
	D3DXMATRIX GetViewMatrix();
	D3DXVECTOR3 GetPos();
	void SetHeight(float height);

private:
	D3DXVECTOR3				mPosition;
	D3DXVECTOR3				mDirection;
	D3DXVECTOR3				mWorldUp;
	const float				mSpeed;

	D3DXVECTOR3 GetRight();
};
#endif