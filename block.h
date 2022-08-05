#pragma once
#include <stdio.h>
#include <iostream>

// 创建一个块类 来把块的属性定义进来
class Block // class declaration
{
// 坐标以左上角为标准
public:
	int x; // 横坐标，以左上角为标志位 block相对于map来说的偏移量
	int y; // 纵坐标，以左上角为标志位 
	int type; // 方块类型 用三维数组的第一个参数来当type
	int director; // 旋转方向 0:向左，1向右 
	int shape[4][4]; // 格子大小 
	/*
	*	 0 1  2 3
	*  0 □□□□    如果是棍子形状 就把00 01 02 03 赋值给1 让他显示
	*  1 □□□□
	*  2 □□□□
	*  3 □□□□
	*/
	int shapes[8][4][4]; // 有8个4*4的二维数组
public:
	// 设置方块属性 
	// 设计一块4*4的二维数组用来存放形状  参数是：生成方块的坐标 以及 方块的类型
	/*
	* x，y就是shape矩阵再map矩阵中的偏移量。
	* 这样我们就可以很清楚地知道这个shape矩阵现在位于map矩阵中的哪个位置，就可以直接进行计算，绘制，以及判断等操作。
	*/
	void set(int _x, int _y, int _shape) {
		x = _x;
		y = _y;
		if (_shape != -1) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					shape[i][j] = shapes[_shape][i][j];
			type = _shape;
			director = 0;
		}

	}
	// 生成俄罗斯方块
	void generate() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					shapes[i][j][k] = 0;

		/* 石头 */
		shapes[0][1][1] = 1;
		/* 棍子 */
		shapes[1][1][0] = shapes[1][1][1] = shapes[1][1][2] = shapes[1][1][3] = 1;
		/* 七 (左）*/
		shapes[2][0][0] = shapes[2][0][1] = shapes[2][1][1] = shapes[2][2][1] = 1;
		/* 七 (右) */
		shapes[3][0][1] = shapes[3][0][2] = shapes[3][1][1] = shapes[3][2][1] = 1;
		/* 凸 */
		shapes[4][0][1] = shapes[4][1][0] = shapes[4][1][1] = shapes[4][2][1] = 1;
		/* 田 */
		shapes[5][1][1] = shapes[5][1][2] = shapes[5][2][1] = shapes[5][2][2] = 1;
		/* Z(左) */
		shapes[6][0][0] = shapes[6][0][1] = shapes[6][1][1] = shapes[6][1][2] = 1;
		/* Z(右)*/
		shapes[7][0][2] = shapes[7][0][1] = shapes[7][1][1] = shapes[7][1][0] = 1;
		/*
		* 其中，4*4的网格（矩阵）中， 
		* 取值为1表示的是此处有方块■，其他为0则表示没有方块，这样就完成了方块的初始化了。
		* 然后外部通过传入方块类型type来指定当前初始化的俄罗斯方块属于什么方块，
		* 这也为了后续的随机生成方块以及方块提示做了准备。
		*/
	}
};