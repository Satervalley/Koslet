#pragma once


class CRegistry
{
public:
	enum cregRestoreEnum
	{
		regVolatile = REG_WHOLE_HIVE_VOLATILE,
		regRefresh = REG_REFRESH_HIVE,
		regNoLazy = REG_NO_LAZY_FLUSH
	};

	HKEY classesRoot{ HKEY_CLASSES_ROOT };
	HKEY currentUser{ HKEY_CURRENT_USER };
	HKEY localMachine{ HKEY_LOCAL_MACHINE };
	HKEY currentConfig{ HKEY_CURRENT_CONFIG };
	HKEY users{ HKEY_USERS };
	HKEY performanceData{ HKEY_PERFORMANCE_DATA };	//Windows NT/2000
	HKEY dynData{ HKEY_DYN_DATA };						//Windows 95/98

	CRegistry()
		{
			m_hKey = NULL;
		};

	~CRegistry()
		{
			CloseKey();
			m_hKey = NULL;
		};

	BOOL OpenKey(HKEY hKey, LPCTSTR szKey);
	BOOL CreateKey(HKEY hKey, LPCTSTR szKey);
	BOOL DeleteKey(HKEY hKey, LPCTSTR szKey);
	BOOL DeleteValue(LPCTSTR lpValueName);
	BOOL GetValue(LPCTSTR lpValueName, CString& strValue);
	BOOL GetValue(LPCTSTR lpValueName, DWORD& dwValue);
	BOOL SetValue(LPCTSTR lpValueName, LPCTSTR lpData);
	BOOL SetValue(LPCTSTR lpValueName, DWORD dwValue);
	BOOL SaveKey(LPCTSTR lpszFileName);
	BOOL RestoreKey(LPCTSTR lpszFileName, DWORD dwFlag);
	BOOL LoadKey(HKEY hKey, LPCTSTR lpszSubKey, LPCTSTR lpszFileName);
	void CloseKey();	

protected:
	HKEY m_hKey;
};