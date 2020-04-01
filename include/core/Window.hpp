#pragma once

namespace core {

class Window {
private:
    void *m_windowHandle;
public:
    void init(int width, int height, const char *title);
    ~Window();
};

} // namespace core
