#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sstream>
#include <iostream>
#include <iomanip>

#include "flatbuffers\flatbuffers.h"

class GameMessage
{
public:
	// header의 허용 길이
	// HEADER_LENGTH 자리수까지 허용
	enum { HEADER_LENGTH = 8 };

	// header의 앞 4자리, body의 길이를 저장
	enum { SIZE_HEADER_LENGTH = 4 };

	// header의 뒤 4자리, body의 타입을 저장
	enum { TYPE_HEADER_LENGTH = 4 };

	// body의 허용 길이
	// body는 flatbuffer와 호환 가능한 buffer를 사용
	enum { MAX_BODY_LENGTH = 10000 };

	GameMessage()
		:
		_body_length(0),
		_type(0)
	{}

	explicit GameMessage(int type)
	{
		setBodyLength(0);
		setType(type);
		encodeHeader();
	}

	GameMessage(const flatbuffers::FlatBufferBuilder& builder, int type)
	{
		setBodyLength(builder.GetSize());
		setType(type);

		std::memcpy(getBody(), builder.GetBufferPointer(), _body_length);
		encodeHeader();	
	}

	const uint8_t* getData() const
	{
		return _data;
	}

	uint8_t* getData()
	{
		return _data;
	}

	size_t getLength() const
	{
		return HEADER_LENGTH + _body_length;
	}

	const uint8_t* getBody() const
	{
		return _data + HEADER_LENGTH;
	}

	uint8_t* getBody()
	{
		return _data + HEADER_LENGTH;
	}

	size_t getBodyLength() const
	{
		return _body_length;
	}

	size_t getType() const
	{
		return _type;
	}

	void setBodyLength(size_t new_length)
	{
		_body_length = new_length;
		if (_body_length > MAX_BODY_LENGTH)
			_body_length = MAX_BODY_LENGTH;
	}

	void setType(size_t new_type)
	{
		_type = new_type;
	}

	// buffer에는 데이터가 저장되지만, 얼마만큼의 길이가 저장되었는지 알 방법이 없다.
	// 그러므로 header와 body를 분리하고, header 부분에 길이를 저장해 준다.
	// 이 함수는 header의 데이터를 길이로 변환해 주는 함수이다.
	// 또한 데이터의 header와 body의 길이가 다른 경우는 이상이 있는 경우이고
	// 이를 return을 통해 알려준다.
	bool decodeHeader()
	{
		using namespace std; // For strncat and atoi.

		char size_header[SIZE_HEADER_LENGTH + 1];
		memcpy(size_header, _data, SIZE_HEADER_LENGTH);
		_body_length = atoi(size_header);

		char type_header[TYPE_HEADER_LENGTH + 1];
		memcpy(type_header, _data + SIZE_HEADER_LENGTH, TYPE_HEADER_LENGTH);
		_type = atoi(type_header);

		if (_body_length > MAX_BODY_LENGTH)
		{
			std::cout << "decide error" << std::endl;
			_body_length = 0;
			return false;
		}
		return true;
	}

	// 반대로 body의 길이만큼을 버퍼에 넣는 작업
	// size header와 type header의 길이만큼의 여백을 두기 위해 setw를 사용한다.
	void encodeHeader()
	{
		std::stringstream ss;

		ss << std::setw(SIZE_HEADER_LENGTH) << _body_length;
		ss << std::setw(TYPE_HEADER_LENGTH) << _type;

		memcpy(_data, ss.str().c_str(), HEADER_LENGTH);
	}

private:
	// 데이터의 몸체를 저장하는 변수
	uint8_t _data[HEADER_LENGTH + MAX_BODY_LENGTH];

	// 데이터의 길이를 저장하는 변수
	size_t _body_length;

	// 데이터의 타입을 저장하는 변수
	size_t _type;
};
