#include "Engine/Scene/Node.h"

Node::Node()
{
  parentNode = nullptr;
}

Node::Node(Node* parentNode)
{
  id = rand() % 1000;
  this->parentNode = parentNode;
  parentNode->addChild(this);
}

Node::~Node()
{
  spdlog::debug("deleting node");
  if (parentNode != nullptr)
  {
    parentNode->killChild(id);
  }

  if (!childNodes.empty())
  {
    spdlog::debug("found child nodes, deleting");
    killChildren();
  }
}

void Node::addChild(Node* childNode)
{
  childNodes.insert(std::pair<uint16_t, Node*>(childNode->getId(), childNode));
}

void Node::killChild(uint16_t childId)
{
  auto it = childNodes.find(childId);

  if (it != childNodes.end())
  {
    childNodes.erase(it);
  }
}

void Node::killChildren()
{
  for (auto it = childNodes.begin(); it != childNodes.end(); it++)
  {
    delete it->second;
  }
}

uint16_t Node::getId()
{
  return id;
}