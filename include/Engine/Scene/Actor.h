#pragma once

#include "Engine/Scene/Node.h"

class Actor : public Node {
private:
public:
  Actor();
  Actor(Node *parentNode);
};