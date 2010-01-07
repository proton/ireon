# -*- coding: utf-8 -*-
#### Utility function for split string > 80 symbols
#### TODO: else modify PythonOutputEx for this  
#### this is geniune code writed in night time... sorry for this ass-code :)
def output(out):
    outStr = str(out)
    if len(outStr) <= 80 :
        print outStr
        return
    outList = outStr.split(" ")
    out80List = []
    out80 = ""
    for x in outList:
        out80 = out80 + x
        
        if len(out80) > 80: 
            out80List.append(out80)
            out80 = ""    
        
    for s in out80List:
        print s
################################################################################
## Export config in the xml
# TODO: mb save in anoter file , after copy in config.xml ?
class Cfg:
    root = None
    def load(self):
        self.root = config.parse('../config/config.xml')
    def save(self):
        import codecs
        f = codecs.open("../config/config.xml","w", 'utf-8')
        #f.write( codecs.BOM_UTF8 )
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        f.write("<!--\n\n    Configuration file for ireon.org client.\n\n-->\n")
        self.root.export(f,0,name_='cfg:config' , root_attrs_ = ' xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"\n      xmlns:cfg="http://www.ireon.org/ireon/client/"\n     xsi:schemaLocation="http://www.ireon.org/ireon/client/ config.xsd" ')
        f.close()


cfg = Cfg()
################################################################################
## Make reference for object(class instance) method in __main__.__dict__
class CHandlers:
    event = {}
    keyPressedEvent = {}
       
    def addEvent(self, idEvent, function):
        if self.event.has_key(idEvent) == False:
            self.event[idEvent] = []
        reference = function.func_name + str(idEvent)
        import __main__
        exec('__main__.__dict__["%s"] = function' %  reference)
        E.addEvent(idEvent, reference)
        self.event[idEvent].append(reference) 
        
    def removeEvent(self, idEvent, reference):
        if self.event.has_key(idEvent) == False:
            return
        reference = reference + str(idEvent)
        if reference not in self.event[idEvent]:
            return
        E.removeEvent(idEvent, reference)
        import __main__  
        exec('del __main__.__dict__["%s"]' %  reference)
        self.event[idEvent].remove(reference)
       
    def removeEventAll(self, idEvent):
        if self.event.has_key(idEvent) == False:
            return
        
        import __main__
        for i in self.event[idEvent]:
            E.removeEvent(idEvent, i)
            exec('del __main__.__dict__["%s"]' %  i)     
        del self.event[idEvent]      
    
    def addKeyPressedEvent(self, idEvent, function):
        if self.keyPressedEvent.has_key(idEvent) == False:
            self.keyPressedEvent[idEvent] = []
        reference = function.func_name + str(idEvent)
        import __main__
        exec('__main__.__dict__["%s"] = function' %  reference)
        E.addKeyPressedEvent(idEvent, reference)
        #print "addKeyPressedEvent '%s' '%s'" % (str(idEvent), reference)
        self.keyPressedEvent[idEvent].append(reference) 
        
    def removeKeyPressedEvent(self, idEvent, reference):
        if self.keyPressedEvent.has_key(idEvent) == False:
            return
        reference = reference + str(idEvent)
        if reference not in self.keyPressedEvent[idEvent]:
            return
        E.removeKeyPressedEvent(idEvent, reference)
        import __main__  
        exec('del __main__.__dict__["%s"]' %  reference)
        self.keyPressedEvent[idEvent].remove(reference)
    #don't work    
    def removeKeyPressedEventAll(self, idEvent):
        if self.keyPressedEvent.has_key(idEvent) == False:
            return
        import __main__
        for i in self.keyPressedEvent[idEvent]:
            E.removeKeyPressedEvent(idEvent, i)
            exec('del __main__.__dict__["%s"]' %  i)   #see here
        del self.keyPressedEvent[idEvent]      
    def addKeyReleasedEvent(self):
        pass
    def removeKeyReleasedEvent(self):
        pass
    def addKeyClickedEvent(self):
        pass
    def removeKeyClickedEvent(self):
        pass
handlers = CHandlers()


def exportToMainDict(method):
    reference = method.func_name #+ str(countMethod())
    import __main__
    exec('__main__.__dict__["%s"] = method' %  reference)
    return reference

def multiExportToMainDict(method):
    reference = method.func_name + str(countMethod())
    import __main__
    exec('__main__.__dict__["%s"] = method' %  reference)
    return reference

################################################################################
def checkEditText(arg):
    nameEditBox, ErrorMessages, setting = arg[0], arg[1], arg[2]
    text = GW.getWindow(nameEditBox).Text
    if  text == '':
        
        p = PopupWindow().create(title = ErrorMessages[0], text = ErrorMessages[1])
        p.show()
        #I.disableWindows
        return False
    else:
        #maybe don't set setting associated with edit box
        if setting!= '':          # TODO: maybe best use None, i think last 
            exec("%s( unicode('%s','utf-8')) " % (setting, text.decode('utf-8')), globals())
            
            
        return True

def execMap (f, *arg):
    for det in xrange(len(arg[0])):
        #print 'EXECMAP = ',det
        i = [x[det] for x in arg]
        if f(i) == False:
            return False
    return True
    
#################
def win2leaf(win):
    obj = GenericLeaf() 
    obj.content = win
    return obj
 
def win2composite(win):
    obj = GenericComposite() 
    obj.content = win
    return obj



class Singleton(object):
    def __new__(type):
        if not '_the_instance' in type.__dict__:
            type._the_instance = object.__new__(type)
        return type._the_instance
    singleton = 0
    def __getattribute__(self, key):
        if key == 'create':
            if self.singleton == 0:
                self.singleton = 1
                return object.__getattribute__(self, key)
            else:
                return object.__getattribute__(self, 'createNone')
        if key == 'show':
            if self.singleton == 1:
                self.singleton = 2
                return object.__getattribute__(self, key)
            else:
                return object.__getattribute__(self, 'createNone')
        return object.__getattribute__(self, key)
    def createNone(self, *arg, **nameArg):
        print self," create none"
        return self
    def showNone(self, *arg, **nameArg):
        print self," show none"
        return self
     