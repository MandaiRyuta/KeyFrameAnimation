#ifndef _INPUT_H_
#define _INPUT_H_

//********************************************************************************
//
// インクルード文
//
//********************************************************************************

#include < Windows.h >
#define DIRECTINPUT_VERSION		( 0x0800 )
#include < dinput.h >



//*****************************************************************************
//
// プロトタイプ宣言
//
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

#endif