/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/

	#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon


class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		
		void keyPressed(int key); // Traps escape key if exit disabled
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void freeMemory();
		bool DFS();
		void DFSdraw();

		void makeTeleMaze();
		void initializeMaze();
		void eraseRightWalls(int rowN);
		void eraseDownWalls(int rowN);
		void makeTeleCandidates(int rowN);
		void activateTele(int rowN);
		void fillRoomNum(int rowN);
		void eraseLastRightWalls();
		void printMaze();

		typedef struct _Coordinate {
			int y; //���� y��ǥ
			int x; //���� x��ǥ
		} Coordinate;

		typedef struct _Node {
			int data;
			struct _Node* next;
		} Node;

		typedef struct _MazeRoom {
			int roomNum; //���� ���� ������ ��ȣ
			bool visited; //DFS �� �湮�Ͽ��°�?
			bool hasRightWall; //������ ���� �ִ°�?
			bool hasDownWall; //�Ʒ� ���� �ִ°�?
		} MazeRoom;

		typedef struct _TeleRoom {
			bool isTele; //teleport platform�ΰ�?
			int teleNum; //teleport platform�� ��ȣ
			int gotoX; //teleport �Ǵ� x��ǥ
			int gotoY; //teleport �Ǵ� y��ǥ
		} TeleRoom;

		typedef struct _RoomNumSet {
			int roomNum;
			int elementN;
			unsigned char connected;

			struct _RoomNumSet *nextSet;
			Node *index;
		} RoomNumSet;

		typedef struct _TeleCandidateSets {
			int setNum; //�ĺ����� ���� ������ ��ȣ
			vector <pair<int, int>> coordinates; // �ش� ���տ� ���� teleport platform�� �ĺ��� 
		} TeleCandidateSets;

	
		MazeRoom **maze;
		TeleRoom **tele;
		vector <TeleCandidateSets> candidateSets;
		vector <Coordinate> s;

		int height, width;
		int lastRoomN;
		int lastTeleN;

		int isOpen; //�̷ΰ� �����Ǿ����� �Ǵ��ϴ� ����. 0�̸� �������� �ʾҰ� 1�̸� �����Ǿ���.
		int isDFS; //DFS�Լ��� ������״��� �Ǵ��ϴ� ����. 0�̸� ���� ���߰� 1�̸� �����ߴ�.
		int isFirst;
		// Menu
		ofxWinMenu * menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function
		
		// Used by example app
		ofTrueTypeFont myFont;
        ofImage myImage;
		float windowWidth, windowHeight;
		HWND hWnd; // Application window
		HWND hWndForeground; // current foreground window

		// Example menu variables
		bool bShowInfo;
		bool bFullscreen;
		bool bTopmost;

		// Example functions
 		void doFullScreen(bool bFull);
		void doTopmost(bool bTop);

};
