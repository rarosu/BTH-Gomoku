#include "Globals.hpp"

D3DXMATRIX gViewPortMatrix;
const D3DXCOLOR	C_COLOR_WINDOW_BG = D3DXCOLOR(0.85f, 0.75f, 0.65f, 1.0);
const D3DXCOLOR	C_COLOR_COMPONENT_BG = D3DXCOLOR(0.8f, 0.7f, 0.6f, 1.0);

D3DXVECTOR2 TransformToViewport(const D3DXVECTOR2& vector)
{
	D3DXVECTOR4 transformedVector;
	D3DXVec4Transform(&transformedVector, &D3DXVECTOR4(vector.x, vector.y, 1, 0), &gViewPortMatrix);

	return D3DXVECTOR2(transformedVector.x, transformedVector.y);
}

void UpdateViewportMatrix(int newWidth, int newHeight)
{
	gViewPortMatrix.m[0][0] = 2.0f / newWidth;
	gViewPortMatrix.m[0][1] = 0.0f;
	gViewPortMatrix.m[0][2] = 0.0f;
	gViewPortMatrix.m[0][3] = 0.0f;

	gViewPortMatrix.m[1][0] = 0.0f;
	gViewPortMatrix.m[1][1] = -2.0f / newHeight;
	gViewPortMatrix.m[1][2] = 0.0f;
	gViewPortMatrix.m[1][3] = 0.0f;

	gViewPortMatrix.m[2][0] = -1.0f;
	gViewPortMatrix.m[2][1] = 1.0f;
	gViewPortMatrix.m[2][2] = 1.0f;
	gViewPortMatrix.m[2][3] = 0.0f;

	gViewPortMatrix.m[3][0] = 0.0f;
	gViewPortMatrix.m[3][1] = 0.0f;
	gViewPortMatrix.m[3][2] = 0.0f;
	gViewPortMatrix.m[3][3] = 1.0f;
}