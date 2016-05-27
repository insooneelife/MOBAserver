#include "ChatServer.h"
#include "ChatMessage.hpp"
#include "ChatSession.h"

ChatServer::ChatServer(
	// 어떤 방법으로 통신할 것인지
	boost::asio::io_service& io_service,
	// 서버로 사용할 endpoint
	const boost::asio::ip::tcp::endpoint& endpoint)
	:
	_io_service(io_service),
	// endpoint와 통신방법으로 acceptor를 생성한다.
	_acceptor(io_service, endpoint)
{
	std::cout << "ChatServer : constructed" << std::endl;
	startAccept();

	std::cout << std::endl << "[ip 주소]" << std::endl;

	using boost::asio::ip::tcp;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(boost::asio::ip::host_name(), "");
	tcp::resolver::iterator iter = resolver.resolve(query);
	tcp::resolver::iterator end; // End marker.
	while (iter != end)
	{
		tcp::endpoint ep = *iter++;
		std::cout << ep << std::endl;
	}
	std::cout << std::endl;

	std::cout << "[port 번호]" << std::endl;
	std::cout << endpoint.port() << std::endl;
}

ChatServer::~ChatServer()
{
	_acceptor.close();
	std::cout << "ChatServer : destructed" << std::endl;
}

void ChatServer::startAccept()
{
	//chat 세션을 생성한다.
	boost::shared_ptr<ChatSession> new_session(new ChatSession(_io_service, _room));

	// 비동기 accept를 수행한다.
	// accept 작업이 끝날 경우 handleAccept 함수를 호출해준다.
	_acceptor.async_accept(
		new_session->socket(),
		boost::bind(
			&ChatServer::handleAccept,
			this,
			new_session,
			boost::asio::placeholders::error));
}

// 비동기 accept가 끝날 때의 대한 작업
void ChatServer::handleAccept(
	boost::shared_ptr<ChatSession> session,
	const boost::system::error_code& error)
{
	if (!error)
	{
		// 세션의 작업을 시작해준다.
		session->start();
	}

	// 다시 accpet를 수행한다.
	startAccept();
}


