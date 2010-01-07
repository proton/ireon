#!/usr/bin/python

# simple server


import socket
import client_context
import server_bot

import char
import fight

SERVER_PORT = 29200
SERVER_MAX_CONNECTIONS = 10

class fight_proto_server(object):
	def __init__(self):
		self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		#serversocket.bind((socket.gethostname(), SERVER_PORT))
		self.serversocket.bind(('', SERVER_PORT))
		self.serversocket.listen(SERVER_MAX_CONNECTIONS)

		self.accounts = {}
		self.clients = []
		self.fights = []
		self.client_id = 1

	def run(self):
		print 'starting bots'
		for i in range(3):
			context = server_bot.bot_context(self, 0, 0, self.client_id)
			self.clients.append(context);
			print 'accepted bot ' + str(self.client_id)
			self.client_id += 1

		print 'ready.'
		while 1:
			connection, address = self.serversocket.accept()
			self.rem_dead_clients()
			client_ip = address[0]
			context = client_context.client_context(self, connection, client_ip, self.client_id)
			self.clients.append(context);
			print 'accepted ' + str(self.client_id) + ' from ' + client_ip
			print 'total live connections: ' + str(len(self.clients))
			context.start()
			self.client_id += 1

	def rem_dead_clients(self):
		for thread in self.clients:
			if not thread.working or (not thread.isAlive() and (thread.context_type != 'bot')):
				print 'removing context ' + str(thread.cid) + ' (' + str(thread.ip) + ')'
				self.clients.remove(thread)
				del thread

		for thread in self.fights:
			if not thread.isAlive():
				self.fights.remove(thread)
				del thread

	def get_account(self, login, passwd):
		if self.accounts.has_key(login):
			if self.accounts[login].passwd == passwd:
				return self.accounts[login]
			else:
				return 0
		else:
			self.accounts[login] = char.char(login, passwd)
			return self.accounts[login]

	def get_players_list(self):
		plist = []
		for cont in self.clients:
			enemy = -1
			if cont.fight:
				enemy = cont.fight.cont1.cid + cont.fight.cont2.cid - cont.cid
			plist.append((cont.cid,cont.ip,cont.char.name,cont.char.pvp_wins,cont.char.pvp_losses,cont.char.pve_wins,cont.char.pve_losses,enemy))
		return plist


	def find_context_by_id(self, context_id):
		for cont in self.clients:
			if (cont.cid == context_id):
				return cont
		return 0

	def start_fight(self, id1, id2):
		c1 = self.find_context_by_id(id1)
		c2 = self.find_context_by_id(id2)
		if c1 and c2 and (c1 != c2) and not c1.fight and not c2.fight:
			f = fight.fight(c1,c2)
			self.fights.append(f)
			return 1
		else:
			return 0

def main():
	srv = fight_proto_server()
	try:
		srv.run()
	except KeyboardInterrupt:
		srv.serversocket.close()



if __name__ == '__main__':
	main()
