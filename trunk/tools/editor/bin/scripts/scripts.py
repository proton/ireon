import sys
sys.path.append('./scripts')
sys.path.append('./scripts/libs')
import string
import Interface
import Event
import App
import Editor
from Interface import *
from Event import *
from App import *
from Editor import *
from World import *
##########################################################
#####Global variables
I = Editor.getInterface() #Interface manager
A = App.get()       #Application
E = Event.get()     #Event manager
##########################################################
#####Counter, returns new value every time
def counter():
 static_counter = 0
 while True:
  static_counter += 1
  yield static_counter
count = counter().next
#########################################################################
#####Function creates simple message box with specified title and name
#####Message box has one button - ok and is destroyed by interface manager
#####after pressing this button.
#####Every new popup window has unique id.
def createPopupWindow(title,text):
 name = 'PopupWindow' + str(count())
 w = I.createWindow('TaharezLook/FrameWindow',name,WinType.FRAME)
 w.text = title
 w.size = Vector2(0.45,0.4)
 w.pos = Vector2(0.3,0.25)
 w.setCloseButtonEnabled(False)
 w.setAlwaysOnTop(True)
 c = I.createWindow('TaharezLook/StaticText',name + '/Text',WinType.STATIC_TEXT)
 c.size = Vector2(0.9,0.65)
 c.pos = Vector2(0.05,0.2)
 c.text = text
 c.setFormatting(HorzFormatting.WordWrapCentered,VertFormatting.VertCentered)
 c.setBackgroundEnabled(False)
 c.setFrameEnabled(False)
 I.linkWindow(c,w)
 c = I.createWindow('TaharezLook/Button',name + '/Button')
 c.size = Vector2(0.3,0.1)
 c.pos = Vector2(0.35,0.87)
 c.text = 'Ага'
 evt = WindowEvent()
 evt.window = w.name
 evt.handler = 'popupWindowOKHandler'
 c.subscribeEvent(PushButton.EventClicked,evt)
 I.linkWindow(c,w)
 return w
def updateRootCharactersList():
 w = I.getWindow('RootMenu/CharactersList/List',WinType.MULTI_LIST)
 if w.rows != len(A.characters):
  w.reset()
  for x in A.characters:
   arr = StringVector()
   arr.append(str(x.id))
   arr.append(x.name)
   w.insertRow( arr )
 if w.getFirstSelected() == -1:
  I.getWindow('RootMenu/CharactersList/RemoveCharacterButton').disable()
  I.getWindow('RootMenu/CharactersList/SelectCharacterButton').disable()
 else:
  I.getWindow('RootMenu/CharactersList/RemoveCharacterButton').enable()
  I.getWindow('RootMenu/CharactersList/SelectCharacterButton').enable()
##########################################################
#                   WINDOW HANDLERS                      #
##########################################################
#####Handler of OK button of popup window. Destroys window
def popupWindowOKHandler(arg):
 w = I.getWindow(arg.window)
 I.destroyWindow(w)
 I.enableWindows()
#####Just close window
def closeWindowHandler(arg):
 I.unlinkWindow(I.findWindow(arg.window))
###########################################################
#              ASSIGN WINDOW HANDLERS                     #
###########################################################
##### Create event handlers for main menu
def createMainMenu():
 pass
##########################################################
def createInfo():
 w = I.createWindow('TaharezLook/CharInfo','InfoWindow',WinType.STATIC_TEXT)
 w.UWidth = Vector2(0.3,0)
 w.UHeight = Vector2(0.2,0)
 w.pos = Vector2(0,0)
 I.linkWindow(w)
def switchLayoutHandler(arg):
 if I.isKeyPressed(KeyCode.KC_RCONTROL) or I.isKeyPressed(KeyCode.KC_LCONTROL):
  I.switchLayout()
###########################################################
#                 DIFFERENT EVENT HANDLERS                #
###########################################################
##########################################################
def infoHandler(arg):
 if I.getWindow('InfoWindow').visible:
  I.getWindow('InfoWindow').visible = False
 else:
  I.getWindow('InfoWindow').visible = True
def frameHandler(arg):
 text = 'FPS: ' + str(A.getFPS()) + '\n' + 'Triangle: ' + str(A.getTriCount())
 I.getWindow('InfoWindow').text = text
###########################################################
#                   SETUP HANDLERS                        #
###########################################################
createMainMenu()
##### Subscribing handlers 
E.addEvent(CommonEventID.FRAME,'frameHandler') 
E.addKeyPressedEvent(KeyCode.KC_LSHIFT,'switchLayoutHandler') 
E.addKeyPressedEvent(KeyCode.KC_RSHIFT,'switchLayoutHandler') 
E.addKeyPressedEvent(KeyCode.KC_I,'infoHandler')
createInfo()
