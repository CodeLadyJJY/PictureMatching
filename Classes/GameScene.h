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
	int total_num;									// ͼƬ����
	int type;										// ͼƬ����
	int touch;										// ������ţ�0��ʾ����һ��ͼƬ��1��ʾ���ڶ���ͼƬ
	int x1, x2, y1, y2;								// �����Ⱥ���������ͼƬ����
	int type_map[height][width];					// ����ͼƬ���ͣ�0��ʾ�գ�1-16��ʾͼƬ���
	Sprite *spriteMap[height][width];				// ����ͼƬ����

	node queue[202];
	node begin, end, cur, ext;
	int head, rear;
	bool visited[height][width];
	int crossNum[height][width];					// ��¼ת����

	void createPicture(int r, int c);				// ����ͼƬ
	void judge();									// �ж��Ƿ�����
	int bfs(int type);								// type=1 ��ʾ�ж�����������ͬͼƬ�Ƿ��������type=2 ��ʾ�ж��Ƿ��п�������ͼƬ
	bool checkCanClear();							// ����Ƿ��п�������ͼƬ
	void selectPicture(int r, int c, int type);		// ѡ��ͼƬ��type��ʾͼƬ���
	void unselectPicture(int r, int c, int type);	// ȡ��ѡ��ͼƬ
	void deletePicture(int r, int c);				// ɾ��ͼƬ

	void gameOver(std::string s);
};

#endif
