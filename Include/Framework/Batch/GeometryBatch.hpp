#ifndef GEOMETRY_BATCH_HPP
#define GEOMETRY_BATCH_HPP

#include <vector>
#include "Buffer.hpp"
#include "Effect.hpp"

class GeometryBatch
{
public:
	struct RenderState
	{
		RenderState();

		bool mBlending;
	};

	struct GeometryJob
	{
		GeometryJob();

		VertexBuffer* mBuffer;
		Effect* mEffect;
		unsigned int mTechniqueIndex;
		RenderState mState;
	};

	GeometryBatch(ID3D10Device* device);

	void Begin();
	void End();

	void Add(const GeometryJob& job);
private:
	ID3D10Device* mDevice;
	std::vector<GeometryJob> mJobs;

	void Sort();

	GeometryBatch(const GeometryBatch& copy) {}
	GeometryBatch& operator=(const GeometryBatch& copy) {}
};

#endif