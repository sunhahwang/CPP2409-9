/*
사용자에게 입력받을 수 있는 명령어
: motorbike, compact, regular, large, electric, map, exit
필요한 함수
: map 출력 함수
: 주차자리 추천 및 좌표 반환 함수
*/
#include <iostream>
#include <string>
using namespace std;

// ##1. 현재 맵을 출력하는 함수
void displayMap() {

}

// ##2. 모든 칸이 채워졌는지 or 해당 차량의 자리가 있는지 체크하는 함수
bool emptyCheck() {
    
}

int main() {
    // mud game처럼 종료신호를 받기 전까지 무한 루프
    while(true){
        // #1. 명령어 입력받기
        string command;
        cout << "차량의 종류나 옵션을 입력해주세요 (motorbike, compact, regular, large, electric, map, exit) : ";
        cin >> command;

        // #2. exit 명령어가 들어온 경우
        if (command == "exit") 
            break;
        // #3. map 명령어가 들어온 경우
        else if (command == "map") {
            displayMap(); // 현재 상태의 map 출력
            continue;
        }

        // #4. 전기차의 충전 선택 확인
        bool charging = false;
        char answer;
        if (command == "electric") {
            cout << "충전을 원하십니까? (y/n) : ";
            cin >> answer;
            if (answer == 'y') {
                charging = true;
            }
        }

        // #5. 모든 자리가 찬 경우 or 해당 차의 자리가 존재하지않는 경우
         if (emptyCheck() == true) {
            cout << "추천할 수 있는 자리가 없습니다.";
            continue;
         }
    }
}