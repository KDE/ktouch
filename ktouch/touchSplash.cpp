
#include "touchSplash.h"
#include "touchSplash.moc"
#include <kapplication.h>
#include <kstandarddirs.h>
#include <kconfig.h>
#include <qcursor.h>

TouchSplash::TouchSplash(QWidget * parent, const char *name)
	   : QWidget(parent,name, WStyle_NoBorderEx | WStyle_Customize | WDestructiveClose ), m_bReadyToHide( false )
{
  QPixmap pm;
  pm.load(locate("appdata", "splash.png"));
  setBackgroundPixmap(pm);

  QRect desk;
  KConfig gc("kdeglobals", false, false);
  gc.setGroup("Windows");
  int scr = gc.readNumEntry("Unmanaged", -3);
  if (QApplication::desktop()->isVirtualDesktop() &&
      gc.readBoolEntry("XineramaEnabled", true) &&
      scr != -2) {
    if (scr == -3)
      scr = QApplication::desktop()->screenNumber(QCursor::pos());
    desk = QApplication::desktop()->screenGeometry(scr);
  } else {
    desk = QApplication::desktop()->geometry();
  }

  setGeometry(desk.width()/2-pm.width()/2 + desk.x(), 
              desk.height()/2-pm.height()/2 + desk.y(), 
	      pm.width(),pm.height());
}

TouchSplash::~TouchSplash(){
}
 
void TouchSplash::mousePressEvent( QMouseEvent*){
  if (m_bReadyToHide) hide();
}
