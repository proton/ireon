# -*- coding: utf-8 -*-
################################################################################
############################    States     #####################################
################################################################################
class GenericState:                   
    def __init__(self, name):
        self.name = name
        self.childs = copy(self.childs)  
        self.states = copy(self.states)
        self.parent = self 
    childs = {}                #reference on active childs created or added in root
    states = {}
    name = None
    parent = None
    def enter(self, name):
        pass
    def exit (self):
        pass 
    def __getitem__(self, key):
        return self.childs[key]
    def __setitem__(self, key, value):
        self.childs[key] = value
    
    def registerState(self, state):
            self.states[state.name] = state
            state.parent = self
    def findState(self, name):
        
        stateFinded = -1
        
        if self.states.has_key(name) :
            stateFinded = name
           
        else:
            for i in self.states:
                if self.states[i].findState(name) != -1:
                    stateFinded = i
                      
        return stateFinded
        
##############################      ERROR       ################################
class EmptyState(GenericState):
    def enter(self):
        pass
    def exit(self):
        pass



################################################################################  
################################################################################ 
################################################################################  
################################################################################ 


class StartState(GenericState):
    def enter(self):
         #initChat()
        handlers.addKeyPressedEvent(KC_F1, self.consoleKeyHandler) 
        self['Console'] = CConsole().create()
    def exit(self):
        A.shutdown()
        ##### Handler of ~ key
    def consoleKeyHandler(self):
        
        if self['Console'].m_active == True:
            self['Console'].m_active = False
            self['Console'].hide()
            #I.enableControl(True)
        else:
            self['Console'].m_active = True
            self['Console'].show()
            self['Console']['ConsoleWindow']['Edit'].content.enable()
            #I.enableControl(False)
            
################################################################################  
################################################################################             
##############################   LOCAL STATE  ##################################    
class LocalState(GenericState):
    def enter(self):
        pass    
                       
    def exit(self):        
        pass
        
        
###########################   MENU STATE  #########################        
class MenuState(GenericState):
    already = 0
    def enter(self):
        
        A.disconnect()
        
        self['Version'] = CharInfo().create('Version')
        version = ''
        for i in A.getVersion():
            version += str(i)+'.'
        
        self['Version'].content.Position = cegui.UVector2(UDim(0.90,0), UDim(0.93,0))
        self['Version'].content.Size  = cegui.UVector2(UDim(0.09,0), UDim(0.05,0) )
        self['Version'].content.Text = version
        #// aarrggbb in Hexadecimal
        #l: top left,  tr: top right,  bl: bottom left,  br: bottom right
        self['Version'].content.setProperty("FrameColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000")
               
        # TODO: need analog
        #self['Version'].content.setTextColour( Colour(0.33,0.33,0.33) )
        self['MainMenu'] = MainMenu().create()
        
        
        if self.already == 0:
            self.already = 1
            handler = exportToMainDict(self.connectHandler) 
            self['MainMenu']['ConnectWindow']['ButtonOK'].content.subscribeScriptedEvent("MouseClick",handler)
            handler = exportToMainDict(self.createExitWindowHandler) 
            a = self['MainMenu']['ConnectWindow']['ButtonCancel'].content.subscribeScriptedEvent("MouseClick",handler)
        
        handlers.addKeyPressedEvent(KC_ESCAPE,self.createExitWindowHandler)
        self['Version'].show()
        self['MainMenu'].show()
    def exit(self):
        pass
        self['Version'].hide()
        self['Version'].destroy()
        self['MainMenu'].hide()
        self['MainMenu'].destroy()
        handlers.removeEvent(ON_CONN_FAILED, 'connectFailedHandler')
        handlers.removeEvent(ON_LOGIN_FAILED,'loginFailedHandler')
        handlers.removeKeyPressedEvent(KC_ESCAPE,'createExitWindowHandler')
        
    #--------------------------------------------------
    # Command for button 'Connect' (main menu)
    def connectHandler(self, arg):
        
        # names element, to need checked on fill
        nameEditElement = ["MainMenu/ConnectWindow/HostEdit", \
                            "MainMenu/ConnectWindow/LoginEdit", \
                            "MainMenu/ConnectWindow/PasswordEdit" ]
        # oops, it's edit elements not filled. may be we must create 
        # PopupWindow with this messages? don't know, don't know :)
        msgError = [['','Сервер не указан'],            \
                    ['Подключение','Логин не указан'],  \
                    ['Подключение','Пароль не указан']]
        # stupidness ! refactor this 
        settings = ['cfg.root.getNet().getRootserver().setIp', \
                    'cfg.root.getProfile().setLogin',   \
                    'cfg.root.getProfile().setPassword']
        
        if execMap(checkEditText, nameEditElement, msgError, settings) == False:
            return
    
        create = self['MainMenu']['ConnectWindow']['CheckBox'].content.Selected
        cfg.root.getProfile().setRegistration(create)
        print 'Registration Account : ', create
        
        cfg.save()
        
        A.startRSConnection()
        
    def createExitWindowHandler(self, arg):
        
        a = MenuExitWindow().create('Exit', "Выйти из игры?")
        a.show()
        
        
###########################  CONNECTING ROOT STATE  #########################    
class ConnectingRootState(GenericState):
    already = 0
    def enter(self):
        self['ConnectionPopupWindow'] = ConnectionPopupWindow().create()
        port = 'localhost'
        serv = '29000' #A.rootConn().host
        text = 'Connecting to root server (' + serv + ':' + str(port) + ')...'
        self['ConnectionPopupWindow']['Text'].content.Text = text
        if self.already == 0:
            self.already =1
            handler =  exportToMainDict(self.cancelConnectionHandler)
            self['ConnectionPopupWindow']['Button'].content.subscribeScriptedEvent("MouseClick",handler)
        self['ConnectionPopupWindow'].show()
        handlers.addEvent(ON_CONN_FAILED, self.connectFailedHandler)
    def exit(self):
        self['ConnectionPopupWindow'].hide()
        self['ConnectionPopupWindow'].destroy()
        
    def connectFailedHandler(self, arg):
        self['ConnectionPopupWindow'].hide()
        popup = PopupWindow().create(title = 'Ошибка соединения', text = 'Не удалось соединиться с '+ arg.host + ':' + str(arg.port) + '\n' + arg.error)
        popup.show()
        #A.state = State.MENU
        
    ##### Cancel connection (connection popup widnow)
    def cancelConnectionHandler(self, arg):
        A.disconnect()
        self['ConnectionPopupWindow'].hide()
        #A.State = State.Menu   
###########################  CONNECTING ROOT STATE  #########################    
class CheckingVersionState(GenericState):
    already = 0
    def enter(self):
        self['ConnectionPopupWindow'] = ConnectionPopupWindow().create()
        
        text = 'Checking version...'
        self['ConnectionPopupWindow']['Text'].content.Text = text
        
    def exit(self):
        self['ConnectionPopupWindow'].hide()
        self['ConnectionPopupWindow'].destroy()
        

###########################  LOGGING ROOT STATE  #########################           
class LoggingRootState(GenericState):
    def enter(self):
        self['LoggingPopupWindow'] = LoggingPopupWindow().create()
        self['LoggingPopupWindow'].show()
        handlers.addEvent(ON_LOGIN_FAILED,self.loginFailedHandler)
    def exit(self):
        
        self['LoggingPopupWindow'].hide()
        self['LoggingPopupWindow'].destroy
        
    #---------------------------------------------
    def loginFailedHandler(self, arg):
        popup = PopupWindow().create(title = "Авторизация", text = "Не удалось авторизироваться на сервере")
        popup.show()    
        
        
        
        
        
################################################################################  
################################################################################                 
###########################   ROOT SERVER STATE  #########################        
        
class RootServerState(GenericState):
    def enter(self):
        pass
        
    def exit(self): 
        pass
         
###########################  ROOT STATE  #########################        
class RootState(GenericState):
    already = 0
    def enter(self):
        #A.worldConn().disconnect()
        handlers.removeEvent(ON_CONN_FAILED, 'connectFailedHandler')
        handlers.removeEvent(ON_LOGIN_FAILED,'loginFailedHandler')
        
        self['RootMenu'] = RootMenu().create()
        self['AddCharacterWindow'] = AddCharacterWindow().create()
        
        
        if self.already == 0:
            self.already =1
            
            handler = exportToMainDict(self.rootAddCharacterHandler) 
            self['RootMenu']['CharactersList']['AddCharacterButton'].content.subscribeScriptedEvent("MouseClick",handler)
        
            handler = exportToMainDict(self.charListSelectionHandler)
            self['RootMenu']['CharactersList']['List'].content.subscribeScriptedEvent("SelectionChanged",handler)
       
            handler = exportToMainDict(self.rootRemoveCharacterHandler) 
            self['RootMenu']['CharactersList']['RemoveCharacterButton'].content.subscribeScriptedEvent("MouseClick",handler)
                
            handler = exportToMainDict(self.rootSelectCharacterHandler)
            self['RootMenu']['CharactersList']['SelectCharacterButton'].content.subscribeScriptedEvent("MouseClick",handler)
               
            handler = exportToMainDict(self.rootExitWindowHandler)
            self['RootMenu']['CharactersList']['ExitButton'].content.subscribeScriptedEvent("MouseClick",handler)
       
        
            handler = exportToMainDict(self.addCharacterHandler) 
            self['AddCharacterWindow']['OK'].content.subscribeScriptedEvent("MouseClick",handler)
        
            handler = exportToMainDict(self.closeWindowHandler) 
            self['AddCharacterWindow']['Cancel'].content.subscribeScriptedEvent("MouseClick",handler)
        
        handlers.addEvent(ON_CHAR_LIST_CHANGED,self.changeCharListHandler)
        handlers.addEvent(ON_ROOT_MENU_UPDATED,self.rootMenuUpdatedHandler)
        handlers.addEvent(ON_CHAR_CREATED,self.charCreatedHandler)
        handlers.addEvent(ON_CHAR_ADD_LIST,self.charAddListHandler)
        handlers.addEvent(ON_CHAR_REMOVED,self.charRemovedHandler)
        
        handlers.addKeyPressedEvent(KC_ESCAPE,self.rootExitWindowHandler)
        #A.worldConn().disconnect()
        self['RootMenu'].show()
        w = self['RootMenu']['CharactersList']['List'].content
        w.SelectionMode = cegui.MultiColumnList.RowSingle
        self.refreshListChar()
  
    def exit(self):
        handlers.removeEvent(ON_CHAR_LIST_CHANGED,'changeCharListHandler')
        handlers.removeEvent(ON_ROOT_MENU_UPDATED,'rootMenuUpdatedHandler')
        handlers.removeEvent(ON_CHAR_CREATED,'charCreatedHandler')
        handlers.removeEvent(ON_CHAR_ADD_LIST,'charAddListHandler')
        handlers.removeEvent(ON_CHAR_REMOVED,'charRemovedHandler') 
        self['RootMenu'].hide()
        self['AddCharacterWindow'].hide()
        self['RootMenu'].destroy()
        self['AddCharacterWindow'].destroy()
        handlers.removeKeyPressedEvent(KC_ESCAPE,'rootExitWindowHandler')
         
    #------------------------------------------------
    def rootExitWindowHandler(self, arg):
        a = RootExitWindow().create('Выход из RS','Выберите действие')
        a.show()
        
    def refreshListChar(self):
        w = self['RootMenu']['CharactersList']['List'].content
        w.resetList()
        A.refreshListChar()
    #------------------------------------------------
    def charAddListHandler(self, arg):
        #arr = StringVector()
        lid   = ListTextItem( str(arg.m_id)   )
        lname = ListTextItem( str(arg.m_name) )
        w = self['RootMenu']['CharactersList']['List'].content
        id = w.RowCount
        #rowNum = w.insertRow(id)
        rowNum = cegui._cegui.MultiColumnList_insertRow(w,id)
        #r1 = w.addRow(lid,   0)
        #r2 = w.addRow(lname, 1)
        w.setItem( lid, 0, rowNum);
        w.setItem( lname, 1, rowNum);
         
        #print "r1",r1
        #print "r2",r2
        
            
    def updateRootCharactersList(self):
        w = self['RootMenu']['CharactersList']['List'].content
        if w.getFirstSelectedItem() == None: # nothing selected
            if w.RowCount != 0:        # list not empty
                w.setItemSelectState(MCLGridRef(0,0), True)
                #w.setSelected(0)       # select top char
            else:                      # disable buttons
                self['RootMenu']['CharactersList']['RemoveCharacterButton'].content.disable()
                self['RootMenu']['CharactersList']['SelectCharacterButton'].content.disable()
                
        else:
                self['RootMenu']['CharactersList']['RemoveCharacterButton'].content.enable()
                self['RootMenu']['CharactersList']['SelectCharacterButton'].content.enable()
                
    def changeCharListHandler(self,arg):
        self.updateRootCharactersList()
    
    def rootMenuUpdatedHandler(self,arg):
        self.updateRootCharactersList()
    

        
    def charCreatedHandler(self, arg):
        #if arg.m_res:
        #    popup = PopupWindow().create(title = 'Char creation',text = 'Error creating character')
        #    popup.show()
        self.refreshListChar()
        
    def charRemovedHandler(self, arg):
        if 0 == 1: #arg.m_res != 1:
            popup = PopupWindow().create(title = 'Удаление персонажа', text = 'Удаление персонажа не удалось')
            popup.show()
        else:
            self.refreshListChar()
            E.execute(ON_CHAR_LIST_CHANGED,None)
    ##### Add character button (root menu)
    def rootAddCharacterHandler(self, arg):
        self['AddCharacterWindow'].parent = self['RootMenu']
        self['AddCharacterWindow'].show()
        
    ##### Add button (add character widnow)
    def addCharacterHandler(self, arg):
        # look , associated setting set to [''] , this say we , what we don't
        # must set setting ( my english is bad)
        if execMap(checkEditText, ['AddCharacterWindow/NameEdit'],           \
                                  [['Создание персонажа','Не указано имя']], \
                                  ['']) == False: 
                                             return
    
        name = str(self['AddCharacterWindow']['NameEdit'].content.Text)
        A.createChar(name)
        self['AddCharacterWindow'].hide()
                
    ##### Remove char
    def rootRemoveCharacterHandler(self, arg):
        w = self['RootMenu']['CharactersList']['List'].content
        id = w.getFirstSelectedItem().Text
        print id
        #id = w.getValue(w.getFirstSelected(),0)
        try:
            id = int(id)
        except:
            print 'Text in list item (%i, %i) dont found or not maybe converted to int' % (0,0)
        A.removeChar(id)
        
    ##### Select char button
    def rootSelectCharacterHandler(self, arg):
        w = self['RootMenu']['CharactersList']['List'].content
        #idItem = w.getItemAtGridReference(MCLGridRef(w.getFirstSelectedItem(),0))
        idItem = w.getFirstSelectedItem()
        id = idItem.Text
        print str(id)
        try:
            id = int(id)
        except:
            print 'Text in list item (%i, %i) dont found or not maybe converted to int' % (0, 0)
        A.selectChar(id)
        
    ##### Selected char in char list
    def charListSelectionHandler(self, arg):
        E.execute(ON_ROOT_MENU_UPDATED,None)

    #####Just close window
    def closeWindowHandler(self, arg):
        self['AddCharacterWindow'].hide()
        
###########################  CONNECTING WORLD STATE  #########################        
class ConnectingWorldState(GenericState):
    already = 0
    def enter(self):
        self['ConnectionPopupWindow'] = ConnectionPopupWindow().create()
        port = 'localhost'
        serv = '29000' #A.rootConn().host
        text = 'Connecting to root server (' + serv + ':' + str(port) + ')...'
        self['ConnectionPopupWindow']['Text'].content.Text = text
        if self.already == 0:
            self.already =1
            handler =  exportToMainDict(self.cancelConnectionHandler)
            self['ConnectionPopupWindow']['Button'].content.subscribeScriptedEvent("MouseClick",handler)
        self['ConnectionPopupWindow'].show()
        handlers.addEvent(ON_CONN_FAILED, self.connectFailedHandler)
     
    def exit(self):
        self['ConnectionPopupWindow'].hide()
        self['ConnectionPopupWindow'].destroy()
    
    def connectFailedHandler(self, arg):
        self['ConnectionPopupWindow'].hide()
        popup = PopupWindow().create(title = 'Ошибка соединения', text = 'Не удалось соединиться с '+ arg.host + ':' + str(arg.port) + '\n' + arg.error)
        popup.show()
        #A.state = State.MENU
        
    ##### Cancel connection (connection popup widnow)
    def cancelConnectionHandler(self, arg):
        A.disconnect()
        self['ConnectionPopupWindow'].hide()
        #A.State = State.Menu       
        
     
################################  LOGGING WORLD STATE ##########################
class LoggingWorldState(GenericState):
    def enter(self):
        self['LoggingPopupWindow'] = LoggingPopupWindow().create()
        self['LoggingPopupWindow'].show()
        handlers.addEvent(ON_LOGIN_FAILED,self.loginFailedHandler)
    def exit(self):
        
        self['LoggingPopupWindow'].hide()
        self['LoggingPopupWindow'].destroy
        
    #---------------------------------------------
    def loginFailedHandler(self, arg):
        popup = PopupWindow().create(title = "Авторизация", text = "Не удалось авторизироваться на сервере")
        popup.show()    



################################################################################  
################################################################################   
###########################   WORLD SERVER STATE  ##############################    

class WorldServerState(GenericState):
    def enter(self):
        global RW
        IreonClient.CClientInterface.instance().getRootWindow().removeChildWindow(RW)
        RW = IreonClient.CClientInterface.instance().getRootWindow()
    
        
        A.enterWorld()
        
        self['CharInfo']= CharInfo().create('InfoWindow')
                
        self['OwnCharHP1'] = StatusImage().create(name = "OwnCharHP1")
        self['OwnCharHP1'].content.setProperty("ImageColours", "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000")
        
        #self['OwnCharHP1'].content.setColour(Colour(1,0,0))
        #self['OwnCharHP1'].content.UWidth = Vector2(0.04, 0)
        #self['OwnCharHP1'].content.UHeight = Vector2(0.3, 0)
               
        self['OwnCharHP2'] = StatusImage().create(name = "OwnCharHP2")
        self['OwnCharHP1'].content.setProperty("ImageColours", "tl:FF000000 tr:FF000000 bl:FF000000 br:FF000000")
        
        #self['OwnCharHP2'].content.setColour(Colour(0,0,0))
        #self['OwnCharHP2'].content.UWidth = Vector2(0.04, 0)
        #self['OwnCharHP2'].content.UHeight = Vector2(0 , 0)
        
        handlers.addEvent(ON_CHAR_TO_WORLD,self.charToWorldHandler) 
        handlers.addEvent(ON_CHAR_INFO_UPDATE,self.charInfoUpdateHandler) 
        handlers.addEvent(ON_CHAR_DIE,self.charDieHandler) 
        handlers.addEvent(ON_FRAME,self.frameHandler) 
        handlers.addKeyPressedEvent(KC_I,self.infoHandler)
        handlers.addKeyPressedEvent(KC_ESCAPE,self.playExitWindowHandler)
        
    def exit(self):
        handlers.removeKeyPressedEvent(KC_ESCAPE,'playExitWindowHandler')
        handlers.removeEvent(ON_CHAR_TO_WORLD,'charToWorldHandler') 
        handlers.removeEvent(ON_CHAR_INFO_UPDATE,'charInfoUpdateHandler') 
        handlers.removeEvent(ON_CHAR_DIE,'charDieHandler') 
        handlers.removeEvent(ON_FRAME,'frameHandler')
        handlers.removeKeyPressedEvent(KC_I,'infoHandler') 
        self['CharInfo'].destroy() 
        self['OwnCharHP1'].destroy()
        self['OwnCharHP2'].destroy() 
    
    def playExitWindowHandler(self, arg):
        a = MenuExitWindow().create('Выход из игры','Выходим?')
        a.show()    
    #---------------------------------------------
    def charToWorldHandler(self, arg):
        print arg
        if arg.ch.type != ObjType.PLAYER and arg.ch.type != ObjType.MOB:
            return
        nameID = arg.ch.name + str(arg.ch.id)
        w = CharInfoWindow().create(name = nameID + '_infoWindow',text = arg.ch.name)
        w.show()
        arg.ch.info = w.content  
        
    def charInfoUpdateHandler(self, arg):
        print arg
        if arg.ch == None:
            return
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
        i = I.getWindow(arg.ch.info.name + '/Text',WinType.STATIC_TEXT);
        
        if W.ownChar.enemy and W.ownChar.enemy.id == arg.ch.id:
            i.setTextColour(Colour(1,0,0))
        elif I.target != None and I.target.id == arg.ch.id:
            i.setTextColour(Colour(0,1,1))
        elif I.highlight != None and I.highlight.id == arg.ch.id:
            I.getWindow(arg.ch.info.name + '/Text',WinType.STATIC_TEXT).setTextColour(Colour(0,0.5,0.5))
        else:
            i.setTextColour(Colour(1,1,1))
        I.getWindow(arg.ch.info.name + '/HP',WinType.STATIC_IMAGE).UWidth = i.UWidth * arg.ch.percentHP  / 100 - Vector2(0,6)
        arg.ch.info.UX = Vector2(pos2.x,-arg.ch.info.UWidth.y/2)
        arg.ch.info.UY = Vector2(pos2.y,-arg.ch.info.UHeight.y) 
     ##########################################################
    def infoHandler(self, arg):
        print arg
        if self['CharInfo'].content.visible:
            self['CharInfo'].content.visible = False
        else:
            self['CharInfo'].content.visible = True

        
    ##########################################################
    def frameHandler(self, arg):
        print arg
        text = 'FPS: ' + str(A.getFPS()) + '\n' + 'Triangle: ' + str(A.getTriCount())
        self['CharInfo'].content.Text = text
        #if A.state == State.PLAY or A.state == State.DEAD:
        #self['OwnCharHP2'].content.Height = UVector2(0.3 * (100 - W.ownChar.percentHP) / 100,0)
    ##########################################################
    def charDieHandler(self, arg):
        if arg.ch.info != None:
            I.destroyWindow(arg.ch.info)
        arg.ch.info = None
        if W.ownChar.id == arg.ch.id:
            W.ownChar.percentHP = 0 
            print 'Hero die'
            A.state = State.DEAD    
     
################################  PLAY  ##############################        

                
class PlayState(GenericState):
    
    def enter(self):
        self.parent['CharInfo'].show()    
        self.parent['OwnCharHP1'].show()
        self.parent['OwnCharHP2'].show()
            
        handlers.addKeyPressedEvent(KC_LSHIFT,self.switchLayoutHandler)
        handlers.addKeyPressedEvent(KC_RSHIFT,self.switchLayoutHandler)
        handlers.addKeyPressedEvent(KC_TAB,self.selectNextTargetHandler)
        handlers.addKeyPressedEvent(KC_SPACE, self.spaceHandler)
        
        #showChat()
        
    def exit(self):       
        self.parent['CharInfo'].hide() 
        self.parent['OwnCharHP1'].hide()
        self.parent['OwnCharHP2'].hide()
        handlers.removeKeyPressedEvent(KC_LSHIFT,'switchLayoutHandler')
        handlers.removeKeyPressedEvent(KC_RSHIFT,'switchLayoutHandler')
        handlers.removeKeyPressedEvent(KC_TAB,'selectNextTargetHandler')
        handlers.removeKeyPressedEvent(KC_SPACE,'spaceHandler') 
        #hideChat()  
   
    def switchLayoutHandler(self, arg):
        #if I.isKeyPressed(KC_RCONTROL) or I.isKeyPressed(KC_LCONTROL):
        I.switchLayout()
        
    ##########################################################
    def selectNextTargetHandler(self, arg):
        #if I.onlyOneKeyPressed:
        I.nextTarget()
        
    ##########################################################
    def spaceHandler(self, arg):
        if W.ownChar.enemy:
            A.worldConn().sendFightCmd(FightCommand.FIGHT_STOP,0)
        elif I.target != None:
            A.worldConn().sendFightCmd(FightCommand.FIGHT_ATTACK,I.target.id)
        
   ##########################################################

class DeadState(GenericState):
    def enter(self):
        handlers.addKeyPressedEvent(KeyCode.KC_SPACE, self.spaceHandler)
        self['DeadText']= CharInfo().create('DeadText')
        self['DeadText'].content.pos = Vector2(0.5, 0.5)
        self['DeadText'].content.text = 'Вы умерли...\n Попробуйте нажать "Пробел" '
        self['DeadText'].show()
    def exit(self):
        handlers.removeKeyPressedEvent(KeyCode.KC_SPACE, 'spaceHandler')
        self['DeadText'].destroy()
    def spaceHandler(self, arg):
        A.worldConn().sendResurrect()


################################################################################



