/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "bagel/baglib/dialogs.h"
#include "bagel/baglib/cursor.h"
#include "bagel/baglib/opt_window.h"
#include "bagel/baglib/buttons.h"
#include "bagel/baglib/bagel.h"

namespace Bagel {

#define OK_BTN 5

struct ST_BUTTONS {
	const CHAR *m_pszName;
	const CHAR *m_pszUp;
	const CHAR *m_pszDown;
	const CHAR *m_pszFocus;
	const CHAR *m_pszDisabled;
	INT m_nLeft;
	INT m_nTop;
	INT m_nWidth;
	INT m_nHeight;
	INT m_nID;
};

static ST_BUTTONS g_stQuitButtons[NUM_QUIT_BUTTONS] = {
	{ "Save", "SAVEQTUP.BMP", "SAVEQTDN.BMP", "SAVEQTUP.BMP", "SAVEQTUP.BMP", 220, 190, 200, 38, SAVE_BTN },
	{ "Quit", "JUSTQTUP.BMP", "JUSTQTDN.BMP", "JUSTQTUP.BMP", "JUSTQTUP.BMP", 220, 237, 200, 38, QUIT_BTN },
	{ "Cancel", "PLAYUP.BMP", "PLAYDN.BMP", "PLAYUP.BMP", "PLAYUP.BMP", 220, 284, 200, 38, CANCEL_BTN }
};

static BOOL g_b1 = FALSE;

CBagQuitDialog::CBagQuitDialog() {
	// Inits
	m_nReturnValue = -1;
	for (INT i = 0; i < NUM_QUIT_BUTTONS; i++) {
		m_pButtons[i] = nullptr;
	}
}


VOID CBagQuitDialog::OnInitDialog(VOID) {
	Assert(IsValidObject(this));

	CBofDialog::OnInitDialog();

	SetReturnValue(-1);

	CBofPalette *pPal;
	INT i;

	Assert(m_pBackdrop != nullptr);
	pPal = m_pBackdrop->GetPalette();
	SelectPalette(pPal);

	// Paint the SaveList Box onto the background
	if (m_pBackdrop != nullptr) {
		CBofBitmap cBmp(BuildSysDir("QUITDBOX.BMP"), pPal);
		cBmp.Paint(m_pBackdrop, 205, 150);
	}

	// Build all our buttons
	for (i = 0; i < NUM_QUIT_BUTTONS; i++) {
		Assert(m_pButtons[i] == nullptr);

		if ((m_pButtons[i] = new CBofBmpButton) != nullptr) {
			CBofBitmap *pUp, *pDown, *pFocus, *pDis;

			pUp = LoadBitmap(BuildSysDir(g_stQuitButtons[i].m_pszUp), pPal);
			pDown = LoadBitmap(BuildSysDir(g_stQuitButtons[i].m_pszDown), pPal);
			pFocus = LoadBitmap(BuildSysDir(g_stQuitButtons[i].m_pszFocus), pPal);
			pDis = LoadBitmap(BuildSysDir(g_stQuitButtons[i].m_pszDisabled), pPal);

			m_pButtons[i]->LoadBitmaps(pUp, pDown, pFocus, pDis);

			m_pButtons[i]->Create(g_stQuitButtons[i].m_pszName, g_stQuitButtons[i].m_nLeft, g_stQuitButtons[i].m_nTop, g_stQuitButtons[i].m_nWidth, g_stQuitButtons[i].m_nHeight, this, g_stQuitButtons[i].m_nID);
			m_pButtons[i]->Show();
		} else {
			ReportError(ERR_MEMORY);
			break;
		}
	}

	// Show System cursor
	CBofCursor::Show();
}


VOID CBagQuitDialog::OnClose() {
	Assert(IsValidObject(this));

	CBofCursor::Hide();

	// Destroy all buttons
	for (INT i = 0; i < NUM_QUIT_BUTTONS; i++) {
		if (m_pButtons[i] != nullptr) {
			delete m_pButtons[i];
			m_pButtons[i] = nullptr;
		}
	}

	if (m_nReturnValue == QUIT_BTN || m_nReturnValue == SAVE_BTN)
		KillBackground();

	CBofDialog::OnClose();
}


VOID CBagQuitDialog::OnPaint(CBofRect *pRect) {
	Assert(IsValidObject(this));

	PaintBackdrop(pRect);

	ValidateAnscestors();
}


VOID CBagQuitDialog::OnKeyHit(ULONG lKey, ULONG nRepCount) {
	Assert(IsValidObject(this));

	switch (lKey) {

	// Cancel
	//
	case BKEY_ESC:
		SetReturnValue(CANCEL_BTN);
		Close();
		break;

	default:
		CBofDialog::OnKeyHit(lKey, nRepCount);
		break;
	}
}


VOID CBagQuitDialog::OnBofButton(CBofObject *pObject, INT nFlags) {
	Assert(IsValidObject(this));
	Assert(pObject != nullptr);

	if (nFlags == BUTTON_CLICKED) {
		CBofBmpButton *pButton;
		INT nId;
		BOOL bQuit;

		if ((pButton = (CBofBmpButton *)pObject) != nullptr) {
			nId = pButton->GetControlID();

			bQuit = TRUE;
			if (nId == SAVE_BTN) {
				CBagel *pApp;
				CBagMasterWin *pWin;

				if ((pApp = CBagel::GetBagApp()) != nullptr) {
					if ((pWin = pApp->GetMasterWnd()) != nullptr) {

						bQuit = pWin->ShowSaveDialog(this, FALSE);
					}
				}
			}

			if (bQuit) {
				SetReturnValue(nId);
				Close();
			}
		}
	}
}

////////////////////////////////////////

CBagNextCDDialog::CBagNextCDDialog() {
	// Inits
	m_nReturnValue = -1;
	m_pButton = nullptr;
	m_lFlags = 0;
}


VOID CBagNextCDDialog::OnInitDialog(VOID) {
	Assert(IsValidObject(this));

	CBofDialog::OnInitDialog();

	SetReturnValue(-1);

	CBofPalette *pPal;

	Assert(m_pBackdrop != nullptr);
	pPal = m_pBackdrop->GetPalette();
	SelectPalette(pPal);

	// Build all our buttons
	if ((m_pButton = new CBofBmpButton) != nullptr) {
		CBofBitmap *pUp, *pDown, *pFocus, *pDis;

		pUp = LoadBitmap(BuildSysDir("CDOKUP.BMP"), pPal);
		pDown = LoadBitmap(BuildSysDir("CDOKDN.BMP"), pPal);
		pFocus = LoadBitmap(BuildSysDir("CDOKUP.BMP"), pPal);
		pDis = LoadBitmap(BuildSysDir("CDOKUP.BMP"), pPal);

		m_pButton->LoadBitmaps(pUp, pDown, pFocus, pDis);

		m_pButton->Create("NextCD", 77, 127, 60, 30, this, OK_BTN);
		m_pButton->Show();

	} else {
		ReportError(ERR_MEMORY);
	}

	// Show System cursor
	CBofCursor::Show();
	CBofCursor::Show();
}


VOID CBagNextCDDialog::OnClose() {
	Assert(IsValidObject(this));

	CBofCursor::Hide();
	CBofCursor::Hide();

	// Destroy my buttons
	if (m_pButton != nullptr) {
		delete m_pButton;
		m_pButton = nullptr;
	}

	CBofDialog::OnClose();
}


VOID CBagNextCDDialog::OnPaint(CBofRect *pRect) {
	Assert(IsValidObject(this));
	Assert(pRect != nullptr);

	m_bFirstTime = FALSE;

	// Paint the dialog
	if (m_pBackdrop != nullptr) {
		m_pBackdrop->Paint(this, pRect, pRect, 1);
	}

	// Paint to stop it showing up behind backdrop
#if BOF_MAC
	m_pButton->Paint();
#endif

	m_bHavePainted = TRUE;

	ValidateAnscestors();
}


VOID CBagNextCDDialog::OnKeyHit(ULONG lKey, ULONG nRepCount) {
	Assert(IsValidObject(this));

	switch (lKey) {

	// Cancel
	case BKEY_ENTER:
	case BKEY_ESC:
		Close();
		break;

	default:
		CBofDialog::OnKeyHit(lKey, nRepCount);
		break;
	}
}


VOID CBagNextCDDialog::OnBofButton(CBofObject * /*pObject*/, INT nFlags) {
	Assert(IsValidObject(this));

	if (nFlags == BUTTON_CLICKED) {
		Close();
	}
}

///////////////////////

struct ST_CREDITS {
	const CHAR *m_pszBackground;
	const CHAR *m_pszTextFile;

	INT m_nLeft;
	INT m_nTop;
	INT m_nRight;
	INT m_nBottom;

	INT m_nScrollRate;
	INT m_nPointSize;

};

#define CREDIT_TIMER_ID 567

#define NUM_SCREENS 12

#define MY_MASK_COLOR 1
#define LINE_HEIGHT 24
#define PIX_SCROLL_DY 1

ST_CREDITS g_cScreen[NUM_SCREENS] = {
	// Background        Credit text    TopLeft   BotRight  Delay   PointSize
	{ "BARAREA.BMP",    "CREDITS1.TXT", 370,   6, 636, 150, 30,     20 },
	{ "CILIA.BMP",      "CREDITS2.TXT",   6, 120, 310, 360, 10,     24 },
	{ "AUDITON.BMP",    "CREDITS3.TXT", 320, 120, 636, 360, 10,     24 },
	{ "CAST1.BMP",      "CAST1.TXT",      6, 120, 310, 360, 10,     24 },
	{ "CAST2.BMP",      "CAST2.TXT",    376, 120, 636, 360, 10,     24 },
	{ "CAST3.BMP",      "CAST3.TXT",      6, 120, 310, 360, 10,     24 },
	{ "CAST4.BMP",      "CAST4.TXT",      6, 120, 290, 360, 10,     24 },
	{ "VILDROID.BMP",   "CREDITS5.TXT", 394, 120, 636, 360, 10,     22 },
	{ "ZZAZZL.BMP",     "CREDITS6.TXT",   6, 220, 284, 474, 10,     24 },
	{ "SRAFFAN.BMP",    "CREDITS7.TXT", 352, 310, 636, 474, 10,     24 },
	{ "FLEEBIX.BMP",    "CREDITS8.TXT", 466, 230, 636, 474, 10,     18 },
	{ "TRISECKS.BMP",   "CREDITS9.TXT",   6, 374, 636, 474, 10,     24 }
};


CBagCreditsDialog::CBagCreditsDialog() {
	m_iScreen = 0;
	m_pszNextLine = m_pszEnd = nullptr;
	m_pszText = nullptr;
	m_nLines = 0;
	m_nNumPixels = 0;
	m_pCreditsBmp = nullptr;
	m_pSaveBmp = nullptr;

	m_bDisplay = FALSE;
}

VOID CBagCreditsDialog::OnInitDialog(VOID) {
	Assert(IsValidObject(this));

	// Hide cursor for credit screens
	CBofCursor::Hide();

	CBofDialog::OnInitDialog();

	SetReturnValue(-1);

	CBofPalette *pPal;

	// Start at 1st credit screen
	m_iScreen = 0;

	Assert(m_pBackdrop != nullptr);
	pPal = m_pBackdrop->GetPalette();
	SelectPalette(pPal);
	g_b1 = TRUE;

	// Load 1st credit text file
	LoadNextTextFile();
}


ERROR_CODE CBagCreditsDialog::LoadNextTextFile(VOID) {
	Assert(IsValidObject(this));

	// Our credits text must exist
	Assert(FileExists(BuildSysDir(g_cScreen[m_iScreen].m_pszTextFile)));

	CBofRect cRect;
	cRect.left = g_cScreen[m_iScreen].m_nLeft;
	cRect.top = g_cScreen[m_iScreen].m_nTop;
	cRect.right = g_cScreen[m_iScreen].m_nRight;
	cRect.bottom = g_cScreen[m_iScreen].m_nBottom;

	// Get rid of any previous work area
	if (m_pCreditsBmp != nullptr) {
		delete m_pCreditsBmp;
		m_pCreditsBmp = nullptr;
	}

	// Create a new work area
	if ((m_pCreditsBmp = new CBofBitmap(cRect.Width(), cRect.Height() + LINE_HEIGHT + 2, m_pBackdrop->GetPalette())) != nullptr) {
		m_pCreditsBmp->FillRect(nullptr, MY_MASK_COLOR);

		// Kill any previous work area
		if (m_pSaveBmp != nullptr) {
			delete m_pSaveBmp;
		}

		CBofRect tmpRect = m_pSaveBmp->GetRect();
		if ((m_pSaveBmp = new CBofBitmap(m_pCreditsBmp->Width(), m_pCreditsBmp->Height(), m_pBackdrop->GetPalette())) != nullptr) {
			m_pBackdrop->Paint(m_pSaveBmp, &tmpRect, &cRect);

		} else {
			ReportError(ERR_MEMORY);
		}
	}

	// Get rid of any previous credits screen
	if (m_pszText != nullptr) {
		BofFree(m_pszText);
		m_pszText = nullptr;
	}

	CBofFile cFile(BuildSysDir(g_cScreen[m_iScreen].m_pszTextFile), CBF_BINARY | CBF_READONLY);

	if (!cFile.ErrorOccurred()) {
		ULONG lSize;

		// Read in text file
		lSize = cFile.GetLength();
		if ((m_pszText = (CHAR *)BofCAlloc(lSize + 1, 1)) != nullptr) {

			cFile.Read(m_pszText, lSize);

			m_pszNextLine = m_pszText;
			m_pszEnd = m_pszText + lSize;
			m_nNumPixels = 0;

			// Determine the number of lines of text in credits
			m_nLines = StrCharCount(m_pszText, '\n');
			StrReplaceChar(m_pszText, '\r', ' ');
			StrReplaceChar(m_pszText, '\n', '\0');

			PaintLine(LinesPerPage() - 1, m_pszNextLine);
			NextLine();
			PaintLine(LinesPerPage(), m_pszNextLine);
			NextLine();

			m_bDisplay = TRUE;

		} else {
			ReportError(ERR_MEMORY, "Unable to allocate %d bytes for Credits.", lSize);
		}
	}

	return (m_errCode);
}

INT CBagCreditsDialog::LinesPerPage(VOID) {
	Assert(IsValidObject(this));

	INT n;
	n = (g_cScreen[m_iScreen].m_nBottom - g_cScreen[m_iScreen].m_nTop) / (LINE_HEIGHT + 2) + 1;

	return (n);
}


VOID CBagCreditsDialog::OnClose() {
	Assert(IsValidObject(this));

	if (m_pCreditsBmp != nullptr) {
		delete m_pCreditsBmp;
		m_pCreditsBmp = nullptr;
	}
	if (m_pSaveBmp != nullptr) {
		delete m_pSaveBmp;
		m_pSaveBmp = nullptr;
	}

	if (m_pszText != nullptr) {
		BofFree(m_pszText);
		m_pszText = nullptr;
	}

	m_pszNextLine = m_pszEnd = nullptr;
	m_nLines = 0;
	m_nNumPixels = 0;

	m_bDisplay = FALSE;

	KillBackdrop();

	CBofDialog::OnClose();

	// Can have cursor back now
	CBofCursor::Show();
}


VOID CBagCreditsDialog::OnPaint(CBofRect *pRect) {
	Assert(IsValidObject(this));

	PaintBackdrop(pRect);

	ValidateAnscestors();
}


VOID CBagCreditsDialog::OnLButtonDown(UINT /*nFlags*/, CBofPoint * /*pPoint*/) {
	Assert(IsValidObject(this));

	NextScreen();
}


VOID CBagCreditsDialog::OnKeyHit(ULONG /*lKey*/, ULONG /*nRepCount*/) {
	Assert(IsValidObject(this));

	NextScreen();
}


VOID CBagCreditsDialog::OnMainLoop(VOID) {
	Assert(IsValidObject(this));

	// If it's OK to show the credits
	//
	if (m_bDisplay) {
		DisplayCredits();
	}

	// Check again...could have changed in DisplayCredits()
	//
	if (m_bDisplay) {
		// Control the scroll rate
		Assert(m_iScreen >= 0 && m_iScreen < NUM_SCREENS);
		Sleep(g_cScreen[m_iScreen].m_nScrollRate);
	}
}


ERROR_CODE CBagCreditsDialog::DisplayCredits(VOID) {
	Assert(IsValidObject(this));

	if (m_nNumPixels < (m_nLines + LinesPerPage() + 1) * LINE_HEIGHT) {
		Assert(m_pCreditsBmp != nullptr);

		if (m_pCreditsBmp != nullptr) {
			Assert(m_pBackdrop != nullptr);
			Assert(m_pSaveBmp != nullptr);

			m_pSaveBmp->Paint(m_pBackdrop, g_cScreen[m_iScreen].m_nLeft, g_cScreen[m_iScreen].m_nTop);
			CBofRect cRect;
			cRect.SetRect(0, 0, m_pCreditsBmp->Width() - 1, m_pCreditsBmp->Height() - 1 - (LINE_HEIGHT + 2));
			m_pCreditsBmp->Paint(m_pBackdrop, g_cScreen[m_iScreen].m_nLeft, g_cScreen[m_iScreen].m_nTop, &cRect, MY_MASK_COLOR);

			if (g_b1) {
				m_pBackdrop->Paint(this, 0, 0);
				g_b1 = FALSE;

			} else {
				cRect.left = g_cScreen[m_iScreen].m_nLeft;
				cRect.top = g_cScreen[m_iScreen].m_nTop;
				cRect.right = cRect.left + m_pCreditsBmp->Width() - 1;
				cRect.bottom = cRect.top + m_pCreditsBmp->Height() - 1 - (LINE_HEIGHT + 2);
				m_pBackdrop->Paint(this, &cRect, &cRect);
			}


			// Strip off top layer so it won't wrap around
			INT i;
			for (i = 0; i < PIX_SCROLL_DY; i++) {
				m_pCreditsBmp->Line(0, i, m_pCreditsBmp->Width() - 1, i, MY_MASK_COLOR);
			}

			// Scroll text up 1 pixel
			m_pCreditsBmp->ScrollUp(PIX_SCROLL_DY);

			m_nNumPixels += PIX_SCROLL_DY;
			if ((m_nNumPixels % LINE_HEIGHT) == 0) {
				PaintLine(LinesPerPage(), m_pszNextLine);
				NextLine();
			}
		}

	} else {
		NextScreen();
	}

	return m_errCode;
}


ERROR_CODE CBagCreditsDialog::NextScreen(VOID) {
	Assert(IsValidObject(this));

	if (++m_iScreen < NUM_SCREENS) {
		CBofBitmap *pBmp;

		// Load next screen (flushes previous backdrop)
		//
		if ((pBmp = Bagel::LoadBitmap(BuildSysDir(g_cScreen[m_iScreen].m_pszBackground))) != nullptr) {
			SetBackdrop(pBmp);
			g_b1 = TRUE;
		}

		// Load credit text for this screen
		LoadNextTextFile();

		// Since there are no more screens to show, then we are outta here

	} else {
		m_bDisplay = FALSE;
		Close();
	}

	return m_errCode;
}


ERROR_CODE CBagCreditsDialog::PaintLine(INT nLine, CHAR *pszText) {
	Assert(IsValidObject(this));
	Assert(pszText != nullptr);
	Assert(nLine >= 0 && nLine <= LinesPerPage());
	Assert(m_pCreditsBmp != nullptr);

	CBofRect cRect;

	cRect.SetRect(0, nLine * LINE_HEIGHT, m_pCreditsBmp->Width() - 1, (nLine + 1) * LINE_HEIGHT - 1);

	m_pCreditsBmp->FillRect(&cRect, MY_MASK_COLOR);

	if (*pszText != '\0') {
		PaintShadowedText(m_pCreditsBmp, &cRect, pszText, g_cScreen[m_iScreen].m_nPointSize, TEXT_NORMAL, RGB(255, 255, 255), JUSTIFY_CENTER);
	}

	return m_errCode;
}


VOID CBagCreditsDialog::NextLine(VOID) {
	Assert(IsValidObject(this));

	Assert(m_pszNextLine != nullptr);

	if ((m_pszNextLine != nullptr) && (m_pszNextLine < m_pszEnd)) {

		while (*m_pszNextLine != '\0') {
			m_pszNextLine++;
		}

		if (m_pszNextLine < m_pszEnd) {
			m_pszNextLine++;
		}

		Assert(m_pszNextLine <= m_pszEnd);
	}
}

} // namespace Bagel
