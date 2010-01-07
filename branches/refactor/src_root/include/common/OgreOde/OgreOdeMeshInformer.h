#ifndef _OGREODEMESHINFORMER_H_
#define _OGREODEMESHINFORMER_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
    class _OgreOdeExport MeshInformer
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

		static Body* createSingleDynamicSphere(SceneNode* node,Real mass,Space* space = 0);
		static Body* createSingleDynamicBox(SceneNode* node,Real mass,Space* space = 0);

		static TriangleMeshGeometry* createStaticTriangleMesh(SceneNode* node,Space* space = 0);
		static BoxGeometry* createSingleStaticBox(SceneNode* node,Space* space = 0);

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


