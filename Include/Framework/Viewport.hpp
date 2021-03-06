#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "WinInclude.hpp"

class Viewport
{
public:
	Viewport(int width, int height, float minDepth, float maxDepth);

	void Recalculate(ID3D10Device* device);
	D3DXVECTOR2 TransformToViewport(const D3DXVECTOR2& vector) const;
	int GetWidth() const;
	void SetWidth(int newWidth);
	int GetHeight() const;
	void SetHeight(int newHeight);
	const D3DXMATRIX& GetMatrix() const;

private:
	D3DXMATRIX			mViewSpaceTransform;
	D3D10_VIEWPORT		mDescription;
};
#endif