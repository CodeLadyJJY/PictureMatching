#ifndef  _GAME_OVER_SCENE_H_
#define  _GAME_OVER_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class GameOverScene : public LayerColor
{
public:
	static Scene* createScene(string s);
	virtual bool init();
	CREATE_FUNC(GameOverScene);
	void menuRestartCallBack(Ref* obj);	// ���¿�ʼ
	void menuCloseCallBack(Ref* obj);	// �˳�
};

#endif