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
	void menuRestartCallBack(Ref* obj);	// 重新开始
	void menuCloseCallBack(Ref* obj);	// 退出
};

#endif