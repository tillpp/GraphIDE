#include "Node.h"

Node::Node() {}
Node::~Node()
{
	for (auto &child : children)
	{
		delete child;
	}
	children.clear();
}

Node *Node::getParent()
{
	return parent;
}
const std::vector<Node *> &Node::getChildren()
{
	return children;
}

void Node::add(Node *n)
{
	n->disconnect();
	//parent connect
	children.push_back(n);
	n->parent = this;
}
void Node::disconnect()
{
	if (parent)
		parent->remove(this);
	parent = nullptr;
}
void Node::remove(Node *n)
{
	//disconnect from parent
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i] == n)
		{
			children.erase(children.begin() + i);
			i--;
		}
	}
	n->parent = nullptr;
}

Node * Node::getPrevious()
{
	if (!parent)
		return nullptr;
	auto &children = parent->getChildren();
	for (size_t i = 1; i < children.size(); i++)
	{
		if (children[i] == this)
			return children[i - 1];
	}
	return nullptr;
}

std::string Node::getType()
{
	return "";
}

std::vector<Node *> Node::getAncestors()
{
	std::vector<Node *> rv;
	if (parent)
	{
		rv = parent->getAncestors();
		rv.push_back(parent);
	}
	return rv;
}
Node* Node::getLastCommonAncestor(Node *bComponent)
{
	auto listA = getAncestors();
	auto listB = bComponent ? bComponent->getAncestors() : std::vector<Node *>();

	Node *currentLCA = nullptr;
	size_t count = std::min(listA.size(), listB.size());
	for (size_t i = 0; i < count; i++)
	{
		if (listA[i] == listB[i])
			currentLCA = listB[i];
	}
	return currentLCA;
}