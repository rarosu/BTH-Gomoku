#ifndef RENDER_BATCH_HPP
#define RENDER_BATCH_HPP

#include <vector>
#include "WinInclude.hpp"
#include "Buffer.hpp"
#include "Effect.hpp"

/**
	Defines an ambient light

	mColor - The color of the light
	mIntensity - A value [0.0f, 1.0f], defining the strength of the light
*/
struct AmbientLight
{
	D3DXCOLOR mColor;
	float mIntensity;
};

/**
	Defines a point light

	mPosition - The origin of the light
	mColor - The color of the light
	mIntensity - A value [0.0f, 1.0f], defining the strength of the light
*/
struct PointLight
{
	D3DXVECTOR3 mPosition;
	D3DXCOLOR mColor;
	float mIntensity;
};

/**
	Defines a directional light

	mDirection - The direction the light will shine from
	mColor - The color of the light
	mIntensity - A value [0.0f, 1.0f], defining the strength of the light
*/
struct DirectionalLight
{
	D3DXVECTOR3 mDirection;
	D3DXCOLOR mColor;
	float mIntensity;
};

/*
struct SpotLight
{
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mDirection;
	D3DXCOLOR mColor;
	float mIntensity;
	float mSpreadAngle;
};
*/


/**
	A class for batching the render process. It will sort items depending 
	on their vertex buffer, chosen effect, topology and render state.
*/
class RenderBatch
{
public:
	/**
		Defines the state to use when rendering a geometric object.
	*/
	struct RenderState
	{
		RenderState();

		bool mBlending;
	};

	/**
		Defines one geometry job - for rendering an object
	*/
	struct GeometryJob
	{
		GeometryJob();

		VertexBuffer* mBuffer;
		Effect* mEffect;
		unsigned int mTechniqueIndex;
		RenderState mState;
	};



	RenderBatch(ID3D10Device* device);

	void Begin();
	void End();

	void AddSceneJob(const GeometryJob& job);
	void AddHUDJob(const GeometryJob& job);

	void SetAmbientLight(const AmbientLight& light);
	void AddPointLight(const PointLight& light);
	void AddDirectionalLight(const DirectionalLight& light);
private:
	typedef std::vector<GeometryJob> GeometryBatch;

	// Store a reference to the Direct3D device
	ID3D10Device* mDevice;
	
	// Store all geometry data
	GeometryBatch mSceneJobs;			// Deferred render, light
	GeometryBatch mBlendedSceneJobs;	// Forward render, light
	GeometryBatch mHUDJobs;				// Forward render, no light

	// Store all light data
	AmbientLight mAmbientLight;
	std::vector<PointLight> mPointLight;
	std::vector<DirectionalLight> mDirectionalLights;

	/**
		Sort all geometric jobs on buffers and effect
		to optimize the pipeline.
	*/
	void SortGeometricJobs(GeometryBatch& batch);

	void RenderDeferred(const GeometryJob& job);
	void RenderForward(const GeometryJob& job);

	/**
		Disable copying
	*/
	RenderBatch(const RenderBatch& copy) {}
	RenderBatch& operator=(const RenderBatch& copy) {}
};

#endif