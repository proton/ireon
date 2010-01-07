#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""launcher.py is a bootstrapper for ireon.org client"""

__author__ = "Dmitry Bitman (zak@ireon.org)"
__version__ = "$Id: launcher.pyw 674 2006-08-20 09:56:35Z zak $"
__date__ = "$Date: 2006-08-20 13:56:35 +0400 (Вск, 20 Авг 2006) $"
__copyright__ = "Copyright (C) 2006 ireon.org developers council "
__license__ = "See LICENSE for details"

import os
if os.path.exists("launcher.startup"):
	execfile("launcher.startup")

import gettext
import socket
import time
from threading import Thread
import wx
from wx import xrc

if os.path.exists("launcher.conf"):
	execfile("launcher.conf")
else:
	import sys
	print "config file not found!"
	sys.exit(1)

GUI_FILENAME = "launcher.xrc"
GUI_MAINFRAME_NAME = "LAUNCHER"

PING_OK = 0
PING_FAILED = 1

PING_STATUS_TO_WIDGET_SUFFIX = {PING_OK: '_ok', PING_FAILED: '_failed'}


# ---------------------------------------------------------------------
# ------------- custom event ------------------------------------------
# ---------------------------------------------------------------------

EVT_PING_RESPONCE_ID = wx.NewEventType()
EVT_PING_RESPONCE = wx.PyEventBinder(EVT_PING_RESPONCE_ID, 0)

class PingResponceEvent(wx.PyEvent):
	def __init__(self, pingid, status):
		wx.PyEvent.__init__(self)
		self.pingid = pingid
		self.status = status
		self.SetEventType(EVT_PING_RESPONCE_ID)

	def Clone(self):
		return PingResponceEvent(self.pingid, self.status)

# ---------------------------------------------------------------------
# --------------- threaded server ping --------------------------------
# ---------------------------------------------------------------------

class pingit(Thread):
	def __init__ (self, app, pingid, host, port):
		Thread.__init__(self)
		self.pingid = pingid
		self.host = host
		self.port = port
		self.app = app
	def run(self):
		try:
			socket.setdefaulttimeout(5)
			s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			s.connect((self.host, self.port))
			s.send('\x00')
			self.app.AddPendingEvent(PingResponceEvent(self.pingid, PING_OK));
			s.close()
		except:
			s.close()
			self.app.AddPendingEvent(PingResponceEvent(self.pingid, PING_FAILED));


#        result = os.popen("ping -n 10 -w 100 "+self.ip,"r")
#        while 1:
#            #line = pingaling.readline()
#            wx.PostEvent(app, PingResponceEvent("---asd---"));
#            return True


# ---------------------------------------------------------------------
# --------------- wx stuff --------------------------------------------
# ---------------------------------------------------------------------

class MyApp(wx.App):
	def __init__(self):
		wx.App.__init__(self,redirect = False)

	def OnInit(self):
		self.InitI18N()
		self.res = xrc.XmlResource(GUI_FILENAME)
		self.frame = self.res.LoadFrame(None, GUI_MAINFRAME_NAME)
		image = wx.Image('..\common\images\system-software-update-16.ico', wx.BITMAP_TYPE_ANY)
		image = image.ConvertToBitmap()

		icon = wx.EmptyIcon()
		icon.CopyFromBitmap(image)

		self.frame.SetIcon(icon)


		self.output = xrc.XRCCTRL(self.frame, "output_area")
		self.OutputText(_("GMT time:")+' '+time.strftime("%c",time.gmtime())+"\n\n")

		self.update_button = xrc.XRCCTRL(self.frame, "update_game_button")
		self.Bind(wx.EVT_BUTTON, self.OnUpdateGame, self.update_button)

		self.launch_button = xrc.XRCCTRL(self.frame, "launch_game_button")
		self.Bind(wx.EVT_BUTTON, self.OnLaunchGame, self.launch_button)

		self.Bind(EVT_PING_RESPONCE, self.OnPingResponceEvent)

		self.frame.Show()
		self.SetTopWindow(self.frame)
		return True

	def OutputText(self, text):
		self.output.AppendText(text.decode('utf-8'))

	def OnUpdateGame(self, evt):
		self.OutputText("\n"+_("Update started...")+"\n")
		self.launch_button.Disable()
		self.update_button.Disable()

		self.Bind(wx.EVT_IDLE,self.OnIdle)
		self.Bind(wx.EVT_END_PROCESS,self.OnEndProcess)

		self.process = wx.Process(self)
		self.process.Redirect()
		self.pid = wx.Execute(os.path.normpath(ROOT_DIR + os.sep + SVN_PATH) + " cleanup " + ROOT_DIR, wx.EXEC_SYNC, self.process)
		if self.pid:
			self.inputstream    = self.process.GetInputStream()
			self.errorstream    = self.process.GetErrorStream()
			self.OnIdle()
		self.process.Destroy()
		self.process    = None

		self.process = wx.Process(self)
		self.process.Redirect()
		self.pid = wx.Execute(os.path.normpath(ROOT_DIR + os.sep + SVN_PATH) + " update " + ROOT_DIR, wx.EXEC_ASYNC, self.process)

		if not self.pid:
			self.Unbind(wx.EVT_IDLE)
			self.Unbind(wx.EVT_END_PROCESS)
			self.process.Destroy()
			self.process    = None
			self.OutputText("\n"+_("Update failed!")+"\n")
			self.launch_button.Enable()
			self.update_button.Enable()
		else:
			self.inputstream    = self.process.GetInputStream()
			self.errorstream    = self.process.GetErrorStream()


	def OnIdle(self, evt):
		if self.inputstream.CanRead():
			text = self.inputstream.read()
			self.OutputText(text)
		if self.errorstream.CanRead():
			text = self.errorstream.read()
			self.OutputText(text)


	def OnEndProcess(self, evt):
		self.Unbind(wx.EVT_IDLE)
		self.Unbind(wx.EVT_END_PROCESS)
		self.OnIdle(evt)
		self.process.Destroy()
		self.process    = None
		self.launch_button.Enable()
		self.update_button.Enable()
		self.OutputText(_("Update finished.")+"\n\n")

	def OnLaunchGame(self, evt):
		os.chdir(os.path.normpath(ROOT_DIR + os.sep + CLIENT_RELATIVE_DIR))
		os.startfile(CLIENT_FILENAME)
		self.frame.Close()

	def OnPingResponceEvent(self, evt):
		img = xrc.XRCCTRL(self.frame, PINGS[evt.pingid][0]+PING_STATUS_TO_WIDGET_SUFFIX[evt.status])
		img.Show()
		img.CenterOnParent()
		if evt.status == PING_OK:
			resp = _("ok")
		else:
			resp = _("failed")
		self.OutputText("PING: " + PINGS[evt.pingid][0] + " -- " + resp +"\n")

	def InitI18N(self):
		self.locale = wx.Locale(wx.LANGUAGE_RUSSIAN, wx.LOCALE_LOAD_DEFAULT | wx.LOCALE_CONV_ENCODING)
#        self.locale = wx.Locale(wx.LANGUAGE_ENGLISH_US, wx.LOCALE_LOAD_DEFAULT | wx.LOCALE_CONV_ENCODING)
		self.locale.AddCatalogLookupPathPrefix('locale')
		self.locale.AddCatalog('ireon_launcher')


def initGettext():
#    import locale
#    locale.setlocale(locale.LC_ALL, 'ru')

	gettext.install('ireon_launcher', 'locale', unicode=True)
	presLan_ru_RU = gettext.translation("ireon_launcher", "locale", languages=['ru_RU'])
	presLan_ru_RU.install()
	presLan_en_US = gettext.translation("ireon_launcher", "locale", languages=['en_US'])
#    presLan_en_US.install()


def main():
	initGettext()
	app = MyApp()

	i = 0
	for servname, servaddr, servport in PINGS:
		pingthread = pingit(app, i, servaddr, servport)
		pingthread.start()
		i += 1

	app.MainLoop()

if __name__ == '__main__':
	main()
