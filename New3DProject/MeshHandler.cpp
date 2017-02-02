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
	for (int i = 0; i < this->nrOfMeshes; i++)
		delete this->meshes[i];

	delete[]this->meshes;
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
	this->capacity += 100;
	Mesh **newMeshes = new Mesh*[this->capacity];

	for (int i = 0; i < this->capacity; i++)
	{
		if (i < this->nrOfMeshes)
			newMeshes[i] = this->meshes[i];
		else
			newMeshes[i] = nullptr;
	}

	delete[]this->meshes;
	this->meshes = newMeshes;
	newMeshes = nullptr;
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
	this->add(Object("Cow.obj"));
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