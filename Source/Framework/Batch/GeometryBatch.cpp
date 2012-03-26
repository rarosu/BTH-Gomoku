#include "GeometryBatch.hpp"

GeometryBatch::RenderState::RenderState()
	: mBlending(false)
{}

GeometryBatch::GeometryJob::GeometryJob()
	: mBuffer(NULL)
	, mEffect(NULL)
	, mTechniqueIndex(0)
{}



GeometryBatch::GeometryBatch(ID3D10Device* device)
	: mDevice(device)
{

}

void GeometryBatch::Begin()
{
	mJobs.clear();
}

void GeometryBatch::End()
{
	// TODO: Sort all jobs, on buffer, effect and render state.

	for (unsigned int j = 0; j < mJobs.size(); ++j)
	{
		Technique& technique = mJobs[j].mEffect->GetTechniqueByIndex(mJobs[j].mTechniqueIndex);

		mJobs[j].mBuffer->Bind();
		for (unsigned int p = 0; p < technique.GetPassCount(); ++p)
		{
			if (!technique.GetPassByIndex(p).Apply(mDevice))
				throw std::runtime_error("Failed to apply effect");

			mJobs[j].mBuffer->Draw();
		}
	}
}

void GeometryBatch::Add(const GeometryJob& job)
{
	mJobs.push_back(job);
}

void GeometryBatch::Sort()
{

}
