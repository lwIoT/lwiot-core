/*
 * NTP client implementation.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/network/udpclient.h>
#include <lwiot/network/ipaddress.h>
#include <lwiot/network/ntpclient.h>

#include <lwiot/stl/referencewrapper.h>

namespace lwiot
{
	NtpClient::NtpClient() : _current_epoch(0UL), _last_update(0UL)
	{
	}

	NtpClient::NtpClient(lwiot::UdpClient &client) :
		_client(client), _current_epoch(0ULL), _last_update(0ULL)
	{
	}

	void NtpClient::begin(lwiot::UdpClient &client)
	{
		this->_client = client;
		this->begin();
	}

	void NtpClient::begin()
	{
		this->_client->begin();
		this->_client->setTimeout(2);
	}

	void NtpClient::sendRequest() const
	{
		ByteBuffer buffer(NtpClient::NTP_PACKET_SIZE, true);

		buffer[0] = 0b11100011;
		buffer[1] = 0;
		buffer[2] = 6;
		buffer[3] = 0xEC;

		/* Bytes 4 through 11 are zero. Initialised by the constructor */
		buffer[12]  = 49;
		buffer[13]  = 0x4E;
		buffer[14]  = 49;
		buffer[15] = 52;

		this->_client->write(buffer.data(), NtpClient::NTP_PACKET_SIZE);
	}

#define merge(h, l) ((h << 8) | l)

#define UPDATE_DELAY 500
	bool NtpClient::update()
	{
		ByteBuffer buffer(NtpClient::NTP_PACKET_SIZE, true);
		uint32_t hi, lo, result;

		this->sendRequest();

		if(unlikely(this->_client->read(buffer.data(), buffer.count()) <= 0))
			return false;

		this->_last_update = lwiot_tick_ms() - UPDATE_DELAY;
		hi = merge(buffer[40], buffer[41]);
		lo = merge(buffer[42], buffer[43]);
		result = (hi << 16) | lo;
		this->_current_epoch = result - NtpClient::SEVENTY_YEARS;

		return true;
	}

	time_t NtpClient::time() const
	{
		time_t rv;

		rv = lwiot_tick_ms() - this->_last_update;
		rv /= 1000;
		rv += this->_current_epoch;
		return rv;
	}
}
