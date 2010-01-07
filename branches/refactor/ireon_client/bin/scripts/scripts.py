import sys
sys.path.append('./scripts')
sys.path.append('./scripts/libs')
import string
import Interface
import Event
import App
from chat import *             #Chat manager
from Interface import *
from Event import *
from App import *

import feedparser

###########################################################
######Global variables
I = Interface.get() #Interface manager
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
 I.enableWindows()
##### Command for button 'Connect' (main menu)
def connectHandler(arg):
 name = arg.window
 editText = I.getWindow(name+'/HostEdit').text
 if editText == '':
  p = createPopupWindow('','Сервер не указан')
  I.linkWindow(p)
  return
 A.setSetting('/config/rootServer/host',editText)

 editText = I.getWindow(name+'/PortEdit').text
 if editText == '':
  p = createPopupWindow('','Порт не указан')
  I.linkWindow(p)
  I.disableWindows(p)
  return
 A.setSetting('/config/rootServer/port',editText)

 if I.getWindow(name+'/CheckBox',WinType.CHECK_BOX).checked:
  A.setSetting('/temp/rootConnection/createAccount','1')
  print 'checked'

 text = I.getWindow(name+'/LoginEdit').text
 if text == '':
  I.linkWindow(createPopupWindow('Подключение','Логин не указан'))
  return
 A.setSetting('/config/profile/login',text)

 text = I.getWindow(name+'/PasswordEdit').text
 if text == '':
  I.linkWindow(createPopupWindow('Подключение','Пароль не указан'))
  return
 A.setSetting('/config/profile/password',text)
 A.connect()
##### Cancel connection (connection popup widnow)
def cancelConnectionHandler(arg):
 A.cancelConnect()
 I.unlinkWindow(I.findWindow(arg.window))
##### Add character button (root menu)
def rootAddCharacterHandler(arg):
 w = I.getWindow('AddCharacterWindow')
 I.linkWindow(w,I.getWindow('RootMenu'))
##### Add button (add character widnow)
def addCharacterHandler(arg):
 name = I.getWindow(arg.window + '/NameEdit').text
 if name == '':
  w = createPopupWindow('Не так','Не указано имя')
  I.linkWindow(w)
  I.disableWindows(w)
  return
 d = ClientOwnCharData()
 d.name = name
 A.createChar(d)
 I.unlinkWindow(I.findWindow(arg.window))
##### Remove char
def rootRemoveCharacterHandler(arg):
 w = I.getWindow(arg.window + '/List',WinType.MULTI_LIST)
 id = w.getValue(w.getFirstSelected(),0)
 id = int(id)
 A.removeChar(id)
##### Select char button
def rootSelectCharacterHandler(arg):
 w = I.getWindow(arg.window + '/List',WinType.MULTI_LIST)
 id = w.getValue(w.getFirstSelected(),0)
 id = int(id)
 A.selectChar(id)
##### Selected char in char list
def charListSelectionHandler(arg):
 E.execute(EventID.ROOT_MENU_UPDATED,None)
###########################################################
#              ASSIGN WINDOW HANDLERS                     #
###########################################################
##### Create event handlers for main menu
def createMainMenu():
 w = I.getWindow('MainMenu/ConnectWindow/Button')
 evt = WindowEvent()
 evt.window = 'MainMenu/ConnectWindow'
 evt.handler = 'connectHandler'
 w.subscribeEvent(PushButton.EventClicked,evt)
 w = I.getWindow('MainMenu/ConnectWindow/HostEdit')
 w.text = A.getSetting('/config/rootServer/host')
 w = I.getWindow('MainMenu/ConnectWindow/PortEdit')
 w.text = A.getSetting('/config/rootServer/port')
 w = I.getWindow('MainMenu/ConnectWindow/LoginEdit')
 w.text = A.getSetting('/config/profile/login')
 w = I.getWindow('MainMenu/ConnectWindow/PasswordEdit')
 w.text = A.getSetting('/config/profile/password')
 w = I.getWindow('MainMenu/ConnectWindow/CheckBox',WinType.CHECK_BOX)
 w.checked = False
# feed = feedparser.parse("http://times.ireon.org/feed/")
# entry = feed.entries[1]
# a = entry.content[0].value.encode('utf-8')
# w = I.getWindow('MainMenu/NewsWindow/NewsText')
# w.text = a
 return I.getWindow('MainMenu')
def createRootMenu():
 evt = WindowEvent()
 evt.window = 'RootMenu/CharactersList'
 evt.handler = 'rootAddCharacterHandler'
 I.getWindow('RootMenu/CharactersList/AddCharacterButton').subscribeEvent(PushButton.EventClicked,evt)
 evt.handler = 'rootRemoveCharacterHandler'
 I.getWindow('RootMenu/CharactersList/RemoveCharacterButton').subscribeEvent(PushButton.EventClicked,evt)
 evt.handler = 'rootSelectCharacterHandler'
 I.getWindow('RootMenu/CharactersList/SelectCharacterButton').subscribeEvent(PushButton.EventClicked,evt)
 evt.window = 'AddCharacterWindow'
 evt.handler = 'addCharacterHandler'
 I.getWindow('AddCharacterWindow/OK').subscribeEvent(PushButton.EventClicked,evt)
 evt.handler = 'closeWindowHandler'
 I.getWindow('AddCharacterWindow/Cancel').subscribeEvent(PushButton.EventClicked,evt)
 evt.window = 'RootMenu/AddCharactersList'
 evt.handler = 'charListSelectionHandler'
 w = I.getWindow('RootMenu/CharactersList/List',WinType.MULTI_LIST)
 w.subscribeEvent(MultiList.EventSelectionChanged,evt)
 w.cols = 2
 arr = ByteVector()
 arr.append(10)
 arr.append(88)
 w.setWidth( arr )
 w.setHead(0,'ID')
 w.setHead(1,'Name')
def createPopups():
 w = I.getWindow('LoggingPopupWindow/Text',WinType.STATIC_TEXT)
 w.setFormatting(HorzFormatting.WordWrapCentered,VertFormatting.VertCentered)
 w = I.getWindow('ConnectionPopupWindow/Text',WinType.STATIC_TEXT)
 w.setFormatting(HorzFormatting.WordWrapCentered,VertFormatting.VertCentered)
 evt = WindowEvent()
 evt.window = 'ConnectionPopupWindow'
 evt.handler = 'cancelConnectionHandler'
 I.getWindow('ConnectionPopupWindow/Button').subscribeEvent(PushButton.EventClicked,evt)
###########################################################
#                 DIFFERENT EVENT HANDLERS                #
###########################################################
##### Change state handler
def changeAppStateHandler(arg):
 print arg.new
 if arg.new == State.MENU:
   I.linkWindow(I.getWindow('MainMenu'))
   A.rootConn().disconnect()
   A.worldConn().disconnect()
 elif arg.old == State.MENU:
   I.unlinkWindow(I.findWindow('MainMenu'))
 if arg.new == State.CONNECTING_ROOT:
  port = A.rootConn().port
  serv = A.rootConn().host
  text = 'Connecting to root server (' + serv + ':' + str(port) + ')...'
  w = I.getWindow('ConnectionPopupWindow/Text')
  w.text = text
  w = I.getWindow('ConnectionPopupWindow');
  w.text = 'Connecting'
  I.linkWindow(w)
 elif arg.old == State.CONNECTING_ROOT:
  I.unlinkWindow(I.findWindow('ConnectionPopupWindow'))
 if arg.new == State.CONNECTING_WORLD:
  port = A.worldConn().port
  serv = A.worldConn().host
  text = 'Connecting to world server (' + serv + ':' + str(port) + ')...'
  w = I.getWindow('ConnectionPopupWindow/Text')
  w.text = text
  w = I.getWindow('ConnectionPopupWindow');
  w.text = 'Connecting'
  I.linkWindow(w)
 elif arg.old == State.CONNECTING_WORLD:
  I.unlinkWindow(I.findWindow('ConnectionPopupWindow'))
 if arg.new == State.LOGGING_ROOT:
  I.linkWindow(I.getWindow('LoggingPopupWindow'))
 elif arg.old == State.LOGGING_ROOT:
  I.unlinkWindow(I.findWindow('LoggingPopupWindow'))
 if arg.new == State.LOGGING_WORLD:
  I.linkWindow(I.getWindow('LoggingPopupWindow'))
 elif arg.old == State.LOGGING_WORLD:
  I.unlinkWindow(I.findWindow('LoggingPopupWindow'))
 if arg.new == State.ROOT:
  A.worldConn().disconnect()
  I.linkWindow(I.getWindow('RootMenu'))
 elif arg.old == State.ROOT:
   I.unlinkWindow(I.findWindow('RootMenu'))
 if arg.new == State.PLAY:
  showChat()
 elif arg.old == State.PLAY:
  hideChat()
#########################################################
def changeCharListHandler(arg):
 updateRootCharactersList()
#########################################################
def rootMenuUpdatedHandler(arg):
 updateRootCharactersList()
#########################################################
def charCreatedHandler(arg):
 if arg.res:
  I.linkWindow(createPopupWindow('Char creation','Error creating character'))
#########################################################
def charRemovedHandler(arg):
 if arg.res != 1:
  I.linkWindow(createPopupWindow('Удаление персонажа','Удаление персонажа не удалось'))
 else:
  for i, x in enumerate(A.characters):
   if x.id == arg.id:
    del A.characters[i]
  E.execute(CHAR_LIST_CHANGED,None)
#########################################################
def loginFailedHandler(arg):
 I.linkWindow(createPopupWindow('Авторизация','Не удалось авторизироваться на сервере'))
#########################################################
def connectFailedHandler(arg):
 I.unlinkWindow(I.findWindow('ConnectionPopupWindow'))
 I.linkWindow(createPopupWindow('Ошибка соединения','Не удалось соединиться с ' + arg.host + ':' + str(arg.port) + '\n' + arg.error))
 if A.state == State.CONNECTING_ROOT:
  A.state = State.MENU
 if A.state == State.CONNECTING_WORLD:
  A.state = State.ROOT
##########################################################
def charToWorldHandler(arg):
 if arg.ch.type != ObjType.PLAYER and arg.ch.type != ObjType.MOB:
  return
 name = arg.ch.name
 w = I.createWindow('DefaultGUISheet',name  + '_infoWindow')
 i = I.createWindow('TaharezLook/CharInfo',name + '_infoWindow/Text')
 I.linkWindow(i,w)
 I.linkWindow(w)
 i.text = name
 width = Vector2(0,I.getTextExtent(name) + 10)
 height = Vector2(0,I.getFontHeight() + 10)
 i.UWidth = width
 i.UHeight = height
 w.UWidth = width
 w.UHeight = height
 w.disable()
 arg.ch.info = w
##########################################################
def charInfoUpdateHandler(arg):
 if arg.ch.info == None:
  return
 if arg.ch.visible == False:
  arg.ch.info.visible = False
  return
 arg.ch.info.visible = True
 pos = arg.ch.position + Vector3(0,2.1,0)
 pos2 = W.translateTo2d(pos)
 if pos2.x < 0 or pos2.y < 0 or pos2.x > 1 or pos2.y > 1:
  arg.ch.info.visible = False
  return
 arg.ch.info.UX = Vector2(pos2.x,-arg.ch.info.UWidth.y/2)
 arg.ch.info.UY = Vector2(pos2.y,-arg.ch.info.UHeight.y)
##########################################################
def switchLayoutHandler(arg):
 if I.isKeyPressed(KeyCode.KC_RCONTROL) or I.isKeyPressed(KeyCode.KC_LCONTROL):
  I.switchLayout()
##########################################################
def switchFirstPersonHandler(arg):
 if A.state == State.PLAY:
  if I.firstPerson == True:
   I.firstPerson = False
   I.showCursor = True
  else:
   I.firstPerson = True
   I.showCursor = False
###########################################################
#                   SETUP HANDLERS                        #
###########################################################
createMainMenu()
createRootMenu()
createPopups()

##### Subscribing handlers
E.addEvent(EventID.APP_CHANGES_STATE,'changeAppStateHandler')
E.addEvent(EventID.CHAR_LIST_CHANGED,'changeCharListHandler')
E.addEvent(EventID.ROOT_MENU_UPDATED,'rootMenuUpdatedHandler')
E.addEvent(EventID.CHAR_CREATED,'charCreatedHandler')
E.addEvent(EventID.CHAR_REMOVED,'charRemovedHandler')
E.addEvent(EventID.LOGIN_FAILED,'loginFailedHandler')
E.addEvent(EventID.CONNECT_FAILED,'connectFailedHandler')
E.addEvent(EventID.CHAR_TO_WORLD,'charToWorldHandler')
E.addEvent(EventID.CHAR_INFO_UPDATE,'charInfoUpdateHandler')
E.addKeyPressedEvent(KeyCode.KC_LSHIFT,'switchLayoutHandler')
E.addKeyPressedEvent(KeyCode.KC_RSHIFT,'switchLayoutHandler')
E.addKeyPressedEvent(KeyCode.KC_TAB,'switchFirstPersonHandler')
initChat()


