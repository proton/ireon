# -*- coding: utf-8 -*-
################################################################################
# Prototype for all element        
class GenericElement:                   
    def __init__(self):
        pass
    
    content = None              #reference on window associated with this element
    parent = None               #reference on parent window
    
    def create(self, name):
        pass
    def show(self):
        pass
    def hide(self):
        pass
    def destroy (self):
        pass 
################################################################################
################################################################################
# Prototype for all element        
class GenericLeaf(GenericElement):                   
    def __init__(self):
        self.parent = None
    content = None              #reference on window associated with this element
    parent = None               #reference on parent window
    def create(self, name):
        pass
    def show(self):
        if self.parent != None:
            parentWnd = self.parent.content
        else:
            parentWnd = RW
        parentWnd.addChildWindow(self.content)
    def hide(self):
        if self.parent != None:
            parentWnd = self.parent.content
        else:
            parentWnd = RW
        parentWnd.removeChildWindow(self.content)
        #I.enableWindows()

    def destroy (self):
        GW.destroyWindow(self.content) 

################################################################################
# Prototype for all element        
class GenericComposite(GenericElement):                   
    def __init__(self):
        self.childs = copy(self.childs)  
        self.parent = None
    childs = {}                 #reference on active childs created or added in root
    content = None              #reference on window associated with this element
    parent = None               #reference on parent window
     
    def create(self, name):
        pass
    def show(self):
        if self.parent != None:
            parentWnd = self.parent.content
        else:
            parentWnd = RW
        parentWnd.addChildWindow(self.content)
        for i in self.childs:
            self.childs[i].show()
    def hide(self):
        if self.parent != None:
            parentWnd = self.parent.content
        else:
            parentWnd = RW
        parentWnd.removeChildWindow(self.content)
        #I.enableWindows()
    def destroy (self):
        pass
    def __getitem__(self, key):
        return self.childs[key]
    def __setitem__(self, key, value):
        value.parent = self
        self.childs[key] = value
################################################################################
class Button(GenericLeaf):
    def create(self, name ):
        c = GW.createWindow('TaharezLook/Button',name + '/Button')
        c.Size  = UVector2(UDim(0.3,0),UDim(0.1,0))
        c.Position   = UVector2(UDim(0.35,0),UDim(0.87,0))
        c.Text  = 'ok'
        self.content = c    # reference on root element
        return self

################################################################################
class StaticText(GenericLeaf):
    def create(self, name ):
        c = GW.createWindow('TaharezLook/StaticText', name + '/Text')
        c.Size  = UVector2(UDim(0.9,0),UDim(0.65,0))
        c.Position   = UVector2(UDim(0.05,0),UDim(0.2,0))
        c.Text  = 'Статик'
        #c.setFormatting(HorzFormatting.WordWrapCentered,VertFormatting.VertCentered)
        #c.setBackgroundEnabled(False)
        #c.setFrameEnabled(False)
        self.content = c
        return self
################################################################################
class FrameWindow(GenericLeaf):
    def create(self, name):
        w = GW.createWindow('TaharezLook/FrameWindow', name)
        w.Text = 'FrameWindow'
        w.Size = UVector2(UDim(0.45,0),UDim(0.4,0))
        w.Position = UVector2(UDim(0.3,0),UDim(0.25,0))
        #w.setCloseButtonEnabled(False)
        #w.setAlwaysOnTop(True)
        self.content = w
        return self
################################################################################
class PopupWindow(GenericComposite):
    def create(self, title = 'Опасность!', text = 'Ошибка'):
        name = 'PopupWindow' + str(count())
        self['FrameWindow'] = FrameWindow().create(name)
        self['StaticText'] = StaticText().create(name)
        self['Button'] = Button().create(name)
         
        self['FrameWindow'].content.Text = title
        self['StaticText'].content.Text = text
   
        handler = multiExportToMainDict(self.OKHandler)
        self['Button'].content.subscribeScriptedEvent("MouseClick",handler)
        
        #GW.addChildWindow(self['StaticText'].content, self['FrameWindow'].content)
        #GW.addChildWindow(self['Button'].content, self['FrameWindow'].content)
        
        self.content = self['FrameWindow'].content
        return self
       #--------------------------------------------------
    def OKHandler(self,arg):
        self.hide()
        self.destroy()
################################################################################
class MainMenu(GenericComposite):
        
    def create(self, name = 'dont need'):
        self['ConnectWindow'] = win2composite(GW.getWindow('MainMenu/ConnectWindow'))
        self['ConnectWindow']['ButtonOK'] = win2leaf(GW.getWindow('MainMenu/ConnectWindow/ButtonOK'))
        self['ConnectWindow']['ButtonCancel'] = win2leaf(GW.getWindow('MainMenu/ConnectWindow/ButtonCancel'))
      
        w = GW.getWindow('MainMenu/ConnectWindow/HostEdit')
        
        w.Text = str(cfg.root.getNet().getRootserver().getIp())
        self['ConnectWindow']['HostEdit'] = win2leaf(w)
        
        w = GW.getWindow('MainMenu/ConnectWindow/LoginEdit')
        w.Text = str(cfg.root.getProfile().getLogin())
        self['ConnectWindow']['LoginEdit'] = win2leaf(w)
        
        w = GW.getWindow('MainMenu/ConnectWindow/PasswordEdit')
        w.Text = str(cfg.root.getProfile().getPassword())
        self['ConnectWindow']['PasswordEdit'] = win2leaf(w)
        
        w = GW.getWindow('MainMenu/ConnectWindow/CheckBox')
        #w.checked = False
        self['ConnectWindow']['CheckBox'] = win2leaf(w)
        
        self.content = GW.getWindow('MainMenu')
        return self

   
################################################################################
class RootMenu(GenericComposite):
    def create(self, name = 'dont need'):
        w = GW.getWindow('RootMenu/CharactersList')
        self['CharactersList'] = win2composite(w)
        
        w = GW.getWindow('RootMenu/CharactersList/AddCharacterButton')
        self['CharactersList']['AddCharacterButton'] = win2leaf(w)
        
        w = GW.getWindow('RootMenu/CharactersList/RemoveCharacterButton')
        self['CharactersList']['RemoveCharacterButton'] = win2leaf(w)
        
        w = GW.getWindow('RootMenu/CharactersList/SelectCharacterButton')
        self['CharactersList']['SelectCharacterButton'] = win2leaf(w)
        
        w = GW.getWindow('RootMenu/CharactersList/ExitButton')
        self['CharactersList']['ExitButton'] = win2leaf(w)
        
        w = GW.getWindow('RootMenu/CharactersList/List')
        #w.cols = 2
        #arr = ByteVector()
        #arr.append(10)
        #arr.append(88)
        #w.setWidth( arr )
        #w.setHead(0,'ID')
        #w.setHead(1,'Name')
        w.addColumn('ID', 0, UDim(0.1,0))
        w.addColumn('Name', 1, UDim(0.1,0))
        w.getHeaderSegmentForColumn(0).Size = UVector2(UDim(0.1,0),UDim(1, 0))
        w.getHeaderSegmentForColumn(1).Size = UVector2(UDim(0.88,0),UDim(1, 0))
        
        self['CharactersList']['List'] = win2leaf(w)
        
        #w = I.getWindow('RootMenu/AddCharactersList')
        #self['AddCharactersList'] = win2leaf?composite(w)
               
        self.content = GW.getWindow('RootMenu')
        return self
################################################################################
class AddCharacterWindow(GenericComposite):
    def create(self, name = 'dont need'):
                
        w = GW.getWindow('AddCharacterWindow/NameEdit')
        self['NameEdit'] = win2leaf(w)
                
        w = GW.getWindow('AddCharacterWindow/OK')
        self['OK'] = win2leaf(w)
        
        w = GW.getWindow('AddCharacterWindow/Cancel')
        self['Cancel'] = win2leaf(w)
        
        w = GW.getWindow('AddCharacterWindow')
        self.content = w
        return self
################################################################################        
class ConnectionPopupWindow(GenericComposite):
    def create(self, name = 'dont need'):

        w = GW.getWindow('ConnectionPopupWindow/Text')
        #w.setFormatting(HorzFormatting.WordWrapCentered,VertFormatting.VertCentered)
        
        self['Text'] = win2leaf(w)
                
        w = GW.getWindow('ConnectionPopupWindow/Button')
        self['Button'] = win2leaf(w)
               
        w = GW.getWindow('ConnectionPopupWindow');
        w.Text = 'Connecting'
        self.content = w 
        print 'connecting popup window create'
        return self
        
################################################################################
class LoggingPopupWindow(GenericComposite):
    def create(self):
        w = GW.getWindow('LoggingPopupWindow/Text')
        #w.setFormatting(HorzFormatting.WordWrapCentered,VertFormatting.VertCentered)
        self['Text'] = win2leaf(w)
        self.content = GW.getWindow('LoggingPopupWindow')
        print 'logging popup window create'
        return self
        
################################################################################
class StatusImage(GenericLeaf):
    def create(self, name):
        
        self.content = GW.getWindow('StatusWindow/HP') 
        self.content.disable()
        
        #hp = GW.createWindow('TaharezLook/StatusImage',name)
        
        #hp.setProperty("Image", "set:TaharezLook/StatusImage image:full_image")
        #d = cegui.ImagesetManager.getSingleton().getImageset("TaharezLook").getImage("TextSelectionBrush")
        #hp.setImage('TaharezLook','TextSelectionBrush')
        #hp.Colour = Colour(1,0,0)
        #hp.UWidth = Vector2(0.04, 0)
        #hp.UHeight = Vector2(0.3, 0)
        #hp.UX = Vector2(0.92,0)
        #hp.UY = Vector2(0.7, -5 )
        #hp.disable()
        #self.content = hp
        return self

################################################################################
class CharInfo(GenericLeaf):
    def create(self, name):
        #w = GW.createWindow('TaharezLook/CharInfo',name,WinType.STATIC_TEXT)
        w = GW.createWindow('TaharezLook/StaticText',name)
        w.Width = UDim(0.3,0)
        w.Height = UDim(0.2,0)
        w.Position = cegui.UVector2(UDim(0,0) , UDim(0,0))
        # c.setFormatting(HorzFormatting.WordWrapCentered,VertFormatting.VertCentered)
        #c.setBackgroundEnabled(False)
        #c.setFrameEnabled(False)
       
        self.content = w
        return self
        
################################################################################
################################################################################
class DefaultGUISheet(GenericLeaf):
    def create(self, name):
        w = GW.createWindow('DefaultGUISheet',name )
        w.Width = UVector2(UDim(0.3,0),UDim(0,0))
        w.Height = UVector2(UDim(0.2,0),UDim(0,0))
        w.disable()
        self.content = w
        return self

################################################################################
        
class CharInfoWindow(GenericComposite):
    def create(self, name, text):
        self['DefaultGUISheet'] = DefaultGUISheet().create(name + '_infoWindow')
       
        width = UVector2(0,I.getTextExtent(text) + 10)
        height = UVector2(0,I.getFontHeight() + 5)
        
        self['CharInfo'] = CharInfo().create(name + '_infoWindow/Text')
        self['CharInfo'].content.text = text
        self['CharInfo'].content.UWidth = width
        self['CharInfo'].content.UHeight = height
       
        self['StatusImage'] = StatusImage().create(name + '_infoWindow/HP')
        self['StatusImage'].content.text = text
        self['StatusImage'].content.UX = Vector2(0,2)
        self['StatusImage'].content.UY = Vector2(0, height.y ) 
        self['StatusImage'].content.UWidth = width - Vector2(0,6)
        self['StatusImage'].content.UHeight = Vector2(0,3)
        self['StatusImage'].content.colour = Colour(1,0,0)
        
        self['DefaultGUISheet'].content.UWidth = width
        self['DefaultGUISheet'].content.UHeight = height + Vector2(0,10)
        
        I.linkWindow( self['StatusImage'].content, self['DefaultGUISheet'].content)
        I.linkWindow( self['CharInfo'].content, self['DefaultGUISheet'].content)
        self.content = self['DefaultGUISheet'].content
        return self

################################################################################
class MenuExitWindow(GenericComposite, Singleton):
    def create(self, title = 'Exit', text = 'Выберите действие'):
        name = 'MenuExitWindow'+str(count())
        self['FrameWindow'] = FrameWindow().create(name)
        
        self['StaticText'] = StaticText().create(name)
        self['ButtonExit'] = Button().create(name + 'Exit')
        self['ButtonCancel'] = Button().create(name + 'Cancel')
        self['ButtonExit'].content.Text = 'Выйти из игры'
        self['ButtonCancel'].content.Text = 'Cancel'
        self['ButtonExit'].content.Position = UVector2(UDim(0.1,0),UDim(0.75,0))
        self['ButtonExit'].content.Size = UVector2(UDim(0.35,0),UDim(0.1,0))
        self['ButtonCancel'].content.Position = UVector2(UDim(0.6,0),UDim(0.75,0))
        self['ButtonCancel'].content.Size = UVector2(UDim(0.35,0),UDim(0.1,0))
        self['FrameWindow'].content.Text = title
        
        self['StaticText'].content.Text = text
        self['StaticText'].content.disable()
        self['StaticText'].content.Size  = UVector2(UDim(0.9,0),UDim(0.3,0))
        self['StaticText'].content.Position  = UVector2(UDim(0.05,0),UDim(0.4,0))
        
        handler = exportToMainDict(self.menuExitHandler)
        self['ButtonExit'].content.subscribeScriptedEvent("MouseClick",handler)
        handler = exportToMainDict(self.menuCancelHandler)
        self['ButtonCancel'].content.subscribeScriptedEvent("MouseClick",handler)
        
        self['FrameWindow'].content.addChildWindow(self['StaticText'].content)
        self['FrameWindow'].content.addChildWindow(self['ButtonExit'].content)
        self['FrameWindow'].content.addChildWindow(self['ButtonCancel'].content)
        

        self.content = self['FrameWindow'].content
        
        return self
       #--------------------------------------------------
    def menuExitHandler(self,arg):
        self.hide()
        self.destroy()
        self.singleton = 0
        A.shutdown()
    
    def menuCancelHandler(self,arg):
        self.hide()
        self.destroy()
        self.singleton = 0
        
class RootExitWindow(MenuExitWindow):
    def create(self, title = 'Выход', text = 'Выберите действие'):
        MenuExitWindow.create(self)
        name = 'RootExitWindow'+str(count())
        self['ButtonMenu'] = Button().create(name + 'Menu')
        self['ButtonMenu'].content.pos = Vector2(0.1,0.25)
        
        self['ButtonMenu'].content.text = 'Menu'
        self['FrameWindow'].content.text = title
        self['StaticText'].content.text = text
   
        evt = WindowEvent()
        evt.window = self['FrameWindow'].content.name
        evt.handler = exportToMainDict(self.rootExitMenuHandler)
        self['ButtonMenu'].content.subscribeEvent(PushButton.EventClicked,evt)
        
        I.linkWindow(self['ButtonMenu'].content, self['FrameWindow'].content)
        self.content = self['FrameWindow'].content
        return self
       #--------------------------------------------------
    def rootExitMenuHandler(self,arg):
        self.hide()
        self.destroy()
        A.state = State.MENU

class PlayExitWindow(RootExitWindow):
    def create(self, title, text):
        RootExitWindow.create(self)
        name = 'PlayExitWindow' + str(count())
        self['ButtonRoot'] = Button().create(name + 'Root')
        self['ButtonRoot'].content.pos = Vector2(0.6,0.25)
        self['ButtonRoot'].content.text = 'Root'
        self['FrameWindow'].content.text = title
        self['StaticText'].content.text = text
   
        evt = WindowEvent()
        evt.window = self['FrameWindow'].content.name
        evt.handler = exportToMainDict(self.playExitRootHandler)
        self['ButtonMenu'].content.subscribeEvent(PushButton.EventClicked,evt)
        
        I.linkWindow(self['ButtonRoot'].content, self['FrameWindow'].content)
        
        self.content = self['FrameWindow'].content
        return self
       #--------------------------------------------------
    def playExitRootHandler(self,arg):
        self.hide()
        self.destroy()
        A.state = State.ROOT

 
class ListTextItem(cegui.ListboxTextItem):
    def __init__(self, text,id=0):
        cegui.ListboxTextItem.__init__(self, text, id)
        # TODO: ERROR!!!!
        # self.selectionBrushImage not called. maybe reason in cegui.fix_args ? 
        d = cegui.ImagesetManager.getSingleton().getImageset("TaharezLook").getImage("MultiListSelectionBrush")
        cegui._cegui.ListboxItem___setSelectionBrushImage(self, d)
        
        #self.selectionBrushImage = \
        #    cegui.ImagesetManager.getSingleton().getImageset("TaharezLook").getImage("MultiListSelectionBrush")
        
    def __del__(self):
        ''' Ignore python GC, the object is in CEGUI control now '''
        pass
        