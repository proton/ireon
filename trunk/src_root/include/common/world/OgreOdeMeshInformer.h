/**
  * @file OgreOdeMeshInformer.h
  * Mesh informer
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: OgreOdeMeshInformer.h 715 2006-09-10 20:12:50Z mip $
 
  * Based on OgreOde project code
 
  *  See LICENSE for details
  */
 
#ifndef _OGREODEMESHINFORMER_H_
#define _OGREODEMESHINFORMER_H_
namespace Ogre
{
	class Mesh;
	class SceneNode;
};
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
 
 
