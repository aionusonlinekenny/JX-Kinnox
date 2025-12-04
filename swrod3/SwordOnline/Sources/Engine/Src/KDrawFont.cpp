//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KDrawFont.cpp
// Date:	2000.08.08
// Code:	Daniel Wang
// Desc:	Alpha Sprite Font Drawing Functions
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KCanvas.h"
#include "KDrawFont.h"
//---------------------------------------------------------------------------
// º¯Êý:	DrawFont
// ¹¦ÄÜ:	»æÖÆ´ø8¼¶AlphaµÄ×ÖÌå
// ²ÎÊý:	KDrawNode*, KCanvas* 
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void g_DrawFont(void* node, void* canvas)
{
	KDrawNode* pNode = (KDrawNode *)node;
	KCanvas* pCanvas = (KCanvas *)canvas;
	
	long nX = pNode->m_nX;// x coord
	long nY = pNode->m_nY;// y coord
	long nWidth = pNode->m_nWidth;// width of font
	long nHeight = pNode->m_nHeight;// height of font
	long nColor = pNode->m_nColor;// color of font
	long nAlpha = pNode->m_nAlpha&0x001f;// nAlphaÖµ
	long nTmpAlpha = 0;// nAlphaÖµ2
	void* lpFont = pNode->m_pBitmap;// font pointer

	// ¶Ô»æÖÆÇøÓò½øÐÐ²Ã¼ô
	KClipper Clipper;
	if (!pCanvas->MakeClip(nX, nY, nWidth, nHeight, &Clipper))
		return;

	int nPitch;
	void* lpBuffer = pCanvas->LockCanvas(nPitch);
	if (lpBuffer == NULL)
		return;

	long nMask32 = pCanvas->m_nMask32;// RGB mask 32bit
	
	// ¼ÆËãÆÁÄ»ÏÂÒ»ÐÐµÄÆ«ÒÆ
	long nNextLine = nPitch - Clipper.width * 2;
	
	__asm
	{
//---------------------------------------------------------------------------
// ¼ÆËã EDI Ö¸ÏòÆÁÄ»ÆðµãµÄÆ«ÒÆÁ¿ (ÒÔ×Ö½Ú¼Æ)
// edi = nPitch * Clipper.y + nX * 2 + lpBuffer
//---------------------------------------------------------------------------
		mov		eax, nPitch
		mov		ebx, Clipper.y
		mul		ebx
		mov     ebx, Clipper.x
		add		ebx, ebx
		add     eax, ebx
		mov 	edi, lpBuffer
		add		edi, eax
//---------------------------------------------------------------------------
// ³õÊ¼»¯ ESI Ö¸ÏòÍ¼¿éÊý¾ÝÆðµã (Ìø¹ý Clipper.top ÐÐÍ¼ÐÎÊý¾Ý)
//---------------------------------------------------------------------------
		mov		esi, lpFont
		mov		ecx, Clipper.top
		or		ecx, ecx
		jz		loc_DrawFont_0011
		xor		eax, eax

loc_DrawFont_0008:

		mov		edx, nWidth

loc_DrawFont_0009:

		mov     al, [esi]
		inc     esi
		and		al, 0x1f
		sub		edx, eax
		jg		loc_DrawFont_0009
		dec     ecx
		jg  	loc_DrawFont_0008
//---------------------------------------------------------------------------
// jump acorrd Clipper.left, Clipper.right
//---------------------------------------------------------------------------
loc_DrawFont_0011:

		mov		eax, Clipper.left
		or		eax, eax
		jz		loc_DrawFont_0012
		jmp		loc_DrawFont_exit

loc_DrawFont_0012:

		mov		eax, Clipper.right
		or		eax, eax
		jz		loc_DrawFont_0100
		jmp		loc_DrawFont_exit
//---------------------------------------------------------------------------
// Clipper.left  == 0
// Clipper.right == 0
//---------------------------------------------------------------------------
loc_DrawFont_0100:

		mov		edx, Clipper.width

loc_DrawFont_0101:

		xor		eax, eax
		mov     al, [esi]
		inc     esi
		mov		ebx, eax
		shr		ebx, 5
		or		ebx, ebx
		jnz		loc_DrawFont_0102

		add		edi, eax
		add		edi, eax
		sub		edx, eax
		jg		loc_DrawFont_0101
		add		edi, nNextLine
		dec		Clipper.height
		jg		loc_DrawFont_0100
		jmp		loc_DrawFont_exit

loc_DrawFont_0102:

        shl     ebx, 2    //Alpha1 
		
		push    eax
		push    edx 
		mov     eax,nAlpha
		mul     ebx
        shr     eax,5 
        mov		nTmpAlpha, eax
		pop     edx  
		pop     eax 
   
		and		eax, 0x1f
		mov     ecx, eax
		push	eax
		push    edx

loc_DrawFont_Loop1:

		push	ecx
		mov     ecx, nColor				// ecx = ...rgb
		mov		ax, cx					// eax = ...rgb
		sal		eax, 16					// eax = rgb...
		mov		ax, cx					// eax = rgbrgb
		and		eax, nMask32			// eax = .g.r.b

		mov		cx, [edi]				// ecx = ...rgb
		mov		bx, cx					// ebx = ...rgb
		sal		ebx, 16					// ebx = rgb...
		mov		bx, cx					// ebx = rgbrgb
		and		ebx, nMask32			// ebx = .g.r.b

		mov		ecx, nTmpAlpha 			// ecx = nAlpha
		mul		ecx						// eax = eax*ecx
		neg		ecx						// ecx = -nAlpha
		add		ecx, 0x20				// ecx = 32-nAlpha
		xchg	eax, ebx				// exchange eax,ebx
		mul		ecx						// eax = eax*ecx
		add		eax, ebx				// eax = eax + ebx
		sar		eax, 5					// eax = eax/32
		and     eax, nMask32			// eax = .g.r.b

		mov     cx, ax					// ecx = ...r.b
		sar     eax, 16					// eax = ....g.
		or      ax, cx					// eax = ...rgb
        stosw                       

		pop		ecx
        loop    loc_DrawFont_Loop1

        pop     edx
        pop     eax

		sub		edx, eax
		jg		loc_DrawFont_0101
		add		edi, nNextLine
		dec		Clipper.height
		jg		loc_DrawFont_0100
		jmp		loc_DrawFont_exit

loc_DrawFont_exit:
	}
	pCanvas->UnlockCanvas();
}

//---------------------------------------------------------------------------
// º¯Êý:	DrawFont
// ¹¦ÄÜ:	»æÖÆ´ø8¼¶AlphaµÄ×ÖÌå
// ²ÎÊý:	KDrawNode*, KCanvas* 
// ·µ»Ø:	void
//---------------------------------------------------------------------------
/*void g_DrawFontWithBorder(void* node, void* canvas)
{
	KDrawNode* pNode = (KDrawNode *)node;
	KCanvas* pCanvas = (KCanvas *)canvas;

	int nX = pNode->m_nX;// x coord
	int nY = pNode->m_nY;// y coord
	int nWidth = pNode->m_nWidth;// width of font
	int nHeight = pNode->m_nHeight;// height of font
	int nColor = pNode->m_nColor;// color of font
	int nBorderColor = pNode->m_nAlpha;// ±ßÔµµÄÑÕÉ«Öµ
	void* lpFont = pNode->m_pBitmap;// font pointer

	// ¶Ô»æÖÆÇøÓò½øÐÐ²Ã¼ô
	KClipper Clipper;
	if (!pCanvas->MakeClip(nX, nY, nWidth, nHeight, &Clipper))
	return;

	int nPitch;
	void* lpBuffer = pCanvas->LockCanvas(nPitch);
	if (lpBuffer == NULL)
		return;
	UINT nCheckColor = nColor & 0xf800;
	if(nCheckColor == 0xf800)
		nColor &= 0xf7ff;
	nCheckColor = nBorderColor & 0xf800;
	if(nCheckColor == 0xf800)
		nBorderColor &= 0xf7ff;
	//offset ®Ó ®i ®Õn ®iÓm xuèng dßng
	int nNextLine = nPitch - Clipper.width * 2;
	//®i ®Õn ®iÓm b¾t ®Çu vÏ trªn khung tranh
	BYTE* pCVBuff = (BYTE*)lpBuffer + nPitch * Clipper.y + Clipper.x*2;	
	//d÷ liÖu cña ký tù
	BYTE* pFontData = (BYTE*)lpFont;
	int nTmpLength;
	BYTE uData, uTrans, uDLen;
	if(!Clipper.top)
	goto	loc_DrawFont_0011;

loc_DrawFont_0008:
		nTmpLength = nWidth;

loc_DrawFont_0009:
		uData = *pFontData++;
		uData &= 0x1f;
		nTmpLength -= uData;
		if(nTmpLength > 0)
		goto	loc_DrawFont_0009;
		Clipper.top--;
		if(Clipper.top > 0)
		goto  	loc_DrawFont_0008;
//---------------------------------------------------------------------------
// jump acorrd Clipper.left, Clipper.right
//---------------------------------------------------------------------------
loc_DrawFont_0011:
    if (!Clipper.left)
    goto loc_DrawFont_0012;
	goto	loc_DrawFont_exit;	

loc_DrawFont_0012:
	if (!Clipper.right)
    goto loc_DrawFont_0100;
	goto	loc_DrawFont_exit;
loc_DrawFont_0100:

		nTmpLength = Clipper.width;

loc_DrawFont_0101:

		uData = *pFontData++;
		uTrans = uData >> 5;
		if(uTrans)
		goto	loc_DrawFont_0102;
		pCVBuff += uData*2;
		
		nTmpLength -= uData;
		if(nTmpLength > 0)
		goto	loc_DrawFont_0101;
		pCVBuff += nNextLine;
		Clipper.height--;
		if(Clipper.height > 0)
		goto	loc_DrawFont_0100;
		goto	loc_DrawFont_exit;

loc_DrawFont_0102:
		uData &= 0x1f;
		if(uTrans < 7)
		goto	DrawFrontWithBorder_DrawBorder;
		//vÏ mµu chÝnh cña ký tù
		uDLen = uData;
		while(uDLen)
		{
			*((WORD*)pCVBuff) = nColor;
			pCVBuff += 2;
			--uDLen;
		}

		nTmpLength -= uData;
		if(nTmpLength > 0)
		goto	loc_DrawFont_0101;
		pCVBuff += nNextLine;
		Clipper.height--;
		if(Clipper.height > 0)
		goto	loc_DrawFont_0100;
		goto	loc_DrawFont_exit;
 
DrawFrontWithBorder_DrawBorder:		//»æÖÆ×Ö·ûµÄ±ßÔµ
		uDLen = uData;
		while(uDLen)
		{
			*((WORD*)pCVBuff) = nBorderColor;
			pCVBuff += 2;
			--uDLen;
		}

		nTmpLength -= uData;
		if(nTmpLength > 0)
		goto	loc_DrawFont_0101;
		pCVBuff += nNextLine;
		Clipper.height--;
		if(Clipper.height > 0)
		goto	loc_DrawFont_0100;
		goto	loc_DrawFont_exit;

loc_DrawFont_exit:
	pCanvas->UnlockCanvas();
}
*/
// Ham ve glyph font co vien (goc goto), giu dung con tro RLE va luon ve phan nam trong canvas
void g_DrawFontWithBorder(void* node, void* canvas)
{
    KDrawNode* pNode   = (KDrawNode *)node;
    KCanvas*   pCanvas = (KCanvas *)canvas;

    // Toa do va kich thuoc logic cua glyph
    int   nX = pNode->m_nX;
    int   nY = pNode->m_nY;
    int   nWidth  = pNode->m_nWidth;     // chieu rong logic cua 1 dong glyph (tinh theo pixel)
    int   nHeight = pNode->m_nHeight;

    // Mau 16-bit RGB565: nColor = mau chinh, nBorderColor = mau vien
    int   nColor        = pNode->m_nColor;
    int   nBorderColor  = pNode->m_nAlpha;

    // Con tro du lieu font (RLE) cua glyph
    BYTE* pFontData     = (BYTE*)pNode->m_pBitmap;

    // Tinh clip: neu glyph con cham canvas thi tra ve TRUE va set Clipper.*
    KClipper Clipper;
    if (!pCanvas->MakeClip(nX, nY, nWidth, nHeight, &Clipper))
        return; // hoan toan ngoai canvas

    // Lock canvas de ghi pixel
    int   nPitch;
    void* lpBuffer = pCanvas->LockCanvas(nPitch);
    if (!lpBuffer) return;

    // Chinh mau neu can (tranh tran bit)
    if ((nColor & 0xF800) == 0xF800)          nColor       &= 0xF7FF;
    if ((nBorderColor & 0xF800) == 0xF800)    nBorderColor &= 0xF7FF;

    // Diem bat dau ve phan visible: (Clipper.x, Clipper.y)
    BYTE* pCVBuff   = (BYTE*)lpBuffer + nPitch * Clipper.y + Clipper.x * 2;

    // Buoc nhay xuong dong canvas cho vung visible (chi tinh tren Clipper.width)
    int   nNextLine = nPitch - Clipper.width * 2;

    // ---- Bien lam viec (khai bao truoc nhan de hop VC6) ----
    int  nTmpLength;       // dem tam
    int  leftRemain;       // so pixel can "an" ben trai (khong ghi)
    int  visibleRemain;    // so pixel can ve tren dong (Clipper.width)
    int  rightRemain;      // so pixel can "an" ben phai (khong ghi)
    int  take;             // so pixel lay tu 1 run
    int  carryLen;         // phan con lai cua run sau khi an trai
    int  hasCarry;         // 0/1: co carry sau khi an trai
    BYTE carryTrans;       // loai run cua carry (0: trong suot, <7: vien, 7: mau chinh)
    BYTE uByte, uTrans, uLen;

    // =======================================================
    // 0) BO QUA TOP: consume du lieu RLE cua Clipper.top dong (khong ghi ra canvas)
    // =======================================================
    if (!Clipper.top) goto loc_AfterTop;

loc_SkipTopRow:
    nTmpLength = nWidth;              // moi dong top phai an du nWidth pixel logic
loc_SkipTopRun:
    uByte = *pFontData++;
    uLen  = (BYTE)(uByte & 0x1F);
    nTmpLength -= (int)uLen;
    if (nTmpLength > 0) goto loc_SkipTopRun;

    Clipper.top--;
    if (Clipper.top > 0) goto loc_SkipTopRow;

    // =======================================================
    // 1) MOI DONG VISIBLE: LEFT -> VISIBLE -> RIGHT
    // =======================================================
loc_AfterTop:
    if (Clipper.height <= 0) goto loc_Exit; // het dong visible

    // -------- LEFT: an ben trai (khong ghi) --------
    leftRemain = Clipper.left;
    hasCarry   = 0;

    if (leftRemain <= 0) goto loc_VisibleInit;

loc_LeftFetch:
    uByte  = *pFontData++;
    uTrans = (BYTE)(uByte >> 5);
    uLen   = (BYTE)(uByte & 0x1F);

    if (leftRemain >= (int)uLen)
    {
        leftRemain -= (int)uLen;          // an tron run
        if (leftRemain > 0) goto loc_LeftFetch;
        goto loc_VisibleInit;             // het LEFT
    }
    else
    {
        // an 1 phan run de het LEFT, phan du con lai "carry" sang VISIBLE
        carryLen   = (int)uLen - leftRemain;
        carryTrans = uTrans;
        hasCarry   = 1;
        leftRemain = 0;
        goto loc_VisibleInit;
    }

    // -------- VISIBLE: ve dung Clipper.width pixel --------
loc_VisibleInit:
    visibleRemain = Clipper.width;

    // Ve phan carry truoc (neu co)
    if (hasCarry)
    {
        take = (carryLen < visibleRemain) ? carryLen : visibleRemain;

        if (carryTrans == 0)
        {
            // trong suot: chi nhay con tro
            pCVBuff += take * 2;
        }
        else if (carryTrans < 7)
        {
            // vien
            nTmpLength = take;
        loc_CarryBorderLoop:
            if (nTmpLength <= 0) goto loc_CarryBorderDone;
            *((WORD*)pCVBuff) = (WORD)nBorderColor; pCVBuff += 2;
            nTmpLength--;
            goto loc_CarryBorderLoop;
        loc_CarryBorderDone:;
        }
        else
        {
            // mau chinh
            nTmpLength = take;
        loc_CarryMainLoop:
            if (nTmpLength <= 0) goto loc_CarryMainDone;
            *((WORD*)pCVBuff) = (WORD)nColor; pCVBuff += 2;
            nTmpLength--;
            goto loc_CarryMainLoop;
        loc_CarryMainDone:;
        }

        visibleRemain -= take;
        carryLen      -= take;

        if (carryLen > 0)
        {
            // run van con du nhung visible da het -> dua phan du sang RIGHT
            rightRemain = nWidth - (Clipper.left + Clipper.width);

            // an phan du cua carry truoc (khong ghi)
            take = (carryLen < rightRemain) ? carryLen : rightRemain;
            rightRemain -= take;

            // neu con RIGHT thi an tiep
            if (rightRemain > 0) goto loc_RightConsume;

            // xuong dong
            pCVBuff += nNextLine;
            Clipper.height--;
            if (Clipper.height > 0) goto loc_AfterTop;
            goto loc_Exit;
        }

        hasCarry = 0;
        if (visibleRemain <= 0) goto loc_RightInit;
    }

    // Ve phan visible tu cac run moi
loc_VisibleLoop:
    uByte  = *pFontData++;
    uTrans = (BYTE)(uByte >> 5);
    uLen   = (BYTE)(uByte & 0x1F);

    take = ((int)uLen < visibleRemain) ? (int)uLen : visibleRemain;

    if (uTrans == 0)
    {
        // trong suot
        pCVBuff += take * 2;
    }
    else if (uTrans < 7)
    {
        // vien
        nTmpLength = take;
    loc_BorderLoop:
        if (nTmpLength <= 0) goto loc_BorderDone;
        *((WORD*)pCVBuff) = (WORD)nBorderColor; pCVBuff += 2;
        nTmpLength--;
        goto loc_BorderLoop;
    loc_BorderDone:;
    }
    else
    {
        // mau chinh
        nTmpLength = take;
    loc_MainLoop:
        if (nTmpLength <= 0) goto loc_MainDone;
        *((WORD*)pCVBuff) = (WORD)nColor; pCVBuff += 2;
        nTmpLength--;
        goto loc_MainLoop;
    loc_MainDone:;
    }

    visibleRemain -= take;

    // neu run con du va visible da het -> dua phan du sang RIGHT
    if ((int)uLen > take)
    {
        rightRemain = nWidth - (Clipper.left + Clipper.width);

        // an phan du con lai cua run hien tai
        nTmpLength = (int)uLen - take;
        take = (nTmpLength < rightRemain) ? nTmpLength : rightRemain;
        rightRemain -= take;

        if (rightRemain > 0) goto loc_RightConsume;

        // xuong dong
        pCVBuff += nNextLine;
        Clipper.height--;
        if (Clipper.height > 0) goto loc_AfterTop;
        goto loc_Exit;
    }

    if (visibleRemain > 0) goto loc_VisibleLoop;

    // -------- RIGHT: an phan ben phai con lai (khong ghi) --------
loc_RightInit:
    rightRemain = nWidth - (Clipper.left + Clipper.width);
    if (rightRemain > 0) goto loc_RightConsume;
    goto loc_NextLine;

loc_RightConsume:
    if (rightRemain <= 0) goto loc_NextLine;
    uByte = *pFontData++;
    uLen  = (BYTE)(uByte & 0x1F);
    if (rightRemain >= (int)uLen)
    {
        rightRemain -= (int)uLen;
        goto loc_RightConsume;
    }
    // an not phan thua
    rightRemain = 0;

    // -------- Xuong dong tiep theo: dua pCVBuff ve dau dong visible moi --------
loc_NextLine:
    pCVBuff += nNextLine;
    Clipper.height--;
    if (Clipper.height > 0) goto loc_AfterTop;

loc_Exit:
    pCanvas->UnlockCanvas();
}
/*
void g_DrawFontWithBorder(void* node, void* canvas)
{
    KDrawNode* pNode = (KDrawNode *)node;
    KCanvas* pCanvas = (KCanvas *)canvas;

    int nX = pNode->m_nX;          // x coord
    int nY = pNode->m_nY;          // y coord
    int nWidth = pNode->m_nWidth;  // width of font
    int nHeight = pNode->m_nHeight;// height of font
    int nColor = pNode->m_nColor;  // color of font
    int nBorderColor = pNode->m_nAlpha; // border color
    void* lpFont = pNode->m_pBitmap;    // font pointer

    KClipper Clipper;
    if (!pCanvas->MakeClip(nX, nY, nWidth, nHeight, &Clipper))
        return;

    int nPitch;
    void* lpBuffer = pCanvas->LockCanvas(nPitch);
    if (lpBuffer == NULL)
        return;

    UINT nCheckColor = nColor & 0xf800;
    if (nCheckColor == 0xf800) nColor &= 0xf7ff;
    nCheckColor = nBorderColor & 0xf800;
    if (nCheckColor == 0xf800) nBorderColor &= 0xf7ff;

    int nNextLine = nPitch - Clipper.width * 2;
    BYTE* pCVBuff = (BYTE*)lpBuffer + nPitch * Clipper.y + Clipper.x * 2;
    BYTE* pFontData = (BYTE*)lpFont;

    int nTmpLength;
    BYTE uData, uTrans, uDLen;

    // B? qua Clipper.top
    while (Clipper.top > 0)
    {
        nTmpLength = nWidth;
        while (nTmpLength > 0)
        {
            uData = *pFontData++;
            uData &= 0x1f;
            nTmpLength -= uData;
        }
        Clipper.top--;
    }

    // -----------------------------
    // X? lý ph?n chính
    // -----------------------------
    while (Clipper.height > 0)
    {
        nTmpLength = Clipper.width;
        while (nTmpLength > 0)
        {
            uData = *pFontData++;
            uTrans = uData >> 5;
            uData &= 0x1f;

            if (uTrans == 0)  // pixel r?ng
            {
                pCVBuff += uData * 2;
                nTmpLength -= uData;
            }
            else if (uTrans < 7) // v? vi?n
            {
                uDLen = uData;
                while (uDLen && nTmpLength > 0)
                {
                    *((WORD*)pCVBuff) = nBorderColor;
                    pCVBuff += 2;
                    --uDLen;
                    --nTmpLength;
                }
            }
            else // v? màu chính
            {
                uDLen = uData;
                while (uDLen && nTmpLength > 0)
                {
                    *((WORD*)pCVBuff) = nColor;
                    pCVBuff += 2;
                    --uDLen;
                    --nTmpLength;
                }
            }
        }

        pCVBuff += nNextLine;
        Clipper.height--;
    }

    pCanvas->UnlockCanvas();
}
*/
//---------------------------------------------------------------------------
// º¯Êý:	DrawFontSolid
// ¹¦ÄÜ:	»æÖÆÊµÐÄ×ÖÌå
// ²ÎÊý:	KDrawNode*, KCanvas* 
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void g_DrawFontSolid(void* node, void* canvas)
{
	KDrawNode* pNode = (KDrawNode *)node;
	KCanvas* pCanvas = (KCanvas *)canvas;
	
	long nX = pNode->m_nX;// x coord
	long nY = pNode->m_nY;// y coord
	long nWidth = pNode->m_nWidth;// width of font
	long nHeight = pNode->m_nHeight;// height of font
	long nColor = pNode->m_nColor;// color of font
	long nAlpha = pNode->m_nAlpha&0x001f;// nAlphaÖµ
	void* lpFont = pNode->m_pBitmap;// font pointer

	// ¶Ô»æÖÆÇøÓò½øÐÐ²Ã¼ô
	KClipper Clipper;
	if (!pCanvas->MakeClip(nX, nY, nWidth, nHeight, &Clipper))
		return;

	int nPitch;
	void* lpBuffer = pCanvas->LockCanvas(nPitch);
	if (lpBuffer == NULL)
		return;

	long nMask32 = pCanvas->m_nMask32;// RGB mask 32bit

	// ¼ÆËãÆÁÄ»ÏÂÒ»ÐÐµÄÆ«ÒÆ
	long nNextLine = nPitch - Clipper.width * 2;
	
	__asm
	{
//---------------------------------------------------------------------------
// ¼ÆËã EDI Ö¸ÏòÆÁÄ»ÆðµãµÄÆ«ÒÆÁ¿ (ÒÔ×Ö½Ú¼Æ)
// edi = nPitch * Clipper.y + nX * 2 + lpBuffer
//---------------------------------------------------------------------------
		mov		eax, nPitch
		mov		ebx, Clipper.y
		mul		ebx
		mov     ebx, Clipper.x
		add		ebx, ebx
		add     eax, ebx
		mov 	edi, lpBuffer
		add		edi, eax
//---------------------------------------------------------------------------
// ³õÊ¼»¯ ESI Ö¸ÏòÍ¼¿éÊý¾ÝÆðµã (Ìø¹ý Clipper.top ÐÐÍ¼ÐÎÊý¾Ý)
//---------------------------------------------------------------------------
		mov		esi, lpFont
		mov		ecx, Clipper.top
		or		ecx, ecx
		jz		loc_DrawFontSolid_0011
		xor		eax, eax

loc_DrawFontSolid_0008:

		mov		edx, nWidth

loc_DrawFontSolid_0009:

		mov     al, [esi]
		inc     esi
		and		al, 0x1f
		sub		edx, eax
		jg		loc_DrawFontSolid_0009
		dec     ecx
		jg  	loc_DrawFontSolid_0008
//---------------------------------------------------------------------------
// jump acorrd Clipper.left, Clipper.right
//---------------------------------------------------------------------------
loc_DrawFontSolid_0011:

		mov		eax, Clipper.left
		or		eax, eax
		jz		loc_DrawFontSolid_0012
		jmp		loc_DrawFontSolid_exit

loc_DrawFontSolid_0012:

		mov		eax, Clipper.right
		or		eax, eax
		jz		loc_DrawFontSolid_0100
		jmp		loc_DrawFontSolid_exit
//---------------------------------------------------------------------------
// Clipper.left  == 0
// Clipper.right == 0
//---------------------------------------------------------------------------
loc_DrawFontSolid_0100:

		mov		edx, Clipper.width

loc_DrawFontSolid_0101:

		xor		eax, eax
		mov     al, [esi]
		inc     esi
		mov		ebx, eax
		shr		ebx, 5
		or		ebx, ebx
		jnz		loc_DrawFontSolid_0102

		add		edi, eax
		add		edi, eax
		sub		edx, eax
		jg		loc_DrawFontSolid_0101
		add		edi, nNextLine
		dec		Clipper.height
		jg		loc_DrawFontSolid_0100
		jmp		loc_DrawFontSolid_exit

loc_DrawFontSolid_0102:

        and		eax, 0x1f
		mov     ecx, eax
		push	eax
		push    edx

loc_DrawFontSolid_Loop1:

		push	ecx
		mov     ecx, nColor				// ecx = ...rgb
		mov		ax, cx					// eax = ...rgb
		sal		eax, 16					// eax = rgb...
		mov		ax, cx					// eax = rgbrgb
		and		eax, nMask32			// eax = .g.r.b

		mov		cx, [edi]				// ecx = ...rgb
		mov		bx, cx					// ebx = ...rgb
		sal		ebx, 16					// ebx = rgb...
		mov		bx, cx					// ebx = rgbrgb
		and		ebx, nMask32			// ebx = .g.r.b

		mov		ecx, nAlpha 			// ecx = nAlpha
		mul		ecx						// eax = eax*ecx
		neg		ecx						// ecx = -nAlpha
		add		ecx, 0x20				// ecx = 32-nAlpha
		xchg	eax, ebx				// exchange eax,ebx
		mul		ecx						// eax = eax*ecx
		add		eax, ebx				// eax = eax + ebx
		sar		eax, 5					// eax = eax/32
		and     eax, nMask32			// eax = .g.r.b

		mov     cx, ax					// ecx = ...r.b
		sar     eax, 16					// eax = ....g.
		or      ax, cx					// eax = ...rgb
        stosw                       

		pop		ecx
        loop    loc_DrawFontSolid_Loop1

        pop     edx
        pop     eax

		sub		edx, eax
		jg		loc_DrawFontSolid_0101
		add		edi, nNextLine
		dec		Clipper.height
		jg		loc_DrawFontSolid_0100
		jmp		loc_DrawFontSolid_exit

loc_DrawFontSolid_exit:
	}
	pCanvas->UnlockCanvas();
}
//---------------------------------------------------------------------------
