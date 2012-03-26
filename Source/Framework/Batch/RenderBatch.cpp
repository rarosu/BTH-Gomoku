#include "RenderBatch.hpp"
#include <algorithm>

RenderBatch::RenderState::RenderState()
	: mBlending(false)
{}

RenderBatch::GeometryJob::GeometryJob()
	: mBuffer(NULL)
	, mEffect(NULL)
	, mTechniqueIndex(0)
{}



RenderBatch::RenderBatch(ID3D10Device* device)
	: mDevice(device)
{}

void RenderBatch::Begin()
{
	mSceneJobs.clear();
	mBlendedSceneJobs.clear();
	mHUDJobs.clear();
}

void RenderBatch::End()
{
	// Sort all items
	SortGeometricJobs(mSceneJobs);
	SortGeometricJobs(mBlendedSceneJobs);
	SortGeometricJobs(mHUDJobs);

	// Render scene items (w/ lights)
	// TODO: Regard lighting
	for (unsigned int j = 0; j < mSceneJobs.size(); ++j)
	{
		Technique& technique = mSceneJobs[j].mEffect->GetTechniqueByIndex(mSceneJobs[j].mTechniqueIndex);

		mSceneJobs[j].mBuffer->Bind();
		for (unsigned int p = 0; p < technique.GetPassCount(); ++p)
		{
			if (!technique.GetPassByIndex(p).Apply(mDevice))
				throw std::runtime_error("Failed to apply effect");

			mSceneJobs[j].mBuffer->Draw();
		}
	}

	// Render HUD elements (w/out lights)
	for (unsigned int j = 0; j < mHUDJobs.size(); ++j)
	{
		Technique& technique = mHUDJobs[j].mEffect->GetTechniqueByIndex(mHUDJobs[j].mTechniqueIndex);

		mHUDJobs[j].mBuffer->Bind();
		for (unsigned int p = 0; p < technique.GetPassCount(); ++p)
		{
			if (!technique.GetPassByIndex(p).Apply(mDevice))
				throw std::runtime_error("Failed to apply effect");

			mHUDJobs[j].mBuffer->Draw();
		}
	}

	// TODO: Reset device state
}

void RenderBatch::AddSceneJob(const GeometryJob& job)
{
	// Since we are using different techniques (deferred and forward) for normal
	// scene objects and blended scene objects, we put them in different lists.
	if (!job.mState.mBlending)
		mSceneJobs.push_back(job);
	else
		mBlendedSceneJobs.push_back(job);
}

void RenderBatch::AddHUDJob(const GeometryJob& job)
{
	mHUDJobs.push_back(job);
}

void RenderBatch::SetAmbientLight(const AmbientLight& light)
{
	mAmbientLight = light;
}

void RenderBatch::AddPointLight(const PointLight& light)
{
	mPointLight.push_back(light);
}

void RenderBatch::AddDirectionalLight(const DirectionalLight& light)
{
	mDirectionalLights.push_back(light);
}

bool CompareGeometricJobs(const RenderBatch::GeometryJob& lhs, const RenderBatch::GeometryJob& rhs)
{
	// Sort first on blending (blended objects are always rendered last) ...
	if (rhs.mState.mBlending)
		return true;

	// ... Then sort on the buffer data ...
	if (lhs.mBuffer != rhs.mBuffer)
	{
		// Sort on vertex buffer first ...
		if (!lhs.mBuffer->IsVertexBufferEqual(*rhs.mBuffer))
			return lhs.mBuffer->GetVertexBuffer() < rhs.mBuffer->GetVertexBuffer();

		// ... Then on index buffer ...
		if (!lhs.mBuffer->IsIndexBufferEqual(*rhs.mBuffer))
			return lhs.mBuffer->GetIndexBuffer() < rhs.mBuffer->GetIndexBuffer();

		// ... Finally on topology.
		if (!lhs.mBuffer->IsTopologyEqual(*rhs.mBuffer))
			return lhs.mBuffer->GetTopology() < rhs.mBuffer->GetTopology();
	}

	// ... If the buffers are equal, sort on effect.
	return lhs.mEffect->GetEffect() < rhs.mEffect->GetEffect();
}

void RenderBatch::SortGeometricJobs(GeometryBatch& batch)
{
	// Sort on vertex buffer first, and secondly on effect.
	std::sort(batch.begin(), batch.end(), CompareGeometricJobs);
}
