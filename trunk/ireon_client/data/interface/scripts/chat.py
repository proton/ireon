# -*- coding: utf-8 -*-
import string
#import Interface
#import Event
#import App
#import World
#import Client
#import chat             #Chat manager
#import cegui
#from Interface import *
#from Event import *
#from App import *
#from World import *
#from Client import *
import cegui
from cegui import *
import IreonClient
from IreonClient import *
##########################################################
#####Global variables
#I = Client.CClientInterface.instance() #Interface manager
#A = App.CClientApp.instance()       #Application
#E = Event.CEventManager.instance()     #Event manager
#W = Client.CClientWorld.instance()
I = IreonClient.CClientInterface.instance() #Interface manager
A = IreonClient.CClientApp.instance()       #Application
E = IreonClient.CEventManager.instance()     #Event manager
W = IreonClient.CClientWorld.instance()
#MainChatWindow = I.getWindow('ChatWindow/MainWindow')                  #Main chat window
#ChannelWindow = I.getWindow('ChatWindow/Channel',WinType.STATIC_TEXT)  #Channel window
#EditWindow = I.getWindow('ChatWindow/Edit')   #Edit window

#chatRef = cegui.WindowManager.getSingleton().loadWindowLayout("Chat.layout")      
#MainChatWindow = cegui.WindowManager.getSingleton().getWindow("ChatWindow/MainWindow")  #Main chat window
#ChannelWindow  = cegui.WindowManager.getSingleton().getWindow("ChatWindow/Channel")      #Channel window
#EditWindow     = cegui.WindowManager.getSingleton().getWindow("ChatWindow/Edit")            #Main chat window

###########################################################
#####Channel class
class CChannel:
    m_messages = ''
    m_name = ''
 
    def addMessage(self,nick,msg):
        self.m_messages += '<' + nick + '> ' + msg + '\n'
        while len(self.m_messages) > 2000 and (string.find(self.m_messages,'\n') != -1):
            self.m_messages = self.m_messages.split('\n',1)[1]
        if Chat.m_curChannel == self.m_name:
            self.show()
 
    def show(self):
        scroll = I.getWindow(ChannelWindow.name + '__auto_vscrollbar__',WinType.SCROLL_BAR)
        atEnd = scroll.atEnd()
        ChannelWindow.text = self.m_messages
        if scroll != None and atEnd:
            scroll.toEnd()
#####Chat class
class CChat:
    m_channels = {}
    m_editActive = False;
    
    def addChannel(self,name):
        self.m_channels[name] = CChannel()
        self.m_channels[name].m_name = name
        self.m_channels[name].m_messages = ''
    
    def hideEdit(self):
        self.m_editActive = False
        I.unlinkWindow(EditWindow)
        height = ChannelWindow.UHeight
        height.y = -5
        ChannelWindow.UHeight = height
    
    def showEdit(self):
        self.m_editActive = True
        I.linkWindow(EditWindow,MainChatWindow)
        height = ChannelWindow.UHeight
        height.y = -27
        ChannelWindow.UHeight = height
        EditWindow.activate()
        self.toEndChan()
 
    def toEndChan(self):
        scroll = I.getWindow(ChannelWindow.name + '__auto_vscrollbar__',WinType.SCROLL_BAR)
        scroll.toEnd()
########### Global chat variable
Chat = CChat()
#################################
#####Module functions 
def initChat():
    evt = WindowEvent()
    evt = WindowKeyEvent()
    evt.handler = 'chatEditKeyHandler'
    I.getWindow('ChatWindow/Edit').subscribeEvent(Window.EventKeyDown,evt)
    evt = WindowEvent()
    evt.handler = 'chatSizedHandler'
    ChannelWindow.subscribeEvent(Window.EventSized,evt)
    Chat.addChannel('')
    Chat.m_curChannel = ''
    E.addKeyPressedEvent(KeyCode.KC_RETURN,'chatEnterKeyHandler')
    E.addEvent(EventID.CHAT,'chatCommandHandler')

def showChat():
    I.linkWindow(I.getWindow('ChatWindow'))
    I.linkWindow(MainChatWindow,I.findWindow('ChatWindow'))
    I.linkWindow(EditWindow,MainChatWindow)
    Chat.hideEdit()

def hideChat():
    pass
###########HANDLERS
##### Void handler, just return false
def clearSelectionHandler(arg):
    I.getWindow(arg.window,WinType.LIST).clearSelection()
##### Pressing keys on chat edit box handler
def chatEditKeyHandler(arg):
    if arg.key == KeyCode.KC_RETURN:
        text = EditWindow.text
    if text == '':
        Chat.hideEdit()
    else:
        Chat.m_channels[Chat.m_curChannel].addMessage(W.ownChar.name,text)
        A.worldConn().sendChatCmd(ChatCommand.MSG,'',text,'')
        EditWindow.text = ''
##### Handler of Return key
def chatEnterKeyHandler(arg):
    if Chat.m_editActive == True:
        arg = WindowKeyEventArg()
        arg.key = KeyCode.KC_RETURN
        EditWindow.activate()
        chatEditKeyHandler(arg)
    else:
        Chat.showEdit()  
##### Sizing chat window handler
def chatSizedHandler(arg):
    Chat.toEndChan() 
##########################################################
#             CHAT COMMAND HANDLER                       #
##########################################################
def chatCommandHandler(arg):
    print 'chat command: (' + arg.arg1 + ')(' + arg.arg2 + ')('+ arg.arg3 + ')\n'
    if arg.cmd == ChatCommand.MSG:
        if Chat.m_channels.has_key(arg.arg1) == False:
            Chat.addChannel(arg.arg1)
    Chat.m_channels[arg.arg1].addMessage(arg.arg2,arg.arg3)
   
