#include "Engine/Scene/Scene.h"

Scene::~Scene() {
  for (int i = 0; i< nodeList.size(); i++) {
    delete nodeList[i];
  }
}

void Scene::addNode(Node* node) {
  nodeList.push_back(node);
}

std::vector<Node*> Scene::getMeshDrawList() {
  std::vector<Node*> drawList;
  for (int i = 0; i < nodeList.size(); i++) {
    if (nodeList[i]->getNodeType() == ACTOR) {
      drawList.push_back(nodeList[i]);
    }
  }
  return drawList;
}

void Scene::update() {
  for (int i = 0; i < nodeList.size(); i++) {
    nodeList[i]->update();
  }
}