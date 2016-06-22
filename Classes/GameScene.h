#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

const int width = 12;
const int height = 6;
const int tiled_width = 72;
const int dir[4][2] = { 1,0,-1,0,0,1,0,-1 };

typedef struct
{
	int x, y;
}node;

class GameScene : public Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(GameScene);

private:
	int total_num;									// 图片个数
	int type;										// 图片类型
	int touch;										// 触摸编号，0表示按第一个图片，1表示按第二个图片
	int x1, x2, y1, y2;								// 保存先后点击的两个图片坐标
	int type_map[height][width];					// 保存图片类型，0表示空，1-16表示图片编号
	Sprite *spriteMap[height][width];				// 保存图片精灵

	node queue[202];
	node begin, end, cur, ext;
	int head, rear;
	bool visited[height][width];
	int crossNum[height][width];					// 记录转弯数

	void createPicture(int r, int c);				// 创造图片
	void judge();									// 判断是否消除
	int bfs(int type);								// type=1 表示判断两张类型相同图片是否可消除，type=2 表示判断是否有可消除的图片
	bool checkCanClear();							// 检查是否还有可消除的图片
	void selectPicture(int r, int c, int type);		// 选择图片，type表示图片序号
	void unselectPicture(int r, int c, int type);	// 取消选择图片
	void deletePicture(int r, int c);				// 删除图片

	void gameOver(std::string s);
};

#endif
