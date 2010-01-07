#!/usr/bin/python

import random
import math

class char:
	def __init__ (self, name, passwd):
		self.name = name
		self.passwd = passwd
		self.pvp_wins = 0
		self.pvp_losses = 0
		self.pve_wins = 0
		self.pve_losses = 0
		self.hp = [0,0,0]
		self.max_hp = [20, 50, 30]
		self.strength = 2
		self.stamina = 100
		self.max_stamina = 100
		self.combos = [(20,5,0,65),(5,25,5,75),(0,5,25,65)]
		self.reset()

	def reset(self):
		for i in range(len(self.hp)):
			self.hp[i] = self.max_hp[i]
		self.stamina = 100
		self.special_hit = -1


	def get_attack(self):
		BASE_ATTACK_POINTS_RANGE = 10
		# multiplier for each strength point
		ATTACK_STRENGTH_POINTS = 3

		attack_strength_bonus = ATTACK_STRENGTH_POINTS * self.get_strength()

		dmg = [0,0,0]
		if (self.special_hit >= 0) and (self.combos[self.special_hit][3] < self.stamina):
			for i in range(3):
				dmg[i] = self.combos[self.special_hit][i]
			self.calc_stamina(self.combos[self.special_hit][3])
		else:
			target_zone = int(random.gauss(1.5,0.4))
			if target_zone not in range(3):
				target_zone = 1

			dmg[target_zone] = \
						int((attack_strength_bonus + attack_strength_bonus * self.get_percent_hp()[0] \
						+ random.randint(0, int(BASE_ATTACK_POINTS_RANGE* self.get_percent_hp()[0]+1)))*self.get_percent_stamina() )
			self.calc_stamina(dmg[target_zone])
#		dmg[target_zone] = \
#					random.randint(1 + math.floor(attack_strength_bonus * attacker.char.get_percent_hp()[0]), \
#									1 + math.ceil(attack_maximum_const_part * (attacker.char.get_percent_hp()[0] + 1)))

		self.special_hit = -1
		return dmg


	def calc_stamina(self, hit_power):
		STAMINA_THRESHOLD = int(self.max_stamina/7)
		self.stamina -= hit_power - STAMINA_THRESHOLD
		if self.stamina > self.max_stamina:
			self.stamina = self.max_stamina
		if self.stamina < 0:
			self.stamina = 0


	def get_percent_stamina(self):
		return float(self.stamina)/self.max_stamina

	def get_abs_stamina(self):
		return self.stamina


	def queue_special_hit(self, combo_id):
		if (combo_id > 0) and (combo_id <= len(self.combos)):
			self.special_hit = combo_id-1


	def damage(self, dmg):
		MAX_BLOCK_PERCENT = 0.75
		block = []
		for i in range(len(dmg)):
			block.append(random.randint(0, math.ceil(MAX_BLOCK_PERCENT * float(dmg[i]) * self.get_percent_hp()[2])))
			self.hp[i] -= (dmg[i] - block[i])
			if self.hp[i]<0:
				self.hp[i] = 0
		return block


	def get_abs_hp(self):
#		return 'arms ' + str(self.hp[0]) + ', trunk ' + str(self.hp[1]) + ', legs ' + str(self.hp[2])
		return self.hp

	def get_percent_hp(self):
		ret = []
		for i in range(len(self.hp)):
			ret.append(float(self.hp[i]) / self.max_hp[i])
		return ret

	def is_alive(self):
		if self.hp[1] > 0:
			return 1
		else:
			return 0

	def get_strength(self):
		return self.strength


	def pvp_win(self):
		self.pvp_wins += 1

	def pvp_loss(self):
		self.pvp_losses += 1

	def pve_win(self):
		self.pve_wins += 1

	def pve_loss(self):
		self.pve_losses += 1

