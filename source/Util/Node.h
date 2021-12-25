#pragma once
#include <mutex>
#include <vector>

class Node
{
	//thread
	std::recursive_mutex mutex;

	Node *parent = nullptr;
	std::vector<Node *> children;

public:
	Node();
	~Node();

	Node *getParent();
	const std::vector<Node *> &getChildren();

	virtual void add(Node * n);
	virtual void disconnect();
	virtual void remove(Node * n);

	Node *getPrevious();

	virtual std::string getType();
	std::vector<Node *> getAncestors();
	Node *getLastCommonAncestor(Node * bComponent);
};