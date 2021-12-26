#include "MessageBus.h"

#include "Engine/Logger/Logger.h"

IMessanger *MessageBus::getMessanger(int index)
{
    // Synchronize access to messangersList
    mtx.lock();

    // Find messanger from messangersList
    if (index < 0 || index >= messangersList.size())
    {
        mtx.unlock();
        return nullptr;
    }

    IMessanger *messanger = messangersList[index];

    // Release access to messangersList
    mtx.unlock();

    return messanger;
}

IMessanger* MessageBus::getRecipient(MessageRecipient messageRecipient)
{
    int index = -1;
    std::map<const char*, int, StringComparator>::iterator itName;
    std::map<int, int>::iterator itId;
    std::map<IMessanger*, int>::iterator itMessanger;

    switch (messageRecipient.recipientType)
    {
    case MessageRecipient::RecipientType::NAME:
        // Find index from name
        itName = nameToIndexDict.find(messageRecipient.name);
        if (itName != nameToIndexDict.end())
        {
            index = itName->second;
        }
        break;
    case MessageRecipient::RecipientType::ID:
        // Find index from ids
        itId = idToIndexDict.find(messageRecipient.id);
        if (itId != idToIndexDict.end())
        {
            index = itId->second;
        }
        break;
    case MessageRecipient::RecipientType::MESSANGER:
	    itMessanger = messangerToIndexDict.find(messageRecipient.messanger);
        if (itMessanger != messangerToIndexDict.end())
        {
            index = itMessanger->second;
        }
        break;
    }

    if (index < 0 || index >= messangersList.size())
    {
        return nullptr;
    }

    return messangersList[index];
}

std::vector<Message *> MessageBus::getMessages(const char *messangerName)
{
    // Get IMessanger's index in nameToIndexDict
    auto it = nameToIndexDict.find(messangerName);
    if (it == nameToIndexDict.end())
    {
        return std::vector<Message *>();
    }

    // Get IMessanger by index
    IMessanger *messanger = getMessanger(it->second);

    // Get messages for messanger
    return getMessages(messanger);
}

std::vector<Message *> MessageBus::getMessages(int messangerId)
{
    // Get IMessanger's index in idToIndexDict
    auto it = idToIndexDict.find(messangerId);
    if (it == idToIndexDict.end())
    {
        return std::vector<Message *>();
    }

    // Get IMessanger by index
    IMessanger *messanger = getMessanger(it->second);

    // Get messages for messanger
    return getMessages(messanger);
}

std::vector<Message *> MessageBus::getMessages(IMessanger *messanger)
{
    // Synchronize access to messangerToMessagesDict
    mtx.lock();

    // Find messages for messanger
    auto it = messangerToMessagesDict.find(messanger);
    if (it == messangerToMessagesDict.end())
    {
        mtx.unlock();
        return std::vector<Message *>();
    }

    // Get messages for messanger
    std::vector<Message *> messages = it->second;

    // Release access to messangerToMessagesDict
    mtx.unlock();

    return messages;
}

void MessageBus::sendMessage(Message *message, IMessanger *sender, const char *recipientName)
{
    // Synchronize access to messangerToMessagesDict
    mtx.lock();

    message->sender = sender;

    // Add message to pendingMessagesList
    pendingMessages.emplace_back(MessageRecipient(recipientName), message);

    // Release access to messangerToMessagesDict
    mtx.unlock();
}

void MessageBus::sendMessage(Message *message, IMessanger *sender, int id)
{
    // Synchronize access to messangerToMessagesDict
    mtx.lock();

    message->sender = sender;

    // Add message to pendingMessagesList
    pendingMessages.emplace_back(MessageRecipient(id), message);

    // Release access to messangerToMessagesDict
    mtx.unlock();
}

void MessageBus::sendMessage(Message *message, IMessanger *sender, IMessanger *recipient)
{
    // Synchronize access to messangerToMessagesDict
    mtx.lock();

    message->sender = sender;

    // Add message to pendingMessagesList
    pendingMessages.emplace_back(MessageRecipient(recipient), message);

    // Release access to messangerToMessagesDict
    mtx.unlock();
}

void MessageBus::emitMessage(IMessanger *messanger, Message *message)
{
    // Get messanger subscribers from messangerToSubscribersDict
    auto it = messangerToSubscribersDict.find(messanger);
    if (it == messangerToSubscribersDict.end())
    {
        // Subscribers list has to be present
        _ASSERT(false);
        return;
    }

    // Get messanger subscribers
    std::vector<IMessanger *> subscribers = it->second;

    // Send message to messanger subscribers
    for (IMessanger *subscriber : subscribers)
    {
        sendMessage(message, messanger, subscriber);
    }
}

bool MessageBus::subscribe(IMessanger *messanger, const char *targetMessangerName)
{
    // Get IMessanger's index in nameToIndexDict
    auto it = nameToIndexDict.find(targetMessangerName);
    if (it == nameToIndexDict.end())
    {
        LOG.ERROR("Cannot subscribe. No recipient named: %s", targetMessangerName);
        return false;
    }

    // Get IMessanger by index
    IMessanger *targetMessanger = getMessanger(it->second);

    // Subscribe to messanger
    return subscribe(messanger, targetMessanger);
}

bool MessageBus::subscribe(IMessanger *messanger, int targetMessangerId)
{
    // Get IMessanger's index in idToIndexDict
    auto it = idToIndexDict.find(targetMessangerId);
    if (it == idToIndexDict.end())
    {
        LOG.ERROR("Cannot subscribe. No recipient with id: %d", targetMessangerId);
        return false;
    }

    // Get IMessanger by index
    IMessanger *targetMessanger = getMessanger(it->second);

    // Subscribe to messanger
    return subscribe(messanger, targetMessanger);
}

bool MessageBus::subscribe(IMessanger *messanger, IMessanger *targetMessanger)
{
    // Synchronize access to messangerToSubscribersDict
    mtx.lock();

    // Find messanger subscribers from messangerToSubscribersDict
    auto it = messangerToSubscribersDict.find(messanger);
    if (it == messangerToSubscribersDict.end())
    {
        // Messanger HAS TO have subscribers
        _ASSERT(false);

        mtx.unlock();
        return false;
    }

    // Check if messanger is already subscribed to targetMessanger
    std::vector<IMessanger *> &subscribers = it->second;

    for (IMessanger *subscriber : subscribers)
    {
        if (subscriber == targetMessanger)
        {
            // Release access to messangerToSubscribersDict
            mtx.unlock();
            return false;
        }
    }

    // Add targetMessanger to messanger subscribers
    subscribers.push_back(targetMessanger);

    // Release access to messangerToSubscribersDict
    mtx.unlock();

    return true;
}

bool MessageBus::unsubscribe(IMessanger *messanger, const char *targetMessangerName)
{
    // Get IMessanger's index in nameToIndexDict
    auto it = nameToIndexDict.find(targetMessangerName);
    if (it == nameToIndexDict.end())
    {
        LOG.ERROR("Cannot unsubscribe. No recipient named: %s", targetMessangerName);
        return false;
    }

    // Get IMessanger by index
    IMessanger *targetMessanger = getMessanger(it->second);

    // Unsubscribe from messanger
    return unsubscribe(messanger, targetMessanger);
}

bool MessageBus::unsubscribe(IMessanger *messanger, int targetMessangerId)
{
    // Get IMessanger's index in idToIndexDict
    auto it = idToIndexDict.find(targetMessangerId);
    if (it == idToIndexDict.end())
    {
        LOG.ERROR("Cannot unsubscribe. No recipient with id: %d", targetMessangerId);
        return false;
    }

    // Get IMessanger by index
    IMessanger *targetMessanger = getMessanger(it->second);

    // Unsubscribe from messanger
    return unsubscribe(messanger, targetMessanger);
}

bool MessageBus::unsubscribe(IMessanger *messanger, IMessanger *targetMessanger)
{
    // Synchronize access to messangerToSubscribersDict
    mtx.lock();

    // Find messanger subscribers from messangerToSubscribersDict
    auto it = messangerToSubscribersDict.find(messanger);
    if (it == messangerToSubscribersDict.end())
    {
        // Messanger HAS TO have subscribers
        _ASSERT(false);

        mtx.unlock();
        return false;
    }

    // Check if messanger is already subscribed to targetMessanger
    std::vector<IMessanger *> &subscribers = it->second;

    for (IMessanger *subscriber : subscribers)
    {
        if (subscriber == targetMessanger)
        {
            // Remove targetMessanger from messanger subscribers
            subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), targetMessanger), subscribers.end());

            // Release access to messangerToSubscribersDict
            mtx.unlock();
            return true;
        }
    }

    // Release access to messangerToSubscribersDict
    mtx.unlock();

    return false;
}

MessageBus::MessageBus()
{
}

int MessageBus::registerMessanger(IMessanger *messanger)
{
    // Synchronize access to messangersList
    mtx.lock();

    // Add messanger to messangersList
    int id = messangersList.size();
    messangersList.push_back(messanger);

    // Add messanger to messangerToMessagesDict
    messangerToMessagesDict[messanger] = std::vector<Message *>();

    // Add messanger to messangerToSubscribersDict
    messangerToSubscribersDict[messanger] = std::vector<IMessanger *>();

    // Add messanger to nameToIndexDict
    nameToIndexDict[messanger->getName()] = id;

    // Add messanger to idToIndexDict
    idToIndexDict[id] = id;

    // Release access to messangersList
    mtx.unlock();

    return id;
}

void MessageBus::publish()
{
    // Synchronize access to messangerToMessagesDict
    mtx.lock();

    // Delete all messages from messangerToMessagesDict
    for (auto &messangerToMessagesPair : messangerToMessagesDict)
    {
        std::vector<Message *> &messages = messangerToMessagesPair.second;

        for (const Message *message : messages)
        {
            delete message;
        }

        messages.clear();
    }

    // Assign all pending messages to messangerToMessagesDict by messangers
    // Iterate through all messangers backwards and pop message after sending
    for (int i = pendingMessages.size() - 1; i >= 0; i--)
    {
        std::pair<MessageRecipient, Message*> message = pendingMessages[i];
        pendingMessages.pop_back();

        // Get message's recipient
        IMessanger *recipient = getRecipient(message.first);
        if (recipient != nullptr)
        {
            message.second->recipient = recipient;
            messangerToMessagesDict[recipient].push_back(message.second);
        }
    }

    mtx.unlock();
}
