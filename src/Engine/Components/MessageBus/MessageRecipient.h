#pragma once

class IMessanger;

struct MessageRecipient
{
	const enum class RecipientType
	{
		NAME,
		ID,
		MESSANGER
	} recipientType;

	const char* name;
	const int id;
	IMessanger* messanger;

	MessageRecipient(const char* name);
	MessageRecipient(int id);
	MessageRecipient(IMessanger* messanger);
};

inline MessageRecipient::MessageRecipient(const char* name)
	: recipientType(RecipientType::NAME), name(name), id(0), messanger(nullptr)
{
}

inline MessageRecipient::MessageRecipient(int id)
	: recipientType(RecipientType::ID), name(nullptr), id(id), messanger(nullptr)
{
}

inline MessageRecipient::MessageRecipient(IMessanger* messanger)
	: recipientType(RecipientType::MESSANGER), name(nullptr), id(0), messanger(messanger)
{
}
