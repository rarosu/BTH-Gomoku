#include "Scene.hpp"

Scene::Scene(ID3D10Device* device) :
	mDevice(device)
{

}


void Scene::Update(const Logic::Grid* grid)
{
	Logic::Grid::MarkerMap::const_iterator it = grid->GetFirstOccupiedCell();
	for (; it != grid->GetMarkerMapEnd(); it++)
	{
		
	}
}

void Scene::Render(const Camera* camera)
{

}
