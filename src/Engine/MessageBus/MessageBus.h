#pragma once
#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <mutex>
#include <map>
#include <vector>

// #include "../CpuPipeline/IProcessable.h"
#include "Message.h"
#include "IMessanger.h"

// Comparator for names dictionary
// TODO: Move to file containing common symbols
struct StringComparator
{
	bool operator()(char const* a, char const* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};

class MessageBus
{
	std::mutex mtx;

	std::vector<IMessanger*> messangersList;
	std::map<IMessanger*, std::vector<Message*>> messangerToMessagesDict;
	std::map<IMessanger*, std::vector<IMessanger*>> messangerToSubscribersDict;

	std::map<int, int> idToIndexDict;
	std::map<const char*, int, StringComparator> nameToIndexDict;

	std::vector<Message*> pendingMessages;


	IMessanger* getMessanger(int index);

	// Get messages for specific messanger
	std::vector<Message*> getMessages(const char* messangerName);
	std::vector<Message*> getMessages(int messangerId);
	std::vector<Message*> getMessages(IMessanger* messanger);

	// Send message to specific messanger
	void sendMessage(Message* message, IMessanger* sender, const char* recipientName);
	void sendMessage(Message* message, IMessanger* sender, int id);
	void sendMessage(Message* message, IMessanger* sender, IMessanger* recipient);

	// Emit message to all messanger subscribers
	void emitMessage(IMessanger* messanger, Message* message);

	// Subscribe messanger to target messanger
	// return true if successfully subscribed
	bool subscribe(IMessanger* messanger, const char* targetMessangerName);
	bool subscribe(IMessanger* messanger, int targetMessangerId);
	bool subscribe(IMessanger* messanger, IMessanger* targetMessanger);

	// Unsubscribe messanger from target messanger
	// return true if successfully unsubscribed
	bool unsubscribe(IMessanger* messanger, const char* targetMessangerName);
	bool unsubscribe(IMessanger* messanger, int targetMessangerId);
	bool unsubscribe(IMessanger* messanger, IMessanger* targetMessanger);

	friend class IMessanger;

public:

	MessageBus();

	// Register messanger
	// returns id of registered messanger
	int registerMessanger(IMessanger* messanger);

	// Publish pending messages
	void publish();
};

#endif