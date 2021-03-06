#include "FILE.H"

#include "CD.H"
#include "SPECIFIC.H"

#include <sys/types.h>
#include <libcd.h>
#include <stdio.h>

#if INTERNAL
	#include <libsn.h>
#endif

int FILE_Load(char* szFileName, void* pDest)//5E528, 5E5D8(<) (F)
{
	CdlFILE fp;
	char buf[10];
	unsigned long dwFileSize = -1;
	
	DEL_ChangeCDMode(0);

	sprintf(buf, "\\%s;1", szFileName);

	CdSearchFile(&fp, buf);

	cdCurrentSector = CdPosToInt(&fp.pos);
	CD_Read(pDest, fp.size);

	return fp.size;
}

#if INTERNAL
int FILE_Read(char* pDest, int nItemSize, int nItems, int nHandle)//5E6A8(<), ?
{
	int nAmount = nItems * nItemSize;
	return PCread(nHandle, pDest, nAmount);
}
#endif

unsigned long FILE_Length(char* szFileName)//5E60C, 5E578(<) (F)
{
	CdlFILE fp;
	char buf[10];
	unsigned long dwFileSize = -1;

	DEL_ChangeCDMode(0);
	sprintf(buf, "\\%s;1", szFileName);
	
	if (CdSearchFile(&fp, buf))
	{
		dwFileSize = fp.size;
	}

	return dwFileSize;
}

void RelocateModule(unsigned long Module, unsigned long* RelocData)
{
	return;
}