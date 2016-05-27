#include <boost/bind.hpp>
#include "ChatSession.h"

ChatSession::ChatSession(boost::asio::io_service& io_service, ChatRoom& room)
	:
	// 소켓을 만든다.
	// 이후에 이 socket은 accepet를 통해 서버와 클라이언트의 통신을 위한 장치가 된다.
	_socket(io_service),
	// 채팅방을 가져온다.
	_room(room)
{
	std::cout << "ChatSession : constructed" << std::endl;
}

ChatSession::~ChatSession()
{
	std::cout << "ChatSession : destructed" << std::endl;
}

boost::asio::ip::tcp::socket& ChatSession::socket()
{
	return _socket;
}

// 세션의 시작
// 채팅방에 참여하고 socket으로부터 read를 수행한다.
void ChatSession::start()
{
	// 채팅방에 세션에 대한 this포인터를 넘겨준다.
	// 포인터를 스마트포인터로 받기 때문에, shared_from_this를 이용한다.
	// shared_from_this는 스마트포인터의 this 버전이다.
	_room.join(shared_from_this());

	// 비동기로 header를 read를 한다.
	// 읽은 데이터는 _read_msg.data()에 저장한다.
	// read가 끝날 경우 recursiveReadHeader함수를 호출한다.
	boost::asio::async_read(
		_socket,
		boost::asio::buffer(
			_read_msg.data(),
			ChatMessage::HEADER_LENGTH),
		boost::bind(
			&ChatSession::handleReadHeader,
			shared_from_this(),
			boost::asio::placeholders::error));
}

// 참여자의 deliver 함수
// 클라이언트에게 다시 write을 해주는 함수
// 즉 다시 전송하는 부분이다.
void ChatSession::deliver(const ChatMessage& msg)
{
	std::cout << "deliver msg" << std::endl;

	// write_msg가 비어있으면 false ???????????
	bool write_in_progress = !_write_msgs.empty();

	_write_msgs.push_back(msg);

	// false일 때 실행
	if (!write_in_progress)
	{
		// client에게 비동기로 _write_buffer의 첫 부분을 write한다.
		// 즉 클라이언트에 write을 한다.
		// 비동기 write가 끝날 경우 handleDeliver함수를 호출한다.
		boost::asio::async_write(
			_socket,
			boost::asio::buffer(
				_write_msgs.front().data(),
				_write_msgs.front().length()),
			boost::bind(
				&ChatSession::handleWrite,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
}

// 비동기 read 작업이 끝날 경우 호출되는 함수
void ChatSession::handleReadHeader(const boost::system::error_code& error)
{
	// error가 없고, decodeHeader가 정상적일 경우
	if (!error && _read_msg.decodeHeader())
	{
		// body를 읽는다.
		// 마찬가지로 비동기로 읽는다.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.body(),
				_read_msg.bodyLength()),
			boost::bind(
				&ChatSession::handleReadBody,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}

// 실제적인 데이터를 받아들이는 함수이다.
void ChatSession::handleReadBody(const boost::system::error_code& error)
{
	if (!error)
	{
		// 채팅방으로 msg를 뿌린다.
		_room.deliver(_read_msg);

		// 다시 header부터 읽는다.
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.data(),
				ChatMessage::HEADER_LENGTH),
			boost::bind(
				&ChatSession::handleReadHeader,
				shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		_room.leave(shared_from_this());
	}
}


void ChatSession::handleWrite(const boost::system::error_code& error)
{
	if (!error)
	{
		_write_msgs.pop_front();
		if (!_write_msgs.empty())
		{
			boost::asio::async_write(
				_socket,
				boost::asio::buffer(
					_write_msgs.front().data(),
					_write_msgs.front().length()),
				boost::bind(
					&ChatSession::handleWrite,
					shared_from_this(),
					boost::asio::placeholders::error));
		}
	}
	else
	{
		_room.leave(shared_from_this());
	}
}
