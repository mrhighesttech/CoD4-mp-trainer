#include <windows.h>
#include <tlhelp32.h>
#include "resource.h"

BOOL isGameRunning;

/** Start of Declarations here **/
BOOL GetProcessList( );
DWORD GetModuleBaseBaseAddress(DWORD iProcId, char* DLLName); 
// Below is the about text that is shown when "About" button is clicked

char *about   =
"This a simple external trainer for Call of Duty 4\n"
"which I wrote in early 2011.  It is a simple cheat that\n"
"operates by overwriting code and variables in the iw3mp.exe \n"
"proccess memory space.  No anti-cheat countermeasures are taken\n"
"in this implementation.  For use offline and in anti-cheat\n"
"insecure servers only. \n"
"NOTE: NumLock must be on to activate toggles\n";
/////////////////////////////////////////////////////////////////////

char *gameWindow = "iw3mp.exe"; // exe name here
DWORD pid; HWND hwndWindow; DWORD bytes; HANDLE hand = NULL;

DWORD dwCoD4Base = 0x00400000;  // Base address for Call of Duty 4's executable code is always the same (at least as up to patch 1.7)

DWORD nametags_team_jump_offset = 0x0002E1AC;  // controls name tags drawn based on team in team based game modes
DWORD nametags_los_jump_offset = 0x00002E1CE;  // controls name tags drawn based on line of sight 
//DWORD nametags_ffa_jump_offset = 0x0002E18F;   // controls name tags drawn in free for all mode (regardless of player entity team)


DWORD wallhack_offset = 0x00045480;
DWORD minimap_jump_offset = 0x0002A0B4;

DWORD fog_jump_offset = 0x0023DB0D;
DWORD recoil_jump_offset = 0x00057CCF;
DWORD flashbang_jump_offset= 0x00051FB9;

DWORD gun_spread_jump_offset = 0x00016C7B;
DWORD laser_sight_jump_offset = 0x00056E78;
DWORD dev_console_jump_offset = 0x0016B386;


BOOL IsHack1On, IsHack2On, IsHack3On, IsHack4On, IsHack5On, IsHack6On, IsHack7On, IsHack8On, IsHack9On, FirstTime1;

BYTE nametags_team_code[6] = { 0x0F, 0x84, 0xCE, 0x00, 0x00, 0x00 };
BYTE nametags_los_code[2] = { 0x75, 0x25 };
//BYTE nametags_ffa_code[6] = { 0x0F, 0x85, 0xEB, 0x00, 0x00, 0x00 };
BYTE wallhack_code[2] = {0x6A, 0x12};
BYTE minimap_nops[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
BYTE no_fog_hack_code[2] = { 0x74, 0x20 };
BYTE no_recoil_hack_code[2] = { 0x75, 0x12 };
BYTE flashbang_code[2] = { 0x75, 0x29 };
BYTE gun_spread_code[2] = { 0x74, 0x15 };
BYTE laser_sight_code[2] = { 0x74, 0x04 };
BYTE dev_console_unlock_code[2] = { 0x74, 0x19 };

BYTE original_nametags_team[6] = { 0 };
BYTE original_nametags_los[2] = { 0 };
//BYTE original_nametags_ffa[6] = { 0 };
BYTE original_wall[2] = { 0 }; 
BYTE original_minimap[5] = { 0 };
BYTE original_fog[2] = { 0 }; 
BYTE original_recoil[2] = { 0 };
BYTE original_flash[2] = { 0 };
BYTE original_spread[2] = { 0 };
BYTE original_laser[2] = { 0 };
BYTE original_console[2] = { 0 };


void aboutButton(HWND hwnd)
{
	MessageBox(hwnd,about,"About",MB_ICONINFORMATION);
}

void Initialize(HWND hwnd,WPARAM wParam, LPARAM lParam) {
	GetProcessList();

	FirstTime1=TRUE;

	IsHack1On=FALSE; 
	IsHack2On=FALSE; 
	IsHack3On=FALSE;
	IsHack4On=FALSE;
	IsHack5On=FALSE;
	IsHack6On=FALSE;
	IsHack7On=FALSE;
	IsHack8On=FALSE;
	IsHack9On=FALSE;


	if(isGameRunning==TRUE)
	{		 
         GetWindowThreadProcessId(hwndWindow, &pid);
		 hand = OpenProcess(PROCESS_ALL_ACCESS,0,pid);
		 SetTimer(hwnd, 1, 500, NULL);	//Timer speed is 500ms, you can change it here
	} 
	else 
	{ //Error message for when game not found in process list
		MessageBox(NULL, "Warning! iw3mp.exe not detected, please run the game before running the hacks", "Error", MB_OK + MB_ICONWARNING);
	}
}

void HookExe() //This function ensures we are attatched to the game at all times
{
	
	CloseHandle(hand);
    GetProcessList( );
    GetWindowThreadProcessId(hwndWindow, &pid);
	hand = OpenProcess(PROCESS_ALL_ACCESS,0,pid);

}

	/*----- Here comes the good stuff -----*/


void timerCall() //functions in here run according to timer above
{
		HookExe(); //Call to function above (game always attatched)



	if(FirstTime1==TRUE) //checks to see if this is the first time its run, if it is continue
	{
		ReadProcessMemory(hand, (void*)(dwCoD4Base + nametags_team_jump_offset) , &original_nametags_team, 6, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + nametags_los_jump_offset), &original_nametags_los, 2, &bytes);
		//ReadProcessMemory(hand, (void*)(dwCoD4Base + nametags_ffa_jump_offset), &original_nametags_ffa, 6, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + wallhack_offset) , &original_wall, 2, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + minimap_jump_offset) , &original_minimap, 5, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + fog_jump_offset) , &original_fog, 2, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + recoil_jump_offset) , &original_recoil, 2, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + flashbang_jump_offset) , &original_flash, 2, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + gun_spread_jump_offset) , &original_spread, 2, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + laser_sight_jump_offset) , &original_laser, 2, &bytes);
		ReadProcessMemory(hand, (void*)(dwCoD4Base + dev_console_jump_offset) , &original_console, 2, &bytes);

		FirstTime1=FALSE;
	}


///////////////////////////////////////////////////////////////////////////
/////Start Hotkey Functions Below


	if(GetAsyncKeyState(VK_NUMPAD1))
	{			
		   
		if(IsHack1On==FALSE) 
			{  
				WriteProcessMemory(hand, (void*)(dwCoD4Base + nametags_team_jump_offset) , &nametags_team_code, 6, &bytes);
				WriteProcessMemory(hand, (void*)(dwCoD4Base + nametags_los_jump_offset), &nametags_los_code, 2, &bytes);
				//WriteProcessMemory(hand, (void*)(dwCoD4Base + nametags_ffa_jump_offset), &nametags_ffa_code, 6, &bytes);  //TODO detect game mode and toggle approiate code  
				IsHack1On=TRUE; 
			}
			else
			{
			
				WriteProcessMemory(hand, (void*)(dwCoD4Base + nametags_team_jump_offset) , &original_nametags_team, 6, &bytes);
				WriteProcessMemory(hand, (void*)(dwCoD4Base + nametags_los_jump_offset), &nametags_los_code, 2, &bytes);
				//WriteProcessMemory(hand, (void*)(dwCoD4Base + nametags_ffa_jump_offset), &original_nametags_ffa, 6, &bytes);
				IsHack1On=FALSE; 
			}
		}
     		
	if(GetAsyncKeyState(VK_NUMPAD2)) 
	{			
		   
		if(IsHack2On==FALSE)
			{  
				WriteProcessMemory(hand, (void*)(dwCoD4Base + wallhack_offset), &wallhack_code, 2, &bytes);
				IsHack2On=TRUE; 
			}
			else
			{
				WriteProcessMemory(hand, (void*)(dwCoD4Base + wallhack_offset), &original_wall, 2, &bytes);
				IsHack2On=FALSE; 
			}
		
	}

	if(GetAsyncKeyState(VK_NUMPAD3))
	{			

		if (IsHack3On == FALSE)
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + minimap_jump_offset), &minimap_nops, 5, &bytes);
			IsHack3On = TRUE;
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + minimap_jump_offset), &original_minimap, 5, &bytes);
			IsHack3On = FALSE;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD4))
	{

		if (IsHack4On == FALSE)
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + fog_jump_offset), &no_fog_hack_code, 2, &bytes);
			IsHack4On = TRUE;
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + fog_jump_offset), &original_fog, 2, &bytes);
			IsHack4On = FALSE;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD5))
	{

		if (IsHack5On == FALSE)
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + recoil_jump_offset), &no_recoil_hack_code, 2, &bytes);
			IsHack5On = TRUE;
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + recoil_jump_offset), &original_recoil, 2, &bytes);
			IsHack5On = FALSE;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD6))
	{

		if (IsHack6On == FALSE)
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + flashbang_jump_offset), &flashbang_code, 2, &bytes);
			IsHack6On = TRUE;
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + flashbang_jump_offset), &original_flash, 2, &bytes);
			IsHack6On = FALSE;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD7))
	{

		if (IsHack7On == FALSE)
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + gun_spread_jump_offset), &gun_spread_code, 2, &bytes);
			IsHack7On = TRUE;
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + gun_spread_jump_offset), &original_spread, 2, &bytes);
			IsHack7On = FALSE;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD8))
	{

		if (IsHack8On == FALSE)
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + laser_sight_jump_offset), &laser_sight_code, 2, &bytes);
			IsHack8On = TRUE;
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + laser_sight_jump_offset), &original_laser, 2, &bytes);
			IsHack8On = FALSE;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD9))
	{

		if (IsHack9On == FALSE)
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + dev_console_jump_offset), &dev_console_unlock_code, 2, &bytes);
			IsHack9On = TRUE;
		}
		else
		{
			WriteProcessMemory(hand, (void*)(dwCoD4Base + dev_console_jump_offset), &original_console, 2, &bytes);
			IsHack9On = FALSE;
		}
	}
}
DWORD GetModuleBaseBaseAddress(DWORD iProcId, char* DLLName)
{
	HANDLE hSnap;
	MODULEENTRY32 xModule;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, iProcId);
	xModule.dwSize = sizeof(MODULEENTRY32);
	if (Module32Next(hSnap,&xModule))
	{
		while(Module32Next(hSnap,&xModule))
		{
			if(strcmp(xModule.szModule, DLLName) == 0)
			{
				CloseHandle(hSnap);
				return (DWORD)xModule.modBaseAddr;
			}
		}			
	}
	CloseHandle(hSnap);
	return (DWORD)0;

}

BOOL GetProcessList( )
{
  HANDLE hProcessSnap;
  HANDLE hProcess;
  PROCESSENTRY32 pe32;
  DWORD dwPriorityClass;
  int PidTest;
  isGameRunning=FALSE;
 
  
  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( hProcessSnap == INVALID_HANDLE_VALUE ) return( FALSE );
  

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if( !Process32First( hProcessSnap, &pe32 ) )
  {
    CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
    return( FALSE );
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  
  do
  {
    // Retrieve the priority class.
    dwPriorityClass = 0;
    hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
    if( hProcess != NULL )
    {
      dwPriorityClass = GetPriorityClass( hProcess );
      if( !dwPriorityClass )
        
      CloseHandle( hProcess );
    }

    PidTest=strcmp(gameWindow, pe32.szExeFile);
	if(PidTest==0){ pid=pe32.th32ProcessID; isGameRunning=TRUE;}

  } while( Process32Next( hProcessSnap, &pe32 ) );

  // Don't forget to clean up the snapshot object!
  CloseHandle( hProcessSnap );
  return( TRUE );
}

BOOL CALLBACK DialogProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
	{
		case WM_INITDIALOG:
			Initialize(hwnd,wParam,lParam);
			return TRUE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_ABOUT:
					aboutButton(hwnd);
					return TRUE;

				case IDC_EXIT:
					EndDialog (hwnd, 0);
					return TRUE;
			}
		return TRUE;

		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;

		case WM_CLOSE:
			PostQuitMessage(0);
			return TRUE;
		case WM_TIMER:
			timerCall();
			return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAINDLG), NULL,DialogProc);
	return 0;
}


