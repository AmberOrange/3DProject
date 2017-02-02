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

void MeshHandler::Expand()
{
}

bool MeshHandler::add(Mesh &mesh)
{
	// Fix the Expand function please D=

	if (this->nrOfMeshes == this->capacity)
		this->Expand();

	this->meshes[this->nrOfMeshes++] = mesh.Clone();

	return true;
}

bool MeshHandler::Initialize(ID3D11Device *device, HWND hwnd)
{
	this->add(Object("Crate.obj"));

	for (int i = 0; i < this->nrOfMeshes; i++)
	{
		this->meshes[i]->Initialize(device);
	}

	return true;
}

bool MeshHandler::DrawAll(ID3D11DeviceContext *deviceContext)
{
	for (int i = 0; i < this->nrOfMeshes; i++)
	{
		this->meshes[i]->Draw(deviceContext);
	}

	return true;
}