#include "NuimDemoPCH.h"
//#include "NuimDemoPCH.h"
//#include "ImGui/imgui.h"
//#include "ImGui/imgui_impl_win32.h"
//#include "ImGui/imgui_impl_dx11.h"
//#include <d3d11.h>
//#include <tchar.h>
//#include <functional>
//#include "EventSystem.hpp"

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//namespace NuimDemo {
//    class Window {
//        using EventCallback = std::function<void(EventSystem::Event& e)>;
//    public:
//        Window() {
//            WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"NuimDemo", nullptr };
//            ::RegisterClassExW(&wc);
//            hwnd = ::CreateWindowW(wc.lpszClassName, L"NuimDemo", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, this);
//
//            // Show the window
//            ::ShowWindow(hwnd, SW_SHOWDEFAULT);
//            ::UpdateWindow(hwnd);
//        }
//    public:
//        void SetEventCallback(EventCallback callbackFn) {
//            this->eventCallbackFn = callbackFn;
//        }
//        const HWND& GetHWND() { return this->hwnd; }
//        static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//        {
//            if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
//                return true;
//            Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
//            switch (msg) {
//            case WM_CREATE: {
//                LPCREATESTRUCT pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
//                Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
//                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
//                break;
//            }
//            case WM_SIZE: {
//                if (pWindow->eventCallbackFn) {
//                    int width = GET_X_LPARAM(lParam);
//                    int height = GET_Y_LPARAM(lParam);
//                    EventSystem::WindowSizeEvent windowSizeEvent(width, height);
//                    pWindow->eventCallbackFn(windowSizeEvent);
//                }
//                break;
//            }
//            case WM_MOUSELEAVE: {
//
//                break;
//            }
//            case WM_MBUTTONDOWN: {
//                EventSystem::MouseMiddleButtonDown mouseMiddleButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
//                pWindow->eventCallbackFn(mouseMiddleButtonDown);
//                break;
//            }
//            case WM_MBUTTONUP: {
//                EventSystem::MouseMiddleButtonRelease mouseMiddleButtonRelease(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
//                pWindow->eventCallbackFn(mouseMiddleButtonRelease);
//                break;
//            }
//            case WM_LBUTTONDOWN: {
//                EventSystem::MousePressEvent mousePressEvent(EventSystem::MouseButton::NM_LEFT, LOWORD(lParam), HIWORD(lParam));
//                pWindow->eventCallbackFn(mousePressEvent);
//                break;
//            }
//            case WM_LBUTTONUP: {
//                break;
//            }
//            case WM_SETFOCUS: {
//                EventSystem::WindowGotFocusEvent windowGotFocusEvent;
//                if (pWindow->eventCallbackFn) {
//                    pWindow->eventCallbackFn(windowGotFocusEvent);
//                }
//                break;
//            }
//
//            case WM_KILLFOCUS: {
//                EventSystem::WindowLostFocusEvent windowLostFocusEvent;
//                if (pWindow->eventCallbackFn) {
//                    pWindow->eventCallbackFn(windowLostFocusEvent);
//                }
//                break;
//            }
//            case WM_MOUSEMOVE: {
//                int x = (int)(short)GET_X_LPARAM(lParam);
//                int y = (int)(short)GET_Y_LPARAM(lParam);
//                EventSystem::MouseMoveEvent mouseMoveEvent(x, y);
//                pWindow->eventCallbackFn(mouseMoveEvent);
//                break;
//            }
//
//            case WM_KEYDOWN: {
//                EventSystem::KeyPressEvent keyPressEvent(static_cast<int>(wParam));
//                pWindow->eventCallbackFn(keyPressEvent);
//                break;
//            }
//            case WM_DESTROY: {
//                PostQuitMessage(0);
//                break;
//            }
//            case WM_SYSCOMMAND: {
//                switch (wParam & 0xFFF0) {
//                case SC_MINIMIZE: {
//                    EventSystem::WindowMinimizeEvent windowMinimizeEvent;
//                    pWindow->eventCallbackFn(windowMinimizeEvent);
//                    break;
//                }
//                case SC_RESTORE: {
//                    EventSystem::WindowRestoreEvent windowRestoreEvent;
//                    pWindow->eventCallbackFn(windowRestoreEvent);
//                    break;
//                }
//                case SC_MAXIMIZE: {
//                    EventSystem::WindowMaxmimizeEvent windowMaximizeEvent;
//                    pWindow->eventCallbackFn(windowMaximizeEvent);
//                    break;
//                }
//                }
//                break;
//            }
//            case WM_CLOSE: {
//                std::cout << "Closing.." << std::endl;
//                break;
//            }
//            default: {
//                return DefWindowProc(hWnd, msg, wParam, lParam);
//            }
//            
//            }
//            return ::DefWindowProcW(hWnd, msg, wParam, lParam);
//        }
//    private:
//        HWND hwnd;
//        EventCallback eventCallbackFn;
//    };
//
//    class Renderer {
//    public:
//        Renderer(Window* window)  : window(window) {
//            // Initialize Direct3D
//            if (!CreateDeviceD3D(window->GetHWND()))
//            {
//                CleanupDeviceD3D();
//                //::UnregisterClassW(wc.lpszClassName, wc.hInstance);
//            }
//        }
//    public:
//        ID3D11Device* GetDevice() { return g_pd3dDevice; }
//        ID3D11DeviceContext* GetContext() { return g_pd3dDeviceContext; }
//        IDXGISwapChain* GetSwapChain() { return g_pSwapChain; }
//        ID3D11RenderTargetView* GetTargetView() { return g_mainRenderTargetView; }
//        Window* GetWnd() { return window; }
//        bool CreateDeviceD3D(HWND hWnd)
//        {
//            // Setup swap chain
//            DXGI_SWAP_CHAIN_DESC sd;
//            ZeroMemory(&sd, sizeof(sd));
//            sd.BufferCount = 2;
//            sd.BufferDesc.Width = 0;
//            sd.BufferDesc.Height = 0;
//            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//            sd.BufferDesc.RefreshRate.Numerator = 60;
//            sd.BufferDesc.RefreshRate.Denominator = 1;
//            sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//            sd.OutputWindow = hWnd;
//            sd.SampleDesc.Count = 1;
//            sd.SampleDesc.Quality = 0;
//            sd.Windowed = TRUE;
//            sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//
//            UINT createDeviceFlags = 0;
//            //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//            D3D_FEATURE_LEVEL featureLevel;
//            const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
//            HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
//            if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
//                res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
//            if (res != S_OK)
//                return false;
//
//            CreateRenderTarget();
//            return true;
//        }
//
//        void CleanupDeviceD3D()
//        {
//            CleanupRenderTarget();
//            if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
//            if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
//            if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
//        }
//
//        void CreateRenderTarget()
//        {
//            ID3D11Texture2D* pBackBuffer;
//            g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
//            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
//            pBackBuffer->Release();
//        }
//
//        void CleanupRenderTarget()
//        {
//            if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
//        }
//    private:
//        // Data
//        ID3D11Device* g_pd3dDevice = nullptr;
//        ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
//        IDXGISwapChain* g_pSwapChain = nullptr;
//        ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
//        Window* window = nullptr;
//    };
//
//    class ImGuiRenderer {
//    public:
//        ImGuiRenderer(Renderer* renderer) : renderer(renderer) {};
//    public:
//        void Begin() {
//            // Create application window
//            //ImGui_ImplWin32_EnableDpiAwareness();
//
//
//            // Setup Dear ImGui context
//            IMGUI_CHECKVERSION();
//            ImGui::CreateContext();
//            ImGuiIO& io = ImGui::GetIO(); (void)io;
//            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//            // Setup Dear ImGui style
//            ImGui::StyleColorsDark();
//            //ImGui::StyleColorsLight();
//
//            // Setup Platform/Renderer backends
//            ImGui_ImplWin32_Init(renderer->GetWnd()->GetHWND());
//            ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetContext());
//            bool done = false;
//            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//        }
//        void End() {
//            // Cleanup
//            ImGui_ImplDX11_Shutdown();
//            ImGui_ImplWin32_Shutdown();
//            ImGui::DestroyContext();
//        }
//        void OnUpdate() {
//
//            // Start the Dear ImGui frame
//            ImGui_ImplDX11_NewFrame();
//            ImGui_ImplWin32_NewFrame();
//            ImGui::NewFrame();
//
//            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//
//            ImGui::ShowDemoWindow();
//
//
//            // Rendering
//            ImGui::Render();
//            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//            const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
//            auto targetView = renderer->GetTargetView();
//            renderer->GetContext()->OMSetRenderTargets(1, &targetView, nullptr);
//            renderer->GetContext()->ClearRenderTargetView(targetView, clear_color_with_alpha);
//            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//        }
//    private:
//        Renderer* renderer = nullptr;
//    };
//
//    class Application {
//    public:
//        Application(const HINSTANCE& instance) : instance(instance) {};
//    public:
//        void Run() {
//            AllocConsole();
//            freopen("CONOUT$", "w", stdout);
//
//            window = new Window();
//            this->window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
//            renderer = new Renderer(window);
//            
//
//            ImGuiRenderer* layer = new ImGuiRenderer(renderer);
//            layer->Begin();
//
//            bool done = false;
//
//            while (!done) {
//
//                MSG msg;
//                while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
//                {
//                    ::TranslateMessage(&msg);
//                    ::DispatchMessage(&msg);
//                    if (msg.message == WM_QUIT)
//                        done = true;
//                }
//                if (done)
//                    break;
//
//
//                layer->OnUpdate();
//
//                // Present
//                HRESULT hr = renderer->GetSwapChain()->Present(1, 0);   // Present with vsync
//                //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
//
//            }
//            layer->End();
//        }
//    public:
//        void OnEvent(EventSystem::Event& e) {
//            std::cout << e.GetName() << std::endl;
//        }
//    private:
//        Window* window = nullptr;
//        Renderer* renderer = nullptr;
//        HINSTANCE instance;
//    };
//}

//int WINAPI wWinMain(
//    HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    PWSTR  pCmdLine,
//    int nCmdShow
//) {
//    NuimDemo::Application* app = new NuimDemo::Application(hInstance);
//    app->Run();
//    delete app;
//}
