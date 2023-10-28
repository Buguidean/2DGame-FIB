#ifndef _CREDIT_SCENE_INCLUDE
#define _CREDIT_SCENE_INCLUDE

#include "GameScene.h"

class CreditScene :
	protected GameScene
{
public:
	CreditScene();
	~CreditScene();

private:
	void initShaders();
};

#endif // _CREDIT_SCENE_INCLUDE
