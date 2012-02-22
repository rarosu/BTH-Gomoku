#include "Globals.hpp"

D3DXMATRIX gViewportMatrix;
const D3DXCOLOR		C_COLOR_WINDOW_BG		= D3DXCOLOR(0.85f, 0.75f, 0.65f, 1.0);
const D3DXCOLOR		C_COLOR_COMPONENT_BG	= D3DXCOLOR(0.8f, 0.7f, 0.6f, 1.0);
const D3DXCOLOR		C_COLOR_TEXT			= D3DXCOLOR(0.4f, 0.3f, 0.2f, 1.0f);

//D3DXVECTOR2 TransformToViewport(const D3DXVECTOR2& vector)
//{
//	D3DXVECTOR4 transformedVector;
//	D3DXVec4Transform(&transformedVector, &D3DXVECTOR4(vector.x, vector.y, 1, 0), &gViewportMatrix);
//
//	return D3DXVECTOR2(transformedVector.x, transformedVector.y);
//}

//void UpdateViewportMatrix(int newWidth, int newHeight)
//{
//	gViewportMatrix.m[0][0] = 2.0f / newWidth;
//	gViewportMatrix.m[0][1] = 0.0f;
//	gViewportMatrix.m[0][2] = 0.0f;
//	gViewportMatrix.m[0][3] = 0.0f;
//
//	gViewportMatrix.m[1][0] = 0.0f;
//	gViewportMatrix.m[1][1] = -2.0f / newHeight;
//	gViewportMatrix.m[1][2] = 0.0f;
//	gViewportMatrix.m[1][3] = 0.0f;
//
//	gViewportMatrix.m[2][0] = -1.0f;
//	gViewportMatrix.m[2][1] = 1.0f;
//	gViewportMatrix.m[2][2] = 1.0f;
//	gViewportMatrix.m[2][3] = 0.0f;
//
//	gViewportMatrix.m[3][0] = 0.0f;
//	gViewportMatrix.m[3][1] = 0.0f;
//	gViewportMatrix.m[3][2] = 0.0f;
//	gViewportMatrix.m[3][3] = 1.0f;
//}