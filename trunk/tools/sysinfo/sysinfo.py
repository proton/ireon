#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""sysinfo.py outputs computer information to xml file"""

__author__ = "Dmitry Bitman (zak@ireon.org)"
__version__ = "$Id: sysinfo.py 627 2006-05-06 16:01:06Z zak $"
__date__ = "$Date: 2006-05-06 20:01:06 +0400 (Сбт, 06 Май 2006) $"
__copyright__ = "Copyright (C) 2006 ireon.org developers council "
__license__ = "See LICENSE for details"

from xml.dom import minidom
import sys
import os

OUTPUT_ELEMENTS =   (
                     ('', 'SystemInformation',
                        (
                         ('', 'Time',''),
                         ('', 'OperatingSystem',''),
                         ('', 'Processor',''),
                         ('', 'Memory',''),
                         ('', 'PageFile',''),
                         ('', 'DirectXVersion',''),
                        )
                     ),
                     ('', 'DisplayDevice',
                        (
                         ('', 'CardName',''),
                         ('', 'Manufacturer',''),
                         ('', 'ChipType',''),
                         ('', 'DisplayMemory',''),
                         ('', 'DriverVersion',''),
                         ('', 'DriverBeta',''),
                         ('', 'DriverDebug',''),
                         ('', 'DriverDate',''),
                         ('', 'DDIVersion',''),
                        )
                     ),
                     ('', 'SoundDevice',
                        (
                         ('', 'Type',''),
                         ('', 'DriverVersion',''),
                         ('', 'DriverBeta',''),
                         ('', 'DriverDebug',''),
                         ('', 'DriverDate',''),
                         ('', 'Description',''),
                        )
                     ),
                    )

class ComputerInfo:
    def __init__(self):
        self.rootElement = "sysinfo"
        self.tempFile = os.path.normpath(os.getcwd()+os.sep+'tmp.xml')
        self.cmd = "dxdiag /x " + self.tempFile

    def getInfo(self):
        """
        Use this method to get computer info and output it to file
        """
        self.runMSInfo()
        self.writeInfoToFile(self.parseInfo())

    def parseInfo(self):
        """
        Processes temporary xml file and returns resulting xml data
        """
        dom = minidom.parse(self.tempFile)
        outputDom = minidom.Document()
        root = outputDom.appendChild(outputDom.createElement(self.rootElement))
        self.processInfo(dom, outputDom, root, OUTPUT_ELEMENTS)
        return outputDom.toprettyxml()

    def processInfo(self, sourceDom, tagetDom, targetElement, elementsList):
        """
        Recursively processes needed elements list (elementsList)
        and creates dom structure accordingly
        """
        for targetName, sourceName, list in elementsList:
            if not targetName:
                targetName = sourceName
            el = targetElement.appendChild(tagetDom.createElement(targetName))
            if not list:
                el.appendChild(tagetDom.createTextNode(sourceDom.getElementsByTagName(sourceName)[0].firstChild.nodeValue))
            else:
                self.processInfo(sourceDom.getElementsByTagName(sourceName)[0], tagetDom, el, list)


    def writeInfoToFile(self, info, filename = "sysinfo.xml"):
        f = open(filename, 'w')
        f.write(info)
        f.close

    def runMSInfo(self):
        """
        Runs MS app, that outputs computer info to temporary xml file
        """
        try:
            os.system(self.cmd)
        except OSError:
            print OSError.__doc__

    def cleanup(self):
        """
        Deletes temporaryxml file
        """
        if os.path.exists(self.tempFile):
            os.unlink(self.tempFile)

    def __del__(self):
        self.cleanup()


if __name__ == '__main__':
    compinfo = ComputerInfo()
    compinfo.getInfo()


