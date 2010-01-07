# -*- coding: utf-8 -*-
import sys
import site
site.execsitecustomize()

sys.path.append('../data/interface/scripts')
sys.path.append('../data/interface/scripts/libs')
import string
#import Interface
#import Event
#import App
#import Client

from chat import *             #Chat manager
#from Interface import *
#from Event import *
#from App import *
#from Client import *

from copy import copy
from cmd import Cmd 
import cegui
from cegui import *
import IreonClient
from IreonClient import *

import config
        

################################################################################
##### Global vars exported from C++
#I = Client.CClientInterface.instance() #Interface manager
#A = App.CClientApp.instance()       #Application
#E = Event.CEventManager.instance()     #Event manager
I = IreonClient.CClientInterface.instance() #Interface manager
A = IreonClient.CClientApp.instance()       #Application
E = IreonClient.CEventManager.instance()     #Event manager
#GS = IreonClient.CClientInterface.instance().getGUISystemPtr()
#GW = IreonClient.CClientInterface.instance().getGUIWinManagerPtr()
GS = cegui.System.getSingleton()
GW = cegui.WindowManager.getSingleton()
RW = IreonClient.CClientInterface.instance().getRootWindow()
################################################################################


def initEvents():
    # dirt hack :(
    FrameEventArg.registerSwigType(0)
    CharacterEventArg.registerSwigType(0)
    CharUpdateEventArg.registerSwigType(0)
    AppChangeStateEventArg.registerSwigType(0)
    CharCreatedEventArg.registerSwigType(0)
    CharRemovedEventArg.registerSwigType(0)
    CharAddListArg.registerSwigType(0)
    CharSelectAckArg.registerSwigType(0)
    CharSelectAckFailedArg.registerSwigType(0)
    CharSelectArg.registerSwigType(0)
    CharSelectFailedArg.registerSwigType(0)
    ConnectFailedEventArg.registerSwigType(0)
    CheckVersionFailedEventArg.registerSwigType(0)
    ChatEventArg.registerSwigType(0)

def initGUI():
    global background
    # load an image to use as a background
    cegui.ImagesetManager.getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga")
    # here we will use a StaticImage as the root, then we can use it to place a background image
    background = cegui.WindowManager.getSingleton().createWindow("TaharezLook/StaticImage", "background_wnd")
    # set position and size
    background.Position = cegui.UVector2(cegui.UDim(0,0), cegui.UDim(0,0))
    background.Size = cegui.UVector2(cegui.UDim(1,0), cegui.UDim(1,0) )
    # disable frame and standard background
    background.setProperty("FrameEnabled", "false")
    background.setProperty("BackgroundEnabled", "false")
    # set the background image
    background.setProperty("Image", "set:BackgroundImage image:full_image")
    # install this as the root GUI sheet
    global RW
    RW.addChildWindow(background)
    RW = background
    #background.activate() 
    
initEvents()
initGUI()

################################################################################
##### Exported others files

      
execfile ('../data/interface/scripts/generator.py')
execfile ('../data/interface/scripts/utils.py')
execfile ('../data/interface/scripts/windows.py')
execfile ('../data/interface/scripts/console.py')
execfile ('../data/interface/scripts/class_state.py')
execfile ('../data/interface/scripts/fsm_init.py')

################################################################################
# Load config from XML
cfg.load()

################################################################################

currentState = startState

################################################################################
############################### STATE MASHINE ##################################
################################################################################

################################################################################
def changeAppStateHandler(arg):
    print 'CLIENT send new_state: ',arg.m_newState  
    global currentState
    if currentState.name == arg.m_newState:
        return
    while True:
        newState = currentState.findState(arg.m_newState)
        print 'Python newState: ',newState 
        if newState != -1:    
            currentState = currentState.states[newState]
            currentState.enter()
            if newState == arg.m_newState:
                break   
        else:
            currentState.exit()
            currentState = currentState.parent
             
################################################################################

 
##### Subscribing handlers

#import rpdb2
#rpdb2.start_embedded_debugger("111", True, True, 300, True) 


E.addEvent(ON_APP_CHANGES_STATE,'changeAppStateHandler')
currentState.enter()
 


