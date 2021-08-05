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
			int y; //방의 y좌표
			int x; //방의 x좌표
		} Coordinate;

		typedef struct _Node {
			int data;
			struct _Node* next;
		} Node;

		typedef struct _MazeRoom {
			int roomNum; //방이 속한 집합의 번호
			bool visited; //DFS 중 방문하였는가?
			bool hasRightWall; //오른쪽 벽이 있는가?
			bool hasDownWall; //아래 벽이 있는가?
		} MazeRoom;

		typedef struct _TeleRoom {
			bool isTele; //teleport platform인가?
			int teleNum; //teleport platform의 번호
			int gotoX; //teleport 되는 x좌표
			int gotoY; //teleport 되는 y좌표
		} TeleRoom;

		typedef struct _RoomNumSet {
			int roomNum;
			int elementN;
			unsigned char connected;

			struct _RoomNumSet *nextSet;
			Node *index;
		} RoomNumSet;

		typedef struct _TeleCandidateSets {
			int setNum; //후보군이 속한 집합의 번호
			vector <pair<int, int>> coordinates; // 해당 집합에 속한 teleport platform의 후보군 
		} TeleCandidateSets;

	
		MazeRoom **maze;
		TeleRoom **tele;
		vector <TeleCandidateSets> candidateSets;
		vector <Coordinate> s;

		int height, width;
		int lastRoomN;
		int lastTeleN;

		int isOpen; //미로가 생성되었는지 판단하는 변수. 0이면 생성되지 않았고 1이면 생성되었다.
		int isDFS; //DFS함수를 실행시켰는지 판단하는 변수. 0이면 실행 안했고 1이면 실행했다.
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
