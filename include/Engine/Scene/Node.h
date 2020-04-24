#pragma once

#include <map>
#include <stdlib.h>

#include "glm/glm.hpp"
#include "spdlog/spdlog.h"

#include "Engine/common/CommonIncludes.h"

class Node {
private:
  uint16_t id;
  Node *parentNode;
  std::map<uint16_t, Node *> childNodes;

  glm::mat4 transform;

public:
  Node();
  Node(Node *parentNode);
  ~Node();

  void addChild(Node *childNode);

  void killChild(uint16_t childId);
  void killChildren();

  uint16_t getId();
};