#include "GameScene.h"
#include "GameOverScene.h"

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	srand(time(NULL));

    Size size = Director::getInstance()->getWinSize();
    
	// 背景
	auto bg = Sprite::create("bg.png");
	bg->setRotation(90);
	bg->setPosition(size / 2);
	this->addChild(bg);

	// 初始化
	total_num = width * height;
	touch = 0;
	head = 0;
	rear = -1;
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
		{
			type_map[i][j] = 0;
			visited[i][j] = 0;
		}

	// 布图
	for (int i = 0; i < height / 2; ++i)
		for (int j = 0; j < width; ++j)
		{
			type = rand() % 16 + 1;
			createPicture(i, j);

			while (1)
			{
				int r = rand() % (height / 2) + height / 2;
				int c = rand() % width;

				if (type_map[r][c] == 0)
				{
					createPicture(r, c);
					break;
				}
			}
		}

	// 触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *t, Event *e)
	{
		int x = t->getLocation().y / tiled_width - 1;
		int y = (t->getLocation().x + tiled_width / 3) / tiled_width - 1;

		if (type_map[x][y] != 0)
		{
			if (touch == 0 && type_map[x][y] != 0)
			{
				x1 = x;				// 获取第一个图片的坐标
				y1 = y;
				this->selectPicture(x1, y1, 1);
				touch = !touch;
			}
			else if (touch == 1 && (x != x1 || y != y2))
			{
				x2 = x;				// 获取第二个图片的坐标
				y2 = y;

				if (type_map[x1][y1] != type_map[x2][y2])
				{
					unselectPicture(x1, y1, 1);
					x1 = x;
					y1 = y;
					this->selectPicture(x1, y1, 1);
				}
				else
				{
					this->selectPicture(x2, y2, 2);
					this->judge();
					touch = !touch;

					if (total_num == 0)
					{
						gameOver("You win!");
						return true;
					}

					if (checkCanClear() == 0)
					{
						gameOver("No Elements To Clear");
					}
				}
			}
		}

		return true;
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GameScene::createPicture(int r, int c)
{
	// 创建图片
	auto sprite = Sprite::create(StringUtils::format("%d.png", type));
	sprite->setAnchorPoint(Point::ZERO);
	sprite->setPosition((c+1) * tiled_width - tiled_width / 3, (r+1) * tiled_width);
	this->addChild(sprite);

	type_map[r][c] = type;
	spriteMap[r][c] = sprite;
}

void GameScene::judge()
{
	// 图片取消选择
	unselectPicture(x1, y1, 1);
	unselectPicture(x2, y2, 2);

	// 起始点和终止点
	head = 0;
	rear = -1;
	begin.x = x1;
	begin.y = y1;
	end.x = x2;
	end.y = y2;

	// 初始化
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			visited[i][j] = 0;
			crossNum[i][j] = -1;
		}
	}

	// 广度优先搜索
	if (bfs(1) != -1)
	{
		deletePicture(x1, y1);
		deletePicture(x2, y2);
		total_num -= 2;
	}
}

int GameScene::bfs(int type) 
{
	queue[++rear] = begin;
	while (head <= rear) 
	{
		cur = queue[head];
		visited[cur.x][cur.y] = 1;
		head++;

		for (int i = 0; i < 4; ++i) // 4个方向
		{
			ext.x = cur.x;
			ext.y = cur.y;

			while (1)
			{
				ext.x = ext.x + dir[i][0];
				ext.y = ext.y + dir[i][1];

				if (ext.x >= height || ext.x < 0 || ext.y >= width || ext.y < 0) // 出界，终止该方向
				{
					break;
				}

				if (visited[ext.x][ext.y] == 0) // 如果该点没有走过
				{
					if (type_map[ext.x][ext.y] != 0)
					{
						if (type == 1)
						{
							if (ext.x == end.x && ext.y == end.y) // 到终点
							{
								crossNum[ext.x][ext.y] = crossNum[cur.x][cur.y] + 1;
								if (crossNum[ext.x][ext.y] <= 2)
									return 0;
								else
									return -1;
							}
							else // 有图片，终止该方向
							{
								break;
							}
						}
						else if (type == 2)
						{
							if (type_map[ext.x][ext.y] == type_map[begin.x][begin.y]) // 遇到相同图片
							{
								crossNum[ext.x][ext.y] = crossNum[cur.x][cur.y] + 1;
								if (crossNum[ext.x][ext.y] <= 2)
									return 0;
								else
									return -1;
							}
							else // 有图片，终止该方向
							{
								break;
							}
						}
					}
					else // 空图片进队列
					{
						visited[ext.x][ext.y] = 1;
						crossNum[ext.x][ext.y] = crossNum[cur.x][cur.y] + 1;
						if (crossNum[ext.x][ext.y] <= 2)
						{
							++rear;
							queue[rear].x = ext.x;
							queue[rear].y = ext.y;
						}
					}
				}
			}
		}
	}
	return -1;
}

bool GameScene::checkCanClear()
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (type_map[i][j] != 0)
			{
				// 起始点和终止点
				head = 0;
				rear = -1;
				begin.x = i;
				begin.y = j;

				// 初始化
				for (int i = 0; i < height; ++i)
				{
					for (int j = 0; j < width; ++j)
					{
						visited[i][j] = 0;
						crossNum[i][j] = -1;
					}
				}

				// 广度优先搜索
				if (bfs(2) != -1)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

void GameScene::selectPicture(int r, int c, int type)
{
	auto sprite = Sprite::create("CloseNormal.png");
	sprite->setTag(type);
	sprite->setAnchorPoint(Point::ZERO);
	sprite->setPosition((c + 1) * tiled_width - tiled_width / 3, (r + 1) * tiled_width);
	this->addChild(sprite);
}

void GameScene::unselectPicture(int r, int c, int type)
{
	this->getChildByTag(type)->removeFromParent();
}

void GameScene::deletePicture(int r, int c)
{
	type_map[r][c] = 0;
	spriteMap[r][c]->removeFromParent();
}

void GameScene::gameOver(std::string s)
{
	auto scene = GameOverScene::createScene(s);
	Director::getInstance()->replaceScene(scene);
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
