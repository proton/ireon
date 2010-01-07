#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""combat system prototype client"""

__author__ = "Dmitry Bitman (zak@ireon.org)"
__version__ = "$Id: fight_proto_client.pyw 664 2006-06-16 18:57:01Z zak $"
__date__ = "$Date: 2006-06-16 22:57:01 +0400 (Птн, 16 Июн 2006) $"
__copyright__ = "Copyright (C) 2006 ireon.org developers council "
__license__ = "See LICENSE for details"


import sys
sys.path = sys.path + ['../../common/py-modules/config-0.3.6']
import config
import socket

import wx
from wx import xrc
from wx.lib import ogl

import server_context
import client_events

DEFAULT_GUI_FILENAME = "combat_prototype.xrc"
DEFAULT_ICON_FILENAME = '..\\..\\common\\\images\\applications-prototype-16.ico'
DEFAULT_GUI_MAINFRAME_NAME = "CombatPrototypeFrame"

DEFAULT_SERVER_HOST = 'alpha.ireon.org'
DEFAULT_SERVER_PORT = 29200

class fight_proto_client(wx.App):
	def __init__(self):
		wx.App.__init__(self,redirect = True)

	def OnInit(self):
		self.InitI18N()
		self.client_id = 0
		self.dlg = 0
		self.configure()

		self.res = xrc.XmlResource(self.xrc_file)
		self.frame = self.res.LoadFrame(None, self.main_frame_name)
		image = wx.Image(self.icon_file, wx.BITMAP_TYPE_ANY)
		image = image.ConvertToBitmap()

		icon = wx.EmptyIcon()
		icon.CopyFromBitmap(image)

		self.frame.SetIcon(icon)

		dialog = wx.TextEntryDialog(None, "Enter your login (not necessary):", "Login Entry", "", style=wx.OK)
		self.login = ""
		self.passwd = ""
		if dialog.ShowModal() == wx.ID_OK:
			self.login = dialog.GetValue()
		dialog.Destroy()

		if self.login:
			dialog = wx.PasswordEntryDialog(None, "Enter your password (necessary!):", "Password Entry", "", style=wx.OK)
			if dialog.ShowModal() == wx.ID_OK:
				self.passwd =  dialog.GetValue()
			dialog.Destroy()

		try:
			socket.setdefaulttimeout(10)
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.sock.connect((self.host, self.port))
			self.context = server_context.server_context(self, self.sock, self.host, 1)
			self.context.start()
		except:
			dlg = wx.MessageDialog(self.frame, 'Could not connect to server'+' ('+self.host+':'+str(self.port)+')', 'Connection error', wx.OK | wx.ICON_EXCLAMATION )
			dlg.ShowModal()
			dlg.Destroy()
			return False

		self.context.send({'cmd_id': 'get client id'})

		if self.login and self.passwd:
			self.context.send({'cmd_id': 'login', 'login': self.login, 'passwd': self.passwd})

		self.players_listing = xrc.XRCCTRL(self.frame, "players_list")
		self.players_listing.InsertColumn(0, "id")
		self.players_listing.InsertColumn(1, "ip", wx.LIST_FORMAT_RIGHT)
		self.players_listing.InsertColumn(2, "name")
		self.players_listing.InsertColumn(3, "pvp w/l/a")
		self.players_listing.InsertColumn(4, "pve w/l/a")
		self.players_listing.InsertColumn(5, "fighting with")

		self.list_update_timer = wx.Timer(self)
		self.list_update_timer.Start(3000)
		self.Bind(wx.EVT_TIMER, self.OnSendPlayerListUpdateRequest,self.list_update_timer)
		self.OnSendPlayerListUpdateRequest(True)

		self.Bind(client_events.EVT_PLAYERS_LISTING_UPDATE, self.OnPlayersListingUpdate)
		self.Bind(wx.EVT_LEFT_DCLICK, self.OnPlayersListDoubleClick, self.players_listing)
		self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnPlayersListingItemSelected, self.players_listing)



		self.status_panel = xrc.XRCCTRL(self.frame, "status_panel")
		self.control_panel = xrc.XRCCTRL(self.frame, "control_panel")
		self.your_stats_panel = xrc.XRCCTRL(self.frame, "your_stats_panel")
		self.enemy_stats_panel = xrc.XRCCTRL(self.frame, "enemy_stats_panel")
#		self.your_hp_1.Destroy()
#		self.your_stats_sizer.DeleteWindows()

#        self.output = xrc.XRCCTRL(self.frame, "output_area")
#        self.OutputText(_("GMT time:")+' '+time.strftime("%c",time.gmtime())+"\n\n")
#
#        self.update_button = xrc.XRCCTRL(self.frame, "update_game_button")
#        self.Bind(wx.EVT_BUTTON, self.OnUpdateGame, self.update_button)
#
#        self.launch_button = xrc.XRCCTRL(self.frame, "launch_game_button")
#        self.Bind(wx.EVT_BUTTON, self.OnLaunchGame, self.launch_button)
#
#        self.Bind(EVT_PING_RESPONCE, self.OnPingResponceEvent)
#

		self.Bind(client_events.EVT_INCORRECT_PASSWORD_MESSAGE, self.OnIncorrectPasswordMessage)
		self.Bind(client_events.EVT_PLAYER_ATTACKS, self.OnPlayerAttack)
		self.Bind(client_events.EVT_ENEMY_ATTACKS, self.OnEnemyAttack)
		self.Bind(client_events.EVT_FIGHT_RESULT, self.OnFightResult)
		self.Bind(client_events.EVT_FIGHT_FINISHED, self.OnFightFinished)
		self.Bind(client_events.EVT_FIGHT_INITIATED, self.OnFightInitiated)
		self.Bind(client_events.EVT_INITIATE_FIGHT_REQUEST, self.OnFightRequest)
		self.Bind(client_events.EVT_FIGHT_STARTED, self.OnFightStarted)
		self.Bind(client_events.EVT_FIGHT_CANCELLED, self.OnFightCancelled)
		self.Bind(client_events.EVT_YOU_MISS, self.OnYouMiss)
		self.Bind(client_events.EVT_ENEMY_MISSES, self.OnEnemyMisses)

		self.button1 = xrc.XRCCTRL(self.frame, "button_1")
		self.Bind(wx.EVT_BUTTON, self.OnCombo1, self.button1)
		self.button2 = xrc.XRCCTRL(self.frame, "button_2")
		self.Bind(wx.EVT_BUTTON, self.OnCombo2, self.button2)
		self.button3 = xrc.XRCCTRL(self.frame, "button_3")
		self.Bind(wx.EVT_BUTTON, self.OnCombo3, self.button3)

		self.fighting = 0
		self.ResetStats()

		self.frame.Show()
		self.SetTopWindow(self.frame)
		return True


	def configure(self):
		try:
			cfg = config.Config(file('fight_proto_client.cfg'))
			self.host = cfg.get('srv_host',DEFAULT_SERVER_HOST)
			self.port = cfg.get('srv_port',DEFAULT_SERVER_PORT)
			self.xrc_file = cfg.get('xrc_file', DEFAULT_GUI_FILENAME)
			self.icon_file = cfg.get('icon_file', DEFAULT_ICON_FILENAME)
			self.main_frame_name = cfg.get('main_frame_name', DEFAULT_GUI_MAINFRAME_NAME)
		except:
			self.host = DEFAULT_SERVER_HOST
			self.port = DEFAULT_SERVER_PORT
			self.xrc_file = DEFAULT_GUI_FILENAME
			self.icon_file = DEFAULT_ICON_FILENAME
			self.main_frame_name = DEFAULT_GUI_MAINFRAME_NAME


	def SetClientId(self, client_id):
		self.client_id = client_id

	def SetStatus(self, status, highlight=0):
		self.status_text_string = status
		self.highlight_status = highlight
		try:
			self.status_text.Destroy()
		except:
			pass

		self.status_text = wx.StaticText(self.status_panel, -1, 'status: ' + str(self.status_text_string), (5, 5))
		if self.highlight_status:
			self.status_text.SetBackgroundColour('Yellow')


	def OnIncorrectPasswordMessage(self, evt):
		dlg = wx.MessageDialog(self.frame, 'You supplied incorrect password. User name was reset to default.', 'Incorrect password', wx.OK | wx.ICON_EXCLAMATION )
		dlg.ShowModal()
		dlg.Destroy()


	def OnSendPlayerListUpdateRequest(self, evt):
		self.context.send({'cmd_id': 'list players'})

	def GetPlayerNameById(self, player_id):
		index = self.players_listing.FindItem(-1,str(player_id))
		if index == -1:
			return 'n/a'
		else:
			return self.players_listing.GetItem(index,2).GetText()

	def OnPlayersListingUpdate(self, evt):
		fighting = []
		self.players_listing.DeleteAllItems()
		for n, item in evt.list.iteritems():
			index = self.players_listing.InsertStringItem(0, str(item[0]))
			self.players_listing.SetStringItem(index, 1, str(item[1]))
			self.players_listing.SetStringItem(index, 2, str(item[2]))
			self.players_listing.SetStringItem(index, 3, str(item[3]) + '/' + str(item[4]) + '/' + str(item[3]+item[4]))
			self.players_listing.SetStringItem(index, 4, str(item[5]) + '/' + str(item[6]) + '/' + str(item[5]+item[6]))
			if item[7] >= 0:
				fighting.append((item[0], item[7]))
				it = self.players_listing.GetItem(index)
				it.SetTextColour(wx.RED)
				self.players_listing.SetItem(it)
			else:
				self.players_listing.SetStringItem(index, 5, '-')
			if item[0] == self.client_id:
				it = self.players_listing.GetItem(index)
				font = it.GetFont()
				font.SetWeight(wx.FONTWEIGHT_BOLD)
				it.SetFont(font)
				self.players_listing.SetItem(it)

		for fight in fighting:
			index = self.players_listing.FindItem(-1,str(fight[0]))
			if index >=0:
				self.players_listing.SetStringItem(index, 5, self.GetPlayerNameById(fight[1])+' ('+str(fight[1])+')')


	def OnPlayersListingItemSelected(self, evt):
		self.players_listing.currentItem = evt.m_itemIndex
		evt.Skip()


	def OnPlayersListDoubleClick(self, evt):
		try:
			self.players_listing.currentItem
			self.players_listing.Disable()
			self.StartFight(self.players_listing.GetItemText(self.players_listing.currentItem))
		except:
			pass
		evt.Skip()

	def RedrawStats(self):
		try:
			for s in self.st:
				s.Destroy()
		except:
			pass

		self.st = []

		enemy_name = self.GetPlayerNameById(self.enemy_id)
		self.st.append(wx.StaticText(self.your_stats_panel, -1, "Your stats", (5, 5)))
		self.st.append(wx.StaticText(self.enemy_stats_panel, -1, "Enemy stats ("+enemy_name+')', (5, 5)))

		stat_max_width  = 140
		stat_left_x = 90
		stat_y_offset = 7

		stats_info = ((20, "GREEN",0,'arms: '),(35, "RED",1,'trunk: '),(50, "GREEN",2,'legs: '))
		for context in ((self.your_stats_panel,self.your_stats,self.your_damage,self.your_block,self.your_max_stat), (self.enemy_stats_panel,self.enemy_stats,self.enemy_damage,self.enemy_block,self.enemy_max_stat)):
			dc = wx.ClientDC(context[0])
			dc.Clear()
			for item in stats_info:
				dc.SetPen(wx.Pen('BLUE',1))
				dc.DrawLine(stat_left_x+stat_max_width,item[0]+stat_y_offset-6,stat_left_x+stat_max_width-int(stat_max_width*context[2][item[2]]/context[4]),item[0]+stat_y_offset-6)
				dc.SetPen(wx.Pen('ORANGE',1))
				dc.DrawLine(stat_left_x+stat_max_width,item[0]+stat_y_offset-4,stat_left_x+stat_max_width-int(stat_max_width*context[3][item[2]]/context[4]),item[0]+stat_y_offset-4)
				dc.SetPen(wx.Pen(item[1],2))
				dc.DrawLine(stat_left_x,item[0]+stat_y_offset,stat_left_x+int(stat_max_width*context[1][item[2]]/context[4]),item[0]+stat_y_offset)
				self.st.append(wx.StaticText(context[0], -1, item[3] + str(context[1][item[2]]), (15, item[0])))
		self.st.append(wx.StaticText(self.your_stats_panel, -1, 'stamina: '+str(self.your_stamina), (15, 65)))
		dc = wx.ClientDC(self.your_stats_panel)
		dc.SetPen(wx.Pen('BLUE',1))
		dc.DrawLine(stat_left_x,65+stat_y_offset,stat_left_x+int(stat_max_width*self.your_stamina/self.your_max_stamina),65+stat_y_offset)


	def ResetStats(self):
		self.your_stats = [0,0,0]
		self.your_block = [0,0,0]
		self.your_damage = [0,0,0]
		self.enemy_stats = [0,0,0]
		self.enemy_block = [0,0,0]
		self.enemy_damage = [0,0,0]
		self.your_stamina = 100
		self.enemy_id = 0
		self.your_max_stat = [0,0,0]
		self.enemy_max_stat = [0,0,0]


	def StartFight(self, enemy_id):
		try:
			self.fight_result_text.Destroy()
			self.fight_init_timeout_timer.Stop()
		except:
			pass

		self.fight_init_timeout_timer = wx.FutureCall(10000, self.OnFightInitTimeout,self)

		self.fighting = 1
		self.SetStatus('sending fight query')
		self.context.send({'cmd_id': 'fight', 'enemy_id': int(enemy_id)})

	def OnFightInitTimeout(self, evt):
		self.fighting = 0
		self.players_listing.Enable()
		self.SetStatus('fight query timeout',1)
		self.context.send({'cmd_id': 'fight: init timeout'})

	def OnFightInitiated(self, evt):
		self.SetStatus('sending fight request to enemy')


	def OnFightRequest(self, evt):
		if self.fighting:
			self.context.send({'cmd_id': 'fight: deny'})
			return
		if not self.dlg:
			self.dlg = wx.MessageDialog(self.frame, 'Start fight with '+self.GetPlayerNameById(evt.attacker)+'?', 'Fight request', wx.YES_NO | wx.NO_DEFAULT | wx.ICON_QUESTION )
			res = self.dlg.ShowModal()
			self.dlg.Destroy()
			self.dlg = 0
			if res == wx.ID_YES:
				try:
					self.fight_result_text.Destroy()
				except:
					pass

				self.fighting = 1
				self.SetStatus('sending fight query')
				self.players_listing.Disable()
				self.context.send({'cmd_id': 'fight: accept'})
			else:
				self.context.send({'cmd_id': 'fight: deny'})



	def OnFightStarted(self, evt):
		try:
			self.fight_init_timeout_timer.Stop()
		except:
			pass
		self.ResetStats()
		self.enemy_id = evt.enemy_id
		self.your_stats = evt.your_hp.values()
		self.enemy_stats = evt.enemy_hp.values()
		self.your_stamina = evt.your_stamina
		self.your_max_stamina = self.your_stamina
		t = []
		t.extend(self.your_stats)
		t.sort()
		self.your_max_stat = t[len(t)-1]
		t = []
		t.extend(self.enemy_stats)
		t.sort()
		self.enemy_max_stat = t[len(t)-1]
		self.RedrawStats()
		self.SetStatus('fight started!')
		self.button1.Enable()
		self.button2.Enable()
		self.button3.Enable()


	def OnFightCancelled(self, evt):
		try:
			self.fight_init_timeout_timer.Stop()
		except:
			pass
		self.fighting = 0
		self.players_listing.Enable()
		self.ResetStats()
		self.SetStatus('fight cancelled!',1)


	def OnFightFinished(self, evt):
		self.fighting = 0
		self.button1.Disable()
		self.button2.Disable()
		self.button3.Disable()
		self.players_listing.Enable()
		self.ResetStats()


	def OnYouMiss(self,evt):
		self.SetStatus("you miss your hit!")

	def OnEnemyMisses(self,evt):
		self.SetStatus("enemy misses his hit")

	def OnPlayerAttack(self, evt):
		self.SetStatus("you attack")
		self.enemy_stats = evt.result.values()
		self.enemy_damage = evt.damage.values()
		self.enemy_block = evt.block.values()
		self.your_stamina = evt.your_stamina
#		for i, val in evt.result.iteritems():
#			self.enemy_stats[int(i)] = val
#		for i, val in evt.damage.iteritems():
#			self.enemy_damage[int(i)] = val
#		for i, val in evt.block.iteritems():
#			self.enemy_block[int(i)] = val
		self.RedrawStats()


	def OnEnemyAttack(self, evt):
		self.SetStatus("enemy attacks")
		self.your_stats = evt.result.values()
		self.your_damage = evt.damage.values()
		self.your_block = evt.block.values()
#		for i, val in evt.result.iteritems():
#			self.your_stats[int(i)] = val
#		for i, val in evt.damage.iteritems():
#			self.your_damage[int(i)] = val
#		for i, val in evt.block.iteritems():
#			self.your_block[int(i)] = val
		self.RedrawStats()


	def OnFightResult(self, evt):
		if evt.result == 'win':
			self.SetStatus("you won!",1)
		elif evt.result == 'lost':
			self.SetStatus("you lost.",1)

	def OnCombo1(self,evt):
		self.context.send({'cmd_id': 'fight: special hit', 'id': 1})

	def OnCombo2(self,evt):
		self.context.send({'cmd_id': 'fight: special hit', 'id': 2})

	def OnCombo3(self,evt):
		self.context.send({'cmd_id': 'fight: special hit', 'id': 3})


	def InitI18N(self):
#        self.locale = wx.Locale(wx.LANGUAGE_RUSSIAN, wx.LOCALE_LOAD_DEFAULT | wx.LOCALE_CONV_ENCODING)
##        self.locale = wx.Locale(wx.LANGUAGE_ENGLISH_US, wx.LOCALE_LOAD_DEFAULT | wx.LOCALE_CONV_ENCODING)
#        self.locale.AddCatalogLookupPathPrefix('locale')
#        self.locale.AddCatalog('ireon_launcher')
		pass



def initGettext():
#    gettext.install('combat_system_prototype_client', 'locale', unicode=True)
#    presLan_ru_RU = gettext.translation("combat_system_prototype_client", "locale", languages=['ru_RU'])
#    presLan_ru_RU.install()
#    presLan_en_US = gettext.translation("combat_system_prototype_client", "locale", languages=['en_US'])
	pass

def main():
	initGettext()
	cnt = fight_proto_client()
	cnt.MainLoop()
	cnt.context.send({'cmd_id': 'bye'})
	cnt.context.disconnect()

if __name__ == '__main__':
	main()
