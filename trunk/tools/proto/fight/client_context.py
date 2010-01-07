#!/usr/bin/python

import context_base
import char

class client_context(context_base.context):
	def __init__ (self, srv, conn, ip, client_id):
		context_base.context.__init__(self, conn, ip, client_id)
		self.srv = srv
		self.fight = 0
		self.char = srv.get_account('player' + str(self.cid), '')
#		self.char = char.char('player' + str(self.cid))
		self.callbacks["list players"] = self.send_players_list
		self.callbacks["get client id"] = self.on_get_id
		self.callbacks["login"] = self.login
		self.callbacks["fight"] = self.init_fight
		self.callbacks["fight: accept"] = self.accept_fight
		self.callbacks["fight: deny"] = self.deny_fight
		self.callbacks["fight: init timeout"] = self.deny_fight
		self.callbacks["fight: special hit"] = self.fight_special_hit
		self.callbacks["bye"] = self.on_bye

	def on_close(self):
		self.log('closing connection')
		self.connection.close()
		self.working = 0
		self.srv.rem_dead_clients()

	def on_bye(self, data):
		self.on_close()

	def on_get_id(self, data):
		self.send({'cmd_id': 'client id', 'id': self.cid})

	def login(self, data):
		if data.has_key('login') and data.has_key('passwd'):
			char = self.srv.get_account(str(data['login']), str(data['passwd']))
			if char:
				self.char = char
				self.log('login as ' + str(data['login']))
				self.send({'cmd_id': 'login: done'})
			else:
				self.log('login as ' + str(data['login'])+' failed!')
				self.send({'cmd_id': 'login: incorrect password'})
		else:
			self.log('no player name or password provided')
			self.send({'cmd_id': 'login: incorrect'})

	def init_fight(self, data):
		if data.has_key('enemy_id'):
			enemy_id = data['enemy_id']
		else:
			enemy_id = 1

		if not self.fight and self.srv.start_fight(self.cid, enemy_id):
			self.send({'cmd_id': 'fight: fight initiated'})
		else:
			self.send({'cmd_id': 'fight: error starting fight'})


	def accept_fight(self, data):
		if self.fight and not self.fight.running:
			self.fight.start()
		else:
			self.send({'cmd_id': 'fight: error starting fight'})

	def deny_fight(self, data):
		if self.fight and not self.fight.running:
			self.fight.fight_denied()
		else:
			self.send({'cmd_id': 'fight: fight denied'})

	def fight_special_hit(self, data):
		if data.has_key('id'):
			self.char.queue_special_hit(data['id'])


	def send_players_list(self, data):
		self.send({'cmd_id': 'players listing', 'list': self.srv.get_players_list()})
