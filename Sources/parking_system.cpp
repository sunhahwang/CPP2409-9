#include <iostream>
#include <string>
#include <vector>
using namespace std;

// 주차 공간의 종류 정의
const int EMPTY = 0;        // 비어있는 공간  
const int MOTORBIKE = 1;    // 오토바이 공간
const int COMPACT = 2;      // 경차 공간
const int REGULAR = 3;       // 일반차 공간
const int LARGE = 4;        // 대형차 공간
const int ELECTRIC = 5;     // 전기차 공간

// 주차 상태 설정
const int OCCUPIED = 1; // 주차된 상태
const int VACANT = 0; // 비어있는 상태

// 주자창 크기 설정
const int rows = 4; // 주차장의 세로 길이
const int cols = 10; // 주차장의 가로 길이

// 2차원 벡터 선언(주차 상태 표시)
vector<vector<int>> parking;           // 각 칸의 주차 공간 타입 저장
vector<vector<int>> motorbike_count;   // 각 칸에 주차된 오토바이 개수 저장
vector<vector<int>> parking_status;    // 각 칸의 주차 상태 저장

// 주차 공간 타입에 대한 심볼 설정
char parkingSymbol (int status, int type, int motorbike_count = 0) {
    if (type == MOTORBIKE) {
        if (motorbike_count == 2) return 'M'; // 두 대 주차된 경우
        else if (motorbike_count == 1) return 'm'; // 한 대 주차된 경우
    }

    if (status == OCCUPIED) { // 주차된 상태일 경우 대문자로 표시
        switch (type) {
            case COMPACT: return 'C';
            case REGULAR: return 'R';
            case LARGE: return 'L';
            case ELECTRIC: return 'E';
            default: return ' ';
        }
    } else { // 비어 있는 상태일 경우 소문자로 표시
        switch (type) {
            case COMPACT: return 'c';
            case ELECTRIC: return 'e';
            default: return ' ';
        }
    }
}

// 초기 주차 공간 세팅
void setParking() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            parking[i][j] = EMPTY;  // 초기 상태를 빈공간으로
            parking_status[i][j] = VACANT;  // 주차되지 않은 상태로 저장
        }
    }

    // 왼쪽, 오른쪽 끝 자리를 경차 전용자리로
    for (int i = 0; i < rows; ++i) {
        parking[i][0] = COMPACT;
        parking[i][cols - 1] = COMPACT;
    }

    // 마지막 행의 2~5열(4칸)을 전기차 충전자리로
    for (int i = 1; i < 5; ++i) {
        parking[rows - 1][i] = ELECTRIC;
    }
}

// 주차 상태 맵 출력 함수
void displayMap() {
    // 가로 경계선
    int width = cols * 4 + 1;
    string line;
    for (int i = 0; i < width; ++i) {
        line += '-';
    }

    // 각 자리에 대한 주차 자리(상태) 출력
    // '\'를 통해 구분
    for (int i = 0; i < rows; ++i) {
        cout << line << endl;
        for (int j = 0; j < cols; ++j) {
            // 공간에 맞는 symbol가져오기
            // 오토바이의 개수에 따른 symbol가져오기
            char symbol = parkingSymbol(parking_status[i][j], parking[i][j], motorbike_count[i][j]);
            cout << "| " << symbol << " ";
        }
        cout << "|" << endl;
    }
    cout << line << endl;
}

// 차량 타입에 따른 주차 공간 추천 함수
vector<pair<int, int>> recommendSpots(int type, bool charging = false, int max = 5) {
    vector<pair<int, int>> recommend; // 추천되는 주차 공간을 저장할 벡터
    vector<pair<int, int>> compact_only; // 경차 전용 자리 벡터
    vector<pair<int, int>> half_full; // 이미 오토바이가 한 대 있는 공간을 저장할 벡터
    vector<pair<int, int>> empty;     // 비어 있는 공간을 나타내는 벡터

    // 주차 자리를 순차적으로 훑으며 탐색
    // 일정 개수를 채울 경우 탐색 중단 후 반환
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // 오토바이일 경우, 오토바이의 개수(0 or 2)에 따라 추천
            if (type == MOTORBIKE) {
                if (motorbike_count[i][j] == 1 && parking[i][j] == MOTORBIKE) {
                    // 이미 한 대가 주차된 공간인 경우 높은 우선순위를 가진 벡터에 추가
                    half_full.push_back({i, j});
                }
                else if (motorbike_count[i][j] == 0 && parking[i][j] == EMPTY) {
                    // 빈 공간인 경우 낮은 우선순위를 가진 벡터에 추가
                    empty.push_back({i, j});
                }
            }
            // 경차일 경우 우선적으로 경차 전용 자리를 확인
            else if (type == COMPACT) {
                if (parking[i][j] == COMPACT) {
                    compact_only.push_back({i, j});
                }
                else if (parking[i][j] == EMPTY) {
                    empty.push_back({i, j});
                }
            }
            // 전기차일 경우, 충전 여부에 따라 추천
            else if (type == ELECTRIC) {
                if (charging && parking[i][j] == ELECTRIC) {
                    recommend.push_back({i, j});  // 충전이 필요한 경우 전기차 자리 추천
                }
                else if (!charging && parking[i][j] == EMPTY) {
                    recommend.push_back({i, j});  // 충전이 아닌 경우 일반 자리 추천
                }
            }
            // 일반 차량 주차 공간 추천
            else if (type == COMPACT && (parking[i][j] == COMPACT || parking[i][j] == EMPTY)) 
                recommend.push_back({i, j});
            else if (type == LARGE && (j < cols - 1 && (parking[i][j] == EMPTY && parking[i][j + 1] == EMPTY)))
                recommend.push_back({i, j});
            else if (type == REGULAR && parking[i][j] == EMPTY)
                recommend.push_back({i, j});
        }
    }

    // 우선순위에 따라 정렬
    if (type == COMPACT) {
        recommend.insert(recommend.end(), compact_only.begin(), compact_only.end());
        recommend.insert(recommend.end(), empty.begin(), empty.end());
    }
    else if (type == MOTORBIKE) {
        recommend.insert(recommend.end(), half_full.begin(), half_full.end());
        recommend.insert(recommend.end(), empty.begin(), empty.end());
    }

    // 최대 개수를 초과한 경우 조정
    if (recommend.size() > max) {
        recommend.resize(max);
    }
    return recommend;
}

// 차량 주차 함수
void park(int row, int col, int type) { 
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        cout << "주차장을 벗어난 위치입니다." << endl;
        return;
    }

    if (parking_status[row][col] == OCCUPIED) {
        cout << "해당 자리는 이미 점유 중입니다." << endl;
        return;
    }

    if (type == MOTORBIKE) {
        motorbike_count[row][col]++;
        if (motorbike_count[row][col] == 2) {
            parking_status[row][col] = OCCUPIED;
        }
    } else {
        parking_status[row][col] = OCCUPIED; // 해당 자리에 주차
    }

    // 대형차의 경우 두 칸 차지
    if (type == LARGE) {
        parking[row][col] = LARGE;
        parking[row][col + 1] = LARGE;
    }
    // 그외의 경우 그 자리만
    else {
        parking[row][col] = type;
    }
    // 주차 후에는 현재 맵 출력
    displayMap();
} 

int main() {
    // 상태 대입
    // 행의 크기만큼 열을 제작하여 push
    // 비어있는 상태(EMPTY)로 초기화
    for (int i = 0; i < rows; ++i) {
        vector<int> row(cols, EMPTY);   // 주차 공간 타입 초기화
        vector<int> status_row(cols, VACANT); // 주차 상태 초기화
        vector<int> motorbike_row(cols, 0); // 오토바이 개수 초기화
        parking.push_back(row);
        parking_status.push_back(status_row);
        motorbike_count.push_back(motorbike_row);
    }
    setParking();   // 초기 설정
    displayMap();   // 맵 출력

    // mud game처럼 종료신호를 받기 전까지 무한 루프
    while(true){
        // #1. 명령어 입력받기
        string command;
        cout << "차량의 종류나 옵션을 입력해주세요 (motorbike, compact, regular, large, electric, map, exit) : ";
        cin >> command;

        // #2. exit 명령어가 들어온 경우
        if (command == "exit") 
            break;  // 프로그램 종료
        // #3. map 명령어가 들어온 경우
        else if (command == "map") {
            displayMap(); // 현재 상태의 map 출력
            continue;   // 다음 명령어 받기
        }

        // #4. 전기차의 충전 선택 확인
        bool charging = false;  // 기본 상태는 충전X
        char answer;
        if (command == "electric") {
            cout << "충전을 원하십니까? (y/n) : ";
            cin >> answer;
            if (answer == 'y') {
                charging = true;    // 충전을 원할 경우 TRUE
            }
        }

        // #5. 차량 타입에 따른 주차 공간 추천
        vector<pair<int, int>> recommendedSpots;    // 추천된 주차 공간(주소)를 저장할 벡터

        // 각 차량 타입을 확인하고 이에 따라 자리 추천
        if (command == "motorbike") {
            recommendedSpots = recommendSpots(MOTORBIKE);
        }
        else if (command == "compact") {
            recommendedSpots = recommendSpots(COMPACT);
        }
        else if (command == "regular") {
            recommendedSpots = recommendSpots(REGULAR);
        }
        else if (command == "large") {
            recommendedSpots = recommendSpots(LARGE);
        }
        // 충전이 필요한 전기차의 경우
        else if (command == "electric" && charging) {
            recommendedSpots = recommendSpots(ELECTRIC, true);
        }
        // 충전이 필요없는 전기차의 경우
        else if (command == "electric") {
            recommendedSpots = recommendSpots(ELECTRIC);
        }

        // #6. 추천된 자리 출력 및 주차
        if (recommendedSpots.empty() == 0) {
            cout << "추천 주차 자리 : ";
            for (auto spot : recommendedSpots) {
                cout << "(" << spot.first << ", " << spot.second << ") ";
            }
            cout << endl;

            // 자리 선택
            int selectR, selectC;
            cout << "주차할 자리를 선택하세요 (행, 열) : ";
            cin >> selectR >> selectC;

            // 선택된 자리가 칸을 넘어가지 않는 지 확인 후 추자 처리
            // 각 차량별로 맞는 Symbol사용
            if (selectR >= 0 && selectR <= rows && selectC >= 0 && selectC <= cols) {
                // 차량의 종류에 맞게 주차 처리
                if (command == "motorbike") {
                    park(selectR, selectC, MOTORBIKE); 
                }
                else if (command == "compact") {
                    park(selectR, selectC, COMPACT);
                }
                else if (command == "regular") {
                    park(selectR, selectC, REGULAR);
                }
                else if (command == "large") {
                    park(selectR, selectC, LARGE);
                }
                else if (command == "electric") {
                    park(selectR, selectC, ELECTRIC);
                }
            }
            else {
                cout << "추천할 자리가 없습니다." << endl;
            }
        }
    }
}