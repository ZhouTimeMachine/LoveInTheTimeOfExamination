#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <tchar.h>
#include "drawer.h"
using namespace std;

void utf82unicode(string src, wchar_t* des) {
	memset(des, 0, sizeof(des));
	int j = 0;
	for (int i = 0; i < src.size();) {
		if (((unsigned char)src[i]) <= 127) {
			unsigned char tmp1 = 0;
			unsigned char tmp2 = src[i];
			des[j] = (tmp1 << 8) | (tmp2);
			j++, i++;
			continue;
		}
		unsigned char tmp1 = ((unsigned char)(src[i] << 4) | (unsigned char)((unsigned char)(src[i + 1] & 0x3C) >> 2));
		unsigned char tmp2 = ((unsigned char)((unsigned char)(src[i + 1] & 0x03) << 6) | (unsigned char)(src[i + 2] & 0x3F));
		des[j] = (tmp1 << 8) | (tmp2);
		j++, i+=3;
	}
	des[j] = 0;
}

drawer::drawer(int x, int y): resolutionX(x), resolutionY(y){
	initgraph(x, y, EW_SHOWCONSOLE); // creates the drawing window
}

void drawer::texting(dialogue D, int x, int y){
	clearrectangle(x, y, resolutionX, resolutionY);
    int linecharacter = (resolutionX-100 - x) / 16;
	int t = 0, i = 0;
	string text = D.text;
	cout << text << endl;
	while(1) {
		wchar_t sentence[500];
        utf82unicode(text.substr(t*3, min(text.size()-t, 3*linecharacter)), sentence);
		outtextxy(x, y+i*20, sentence);
        t += linecharacter;
		i++;
		if (t*3 > text.size()) break;
	}
	if (D.hasBranch) {
		vector<string>::iterator iter;
		int op = 1;
		for (iter = D.branches.begin(); iter != D.branches.end(); iter++) {
			cout << op << ": " << *iter << endl;
			string tmp = to_string(op) + ": " + (*iter);
			t = 0, op++;
			while (1) {
				wchar_t sentence[500];
				utf82unicode(tmp.substr(t * 3, min(tmp.size() - t, 3 * linecharacter)), sentence);
				outtextxy(x, y + i * 20, sentence);
				t += linecharacter;
				i++;
				if (t * 3 > tmp.size()) break;
			}
		}
	}
	line(x, y-10, resolutionX - 100, y-10);
}

void drawer::drawSpeaker(int speaker) {
	wchar_t filename[100];
	IMAGE img;
	wsprintf(filename, L".//pic//role%d.png", speaker);
	loadimage(&img, filename, 200, 240, true);
	cout << "w, h: " << img.getwidth() << " " << img.getheight() << endl;
	putimage(50, 350, &img);
}

void drawer::drawDateTime(int month, int day, int time) {
	int startpos = 20;
	wchar_t tmp[10];
	wsprintf(tmp, _T("%d"), month);
	outtextxy(startpos, 20, tmp);
	outtextxy(startpos+15, 20, L"月");
	wsprintf(tmp, _T("%d"), day);
	outtextxy(startpos+35, 20, tmp);
	outtextxy(startpos+55, 20, L"日:");
	if (time == 0) {
		outtextxy(startpos + 80, 20, L"上午");
	} else if (time == 1) {
		outtextxy(startpos + 80, 20, L"下午");
	} else if (time == 2) {
		outtextxy(startpos + 80, 20, L"晚上");
	} else outtextxy(startpos + 80, 20, L"凌晨");
}