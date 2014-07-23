#ifndef NETWORKPACKET_H_INCLUDED
#define NETWORKPACKET_H_INCLUDED

#include <stdint.h>

class NetworkPacket {
	public:
		/// Liste der Netzwerk Paket Header
		enum PacketHeader {
			REQUEST_MAP = 0x01,
			REQUEST_WAYMAP = 0x02,
			REQUEST_PATH = 0x03,
			REQUEST_CHASSIS_LIST = 0x04,
			CLIENT_SPAWN_UNIT = 0x05,
			REQUEST_UNITS = 0x06,
			CLIENT_SEND_UNIT_TO = 0x07,
			REQUEST_PROTOCOL_VERSION = 0xFF
		};

		static const uint32_t PROTOCOL_VERSION = 1;

		static const uint32_t PROTOCOL_NEGOTIATION_HEADER = 0xABCDEF01;

};


#endif // NETWORKPACKET_H_INCLUDED
