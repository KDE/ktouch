
#include "touchSplash.h"
#include <kapplication.h>
#include <kstandarddirs.h>

TouchSplash::TouchSplash(QWidget * parent, const char *name)
	   : QWidget(parent,name, WStyle_NoBorderEx | WStyle_Customize | WDestructiveClose ), m_bReadyToHide( false )
{
  QPixmap pm;
  pm.load(locate("appdata", "splash.png"));
  setBackgroundPixmap(pm);
  int screen = QApplication::desktop()->screenNumber(parent);
  setGeometry(QApplication::desktop()->screenGeometry(screen).width()/2-pm.width()/2, 
              QApplication::desktop()->screenGeometry(screen).height()/2-pm.height()/2, 
	      pm.width(),pm.height());
}

TouchSplash::~TouchSplash(){
}
 
void TouchSplash::mousePressEvent( QMouseEvent*){
  if (m_bReadyToHide) hide();
}
