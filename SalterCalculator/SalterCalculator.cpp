#include "framework.h"
#include "SalterCalculator.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#define MAX_LOADSTRING 100
#define IDC_CALCULATE_BUTTON 101 // Button ID
#define IDC_RESULT_TEXT 102 // Result text ID

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Function prototypes for damage calculation
int rollDice(int numberOfDice, int diceType);
int calculateDamage();

// Entry point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SALTERCALCULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SALTERCALCULATOR));

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

// Register the window class
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SALTERCALCULATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SALTERCALCULATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

// Initialize the instance
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 500, 300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // Create a button
   HWND hButton = CreateWindow(
       L"BUTTON",  // Predefined class; Unicode assumed 
       L"Calculate",      // Button text 
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
       10,         // x position 
       10,         // y position 
       100,        // Button width
       30,        // Button height
       hWnd,     // Parent window
       (HMENU)IDC_CALCULATE_BUTTON,       // No menu.
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);      // Pointer not needed.

   // Create a static text control to display the result
   HWND hStatic = CreateWindow(
       L"STATIC",
       L"Result: ",
       WS_VISIBLE | WS_CHILD | SS_LEFT,
       10, 50, 300, 30,
       hWnd,
       (HMENU)IDC_RESULT_TEXT,
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Window procedure for handling messages
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            if (wmId == IDC_CALCULATE_BUTTON)
            {
                // Handle the Calculate button click
                int damage = calculateDamage();
                std::wstring resultText = L"Result: " + std::to_wstring(damage);
                SetWindowText(GetDlgItem(hWnd, IDC_RESULT_TEXT), resultText.c_str());
            }
            else
            {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Example function to roll dice (for the GUI button)
int rollDice(int numberOfDice, int diceType) {
    int result = 0;
    for (int i = 0; i < numberOfDice; ++i) {
        result += (std::rand() % diceType) + 1; // Rolling diceType-sided dice
    }
    return result;
}

// Example function to calculate damage
int calculateDamage() {
    std::srand(std::time(0)); // Initialize random seed
    // Add your damage calculation logic here
    int totalDamage = 0;
    totalDamage += rollDice(2, 1); // Example modifier
    totalDamage += rollDice(6, 1); // Example modifier
    return totalDamage;
}
