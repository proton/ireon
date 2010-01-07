# -*- coding: utf-8 -*-

#
# Generated Thu Dec 28 21:34:06 2006 by generateDS.py.
#

import sys
import getopt
from xml.dom import minidom
from xml.dom import Node

class sys_stdout:
    def write(self, s):
        pass
SysStdout = sys_stdout()
#
# If you have installed IPython you can uncomment and use the following.
# IPython is available from http://ipython.scipy.org/.
#

## from IPython.Shell import IPShellEmbed
## args = ''
## ipshell = IPShellEmbed(args,
##     banner = 'Dropping into IPython',
##     exit_msg = 'Leaving Interpreter, back to program.')

# Then use the following line where and when you want to drop into the
# IPython shell:
#    ipshell('<some message> -- Entering ipshell.\nHit Ctrl-D to exit')

#
# Support/utility functions.
#

def showIndent(outfile, level):
    for idx in range(level):
        outfile.write('    ')

def quote_xml(inStr):
    if type(inStr) == unicode:
        return inStr
    if type(inStr) == str:
        return inStr
    s1 = str(inStr)
    if type(inStr) == bool:
        s1 = s1.lower()
        return s1    
    
    s1 = s1.replace('&', '&amp;')
    s1 = s1.replace('<', '&lt;')
    s1 = s1.replace('"', '&quot;')
    return s1

def quote_python(inStr):
    s1 = inStr
    if s1.find("'") == -1:
        if s1.find('\n') == -1:
            return "'%s'" % s1
        else:
            return "'''%s'''" % s1
    else:
        if s1.find('"') != -1:
            s1 = s1.replace('"', '\\"')
        if s1.find('\n') == -1:
            return '"%s"' % s1
        else:
            return '"""%s"""' % s1


class MixedContainer:
    # Constants for category:
    CategoryNone = 0
    CategoryText = 1
    CategorySimple = 2
    CategoryComplex = 3
    # Constants for content_type:
    TypeNone = 0
    TypeText = 1
    TypeString = 2
    TypeInteger = 3
    TypeFloat = 4
    TypeDecimal = 5
    TypeDouble = 6
    TypeBoolean = 7
    def __init__(self, category, content_type, name, value):
        self.category = category
        self.content_type = content_type
        self.name = name
        self.value = value
    def getCategory(self):
        return self.category
    def getContenttype(self, content_type):
        return self.content_type
    def getValue(self):
        return self.value
    def getName(self):
        return self.name
    def export(self, outfile, level, name):
        if self.category == MixedContainer.CategoryText:
            outfile.write(self.value)
        elif self.category == MixedContainer.CategorySimple:
            self.exportSimple(outfile, level, name)
        else:    # category == MixedContainer.CategoryComplex
            self.value.export(outfile, level, name)
    def exportSimple(self, outfile, level, name):
        if self.content_type == MixedContainer.TypeString:
            outfile.write('<%s>%s</%s>' % (self.name, self.value, self.name))
        elif self.content_type == MixedContainer.TypeInteger or \
                self.content_type == MixedContainer.TypeBoolean:
            outfile.write('<%s>%d</%s>' % (self.name, self.value, self.name))
        elif self.content_type == MixedContainer.TypeFloat or \
                self.content_type == MixedContainer.TypeDecimal:
            outfile.write('<%s>%f</%s>' % (self.name, self.value, self.name))
        elif self.content_type == MixedContainer.TypeDouble:
            outfile.write('<%s>%g</%s>' % (self.name, self.value, self.name))
    def exportLiteral(self, outfile, level, name):
        if self.category == MixedContainer.CategoryText:
            showIndent(outfile, level)
            outfile.write('MixedContainer(%d, %d, "%s", "%s"),\n' % \
                (self.category, self.content_type, self.name, self.value))
        elif self.category == MixedContainer.CategorySimple:
            showIndent(outfile, level)
            outfile.write('MixedContainer(%d, %d, "%s", "%s"),\n' % \
                (self.category, self.content_type, self.name, self.value))
        else:    # category == MixedContainer.CategoryComplex
            showIndent(outfile, level)
            outfile.write('MixedContainer(%d, %d, "%s",\n' % \
                (self.category, self.content_type, self.name,))
            self.value.exportLiteral(outfile, level + 1)
            showIndent(outfile, level)
            outfile.write(')\n')


#
# Data representation classes.
#

class CPCConfig:
    subclass = None
    def __init__(self, net=None, resources=None, world=None, profile=None, _interface=None):
        self.net = net
        self.resources = resources
        self.world = world
        self.profile = profile
        self._interface = _interface
    def factory(*args_, **kwargs_):
        if CPCConfig.subclass:
            return CPCConfig.subclass(*args_, **kwargs_)
        else:
            return CPCConfig(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getNet(self): return self.net
    def setNet(self, net): self.net = net
    def getResources(self): return self.resources
    def setResources(self, resources): self.resources = resources
    def getWorld(self): return self.world
    def setWorld(self, world): self.world = world
    def getProfile(self): return self.profile
    def setProfile(self, profile): self.profile = profile
    def get_interface(self): return self._interface
    def set_interface(self, _interface): self._interface = _interface
    def export(self, outfile, level, name_='CPCConfig', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CPCConfig'):
        pass
    def exportChildren(self, outfile, level, name_='CPCConfig'):
        if self.net:
            self.net.export(outfile, level, name_='net')
        if self.resources:
            self.resources.export(outfile, level, name_='resources')
        if self.world:
            self.world.export(outfile, level, name_='world')
        if self.profile:
            self.profile.export(outfile, level, name_='profile')
        if self._interface:
            self._interface.export(outfile, level, name_='_interface')
    def exportLiteral(self, outfile, level, name_='CPCConfig'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        if self.net:
            showIndent(outfile, level)
            outfile.write('net=CNet(\n')
            self.net.exportLiteral(outfile, level, name_='net')
            showIndent(outfile, level)
            outfile.write('),\n')
        if self.resources:
            showIndent(outfile, level)
            outfile.write('resources=CResources(\n')
            self.resources.exportLiteral(outfile, level, name_='resources')
            showIndent(outfile, level)
            outfile.write('),\n')
        if self.world:
            showIndent(outfile, level)
            outfile.write('world=CWorldSettings(\n')
            self.world.exportLiteral(outfile, level, name_='world')
            showIndent(outfile, level)
            outfile.write('),\n')
        if self.profile:
            showIndent(outfile, level)
            outfile.write('profile=CProfile(\n')
            self.profile.exportLiteral(outfile, level, name_='profile')
            showIndent(outfile, level)
            outfile.write('),\n')
        if self._interface:
            showIndent(outfile, level)
            outfile.write('_interface=CInterfaceSettings(\n')
            self._interface.exportLiteral(outfile, level, name_='_interface')
            showIndent(outfile, level)
            outfile.write('),\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'net':
            obj_ = CNet.factory()
            obj_.build(child_)
            self.setNet(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'resources':
            obj_ = CResources.factory()
            obj_.build(child_)
            self.setResources(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'world':
            obj_ = CWorldSettings.factory()
            obj_.build(child_)
            self.setWorld(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'profile':
            obj_ = CProfile.factory()
            obj_.build(child_)
            self.setProfile(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == '_interface':
            obj_ = CInterfaceSettings.factory()
            obj_.build(child_)
            self.set_interface(obj_)
# end class CPCConfig


class CNet:
    subclass = None
    def __init__(self, rootServer=None, worldServer=None):
        self.rootServer = rootServer
        self.worldServer = worldServer
    def factory(*args_, **kwargs_):
        if CNet.subclass:
            return CNet.subclass(*args_, **kwargs_)
        else:
            return CNet(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getRootserver(self): return self.rootServer
    def setRootserver(self, rootServer): self.rootServer = rootServer
    def getWorldserver(self): return self.worldServer
    def setWorldserver(self, worldServer): self.worldServer = worldServer
    def export(self, outfile, level, name_='CNet', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CNet'):
        pass
    def exportChildren(self, outfile, level, name_='CNet'):
        if self.rootServer:
            self.rootServer.export(outfile, level, name_='rootServer')
        if self.worldServer:
            self.worldServer.export(outfile, level, name_='worldServer')
    def exportLiteral(self, outfile, level, name_='CNet'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        if self.rootServer:
            showIndent(outfile, level)
            outfile.write('rootServer=CRootServer(\n')
            self.rootServer.exportLiteral(outfile, level, name_='rootServer')
            showIndent(outfile, level)
            outfile.write('),\n')
        if self.worldServer:
            showIndent(outfile, level)
            outfile.write('worldServer=CWorldServer(\n')
            self.worldServer.exportLiteral(outfile, level, name_='worldServer')
            showIndent(outfile, level)
            outfile.write('),\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'rootServer':
            obj_ = CRootServer.factory()
            obj_.build(child_)
            self.setRootserver(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'worldServer':
            obj_ = CWorldServer.factory()
            obj_.build(child_)
            self.setWorldserver(obj_)
# end class CNet


class CRootServer:
    subclass = None
    def __init__(self, ip='', port=''):
        self.ip = ip
        self.port = port
    def factory(*args_, **kwargs_):
        if CRootServer.subclass:
            return CRootServer.subclass(*args_, **kwargs_)
        else:
            return CRootServer(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getIp(self): return self.ip
    def setIp(self, ip): self.ip = ip
    def validate_CIPv4Address(self, value):
        # Validate type CIPv4Address, a restriction on xsd:string.
        pass
    def getPort(self): return self.port
    def setPort(self, port): self.port = port
    def validate_CPort(self, value):
        # Validate type CPort, a restriction on xsd:unsignedShort.
        pass
    def export(self, outfile, level, name_='CRootServer', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CRootServer'):
        pass
    def exportChildren(self, outfile, level, name_='CRootServer'):
        if self.getIp() != "":
            showIndent(outfile, level)
            outfile.write('<ip>%s</ip>\n' % quote_xml(self.getIp()))
        if self.getPort() != "":
            showIndent(outfile, level)
            outfile.write('<port>%s</port>\n' % quote_xml(self.getPort()))
    def exportLiteral(self, outfile, level, name_='CRootServer'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('ip=%s,\n' % quote_python(self.getIp()))
        showIndent(outfile, level)
        outfile.write('port=%s,\n' % quote_python(self.getPort()))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'ip':
            ip_ = ''
            for text__content_ in child_.childNodes:
                ip_ += text__content_.nodeValue
            self.ip = ip_
            self.validate_CIPv4Address(self.ip)    # validate type CIPv4Address
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'port':
            port_ = ''
            for text__content_ in child_.childNodes:
                port_ += text__content_.nodeValue
            self.port = port_
            self.validate_CPort(self.port)    # validate type CPort
# end class CRootServer


class CWorldServer:
    subclass = None
    def __init__(self, port=''):
        self.port = port
    def factory(*args_, **kwargs_):
        if CWorldServer.subclass:
            return CWorldServer.subclass(*args_, **kwargs_)
        else:
            return CWorldServer(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getPort(self): return self.port
    def setPort(self, port): self.port = port
    def validate_CPort(self, value):
        # Validate type CPort, a restriction on xsd:unsignedShort.
        pass
    def export(self, outfile, level, name_='CWorldServer', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CWorldServer'):
        pass
    def exportChildren(self, outfile, level, name_='CWorldServer'):
        if self.getPort() != "":
            showIndent(outfile, level)
            outfile.write('<port>%s</port>\n' % quote_xml(self.getPort()))
    def exportLiteral(self, outfile, level, name_='CWorldServer'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('port=%s,\n' % quote_python(self.getPort()))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'port':
            port_ = ''
            for text__content_ in child_.childNodes:
                port_ += text__content_.nodeValue
            self.port = port_
            self.validate_CPort(self.port)    # validate type CPort
# end class CWorldServer


class CWorldSettings:
    subclass = None
    def __init__(self, light=None, dirCfg='', zones=''):
        self.light = light
        self.dirCfg = dirCfg
        self.zones = zones
    def factory(*args_, **kwargs_):
        if CWorldSettings.subclass:
            return CWorldSettings.subclass(*args_, **kwargs_)
        else:
            return CWorldSettings(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getLight(self): return self.light
    def setLight(self, light): self.light = light
    def getDircfg(self): return self.dirCfg
    def setDircfg(self, dirCfg): self.dirCfg = dirCfg
    def getZones(self): return self.zones
    def setZones(self, zones): self.zones = zones
    def export(self, outfile, level, name_='CWorldSettings', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CWorldSettings'):
        pass
    def exportChildren(self, outfile, level, name_='CWorldSettings'):
        if self.light:
            self.light.export(outfile, level, name_='light')
        if self.getDircfg() != "":
            showIndent(outfile, level)
            outfile.write('<dirCfg>%s</dirCfg>\n' % quote_xml(self.getDircfg()))
        if self.getZones() != "":
            showIndent(outfile, level)
            outfile.write('<zones>%s</zones>\n' % quote_xml(self.getZones()))
    def exportLiteral(self, outfile, level, name_='CWorldSettings'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        if self.light:
            showIndent(outfile, level)
            outfile.write('light=CLightSettings(\n')
            self.light.exportLiteral(outfile, level, name_='light')
            showIndent(outfile, level)
            outfile.write('),\n')
        showIndent(outfile, level)
        outfile.write('dirCfg=%s,\n' % quote_python(self.getDircfg()))
        showIndent(outfile, level)
        outfile.write('zones=%s,\n' % quote_python(self.getZones()))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'light':
            obj_ = CLightSettings.factory()
            obj_.build(child_)
            self.setLight(obj_)
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'dirCfg':
            dirCfg_ = ''
            for text__content_ in child_.childNodes:
                dirCfg_ += text__content_.nodeValue
            self.dirCfg = dirCfg_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'zones':
            zones_ = ''
            for text__content_ in child_.childNodes:
                zones_ += text__content_.nodeValue
            self.zones = zones_
# end class CWorldSettings


class CLightSettings:
    subclass = None
    def __init__(self, enabled=None, main='', ambient='', shadows=None):
        self.enabled = enabled
        self.main = main
        self.ambient = ambient
        self.shadows = shadows
    def factory(*args_, **kwargs_):
        if CLightSettings.subclass:
            return CLightSettings.subclass(*args_, **kwargs_)
        else:
            return CLightSettings(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getMain(self): return self.main
    def setMain(self, main): self.main = main
    def getAmbient(self): return self.ambient
    def setAmbient(self, ambient): self.ambient = ambient
    def getShadows(self): return self.shadows
    def setShadows(self, shadows): self.shadows = shadows
    def getEnabled(self): return self.enabled
    def setEnabled(self, enabled): self.enabled = enabled
    def export(self, outfile, level, name_='CLightSettings', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='CLightSettings')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CLightSettings'):
        if self.getEnabled() is not None:
            outfile.write(' enabled="%s"' % (self.getEnabled(), ))
    def exportChildren(self, outfile, level, name_='CLightSettings'):
        if self.getMain() != "":
            showIndent(outfile, level)
            outfile.write('<main>%s</main>\n' % quote_xml(self.getMain()))
        if self.getAmbient() != "":
            showIndent(outfile, level)
            outfile.write('<ambient>%s</ambient>\n' % quote_xml(self.getAmbient()))
        if self.shadows:
            self.shadows.export(outfile, level, name_='shadows')
    def exportLiteral(self, outfile, level, name_='CLightSettings'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('enabled = "%s",\n' % (self.getEnabled(),))
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('main=%s,\n' % quote_python(self.getMain()))
        showIndent(outfile, level)
        outfile.write('ambient=%s,\n' % quote_python(self.getAmbient()))
        if self.shadows:
            showIndent(outfile, level)
            outfile.write('shadows=CShadowSettings(\n')
            self.shadows.exportLiteral(outfile, level, name_='shadows')
            showIndent(outfile, level)
            outfile.write('),\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('enabled'):
            self.enabled = attrs.get('enabled').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'main':
            main_ = ''
            for text__content_ in child_.childNodes:
                main_ += text__content_.nodeValue
            self.main = main_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'ambient':
            ambient_ = ''
            for text__content_ in child_.childNodes:
                ambient_ += text__content_.nodeValue
            self.ambient = ambient_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'shadows':
            obj_ = CShadowSettings.factory()
            obj_.build(child_)
            self.setShadows(obj_)
# end class CLightSettings


class CShadowSettings:
    subclass = None
    def __init__(self, enabled=None, colour=''):
        self.enabled = enabled
        self.colour = colour
    def factory(*args_, **kwargs_):
        if CShadowSettings.subclass:
            return CShadowSettings.subclass(*args_, **kwargs_)
        else:
            return CShadowSettings(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getColour(self): return self.colour
    def setColour(self, colour): self.colour = colour
    def getEnabled(self): return self.enabled
    def setEnabled(self, enabled): self.enabled = enabled
    def export(self, outfile, level, name_='CShadowSettings', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='CShadowSettings')
        outfile.write('>\n')
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CShadowSettings'):
        if self.getEnabled() is not None:
            outfile.write(' enabled="%s"' % (self.getEnabled(), ))
    def exportChildren(self, outfile, level, name_='CShadowSettings'):
        if self.getColour() != "":
            showIndent(outfile, level)
            outfile.write('<colour>%s</colour>\n' % quote_xml(self.getColour()))
    def exportLiteral(self, outfile, level, name_='CShadowSettings'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('enabled = "%s",\n' % (self.getEnabled(),))
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('colour=%s,\n' % quote_python(self.getColour()))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('enabled'):
            self.enabled = attrs.get('enabled').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'colour':
            colour_ = ''
            for text__content_ in child_.childNodes:
                colour_ += text__content_.nodeValue
            self.colour = colour_
# end class CShadowSettings


class CProfile:
    subclass = None
    def __init__(self, login='', password='', registration=''):
        self.login = login
        self.password = password
        self.registration = registration
    def factory(*args_, **kwargs_):
        if CProfile.subclass:
            return CProfile.subclass(*args_, **kwargs_)
        else:
            return CProfile(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getLogin(self): return self.login
    def setLogin(self, login): self.login = login
    def getPassword(self): return self.password
    def setPassword(self, password): self.password = password
    def getRegistration(self): return self.registration
    def setRegistration(self, registration): self.registration = registration
    def export(self, outfile, level, name_='CProfile', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CProfile'):
        pass
    def exportChildren(self, outfile, level, name_='CProfile'):
        if self.getLogin() != "":
            showIndent(outfile, level)
            outfile.write('<login>%s</login>\n' % quote_xml(self.getLogin()))
        if self.getPassword() != "":
            showIndent(outfile, level)
            outfile.write('<password>%s</password>\n' % quote_xml(self.getPassword()))
        if self.getRegistration() != "":
            showIndent(outfile, level)
            outfile.write('<registration>%s</registration>\n' % quote_xml(self.getRegistration()))
    def exportLiteral(self, outfile, level, name_='CProfile'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('login=%s,\n' % quote_python(self.getLogin()))
        showIndent(outfile, level)
        outfile.write('password=%s,\n' % quote_python(self.getPassword()))
        showIndent(outfile, level)
        outfile.write('registration=%s,\n' % quote_python(self.getRegistration()))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'login':
            login_ = ''
            for text__content_ in child_.childNodes:
                login_ += text__content_.nodeValue
            self.login = login_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'password':
            password_ = ''
            for text__content_ in child_.childNodes:
                password_ += text__content_.nodeValue
            self.password = password_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'registration':
            registration_ = ''
            for text__content_ in child_.childNodes:
                registration_ += text__content_.nodeValue
            self.registration = registration_
# end class CProfile


class CInterfaceSettings:
    subclass = None
    def __init__(self, dir='', symbols='', script=None):
        self.dir = dir
        self.symbols = symbols
        if script is None:
            self.script = []
        else:
            self.script = script
    def factory(*args_, **kwargs_):
        if CInterfaceSettings.subclass:
            return CInterfaceSettings.subclass(*args_, **kwargs_)
        else:
            return CInterfaceSettings(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getDir(self): return self.dir
    def setDir(self, dir): self.dir = dir
    def getSymbols(self): return self.symbols
    def setSymbols(self, symbols): self.symbols = symbols
    def getScript(self): return self.script
    def setScript(self, script): self.script = script
    def addScript(self, value): self.script.append(value)
    def insertScript(self, index, value): self.script[index] = value
    def export(self, outfile, level, name_='CInterfaceSettings', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CInterfaceSettings'):
        pass
    def exportChildren(self, outfile, level, name_='CInterfaceSettings'):
        if self.getDir() != "":
            showIndent(outfile, level)
            outfile.write('<dir>%s</dir>\n' % quote_xml(self.getDir()))
        if self.getSymbols() != "":
            showIndent(outfile, level)
            outfile.write('<symbols>%s</symbols>\n' % quote_xml(self.getSymbols()))
        for script_ in self.getScript():
            script_.export(outfile, level, name_='script')
    def exportLiteral(self, outfile, level, name_='CInterfaceSettings'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('dir=%s,\n' % quote_python(self.getDir()))
        showIndent(outfile, level)
        outfile.write('symbols=%s,\n' % quote_python(self.getSymbols()))
        showIndent(outfile, level)
        outfile.write('script=[\n')
        level += 1
        for script in self.script:
            showIndent(outfile, level)
            outfile.write('CResourceItem(\n')
            script.exportLiteral(outfile, level, name_='script')
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'dir':
            dir_ = ''
            for text__content_ in child_.childNodes:
                dir_ += text__content_.nodeValue
            self.dir = dir_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'symbols':
            symbols_ = ''
            for text__content_ in child_.childNodes:
                symbols_ += text__content_.nodeValue
            self.symbols = symbols_
        elif child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'script':
            obj_ = CResourceItem.factory()
            obj_.build(child_)
            self.script.append(obj_)
# end class CInterfaceSettings


class CResources:
    subclass = None
    def __init__(self, resource=None):
        if resource is None:
            self.resource = []
        else:
            self.resource = resource
    def factory(*args_, **kwargs_):
        if CResources.subclass:
            return CResources.subclass(*args_, **kwargs_)
        else:
            return CResources(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getResource(self): return self.resource
    def setResource(self, resource): self.resource = resource
    def addResource(self, value): self.resource.append(value)
    def insertResource(self, index, value): self.resource[index] = value
    def export(self, outfile, level, name_='CResources', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s%s>\n' % (name_, root_attrs_))
        self.exportChildren(outfile, level + 1, name_)
        showIndent(outfile, level)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CResources'):
        pass
    def exportChildren(self, outfile, level, name_='CResources'):
        for resource_ in self.getResource():
            resource_.export(outfile, level, name_='resource')
    def exportLiteral(self, outfile, level, name_='CResources'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        pass
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('resource=[\n')
        level += 1
        for resource in self.resource:
            showIndent(outfile, level)
            outfile.write('CResourceItem(\n')
            resource.exportLiteral(outfile, level, name_='resource')
            showIndent(outfile, level)
            outfile.write('),\n')
        level -= 1
        showIndent(outfile, level)
        outfile.write('],\n')
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        pass
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.ELEMENT_NODE and \
            nodeName_ == 'resource':
            obj_ = CResourceItem.factory()
            obj_.build(child_)
            self.resource.append(obj_)
# end class CResources


class CResourceItem:
    subclass = None
    def __init__(self, category=None, ttype=None, valueOf_=''):
        self.category = category
        self.ttype = ttype
        self.valueOf_ = valueOf_
    def factory(*args_, **kwargs_):
        if CResourceItem.subclass:
            return CResourceItem.subclass(*args_, **kwargs_)
        else:
            return CResourceItem(*args_, **kwargs_)
    factory = staticmethod(factory)
    def getCategory(self): return self.category
    def setCategory(self, category): self.category = category
    def getType(self): return self.ttype
    def setType(self, ttype): self.ttype = ttype
    def getValueOf_(self): return self.valueOf_
    def setValueOf_(self, valueOf_): self.valueOf_ = valueOf_
    def export(self, outfile, level, name_='CResourceItem', root_attrs_ = ''):
        showIndent(outfile, level)
        outfile.write('<%s' % (name_, ))
        self.exportAttributes(outfile, level, name_='CResourceItem')
        outfile.write('>')
        self.exportChildren(outfile, level + 1, name_)
        outfile.write('</%s>\n' % name_)
    def exportAttributes(self, outfile, level, name_='CResourceItem'):
        outfile.write(' category="%s"' % (self.getCategory(), ))
        outfile.write(' type="%s"' % (self.getType(), ))
    def exportChildren(self, outfile, level, name_='CResourceItem'):
        outfile.write(self.valueOf_)
    def exportLiteral(self, outfile, level, name_='CResourceItem'):
        level += 1
        self.exportLiteralAttributes(outfile, level, name_)
        self.exportLiteralChildren(outfile, level, name_)
    def exportLiteralAttributes(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('category = "%s",\n' % (self.getCategory(),))
        showIndent(outfile, level)
        outfile.write('ttype = "%s",\n' % (self.getType(),))
    def exportLiteralChildren(self, outfile, level, name_):
        showIndent(outfile, level)
        outfile.write('valueOf_ = "%s",\n' % (self.valueOf_,))
    def build(self, node_):
        attrs = node_.attributes
        self.buildAttributes(attrs)
        self.valueOf_ = ''
        for child_ in node_.childNodes:
            nodeName_ = child_.nodeName.split(':')[-1]
            self.buildChildren(child_, nodeName_)
    def buildAttributes(self, attrs):
        if attrs.get('category'):
            self.category = attrs.get('category').value
        if attrs.get('type'):
            self.ttype = attrs.get('type').value
    def buildChildren(self, child_, nodeName_):
        if child_.nodeType == Node.TEXT_NODE:
            self.valueOf_ += child_.nodeValue
# end class CResourceItem


from xml.sax import handler, make_parser

class SaxStackElement:
    def __init__(self, name='', obj=None):
        self.name = name
        self.obj = obj
        self.content = ''

#
# SAX handler
#
class SaxConfigHandler(handler.ContentHandler):
    def __init__(self):
        self.stack = []
        self.root = None

    def getRoot(self):
        return self.root

    def setDocumentLocator(self, locator):
        self.locator = locator
    
    def showError(self, msg):
        print '*** (showError):', msg
        sys.exit(-1)

    def startElement(self, name, attrs):
        done = 0
        if name == 'config':
            obj = config.factory()
            stackObj = SaxStackElement('config', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'net':
            obj = CNet.factory()
            stackObj = SaxStackElement('net', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'resources':
            obj = CResources.factory()
            stackObj = SaxStackElement('resources', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'world':
            obj = CWorldSettings.factory()
            stackObj = SaxStackElement('world', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'profile':
            obj = CProfile.factory()
            stackObj = SaxStackElement('profile', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == '_interface':
            obj = CInterfaceSettings.factory()
            stackObj = SaxStackElement('_interface', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'rootServer':
            obj = CRootServer.factory()
            stackObj = SaxStackElement('rootServer', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'worldServer':
            obj = CWorldServer.factory()
            stackObj = SaxStackElement('worldServer', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'ip':
            stackObj = SaxStackElement('ip', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'port':
            stackObj = SaxStackElement('port', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'light':
            obj = CLightSettings.factory()
            stackObj = SaxStackElement('light', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'dirCfg':
            stackObj = SaxStackElement('dirCfg', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'zones':
            stackObj = SaxStackElement('zones', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'main':
            stackObj = SaxStackElement('main', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'ambient':
            stackObj = SaxStackElement('ambient', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'shadows':
            obj = CShadowSettings.factory()
            stackObj = SaxStackElement('shadows', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'colour':
            stackObj = SaxStackElement('colour', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'login':
            stackObj = SaxStackElement('login', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'password':
            stackObj = SaxStackElement('password', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'registration':
            stackObj = SaxStackElement('registration', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'dir':
            stackObj = SaxStackElement('dir', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'symbols':
            stackObj = SaxStackElement('symbols', None)
            self.stack.append(stackObj)
            done = 1
        elif name == 'script':
            obj = CResourceItem.factory()
            stackObj = SaxStackElement('script', obj)
            self.stack.append(stackObj)
            done = 1
        elif name == 'resource':
            obj = CResourceItem.factory()
            stackObj = SaxStackElement('resource', obj)
            self.stack.append(stackObj)
            done = 1
        if not done:
            self.reportError('"%s" element not allowed here.' % name)

    def endElement(self, name):
        done = 0
        if name == 'config':
            if len(self.stack) == 1:
                self.root = self.stack[-1].obj
                self.stack.pop()
                done = 1
        elif name == 'net':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setNet(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'resources':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setResources(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'world':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setWorld(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'profile':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setProfile(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == '_interface':
            if len(self.stack) >= 2:
                self.stack[-2].obj.set_interface(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'rootServer':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setRootserver(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'worldServer':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setWorldserver(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'ip':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setIp(content)
                self.stack.pop()
                done = 1
        elif name == 'port':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setPort(content)
                self.stack.pop()
                done = 1
        elif name == 'light':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setLight(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'dirCfg':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setDircfg(content)
                self.stack.pop()
                done = 1
        elif name == 'zones':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setZones(content)
                self.stack.pop()
                done = 1
        elif name == 'main':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setMain(content)
                self.stack.pop()
                done = 1
        elif name == 'ambient':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setAmbient(content)
                self.stack.pop()
                done = 1
        elif name == 'shadows':
            if len(self.stack) >= 2:
                self.stack[-2].obj.setShadows(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'colour':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setColour(content)
                self.stack.pop()
                done = 1
        elif name == 'login':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setLogin(content)
                self.stack.pop()
                done = 1
        elif name == 'password':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setPassword(content)
                self.stack.pop()
                done = 1
        elif name == 'registration':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setRegistration(content)
                self.stack.pop()
                done = 1
        elif name == 'dir':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setDir(content)
                self.stack.pop()
                done = 1
        elif name == 'symbols':
            if len(self.stack) >= 2:
                content = self.stack[-1].content
                self.stack[-2].obj.setSymbols(content)
                self.stack.pop()
                done = 1
        elif name == 'script':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addScript(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        elif name == 'resource':
            if len(self.stack) >= 2:
                self.stack[-2].obj.addResource(self.stack[-1].obj)
                self.stack.pop()
                done = 1
        if not done:
            self.reportError('"%s" element not allowed here.' % name)

    def characters(self, chrs, start, end):
        if len(self.stack) > 0:
            self.stack[-1].content += chrs[start:end]

    def reportError(self, mesg):
        locator = self.locator
        sys.stderr.write('Doc: %s  Line: %d  Column: %d\n' % \
            (locator.getSystemId(), locator.getLineNumber(), 
            locator.getColumnNumber() + 1))
        sys.stderr.write(mesg)
        sys.stderr.write('\n')
        sys.exit(-1)
        #raise RuntimeError

USAGE_TEXT = """
Usage: python <Parser>.py [ -s ] <in_xml_file>
Options:
    -s        Use the SAX parser, not the minidom parser.
"""

def usage():
    print USAGE_TEXT
    sys.exit(-1)


#
# SAX handler used to determine the top level element.
#
class SaxSelectorHandler(handler.ContentHandler):
    def __init__(self):
        self.topElementName = None
    def getTopElementName(self):
        return self.topElementName
    def startElement(self, name, attrs):
        self.topElementName = name
        raise StopIteration


def parseSelect(inFileName):
    infile = file(inFileName, 'r')
    topElementName = None
    parser = make_parser()
    documentHandler = SaxSelectorHandler()
    parser.setContentHandler(documentHandler)
    try:
        try:
            parser.parse(infile)
        except StopIteration:
            topElementName = documentHandler.getTopElementName()
        if topElementName is None:
            raise RuntimeError, 'no top level element'
        topElementName = topElementName.replace('-', '_').replace(':', '_')
        if topElementName not in globals():
            raise RuntimeError, 'no class for top element: %s' % topElementName
        topElement = globals()[topElementName]
        infile.seek(0)
        doc = minidom.parse(infile)
    finally:
        infile.close()
    rootNode = doc.childNodes[0]
    rootObj = topElement.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    SysStdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(SysStdout, 0)
    return rootObj


def saxParse(inFileName):
    parser = make_parser()
    documentHandler = SaxConfigHandler()
    parser.setDocumentHandler(documentHandler)
    parser.parse('file:%s' % inFileName)
    root = documentHandler.getRoot()
    SysStdout.write('<?xml version="1.0" ?>\n')
    root.export(SysStdout, 0)
    return root


def saxParseString(inString):
    parser = make_parser()
    documentHandler = SaxConfigHandler()
    parser.setDocumentHandler(documentHandler)
    parser.feed(inString)
    parser.close()
    rootObj = documentHandler.getRoot()
    #SysStdout.write('<?xml version="1.0" ?>\n')
    #rootObj.export(SysStdout, 0)
    return rootObj


def parse(inFileName):
    doc = minidom.parse(inFileName)
    rootNode = doc.documentElement
    rootObj = CPCConfig.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    SysStdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(SysStdout, 0, name_="config")
    return rootObj


def parseString(inString):
    doc = minidom.parseString(inString)
    rootNode = doc.documentElement
    rootObj = CPCConfig.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    SysStdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(SysStdout, 0, name_="config")
    return rootObj


def parseLiteral(inFileName):
    doc = minidom.parse(inFileName)
    rootNode = doc.documentElement
    rootObj = CPCConfig.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    SysStdout.write('from config import *\n\n')
    SysStdout.write('rootObj = config(\n')
    rootObj.exportLiteral(SysStdout, 0, name_="config")
    SysStdout.write(')\n')
    return rootObj


def main():
    args = sys.argv[1:]
    if len(args) == 2 and args[0] == '-s':
        saxParse(args[1])
    elif len(args) == 1:
        parse(args[0])
    else:
        usage()


if __name__ == '__main__':
    main()
    #import pdb
    #pdb.run('main()')

