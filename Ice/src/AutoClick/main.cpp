/*
 * main.cpp
 *
 *  Created on: 2015��5��8��
 *      Author: Ice
 */

#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char** argv) {
	POINT pt1, pt2;
	GetCursorPos(&pt1);
	cout << "�ƶ���굽ָ��λ�ã�ֹͣ10s��ʼִ��:" << endl;
	int sec = 1000;
	int click = 10;
	for (;1;) {
		Sleep(sec);
		GetCursorPos(&pt2);
		int mov = abs(pt2.x-pt1.x) +  abs(pt2.y-pt1.y);
		pt1 = pt2;
		click--;
		if (mov < 8) {
			if (click <= 0) {
				//cout << pt.x << ":" << pt.y << endl;
				if (click == 0) cout << "��ʼִ��:" << endl;
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0 ,0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP,0 ,0, 0, 0);
			}
		}
		else {
			click = 10;
			cout << "��⵽��굽λ���ƶ���ֹͣ10s��ʼִ��:" << endl;
		}

	}
	return 0;
}


