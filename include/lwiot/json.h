/*
 * ArduinoJson wrapper header.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <ArduinoJson.h>

namespace lwiot
{
	template<size_t T>
	using StaticJsonBuffer = ArduinoJson::StaticJsonBuffer<T>;

	typedef ArduinoJson::JsonArray JsonArray;
	typedef ArduinoJson::JsonObject JsonObject;
	typedef ArduinoJson::DynamicJsonBuffer DynamicJsonBuffer;
}
