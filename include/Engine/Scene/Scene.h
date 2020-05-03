#pragma once

#include "Engine/Scene/Node.h"

class Scene {
private:
  std::vector<Node*> nodeList;
public:
  ~Scene();
  
  void addNode(Node* node);

  std::vector<Node*> getMeshDrawList();

  void update();
};