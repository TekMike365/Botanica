#include "Log.h"
#include "Application.h"

int main()
{
    Log::Init();
    BT_DLOG_INFO("Initialised Logger");

    Application app;
    app.Run();
}
