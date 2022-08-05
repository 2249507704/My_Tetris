#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "block.h"
/* draw */
extern void cleanBlock(Block block);
extern void drawBlock(Block block);
extern void addBlock(int** map, Block block);
/* check */
extern int checkCrash(int** map, Block* block);

int ch = 0;
int t = 50;

void getAction() {
	clock_t start, now;
	start = clock();


	while (1) {
		if (_kbhit()) {
			ch = _getch();
			break;
		}
		now = clock();
		int duration = (double)(now - start);
		if (duration > t)
			break;
	}
}
/* x，y来存储一个俄罗斯方块在地图map中的偏移量 */
int _move(int** map, Block* block, int x, int y, int sign) {
	// sign： 1表示向下移动， 0表示左右移动 
	/* 消除所有属于block的块 */
	/* 重新定位block的位置，生成新的block */
	cleanBlock(*block);
	block->set(block->x + x, block->y + y, -1);  // 这个+x +y是控制往下移动的 
	if (checkCrash(map, block) == 1) { // 如果block碰到了地图
		block->set(block->x - x, block->y - y, -1); //如果方块和地图碰撞了 就不能继续加了 要减去一个偏移量
		drawBlock(*block); // 把落地的这个画出来
		if (sign == 1) // 一直向下碰撞到了 
			addBlock(map, *block); //在碰撞到的地方把这个俄罗斯方块画出来
		return 1;
	}
	drawBlock(*block);
	return 0;
}

/* 矩阵旋转90度 */
void rotation(Block* block, int director) {
	// 我们只旋转左上角3*3的矩阵，并且向director旋转90度

	/* 向左旋转 */
	if (director == 0) {
		/* 角转换 */
		int value = block->shape[0][0];
		block->shape[0][0] = block->shape[0][2];
		block->shape[0][2] = block->shape[2][2];
		block->shape[2][2] = block->shape[2][0];
		block->shape[2][0] = value;
		/* 十字转换 */
		value = block->shape[0][1];
		block->shape[0][1] = block->shape[1][2];
		block->shape[1][2] = block->shape[2][1];
		block->shape[2][1] = block->shape[1][0];
		block->shape[1][0] = value;
	}
	/* 向右转换 */
	else if (director == 1) {
		/* 角转换 */
		int value = block->shape[0][0];
		block->shape[0][0] = block->shape[2][0];
		block->shape[2][0] = block->shape[2][2];
		block->shape[2][2] = block->shape[0][2];
		block->shape[0][2] = value;
		/* 十字转换 */
		value = block->shape[0][1];
		block->shape[0][1] = block->shape[1][0];
		block->shape[1][0] = block->shape[2][1];
		block->shape[2][1] = block->shape[1][2];
		block->shape[1][2] = value;
	}

	/* 处理棍子的特殊情况 */
	if (block->type == 1) {
		if (block->shape[1][3] == 1) {
			block->shape[1][3] = 0;
			block->shape[3][1] = 1;
		}
		else {
			block->shape[1][3] = 1;
			block->shape[3][1] = 0;
		}
	}
}
/* 函数参数：地图 方块 和 旋转方式*/
void _transfer(int** map, Block* block, int sign) {
	int director = block->director;
	cleanBlock(*block);	 // 擦除旋转前的block  也就是把旋转前的实心方块变为空白方块
	switch (sign) {
	case 0:
		rotation(block, director);
		// 旋转后进行碰撞检查
		if (checkCrash(map, block) == 1) {
			rotation(block, director ^ 1); //异或 相同为0 不同为1 向左转 如果有碰撞 就向反方向旋转
		}
		break;
	case 1:
		/* 棍子和 Z 只能 两个方向旋转 */
		rotation(block, director);
		block->director = block->director ^ 1; // 向相反方向旋转
		// 旋转后进行碰撞检查
		if (checkCrash(map, block) == 1) {
			rotation(block, director ^ 1);
			block->director = block->director ^ 1;
		}
		break;
	}

	drawBlock(*block);  // 重新绘制旋转后的block 
}
/* 旋转 */
void transfer(int** map, Block* block) {

	int sign = block->type;
	/*
	* sign： 什么类型的方块， 不同类型的方块的旋转策略不同
	*/
	if (sign == 2 || sign == 3 || sign == 4) { // 在左上角的三格内旋转 
		_transfer(map, block, 0);
	}
	else if (sign == 1 || sign == 6 || sign == 7) {	// 处理棍子,Z
		_transfer(map, block, 1);
	}
	else if (sign == 0 || sign == 5) {	// 不需要处理的  石头和田
//		_transfer(block, 2);
		return;
	}


}

int _action(int** map, Block* block, int c) {
	int isButtom = 0;
	ch = c;
	if (ch == -1) {
		getAction();
	}
	switch (ch) {
	case 119:
		/* 旋转 */
		transfer(map, block);
		break;
	case 115:
		/* 向下走 */
		isButtom = _move(map, block, 1, 0, 1); // 检查是否接触到了底部 
		break;
	case 97:
		/* 向左走 */
		_move(map, block, 0, -1, 0);
		break;
	case 100:
		/* 向右走 */
		_move(map, block, 0, 1, 0);
		break;
	case 224:                    //方向键区的ASCII码
		switch (_getch()) {
		case 72:
			/* 旋转 */
			transfer(map, block);
			break;
		case 80:
			/* 向下走 */
			isButtom = _move(map, block, 1, 0, 1); // 检查是否接触到了底部 
			break;
		case 75:
			/* 向左走 */
			_move(map, block, 0, -1, 0);
			break;
		case 77:
			/* 向右走 */
			_move(map, block, 0, 1, 0);
			break;
		}
	}
	if (isButtom == 1) {
		return 1;
	}
	return 0;
}