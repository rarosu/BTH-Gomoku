#include "Viewport.hpp"

Viewport::Viewport(int width, int height, float minDepth, float maxDepth)
{
	mDescription.Width = width;
	mDescription.Height = height;
	mDescription.TopLeftX = 0;
	mDescription.TopLeftY = 0;
	mDescription.MinDepth = minDepth;
	mDescription.MaxDepth = maxDepth;
}

void Viewport::Recalculate(ID3D10Device* device)
{
	// Recalculate the transform matrix
	mViewSpaceTransform.m[0][0] = 2.0f / mDescription.Width;
	mViewSpaceTransform.m[0][1] = 0.0f;
	mViewSpaceTransform.m[0][2] = 0.0f;
	mViewSpaceTransform.m[0][3] = 0.0f;

	mViewSpaceTransform.m[1][0] = 0.0f;
	mViewSpaceTransform.m[1][1] = -2.0f / mDescription.Height;
	mViewSpaceTransform.m[1][2] = 0.0f;
	mViewSpaceTransform.m[1][3] = 0.0f;

	mViewSpaceTransform.m[2][0] = -1.0f;
	mViewSpaceTransform.m[2][1] = 1.0f;
	mViewSpaceTransform.m[2][2] = 1.0f;
	mViewSpaceTransform.m[2][3] = 0.0f;

	mViewSpaceTransform.m[3][0] = 0.0f;
	mViewSpaceTransform.m[3][1] = 0.0f;
	mViewSpaceTransform.m[3][2] = 0.0f;
	mViewSpaceTransform.m[3][3] = 1.0f;

	device->RSSetViewports(1,						// Number of viewports
							&mDescription);			// First element of array with viewports to bind
}

D3DXVECTOR2 Viewport::TransformToViewport(const D3DXVECTOR2& vector) const
{
	D3DXVECTOR4 transformedVector;
	D3DXVec4Transform(&transformedVector, &D3DXVECTOR4(vector.x, vector.y, 1, 0), &mViewSpaceTransform);

	return D3DXVECTOR2(transformedVector.x, transformedVector.y);
}

int Viewport::GetWidth() const
{
	return mDescription.Width;
}

void Viewport::SetWidth(int newWidth)
{
	mDescription.Width = newWidth;
}

int Viewport::GetHeight() const
{
	return mDescription.Height;
}

void Viewport::SetHeight(int newHeight)
{
	mDescription.Height = newHeight;
}

const D3DXMATRIX& Viewport::GetMatrix() const
{
	return mViewSpaceTransform;
}