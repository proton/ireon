/**
 * @file OgreOdeMeshInformer.h
 * Mesh informer
 */

/* Copyright (C) 2005 ireon.org developers council
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

#ifndef _OGREODEMESHINFORMER_H_
#define _OGREODEMESHINFORMER_H_

namespace OgreOde 
{
    class MeshInformer
    {
	public:
		MeshInformer(Mesh* mesh = 0);
		~MeshInformer();

		void addMesh(Mesh* mesh,const Vector3& position = Vector3::ZERO,const Quaternion& orient = Quaternion::IDENTITY,const Vector3& scale = Vector3::UNIT_SCALE);
		int addAllMeshes(SceneNode* node,const Vector3& position = Vector3::ZERO,const Quaternion& orient = Quaternion::IDENTITY,const Vector3& scale = Vector3::UNIT_SCALE);

		Real getRadius();
		Vector3 getSize();

		static Vector3 getSize(Mesh* mesh);
		static Real getRadius(Mesh* mesh);

		const Vector3* getVertices();
		int getVertexCount();
		const int* getIndices();
		int getIndexCount();

	protected:
		Vector3*	_vertices;
		int*		_indices;
		size_t		_vertex_count;
		size_t		_index_count;

		Real		_radius;
		Vector3		_size;

		bool		_got_radius,_got_size;
	};
}

#endif


