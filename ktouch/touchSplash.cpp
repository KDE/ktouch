
#include "touchSplash.h"
#include "touchSplash.moc"
#include <kapplication.h>
#include <kstandarddirs.h>
#include <qcursor.h>

TouchSplash::TouchSplash(QWidget * parent, const char *name)
	   : QWidget(parent,name, WStyle_NoBorderEx | WStyle_Customize | WDestructiveClose ), m_bReadyToHide( false )
{
  QPixmap pm;
  pm.load(locate("appdata", "splash.png"));
  setBackgroundPixmap(pm);
  int screen = QApplication::desktop()->screenNumber(QCursor::pos());
  QRect geom = QApplication::desktop()->screenGeometry(screen);
  setGeometry(geom.width()/2-pm.width()/2 + geom.x(), 
              geom.height()/2-pm.height()/2 + geom.y(), 
	      pm.width(),pm.height());
}

TouchSplash::~TouchSplash(){
}
 
void TouchSplash::mousePressEvent( QMouseEvent*){
  if (m_bReadyToHide) hide();
}
