# -*- coding: utf-8 -*-
################################################################################
#                                 SETUP HANDLERS                               #
################################################################################

startState = StartState("StartState")

localState = LocalState("LocalState")
menuState = MenuState(CClientApp.AS_MENU)
connectingRootState = ConnectingRootState(CClientApp.AS_CONNECTING_ROOT)
checkingVersionState = CheckingVersionState(CClientApp.AS_CHECKING_VERSION)
loggingRootState = LoggingRootState(CClientApp.AS_LOGING_ROOT)
localEmptyState = EmptyState(CClientApp.AS_EMPTY)

rootServerState = RootServerState("RootServerState")
rootState = RootState(CClientApp.AS_ROOT)
connectingWorldState = ConnectingWorldState(CClientApp.AS_CONNECTING_WORLD)
loggingWorldState = LoggingWorldState(CClientApp.AS_LOGING_WORLD)
rootEmptyState = EmptyState(CClientApp.AS_EMPTY)

worldServerState = WorldServerState("WorldServerState")
playState = PlayState(CClientApp.AS_PLAY)
deadState = DeadState(CClientApp.AS_DEAD)


startState.registerState(localState)
startState.registerState(rootServerState)
startState.registerState(worldServerState)

localState.registerState(menuState)
localState.registerState(checkingVersionState)
localState.registerState(connectingRootState)
localState.registerState(loggingRootState)
localState.registerState(localEmptyState)

rootServerState.registerState( rootState)
rootServerState.registerState( connectingWorldState)
rootServerState.registerState( loggingWorldState)
rootServerState.registerState( rootEmptyState)

worldServerState.registerState( playState)
worldServerState.registerState( deadState)

