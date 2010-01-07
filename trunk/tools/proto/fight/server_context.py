#!/usr/bin/python

import context_base
import client_events

class server_context(context_base.context):
	def __init__ (self, app, conn, ip, client_id):
		context_base.context.__init__(self, conn, ip, client_id)
		self.app = app
		self.callbacks["players listing"] = self.display_players_listing
		self.callbacks["fight: result"] = self.display_fight_result
		self.callbacks["fight: fight finished"] = self.fight_finished
		self.callbacks["fight: fight started"] = self.fight_started
		self.callbacks["fight: fight initiated"] = self.pass_it
		self.callbacks["fight: initiate fight"] = self.intiate_fight_request
		self.callbacks["fight: error starting fight"] = self.fight_cancelled
		self.callbacks["fight: fight denied"] = self.fight_cancelled
		self.callbacks["fight: you miss your hit"] = self.you_miss
		self.callbacks["fight: enemy misses"] = self.enemy_misses
		self.callbacks["client id"] = self.got_id
		self.callbacks["login: done"] = self.pass_it
		self.callbacks["login: incorrect password"] = self.login_incorrect_password
		self.callbacks["fight: your attack"] = self.display_your_attack_results
		self.callbacks["fight: enemy attacks"] = self.display_enemy_attack_results

	def you_miss(self, data):
		self.app.AddPendingEvent(client_events.YouMissEvent())

	def enemy_misses(self, data):
		self.app.AddPendingEvent(client_events.EnemyMissesEvent())

	def fight_cancelled(self, data):
		self.app.AddPendingEvent(client_events.FightCancelledEvent())

	def got_id(self, data):
		if data.has_key('id'):
			self.app.SetClientId(data['id'])

	def login_incorrect_password(self, data):
		self.app.AddPendingEvent(client_events.IncorrectPasswordMessageEvent())

	def intiate_fight_request(self, data):
		if data.has_key('attacker'):
			self.app.AddPendingEvent(client_events.InitiateFightRequestEvent(data['attacker']))

	def display_your_attack_results(self, data):
		if data.has_key('result') and data.has_key('damage') and data.has_key('block') and data.has_key('your_stamina'):
			self.app.AddPendingEvent(client_events.PlayerAttacksEvent(data['damage'], data['block'], data['result'],data['your_stamina']))

	def display_enemy_attack_results(self, data):
		if data.has_key('result') and data.has_key('damage') and data.has_key('block'):
			self.app.AddPendingEvent(client_events.EnemyAttacksEvent(data['damage'], data['block'], data['result']))

	def display_players_listing(self, data):
		if data.has_key('list'):
			self.app.AddPendingEvent(client_events.PlayersListingUpdateEvent(data['list']))

	def display_fight_result(self, data):
		if data.has_key('result'):
			self.app.AddPendingEvent(client_events.FightResultEvent(data['result']))

	def fight_finished(self, data):
		self.app.AddPendingEvent(client_events.FightFinishedEvent())

	def fight_initiated(self, data):
		self.app.AddPendingEvent(client_events.FightInitiatedEvent())

	def fight_started(self, data):
		if data.has_key('enemy') and data.has_key('your_hp') and data.has_key('enemy_hp') and data.has_key('your_stamina'):
			self.app.AddPendingEvent(client_events.FightStartedEvent(data['enemy'],data['enemy_hp'],data['your_hp'],data['your_stamina']))

	def pass_it(self, data):
		pass


