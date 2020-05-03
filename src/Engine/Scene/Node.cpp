#include "Engine/Scene/Node.h"

Node::Node()
{
  parentNode = nullptr;
  nodeType = BASE;
}

Node::Node(Node* parentNode)
{
  id = rand() % 1000;
  this->parentNode = parentNode;
  parentNode->addChild(this);
}

Node::~Node() {
  return;
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

glm::mat4 Node::getTransform()
{
  return translation * rotation * scale;
}

NODE_TYPE Node::getNodeType() {
  return nodeType;
}