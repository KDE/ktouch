
#ifndef TOUCHSPLASH_H
#define TOUCHSPLASH_H

#include <qwidget.h>

/**
  *@author haavard froeiland
  */

class TouchSplash : public QWidget  {
   Q_OBJECT

public:
	TouchSplash(QWidget *parent=0, const char *name=0);
	~TouchSplash();

	void setHideEnabled(bool bEnabled) { m_bReadyToHide = bEnabled; };

protected:
  virtual void mousePressEvent( QMouseEvent*);
  bool m_bReadyToHide;
};

#endif
