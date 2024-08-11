#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <windowsx.h>
//#include <wingdi.h>
#include <commctrl.h>

#include <process.h>

#include <stdio.h>

#include "resource.h"

#pragma comment (lib, "Comctl32.lib")

#define WINDOW_WIDTH 275
#define WINDOW_HEIGHT 247

HINSTANCE g_hInst;
HWND g_hDlg;

HWND hwndCheckBox_Aspect;

#define IDC_MYCHECKBOX_ASPECT 103

bool lButtonDown_checkbox_aspect = true;

HWND hwndStaticText_Aspect;
HWND hwndStaticText_AspectWH;
HWND hwndText_Result;

#define IDC_MYSTATICTEXT_ASPECT 110
#define IDC_MYSTATICTEXT_ASPECTWH 111
#define IDC_MYSTATICTEXT_RESULT 116

HWND hwndEdit_AspectW;
HWND hwndEdit_AspectH;

#define IDC_MYEDIT_ASPECTW 106
#define IDC_MYEDIT_ASPECTH 107

HWND hwndButton_Apply;
#define IDC_MYBUTTON_APPLY 112

HWND hwndButton_Exit;
#define IDC_MYBUTTON_EXIT 114

HWND hwndTextCaption;
#define IDC_MYSTATICTEXT_CAPTION 110


HWND hwndButtonExit_Title;
#define IDC_MYBUTTON_EXIT_TITLE 115

enum {FIRST_SCREEN, AFTER_APPLY_RESULT};

int nStatus = FIRST_SCREEN;

struct THREADPARMS
{
    float f_aspect;
	char *widescreen_res;
	char *file_name;
};


LRESULT CALLBACK CheckBox_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HDC hDC;
	PAINTSTRUCT ps;
	HBRUSH   hBrush = NULL;
	RECT rect;

	bool bRedraw = false;

    WNDPROC Wndproc=(WNDPROC)(LONG_PTR)GetWindowLong(hWnd,GWL_USERDATA);
    //if(Wndproc)
    //{
        switch(uMsg)
        {
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);

			GetClientRect(hWnd, &rect);

			hBrush = ::CreateSolidBrush( RGB( 87, 114, 144) );
			if (!hBrush) return 0;

			::FillRect(hDC, &rect, hBrush);

		    ::DeleteObject( hBrush );
			
			rect.top = rect.top + 3;
			rect.left = rect.left + 3;
			rect.right = rect.right - 3;
			rect.bottom = rect.bottom - 3;

			hBrush = ::CreateSolidBrush( RGB( 184, 190, 202) );
			if (!hBrush) return 0;

			::FillRect(hDC, &rect, hBrush);

		    ::DeleteObject( hBrush );

			if(lButtonDown_checkbox_aspect && hWnd == hwndCheckBox_Aspect)
			{

				bRedraw = true;
			}
		

			if(bRedraw)
			{
				rect.top = rect.top + 3;
				rect.left = rect.left + 3;
				rect.right = rect.right - 3;
				rect.bottom = rect.bottom - 3;

				hBrush = ::CreateSolidBrush( RGB( 87, 114, 144) );
				if (!hBrush) return 0;
	
				::FillRect(hDC, &rect, hBrush);

				::DeleteObject( hBrush );
			}
			
			EndPaint(hWnd, &ps);

			break;

		
		case WM_LBUTTONDBLCLK:
        case WM_LBUTTONDOWN:

			if(hWnd == hwndCheckBox_Aspect)
			{
				lButtonDown_checkbox_aspect = lButtonDown_checkbox_aspect ? 0 : 1;
				GetClientRect(hWnd, &rect);
				::InvalidateRect(hWnd, &rect, TRUE);

			}

        break;

        case WM_NCDESTROY:
            SetWindowLong(hWnd,GWL_WNDPROC,(LONG)(LONG_PTR)Wndproc);
            SetWindowLong(hWnd,GWL_USERDATA,0);
            break;
        }
        return CallWindowProc(Wndproc,hWnd,uMsg,wParam,lParam);
    //}
    //return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CALLBACK Edit_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	HBRUSH   hBrush = NULL;
	RECT rect;
	const char *szBuff;

    WNDPROC Wndproc=(WNDPROC)(LONG_PTR)GetWindowLong(hWnd,GWL_USERDATA);
    //if(Wndproc)
    //{
        switch(uMsg)
        {
		case WM_NCPAINT:
				hDC = GetWindowDC(hWnd);
			    
				GetWindowRect(hWnd, &rect);
				::OffsetRect(&rect, -rect.left, -rect.top);
				
				hBrush = ::CreateSolidBrush( RGB( 87, 114, 144) );
				if (!hBrush) return 0;
				
				::FillRect(hDC, &rect, hBrush);
				::DeleteObject( hBrush );
				ReleaseDC(hWnd, hDC);
				
				return 0;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			
			if(hWnd == hwndEdit_AspectW)
			{
				szBuff="16";
				SendMessage(hWnd, WM_SETTEXT, 0, LPARAM(szBuff));
			}

			if(hWnd == hwndEdit_AspectH)
			{
				szBuff="9";
				SendMessage(hWnd, WM_SETTEXT, 0, LPARAM(szBuff));
			}
			
				
 
			EndPaint(hWnd, &ps);
			break;
 
        case WM_NCDESTROY:
            SetWindowLong(hWnd,GWL_WNDPROC,(LONG)(LONG_PTR)Wndproc);
            SetWindowLong(hWnd,GWL_USERDATA,0);
            break;
        }
        return CallWindowProc(Wndproc,hWnd,uMsg,wParam,lParam);
    //}
    //return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CALLBACK TextCaption_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HBRUSH   hBrush = NULL, hOldBrush = NULL;

	POINT pt;
	RECT rs;

    WNDPROC Wndproc=(WNDPROC)(LONG_PTR)GetWindowLong(hWnd,GWL_USERDATA);
    //if(Wndproc)
    //{
        switch(uMsg)
        {
        case WM_LBUTTONDOWN:
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			rs.left = 0;
			rs.top = 0;
			rs.right = 570;
			rs.bottom = 22;

			if(PtInRect(&rs, pt))
			{
				SendMessage(g_hDlg, WM_SYSCOMMAND, SC_MOVE|0x0002, 0);
			}


        break;

		case WM_LBUTTONUP:

		break;
 
        case WM_NCDESTROY:
            SetWindowLong(hWnd,GWL_WNDPROC,(LONG)(LONG_PTR)Wndproc);
            SetWindowLong(hWnd,GWL_USERDATA,0);
            break;
        }
        return CallWindowProc(Wndproc,hWnd,uMsg,wParam,lParam);
    //}
    //return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void __cdecl Thread_Func1(void * pParam)
{
	THREADPARMS* pThreadParms = (THREADPARMS* )pParam;

	float f_aspect =   pThreadParms->f_aspect;

	bool bWScreen = false;

	FILE *fp;

	errno_t err;


	OPENFILENAME ofn;
	char szFile[256];
	char szFileTitle[256];

	strcpy_s(szFileTitle, 256, "\0");

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.nFilterIndex=1;
	ofn.lpstrFile=szFile;
	ofn.nMaxFile=sizeof(szFile);
	ofn.lpstrFileTitle=(szFileTitle);
	ofn.nMaxFileTitle=sizeof(szFileTitle);
	ofn.Flags=OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle="Open File";
	ofn.lpstrFilter="Executable files *.exe\0*.exe\0";
	szFile[0]='\0';
	if (!GetOpenFileName(&ofn)) return;

	strcpy_s(pThreadParms->file_name, 256, szFileTitle);

	err  = fopen_s(&fp, pThreadParms->file_name, "r+b");
	if( err == 0 )
    {
	}
	

	UINT n_seek_pos = 0;

	fseek(fp, 0, SEEK_END);
	
	DWORD dwFileSize = ftell(fp);

	rewind(fp);

	BYTE *buff = new BYTE[dwFileSize];

	fread((BYTE*)buff, dwFileSize, 1, fp);

	if(hwndCheckBox_Aspect && f_aspect!=0.0f)
	{
		UINT i = 0;
		do
		{

			int n_buff;
			n_buff = buff[i+3] << 24 | buff[i + 2] << 16 | buff[i+1] << 8 | buff[i];
			
			char szBuff[256];
			sprintf_s(szBuff, 256, "%x", n_buff);

			
			if(n_buff == 0x3FAAAAAB)
			{

				BYTE data[4];
				memcpy(data, &f_aspect, sizeof (f_aspect));

				buff[i + 0] = data[0];
				buff[i + 1] = data[1];
				buff[i + 2] = data[2];
				buff[i + 3] = data[3];

				strcpy_s(pThreadParms->widescreen_res, 256, "Widescreen correction OK");
				bWScreen = true;

				break;
				
			}
			i++;

		} while (i < dwFileSize);
	}

	rewind(fp);
	fwrite(buff, dwFileSize , 1, fp);
	fclose(fp);
	
	if(!bWScreen)
		strcpy_s(pThreadParms->widescreen_res, 256, "Widescreen correction not used");
	
	_endthread();
}


INT_PTR CALLBACK WndProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH   hBrush = NULL;
	POINT pt[6];
	HRGN hRgn;
	POINT pt1;
	RECT rs;
	HANDLE m_fonthandle;
	HRSRC res;
	HFONT hFont;
	const char *szBuff = "";
	LPDRAWITEMSTRUCT lpDrawItemStruct;
	UINT state;
	int nMode;
	int nColor;
	SIZE size;
	HPEN hPen, hOldPen;
	   
    switch(uMsg)
    {

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				//обрабатываем нажатие на кнопку закрытия
				//в правом верхнем углу заголовка окна
				case IDC_MYBUTTON_EXIT_TITLE:
						DestroyWindow(hDlg);
						break;
				
				//обрабатываем нажатие на кнопку exit
				//появляется после нажатия на кнопку apply
				case IDC_MYBUTTON_EXIT:
						DestroyWindow(hDlg);
						break;
				
				//обрабатываем нажатие на кнопку apply
				//в главном окне приложения
				case IDC_MYBUTTON_APPLY:
				
				THREADPARMS* pThreadParms = new THREADPARMS;
				ZeroMemory(pThreadParms, sizeof(THREADPARMS));
				
				pThreadParms->file_name = new char[256];
				pThreadParms->file_name[0] = 0;

				pThreadParms->widescreen_res = new char[256];
				pThreadParms->widescreen_res[0] = 0;

				char szBuff_W[256];
				::SendMessage(hwndEdit_AspectW, WM_GETTEXT, 256, LPARAM(szBuff_W));

				char szBuff_H[256];
				::SendMessage(hwndEdit_AspectH, WM_GETTEXT, 256, LPARAM(szBuff_H));

				UINT nw = atoi(szBuff_W);
				UINT nh = atoi(szBuff_H);

				if( strlen(szBuff_W) == 0 || strlen(szBuff_H) == 0 )
					pThreadParms->f_aspect = 0.0f;
				else
					pThreadParms->f_aspect =  (float) nw / nh;
	
				HANDLE hThread;

				hThread = (HANDLE)_beginthread(Thread_Func1, 0, (LPVOID)pThreadParms);
				WaitForSingleObject(hThread, INFINITE);
				//CloseHandle(hThread);

				DestroyWindow(hwndStaticText_Aspect);
				DestroyWindow(hwndStaticText_AspectWH);

				DestroyWindow(hwndEdit_AspectW);
				DestroyWindow(hwndEdit_AspectH);

				DestroyWindow(hwndCheckBox_Aspect);
				
				DestroyWindow(hwndButton_Apply);
				
				//создаем фонт для кнопки exit
				hFont = CreateFont(-15,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,FALSE,FALSE,
                FALSE,0, FALSE,"Exo 2 Light");

				//создаем кнопку закрытия окна на месте кнопки appy
				//обрабатываем нажатие в WM_COMMAND
				hwndButton_Exit=CreateWindowEx(0,"Button", "",
					WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_OWNERDRAW|BS_PUSHBUTTON,
					143, 183, 107, 42,hDlg,
					(HMENU)IDC_MYBUTTON_EXIT,g_hInst,NULL);
 
				if(!hwndButton_Exit)
				{
					MessageBox(NULL,"Button создать не удалось", "Info",MB_OK);
					return -1;
				}

				//устанавливаем фонт для кнопки exit
				SendMessage(hwndButton_Exit, WM_SETFONT, WPARAM (hFont), TRUE);

				if(strlen(pThreadParms->file_name) == 0)
				{
					szBuff = "No files\r\nMaybe file is busy";
				}
				else
				{
					char szBuff_t[256];
					sprintf_s(szBuff_t, 256, "Done %s!",
					pThreadParms->file_name);
					//sprintf_s(szBuff_t, 256, "Done %s!\r\n\r\n%s\r\n",
					//pThreadParms->file_name,
					//pThreadParms->widescreen_res);

					szBuff = szBuff_t;
				}
					
					

				hwndText_Result=CreateWindowEx(0,"Static", "",
					WS_CHILD|WS_TABSTOP|WS_VISIBLE | SS_LEFT | SS_NOTIFY,
					40,60,150,70,hDlg,(HMENU)(UINT_PTR)IDC_MYSTATICTEXT_RESULT,g_hInst,NULL);
 
				if(!hwndText_Result)
				{
						MessageBox(NULL,"Static создать не удалось", "Info",MB_OK);
						return -1;
				}
 
    
				SendMessage(hwndText_Result, WM_SETFONT, WPARAM (hFont), TRUE);
	
	
				SendMessage(hwndText_Result, WM_SETTEXT, 0, LPARAM(szBuff));
				
				//рисуем окно результата в WM_PAINT
				nStatus = AFTER_APPLY_RESULT;

				::InvalidateRect(hDlg, NULL, true);

			}


			return (INT_PTR)TRUE;


		//создаем фонт для текста и edit
		//создаем check box три штуки
		//создаем static для текстовой подписи каждого check box
		//создаем надпись в заголовке окна
		//создаем кнопку закрытия в заголовке окна
		//создаем кнопку apply
		case WM_INITDIALOG:
			//в ресурсах ищем фонт
			res = FindResource(g_hInst, MAKEINTRESOURCE(IDR_MY_FONT1),"MY_FONT");

			//создаем фонт из ресурсов программы
			if (res) 
			{
				HGLOBAL mem = LoadResource(g_hInst, res);
				void *data = LockResource(mem);
				size_t len = SizeofResource(g_hInst, res);

				DWORD nFonts;
				m_fonthandle = AddFontMemResourceEx(
				data,       	// font resource
				(int) len,       	// number of bytes in font resource 
				NULL,          	// Reserved. Must be 0.
				&nFonts      	// number of fonts installed
				);

				if(m_fonthandle==0)
				{
					MessageBox(hDlg, "Font add fails", "Error", MB_OK);
				}
			}

			
			
			//создаем окно check box для aspect на базе static
			hwndCheckBox_Aspect=CreateWindowEx(0,"Static", "",
				WS_CHILD|WS_TABSTOP|WS_VISIBLE | SS_LEFT | SS_NOTIFY,
				17, 35, 25, 25,hDlg,(HMENU)(UINT_PTR)IDC_MYCHECKBOX_ASPECT,g_hInst,NULL);
 
			if(!hwndCheckBox_Aspect)
			{
				MessageBox(NULL,"Static создать не удалось", "Info",MB_OK);
				return -1;
			}
 
			// Установить новую функцию окна
			SetWindowLong(hwndCheckBox_Aspect,GWL_USERDATA,GetWindowLong(hwndCheckBox_Aspect,GWL_WNDPROC));
			SetWindowLong(hwndCheckBox_Aspect,GWL_WNDPROC,(LONG)(LONG_PTR)CheckBox_WndProc);
 
			//создаем фонт что бы выводить текст в static окнах
			//dist, fov, aspect hw
			hFont = CreateFont(-15,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,FALSE,FALSE,
                FALSE,0, FALSE,"Exo 2 Light");
			
			//на базе static создаем текст для Aspect
			//в сообщении WM_CTLCOLORSTATIC закрасим цвет фона для текста
			hwndStaticText_Aspect=CreateWindowEx(0,"Static", "",
				WS_CHILD|WS_TABSTOP|WS_VISIBLE | SS_LEFT | SS_NOTIFY,
				67, 35, 180, 53,hDlg,(HMENU)(UINT_PTR)IDC_MYSTATICTEXT_ASPECT,g_hInst,NULL);
 
			if(!hwndStaticText_Aspect)
			{
				MessageBox(NULL, "Static создать не удалось", "Info",MB_OK);
				return -1;
			}
 
			//устанавливаем фонт для static текста Aspect
			SendMessage(hwndStaticText_Aspect, WM_SETFONT, WPARAM (hFont), TRUE);
	
			//устанавливаем текст для static Aspect
			szBuff = "Widescreen aspect\r\nratio default game value is 4:3";
			SendMessage(hwndStaticText_Aspect, WM_SETTEXT, 0, LPARAM(szBuff));

	
			//на базе static создаем текст для AspectWH
			//в сообщении WM_CTLCOLORSTATIC закрасим цвет фона для текста
		    hwndStaticText_AspectWH=CreateWindowEx(0,"Static", "",
		        WS_CHILD|WS_TABSTOP|WS_VISIBLE | SS_LEFT | SS_NOTIFY,
				74, 92, 125, 70,hDlg,(HMENU)(UINT_PTR)IDC_MYSTATICTEXT_ASPECTWH,g_hInst,NULL);
 
			if(!hwndStaticText_AspectWH)
			{
				MessageBox(NULL,"Static создать не удалось", "Info",MB_OK);
				return -1;
			}
			
			//устанавливаем фонт для static текста AspectWH
			SendMessage(hwndStaticText_AspectWH, WM_SETFONT, WPARAM (hFont), TRUE);

			//устанавливаем текст для static AspectWH
			szBuff = "Display width\r\n\r\nDisplay height";
			SendMessage(hwndStaticText_AspectWH, WM_SETTEXT, 0, LPARAM(szBuff));
			
			//создаем edit для aspect W
			//в сообщении WM_CTLCOLOREDIT закрашиваем фон для edit aspect W
		    hwndEdit_AspectW=CreateWindowEx(WS_EX_CLIENTEDGE,"Edit", "16",
				WS_CHILD|WS_TABSTOP|WS_VISIBLE | WS_BORDER|ES_LEFT,
				202, 83, 45,32,hDlg,(HMENU)IDC_MYEDIT_ASPECTW,g_hInst,NULL);
 
			if(!hwndEdit_AspectW)
			{
				MessageBox(NULL,"Edit создать не удалось", "Info",MB_OK);
				return -1;
			}
 
			// Установить функцию обработки сообщений
			SetWindowLong(hwndEdit_AspectW,GWL_USERDATA,GetWindowLong(hwndEdit_AspectW,GWL_WNDPROC));
			SetWindowLong(hwndEdit_AspectW,GWL_WNDPROC,(LONG)(LONG_PTR)Edit_WndProc);
		
			//устанавливаем фонт
			SendMessage(hwndEdit_AspectW, WM_SETFONT, WPARAM (hFont), TRUE);



			//создаем edit для aspect H
			//в сообщении WM_CTLCOLOREDIT закрашиваем фон для edit aspect H
		    hwndEdit_AspectH=CreateWindowEx(WS_EX_CLIENTEDGE,"Edit", "9",
				WS_CHILD|WS_TABSTOP|WS_VISIBLE | WS_BORDER|ES_LEFT,
				202, 122, 45,32,hDlg,(HMENU)IDC_MYEDIT_ASPECTH,g_hInst,NULL);
 
			if(!hwndEdit_AspectH)
			{
				MessageBox(NULL,"Edit создать не удалось", "Info",MB_OK);
				return -1;
			}
 
			// Установить функцию обработки сообщений
			SetWindowLong(hwndEdit_AspectH,GWL_USERDATA,GetWindowLong(hwndEdit_AspectH,GWL_WNDPROC));
			SetWindowLong(hwndEdit_AspectH,GWL_WNDPROC,(LONG)(LONG_PTR)Edit_WndProc);

			//устанавливаем фонт
			SendMessage(hwndEdit_AspectH, WM_SETFONT, WPARAM (hFont), TRUE);


			//создаем в гл.окне кнопку apply
			//сама кнопка рисуется в сообщении WM_DRAWITEM
			//обрабатывается в WM_COMMAND
			hwndButton_Apply=CreateWindowEx(0,"Button", "",
				WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_OWNERDRAW|BS_PUSHBUTTON,
				143, 183, 107, 42,hDlg,
				(HMENU)IDC_MYBUTTON_APPLY,g_hInst,NULL);
 
			if(!hwndButton_Apply)
			{
				MessageBox(NULL,"Button создать не удалось", "Info",MB_OK);
				return -1;
			}
			//устанавливаем фонт для кнопки
			SendMessage(hwndButton_Apply, WM_SETFONT, WPARAM (hFont), TRUE);
	

			//создаем фонт для текста в заголовке окна
			hFont = CreateFont(-13,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,FALSE,FALSE,
                FALSE,0, FALSE,"Exo 2 Light");


			//создаем staic text для заголовка окна
		    hwndTextCaption=CreateWindowEx(0,"Static", "",
		        WS_CHILD|WS_TABSTOP|WS_VISIBLE | SS_LEFT | SS_NOTIFY,
				13, 4, 245, 17,hDlg,(HMENU)(UINT_PTR)IDC_MYSTATICTEXT_CAPTION,g_hInst,NULL);
 
			if(!hwndTextCaption)
			{
				MessageBox(NULL,"Static создать не удалось", "Info",MB_OK);
				return -1;
			}
 
			// Установить оконную процедуру нажимать будем
			// левую кнопку мыши на static для перемещения окна (заголовок)
			SetWindowLong(hwndTextCaption,GWL_USERDATA,GetWindowLong(hwndTextCaption,GWL_WNDPROC));
			SetWindowLong(hwndTextCaption,GWL_WNDPROC,(LONG)(LONG_PTR)TextCaption_WndProc);
			
			//установить фонт для текста
			SendMessage(hwndTextCaption, WM_SETFONT, WPARAM (hFont), TRUE);
	
			//установить текст
			szBuff = "Old Games Widescreen v1.22";
			SendMessage(hwndTextCaption, WM_SETTEXT, 0, LPARAM(szBuff));

			//создаем кнопку закрытия справа в заголовке окна
			//рисуем саму кнопку в сообщении WM_DRAWITEM
			//обрабатывается в WM_COMMAND
			hwndButtonExit_Title=CreateWindowEx(0,"Button", "",
				WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_OWNERDRAW|BS_PUSHBUTTON,
				253, 3, 16, 16,hDlg,
				(HMENU)IDC_MYBUTTON_EXIT_TITLE,g_hInst,NULL);
 
			if(!hwndButtonExit_Title)
			{
				MessageBox(hDlg,"Button создать не удалось", "Info",MB_OK);
				return -1;
			}
 

			return (INT_PTR)TRUE;

		case WM_CTLCOLORSTATIC:
			//текст у нас рисуется на static
			//закрашиваем фон текста для текста Dist, Fov, Aspect hw
			if( ((HWND)lParam == hwndStaticText_Aspect) || ((HWND)lParam == hwndStaticText_AspectWH)
			|| ((HWND)lParam == hwndText_Result) )
			{
				hDC = (HDC) wParam;
				::SetTextColor(hDC, RGB(0, 0, 0));
				::SetBkMode(hDC, TRANSPARENT);

				hBrush = ::CreateSolidBrush(RGB(175, 207, 206));

				return (LRESULT) hBrush;				
			}

		//закрашиваем фон для edit dist, fov, aspect wh
		case WM_CTLCOLOREDIT:
			hDC = (HDC) wParam;
			::SetTextColor(hDC, RGB(0, 0, 0));
			::SetBkMode(hDC, TRANSPARENT);

			hBrush = ::CreateSolidBrush(RGB( 184, 190, 202));

			return (LRESULT) hBrush;

		
		case WM_DRAWITEM:

			//рисуем кнопку выхода справа в заголовке окна
			if (LOWORD(wParam) == IDC_MYBUTTON_EXIT_TITLE )
			{
				lpDrawItemStruct = (LPDRAWITEMSTRUCT )lParam;
				hDC = lpDrawItemStruct->hDC;
				rect=lpDrawItemStruct->rcItem;
				state=lpDrawItemStruct->itemState;

				if(state&ODS_SELECTED)
				{
					hBrush = CreateSolidBrush(RGB(184, 190, 202));
					::FillRect(hDC, &rect, hBrush);
					hPen = ::CreatePen(PS_SOLID, 2, RGB(87, 114, 144));
					hOldPen = (HPEN)::SelectObject(hDC, hPen);
					POINT pt;
					::MoveToEx(hDC, 5,5, &pt);
					::LineTo(hDC, 11,11);

					::MoveToEx(hDC, 11,5, &pt);
					::LineTo(hDC, 5,11);

				}
				else 
				{

					hBrush = CreateSolidBrush(RGB(184, 190, 202));
					::FillRect(hDC, &rect, hBrush);
					hPen = ::CreatePen(PS_SOLID, 2, RGB(0,0,0));
					hOldPen = (HPEN)::SelectObject(hDC, hPen);
					POINT pt;
					::MoveToEx(hDC, 5,5, &pt);
					::LineTo(hDC, 11,11);

					::MoveToEx(hDC, 5,5, &pt);
					::LineTo(hDC, 11,11);

					::MoveToEx(hDC, 11,5, &pt);
					::LineTo(hDC, 5,11);
				}

				return (INT_PTR)TRUE;
			}

			//рисуем кнопку гл.окна apply и результ.окна кнопка exit
			if( (LOWORD(wParam) == IDC_MYBUTTON_APPLY ) ||
				(LOWORD(wParam) == IDC_MYBUTTON_EXIT ) )
			{
				lpDrawItemStruct = (LPDRAWITEMSTRUCT )lParam;
				hDC = lpDrawItemStruct->hDC;
				rect=lpDrawItemStruct->rcItem;
				state=lpDrawItemStruct->itemState;

				if(state&ODS_SELECTED)
				{	
					::DrawFrameControl(hDC, &rect,DFC_BUTTON, 	DFCS_BUTTONPUSH);
			
					hBrush = ::CreateSolidBrush(RGB(87, 114, 144));
					::FillRect(hDC, &rect, hBrush);
				
					rect.left += 4;
					rect.top += 4;
					rect.right -= 4;
					rect.bottom -= 4;

					hBrush = ::CreateSolidBrush(RGB(184, 190, 202));
					::FillRect(hDC, &rect, hBrush);
				}
				else
				{
					::DrawFrameControl(hDC, &rect,DFC_BUTTON, 	DFCS_BUTTONPUSH);

					hBrush = ::CreateSolidBrush(RGB(87, 114, 144));
					::FillRect(hDC, &rect, hBrush);
			
					rect.left += 2;
					rect.top += 2;
					rect.right -= 6;
					rect.bottom -= 6;

					hBrush = ::CreateSolidBrush(RGB(184, 190, 202));
					::FillRect(hDC, &rect, hBrush);
				}

				nMode = ::SetBkMode(hDC, TRANSPARENT);
				nColor = ::SetTextColor(hDC,RGB(0,0,0));
			
				if (LOWORD(wParam) == IDC_MYBUTTON_APPLY )
					szBuff= "Apply";

				if (LOWORD(wParam) == IDC_MYBUTTON_EXIT )
					szBuff= "Exit";

				GetTextExtentPoint32(hDC, (LPCSTR)szBuff, (int) strlen(szBuff), &size);
				POINT pt;
				pt.x = rect.right / 2 - size.cx / 2;
				pt.y = rect.bottom / 2 - size.cy / 2;

				TextOut(hDC, pt.x,pt.y, szBuff, (int) strlen(szBuff));
		
				SetBkMode(hDC, nMode);
				SetTextColor(hDC,nColor);

				return (INT_PTR)TRUE;

			}

			

			

        case WM_CLOSE:
            DestroyWindow(hDlg);
            return (INT_PTR)TRUE;
 
        case WM_DESTROY:
            PostQuitMessage(0);
			return (INT_PTR)TRUE;

		case WM_PAINT:
			hDC = BeginPaint(hDlg, &ps);
			
			hBrush = ::CreateSolidBrush( RGB( 184, 190, 202) );
			if (!hBrush) return 0;

			GetClientRect(hDlg, &rect);
			::FillRect(hDC, &rect, hBrush);

		    ::DeleteObject( hBrush );

			hBrush = ::CreateSolidBrush( RGB(87, 114, 144) );
			if (!hBrush) return 0;

			GetClientRect(hDlg, &rect);
			rect.top = 22;
			rect.left = 5;
			rect.right = rect.right - 5;
			rect.bottom = rect.bottom - 5;

			::FillRect(hDC, &rect, hBrush);

			::DeleteObject( hBrush );

			//рисуем в окне приложения первый экран
			if(nStatus == FIRST_SCREEN)
			{
				//aspect ratio

				pt[0].x = 10;
				pt[0].y = 28;

				pt[1].x = 265; 
				pt[1].y = 28;

				pt[2].x = 265;
				pt[2].y = 165;

				pt[3].x = 52;
				pt[3].y = 165;

				pt[4].x = 52;
				pt[4].y = 68;

				pt[5].x = 10;
				pt[5].y = 68;

				hRgn = ::CreatePolygonRgn((const POINT *)&pt, 6, ALTERNATE);

				hBrush = ::CreateSolidBrush( RGB(175, 207, 206) );
				if (!hBrush) return 0;

				::FillRgn(hDC, hRgn, hBrush);

				::DeleteObject( hBrush );
				::DeleteObject( hRgn );			
					
				//button apply
				pt[0].x = 110;
				pt[0].y = 171;

				pt[1].x = 265;
				pt[1].y = 171;

				pt[2].x = 265;
				pt[2].y = 238;

				pt[3].x = 110;
				pt[3].y = 238;

				hRgn = ::CreatePolygonRgn((const POINT *)&pt, 4, ALTERNATE);

				hBrush = ::CreateSolidBrush( RGB(175, 207, 206) );
				if (!hBrush) return 0;

				::FillRgn(hDC, hRgn, hBrush);

				::DeleteObject( hBrush );
				::DeleteObject( hRgn );

			}
			//рисуем в окне приложения result screen
			else if(nStatus == AFTER_APPLY_RESULT)
			{
				//тут выводим текст
				pt[0].x = 10;
				pt[0].y = 50;

				pt[1].x = 245;
				pt[1].y = 50;

				pt[2].x = 245;
				pt[2].y = 150;

				pt[3].x = 10;
				pt[3].y = 150;

				hRgn = ::CreatePolygonRgn((const POINT *)&pt, 4, ALTERNATE);
	    
				hBrush = ::CreateSolidBrush( RGB(175, 207, 206) );
				if (!hBrush) return 0;

				::FillRgn(hDC, hRgn, hBrush);

				DeleteObject(hBrush);
				
				//тут расположена кнопка
				pt[0].x = 110;
				pt[0].y = 171;

				pt[1].x = 265;
				pt[1].y = 171;

				pt[2].x = 265;
				pt[2].y = 238;

				pt[3].x = 110;
				pt[3].y = 238;

				hRgn = ::CreatePolygonRgn((const POINT *)&pt, 4, ALTERNATE);
				
				hBrush = ::CreateSolidBrush( RGB(175, 207, 206) );
				if (!hBrush) return 0;

				::FillRgn(hDC, hRgn, hBrush);

				DeleteObject(hBrush);

			}

			EndPaint(hDlg, &ps);
			
			return (INT_PTR)TRUE;

			//перемещаем окно держа мышей за заголовок
			case WM_LBUTTONDOWN:

				pt1.x = LOWORD(lParam);
				pt1.y = HIWORD(lParam);

				rs.left = 0;
				rs.top = 0;
				rs.right = 570;
				rs.bottom = 22;

				if(PtInRect(&rs, pt1))
				{
					SendMessage(hDlg, WM_SYSCOMMAND, SC_MOVE|0x0002, 0);
				}

				return (INT_PTR)TRUE;

	}

	return (INT_PTR)FALSE;
}
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_hInst = hInstance;

	g_hDlg = CreateDialog(hInstance, 
				MAKEINTRESOURCE(DLG_MAIN), 
				NULL, (DLGPROC) WndProc); 

	if(!g_hDlg) return 0;

	DWORD dwStyle = GetWindowLong(g_hDlg,GWL_STYLE);
	dwStyle &= ~(WS_CAPTION|WS_SYSMENU|WS_BORDER);
	SetWindowLong(g_hDlg,GWL_STYLE,dwStyle);
	
	int nWidth  = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	int r_width = (nWidth - WINDOW_WIDTH) / 2;
	int r_height = (nHeight - WINDOW_HEIGHT) / 2;

	::MoveWindow(g_hDlg,r_width, r_height + 1, WINDOW_WIDTH, WINDOW_HEIGHT, true);

	ShowWindow(g_hDlg, nCmdShow);

	InvalidateRect(g_hDlg, NULL, true);

	UpdateWindow(g_hDlg);

	InitCommonControls();

	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;

}
 
