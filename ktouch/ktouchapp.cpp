#include <kwin.h>
#include <kdebug.h>
#include <kcmdlineargs.h>
#include <kurl.h>

#include "ktouchapp.h"
#include "ktouch.h"
#include "ktouchapp.moc"

KTouchApp::KTouchApp()
         : KUniqueApplication(),top()
{
}

KTouchApp::~KTouchApp()
{
}

int KTouchApp::newInstance()
{
	if (isRestored())
	{
		// There can only be one main window
		if (KMainWindow::canBeRestored(1)) {
			top = new KTouch;
			top->restore(1);
		}
	}
	else
	{
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

		if (top)
		{
			kdDebug() << "KTouch already running." << endl;
			KWin::setActiveWindow(top->winId());
		}
		else
		{
			top = new KTouch;
			top->show();
		}

	}

	return 0;
}

