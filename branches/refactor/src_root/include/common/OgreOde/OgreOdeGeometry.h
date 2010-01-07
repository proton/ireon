#ifndef _OGREODEGEOMETRY_H_
#define _OGREODEGEOMETRY_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"

class CObject;

namespace OgreOde 
{
	class TransformGeometry;
	class TriangleMeshGeometry;

	class _OgreOdeExport Geometry:public MaintainedItem
	{
		friend class Space;
		friend class World;
		friend class TransformGeometry;

	public:
		enum Class
		{
			Class_Sphere		= dSphereClass,
			Class_Box			= dBoxClass,
			Class_Capsule		= dCCylinderClass,
			Class_InfinitePlane = dPlaneClass,
			Class_Ray			= dRayClass,
			Class_Transform		= dGeomTransformClass,
			Class_TriangleMesh	= dTriMeshClass
#ifdef OGREODE_TERRAINGEOMETRY
			,Class_Terrain		= dTerrainCallbackClass	
#endif
			,Class_NoGeometry
		};

	public:
		Geometry(Space* space = 0);
		virtual ~Geometry();

		virtual void setBody(Body* body);
		Body* getBody(); 

		virtual void setPosition(const Vector3& position);
		virtual void setOrientation(const Quaternion& orientation);	
		virtual const Vector3& getPosition();
		virtual const Quaternion& getOrientation();
		virtual const AxisAlignedBox& getAxisAlignedBox(); 

		void reparentDebugObject(Node* node);
		virtual void createDebugObject();
		virtual void destroyDebugObject();

		Space* getSpace(); 

		void enable();
		void disable();
		bool isEnabled(); 

		Geometry::Class getClass(); 

		void setCategoryBitfield(unsigned long bits); 
		void setCollisionBitfield(unsigned long bits); 
		unsigned long getCategoryBitfield(); 
		unsigned long getCollisionBitfield(); 

		int collide(Geometry* geometry,CollisionListener* listener = 0);

		void setMaxContacts(int max_contacts);
		int getMaxContacts();
		int getContactHighWaterMark();

		void setEncapsulator(Geometry* encapsulator);
		Geometry* getEncapsulator(){return _encapsulator;}

		void setUserData(unsigned long user_data){_user_data = user_data;}
		unsigned long getUserData(){return _user_data;}

		void setObject(CObject* object){_object = object;}
		CObject* getObject(){return _object;}

		virtual unsigned long getID();
		virtual void notify(Body* body);
		virtual void setDebug(bool debug);

	protected:
		dGeomID getGeometryID() const;
		void registerGeometry();
		dSpaceID getSpaceID(Space* space) const;

	protected:
		dGeomID _geom;
		int _max_contacts,_contact_high_water_mark;
		dContact* _contacts;
		DebugObject* _debug_obj;
		Node* _debug_node;
		Geometry* _encapsulator;
		AxisAlignedBox _bounding_box;
		Quaternion _orientation;
		Vector3 _position;

		static int _geometry_count;
		unsigned long _user_data;
		CObject* _object;
	};

	class _OgreOdeExport SphereGeometry:public Geometry
	{
	public:
		SphereGeometry(Real radius,Space* space = 0);
		~SphereGeometry();

		virtual void createDebugObject();

		void setRadius(Real radius);
		Real getRadius();
		Real getPointDepth(const Vector3& point);
	};

	class _OgreOdeExport BoxGeometry:public Geometry
	{
	public:
		BoxGeometry(const Vector3& size,Space* space = 0);
		~BoxGeometry();

		virtual void createDebugObject();

		void setSize(const Vector3& size);
		const Vector3& getSize();
		Real getPointDepth(const Vector3& point);

	protected:
		Vector3 _size;
	};

	class _OgreOdeExport InfinitePlaneGeometry:public Geometry
	{
	public:
		InfinitePlaneGeometry(const Plane& plane,Space* space = 0);
		~InfinitePlaneGeometry();

		void setDefinition(const Plane& plane);
		const Plane& getDefinition();
		Real getPointDepth(const Vector3& point);

		void setPosition(const Vector3& position);
		void setOrientation(const Quaternion& orientation);	
		const Vector3& getPosition();
		const Quaternion& getOrientation();
		const AxisAlignedBox& getAxisAlignedBox(); 

	protected:
		Plane _plane;
	};

	class _OgreOdeExport CapsuleGeometry:public Geometry
	{
	public:
		CapsuleGeometry(Real radius,Real length,Space* space = 0);
		~CapsuleGeometry();

		virtual void createDebugObject();

		void setDefinition(Real radius,Real length);
		Real getRadius();
		Real getLength();
		Real getPointDepth(const Vector3& point);
	};

	class _OgreOdeExport RayGeometry:public Geometry
	{
	public:
		RayGeometry(Real length,Space* space = 0);
		~RayGeometry();

		virtual void createDebugObject();
		
		void setLength(Real length);
		Real getLength();
		void setDefinition(const Vector3& start,const Vector3& direction);
		const Vector3& getStart();
		const Vector3& getDirection();

	protected:
		Vector3 _direction,_start;
	};

	class _OgreOdeExport TransformGeometry:public Geometry
	{
	public:
		TransformGeometry(Space* space);
		~TransformGeometry();

		virtual void createDebugObject();
		virtual void destroyDebugObject();

		void setEncapsulatedGeometry(Geometry* geometry);
		Geometry* getEncapsulatedGeometry() const;
	};

	class _OgreOdeExport TriangleMeshCollisionListener
	{
	public:
		TriangleMeshCollisionListener(){}
		virtual ~TriangleMeshCollisionListener(){}

		virtual bool collide(TriangleMeshGeometry* triangle_mesh,Geometry* geometry,int triangle){return true;}
	};

	class _OgreOdeExport TriangleMeshIntersectionListener
	{
	public:
		TriangleMeshIntersectionListener(){}
		virtual ~TriangleMeshIntersectionListener(){}

		virtual void intersect(TriangleMeshGeometry* triangle_mesh,Geometry* geometry,const int* triangles,int triangle_count){}
	};

	class _OgreOdeExport TriangleMeshRayListener
	{
	public:
		TriangleMeshRayListener(){}
		virtual ~TriangleMeshRayListener(){}

		virtual bool collide(TriangleMeshGeometry* triangle_mesh,RayGeometry* ray,int triangle,const Vector3& uv){return true;}
	};

	struct _OgreOdeExport TriangleMeshTriangle
	{
		Vector3 v0,v1,v2;
	};

	class _OgreOdeExport TriangleMeshGeometry:public Geometry
	{
	public:
		TriangleMeshGeometry(const Vector3* vertices,int vertex_count,const int* indices,int index_count,Space* space = 0);
		~TriangleMeshGeometry();

		Vector3 getPoint(int index,const Vector3& uv);
		TriangleMeshTriangle getTriangle(int index);

		void clearTemporalCoherenceCache(); 
		void enableTemporalCoherence(Geometry::Class geometry_class,bool enable); 
		bool isTemporalCoherenceEnabled(Geometry::Class geometry_class);

		static int _collisionCallback(dGeomID mesh,dGeomID object,int triangle);
		void setCollisionListener(TriangleMeshCollisionListener* collision_listener);

		static void _intersectionCallback(dGeomID mesh,dGeomID object,const int* triangles,int triangle_count);
		void setIntersectionListener(TriangleMeshIntersectionListener* intersection_listener); 

		static int _rayCallback(dGeomID mesh,dGeomID ray,int triangle,dReal u,dReal v);
		void setRayListener(TriangleMeshRayListener* ray_listener); 

		virtual void createDebugObject();

	protected:
		dTriMeshDataID _data;
		dVector3* _vertices;
		int* _indices;
		int _vertex_count,_index_count;

		TriangleMeshCollisionListener* _collision_listener;
		TriangleMeshIntersectionListener* _intersection_listener;
		TriangleMeshRayListener* _ray_listener;
	};

#ifdef OGREODE_TERRAINGEOMETRY
	class _OgreOdeExport TerrainGeometryHeightListener
	{
	public:
		TerrainGeometryHeightListener(){}
		virtual ~TerrainGeometryHeightListener(){}

		virtual Real heightAt(const Vector3& position) = 0;
	};

	class _OgreOdeExport TerrainGeometry:public Geometry
	{
	public:
		TerrainGeometry(uint nodes_per_side, Real side_length, Real max_height, Space* space = 0);
		~TerrainGeometry();

		Real getPointDepth(const Vector3& point);
		const Vector3& getPosition();
		const Quaternion& getOrientation();
		
		static dReal _heightCallback(void* data,dReal x,dReal z);
		void setHeightListener(TerrainGeometryHeightListener* listener);
		Real getHeightAt(const Vector3& position);

	protected:
		Real _max_height,_height_at;
		TerrainGeometryHeightListener* _listener;
		Ray _ray;
		RaySceneQuery* _ray_query;
	};
#endif
}

#endif
