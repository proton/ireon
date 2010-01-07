#!/usr/bin/python

from threading import Thread
import sys
sys.path = sys.path + ['../../common/py-modules/PHPSerialize']
import PHPUnserialize
import PHPSerialize

class context(Thread):
	def __init__ (self, conn, ip, client_id):
		Thread.__init__(self)
		self.working = 1
		self.context_type = 'generic'
		self.connection = conn
		self.ip = ip
		self.cid = client_id
		self.callbacks = { \
							"unknown command": self.unknown_command, \
							}
	def on_close(self):
		self.log('closing connection')
		self.connection.close()
		self.working = 0

	def disconnect(self):
		self.connection.close()
		self.working = 0

	def run(self):
		while self.working:
				data = self.connection.recv(2048)
#				try:
				if data:
					for msg in str(data).split('\n'):
						if msg:
							try:
								self.dispatch(PHPUnserialize.PHPUnserialize().unserialize(msg))
							except:
								pass
				else:
					self.on_close()
					return 0
#			except:
#				pass
#				self.log('connection timed out')
#				self.on_close()
#				return 0

	def log(self, msg):
		print str(self.cid) + ' (' + self.ip + '): ' + msg

	def send(self, data):
		try:
			self.connection.send(PHPSerialize.PHPSerialize().serialize(data)+'\n')
		except:
			self.log('net send exception')

	def dispatch(self, data):
		if data.has_key('cmd_id'):
			if self.callbacks.has_key(data['cmd_id']):
#				if callable(self.callbacks[data['cmd_id']]):
					self.callbacks[data['cmd_id']](data)
#				else:
#					self.log('callback for command <<' + data['cmd_id'] + '>> is not callable')
#					self.send({'cmd_id': 'error executing command'})
			else:
				self.log('unknown command ' + data['cmd_id'])
				self.send({'cmd_id': 'unknown command'})
		else:
			self.log('no command id in data packet')
			self.send({'cmd_id': 'no command id'})

	def unknown_command(self, data):
		self.log('got <<unknown command>> packet')
