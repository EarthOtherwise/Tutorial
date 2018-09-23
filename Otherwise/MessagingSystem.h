#pragma once
#include <map>
#include <memory>
#include "GLM/glm.hpp"

namespace Otherwise
{
	struct Message
	{
		virtual glm::vec2 getMouseMessage() { glm::vec2(0.0f); }
	};

	struct MouseMessage : Message
	{
		glm::vec2 getMouseMessage() { return mouseCoords; }
		glm::vec2 mouseCoords;
	};

	class CorrespondentManager
	{
	public:
		CorrespondentManager();
		~CorrespondentManager();

		void createConnectionRequestListFromINIFile(std::string &iniFilePath);

		void createSubscription(std::string &subscriber, std::string &publisher);
		void cancelSubscription(std::string &subscriber, std::string &publisher);

		void listNewCorrespondent(std::string &correspondentSignature, Correspondent* correspondent);
		void deListCorrespondent(std::string &correspondentSignature);

		void createCorrespondentSignature(std::string &signature);
		void removeCorrespondentSignature(std::string &signature);
		bool isCoorespondentSignature(std::string &signature);
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

		void init(CorrespondentManager* manager);
		void destroy();

		void recieveSubscriber(std::string &subscriberSignature, Correspondent* subscriber);
		void removeSubscriber(std::string &subscriberSignature);

		void publishMessage();
		void publishMouseMessage(glm::vec2 &mouseCoords);

		void recieveMessage(std::shared_ptr<Message> message);

		bool getMessage();
		glm::vec2 getMouseMessage();

		void clearMessage();

	private:
		CorrespondentManager* mManager;
		std::map<std::string, Correspondent*> mSubscribers;
		std::shared_ptr<Message> mMessage;
	};
}