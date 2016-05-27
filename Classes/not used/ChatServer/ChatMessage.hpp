#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <sstream>
#include <iomanip>

class ChatMessage
{
public:
	// header의 허용 길이
	// HEADER_LENGTH 자리수까지 허용
	enum { HEADER_LENGTH = 4 };

	// body의 허용 길이
	// message의 허용 글자수
	enum { MAX_BODY_LENGTH = 1000 };

	ChatMessage()
		:
		_body_length(0)
	{
	}

	const char* data() const
	{
		return _data;
	}

	char* data()
	{
		return _data;
	}

	size_t length() const
	{
		return HEADER_LENGTH + _body_length;
	}

	const char* body() const
	{
		return _data + HEADER_LENGTH;
	}

	char* body()
	{
		return _data + HEADER_LENGTH;
	}

	size_t bodyLength() const
	{
		return _body_length;
	}

	void bodyLength(size_t new_length)
	{
		_body_length = new_length;
		if (_body_length > MAX_BODY_LENGTH)
			_body_length = MAX_BODY_LENGTH;
	}

	// buffer에는 데이터가 저장되지만, 얼마만큼의 길이가 저장되었는지 알 방법이 없다.
	// 그러므로 header와 body를 분리하고, header 부분에 길이를 저장해 준다.
	// 이 함수는 header의 데이터를 길이로 변환해 주는 함수이다.
	// 또한 데이터의 header와 body의 길이가 다른 경우는 이상이 있는 경우이고
	// 이를 return을 통해 알려준다.
	bool decodeHeader()
	{
		char header[HEADER_LENGTH + 1] = "";
		memcpy(header, _data, HEADER_LENGTH);
		_body_length = atoi(header);
		if (_body_length > MAX_BODY_LENGTH)
		{
			_body_length = 0;
			return false;
		}
		return true;
	}

	// 반대로 body의 길이만큼을 버퍼에 넣는 작업
	// 헤더의 길이가 HEADER_LENGTH 이기 때문에, 
	// HEADER_LENGTH라는 여백을 두어야 하고,
	// 그렇게 하기 위해 setw(HEADER_LENGTH)를 이용한다.
	void encodeHeader()
	{
		std::stringstream ss;
		ss << std::setw(HEADER_LENGTH) << _body_length;
		memcpy(_data, ss.str().c_str(), HEADER_LENGTH);
	}

private:
	// 데이터의 몸체를 저장하는 변수
	char _data[HEADER_LENGTH + MAX_BODY_LENGTH];

	// 데이터의 길이를 저장하는 변수
	size_t _body_length;
};