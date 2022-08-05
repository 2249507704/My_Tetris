#include<stdio.h>
#include "block.h"

/* 检查是否旋转后有碰撞 */
int checkCrash(int** map, Block* block) {
	int x = block->x;
	int y = block->y;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			/* 旋转前这个块是1  旋转后这个块也是1 则碰撞了 */
			if (block->shape[i][j] == 1 && map[x + i][y + j] == 1) {
				return 1;
			}
		}
	}
	return 0;
}

/* 检查是否成行 如果成行 就消掉 加分 */
int _checkLine(int** map, int line, int width) {
	for (int i = 0; i < width; i++) {
		if (map[line][i] == 0)
			return 0;
	}
	return 1;
}
int checkLine(int** map, int height, int width) {
	int indexL = -1;
	/* 检查那一行是成行的 */
	for (int i = 0; i < height - 1; i++) {
		int sign = 0;
		if (_checkLine(map, i, width) == 1) {
			indexL = i;
			break;
		}
	}
	//如果某一行成行，则将当前行的值替换成上一行的值，并且以上的行也进行相同操作，除了第一行。
	if (indexL != -1) {
		for (int i = indexL; i > 0; i--) {
			for (int j = 0; j < width; j++) {
				map[i][j] = map[i - 1][j];
			}
		}
	}
	else {
		return 0;
	}

	return 1;
}