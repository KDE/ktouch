
#include "touchSplash.h"
#include <kapplication.h>
#include <kstandarddirs.h>

TouchSplash::TouchSplash(QWidget * parent, const char *name)
	   : QWidget(parent,name, WStyle_NoBorderEx | WStyle_Customize | WDestructiveClose ), m_bReadyToHide( false )
{
  QPixmap pm;
  pm.load(locate("appdata", "splash.png"));
  setBackgroundPixmap(pm);
  setGeometry(QApplication::desktop()->width()/2-pm.width()/2, QApplication::desktop()->height()/2-pm.height()/2, pm.width(),pm.height());
}

TouchSplash::~TouchSplash(){
}
 
void TouchSplash::mousePressEvent( QMouseEvent*){
  if (m_bReadyToHide) hide();
}
