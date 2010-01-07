#!/usr/bin/python

import wx

EVT_PLAYERS_LISTING_UPDATE_ID = wx.NewEventType()
EVT_PLAYERS_LISTING_UPDATE = wx.PyEventBinder(EVT_PLAYERS_LISTING_UPDATE_ID, 0)

EVT_INCORRECT_PASSWORD_MESSAGE_ID = wx.NewEventType()
EVT_INCORRECT_PASSWORD_MESSAGE = wx.PyEventBinder(EVT_INCORRECT_PASSWORD_MESSAGE_ID, 0)

EVT_PLAYER_ATTACKS_ID = wx.NewEventType()
EVT_PLAYER_ATTACKS = wx.PyEventBinder(EVT_PLAYER_ATTACKS_ID, 0)

EVT_ENEMY_ATTACKS_ID = wx.NewEventType()
EVT_ENEMY_ATTACKS = wx.PyEventBinder(EVT_ENEMY_ATTACKS_ID, 0)

EVT_FIGHT_RESULT_ID = wx.NewEventType()
EVT_FIGHT_RESULT = wx.PyEventBinder(EVT_FIGHT_RESULT_ID, 0)

EVT_FIGHT_FINISHED_ID = wx.NewEventType()
EVT_FIGHT_FINISHED = wx.PyEventBinder(EVT_FIGHT_FINISHED_ID, 0)

EVT_FIGHT_INITIATED_ID = wx.NewEventType()
EVT_FIGHT_INITIATED = wx.PyEventBinder(EVT_FIGHT_INITIATED_ID, 0)

EVT_INITIATE_FIGHT_REQUEST_ID = wx.NewEventType()
EVT_INITIATE_FIGHT_REQUEST = wx.PyEventBinder(EVT_INITIATE_FIGHT_REQUEST_ID, 0)

EVT_FIGHT_STARTED_ID = wx.NewEventType()
EVT_FIGHT_STARTED = wx.PyEventBinder(EVT_FIGHT_STARTED_ID, 0)

EVT_FIGHT_CANCELLED_ID = wx.NewEventType()
EVT_FIGHT_CANCELLED = wx.PyEventBinder(EVT_FIGHT_CANCELLED_ID, 0)

EVT_YOU_MISS_ID = wx.NewEventType()
EVT_YOU_MISS = wx.PyEventBinder(EVT_YOU_MISS_ID, 0)

EVT_ENEMY_MISSES_ID = wx.NewEventType()
EVT_ENEMY_MISSES = wx.PyEventBinder(EVT_ENEMY_MISSES_ID, 0)

class FightCancelledEvent(wx.PyEvent):
    def __init__(self):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_FIGHT_CANCELLED_ID)

class YouMissEvent(wx.PyEvent):
    def __init__(self):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_YOU_MISS_ID)

class EnemyMissesEvent(wx.PyEvent):
    def __init__(self):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_ENEMY_MISSES_ID)

class FightFinishedEvent(wx.PyEvent):
    def __init__(self):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_FIGHT_FINISHED_ID)

class FightStartedEvent(wx.PyEvent):
    def __init__(self, enemy_id, enemy_hp, your_hp, your_stamina):
        wx.PyEvent.__init__(self)
        self.enemy_id = enemy_id
        self.enemy_hp = enemy_hp
        self.your_hp = your_hp
        self.your_stamina = your_stamina
        self.SetEventType(EVT_FIGHT_STARTED_ID)

class FightInitiatedEvent(wx.PyEvent):
    def __init__(self):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_FIGHT_INITIATED_ID)

class IncorrectPasswordMessageEvent(wx.PyEvent):
    def __init__(self):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_INCORRECT_PASSWORD_MESSAGE_ID)

class InitiateFightRequestEvent(wx.PyEvent):
    def __init__(self, attacker):
        wx.PyEvent.__init__(self)
        self.attacker = attacker
        self.SetEventType(EVT_INITIATE_FIGHT_REQUEST_ID)

    def Clone(self):
        return InitiateFightRequestEvent(self.attacker)

class PlayersListingUpdateEvent(wx.PyEvent):
    def __init__(self, list):
        wx.PyEvent.__init__(self)
        self.list = list
        self.SetEventType(EVT_PLAYERS_LISTING_UPDATE_ID)

    def Clone(self):
        return PlayersListingUpdateEvent(self.list)

class FightResultEvent(wx.PyEvent):
    def __init__(self, result):
        wx.PyEvent.__init__(self)
        self.result = result
        self.SetEventType(EVT_FIGHT_RESULT_ID)

    def Clone(self):
        return FightResultEvent(self.result)

class PlayerAttacksEvent(wx.PyEvent):
    def __init__(self, damage, block, result, your_stamina):
        wx.PyEvent.__init__(self)
        self.damage = damage
        self.block = block
        self.result = result
        self.your_stamina = your_stamina
        self.SetEventType(EVT_PLAYER_ATTACKS_ID)

    def Clone(self):
        return PlayerAttacksEvent(self.damage, self.block, self.result)

class EnemyAttacksEvent(wx.PyEvent):
    def __init__(self, damage, block, result):
        wx.PyEvent.__init__(self)
        self.damage = damage
        self.block = block
        self.result = result
        self.SetEventType(EVT_ENEMY_ATTACKS_ID)

    def Clone(self):
        return EnemyAttacksEvent(self.damage, self.block, self.result)

