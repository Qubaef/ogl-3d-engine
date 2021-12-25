#include "MessageBus.h"

IMessanger *MessageBus::getMessanger(int index)
{
    // Synchronize access to messangersList
    mtx.lock();

    // Find messanger from messangersList
    if (index < 0 || index >= messangersList.size())
    {
        return nullptr;
    }

    IMessanger *messanger = messangersList[index];

    // Release access to messangersList
    mtx.unlock();

    return messanger;
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
    // Get IMessanger's index in nameToIndexDict
    auto it = nameToIndexDict.find(recipientName);
    if (it == nameToIndexDict.end())
    {
        return;
    }

    // Get IMessanger by index
    IMessanger *recipient = getMessanger(it->second);

    // Send message to messanger
    sendMessage(message, sender, recipient);
}

void MessageBus::sendMessage(Message *message, IMessanger *sender, int id)
{
    // Get IMessanger's index in idToIndexDict
    auto it = idToIndexDict.find(id);
    if (it == idToIndexDict.end())
    {
        return;
    }

    // Get IMessanger by index
    IMessanger *recipient = getMessanger(it->second);

    // Send message to sender
    sendMessage(message, sender, recipient);
}

void MessageBus::sendMessage(Message *message, IMessanger *sender, IMessanger *recipient)
{
    if (sender == nullptr || recipient == nullptr)
    {
        return;
    }

    message->sender = sender;
    message->recipient = recipient;

    // Add message to pendingMessages
    pendingMessages.push_back(message);
}

void MessageBus::emitMessage(IMessanger *messanger, Message *message)
{
    // Get messanger subscribers from messangerToSubscribersDict
    auto it = messangerToSubscribersDict.find(messanger);
    if (it == messangerToSubscribersDict.end())
    {
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

    // Assign all pending messages to messangerToMessagesDict by recipients
    for (Message *message : pendingMessages)
    {
        if (message->recipient != nullptr)
        {
            messangerToMessagesDict[message->recipient].push_back(message);
        }
    }
}
