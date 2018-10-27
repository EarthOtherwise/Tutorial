#include "MessagingSystem.h"
#include "InitFileReadWrite.h"
#include <fstream>

namespace Otherwise
{
	CorrespondentManager::CorrespondentManager()
	{

	}

	CorrespondentManager::~CorrespondentManager()
	{

	}

	void CorrespondentManager::init()
	{
		std::string filePath = "CorrespondentMasterList.txt";
		std::vector<std::string> tempVector = readFile(filePath);

		for (std::string i : tempVector)
		{
			createCorrespondentSignature(i);
		}

		tempVector.clear();
		filePath = "Init.txt";
		std::string startline = "CorrespondentManager";
		std::string endLine = "CorrespondentManagerEnd";
		extractLinesFromFile(&tempVector, startline, endLine, filePath);

		for (std::string i : tempVector)
		{
			findAllConnections(i);
		}
	}

	void CorrespondentManager::findAllConnections(std::string & filePath)
	{
		std::string tempString;
		std::string currentPublisher;
		std::ifstream fileStream(filePath);
		if (fileStream.is_open())
		{
			while (getline(fileStream, tempString))
			{
				if (tempString == "PUBLISHER")
				{
					getline(fileStream, tempString);
					if (isCorrespondentSignature(tempString))
					{
						currentPublisher = tempString;
					}
					else
					{
						throwError("PUBLISHER", "Program does not recognize publisher " + tempString);
					}
				}
				if (tempString == "SUBSCRIBER")
				{
					getline(fileStream, tempString);
					if (isCorrespondentSignature(tempString))
					{
						createSubscription(currentPublisher, tempString);
					}
					else
					{
						throwError("SUBSCRIBER", "Program does not recognize subscriber " + tempString);
					}
				}
			}
		}
		else
		{
			throwError(filePath, filePath + " file is corrupt or missing.");
		}

	}

	void CorrespondentManager::checkConnectionRequests()
	{
		for (std::multimap<std::string, std::string>::iterator currentRequest = connectionRequests.begin(); currentRequest != connectionRequests.end(); currentRequest++)
		{
			auto publisher = activeCorrespondents.find(currentRequest->first);
			if (publisher != activeCorrespondents.end())
			{
				publisher = activeCorrespondents.find(currentRequest->second);
				if (publisher != activeCorrespondents.end())
				{
					createSubscription((std::string)currentRequest->first, currentRequest->second);
					connectionRequests.erase(currentRequest);
					return;
				}
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
		if (isCorrespondentSignature(correspondentSignature))
		{
			activeCorrespondents.insert(std::pair<std::string, Correspondent *>(correspondentSignature, correspondent));
			checkConnectionRequests();
		}
		else
		{
			throwError("CorrespondentError", "Correspondent " + correspondentSignature + " does not appear in CorrespondentMasterList");
		}
	}

	void CorrespondentManager::deListCorrespondent(std::string & correspondentSignature)
	{
		auto activeCorrespondent = activeCorrespondents.find(correspondentSignature);
		if (activeCorrespondent != activeCorrespondents.end())
		{
			activeCorrespondents.erase(activeCorrespondent);
		}
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

	bool CorrespondentManager::isCorrespondentSignature(std::string & signature)
	{
		unsigned int iterator = 0;
		while (iterator < mAllPossibleCorrespondentSignatures.size())
		{
			if (signature == mAllPossibleCorrespondentSignatures[iterator])
			{
				return true;
			}
			iterator++;
		}
		return false;
	}

	Correspondent::Correspondent()
	{

	}

	Correspondent::~Correspondent()
	{

	}

	void Correspondent::init(CorrespondentManager * manager, std::string &signature)
	{
		mManager = manager;
		mSignature = signature;
		manager->listNewCorrespondent(signature, this);
	}

	void Correspondent::destroy()
	{
		mManager->deListCorrespondent(mSignature);
	}

	void Correspondent::passToSubscribers(std::shared_ptr<Message> messagePtr)
	{
		for (auto iterator = mSubscribers.begin(); iterator != mSubscribers.end(); iterator++)
		{
			iterator->second->recieveMessage(messagePtr);
		}
	}

	void Correspondent::recieveSubscriber(std::string & subscriberSignature, Correspondent * subscriber)
	{
		mSubscribers.insert(std::pair<std::string, Correspondent *>(subscriberSignature, subscriber));
	}

	void Correspondent::removeSubscriber(std::string & subscriberSignature)
	{
		auto findSubscriber = mSubscribers.find(subscriberSignature);
		if (findSubscriber != mSubscribers.end())
		{
			mSubscribers.erase(subscriberSignature);
		}
	}

	void Correspondent::publish()
	{
		std::shared_ptr<Message> newMessage(new Message);
		passToSubscribers(newMessage);
	}

	void Correspondent::publish(glm::vec2 & mouseCoords)
	{
		std::shared_ptr<MouseMessage> newMessage(new MouseMessage);
		newMessage->mouseCoords = mouseCoords;
		passToSubscribers(newMessage);
	}

	void Correspondent::publish(SDL_Event & evnt)
	{
		std::shared_ptr<EventMessage> newMessage(new EventMessage);
		newMessage->evnt = evnt;
		passToSubscribers(newMessage);
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

	SDL_Event Correspondent::getEventMessage()
	{
		return mMessage->getEventMessage();
	}

	void Correspondent::clearMessage()
	{
		mMessage = nullptr;
	}
}