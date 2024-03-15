## Teleport Maze Generator
2021년 서강대 컴실1 프로젝트로 만든 미로 생성 프로그램입니다.

Eller's algorithm을 기반으로 한 알고리즘을 통해 teleport platform이 포함된 perfect maze를 생성합니다.

<br />

## How to use
**(실행 파일의 위치 : /teleMazeGenerator/bin/teleMazeGenerator.exe)**

<br />

File -> Make을 통해 미로를 생성할 수 있습니다.

터미널에서 미로의 높이와 너비를 입력하면 화면에 미로가 그려집니다.

<br />

File -> Print and Export를 통해 미로를 출력할 수 있습니다.

미로의 구조를 나타내는 maz.maz 파일이 생성되며, 터미널에서도 텍스트로 출력됩니다.

<br />

View -> Show DFS를 통해 미로를 DFS한 결과를 화면에 그릴 수 있습니다.

<br />

## How to build
1\. OpenFrameworks v0.10 혹은 v0.11을 다운받습니다.

2\. 다운받은 OpenFrameworks의 /apps/myApps 디렉토리에 teleMazeGenerator 폴더를 복사 및 붙여넣기 합니다.

3\. Windows Visual Studio 2017로 프로젝트를 빌드합니다.  
