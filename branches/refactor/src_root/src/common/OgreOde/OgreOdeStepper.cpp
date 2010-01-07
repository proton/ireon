#include "OgreOdeWorld.h"
#include "OgreOdeSpace.h"
#include "OgreOdeStepper.h"

using namespace OgreOde;

Stepper::Stepper()
{
	_world = World::getSingletonPtr();
	_paused = false;
	_listener = 0;
	_auto_pre = false;
	_auto_post = false;
	_root = 0;
}

void Stepper::pause(bool pause)
{
	_paused = pause;
}

void Stepper::setStepListener(StepListener* listener)
{
	_listener = listener;
}

bool Stepper::isPaused()
{
	return _paused;
}

void Stepper::setAutomatic(Stepper::AutoMode mode,Root* root)
{
	if((_root)&&((_auto_pre)||(_auto_post))) _root->removeFrameListener(this);

	_auto_pre = (mode == AutoMode_PreFrame)?true:false;
	_auto_post = (mode == AutoMode_PostFrame)?true:false;

	if((root)&&((_auto_pre)||(_auto_post)))
	{
		_root = root;
		_root->addFrameListener(this);
	}
}

bool Stepper::frameStarted(const FrameEvent& evt)
{
	if(_auto_pre)
	{
		step(evt.timeSinceLastFrame);
		_world->synchronise();
	}
	return true;
}

bool Stepper::frameEnded(const FrameEvent& evt)
{
	if(_auto_post)
	{
		step(evt.timeSinceLastFrame);
		_world->synchronise();
	}
	return true;
}

Stepper::~Stepper()
{
	setAutomatic(AutoMode_NotAutomatic);
}

QuickStepper::QuickStepper(Real step_size):Stepper()
{
	assert((step_size > 0)&&("Step size must be greater than zero"));
	_step_size = step_size;
	_total_time = 0.0;
}

void QuickStepper::step(Real time)
{
	if((!_listener)||(_listener->preStep(time)))
	{
		_world->getDefaultSpace()->collide();
		_world->quickStep(time);
		_world->clearContacts();
	}
}

QuickStepper::~QuickStepper()
{
}

ForwardFixedQuickStepper::ForwardFixedQuickStepper(Real step_size):QuickStepper(step_size)
{
}

void ForwardFixedQuickStepper::step(Real time)
{
	if(!_paused)
	{
		// Slightly different type of ODE stepper, always makes sure
		// the ODE world is at least level with real-time, and possibly
		// up to one step ahead, seems to make things a bit smoother (?)
		_total_time += time;
		while(_total_time > 0.0)
		{
			QuickStepper::step(_step_size);
			_total_time -= _step_size;
		}
	}
}

ForwardFixedQuickStepper::~ForwardFixedQuickStepper()
{
}

ExactVariableQuickStepper::ExactVariableQuickStepper(Real step_size):QuickStepper(step_size)
{
}

void ExactVariableQuickStepper::step(Real time)
{
	if(!_paused)
	{
		// Steps in fixed time slices first...
		_total_time += time;
		while(_total_time >= _step_size)
		{
			QuickStepper::step(_step_size);
			_total_time -= _step_size;
		}
		// Then do a little clean-up step
		if(_total_time > 0.0)
		{
			QuickStepper::step(_total_time);
			_total_time = 0.0;
		}
	}
}

ExactVariableQuickStepper::~ExactVariableQuickStepper()
{
}
