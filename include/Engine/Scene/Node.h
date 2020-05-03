#pragma once

#include <map>
#include <stdlib.h>

#include "glm/glm.hpp"
#include "spdlog/spdlog.h"

#include "Engine/common/CommonIncludes.h"

typedef enum {
  BASE = 0,
  ACTOR = 1
} NODE_TYPE;

class Node {
protected:
  NODE_TYPE nodeType;
  uint16_t id;
  Node *parentNode;
  std::map<uint16_t, Node *> childNodes;

  glm::mat4 translation = glm::mat4(1.0f);
  glm::mat4 rotation = glm::mat4(1.0f);
  glm::mat4 scale = glm::mat4(1.0f);

public:
  Node();
  Node(Node *parentNode);
  virtual ~Node();

  void addChild(Node *childNode);

  glm::mat4 getTransform();

  void killChild(uint16_t childId);
  void killChildren();

  uint16_t getId();

  NODE_TYPE getNodeType();

  virtual void update() = 0;
};