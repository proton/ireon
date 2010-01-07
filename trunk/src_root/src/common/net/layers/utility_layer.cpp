/* Copyright (C) 2005 ireon.org developers council
  * $Id$
 
  *  See LICENSE for details
  */
 
 /**
  * @file utility_layer.cpp
  * Utility layer 
  */
 #include "stdafx.h"
 #include "net/layers/utility_layer.h"
 
 void CUtilityLayer::SetUnderlyingLayer(CNetLayerBase *layer)
 {
 	m_nextLayer = layer;
 };