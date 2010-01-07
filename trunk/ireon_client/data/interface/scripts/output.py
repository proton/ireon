# -*- coding: utf-8 -*-
import sys
sys.path.append('../data/interface/scripts')
sys.path.append('../data/interface/scripts/libs')

#import App
#import OgreTypes
#from OgreTypes import *
#from string import *
import IreonClient
from IreonClient import *

##### Standart output redirect for logging error in Application in this script #####
sys.stderr = IreonClient.PythonOutput()
sys.stdout = IreonClient.PythonOutput()
##### Wrapper for output redirect in registered file object  ####
class COutputEx:
    out = []
    stderr = None
    stdout = None
    def __init__(self, stdout, stderr):
        self.stdout = stdout
        self.stderr = stderr
        
    def reg(self, out):
        if out not in self.out:
            self.out.append([out,True])
            
    def changeOutputActivity(output, activity):
        for i in self.out():
            if id(output) == id(i[0]):
                i[1] = activity
        
    def write(self,s):
        for i in self.out:
            if i[1] == True:
                #if s!='\n':
                #    s += '\b' # non break line flag, wherefore python automate send '\n', but loglistener add yet '\n' 
                i[0](s)
    def flush(self):
        for i in self.out:
            if i[1] == True:
                i[0]('\n')

    def restoreStandartSystemOutput(self):
        sys.stderr = self.stderr
        sys.stdout = self.stdout
    def restoreClientOutput(self):
        sys.stderr = OutputEx
        sys.stdout = OutputEx
    
        
OutputEx = COutputEx( sys.stdout, sys.stdout)
#### registered in wrapper our App output        
OutputEx.reg(IreonClient.PythonOutput().write)
#### replace standart output redirect with this wrapper
#sys.stderr = OutputEx
#sys.stdout = OutputEx
OutputEx.restoreClientOutput()
