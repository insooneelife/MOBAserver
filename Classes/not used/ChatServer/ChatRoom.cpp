#include <boost\asio.hpp>
#include <boost/bind.hpp>
#include "ChatRoom.h"
#include "ChatParticipant.h"

void ChatRoom::join(boost::shared_ptr<ChatParticipant> participant)
{
	std::cout << "ChatRoom : join" << std::endl;

	// 참여자 리스트에 추가
	_participants.insert(participant);

	// 모든 읽은 msg를 참여자에게 deliver 함수로 뿌려준다.
	std::for_each(_recent_msgs.begin(), _recent_msgs.end(),
		boost::bind(&ChatParticipant::deliver, participant, _1));
}

// 세션(참여자)를 채팅방에서 제거한다.
void ChatRoom::leave(boost::shared_ptr<ChatParticipant> participant)
{
	std::cout << "ChatRoom : leave" << std::endl;
	_participants.erase(participant);
}

// 채팅방으로 msg를 뿌린다.
void ChatRoom::deliver(const ChatMessage& msg)
{
	std::cout << "ChatRoom : deliver" << std::endl;

	// 대화 목록 리스트에 msg를 추가
	_recent_msgs.push_back(msg);

	// 최대 허용 msg 개수보다 msg가 많으면 msg를 제거한다.
	while (_recent_msgs.size() > MAX_RECENT_MSG)
		_recent_msgs.pop_front();

	// 모든 참여자의 deliver를 호출해준다.
	std::for_each(_participants.begin(), _participants.end(),
		boost::bind(&ChatParticipant::deliver, _1, boost::ref(msg)));
}
