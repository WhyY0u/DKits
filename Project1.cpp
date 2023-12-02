#include "WindowsHelper.h"
#include <Windows.h>
FT_Library ft;
Windows& windows = Windows::getInstance();
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    double xpos, ypos;
    if (uMsg == WM_NCCREATE)
    {
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    switch (uMsg) {

    case WM_RBUTTONUP:
        glfwGetCursorPos(windows.getWindows(), &xpos, &ypos);
        std::cout << "RButtonUp" << std::endl;
        windows.mouseClicked(1, 0);
        break;
    case WM_LBUTTONUP:
        std::cout << "LButtonUp" << std::endl;
        glfwGetCursorPos(windows.getWindows(), &xpos, &ypos);
        windows.mouseClicked(0, 0);
        break;
    case WM_RBUTTONDOWN:
        glfwGetCursorPos(windows.getWindows(), &xpos, &ypos);
        std::cout << "RButtonDown" << std::endl;
        windows.mouseClicked(1, 1);
        break;
    case WM_LBUTTONDOWN:
        glfwGetCursorPos(windows.getWindows(), &xpos, &ypos);
        std::cout << "LButtonDown" << std::endl;
        windows.mouseClicked(0, 1);
        if (windows.getWindows() && glfwGetWindowAttrib(windows.getWindows(), GLFW_HOVERED) && !windows.getFullScreen()) {
            ReleaseCapture();
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        }
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int main() {
    if (windows.EnableGLFW()) {
        return -1;
    }
    if (windows.EnableFontRender()) {
        glfwTerminate();
        return -1;
    }
    windows.clear();
    windows.setWindows(windows.createWindow(20, 20, 857, 640, Color("#111111"), true, "Dissimilate", "null", ft));
    windows.outlinecolor = Color("#3D3D3D");
    windows.setRounded(0.035f);
    windows.setScene(new Menu("Menu"));
    if (!windows.getWindows()) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(windows.getWindows());
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }
    glfwSetFramebufferSizeCallback(windows.getWindows(), framebuffer_size_callback);
    HWND hwnd = GetActiveWindow();
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
    ShowWindow(hwnd, SW_SHOW);
    while (!glfwWindowShouldClose(windows.getWindows())) {
        windows.tick();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        windows.Render2D();
  
        glfwSwapBuffers(windows.getWindows());
        glfwPollEvents();
    

    }

    glfwTerminate();

    return 0;
}
