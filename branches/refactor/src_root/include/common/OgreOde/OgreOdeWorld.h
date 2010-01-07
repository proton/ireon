#ifndef _OGREODEWORLD_H_
#define _OGREODEWORLD_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"
#include "OgreSingleton.h"

#include <map>

namespace OgreOde 
{
	/** The main class which you will use to simulate your world.
     * 
	 * This is the core class of OgreOde, it is directly analogous to the ODE world except that
	 * in OgreOde you can have only one world, which is why it is Singleton.
	 *
	 * A World is a container for all the bodies and geometries in you simulation. You also
	 * use it to set global options for things like gravity, ERP, CFM and automatic sleeping.
	 * Also, the World is what you step when you want to advance your simulation by a certain time
	 * period.
	 */
	class _OgreOdeExport World:public Singleton<World>
    {
		friend class Body;
		friend class JointGroup;
		friend class Joint;
		friend class Space;
		friend class Geometry;
		friend class Stepper;

	public:
		/** Construct a new World.
		 * 
		 * You will need to call this to create the World into which you cabn place your physical objects.
		 * The World requires a reference to the scene manager you are using in order to create things
		 * like nodes for debug objects and to obtain configuration options for world geometry.
		 * 
		 * \param manager The scene manager you are using.
		 */
		World(SceneManager* manager);
		~World();

		void setGravity(const Vector3& gravity = Vector3(0,0,0));
		const Vector3& getGravity();

		void setERP(Real erp = 0.2);
		Real getERP();
		void setCFM(Real cfm = 10e-5);
		Real getCFM();

		void setContactCorrectionVelocity(Real velocity = 0.1);
		void setContactSurfaceLayer(Real layer = 0.001);

		void setAutoSleep(bool auto_sleep = false);
		bool getAutoSleep();
		void setAutoSleepLinearThreshold(Real linear_threshold = 0.01);
		Real getAutoSleepLinearThreshold();
		void setAutoSleepAngularThreshold(Real angular_threshold = 0.01);
		Real getAutoSleepAngularThreshold();
		void setAutoSleepSteps(int steps = 10);
		int getAutoSleepSteps();
		void setAutoSleepTime(Real time = 0);
		Real getAutoSleepTime(); 

		void step(Real stepsize); 
		void quickStep(Real stepsize);

		void setQuickStepIterations(int iterations);
		int getQuickStepIterations();

		static World& getSingleton();
        static World* getSingletonPtr();

		Body* findBody(SceneNode* node);
		Body* findBody(const String& name);

		void clearContacts();

		void setCollisionListener(CollisionListener* collision_listener);
		CollisionListener* getCollisionListener();

		void setShowDebugObjects(bool show);
		bool getShowDebugObjects(){return _show_debug;}

		void notifyGeometry(Body* body);

		Space* getDefaultSpace(){return _default_space;}
		void setDefaultSpace(Space* space);

		SceneManager* getSceneManager(){return _manager;}

		void setDamping(Real linear_damping,Real angular_damping);
		Real getLinearDamping();
		Real getAngularDamping();

		void synchronise();

	protected:
		dWorldID getWorldID();
		dJointGroupID getContactGroupID();

		MaintainedList &getBodyList(){return _body_list;}
		MaintainedList &getJointList(){return _joint_list;}
		MaintainedList &getJointGroupList(){return _joint_group_list;}
		MaintainedList &getGeometryList(){return _geometry_list;}
		MaintainedList &getSpaceList(){return _space_list;}

		static void collisionCallback(void *data,dGeomID geom_a,dGeomID geom_b);

	protected:
		dWorldID _world;
		dJointGroupID _contacts;

		Space* _default_space;
		
		static CollisionListener* _collision_listener;

		MaintainedList _body_list;
		MaintainedList _joint_list;
		MaintainedList _joint_group_list;
		MaintainedList _geometry_list;
		MaintainedList _space_list;

		bool _show_debug;
		SceneManager* _manager;

		Vector3 _gravity;
		dReal _linear_damping,_angular_damping;
	};
}

#endif
