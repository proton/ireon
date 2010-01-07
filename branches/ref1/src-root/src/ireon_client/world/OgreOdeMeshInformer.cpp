/**
 * @file OgreOdeMeshInformer.cpp
 * Mesh informer
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

 * Based on OgreOde project code

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "stdafx.h"
#include "world/OgreOdeMeshInformer.h"

using namespace OgreOde;

MeshInformer::MeshInformer(Mesh* mesh)
{
	_vertices = 0;
	_indices = 0;
	_vertex_count = 0;
	_index_count = 0;
	_size = Vector3(0,0,0);
	_radius = 0;
	_got_radius = false;
	_got_size = false;

	if(mesh) addMesh(mesh);
}

Real MeshInformer::getRadius()
{
	if(!_got_radius)
	{
		getSize();
		_radius = (std::max(_size.x,std::max(_size.y,_size.z)) * 0.5);
		_got_radius = true;
	}
	return _radius;
}

Vector3 MeshInformer::getSize()
{
	if(!_got_size)
	{
		Vector3 vmin(FLT_MAX,FLT_MAX,FLT_MAX);
		Vector3 vmax(FLT_MIN,FLT_MIN,FLT_MIN);

		int i = getVertexCount();
		const Vector3* v = getVertices();

		for(int j = 0;j < i;j++)
		{
			vmin.x = std::min(vmin.x,v[j].x);
			vmin.y = std::min(vmin.y,v[j].y);
			vmin.z = std::min(vmin.z,v[j].z);

			vmax.x = std::max(vmax.x,v[j].x);
			vmax.y = std::max(vmax.y,v[j].y);
			vmax.z = std::max(vmax.z,v[j].z);
		}

		_size.x = vmax.x - vmin.x;
		_size.y = vmax.y - vmin.y;
		_size.z = vmax.z - vmin.z;

		_got_size = true;
	}

	return _size;
}

void MeshInformer::addMesh(Mesh* mesh,const Vector3& position,const Quaternion& orient,const Vector3& scale)
{
	bool added_shared = false;
	size_t current_offset = _vertex_count,shared_offset = _vertex_count,next_offset = _vertex_count,index_offset = _index_count;
	size_t prev_vert = _vertex_count,prev_ind = _index_count;

	_got_size = _got_radius = false;

	// Calculate how many vertices and indices we're going to need
	for(int i = 0;i < mesh->getNumSubMeshes();i++)
	{
		SubMesh* submesh = mesh->getSubMesh(i);

		// We only need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if(!added_shared)
			{
				VertexData* vertex_data = mesh->sharedVertexData;
				_vertex_count += vertex_data->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			VertexData* vertex_data = submesh->vertexData;
			_vertex_count += vertex_data->vertexCount;
		}
				
		// Add the indices
		Ogre::IndexData* index_data = submesh->indexData;
		_index_count += index_data->indexCount;
	}
		
	// (re)allocate space for the vertices and indices
	Vector3* tmp_vert = new Vector3[_vertex_count];
	if(prev_vert)
	{
		memcpy(tmp_vert,_vertices,sizeof(Vector3) * prev_vert);
		delete[] _vertices;
	}
	_vertices = tmp_vert;

	int* tmp_ind = new int[_index_count];
	if(prev_ind)
	{
		memcpy(tmp_ind,_indices,sizeof(int) * prev_ind);
		delete[] _indices;
	}
	_indices = tmp_ind;
			
	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for(int i = 0;i < mesh->getNumSubMeshes();i++)
	{
		SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			Ogre::Real* pReal;

			for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Vector3 pt;

				pt.x = (*pReal++);
				pt.y = (*pReal++);
				pt.z = (*pReal++);

				pt = (orient * (pt * scale)) + position;

				_vertices[current_offset + j].x = pt.x;
				_vertices[current_offset + j].y = pt.y;
				_vertices[current_offset + j].z = pt.z;
			}
			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}

		Ogre::IndexData* index_data = submesh->indexData;

		size_t numTris = index_data->indexCount / 3;
		unsigned short* pShort;
		unsigned int* pInt;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
		if (use32bitindexes) pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		else pShort = static_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		for(size_t k = 0; k < numTris; ++k)
		{
			size_t offset = (submesh->useSharedVertices)?shared_offset:current_offset;
					
			unsigned int vindex = use32bitindexes? *pInt++ : *pShort++;
			_indices[index_offset + 0] = vindex + offset;
			vindex = use32bitindexes? *pInt++ : *pShort++;
			_indices[index_offset + 1] = vindex + offset;
			vindex = use32bitindexes? *pInt++ : *pShort++;
			_indices[index_offset + 2] = vindex + offset;

			index_offset += 3;
		}
		ibuf->unlock();
		current_offset = next_offset;
	}
}

const Vector3* MeshInformer::getVertices()
{
	return _vertices;
}

int MeshInformer::getVertexCount()
{
	return (int)_vertex_count;
}

const int* MeshInformer::getIndices()
{
	return _indices;
}

int MeshInformer::getIndexCount()
{
	return (int)_index_count;
}

Vector3 MeshInformer::getSize(Mesh* mesh)
{
	MeshInformer m(mesh);
	return m.getSize();
}

Real MeshInformer::getRadius(Mesh* mesh)
{
	MeshInformer m(mesh);
	return m.getRadius();
}

int MeshInformer::addAllMeshes(SceneNode* node,const Vector3& position,const Quaternion& orient,const Vector3& scale)
{
	unsigned short i,attached = node->numAttachedObjects();

	for(i = 0;i < attached;++i)
	{
		MovableObject* mo = node->getAttachedObject(i);
		if(mo->getMovableType() == "Entity")
		{
			addMesh(((Entity*)mo)->getMesh().get(),position,orient,scale);
		}
	}
	return attached;
}

MeshInformer::~MeshInformer()
{
	delete[] _vertices;
	delete[] _indices;
}
