#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

namespace Network
{
	const int C_MESSAGE_CHAT = 1;

	class Message
	{
	public:
		std::string Flatten() const;
	protected:
		virtual std::string FlattenArguments() const = 0;
		virtual int ID() const = 0;
	private:
	};

	class MessageFactory
	{
	public:
		static Message* Inflate(const std::string& message);
	private:
	};
}

#endif