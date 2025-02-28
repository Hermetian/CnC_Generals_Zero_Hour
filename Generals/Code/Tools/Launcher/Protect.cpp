/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/******************************************************************************
*
* FILE
*     $Archive: /APILauncher/Protect.cpp $
*
* DESCRIPTION
*     Dummy protection implementation. All copy protection has been removed.
*
* PROGRAMMER
*     Denzil E. Long, Jr.
*     $Author: Mcampbell $
*
* VERSION INFO
*     $Modtime: 6/21/01 5:09p $
*     $Revision: 6 $
*
******************************************************************************/

#include "Protect.h"

#ifdef COPY_PROTECT

#include <windows.h>
#include <stdio.h>
#include <Debug\DebugPrint.h>

// Launcher GUID - retained for compatibility
const char* const LAUNCHER_GUID = "FB327081-64F2-43d8-9B72-503C3B765134";
const char* const protectGUID = "7BEB9006-CC19-4aca-913A-C870A88DE01A";

HANDLE mLauncherMutex = NULL;
HANDLE mMappedFile = NULL;

void InitializeProtect(void)
{
	// Copy protection has been removed
	DebugPrint("Protection system disabled\n");
}

void SendProtectMessage(HANDLE process, DWORD threadID)
{
	// Copy protection has been removed
	DebugPrint("Protection verification disabled\n");
}

void ShutdownProtect(void)
{
	// Nothing to do - protection system has been removed
}

#endif // COPY_PROTECT
