#ifndef _OGREODEDEBUGOBJECT_H_
#define _OGREODEDEBUGOBJECT_H_

#include "OgreOdePreReqs.h"

namespace OgreOde
{
	class _OgreOdeExport DebugLines	:public Ogre::SimpleRenderable
	{
	public:
		DebugLines(void);
		~DebugLines(void);

		void addLine(const Vector3 &start,const Vector3 &end)
		{
			clear();

			_points.push_back(start);
			_points.push_back(end);
		}
		void addLine(Real start_x,Real start_y,Real start_z,Real end_x,Real end_y,Real end_z)
		{
			addLine(Vector3(start_x,start_y,start_z),Vector3(end_x,end_y,end_z));
		}
		void draw();
		void clear();

		Real getSquaredViewDepth(const Camera *cam) const;
		Real getBoundingRadius(void) const;

	protected:

		std::vector<Vector3> _points;
		bool _drawn;

		static bool _materials_created;
	};

	class _OgreOdeExport DebugObject:public DebugLines
	{
	public:
		enum Mode
		{
			Mode_Unknown,
			Mode_Enabled,
			Mode_Disabled,
			Mode_Static
		};

	public:
        DebugObject(DebugObject::Mode mode = DebugObject::Mode_Enabled);
		virtual ~DebugObject();

		void setMode(DebugObject::Mode mode);

	protected:
		DebugObject::Mode _mode;
	};

	class _OgreOdeExport BoxDebugObject:public DebugObject
	{
	public:
		BoxDebugObject(const Vector3& size);
		virtual ~BoxDebugObject();
	};

	class _OgreOdeExport SphereDebugObject:public DebugObject
	{
	public:
		SphereDebugObject(Real radius);
		virtual ~SphereDebugObject();
	};

	class _OgreOdeExport CapsuleDebugObject:public DebugObject
	{
	public:
		CapsuleDebugObject(Real radius,Real length);
		virtual ~CapsuleDebugObject();
	};

	class _OgreOdeExport TriangleMeshDebugObject:public DebugObject
	{
	public:
		TriangleMeshDebugObject(int vertex_count);
		virtual ~TriangleMeshDebugObject();

		void beginDefinition();
		void setVertex(int index,const Vector3& vertex);
		void endDefinition();
	};

	class _OgreOdeExport RayDebugObject:public DebugObject
	{
	public:
		RayDebugObject(const Vector3& start,const Vector3& direction,Real length);
		virtual ~RayDebugObject();
	};
}

#endif
