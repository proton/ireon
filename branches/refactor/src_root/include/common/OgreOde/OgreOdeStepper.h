#ifndef _OGREODESTEPPER_H_
#define _OGREODESTEPPER_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
	class _OgreOdeExport StepListener
	{
	public:
		StepListener(){};
		~StepListener(){};

		virtual bool preStep(Real time) = 0;
	};

	class _OgreOdeExport Stepper
		:public FrameListener
    {
	public:
		enum AutoMode
		{
			AutoMode_NotAutomatic,
			AutoMode_PreFrame,
			AutoMode_PostFrame
		};

	public:
		Stepper();
		virtual ~Stepper();

		virtual void step(Real time) = 0;

		void pause(bool pause);
		bool isPaused();

		void setStepListener(StepListener* listener);
		void setAutomatic(Stepper::AutoMode mode,Root* root = 0);

		bool frameStarted(const FrameEvent& evt);
		bool frameEnded(const FrameEvent& evt);
	protected:
		World* _world;
		bool _paused,_auto_pre,_auto_post;
		StepListener* _listener;
		Root* _root;
	};

	class _OgreOdeExport QuickStepper:public Stepper
	{
	public:
		QuickStepper(Real step_size);
		virtual ~QuickStepper();

		virtual void step(Real time);

		Real getStepSize(){return _step_size;}
		void setStepSize(Real step_size){_step_size = step_size;}

	protected:
		Real _step_size,_total_time;
	};

	class _OgreOdeExport ForwardFixedQuickStepper:public QuickStepper
	{
	public:
		ForwardFixedQuickStepper(Real step_size);
		virtual ~ForwardFixedQuickStepper();

		virtual void step(Real time);
	};

	class _OgreOdeExport ExactVariableQuickStepper:public QuickStepper
	{
	public:
		ExactVariableQuickStepper(Real step_size);
		virtual ~ExactVariableQuickStepper();

		virtual void step(Real time);
	};
}

#endif
