#include "framework.h"
#include "SalterCalculator.h"
#include <string>
#include <vector>
#include <random>  
#include <ctime>
#include <windows.h>

#define MAX_LOADSTRING 100
#define IDC_CALCULATE_BUTTON 101 // Button ID
#define IDC_RESULT_TEXT 102 // Result text ID
#define IDC_FORMULA_TEXT 135 // Formula text ID
#define IDC_CHECKBOX_MANABURST 103 // Checkbox ID for Mana Burst
#define IDC_CHECKBOX_ELECTRICITYBUFF 104 // Checkbox ID for Electricity Buff
#define IDC_CHECKBOX_VORTIGERN 105 // Checkbox ID for Hammer
#define IDC_CHECKBOX_CRIT 106 // Checkbox ID for crit hits
#define IDC_CHECKBOX_DREADMARSHAL 107 // Checkbox ID for Draed Marshal buff
#define IDC_CHECKBOX_UNKOWNBUFF 108 // Checkbox ID for Unknown buff
#define IDC_CHECKBOX_IMPACTFULGLYPHBUFF 109 // Checkbox ID for Impactful Glyph buff
#define IDC_CHECKBOX_UNHOLYHOLYGLYPHBUFF 110 // Checkbox ID for Unholy/Holy Glyph buff
#define IDC_CHECKBOX_UNHOLYHOLYTARGETBUFF 111 // Checkbox ID for Unholy/Holy Target buff
#define IDC_CHECKBOX_BRILLIANTGLYPHBUFF 112 // Checkbox ID for Brilliant Glyph buff
#define IDC_CHECKBOX_BRILLIANTFIENDBUFF 113 // Checkbox ID for Brillaint Fiend buff
#define IDC_CHECKBOX_BRILLIANTUNDEADBUFF 114 // Checkbox ID for Brillaint Undead buff
#define IDC_CHECKBOX_BOOSTRUNEBUFF 115 // Checkbox ID for Boost Rune buff
#define IDC_CRIT_TEXT 116 // Crit effects Text ID
#define IDC_CHECKBOX_FORCEFULBONUS 117 // Checkbox ID for Forceful Bonus
#define IDC_BUTTON_ATTACK_2 118 // Button ID for 2nd attack
#define IDC_BUTTON_ATTACK_3 119 // Button ID for 3rd attack
#define IDC_CHECKBOX_INVISIBILITY_STACKS 120 // Checkbox ID for Invisibility Stacks
#define IDC_INVISIBILITY_STACKS_TEXT 121 // Text ID for how many Invisibility stacks are on
#define IDC_INVISIBILITY_STACKS_UP 122 // Button ID for increasing the stacks
#define IDC_INVISIBILITY_STACKS_DOWN 123 // Button ID for decreasing the stacks





// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Global toggle states
bool manaBurstActive = true;
bool electricityBuffActive = true;
bool vortigernBuffActive = false;
bool dreadMarshalBuffActive = false;
bool unknownBuffActive = true;
bool impactfulGlyphBuffActive = true;
bool unholyholyGlyphBuffActive = true;
bool unholyholyTargetBuffActive = false;
bool brillaintGlyphBuffActive = true;
bool brillaintFiendBuffActive = false;
bool brillaintUndeadBuffActive = false;
bool boostRuneActive = false;
bool criticalHit = false;
bool forcefulBonusActive = false;
bool secondAttackActive = false;
bool thirdAttackActive = false;
bool invisibilityStacksActive = false;

// Random number generator
std::mt19937 rng(std::random_device{}());  

// Function prototypes for damage calculation
int rollDice(int numberOfDice, int diceType);

// Global variables for calculation
int level = 11;
int strModifier = 6;
int weaponSpecialization = 2;
int dreadMarshal = 0;
int forcefulBonus = 0;
int invisibilityStacks = 0;
int loops = 1;
int numberOfd4 = 0;
int numberOfd6 = 0;
int numberOfd8 = 0;
int numberOfd10 = 0;
int numberOfd12 = 0;
int numberOfd20 = 0;
int totalNumberOfD = 10;




// Entry point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_ LPWSTR    lpCmdLine,
                    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SALTERCALCULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SALTERCALCULATOR));

    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

// Register the window class
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SALTERCALCULATOR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SALTERCALCULATOR);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

// Initialize the instance
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 530, 770, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    // Button
    HWND hButton = CreateWindow(
        L"BUTTON",   // Predefined class; Unicode assumed 
        L"roll",     // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        210,         // x position 
        10,          // y position 
        100,         // Button width
        30,          // Button height
        hWnd,        // Parent window
        (HMENU)IDC_CALCULATE_BUTTON,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    // Static text control to display the formula
    HWND hFormulaStatic = CreateWindow(
        L"STATIC",
        L"",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        140, 50, 250, 30,
        hWnd,
        (HMENU)IDC_FORMULA_TEXT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    // Static text control to display the result
    HWND hResultStatic = CreateWindow(
        L"STATIC",
        L"Result: ",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        140, 90, 250, 30,
        hWnd,
        (HMENU)IDC_RESULT_TEXT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    // checkboxes for all the effects n shit
    HWND hCheckboxManaBurst = CreateWindow(
        L"BUTTON",
        L"Mana Burst",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 130, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_MANABURST,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxElectricityBuff = CreateWindow(
        L"BUTTON",
        L"Electricity Buff",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 170, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_ELECTRICITYBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    HWND hCheckboxVortigern = CreateWindow(
        L"BUTTON",
        L"Vortigern Hammer",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 210, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_VORTIGERN,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    HWND hCheckboxCrit = CreateWindow(
        L"BUTTON",
        L"Critical Hit",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 250, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_CRIT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxDreadMarshalBuff = CreateWindow(
        L"BUTTON",
        L"Dread Marshal Buff",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 290, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_DREADMARSHAL,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxUnkownBuff = CreateWindow(
        L"BUTTON",
        L"Unknown",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 330, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_UNKOWNBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxImpactGlyphBuff = CreateWindow(
        L"BUTTON",
        L"Impactful Glyph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 370, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_IMPACTFULGLYPHBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxUnholyHolyGlyphBuff = CreateWindow(
        L"BUTTON",
        L"Unholy/Holy Glyph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 410, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_UNHOLYHOLYGLYPHBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxUnholyHolyTargetBuff = CreateWindow(
        L"BUTTON",
        L"Unholy/Holy Target",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 450, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_UNHOLYHOLYTARGETBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxBrilliantGlyphBuff = CreateWindow(
        L"BUTTON",
        L"Brillaint Glyph",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 490, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_BRILLIANTGLYPHBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxBrilliantFiendBuff = CreateWindow(
        L"BUTTON",
        L"Brillaint Fiend",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 530, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_BRILLIANTFIENDBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxBrilliantUndeadBuff = CreateWindow(
        L"BUTTON",
        L"Brillaint Undead",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 570, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_BRILLIANTUNDEADBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxBoostRunedBuff = CreateWindow(
        L"BUTTON",
        L"Boost Rune",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 610, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_BOOSTRUNEBUFF,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hCheckboxForcfulBonus = CreateWindow(
        L"BUTTON",
        L"Forceful Bonus",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        20, 650, 150, 30,
        hWnd,
        (HMENU)IDC_CHECKBOX_FORCEFULBONUS,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hButtonAttack2 = CreateWindow(
        L"BUTTON",
        L"2",
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
        200, 650, 30, 30, 
        hWnd,
        (HMENU)IDC_BUTTON_ATTACK_2,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);

    HWND hButtonAttack3 = CreateWindow(
        L"BUTTON",
        L"3",
        WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON,
        240, 650, 30, 30, 
        hWnd,
        (HMENU)IDC_BUTTON_ATTACK_3,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);
    HWND hInvisibilityStackUp = CreateWindow(
        L"BUTTON",
        L"∧",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        220, 570, 30, 30,
        hWnd, (HMENU)IDC_INVISIBILITY_STACKS_UP,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    HWND hInvisibilityStackDown = CreateWindow(
        L"BUTTON",
        L"v",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        260, 570, 30, 30,
        hWnd, (HMENU)IDC_INVISIBILITY_STACKS_DOWN,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    HWND hInvisibilityStackText = CreateWindow(
        L"STATIC",
        L"0",  
        WS_VISIBLE | WS_CHILD | SS_CENTER ,
        300, 570, 30, 30,
        hWnd, (HMENU)IDC_INVISIBILITY_STACKS_TEXT,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    HWND hCheckboxInvisibilityStacks = CreateWindow(
        L"BUTTON",
        L"Invisibility Stacks",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
        340, 570, 140, 30,
        hWnd, (HMENU)IDC_CHECKBOX_INVISIBILITY_STACKS,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

    // Initialize formula display
    std::wstring formula;

    SetWindowText(GetDlgItem(hWnd, IDC_FORMULA_TEXT), formula.c_str());

    // Initially checked things
    CheckDlgButton(hWnd, IDC_CHECKBOX_MANABURST, BST_CHECKED);
    CheckDlgButton(hWnd, IDC_CHECKBOX_ELECTRICITYBUFF, BST_CHECKED);
    CheckDlgButton(hWnd, IDC_CHECKBOX_UNHOLYHOLYGLYPHBUFF, BST_CHECKED);
    CheckDlgButton(hWnd, IDC_CHECKBOX_BRILLIANTGLYPHBUFF, BST_CHECKED);
    CheckDlgButton(hWnd, IDC_CHECKBOX_IMPACTFULGLYPHBUFF, BST_CHECKED);
    CheckDlgButton(hWnd, IDC_CHECKBOX_UNKOWNBUFF, BST_CHECKED);

    ShowWindow(hWnd, nCmdShow);
    ShowWindow(hButtonAttack2, SW_HIDE);
    ShowWindow(hButtonAttack3, SW_HIDE);
    UpdateWindow(hWnd);

    return TRUE;
}
// Base Weapon damage depending on crit 
int getBaseWeaponDamage(bool critState)
{
    int damage = 0;
    switch (critState) 
    {
    case true:
        damage = rollDice(5, 20);
        break;
    case false:
        damage = rollDice(4, 12);
        break;
    }
    return damage;
}


int rollDice(int numberOfDice, int diceType) {
    int result = 0;
    switch (boostRuneActive) 
    {
    case true:
        for (int i = 0; i < numberOfDice; ++i) {
            result += diceType; // Rolling max dice 
        }
        break;
    case false:
        for (int i = 0; i < numberOfDice; ++i) {
            result += (std::rand() % diceType) + 1; // Rolling diceType-sided dice
        }
        break;
    }
    return result;
}

int calculateDamageAndUpdateFormula(HWND hWnd) {
    int totalDamage = strModifier + weaponSpecialization;
    // Formula substrings n shit idk
    std::wstring formula;
    std::wstring diceFormula;
    std::wstring multiplierFormula;

    if (dreadMarshalBuffActive)
    {
        dreadMarshal = 9;
        totalDamage += dreadMarshal;
    }
    if (vortigernBuffActive) {
        totalDamage += rollDice(2, 6);
        numberOfd6 += 2;
        loops = 2;
    }
    // While loop for double dice if hammer is checked
    while (loops >= 1)
    {
        totalDamage += getBaseWeaponDamage(criticalHit);

        if (criticalHit) 
        {
            numberOfd20 += 5;
        }
        else 
        {
            numberOfd12 += 4;
        }

        if (manaBurstActive) 
        {
            totalDamage += rollDice(1, 4);
            numberOfd4 += 1;
        }

        if (electricityBuffActive) 
        {
            totalDamage += rollDice(1, 4);
            numberOfd4 += 1;
        }
        
        if (unknownBuffActive)
        {
            totalDamage += rollDice(1, 4);
            numberOfd4 += 1;
        }
        if (impactfulGlyphBuffActive)
        {
            totalDamage += rollDice(1, 6);
            numberOfd6 += 1;
        }
         
        if (unholyholyGlyphBuffActive)
        {
            totalDamage += rollDice(1, 4);
            numberOfd4 += 1;
        }
        
        if (unholyholyTargetBuffActive)
        {
            totalDamage += rollDice(2, 4);
            numberOfd4 += 2;
        }
        
        if (brillaintGlyphBuffActive)
        {
            totalDamage += rollDice(1, 4);
            numberOfd4 += 1;
        }
        if (brillaintFiendBuffActive)
        {
            totalDamage += rollDice(1, 4);
            numberOfd4 += 1;
        }
        if (brillaintUndeadBuffActive)
        {
            totalDamage += rollDice(1, 4);
            numberOfd4 += 1;
        }
        if (invisibilityStacksActive && invisibilityStacks > 0) {
            
            totalDamage += rollDice((level/2) * invisibilityStacks, 8);
            numberOfd8 += (level/2) * invisibilityStacks;
        }
        loops--; // Im stupid
        
    }

    //forceful bonus dice
    totalNumberOfD = numberOfd4 + numberOfd6 + numberOfd8 + numberOfd10 + numberOfd12 + numberOfd20;
    // Yes U know this is very weirdly ordered
    if (forcefulBonusActive)
    {
        if (secondAttackActive)
        {
            forcefulBonus = totalNumberOfD;
        }
        else
        {
            forcefulBonus = totalNumberOfD * 2;
        }

        totalDamage += forcefulBonus; 
    }

    // All em numbers n shit
    if (numberOfd20 != 0)
    {
        diceFormula += std::to_wstring(numberOfd20);
        diceFormula += L"d20 + ";
    }
    if (numberOfd12 != 0)
    {
        diceFormula += std::to_wstring(numberOfd12);
        diceFormula += L"d12 + ";
    }
    if (numberOfd10 != 0)
    {
        diceFormula += std::to_wstring(numberOfd10);
        diceFormula += L"d10 + ";
    }
    if (numberOfd8 != 0)
    {
        diceFormula += std::to_wstring(numberOfd8);
        diceFormula += L"d8 + ";
    }
    if (numberOfd6 != 0)
    {
        diceFormula += std::to_wstring(numberOfd6);
        diceFormula += L"d6 + ";
    }
    if ( numberOfd4 != 0)
    {
        diceFormula += std::to_wstring(numberOfd4);
        diceFormula += L"d4 + ";
    }
    if (criticalHit)
    {
        totalDamage = totalDamage * 2;
        multiplierFormula += L" × 2";
    }
    // Final formula
    formula = diceFormula + std::to_wstring( strModifier + weaponSpecialization + dreadMarshal + forcefulBonus) + multiplierFormula;
    // Resetting values
    loops = 1;
    numberOfd4 = 0;
    numberOfd6 = 0;
    numberOfd8 = 0;
    numberOfd10 = 0;
    numberOfd12 = 0;
    numberOfd20 = 0;
    dreadMarshal = 0;
    // Update the formula display
    SetWindowText(GetDlgItem(hWnd, IDC_FORMULA_TEXT), formula.c_str());
    return totalDamage;
}

// Window procedure for handling messages
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_CALCULATE_BUTTON:
        {
            // Calculate damage and update the formula display
            int damage = calculateDamageAndUpdateFormula(hWnd);
            std::wstring resultText = L"Result: " + std::to_wstring(damage);
            SetWindowText(GetDlgItem(hWnd, IDC_RESULT_TEXT), resultText.c_str());
        }
        break;
        case IDC_BUTTON_ATTACK_2: 
        {
            // Set them balls
            forcefulBonus = totalNumberOfD;
            secondAttackActive = true;
            thirdAttackActive = false;

            // Update UI to SHOW which button is active
            EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_2), FALSE);
            EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_3), TRUE);

            calculateDamageAndUpdateFormula(hWnd);
        }
        break;

        case IDC_BUTTON_ATTACK_3:
        {
            // Set the them balls
            secondAttackActive = false;
            thirdAttackActive = true;

            // Update UI to SHOW which button is active
            EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_2), TRUE);
            EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_3), FALSE);

            calculateDamageAndUpdateFormula(hWnd);
        }
        break;

        case IDC_INVISIBILITY_STACKS_UP:
        {
            // Increase stacks by 1
            invisibilityStacks++;

            // Update the stack count display
            std::wstring stackText = std::to_wstring(invisibilityStacks);
            SetWindowText(GetDlgItem(hWnd, IDC_INVISIBILITY_STACKS_TEXT), stackText.c_str());

            // Update the formula display based on current states
            calculateDamageAndUpdateFormula(hWnd);
        }
        break;

        case IDC_INVISIBILITY_STACKS_DOWN:
        {
            // Decrease stacks by 1 but no 0
            if (invisibilityStacks > 0) {
                invisibilityStacks--;
            }

            // Update the stack count display
            std::wstring stackText = std::to_wstring(invisibilityStacks);
            SetWindowText(GetDlgItem(hWnd, IDC_INVISIBILITY_STACKS_TEXT), stackText.c_str());

            // Update the formula display based on current states
            calculateDamageAndUpdateFormula(hWnd);
        }
        case IDC_CHECKBOX_MANABURST:
        case IDC_CHECKBOX_ELECTRICITYBUFF:
        case IDC_CHECKBOX_VORTIGERN:
        case IDC_CHECKBOX_CRIT:
        case IDC_CHECKBOX_DREADMARSHAL:
        case IDC_CHECKBOX_UNKOWNBUFF:
        case IDC_CHECKBOX_IMPACTFULGLYPHBUFF:
        case IDC_CHECKBOX_UNHOLYHOLYGLYPHBUFF:
        case IDC_CHECKBOX_UNHOLYHOLYTARGETBUFF:
        case IDC_CHECKBOX_BRILLIANTGLYPHBUFF:
        case IDC_CHECKBOX_BRILLIANTUNDEADBUFF:
        case IDC_CHECKBOX_BRILLIANTFIENDBUFF:
        case IDC_CHECKBOX_BOOSTRUNEBUFF:
        case IDC_CHECKBOX_FORCEFULBONUS:
        case IDC_CHECKBOX_INVISIBILITY_STACKS:
        {
            // Update the states of the checkboxes
            dreadMarshalBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_DREADMARSHAL);
            manaBurstActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_MANABURST);
            electricityBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_ELECTRICITYBUFF);
            vortigernBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_VORTIGERN);
            criticalHit = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_CRIT);
            unknownBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_UNKOWNBUFF);
            impactfulGlyphBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_IMPACTFULGLYPHBUFF);
            unholyholyGlyphBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_UNHOLYHOLYGLYPHBUFF);
            unholyholyTargetBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_UNHOLYHOLYTARGETBUFF);
            brillaintGlyphBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_BRILLIANTGLYPHBUFF);
            brillaintFiendBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_BRILLIANTUNDEADBUFF);
            brillaintUndeadBuffActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_BRILLIANTFIENDBUFF);
            boostRuneActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_BOOSTRUNEBUFF);
            invisibilityStacksActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_INVISIBILITY_STACKS); 
            forcefulBonusActive = IsDlgButtonChecked(hWnd, IDC_CHECKBOX_FORCEFULBONUS);

            if (forcefulBonusActive) {
                // Show the buttons if forceful bonus is active
                ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_2), SW_SHOW);
                ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_3), SW_SHOW);
                secondAttackActive = true;
                thirdAttackActive = false;

            }
            else {
                // Hide the buttons if forceful bonus is not active
                ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_2), SW_HIDE);
                ShowWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_3), SW_HIDE);
                secondAttackActive = false;
                thirdAttackActive = false;
                EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_2), TRUE);
                EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ATTACK_3), TRUE);
            }

            // Update the formula display based on current states
            calculateDamageAndUpdateFormula(hWnd);
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Set background color for the box
        HBRUSH hBrush = CreateSolidBrush(RGB(45, 45, 45)); //background color
        HBRUSH lBrush = CreateSolidBrush(RGB(35, 35, 35));

        FillRect(hdc, &ps.rcPaint, lBrush);

        RECT rect = { 200, 130, 490, 550 }; // Box size

        // Fill the box
        FillRect(hdc, &rect, hBrush);

        // Clean up the brush
        DeleteObject(hBrush);

        // background mode to transparent or we get ugly white box behind text
        SetBkMode(hdc, TRANSPARENT);

        // Font settibngs
        HFONT hFont = CreateFont(
            19,                        // Height of the font (size)
            0,                         // Width of the font (0 means default)
            0,                         // Escapement (rotation angle, 0 means no rotation)
            0,                         // Orientation angle
            FW_BOLD,                   // Font weight (FW_NORMAL for normal, FW_BOLD for bold)
            FALSE,                     // Italic
            FALSE,                     // Underline
            FALSE,                     // Strikeout
            ANSI_CHARSET,              // Character set
            OUT_DEFAULT_PRECIS,        // Output precision
            CLIP_DEFAULT_PRECIS,       // Clipping precision
            DEFAULT_QUALITY,           // Output quality
            DEFAULT_PITCH | FF_SWISS,  // Pitch and family
            L"Gill Sans MT");          // Font type 

        // Select the new font
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        // Drawing Crit effects inside the box
        SetTextColor(hdc, RGB(190, 249, 190));
        TextOut(hdc, 210, 140, L"Crit Effects:", 12);

        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 210, 170, L" Target is Off-Guard until the start", 37);
        TextOut(hdc, 210, 190, L" of your next turn.", 19);

        TextOut(hdc, 210, 220, L" Target becomes clumsy 2 and enfeebled 2", 39);
        TextOut(hdc, 210, 240, L" until the end of your next turn.", 33);

        TextOut(hdc, 210, 270, L" Target must succeed at a DC 29 Fortitude", 42);
        TextOut(hdc, 210, 290, L" save or be blinded for 1 round.", 32);

        TextOut(hdc, 210, 320, L" Choose to force the target to succeed", 38);
        TextOut(hdc, 210, 340, L" at a DC 27 Fortitude save or be pushed", 39);
        TextOut(hdc, 210, 360, L" 5 feet away from you.", 23);

        SetTextColor(hdc, RGB(169, 240, 254));
        TextOut(hdc, 210, 390, L" Target Undead is also enfeebled 1 until", 40);
        TextOut(hdc, 210, 410, L" the end of your next turn. (Saber Only)", 42);

        SetTextColor(hdc, RGB(219, 87, 87));
        TextOut(hdc, 210, 440, L" Target becomes frightened 1. (Salter Only)", 45);

        SetTextColor(hdc, RGB(190, 249, 190));
        TextOut(hdc, 210, 470, L"On Kill Effects:", 17);

        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 210, 500, L" Stride up to 15 feet. This movement ", 40);
        TextOut(hdc, 210, 520, L" doesn't trigger reactions.", 29);
        

        

        // Clean up
        EndPaint(hWnd, &ps);
        return 0;
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
