#ifndef GLOBALS_HPP
#define GLOBALS_HPP

//#define SafeRelease(x) { if(x) { (x)->Release(); x = NULL; } }

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

#endif