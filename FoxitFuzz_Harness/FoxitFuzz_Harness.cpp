#pragma warning(disable:4996)
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

typedef int(__thiscall*pFun1)(void * thiscall, int);
typedef int(__thiscall* pFun2)(void* thiscall, wchar_t*);
typedef int(__thiscall* pFun3)(void* thiscall, wchar_t*, int, int, int, int, int, int, int, int, int, int, int);
typedef int(__thiscall* pFun4)(void* thiscall, int, wchar_t*);
pFun1 Function1;
pFun2 Function2;
pFun3 Function3;
pFun4 Function4;


wchar_t tempPath[MAX_PATH];
wchar_t TempPdfToBeCreated[520] = { 0 };
HMODULE hDll = NULL;
DWORD Result = 0;

DWORD unkonwnadd;
DWORD arr[3];
int UselessValue = 1094795585;
void * thispointer;
unsigned int vtable;


wchar_t FilePath[7400] = { 0 }; 
wchar_t* fileName = (wchar_t*)L"Converted.pdf";
wchar_t* printerinfo = (wchar_t*)L"Foxit Reader Printer Version 9.7.0.2220";
wchar_t* printerName = (wchar_t*)L"Foxit Reader PDF Printer";
wchar_t* language = (wchar_t*)L"en-US";

extern "C" __declspec(dllexport)  int main(int argc, char** argv);
extern "C" __declspec(dllexport)  int fuzzme(char*);

typedef unsigned int(__stdcall* CreateFXPDFConvertor)(void);
typedef void (*FunctionPointer)();
CreateFXPDFConvertor obj = NULL;

void callbackfunc()
{
	__asm
	{
		mov eax, ecx	//ECX is the Heap Address of FilePath Variable, EAX is the retun value, and it is expected by the Convertor Plugin. I think foxit developers add this to tackle our fuzzing effort.	
	};
}

typedef struct {
	FunctionPointer funcpointer1;
	FunctionPointer funcpointer2;
}MyUnkownType;

wchar_t* charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = (wchar_t*)malloc(sizeof(wchar_t) * size);
	mbstowcs(wa, text, size);
	return wa;
}

int fuzzme(char* Path)
{
	thispointer = (void *)(obj)();
	vtable = *(unsigned int*)thispointer;
	//printf("\n\t[x] vtable.................................0x%04x",  vtable);
	Function3 = (pFun3)(*(unsigned int*)(vtable));
	Function1 = (pFun1)(*(unsigned int*)(vtable + 4 * 1));
	Function2 = (pFun2)(*(unsigned int*)(vtable + 4 * 2));
	Function4 = (pFun4)(*(unsigned int*)(vtable + 4 * 3));
	//printf("\n\t[x] Calling 0x%04x......................",  Function4);

	Result = Function4(thispointer, 0x2, language);
	if (Result >= 0)
	{
		//printf("Successful!\n\t[x] Calling 0x%04x......................",  Function1);
		Result = Function1(thispointer, 0x2);
		if (Result >= 0)
		{
			//printf("Successful!\n\t[x] Calling 0x%04x......................",  Function2);
			Result = Function2(thispointer, printerName);
			if (Result >= 0)
			{
				//printf("Successful!\n\t[x] Calling 0x%04x......................",  Function3);
				wchar_t* filepath = charToWChar(Path);
				memcpy(FilePath + 0x1624 / 2, filepath, wcslen(filepath) * 2);
				Function3(thispointer, FilePath, unkonwnadd, UselessValue, UselessValue, UselessValue, UselessValue, UselessValue, UselessValue, UselessValue, UselessValue, 0, 0);
			}
			else
			{
				//printf("Unsuccessful!\n");
				return 1;
			}
		}
		else
		{
			//printf("Unsuccessful!\n");
			return 1;
		}
	}
	else
	{
		//printf("Unsuccessful!\n");
		return 1;
	}
	return 0;
}

int __declspec(noinline) _main(int argc, char* argv[]) //Stack Alignment
{
	int a = 0;

	printf("stack alignment = %d\n", ((size_t)&a) % 8);

	return 1;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage %s: <input file>\n", argv[0]);
		return 1;
	}
	size_t a = 0;

	if (((size_t)&a) % 8 != 0)
	{
		printf("alignment != 8\n");
		alloca(4);
	}
	else
	{
		printf("alignment == 8\n");
	}

	//return _main(argc, argv);//Stack Alignment

	wcscpy(tempPath, L"E:\\test\\");//To run it on the server ram
	//GetTempPathW(MAX_PATH, tempPath);		
	memcpy(TempPdfToBeCreated, tempPath, wcslen(tempPath) * 2);
	memcpy(TempPdfToBeCreated + wcslen(tempPath), fileName, wcslen(fileName) * 2);
	//printf("\t[x] Temp Path -> %ls\n",TempPdfToBeCreated);

	//printf("\t[x] Loading ConvertToPDF_x86.dll...........");
	hDll = LoadLibraryA("E:\\ConvertToPDF_x86.dll");

	if (hDll > 0)
	{
		//printf("Successful!\n\t[x] Calling CreateFXPDFConvertor...........");		
		obj = reinterpret_cast<CreateFXPDFConvertor>(GetProcAddress(hDll, "CreateFXPDFConvertor"));
		if (obj != NULL)
		{
			//printf("Successful!");

			//  403c2f30  3ff91590 ConvertToPDF_x86 + 0x31590............................ Fourth and takes 12 arguments (Important,Important,0,0,0,0,0,0,0,0,Important,Important)
			//	403c2f34  3ff92640 ConvertToPDF_x86!GetFileType_ + 0x6d0................. Second and it takes 1 argument which is 0x2
			//	403c2f38  3ff93170 ConvertToPDF_x86!ReleaseFXURLToHtml + 0x270........... Third and takes 1 argument which is "Foxit Reader PDF Printer"
			//	403c2f3c  3ff92fd0 ConvertToPDF_x86!ReleaseFXURLToHtml + 0xd0............ First and takes 2 arguments which is 0x2 and "en-US"
			//	403c2f40  3ff92930 ConvertToPDF_x86!GetFileType_ + 0x9c0
			//	403c2f44  3ff91eb0 ConvertToPDF_x86!DestorFXPDFConvertor + 0xa0
			//	403c2f48  3ff91e50 ConvertToPDF_x86!DestorFXPDFConvertor + 0x40
			//	403c2f4c  3ff92cd0 ConvertToPDF_x86!GetFileType_ + 0xd60
			//	403c2f50  3ff92f30 ConvertToPDF_x86!ReleaseFXURLToHtml + 0x30
			//	403c2f54  3ff92d10 ConvertToPDF_x86!GetFileType_ + 0xda0................. Fifth with zero argument	


			//
			MyUnkownType tt;
			tt.funcpointer1 = &callbackfunc;
			tt.funcpointer2 = &callbackfunc;
			MyUnkownType* t2 = &tt;
			arr[0] = DWORD(t2);
			arr[1] = 0XB;
			arr[2] = 0;
			//int b = 7;
			//memcpy(FilePath + 0x1e4c / 2, (const void *)b, sizeof(int));
			//FilePath[0x1e4c / 2] = b;
			memcpy(FilePath, arr, sizeof(arr));
			memcpy(FilePath + 0xb68 / 2, printerinfo, wcslen(printerinfo) * 2);
			memcpy(FilePath + 0x182C / 2, TempPdfToBeCreated, sizeof(TempPdfToBeCreated));


			fuzzme(argv[1]);
		}
	}
	return 0;
}