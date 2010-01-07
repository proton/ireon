/* Copyright (C) 2005 ireon.org developers council
  * $Id$
 
  *  See LICENSE for details
  */
 
 /**
  * @file encrypting_layer.cpp
  * Encrypting layer 
  */
 #include "stdafx.h"
 #include "net/layers/encrypting_layer.h"
 
 void CEncryptingLayer::write(const CData& data)
 {
 	m_outputBuffer.push_back(data);
 }
 bool CEncryptingLayer::flush()
 {
 	std::vector<CData>::iterator i;
 	/// it is simple write all data to underlying layer
 	for (i = m_outputBuffer.begin(); i != m_outputBuffer.end(); i++)
 		m_nextLayer->write(*i);
 	m_outputBuffer.clear();
 	if (!m_nextLayer->flush())
 		return false;
 
 	return true;
 }
 
 bool CEncryptingLayer::read(std::vector<CData> &inputBuffer)
 {
 	///read input data from underlying layer
 	inputBuffer.clear();
 	std::vector<CData> input;
 	if (!m_nextLayer->read(input))
 		return false;
 	/// process input data
 	std::vector<CData>::iterator i;
 	for (i = input.begin();i!=input.end(); i++)
 		inputBuffer.push_back(*i);
 		
 	return true;
 
 };
 
 
 
