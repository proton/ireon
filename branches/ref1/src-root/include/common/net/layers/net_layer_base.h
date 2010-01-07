/**
 * @file common/net/layers/net_layer_base.h
 * abstract base network layer 
 */


/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */


#ifndef _NET_LAYER_BASE_H
#define _NET_LAYER_BASE_H

#include "common/common_stdafx.h"

class CNetLayerBase;
typedef boost::shared_ptr<CNetLayerBase>            CNetLayerBasePtr;

class CNetLayerBase
{
public:
	/// set underlying layer for output and input 
	void setUnderlyingLayer(CNetLayerBasePtr layer) { m_nextLayer = layer; }
	CNetLayerBase * getUnderlyingLayer() { return m_nextLayer.get(); }
	/// read from underlying layer
	virtual bool read(std::vector<CData> &inputBuffer)=0;
	/// write to output buffer
	virtual void write(const CData &data)=0;
	/// process output buffer to underlying layer
	virtual bool flush()=0;
	virtual String getInfo() {if (m_nextLayer.get()) return m_nextLayer->getInfo(); else return String("");}

protected:
	CNetLayerBase(): m_nextLayer(CNetLayerBasePtr((CNetLayerBase *) NULL)) {}
	CNetLayerBasePtr m_nextLayer;
};
#endif