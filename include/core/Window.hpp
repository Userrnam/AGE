#pragma once

namespace core {

class Window {
private:
    void *m_windowHandle;
    void *m_surfaceHandle;
public:
    void init(int width, int height, const char *title);
    ~Window();

    void pollEvents();
    bool closed();
};

} // namespace core
