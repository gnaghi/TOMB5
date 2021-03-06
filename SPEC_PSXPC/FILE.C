#include "FILE.H"

#include "CD.H"
#include "SPECIFIC.H"

#ifdef PSX
	#include <sys/types.h>
	#include <libcd.h>
#endif

#include <stdio.h>

int FILE_Load(char* szFileName, void* pDest)//5E528, 5E5D8
{
	FILE* fileHandle = NULL;
	long dwFileSize = 0;
	unsigned long dwBytesRead = 0;

	printf("Open\n");
	fileHandle = fopen(szFileName, "rb");

	if (fileHandle != NULL)
	{
		printf("Seek\n");
		fseek(fileHandle, 0, SEEK_END);
		dwFileSize = ftell(fileHandle);
		fseek(fileHandle, 0, SEEK_SET);

		printf("Read\n");
		dwBytesRead = fread(pDest, 1, dwFileSize, fileHandle);

		printf("Close\n");
		fclose(fileHandle);

		
	}
	else
	{
		printf("FILE_Load: '%s' Could not open!\n", szFileName);
		S_ExitSystem("FILE_Load: '%s' Could not open!\n");
		printf("Can't open file");//?
	}

	return dwBytesRead;
}

int FILE_Read(void* pDest, int nItemSize, int nItems, FILE* nHandle)//5E6A8, 
{
	return fread(pDest, nItemSize, nItems, nHandle);
}

unsigned long FILE_Length(char* szFileName)//5E60C, 5E578
{
	FILE* fileHandle = NULL;
	unsigned long dwFileSize = -1;

	printf("Open\n");
	fileHandle = fopen(szFileName, "rb");

	if (fileHandle != NULL)
	{
		printf(".");//?
		printf("Seek\n");
		fseek(fileHandle, 0, SEEK_END);
		dwFileSize = ftell(fileHandle);

		printf("Close\n");
		fclose(fileHandle);
	}
	else
	{
		printf("FILE_Length: '%s' Could not find!\n", szFileName);
	}

	return dwFileSize;
}

void RelocateModule(unsigned long Module, unsigned long* RelocData)
{
	//TODO
}