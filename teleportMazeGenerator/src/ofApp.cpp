/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

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

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#define _CRT_SECURE_NO_WARNINGS

#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Teleportation Maze"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	ofSetBackgroundAuto(TRUE);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = 0;
	isOpen = 0;
	isFirst = 1;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Make", false, false); // Not checked and not auto-checked
	menu->AddPopupItem(hPopup, "Print and export", false, false);
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;

	//
	// File menu
	//
	if(title == "Make") {
		if (!isFirst) {
			freeMemory();
		}
		else isFirst = 0;
		makeTeleMaze();
		isOpen = 1;
	}
	if (title == "Print and export") {
		if (isOpen) {
			printMaze();
		}
		else cout << "You must make a maze first" << endl;
	}
	if(title == "Exit") {
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		if (isOpen)
		{
			if(DFS()) isDFS = TRUE;
			else printf("DFS failed\n");
		}
		else
			cout << "You must make a maze first" << endl;
		
	}
	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}
	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {
}


//--------------------------------------------------------------
void ofApp::draw() {
	int i, j;
	float y, x;
	// 저장된 자료구조를 이용해 미로를 그린다.
	if (isDFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			DFSdraw();
		}
		else {
			cout << "You must make a maze first." << endl;
			isDFS = FALSE;
		}
	}
	ofSetColor(100);
	ofSetLineWidth(5);
	if (isOpen)
	{
		float roomSize = (width > height) ? 700 / (float)width : 700 / (float)height;
		ofDrawLine(0, 0, roomSize * width, 0);
		ofDrawLine(0, 0, 0, roomSize * height);
		for (i = 0, y = 0; i < height; i++, y += roomSize) {
			for (j = 0, x = 0; j < width; j++, x += roomSize) {
				if (maze[i][j].hasDownWall == TRUE) ofDrawLine(x, y + roomSize, x + roomSize, y + roomSize);
				if (maze[i][j].hasRightWall == TRUE) ofDrawLine(x + roomSize, y, x + roomSize, y + roomSize);
	
				if (tele[i][j].isTele == 1) {
					int colorN = tele[i][j].teleNum;
					
					string teleString = "";
					teleString.push_back(colorN);
					srand(colorN);
					ofSetColor(rand() % 256, rand() % 256, rand() % 256);
					ofDrawCircle(x + roomSize / 2, y + roomSize / 2, roomSize / 3);
					ofSetColor(255);
					ofDrawBitmapString(teleString, x + roomSize / 2 - 4, y + roomSize / 2 + 4);
					ofSetColor(100);
				}
			}
		}
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::freeMemory() {

	// 동적할당 받은 memory를 해제해주는 함수
	int i;
	for (i = 0; i < height; i++) {
		delete[] maze[i];
		delete[] tele[i];
	}
	delete[] maze;
	delete[] tele;
	for (i = 0; i < candidateSets.size(); i++) {
		candidateSets[i].coordinates.clear();
		vector<pair<int, int>>().swap(candidateSets[i].coordinates);
	}
	candidateSets.clear();
	vector<TeleCandidateSets>().swap(candidateSets);
	s.clear();
	vector<Coordinate>().swap(s);
}

//DFS탐색을 하는 함수
bool ofApp::DFS()
{
	int i, j;
	Coordinate target = { height - 1, width - 1 };
	Coordinate start = { 0, 0 };
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			maze[i][j].visited = FALSE;
		}
	}
	s.clear();
	s.push_back(start);
	maze[s.back().y][s.back().x].visited = TRUE;
	while (!s.empty()) {
		if (s.back().x == target.x && s.back().y == target.y) return true;
		if (s.back().x != width-1 && maze[s.back().y][s.back().x + 1].visited == FALSE && maze[s.back().y][s.back().x].hasRightWall == FALSE) {
			maze[s.back().y][s.back().x + 1].visited = TRUE;
			Coordinate temp = { s.back().y , s.back().x + 1 };
			s.push_back(temp);
		}
		else if (s.back().y != height - 1 && maze[s.back().y + 1][s.back().x].visited == FALSE && maze[s.back().y][s.back().x].hasDownWall == FALSE) {
			maze[s.back().y + 1][s.back().x].visited = TRUE;
			Coordinate temp = { s.back().y + 1, s.back().x };
			s.push_back(temp);
		}
		else if (s.back().x != 0 && maze[s.back().y][s.back().x - 1].visited == FALSE && maze[s.back().y][s.back().x - 1].hasRightWall == FALSE) {
			maze[s.back().y][s.back().x - 1].visited = TRUE;
			Coordinate temp = { s.back().y , s.back().x - 1 };
			s.push_back(temp);
		}
		else if (s.back().y != 0 && maze[s.back().y - 1][s.back().x].visited == FALSE && maze[s.back().y - 1][s.back().x].hasDownWall == FALSE) {
			maze[s.back().y - 1][s.back().x].visited = TRUE;
			Coordinate temp = { s.back().y - 1, s.back().x };
			s.push_back(temp);
		}
		else if (tele[s.back().y][s.back().x].isTele == 1 && maze[tele[s.back().y][s.back().x].gotoY][tele[s.back().y][s.back().x].gotoX].visited == FALSE) {
			maze[tele[s.back().y][s.back().x].gotoY][tele[s.back().y][s.back().x].gotoX].visited = TRUE;
			Coordinate temp = { tele[s.back().y][s.back().x].gotoY, tele[s.back().y][s.back().x].gotoX };
			s.push_back(temp);
		}
		else {
			s.pop_back();
		}
	}
	return false;
}
void ofApp::DFSdraw()
{
	//DFS를 수행한 결과를 그린다.
	int i, j;
	float roomSize = (width > height) ? 700 / (float)width : 700 / (float)height;
	float x_coord = roomSize / 2, y_coord = roomSize / 2;
	for (i = 0; i < height; i++){
		x_coord = roomSize / 2;
		for (j = 0; j < width; j++) {
			if (maze[i][j].visited) {
				if (j + 1 < width && maze[i][j + 1].visited && !maze[i][j].hasRightWall) {
					ofDrawLine(x_coord, y_coord, x_coord + roomSize, y_coord);
				}
				if (i + 1 < height && maze[i + 1][j].visited && !maze[i][j].hasDownWall) {
					ofDrawLine(x_coord, y_coord, x_coord, y_coord + roomSize);
				}
			}
			x_coord += roomSize;
		}
		y_coord += roomSize;
	}

	ofSetColor(255, 0, 0);
	i = s.size() - 1;
	while (i >= 0) {
		int y1, x1, y2, x2;
		y1 = s[i].y;
		x1 = s[i].x;
		i--;
		if (i < 0) break;
		y2 = s[i].y;
		x2 = s[i].x;
		if (!(tele[y1][x1].isTele == 1 && tele[y2][x2].isTele == 1)) {
			x1 = roomSize / 2 + x1 * roomSize;
			y1 = roomSize / 2 + y1 * roomSize;
			x2 = roomSize / 2 + x2 * roomSize;
			y2 = roomSize / 2 + y2 * roomSize;
			ofDrawLine(x1, y1, x2, y2);
		}
	}
}

void ofApp::makeTeleMaze() {

	auto getIntInput = [](const string& prompt, int min_input, int max_input) -> int {

		string input_line;
		int input;

		while (1) {
			cout << prompt;
			getline(cin, input_line);
			if (cin.fail()) break;
			istringstream iss(input_line);
			iss >> input;
			if (iss.fail()) {
				cout << "Input must be an integer." << endl;
			}
			else if (input > max_input || input < min_input) {
				cout << "Input out of range." << endl;
				cout << "Please enter a number between " << min_input << " and " << max_input << "." << endl;
			}
			else break;
		}
		return input;
	};

	height = getIntInput("Enter a height of a maze: ", 1, 50);
	width = getIntInput("Enter a width of a maze: ", 1, 50);

	int i;
	srand(time(NULL));
	initializeMaze();
	for (i = 0; i < height - 1; i++) {
		fillRoomNum(i);
		eraseRightWalls(i);
		eraseDownWalls(i);
		makeTeleCandidates(i);
		activateTele(i);
	}
	fillRoomNum(i);
	eraseLastRightWalls();

}

void ofApp::initializeMaze() {
	int i, j;
	maze = new MazeRoom*[height];
	tele = new TeleRoom*[height];
	for (i = 0; i < height; i++) {
		*(maze + i) = new MazeRoom[width];
		*(tele + i) = new TeleRoom[width];
		for (j = 0; j < width; j++) {
			maze[i][j].roomNum = -1000;
			maze[i][j].visited = 0;
			maze[i][j].hasRightWall = 1;
			maze[i][j].hasDownWall = 1;
			tele[i][j].isTele = 0;
			tele[i][j].teleNum = 0;
			tele[i][j].gotoX = 0;
			tele[i][j].gotoY = 0;
		}
	}
	lastRoomN = 0;
	lastTeleN = ' ';
	return;
}

void ofApp::eraseRightWalls(int rowN) {
	int j, k;
	for (j = 0; j < width - 1; j++) {
		if (maze[rowN][j].roomNum != maze[rowN][j + 1].roomNum) {
			if (rand() % 2 == 0) {
				maze[rowN][j].hasRightWall = 0;
				int compareWith = maze[rowN][j + 1].roomNum;
				for (k = 0; k < width; k++) {
					if (maze[rowN][k].roomNum == compareWith) {
						maze[rowN][k].roomNum = maze[rowN][j].roomNum;
					}
				}

				int selectedSet1, selectedSet2;
				int found1 = 0, found2 = 0;
				for (k = 0; k < candidateSets.size(); k++) {
					if (candidateSets[k].setNum == maze[rowN][j].roomNum) {
						selectedSet1 = k;
						found1 = 1;
					}
					else if (candidateSets[k].setNum == compareWith) {
						selectedSet2 = k;
						found2 = 1;
					}
				}
				if (found1 && found2) {
					candidateSets[selectedSet1].coordinates.reserve(candidateSets[selectedSet1].coordinates.size() + candidateSets[selectedSet2].coordinates.size());
					candidateSets[selectedSet1].coordinates.insert(candidateSets[selectedSet1].coordinates.end(), candidateSets[selectedSet2].coordinates.begin(), candidateSets[selectedSet2].coordinates.end());
					candidateSets.erase(candidateSets.begin() + selectedSet2);
				}
				else if (found2) {
					candidateSets[selectedSet2].setNum = maze[rowN][j].roomNum;
				}
			}
		}
	}
	return;
}

void ofApp::eraseDownWalls(int rowN) {
	int i, j;
	int setN = 0;
	unsigned char findFlag = 0;

	RoomNumSet *headSet = new RoomNumSet;
	headSet->nextSet = NULL;
	RoomNumSet *setPtr;
	Node *indexPtr;

	for (j = 0; j < width; j++) {
		if (rand() % 2 == 0) {
			maze[rowN][j].hasDownWall = 0;
			maze[rowN + 1][j].roomNum = maze[rowN][j].roomNum;
		}
	}
	// 집합 중 최소 하나의 벽은 지워져야함.
	for (j = 0; j < width; j++) {
		findFlag = 0;
		setPtr = headSet;
		while (setPtr->nextSet != NULL) {
			setPtr = setPtr->nextSet;
			if (setPtr->roomNum == maze[rowN][j].roomNum) {
				(setPtr->elementN)++;
				if (maze[rowN][j].hasDownWall == 0) setPtr->connected = 1;
				for (indexPtr = setPtr->index; indexPtr->next != NULL; indexPtr = indexPtr->next);
				indexPtr->next = new Node;
				indexPtr->next->data = j;
				indexPtr->next->next = NULL;
				findFlag = 1;
				break;
			}
		}
		if (findFlag == 0) {
			setPtr->nextSet = new RoomNumSet;
			setPtr->nextSet->nextSet = NULL;
			setPtr->nextSet->index = new Node;

			setPtr->nextSet->roomNum = maze[rowN][j].roomNum;
			setPtr->nextSet->elementN = 1;
			if (maze[rowN][j].hasDownWall == 0) setPtr->nextSet->connected = 1;
			else setPtr->nextSet->connected = 0;
			setPtr->nextSet->index->data = j;
			setPtr->nextSet->index->next = NULL;
		}
	}
	RoomNumSet* preSetPtr = headSet;
	for (setPtr = headSet->nextSet; setPtr != NULL; setPtr = setPtr->nextSet) {
		Node* preIndexPtr;
		if (setPtr->connected == 0) {
			int randIndex = rand() % setPtr->elementN;
			indexPtr = setPtr->index;
			for (i = 0; i < randIndex; i++) {
				indexPtr = indexPtr->next;
			}
			maze[rowN][indexPtr->data].hasDownWall = 0;
			maze[rowN + 1][indexPtr->data].roomNum = maze[rowN][indexPtr->data].roomNum;
			setPtr->connected = 1;
		}
		preIndexPtr = setPtr->index;
		for (indexPtr = setPtr->index->next; indexPtr != NULL; indexPtr = indexPtr->next) {
			free(preIndexPtr);
			preIndexPtr = indexPtr;
		}
		free(preIndexPtr);

		free(preSetPtr);
		preSetPtr = setPtr;
	}
	free(preSetPtr);
	return;
}

void ofApp::makeTeleCandidates(int rowN) {
	int j, k;
	int blocked = 0; //둘러싸인 벽의 수
	int pushed = 0; // coordinate 벡터에 push_back 되었는가?
	for (j = 0; j < width; j++) {
		if (rowN == 0 && j == 0) j++;
		blocked = 0;
		pushed = 0;
		//둘러싸인 벽의 수를 센다.
		if (maze[rowN][j].hasDownWall == 1) {
			blocked++;
		}
		if (maze[rowN][j].hasRightWall == 1) {
			blocked++;
		}
		if (rowN == 0 || maze[rowN - 1][j].hasDownWall == 1) {
			blocked++;
		}
		if (j == 0 || maze[rowN][j - 1].hasRightWall == 1) {
			blocked++;
		}
		//만약 미로의 막다른 길이라면 텔레포트 플랫폼의 후보군에 추가한다.
		if (blocked >= 3) {
			//텔레포트 플랫폼의 set number와 일치하는 set number가 후보군에 있다면 그 set에 좌표를 추가한다.
			for (k = 0; k < candidateSets.size(); k++) {
				if (candidateSets[k].setNum == maze[rowN][j].roomNum) {
					pair<int, int> temp(rowN, j);
					candidateSets[k].coordinates.push_back(temp);
					pair<int, int>().swap(temp);
					pushed = 1;
					break;
				}
			}
			//없다면 새로운 set을 만들고 그 set에 좌표를 추가한다.
			if (!pushed) {
				TeleCandidateSets temp1;
				pair<int, int> temp2(rowN, j);
				temp1.setNum = maze[rowN][j].roomNum;
				temp1.coordinates.push_back(temp2);
				candidateSets.push_back(temp1);
				pair<int, int>().swap(temp2);
			}
		}
	}
}

void ofApp::activateTele(int rowN) {

	int j;
	// 후보군에 충분한 set이 있을 경우 텔레포트 플랫폼 활성화
	if (candidateSets.size() >= 2) {
		//랜덤하게 두 set을 고른 후, 각 set에서 다시 랜덤하게 coordinate를 고른다.
		//이렇게 고른 두 coordinate는 활성화될 텔레포트 플랫폼의 좌표 쌍이다.
		int selectedSet1 = rand() % candidateSets.size();
		int selectedSet2 = rand() % (candidateSets.size() - 1);
		if (selectedSet2 >= selectedSet1) selectedSet2++;

		int selectedCoord1 = rand() % candidateSets[selectedSet1].coordinates.size();
		int selectedCoord2 = rand() % candidateSets[selectedSet2].coordinates.size();

		int y1 = candidateSets[selectedSet1].coordinates[selectedCoord1].first;
		int x1 = candidateSets[selectedSet1].coordinates[selectedCoord1].second;
		int y2 = candidateSets[selectedSet2].coordinates[selectedCoord2].first;
		int x2 = candidateSets[selectedSet2].coordinates[selectedCoord2].second;

		//배열 tele에 확정된 텔레포트 플랫폼의 정보를 담는다.
		
		lastTeleN++;
		tele[y1][x1].isTele = 1;
		tele[y1][x1].teleNum = lastTeleN;
		tele[y1][x1].gotoX = x2;
		tele[y1][x1].gotoY = y2;
		tele[y2][x2].isTele = 1;
		tele[y2][x2].teleNum = lastTeleN;
		tele[y2][x2].gotoX = x1;
		tele[y2][x2].gotoY = y1;
		//텔레포트 플랫폼 확정 후 후보군에서 제거
		candidateSets[selectedSet1].coordinates.erase(candidateSets[selectedSet1].coordinates.begin() + selectedCoord1);
		candidateSets[selectedSet2].coordinates.erase(candidateSets[selectedSet2].coordinates.begin() + selectedCoord2);

		//연결된 두 텔레포트 플랫폼 확정 후
		//1. maze의 room number 갱신 - 두 set의 room number를 같게 만든다.
		int set1Num = candidateSets[selectedSet1].setNum;
		int set2Num = candidateSets[selectedSet2].setNum;
		for (j = 0; j < width; j++) {
			if (maze[rowN][j].roomNum == set2Num) {
				maze[rowN][j].roomNum = set1Num;
			}
			if (maze[rowN + 1][j].roomNum == set2Num) {
				maze[rowN + 1][j].roomNum = set1Num;
			}
		}
		//2. 후보군 갱신 - set이 합쳐짐에 따라 후보군이 속한 set 또한 변경된다.
		candidateSets[selectedSet1].coordinates.reserve(candidateSets[selectedSet1].coordinates.size() + candidateSets[selectedSet2].coordinates.size());
		candidateSets[selectedSet1].coordinates.insert(candidateSets[selectedSet1].coordinates.end(), candidateSets[selectedSet2].coordinates.begin(), candidateSets[selectedSet2].coordinates.end());
		candidateSets.erase(candidateSets.begin() + selectedSet2);
		if (selectedSet2 < selectedSet1) selectedSet1--;
		if (candidateSets[selectedSet1].coordinates.empty()) {
			candidateSets.erase(candidateSets.begin() + selectedSet1);
		}
	}

}

void ofApp::fillRoomNum(int rowN) {
	int j;
	for (j = 0; j < width; j++) {
		if (maze[rowN][j].roomNum < 0) {
			lastRoomN++;
			maze[rowN][j].roomNum = lastRoomN;
		}
	}
	return;
}

void ofApp::eraseLastRightWalls() {
	int j, k;
	for (j = 0; j < width - 1; j++) {
		if (maze[height - 1][j].roomNum != maze[height - 1][j + 1].roomNum) {
			int compareWith = maze[height - 1][j + 1].roomNum;
			maze[height - 1][j].hasRightWall = 0;
			for (k = 0; k < width; k++) {
				if (maze[height - 1][k].roomNum == compareWith) {
					maze[height - 1][k].roomNum = maze[height - 1][j].roomNum;
				}
			}

			int selectedSet1, selectedSet2;
			int found1 = 0, found2 = 0;
			for (k = 0; k < candidateSets.size(); k++) {
				if (candidateSets[k].setNum == maze[height - 1][j].roomNum) {
					selectedSet1 = k;
					found1 = 1;
				}
				else if (candidateSets[k].setNum == compareWith) {
					selectedSet2 = k;
					found2 = 1;
				}
			}
			if (found1 && found2) {
				candidateSets[selectedSet1].coordinates.reserve(candidateSets[selectedSet1].coordinates.size() + candidateSets[selectedSet2].coordinates.size());
				candidateSets[selectedSet1].coordinates.insert(candidateSets[selectedSet1].coordinates.end(), candidateSets[selectedSet2].coordinates.begin(), candidateSets[selectedSet2].coordinates.end());
				candidateSets.erase(candidateSets.begin() + selectedSet2);
			}
		}
	}
	return;
}

void ofApp::printMaze() {

	FILE* fp = fopen("maze.maz", "w");
	int i, j;

	for (j = 0; j < width; j++) {
		fprintf(fp, "+-");
		printf("+-");
	}
	fprintf(fp, "+\n");
	printf("+\n");
	for (i = 0; i < height; i++) {
		fprintf(fp, "|");
		printf("|");
		for (j = 0; j < width; j++) {
			if (tele[i][j].isTele == 1) {
				fprintf(fp, "%c", tele[i][j].teleNum);
				printf("%c", tele[i][j].teleNum);
			}
			else {
				fprintf(fp, " ");
				printf(" ");
			}
			if (maze[i][j].hasRightWall == 1) {
				fprintf(fp, "|");
				printf("|");
			}
			else {
				fprintf(fp, " ");
				printf(" ");
			}
		}
		fprintf(fp, "\n");
		printf("\n");
		for (j = 0; j < width; j++) {
			fprintf(fp, "+");
			printf("+");
			if (maze[i][j].hasDownWall == 1) {
				fprintf(fp, "-");
				printf("-");
			}
			else {
				fprintf(fp, " ");
				printf(" ");
			}
		}
		fprintf(fp, "+\n");
		printf("+\n");
	}
	fclose(fp);
	printf("\n");
	return;
}