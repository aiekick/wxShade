// AVIGenerator.cpp: implementation of the CAVIGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "AVIGenerator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CAVIGenerator::CAVIGenerator()
: m_sFile(_T("Untitled.avi")), m_dwRate(30),
m_pAVIFile(NULL), m_pStream(NULL), m_pStreamCompressed(NULL)
{
	memset(&m_bih,0,sizeof(BITMAPINFOHEADER));
}

#ifdef _AVIGENERATOR_USE_MFC
CAVIGenerator::CAVIGenerator(LPCTSTR sFileName, CView* pView, DWORD dwRate)
: m_sFile(sFileName), m_dwRate(dwRate),
m_pAVIFile(NULL), m_pStream(NULL), m_pStreamCompressed(NULL)
{
		MakeExtAvi();
		SetBitmapHeader(pView);
}
#endif

CAVIGenerator::CAVIGenerator(LPCTSTR sFileName, BITMAPINFOHEADER lpbih, DWORD dwRate)
: m_sFile(sFileName), m_dwRate(dwRate),
m_pAVIFile(NULL), m_pStream(NULL), m_pStreamCompressed(NULL)
{
		MakeExtAvi();
		SetBitmapHeader(lpbih);
}

CAVIGenerator::~CAVIGenerator()
{
	// Just checking that all allocated ressources have been released
	cAssert(m_pStream==NULL, "m_pStream==NULL");
	cAssert(m_pStreamCompressed == NULL, "m_pStreamCompressed == NULL");
	cAssert(m_pAVIFile == NULL, "m_pAVIFile == NULL");
}

void CAVIGenerator::SetBitmapHeader(BITMAPINFOHEADER lpbih)
{
	// checking that bitmap size are multiple of 4
	cAssert(lpbih.biWidth % 4 == 0, "lpbih.biWidth % 4 == 0");
	cAssert(lpbih.biHeight % 4 == 0, "lpbih.biHeight % 4 == 0");

	// copying bitmap info structure.
	// corrected thanks to Lori Gardi
	memcpy(&m_bih,&lpbih, sizeof(BITMAPINFOHEADER));
}

#ifdef _AVIGENERATOR_USE_MFC
void CAVIGenerator::SetBitmapHeader(CView *pView)
{
	ASSERT_VALID(pView);

	////////////////////////////////////////////////
	// Getting screen dimensions
	// Get client geometry 
	CRect rect; 
	pView->GetClientRect(&rect); 
	CSize size(rect.Width(),rect.Height()); 

	/////////////////////////////////////////////////
	// changing size of image so dimension are multiple of 4
	size.cx=(size.cx/4)*4;
	size.cy=(size.cy/4)*4;

	// initialize m_bih
	memset(&m_bih,0, sizeof(BITMAPINFOHEADER));
	// filling bitmap info structure.
	m_bih.biSize=sizeof(BITMAPINFOHEADER);
	m_bih.biWidth=size.cx;
	m_bih.biHeight=size.cy;
	m_bih.biPlanes=1;
	m_bih.biBitCount=24;
	m_bih.biSizeImage=((m_bih.biWidth*m_bih.biBitCount+31)/32 * 4)*m_bih.biHeight; 
	m_bih.biCompression=BI_RGB;		//BI_RGB means BRG in reality
}
#endif

HRESULT CAVIGenerator::InitEngine()
{
	AVISTREAMINFO strHdr; // information for a single stream 
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};

	HRESULT hr;

	m_sError = "Ok\n";

	// Step 0 : Let's make sure we are running on 1.1 
	DWORD wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a)
	{
		 // oops, we are too old, blow out of here 
		m_sError= "Version of Video for Windows too old. Come on, join the 21th century!\n";
		return S_FALSE;
	}

	// Step 1 : initialize AVI engine
	AVIFileInit();

	// Step 2 : Open the movie file for writing....
	hr = AVIFileOpen(&m_pAVIFile,			// Address to contain the new file interface pointer
			(LPCWSTR)m_sFile,				// Null-terminated string containing the name of the file to open
		       OF_WRITE | OF_CREATE,	    // Access mode to use when opening the file. 
		       NULL);						// use handler determined from file extension.

	if (hr != AVIERR_OK)
	{
		m_sError = "AVI Engine failed to initialize. Check filename " + m_sFile + "\n";
		// Check it succeded.
		switch(hr)
		{
		case AVIERR_BADFORMAT: 
			m_sError += "The file couldn't be read, indicating a corrupt file or an unrecognized format.\n";
			break;
		case AVIERR_MEMORY:		
			m_sError +="The file could not be opened because of insufficient memory.\n"; 
			break;
		case AVIERR_FILEREAD:
			m_sError +="A disk error occurred while reading the file.\n"; 
			break;
		case AVIERR_FILEOPEN:		
			m_sError +="A disk error occurred while opening the file.\n";
			break;
		case REGDB_E_CLASSNOTREG:		
			m_sError+="According to the registry, the type of file specified in AVIFileOpen does not have a handler to process it\n";
			break;
		}

		return hr;
	}

	// Fill in the header for the video stream....
	memset(&strHdr, 0, sizeof(strHdr));
	strHdr.fccType                = streamtypeVIDEO;	// video stream type
	strHdr.fccHandler             = 0;
	strHdr.dwScale                = 1;					// should be one for video
	strHdr.dwRate                 = m_dwRate;		    // fps
	strHdr.dwSuggestedBufferSize  = m_bih.biSizeImage;	// Recommended buffer size, in bytes, for the stream.
	SetRect(&strHdr.rcFrame, 0, 0,		    // rectangle for stream
	    (int) m_bih.biWidth,
	    (int) m_bih.biHeight);

	// Step 3 : Create the stream;
	hr = AVIFileCreateStream(m_pAVIFile,		    // file pointer
			         &m_pStream,		    // returned stream pointer
			         &strHdr);	    // stream header

	// Check it succeded.
	if (hr != AVIERR_OK)
	{
		m_sError = "AVI Stream creation failed. Check Bitmap info.\n";
		if (hr==AVIERR_READONLY)
		{
			m_sError += " Read only file.\n";
		}
		return hr;
	}


	// Step 4: Get codec and infos about codec
	memset(&opts, 0, sizeof(opts));
	// Poping codec dialog
	if (!AVISaveOptions(NULL, 0, 1, &m_pStream, (LPAVICOMPRESSOPTIONS FAR *) &aopts))
	{
		AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
		return S_FALSE;
	}

	// Step 5:  Create a compressed stream using codec options.
	hr = AVIMakeCompressedStream(&m_pStreamCompressed, 
				m_pStream, 
				&opts, 
				NULL);

	if (hr != AVIERR_OK)
	{
		m_sError = "AVI Compressed Stream creation failed.\n";
		
		switch(hr)
		{
		case AVIERR_NOCOMPRESSOR:
			m_sError += " A suitable compressor cannot be found.\n";
				break;
		case AVIERR_MEMORY:
			m_sError += " There is not enough memory to complete the operation.\n";
				break; 
		case AVIERR_UNSUPPORTED:
			m_sError += "Compression is not supported for this type of data. This error might be returned if you try to compress data that is not audio or video.\n";
			break;
		}

		return hr;
	}

	// releasing memory allocated by AVISaveOptionFree
	hr=AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
	if (hr!=AVIERR_OK)
	{
		m_sError = "Error releasing memory\n";
		return hr;
	}

	// Step 6 : sets the format of a stream at the specified position
	hr = AVIStreamSetFormat(m_pStreamCompressed, 
					0,			// position
					&m_bih,	    // stream format
					m_bih.biSize +   // format size
					m_bih.biClrUsed * sizeof(RGBQUAD));

	if (hr != AVIERR_OK)
	{
		m_sError = "AVI Compressed Stream format setting failed.\n";
		return hr;
	}

	// Step 6 : Initialize step counter
	m_lFrame=0;

	return hr;
}

void CAVIGenerator::ReleaseEngine()
{
	if (m_pStream)
	{
		AVIStreamRelease(m_pStream);
		m_pStream=NULL;
	}

	if (m_pStreamCompressed)
	{
		AVIStreamRelease(m_pStreamCompressed);
		m_pStreamCompressed=NULL;
	}

	if (m_pAVIFile)
	{
		AVIFileRelease(m_pAVIFile);
		m_pAVIFile=NULL;
	}

	// Close engine
	AVIFileExit();
}

HRESULT CAVIGenerator::AddFrame(BYTE *bmBits)
{
	HRESULT hr;

	// compress bitmap
	hr = AVIStreamWrite(m_pStreamCompressed,	// stream pointer
		m_lFrame,						// time of this frame
		1,						// number to write
		bmBits,					// image buffer
		m_bih.biSizeImage,		// size of this frame
		AVIIF_KEYFRAME,			// flags....
		NULL,
		NULL);

	// updating frame counter
	m_lFrame++;

	return hr;
}

void CAVIGenerator::MakeExtAvi()
{
	
	// finding avi
/*	char *d = _tcsstr(m_sFile,"avi");
	if( d==NULL )
	{
		m_sFile+=".avi";
	}*/
}
#ifdef _AVIGENERATOR_USE_MFC
void CAVIGenerator::AviToCImageList(string filepathname, CImageList *imglist)
{
	AVIFileInit();

    PAVIFILE avi;
	int res=AVIFileOpen(&avi, filepathname.c_str(), OF_READ, NULL);

    if (res!=AVIERR_OK)
    {
        //an error occures
        if (avi!=NULL)
            AVIFileRelease(avi);
        
        return ;
    }

    AVIFILEINFO avi_info;
    AVIFileInfo(avi, &avi_info, sizeof(AVIFILEINFO));

    CString szFileInfo;
    szFileInfo.Format("Dimention: %dx%d\n"
                      "Length: %d frames\n"
                      "Max bytes per second: %d\n"
                      "Samples per second: %d\n"
                      "Streams: %d\n"
                      "File Type: %d", avi_info.dwWidth,
                            avi_info.dwHeight,
                            avi_info.dwLength,
                            avi_info.dwMaxBytesPerSec,
                            (DWORD) (avi_info.dwRate / avi_info.dwScale),
                            avi_info.dwStreams,
                            avi_info.szFileType);

    AfxMessageBox(szFileInfo, MB_ICONINFORMATION | MB_OK);

    PAVISTREAM pStream;
    res=AVIFileGetStream(avi, &pStream, streamtypeVIDEO /*video stream*/, 
                                               0 /*first stream*/);

    if (res!=AVIERR_OK)
    {
        if (pStream!=NULL)
            AVIStreamRelease(pStream);

        AVIFileExit();
        return ;
    }

    //do some task with the stream
    int iNumFrames;
    int iFirstFrame;

    iFirstFrame=AVIStreamStart(pStream);
    if (iFirstFrame==-1)
    {
        //Error getteing the frame inside the stream

        if (pStream!=NULL)
            AVIStreamRelease(pStream);

        AVIFileExit();
        return ;
    }

    iNumFrames=AVIStreamLength(pStream);
    if (iNumFrames==-1)
    {
        //Error getteing the number of frames inside the stream
        
        if (pStream!=NULL)
            AVIStreamRelease(pStream);
        
        AVIFileExit();
        return ;
    }

    //getting bitmap from frame
    BITMAPINFOHEADER bih;
    ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));

    bih.biBitCount=24;    //24 bit per pixel
    bih.biClrImportant=0;
    bih.biClrUsed = 0;
    bih.biCompression = BI_RGB;
    bih.biPlanes = 1;
    bih.biSize = 40;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    //calculate total size of RGBQUAD scanlines (DWORD aligned)
    bih.biSizeImage = (((bih.biWidth * 3) + 3) & 0xFFFC) * bih.biHeight ;

    PGETFRAME pFrame;
    pFrame=AVIStreamGetFrameOpen(pStream, (BITMAPINFOHEADER*) AVIGETFRAMEF_BESTDISPLAYFMT/*&bih*/);
    int count = 0;
    //Get the first frame
    for (int i=0; i<iNumFrames; i++)
    {
        BYTE* pDIB = (BYTE*) AVIStreamGetFrame(pFrame, i);
        
/*		int res;
		CBitmap bmp;
		res = bmp.CreateCompatibleBitmap(reateBitmap(bih.biWidth,bih.biHeight,bih.biPlanes,bih.biBitCount,(BYTE*)pDIB);
		if ( res == 0 )
			AfxMessageBox("Impossible de creer le bitmap", MB_ICONINFORMATION | MB_OK);

		res = imglist->Add(&bmp,RGB(0,0,0));
		if ( res == -1 )
			AfxMessageBox("Impossible d'ajouter une image a la liste", MB_ICONINFORMATION | MB_OK);
	
		count = imglist->GetImageCount();
 */
//       CreateFromPackedDIBPointer(pDIB, index, imglist);
    }

    AVIStreamGetFrameClose(pFrame);

    //close the stream after finishing the task
    if (pStream!=NULL)
        AVIStreamRelease(pStream);

    AVIFileExit();

    return ;

}
BOOL CAVIGenerator::CreateFromPackedDIBPointer(LPBYTE pDIB, int iFrame, CImageList *imglist)
{
    ASSERT(pDIB!=NULL);

    //Creates a full-color (no palette) DIB from a pointer to a
    //full-color memory DIB

    //get the BitmapInfoHeader
    BITMAPINFOHEADER bih;
    RtlMoveMemory(&bih.biSize, pDIB, sizeof(BITMAPINFOHEADER));

    //now get the bitmap bits
    if (bih.biSizeImage < 1)
    {
        return FALSE;
    }

    BYTE* Bits=new BYTE[bih.biSizeImage];

    RtlMoveMemory(Bits, pDIB + sizeof(BITMAPINFOHEADER), bih.biSizeImage);

    //and BitmapInfo variable-length UDT
    BYTE memBitmapInfo[40];
    RtlMoveMemory(memBitmapInfo, &bih, sizeof(bih));

    BITMAPFILEHEADER bfh;
    bfh.bfType=19778;    //BM header
    bfh.bfSize=55 + bih.biSizeImage;
    bfh.bfReserved1=0;
    bfh.bfReserved2=0;
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER); //54
    
    CString FileName;
    FileName.Format("Frame-%05d.bmp", iFrame);
    
	int res;
	CBitmap *bmp = new CBitmap();
	res = bmp->CreateBitmap(bih.biWidth,bih.biHeight,bih.biPlanes,bih.biBitCount,NULL);
	if ( res == 0 )
		AfxMessageBox("Impossible de creer le bitmap", MB_ICONINFORMATION | MB_OK);

	bmp->SetBitmapBits(bih.biSizeImage,(BYTE*)Bits);
	if ( res == 0 )
		AfxMessageBox("Impossible de remplir le bitmap", MB_ICONINFORMATION | MB_OK);

	res = imglist->Add(bmp,RGB(0,0,0));
	if ( res == -1 )
		AfxMessageBox("Impossible d'ajouter une image a la liste", MB_ICONINFORMATION | MB_OK);

    delete [] Bits;
    return TRUE;
}
#endif