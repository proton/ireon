#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include "common/common_stdafx.h"
#include "common/net/net_data.h"

struct testPacket: public PacketBase
{
	static const ireon::net::commandId commandId = 1;

	enum int_
	{
		int1, int2
	};

	enum uint_
	{
		uint1
	};

	enum bool_
	{
		bool1
	};

	enum byte_
	{
		byte1
	};

	enum ushort_
	{
		ushort1
	};

	enum string_
	{
		str1
	};
};

BOOST_AUTO_UNIT_TEST(simpleCNetDataTest)
{
	GENERATE_PACKET_TYPE(testPacket) data;

	data.set(testPacket::int1, 10);
	BOOST_CHECK(data.get(testPacket::int1) == 10);

	// test value overwrite
	data.set(testPacket::int2, 123);
	data.set(testPacket::int2, 123);
	BOOST_CHECK(data.get(testPacket::int2) == 123);
	data.set(testPacket::int2, data.get(testPacket::int2) - 1234 ); // 123 - 1234 = -1111
	BOOST_CHECK(data.get(testPacket::int2) == -1111);

	data.set(testPacket::uint1, 321);
	BOOST_CHECK(data.get(testPacket::uint1) == 321);

	data.set(testPacket::bool1, true);
	BOOST_CHECK( !data.get(testPacket::bool1) == false);

	data.set(testPacket::str1, "blablabla");
	BOOST_CHECK( data.get(testPacket::str1) == "blablabla");
	data.set(testPacket::str1, "פûגא");
	BOOST_CHECK( data.get(testPacket::str1) == "פûגא");
}


BOOST_AUTO_UNIT_TEST(testValueNotFound)
{
	GENERATE_PACKET_TYPE(testPacket) data;

	BOOST_CHECK_THROW(data.get(testPacket::int1), EItemNotFound);

	data.set(testPacket::int1, 10);
	BOOST_CHECK(data.get(testPacket::int1) == 10);
}

BOOST_AUTO_UNIT_TEST(testCNetDataSerialization)
{

	String s;
	{
		GENERATE_PACKET_TYPE(testPacket) data;
		s = data.serialize();

	}
	{
		GENERATE_PACKET_TYPE(testPacket) data;
		String t;
		BOOST_CHECK_THROW(data.unserialize(t), EUnexpectedDataEnd);
		data.unserialize(s);
	}

	s.clear();

	{
		GENERATE_PACKET_TYPE(testPacket) data;
		data.set(testPacket::int1, -65535);
		data.set(testPacket::int2, 5);
		data.set(testPacket::bool1, true);
		data.set(testPacket::str1, "truetruetruetrueפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגא");

		s = data.serialize();
	}

	{
		GENERATE_PACKET_TYPE(testPacket) data;
		data.set(testPacket::uint1, 2);
		data.unserialize(s);
		// unserialize modifies string (cuts unserialized part)
		BOOST_CHECK(s.size() == 0);
		// unserialization resets data
		BOOST_CHECK_THROW(data.get(testPacket::uint1), EItemNotFound);
		BOOST_CHECK(data.get(testPacket::int1) == -65535);
		BOOST_CHECK(data.get(testPacket::int2) == 5);
		BOOST_CHECK(data.get(testPacket::bool1) == true);
		BOOST_CHECK(data.get(testPacket::str1) == "truetruetruetrueפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגא");
	}
}

BOOST_AUTO_UNIT_TEST(testCNetDataManyTypesInOnePacket)
{
	String s;
	{
		GENERATE_PACKET_TYPE(testPacket) data;
		data.set(testPacket::int1, -65535);
		data.set(testPacket::int2, 5);
		data.set(testPacket::bool1, false);
		data.set(testPacket::byte1, 23);
		data.set(testPacket::ushort1, 30000);

		s = data.serialize();
	}
	{
		GENERATE_PACKET_TYPE(testPacket) data;
		data.unserialize(s);
		BOOST_CHECK(data.get(testPacket::int1) == -65535);
		BOOST_CHECK(data.get(testPacket::int2) == 5);
		BOOST_CHECK(data.get(testPacket::bool1) == false);
		BOOST_CHECK(data.get(testPacket::byte1) == 23);
		BOOST_CHECK(data.get(testPacket::ushort1) == 30000);
	}
}

BOOST_AUTO_UNIT_TEST(testCNetDataMultipleSerialization)
{
	String s;
	{
		GENERATE_PACKET_TYPE(testPacket) data;
		data.set(testPacket::int1, -65535);
		data.set(testPacket::int2, 5);
		data.set(testPacket::bool1, true);
		data.set(testPacket::str1, "truetruetruetrueפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגא");

		s = data.serialize();
	}
	{
		GENERATE_PACKET_TYPE(testPacket) data;
		data.set(testPacket::int2, 15);
		data.set(testPacket::bool1, false);
		data.set(testPacket::str1, "truetruetruetrue");

		s += data.serialize();
	}

	{
		GENERATE_PACKET_TYPE(testPacket) data;
		data.unserialize(s);
		BOOST_CHECK(data.get(testPacket::int1) == -65535);
		BOOST_CHECK(data.get(testPacket::int2) == 5);
		BOOST_CHECK(data.get(testPacket::bool1) == true);
		BOOST_CHECK(data.get(testPacket::str1) == "truetruetruetrueפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגאפûגא");
		data.unserialize(s);
		BOOST_CHECK_THROW(data.get(testPacket::int1), EItemNotFound);
		BOOST_CHECK(data.get(testPacket::int2) == 15);
		BOOST_CHECK(data.get(testPacket::bool1) == false);
		BOOST_CHECK(data.get(testPacket::str1) == "truetruetruetrue");
	}
}
