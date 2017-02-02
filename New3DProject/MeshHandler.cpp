#include "MeshHandler.hpp"

MeshHandler::MeshHandler()
{
	this->capacity = CAP;
	this->nrOfMeshes = 0;

	this->meshes = new Mesh*[this->capacity];
	this->nullArray();
}

MeshHandler::~MeshHandler()
{
	this->nullArray();
}

void MeshHandler::nullArray()
{
	for (int i = 0; i < this->capacity; i++)
	{
		// May delete object.
		this->meshes[i] = nullptr;
	}
}