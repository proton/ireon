#!/usr/bin/python

from threading import Thread
import time
import random

RECALC_INTERVAL = 2

class fight(Thread):
	def __init__ (self, attacker, victim):
		Thread.__init__(self)
		random.seed()
		self.lastcalc = 0
		self.fight_iterations = 1
		self.running = 0
		self.cont1 = attacker
		self.cont2 = victim
		self.cont1.fight = self
		self.cont2.fight = self
		print 'fight: ' + str(self.cont1.cid) + ' -> ' + str(self.cont2.cid)
		self.cont2.send({'cmd_id': 'fight: initiate fight', 'attacker': self.cont1.cid})

	def send_to_both(self, data):
		self.cont1.send(data)
		self.cont2.send(data)

	def fight_denied(self):
		self.denied = 1
		self.cont1.fight = 0
		self.cont2.fight = 0
		self.send_to_both({'cmd_id': 'fight: fight denied'})



	def fight_round(self, iter):
		if iter % 2 == 1:
			attacker = self.cont1
			victim = self.cont2
		else:
			attacker = self.cont2
			victim = self.cont1

		# PROB_REV = 1 / probability of miss
		PROB_REV = 20

		if random.randint(0, PROB_REV-1) < 1:
			attacker.send({'cmd_id': 'fight: you miss your hit'})
			victim.send({'cmd_id': 'fight: enemy misses'})
			return victim.char.is_alive()


		dmg = attacker.char.get_attack()
		block = victim.char.damage(dmg)
		attacker.send({'cmd_id': 'fight: your attack', 'damage': dmg, 'result': victim.char.get_abs_hp(), 'block': block, 'your_stamina': attacker.char.get_abs_stamina()})
		victim.send({'cmd_id': 'fight: enemy attacks', 'damage': dmg, 'result': victim.char.get_abs_hp(), 'block': block})
		return victim.char.is_alive()



	def run(self):
		self.running = 1
		self.cont1.send({'cmd_id': 'fight: fight started', 'enemy': self.cont2.cid, 'enemy_hp': self.cont2.char.get_abs_hp(), 'your_hp': self.cont1.char.get_abs_hp(), 'your_stamina': self.cont1.char.get_abs_stamina()})
		self.cont2.send({'cmd_id': 'fight: fight started', 'enemy': self.cont1.cid, 'enemy_hp': self.cont1.char.get_abs_hp(), 'your_hp': self.cont2.char.get_abs_hp(), 'your_stamina': self.cont2.char.get_abs_stamina()})
		while (self.fight_iterations < 200):
			if (time.time() - RECALC_INTERVAL > self.lastcalc):
				self.lastcalc = time.time()
				if not self.fight_round(self.fight_iterations):
					break
				self.fight_iterations += 1
			time.sleep(1)

		if self.cont1.char.is_alive() and not self.cont2.char.is_alive():
			if self.cont2.context_type == 'bot':
				self.cont1.char.pve_win()
			else:
				self.cont1.char.pvp_win()
			if self.cont1.context_type == 'bot':
				self.cont2.char.pve_loss()
			else:
				self.cont2.char.pvp_loss()
			self.cont1.send({'cmd_id': 'fight: result', 'result': 'win'})
			self.cont2.send({'cmd_id': 'fight: result', 'result': 'lost'})
		elif not self.cont1.char.is_alive() and self.cont2.char.is_alive():
			if self.cont1.context_type == 'bot':
				self.cont2.char.pve_win()
			else:
				self.cont2.char.pvp_win()
			if self.cont2.context_type == 'bot':
				self.cont1.char.pve_loss()
			else:
				self.cont1.char.pvp_loss()
			self.cont1.send({'cmd_id': 'fight: result', 'result': 'lost'})
			self.cont2.send({'cmd_id': 'fight: result', 'result': 'win'})
		self.cont1.fight = 0
		self.cont2.fight = 0
		self.cont1.char.reset()
		self.cont2.char.reset()
		self.send_to_both({'cmd_id': 'fight: fight finished'})

