#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <D3DX10.h>

extern D3DXMATRIX				gViewportMatrix;
extern const D3DXCOLOR			C_COLOR_WINDOW_BG;
extern const D3DXCOLOR			C_COLOR_COMPONENT_BG;

//void UpdateViewportMatrix(int newWidth, int newHeight);
//D3DXVECTOR2 TransformToViewport(const D3DXVECTOR2& vector);

template <typename T>
inline void SafeRelease(T*& resource)
{
	if(resource != NULL)
	{
		resource->Release();
		resource = NULL;
	}
}

template <typename T>
inline void SafeDelete(T*& item)
{
	delete item;
	item = NULL;
}

template <typename T>
inline void SafeDeleteArray(T*& item)
{
	delete[] item;
	item = NULL;
}

template <typename T>
inline T Clamp(T value, const T& minValue, const T& maxValue)
{
	return value < minValue ? minValue : (value > maxValue ? maxValue : value);
}

#endif