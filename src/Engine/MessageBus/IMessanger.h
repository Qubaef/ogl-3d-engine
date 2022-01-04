#pragma once
#ifndef IMESSANGER_H
#define IMESSANGER_H

#include <vector>

struct Message;
class MessageBus;

class IMessanger
{
	MessageBus* messageBus;
	const char* name;
	int id;

protected:
	IMessanger(MessageBus* messageBus, const char* name);

	// Send message to specific recipient by name
	// message will be deleted by MessageBus after sending
	void sendMessage(Message* message, const char* recipientName);

	// Send message to specific recipient by id
	// message will be deleted by MessageBus after sending
	void sendMessage(Message* message, int recipientId);

	// Send message to all recipients
	// message will be deleted by MessageBus after sending
	void sendMessage(Message* message, IMessanger* recipient);

	// Send message to all subscribers
	// message will be deleted by MessageBus after sending
	void emitMessage(Message* message);

	// Get current messages
	std::vector<Message*> getMessages();

	// Subscribe to recipient
	bool subscribe(const char* targetMessangerName);
	bool subscribe(int targetMessangerId);
	bool subscribe(IMessanger* targetMessanger);

	// Unsubscribe from recipient
	bool unsubscribe(const char* targetMessangerName);
	bool unsubscribe(int targetMessangerId);
	bool unsubscribe(IMessanger* targetMessanger);
public:
	int getId();
	const char* getName();
};

#endif
