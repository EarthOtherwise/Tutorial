#include "MessagingSystem.h"
#include "InitFileReadWrite.h"

namespace Otherwise
{
	CorrespondentManager::CorrespondentManager()
	{

	}

	CorrespondentManager::~CorrespondentManager()
	{

	}



	void CorrespondentManager::createConnectionRequestListFromINIFile(std::string & iniFilePath)
	{
		std::vector<std::string> iniFileStrings = readFile(iniFilePath);

		unsigned int iterator = 0;
		std::string subscriber;
		std::string publisher;
		while (iterator < iniFileStrings.size())
		{
			if (iniFileStrings[iterator] == "Correspondents")
			{
				iterator++;
				while (iniFileStrings[iterator] != "EndCorrespondents")
				{
					if (isCoorespondentSignature(iniFileStrings[iterator]))
					{
						subscriber = iniFileStrings[iterator];
						iterator++;
						if (isCoorespondentSignature(iniFileStrings[iterator]))
						{
							publisher = iniFileStrings[iterator];
							createSubscription(subscriber, publisher);
							iterator++;
						}
						else
						{
							throwError("CorrespondentError", "Requested correspondent " + iniFileStrings[iterator] + " does not exist.");
						}
					}
					else
					{
						throwError("CorrespondentError", "Requested correspondent " + iniFileStrings[iterator] + " does not exist.");
					}
				}
			}
			else
			{
				iterator++;
			}
		}
	}

	void CorrespondentManager::createSubscription(std::string & subscriber, std::string & publisher)
	{
		auto doesSubscriberExist = activeCorrespondents.find(subscriber);
		if (doesSubscriberExist == activeCorrespondents.end())
		{
			connectionRequests.emplace(publisher, subscriber);
			return;
		}

		auto doesPublisherExist = activeCorrespondents.find(publisher);
		if (doesPublisherExist == activeCorrespondents.end())
		{
			connectionRequests.emplace(publisher, subscriber);
			return;
		}
		doesPublisherExist->second->recieveSubscriber(subscriber, doesSubscriberExist->second);
	}

	void CorrespondentManager::cancelSubscription(std::string & subscriber, std::string & publisher)
	{
		auto doesPublisherExist = activeCorrespondents.find(publisher);
		if (doesPublisherExist != activeCorrespondents.end())
		{
			doesPublisherExist->second->removeSubscriber(subscriber);
		}
		
		for (auto connectionIterator = connectionRequests.begin(); connectionIterator != connectionRequests.end(); connectionIterator++)
		{
			if (connectionIterator->first == publisher && connectionIterator->second == subscriber)
			{
				connectionRequests.erase(connectionIterator);
			}
		}
	}

	void CorrespondentManager::listNewCorrespondent(std::string & correspondentSignature, Correspondent * correspondent)
	{
		auto activeCorrespondent = activeCorrespondents.find(correspondentSignature);
		if (activeCorrespondent == activeCorrespondents.end())
		{
			activeCorrespondents[correspondentSignature] = correspondent;
		}
		return;
	}

	void CorrespondentManager::deListCorrespondent(std::string & correspondentSignature)
	{
		auto activeCorrespondent = activeCorrespondents.find(correspondentSignature);
		if (activeCorrespondent != activeCorrespondents.end())
		{
			activeCorrespondents.erase(activeCorrespondent);
		}
		return;
	}

	void CorrespondentManager::createCorrespondentSignature(std::string & signature)
	{
		mAllPossibleCorrespondentSignatures.push_back(signature);
	}

	void CorrespondentManager::removeCorrespondentSignature(std::string & signature)
	{
		unsigned int iterator = 0;
		while (iterator < mAllPossibleCorrespondentSignatures.size())
		{
			if (signature == mAllPossibleCorrespondentSignatures[iterator])
			{
				mAllPossibleCorrespondentSignatures.erase(mAllPossibleCorrespondentSignatures.begin() + iterator);
			}
		}
	}

	bool CorrespondentManager::isCoorespondentSignature(std::string & signature)
	{
		unsigned int iterator = 0;
		while (iterator < mAllPossibleCorrespondentSignatures.size())
		{
			if (signature == mAllPossibleCorrespondentSignatures[iterator])
			{
				return true;
			}
		}
		return false;
	}

	Correspondent::Correspondent()
	{

	}

	Correspondent::~Correspondent()
	{

	}

	void Correspondent::init(CorrespondentManager * manager)
	{
		mManager = manager;
	}

	void Correspondent::destroy()
	{

	}

	void Correspondent::recieveSubscriber(std::string & subscriberSignature, Correspondent * subscriber)
	{
		auto findSubscriber = mSubscribers.find(subscriberSignature);
		if (findSubscriber != mSubscribers.end())
		{
			mSubscribers[subscriberSignature] = subscriber;
		}
	}

	void Correspondent::removeSubscriber(std::string & subscriberSignature)
	{
		auto findSubscriber = mSubscribers.find(subscriberSignature);
		if (findSubscriber != mSubscribers.end())
		{
			mSubscribers.erase(subscriberSignature);
		}
	}

	void Correspondent::publishMessage()
	{
		std::shared_ptr<Message> newMessage(new Message);
		for (auto iterator = mSubscribers.begin(); iterator != mSubscribers.end(); iterator++)
		{
			iterator->second->recieveMessage(newMessage);
		}
	}

	void Correspondent::publishMouseMessage(glm::vec2 & mouseCoords)
	{
		std::shared_ptr<MouseMessage> newMessage(new MouseMessage);
		newMessage->mouseCoords = mouseCoords;
		for (auto iterator = mSubscribers.begin(); iterator != mSubscribers.end(); iterator++)
		{
			iterator->second->recieveMessage(newMessage);
		}
	}

	void Correspondent::recieveMessage(std::shared_ptr<Message> message)
	{
		mMessage = message;
	}

	bool Correspondent::getMessage()
	{
		if (mMessage != nullptr)
		{
			return true;
		}
		return false;
	}

	glm::vec2 Correspondent::getMouseMessage()
	{
		return mMessage->getMouseMessage();
	}

	void Correspondent::clearMessage()
	{
		mMessage = nullptr;
	}
}