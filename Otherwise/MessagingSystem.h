#pragma once
#include <map>
#include <memory>
#include <vector>
#include "GLM/glm.hpp"

namespace Otherwise
{
	struct Message
	{
		virtual glm::vec2 getMouseMessage() { return glm::vec2(0.0f); }
		virtual std::string getStringMessage() { return std::string(""); }
	};

	struct MouseMessage : Message
	{
		glm::vec2 getMouseMessage() { return mouseCoords; }
		glm::vec2 mouseCoords;
	};

	struct StringMessage : Message
	{
		std::string getStringMessage() { return string; }
		std::string string;
	};

	class Correspondent;

	class CorrespondentManager
	{
	public:
		CorrespondentManager();
		~CorrespondentManager();

		void init();

		void findAllConnections(std::string &filePath);

		void checkConnectionRequests();

		void createSubscription(std::string &subscriber, std::string &publisher);
		void cancelSubscription(std::string &subscriber, std::string &publisher);

		void listNewCorrespondent(std::string &correspondentSignature, Correspondent* correspondent);
		void deListCorrespondent(std::string &correspondentSignature);

		void createCorrespondentSignature(std::string &signature);
		void removeCorrespondentSignature(std::string &signature);
		bool isCorrespondentSignature(std::string &signature);
	private:
		std::map<std::string, Correspondent*> activeCorrespondents;
		std::multimap<std::string, std::string> connectionRequests;
		std::vector<std::string> mAllPossibleCorrespondentSignatures;
	};

	class Correspondent
	{
	public:
		Correspondent();
		~Correspondent();

		void init(CorrespondentManager* manager, std::string &signature);
		void destroy();

		void passToSubscribers(std::shared_ptr<Message> messagePtr);

		void recieveSubscriber(std::string &subscriberSignature, Correspondent* subscriber);
		void removeSubscriber(std::string &subscriberSignature);

		void publish();
		void publish(glm::vec2 &mouseCoords);
		void publish(std::string &string);

		void recieveMessage(std::shared_ptr<Message> message);

		bool getMessage();
		glm::vec2 getMouseMessage();
		std::string getStringMessage();

		void clearMessage();

	private:
		CorrespondentManager* mManager;
		std::map<std::string, Correspondent*> mSubscribers;
		std::shared_ptr<Message> mMessage;
		std::string mSignature;
	};
}