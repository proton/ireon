/**
* @file common/net/net_data.h
* Network packets data container/serializer
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: net_data.h 828 2006-11-29 18:34:44Z zak $

*  See LICENSE for details
*/

#ifndef _NET_DATA_H_
#define _NET_DATA_H_

#include "common/common_stdafx.h"
#include "common/net/net_types.h"
#include <limits>

// ----------------------------------------------------------------

class ENetDataException{};
class EItemIndexOutOfRange : public ENetDataException{};
class EItemNotFound : public ENetDataException{};
class EUnknownSerializedDataType : public ENetDataException{};
class EUnexpectedDataEnd : public ENetDataException{};
class EIDDoesNotMatch : public ENetDataException{};

// ----------------------------------------------------------------

struct PacketBase
{
	enum bool_{};
	enum byte_{};
	enum ushort_{};
	enum int_{};
	enum uint_{};
	enum string_{};
	enum vector2_{};
};

#define GENERATE_PACKET_TYPE(packetStruct) CNetData< packetStruct, packetStruct::int_, \
	packetStruct::uint_, packetStruct::ushort_, packetStruct::bool_,\
	packetStruct::byte_, packetStruct::string_, packetStruct::vector2_ >

template <class PacketStruct, typename intEnum, typename uintEnum, typename ushortEnum, typename boolEnum, typename byteEnum, typename StringEnum, typename Vector2Enum> 
class CNetData
{

	template <class ValueType, class KeyType = byte>
	class CNetDataListItem
	{
	public:
		CNetDataListItem(CNetDataListItem<ValueType>* next = NULL): next_(next){}
		CNetDataListItem(KeyType key, ValueType val, CNetDataListItem<ValueType>* next):
		key_(key),val_(val),next_(next){}
		~CNetDataListItem(){if (getNext()) delete getNext();}

		KeyType& getKey() {return key_;}
		ValueType& getValue(){return val_;}
		void setValue(ValueType value) {val_ = value;}
		CNetDataListItem<ValueType>* getNext() {return next_;}
		void setNext(CNetDataListItem<ValueType>* next) {next_ = next;}

		typedef KeyType KeyType_;

	private:
		KeyType			key_;
		ValueType		val_;
		CNetDataListItem<ValueType>*	next_;
	};

	// ============================================================

	template <class ValueType>
	void putValue(int key, ValueType val, CNetDataListItem<ValueType> &listHead)
	{
		if (std::numeric_limits<CNetDataListItem<ValueType>::KeyType_>::max() < key)
			throw EItemIndexOutOfRange();
	
		CNetDataListItem<ValueType> *item;
		try
		{
			item = getItem<ValueType>(static_cast<CNetDataListItem<ValueType>::KeyType_> (key), listHead);
			item->setValue(val);
		}
		catch (EItemNotFound&)
		{
			item = new CNetDataListItem<ValueType>(static_cast<CNetDataListItem<ValueType>::KeyType_> (key), val, NULL);
			if (listHead.getNext())
				item->setNext(listHead.getNext());
			listHead.setNext(item);
		}
	}

	// ---------------------------------------

	template <class ValueType>
	ValueType& getValue(int key, CNetDataListItem<ValueType> &listHead)
	{
		if (std::numeric_limits<CNetDataListItem<ValueType>::KeyType_>::max() < key)
			throw EItemIndexOutOfRange();

		return getItem(static_cast<CNetDataListItem<ValueType>::KeyType_> (key), listHead)->getValue();
	}

	// ---------------------------------------

	template <class ValueType>
	CNetDataListItem<ValueType>* getItem(int key, CNetDataListItem<ValueType> &listHead)
	{
		CNetDataListItem<ValueType> *curItem = listHead.getNext();
		while (curItem != NULL)
			if (curItem->getKey() == key)
				return curItem;
			else
				curItem = curItem->getNext();
		throw EItemNotFound();
	}

	// ---------------------------------------

	template <class ValueType>
	void serializePlainType(String &s, CNetDataListItem<ValueType> &listHead, char typeId)
	{
		CNetDataListItem<ValueType> *curItem = listHead.getNext();
		while (curItem != NULL)
		{
			s.append(&typeId, sizeof(typeId));
			s.append((char*) &(curItem->getKey()), sizeof(CNetDataListItem<ValueType>::KeyType_));
			s.append((char*) &(curItem->getValue()), sizeof(ValueType));
			curItem = curItem->getNext();
		}
	};

	// ---------------------------------------

	void serializeStrings(String &s, CNetDataListItem<String> &listHead, char typeId)
	{
		CNetDataListItem<String> *curItem = listHead.getNext();
		size_t size;
		while (curItem != NULL)
		{
			size = curItem->getValue().size();
			s.append(&typeId, sizeof(typeId));
			s.append((char*) &(curItem->getKey()), sizeof(CNetDataListItem<String>::KeyType_));
			s.append((char*) &size, sizeof(size));
			s.append(curItem->getValue().c_str(), size);
			curItem = curItem->getNext();
		}
	}

	// ---------------------------------------

	void serializeVectors(String &s, CNetDataListItem<Vector2> &listHead, char typeId)
	{
		CNetDataListItem<Vector2> *curItem = listHead.getNext();
		while (curItem != NULL)
		{
			s.append(&typeId, sizeof(typeId));
			s.append((char*) &(curItem->getKey()), sizeof(CNetDataListItem<Vector2>::KeyType_));
			s.append((char*) &(curItem->getValue().x), sizeof(Vector2::UNIT_X));
			s.append((char*) &(curItem->getValue().y), sizeof(Vector2::UNIT_Y));
			curItem = curItem->getNext();
		}
	}

	// ---------------------------------------

	template <class ValueType>
	void unserializeNextItem(char *&curSource, const char *sourceEnd, 
		CNetDataListItem<ValueType> &listHead)
	{
		if (curSource + sizeof(CNetDataListItem<ValueType>::KeyType_) + sizeof(ValueType) > sourceEnd)
			throw EUnexpectedDataEnd();

		CNetDataListItem<ValueType>::KeyType_ key;
		memcpy(&key,curSource,sizeof(key));
		curSource += sizeof(key);

		ValueType val;
		memcpy(&val,curSource,sizeof(ValueType));
		putValue<ValueType>(key,val,listHead);
		curSource += sizeof(ValueType);
	}

	// ---------------------------------------

	void unserializeStringItem(char *&curSource, const char *sourceEnd, 
		CNetDataListItem<String> &listHead)
	{
		if (curSource + sizeof(CNetDataListItem<String>::KeyType_) + sizeof(size_t) > sourceEnd)
			throw EUnexpectedDataEnd();

		CNetDataListItem<String>::KeyType_ key;
		memcpy(&key,curSource,sizeof(key));
		curSource += sizeof(key);

		size_t size;
		memcpy(&size,curSource,sizeof(size));
		curSource += sizeof(size);

		if (curSource + size > sourceEnd)
			throw EUnexpectedDataEnd();

		String val;
		val.append(curSource,size);
		putValue<String>(key,val,listHead);
		curSource += size;
	}

	// ---------------------------------------

	void unserializeVector2Item(char *&curSource, const char *sourceEnd, 
		CNetDataListItem<Vector2> &listHead)
	{
		if (curSource + sizeof(CNetDataListItem<Vector2>::KeyType_) + sizeof(Vector2::UNIT_X) + sizeof(Vector2::UNIT_Y) > sourceEnd)
			throw EUnexpectedDataEnd();

		CNetDataListItem<Vector2>::KeyType_ key;
		memcpy(&key,curSource,sizeof(key));
		curSource += sizeof(key);

		Vector2 val;
		memcpy(&val.x,curSource,sizeof(Vector2::UNIT_X));
		curSource += sizeof(Vector2::UNIT_X);
		memcpy(&val.y,curSource,sizeof(Vector2::UNIT_Y));
		curSource += sizeof(Vector2::UNIT_Y);
		putValue<Vector2>(key,val,listHead);
	}

	// ---------------------------------------

	CNetDataListItem<bool>	m_boolList;
	CNetDataListItem<byte>	m_byteList;
	CNetDataListItem<ushort>	m_ushortList;
	CNetDataListItem<int>	m_intList;
	CNetDataListItem<uint>	m_uintList;
	CNetDataListItem<String>	m_StringList;
	CNetDataListItem<Vector2>	m_Vector2List;

	// ---------------------------------------

	void reset()
	{
		if (m_boolList.getNext())
		{
			delete m_boolList.getNext();
			m_boolList.setNext(NULL);
		}
		if (m_byteList.getNext())
		{
			delete m_byteList.getNext();
			m_byteList.setNext(NULL);
		}
		if (m_ushortList.getNext())
		{
			delete m_ushortList.getNext();
			m_ushortList.setNext(NULL);
		}
		if (m_intList.getNext())
		{
			delete m_intList.getNext();
			m_intList.setNext(NULL);
		}
		if (m_uintList.getNext())
		{
			delete m_uintList.getNext();
			m_uintList.setNext(NULL);
		}
		if (m_StringList.getNext())
		{
			delete m_StringList.getNext();
			m_StringList.setNext(NULL);
		}
		if (m_Vector2List.getNext())
		{
			delete m_Vector2List.getNext();
			m_Vector2List.setNext(NULL);
		}
	}

public:

	const ireon::net::commandId& getCommandId() { return PacketStruct.commandId; }


	String serialize()
	{
		String s;
		s.append((char *) &(getCommandId()), sizeof(ireon::net::commandId));
		serializePlainType<bool>(s, m_boolList, 'b');
		serializePlainType<byte>(s, m_byteList, 'y');
		serializePlainType<ushort>(s, m_ushortList, 'h');
		serializePlainType<int>(s, m_intList, 'i');
		serializePlainType<uint>(s, m_uintList, 'u');
		serializeStrings(s, m_StringList, 's');
		serializeVectors(s, m_Vector2List, 'v');
		s.push_back('.');
		return s;
	};

	// ---------------------------------------

	/// restores data from string buffer, __MODIFIES__INPUT__STRING__!
	void unserialize(String &s)
	{
		if (s.length() < 2)
			throw EUnexpectedDataEnd();

		char *ptr = const_cast<char *> (s.data());
		if (*((ireon::net::commandId *) (ptr)) != getCommandId())
			throw EIDDoesNotMatch();
		ptr += sizeof(ireon::net::commandId);
		const char *dataEnd = s.data() + s.length();

		reset();

		while ( ptr + sizeof(char) < dataEnd )
		{
			switch(*ptr)
			{
			case 'i':
				unserializeNextItem<int>(++ptr, dataEnd, m_intList);
				break;
			case 'b':
				unserializeNextItem<bool>(++ptr, dataEnd, m_boolList);
				break;
			case 'y':
				unserializeNextItem<byte>(++ptr, dataEnd, m_byteList);
				break;
			case 'u':
				unserializeNextItem<uint>(++ptr, dataEnd, m_uintList);
				break;
			case 'h':
				unserializeNextItem<ushort>(++ptr, dataEnd, m_ushortList);
				break;
			case 's':
				unserializeStringItem(++ptr, dataEnd, m_StringList);
				break;
			case 'v':
				unserializeVector2Item(++ptr, dataEnd, m_Vector2List);
				break;
			case '.':
				break;
			default:
				throw EUnknownSerializedDataType();
				break;
			}
			if ( *ptr == '.' )
				break;
		}

		++ptr;
		if ( ptr <= dataEnd )
		{

			String temp(ptr, dataEnd - ptr);
			s.swap(temp);
		}
		else
			throw EUnexpectedDataEnd();
	};

#define GENERATE_GETTERS_AND_SETTERS(typename_) typename_ get(##typename_##Enum key) { return getValue<typename_>(key, m_##typename_##List); } \
	void set(##typename_##Enum key, const typename_& val) { putValue<typename_>(key, val, m_##typename_##List); }
		GENERATE_GETTERS_AND_SETTERS(bool)
		GENERATE_GETTERS_AND_SETTERS(byte)
		GENERATE_GETTERS_AND_SETTERS(ushort)
		GENERATE_GETTERS_AND_SETTERS(int)
		GENERATE_GETTERS_AND_SETTERS(uint)
		GENERATE_GETTERS_AND_SETTERS(String)
		GENERATE_GETTERS_AND_SETTERS(Vector2)
#undef GENERATE_GETTERS_AND_SETTERS
};


#endif
