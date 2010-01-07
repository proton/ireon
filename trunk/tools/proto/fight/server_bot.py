#!/usr/bin/python

import client_context
import char

class bot_context(client_context.client_context):
	def __init__ (self, srv, conn, ip, client_id):
		client_context.client_context.__init__(self, srv, conn, ip, client_id)
		self.char = srv.get_account('bot' + str(self.cid),'')
		self.context_type = 'bot'
		self.ip = 'bot' + str(self.cid) # FIXME hack for log function

	def send(self, data):
		if data.has_key('cmd_id'):
			if (data['cmd_id'] == 'fight: initiate fight'):
				self.log('accepting fight')
				self.accept_fight({})
			else:
				self.log('skipping unknown command ' + data['cmd_id'])
		else:
			self.log('no command id')
