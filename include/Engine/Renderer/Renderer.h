#pragma once

struct RendererParams {
    int x, y;
};

class Renderer {
public:
    virtual void init(const RendererParams &params) = 0;
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
};