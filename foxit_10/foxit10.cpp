
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>

typedef void(*pFun1)(int);
typedef void(*pFun2)(int, wchar_t*);
typedef void(*pFun3)(wchar_t*, int, int, int, int, int, int, int, int, int, int, int);
typedef void(*pFun4)(int);
typedef void(*pFun5)(int);
typedef void(*pFun6)();
pFun1 Function1;
pFun2 Function2;
pFun3 Function3;
pFun4 Function4;
pFun5 Function5;
pFun6 Function6;



wchar_t tempPath[MAX_PATH];
wchar_t TempPdfToBeCreated[520] = { 0 };
HMODULE hDll = NULL;
DWORD Result = 0;

DWORD unkonwnadd;
DWORD arr[3];
int UselessValue = 1094795585;
int two = 2;
unsigned int thispointer;
unsigned int vtable;


wchar_t FilePath[7760] = { 0 };
wchar_t* fileName = L"Converted.pdf";
wchar_t* printerinfo = L"Foxit PhantomPDF Printer Version 10.0.0.3443";
wchar_t* language = L"en-US";

extern "C" __declspec(dllexport)  int main(int argc, char** argv);
extern "C" __declspec(dllexport)  int fuzzme(char*);

typedef unsigned int(__stdcall* CreateFXPDFConvertor)(void);
typedef void (*FunctionPointer)();
typedef void (*FunctionPointer2)(void*);
CreateFXPDFConvertor obj = NULL;

void dealloccallbackfunc(void* mem)
{
}

void alloccallbackfunc()
{
	__asm
	{
		mov eax, ecx	//ECX is the Heap Address of FilePath Variable, EAX is the retun value, and it is expected by the Convertor Plugin. I think foxit developers add this to tackle our fuzzing effort.	
	};
}


typedef struct {
	FunctionPointer2 funcpointer1;
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
	thispointer = (unsigned int)(obj)();
	vtable = *(unsigned int*)thispointer;
	//printf("\n\t[x] vtable.................................0x%04x",  vtable);
	Function3 = (pFun3)(*(unsigned int*)(vtable));
	Function2 = (pFun2)(*(unsigned int*)(vtable + 4 * 1));
	Function1 = (pFun1)(*(unsigned int*)(vtable + 4 * 3));
	Function4 = (pFun4)(*(unsigned int*)(vtable + 4 * 9));
	Function5 = (pFun5)(*(unsigned int*)(vtable + 4 * 10));
	Function6 = (pFun6)(*(unsigned int*)(vtable + 4 * 11));
	//printf("\n\t[x] Calling 0x%04x......................",  Function4);
	__asm
	{
		mov ecx, thispointer
		mov edx, vtable
		push language
		push 2
		call Function1
		mov Result, eax
	}
	if (Result >= 0)
	{
		//printf("Successful!\n\t[x] Calling 0x%04x......................",  Function1);
		__asm
		{
			mov ecx, thispointer
			mov edx, vtable
			push 2
			call Function2
			mov Result, eax
		}
		if (Result >= 0)
		{
			//printf("Successful!\n\t[x] Calling 0x%04x......................",  Function3);
			wchar_t* filepath = charToWChar(Path);
			memcpy(FilePath + 0x1624 / 2, filepath, wcslen(filepath) * 2);
			__asm
			{
				mov esi, thispointer
				mov edx, vtable
				push 0
				push 0
				push UselessValue
				push UselessValue
				push UselessValue
				push UselessValue
				push UselessValue
				push UselessValue
				push UselessValue
				push UselessValue
				push 2
				lea ecx, FilePath
				push ecx
				mov ecx, esi
				call Function3
				mov Result, eax

			}
			if (Result >= 0)
			{
				__asm
				{
					mov ecx, thispointer
					mov eax, vtable
					lea ebx, FilePath
					push ebx
					call Function4
					mov Result, eax
				}
				__asm
				{
					mov ecx, thispointer
					mov edx, vtable
					lea ebx, FilePath
					push ebx
					call Function5
					mov Result, eax
				}
				__asm
				{
					mov ecx, thispointer
					mov edx, thispointer
					mov eax, vtable
					lea ebx, FilePath
					call Function6
					mov Result, eax
				}
			}
			else
			{

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
	if (argc < 3)
	{
		printf("Usage %s: <input file> <output file> \n", argv[0]);
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
	fileName = charToWChar(argv[2]);
	memcpy(TempPdfToBeCreated, tempPath, wcslen(tempPath) * 2);
	memcpy(TempPdfToBeCreated + wcslen(tempPath), fileName, wcslen(fileName) * 2);
	//printf("\t[x] Temp Path -> %ls\n",TempPdfToBeCreated);

	//printf("\t[x] Loading ConvertToPDF_x86.dll...........");
	hDll = LoadLibraryA("ConvertToPDF_x86.dll");

	if (hDll > 0)
	{
		//printf("Successful!\n\t[x] Calling CreateFXPDFConvertor...........");		
		obj = reinterpret_cast<CreateFXPDFConvertor>(GetProcAddress(hDll, "CreateFXPDFConvertor"));
		if (obj != NULL)
		{
			//printf("Successful!");

			//  5dbed03c  5d78da70 ConvertToPDF_x86+0x3da70............................ Third and takes 12 arguments (Important,Important,0,0,0,0,0,0,0,0,Important,Important)
			//	5dbed040  5d78fbf0 ConvertToPDF_x86!GetFileType_+0x6d0................. Second and it takes 1 argument which is 0x2
			//	5dbed044  5d790180 ConvertToPDF_x86!GetFileType_+0xc60 
			//	5dbed048  5d790880 ConvertToPDF_x86!ReleaseFXURLToHtml+0xd0............ First and takes 2 arguments which is 0x1 and "en-US"
			//	5dbed04c  5d78fee0 ConvertToPDF_x86!GetFileType_+0x9c
			//	5dbed050  5d78f460 ConvertToPDF_x86!DestorFXPDFConvertor+0xba0
			//	5dbed054  5d78f400 ConvertToPDF_x86!DestorFXPDFConvertor+0xb40
			//	5dbed058  5d790330 ConvertToPDF_x86!GetFileType_+0xe10
			//	5dbed05c  5d7907e0 ConvertToPDF_x86!ReleaseFXURLToHtml+0x30
			//  5dbed060  5d78f210 ConvertToPDF_x86!DestorFXPDFConvertor + 0x950...............fourth fun with 1 arg, which is adreess of filepath
			//  5dbed064  5d78e900 ConvertToPDF_x86!DestorFXPDFConvertor + 0x40............fifth  fun with 1 arg, which is adreess of filepath
			//  5dbed068  5d7905c0 ConvertToPDF_x86!GetFileType_ + 0x10a0...........sixth fun without arg



			//
			MyUnkownType tt;
			tt.funcpointer1 = &dealloccallbackfunc;
			tt.funcpointer2 = &alloccallbackfunc;
			MyUnkownType* t2 = &tt;
			arr[0] = DWORD(t2);
			arr[1] = 0XB;
			arr[2] = 0;
			memcpy(FilePath, arr, sizeof(arr));
			memcpy(FilePath + 0xb68 / 2, printerinfo, wcslen(printerinfo) * 2);
			memcpy(FilePath + 0x182C / 2, TempPdfToBeCreated, sizeof(TempPdfToBeCreated));


			fuzzme(argv[1]);
		}
	}
	return 0;
}