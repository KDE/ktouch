#ifndef KTOUCHAPP_H
#define KTOUCHAPP_H

#include <kuniqueapp.h>

class KTouch;

class KTouchApp : public KUniqueApplication
{
	Q_OBJECT

public:
	KTouchApp();
	~KTouchApp();

	int newInstance();
	KTouch* getInstance();

private:
	KTouch *top;

};

#endif
