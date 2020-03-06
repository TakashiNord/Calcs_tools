//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("CalcTools.res");
USEFORM("main.cpp", FormMain);
USEFORM("about.cpp", FormAbout);
USEFORM("Message.cpp", FormMessage);
USEFORM("Miniature.cpp", FormMiniature);
USEFORM("Graphic.cpp", FormGraphic);
USEFORM("Help.cpp", FormHelp);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFormMain), &FormMain);
                 Application->CreateForm(__classid(TFormMessage), &FormMessage);
                 Application->CreateForm(__classid(TFormMiniature), &FormMiniature);
                 Application->CreateForm(__classid(TFormGraphic), &FormGraphic);
                 Application->CreateForm(__classid(TFormHelp), &FormHelp);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
