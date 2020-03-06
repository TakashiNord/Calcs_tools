//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "about.h"
#include "MAPI.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAbout *FormAbout;
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::Image2Click(TObject *Sender)
{
 	TMapiMessage MapiMessage;
	Cardinal MError;

	MapiMessage.ulReserved = 0;
	MapiMessage.lpszSubject = "Замечание к программе..."; // ....@knaapo.ru
	MapiMessage.lpszNoteText = "Привет. Меня зовут ....";
	MapiMessage.lpszMessageType = NULL;
	MapiMessage.lpszDateReceived = NULL;
	MapiMessage.lpszConversationID = NULL;
	MapiMessage.flFlags = 0;
	MapiMessage.lpOriginator = NULL;
	MapiMessage.nRecipCount = 0;
	MapiMessage.lpRecips = NULL;
	MapiMessage.nFileCount = 0;
	MapiMessage.lpFiles = NULL;

	MError = MapiSendMail(0, reinterpret_cast<unsigned int>(Application->Handle), 
                 MapiMessage, MAPI_DIALOG | MAPI_LOGON_UI | MAPI_NEW_SESSION, 0);

	if (MError)
		MessageDlg("Ошибка посылки письма...", mtError, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

