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
const int ROWS = 4; // 주차장의 세로 길이
const int COLS = 10; // 주차장의 가로 길이

// 2차원 벡터 선언(주차 상태 표시)
vector<vector<int>> parking;           // 각 칸의 주차 공간 타입 저장
vector<vector<int>> motorbike_count;   // 각 칸에 주차된 오토바이 개수 저장
vector<vector<int>> parking_status;    // 각 칸의 주차 상태 저장

// 주차 공간 타입에 대한 심볼 설정
char ParkingSymbol (int status, int type, int motorbike_count = 0) {
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
    }
    else { // 비어 있는 상태일 경우 소문자로 표시
        switch (type) {
            case COMPACT: return 'c';
            case ELECTRIC: return 'e';
            default: return ' ';
        }
    }
}

// 초기 주차 공간 세팅
void SetParking() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            parking[i][j] = EMPTY;  // 초기 상태를 빈공간으로
            parking_status[i][j] = VACANT;  // 주차되지 않은 상태로 저장
        }
    }

    // 왼쪽, 오른쪽 끝 자리를 경차 전용자리로
    for (int i = 0; i < ROWS; ++i) {
        parking[i][0] = COMPACT;
        parking[i][COLS - 1] = COMPACT;
    }

    // 마지막 행의 2~5열(4칸)을 전기차 충전자리로
    for (int i = 1; i < 5; ++i) {
        parking[ROWS - 1][i] = ELECTRIC;
    }
}

// 주차 상태 맵 출력 함수
void DisplayMap() {
    // 가로 경계선
    int width = COLS * 4 + 1;
    string line;
    for (int i = 0; i < width; ++i) {
        line += '-';
    }

    // 각 자리에 대한 주차 자리(상태) 출력
    // '\'를 통해 구분
    for (int i = 0; i < ROWS; ++i) {
        cout << line << endl;
        for (int j = 0; j < COLS; ++j) {
            // 공간에 맞는 symbol가져오기
            // 오토바이의 개수에 따른 symbol가져오기
            char symbol = ParkingSymbol(parking_status[i][j], parking[i][j], motorbike_count[i][j]);
            cout << "| " << symbol << " ";
        }
        cout << "|" << endl;
    }
    cout << line << endl;
}

// 주차 자리의 양 옆 각각 두 자리씩 확인하는 함수
bool CheckSideEmpty(int row, int col) {
    // 왼쪽에 두 자리 확인
    if (col > 1) { // 맵을 벗어난 판단을 막기 위한 조건
        if (parking_status[row][col - 1] == VACANT && 
            parking_status[row][col - 2] == VACANT &&
            parking[row][col - 1] != COMPACT && 
            parking[row][col - 2] != COMPACT && 
            parking[row][col - 1] != ELECTRIC && 
            parking[row][col - 2] != ELECTRIC &&
            parking[row][col - 1] != MOTORBIKE &&
            parking[row][col - 2] != MOTORBIKE) {
            return true;
        }
    }

    // 오른쪽에 두 자리 확인
    if (col < COLS - 2) { // 맵을 벗어난 판단을 막기 위한 조건
        if (parking_status[row][col + 1] == VACANT && 
            parking_status[row][col + 2] == VACANT &&
            parking[row][col + 1] != COMPACT && 
            parking[row][col + 2] != COMPACT && 
            parking[row][col + 1] != ELECTRIC && 
            parking[row][col + 2] != ELECTRIC &&
            parking[row][col + 1] != MOTORBIKE &&
            parking[row][col + 2] != MOTORBIKE) {
            return true; 
        }
    }

    // 양 옆 모두 비어있지 않거나 판단 시 맵을 벗어나는 경우 false
    return false;
}

// 주차 자리의 양 옆 각각 한 자리씩 확인하는 함수
bool CheckSideFull(int row, int col) {
    bool left_check = false; // 왼쪽 자리의 채워짐 여부

    // 왼쪽 한 자리 확인 
    if (col > 0) { // 맵을 벗어난 판단을 막기 위한 조건
        if (parking_status[row][col - 1] == OCCUPIED ||
            parking[row][col - 1] == COMPACT ||
            parking[row][col - 1] == ELECTRIC ||
            parking[row][col - 1] == MOTORBIKE) {
            left_check = true;
        }
    }

    // 오른쪽 한 자리 확인
    if (col < COLS - 1 && left_check) { // 맵을 벗어난 판단을 막고 왼쪽이 채워졌을 때
        if (parking_status[row][col + 1] == OCCUPIED ||
            parking[row][col + 1] == COMPACT ||
            parking[row][col + 1] == ELECTRIC ||
            parking[row][col + 1] == MOTORBIKE) {
            return true;
        }
    }

    // 두 자리 모두 비워져있거나 한 자리만 채워진 경우 false
    return false;
}

// 차량 타입에 따른 주차 공간 추천 함수
vector<pair<int, int>> recommend_spots(int type, bool charging = false, int max = 5) {
    vector<pair<int, int>> recommend;    // 추천되는 주차 공간을 저장할 벡터
    vector<pair<int, int>> compact_only; // 경차 전용 자리 벡터
    vector<pair<int, int>> half_full;    // 이미 오토바이가 한 대 있는 공간을 저장할 벡터
    vector<pair<int, int>> empty;        // 비어 있는 공간을 나타내는 벡터
    vector<pair<int, int>> for_large;    // 대형 차랑의 최대 자리를 위한 추천 자리 벡터
    vector<pair<int, int>> isolated;     // (양 옆이 채워진) 고립된 자리 벡터

    // 우선순위: recommend(0) - compact_only, half_full(1) - isolated(2)- for_large(3) - empty(4)

    // 주차 자리를 순차적으로 훑으며 탐색
    // 일정 개수를 채울 경우 탐색 중단 후 반환
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            // 점유된 자리는 무시
            if (parking_status[i][j] == OCCUPIED)
                continue;

            // 오토바이일 경우
            if (type == MOTORBIKE) {
                if (motorbike_count[i][j] == 1 && parking[i][j] == MOTORBIKE) {
                    // 이미 한 대가 주차된 공간인 경우 1번 우선순위를 가진 벡터에 추가
                    half_full.push_back({i, j});
                }
                else if (CheckSideFull(i, j) && parking[i][j] == EMPTY) {
                    // 고립된 자리인 경우 2번 우선순위를 가진 벡터에 추가
                    isolated.push_back({i, j});
                }
                else if (CheckSideEmpty(i, j) && parking[i][j] == EMPTY) {
                    // 양 옆 중 하나라도 두 자리씩 빈 경우 3번 우선순위를 가진 벡터에 추가
                    for_large.push_back({i, j});
                }
                else if (motorbike_count[i][j] == 0 && parking[i][j] == EMPTY) {
                    // 빈 공간인 경우 4번 우선순위를 가진 벡터에 추가
                    empty.push_back({i, j});
                }
            }

            // 경차일 경우
            else if (type == COMPACT) {
                if (parking[i][j] == COMPACT) {
                    // 경차 전용 자리인 경우 1번 우선순위를 가진 벡터에 추가
                    compact_only.push_back({i, j});
                }
                else if (CheckSideFull(i, j) && parking[i][j] == EMPTY) {
                    // 고립된 자리인 경우 2번 우선순위를 가진 벡터에 추가
                    isolated.push_back({i, j});
                }
                else if (CheckSideEmpty(i, j) && parking[i][j] == EMPTY) {
                    // 양 옆 중 하나라도 두 자리씩 빈 경우 3번 우선순위를 가진 벡터에 추가
                    for_large.push_back({i, j});
                }
                else if (parking[i][j] == EMPTY) {
                    // 빈 공간인 경우 4번 우선순위를 가진 벡터에 추가
                    empty.push_back({i, j});
                }
            }

            // 전기차일 경우
            else if (type == ELECTRIC) {
                if (charging && parking[i][j] == ELECTRIC) {
                    // 전기차 충전을 원하는 경우 0번 우선순위를 가진 벡터에 추가
                    recommend.push_back({i, j});  
                }
                else if (!charging && (CheckSideFull(i, j) && parking[i][j] == EMPTY)) {
                    // 충전을 원하지 않고 고립된 자리인 경우 2번 우선순위를 가진 벡터에 추가
                    isolated.push_back({i, j});
                }
                else if (!charging && (CheckSideEmpty(i, j) && parking[i][j] == EMPTY)) {
                    // 충전을 원하지 않고 양 옆 중 하나라도 두 자리씩 빈 경우 3번 우선순위를 가진 벡터에 추가
                    for_large.push_back({i, j}); 
                }
                else if (!charging && parking[i][j] == EMPTY) {
                    // 충전을 원하지 않고 빈 공간인 경우 4번 우선순위를 가진 벡터에 추가
                    empty.push_back({i, j}); 
                }
            }

            // 대형차일 경우
            else if (type == LARGE) {
                if (j < COLS - 1 && (parking[i][j] == EMPTY && parking[i][j + 1] == EMPTY))
                    // 두 자리 주차를 위해 0번 우선순위를 가진 벡터에 추가
                    recommend.push_back({i, j});
            }

            // 일반차일 경우
            else if (type == REGULAR) {
                if (CheckSideFull(i, j) && parking[i][j] == EMPTY) {
                    // 고립된 자리인 경우 2번 우선순위를 가진 벡터에 추가
                    isolated.push_back({i, j});
                }
                else if (CheckSideEmpty(i, j) && parking[i][j] == EMPTY) {
                    // 양 옆 중 하나라도 두 자리씩 빈 경우 3번 우선순위를 가진 벡터에 추가
                    for_large.push_back({i, j});
                }
                else if (parking[i][j] == EMPTY) {
                    // 빈 공간인 경우 4번 우선순위를 가진 벡터에 추가
                    empty.push_back({i, j});
                }
            }
        }
    }

    // 우선순위에 따라 정렬
    if (type == MOTORBIKE) {
        recommend.insert(recommend.end(), half_full.begin(), half_full.end());
        recommend.insert(recommend.end(), isolated.begin(), isolated.end());
        recommend.insert(recommend.end(), for_large.begin(), for_large.end());
        recommend.insert(recommend.end(), empty.begin(), empty.end());
    }

    else if (type == COMPACT) {
        recommend.insert(recommend.end(), compact_only.begin(), compact_only.end());
        recommend.insert(recommend.end(), isolated.begin(), isolated.end());
        recommend.insert(recommend.end(), for_large.begin(), for_large.end());
        recommend.insert(recommend.end(), empty.begin(), empty.end());
    }

    else if (type == ELECTRIC) {
        recommend.insert(recommend.end(), isolated.begin(), isolated.end());
        recommend.insert(recommend.end(), for_large.begin(), for_large.end());
        recommend.insert(recommend.end(), empty.begin(), empty.end());
    }

    else if (type == REGULAR) {
        recommend.insert(recommend.end(), isolated.begin(), isolated.end());
        recommend.insert(recommend.end(), for_large.begin(), for_large.end());
        recommend.insert(recommend.end(), empty.begin(), empty.end());
    }

    // 최대 개수를 초과한 경우 조정
    if (recommend.size() > max) {
        recommend.resize(max);
    }
    return recommend;
}

// 경고문 출력 함수
bool Warning(int row, int col, int type) {
    // 입력 범위 확인
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        cout << "주차장을 벗어난 위치입니다." << endl;
        return true; // 잘못된 범위
    }

    // 이미 점유된 자리인지 확인
    if (parking_status[row][col] == OCCUPIED) {
        cout << "해당 자리는 이미 점유 중입니다." << endl;
        return true; // 이미 점유된 자리
    }

    // 전용 주차 공간 확인 및 경고문 출력
    if (parking[row][col] == COMPACT && type != COMPACT) {
        cout << "이 자리는 경차 전용 자리입니다. 다른 차량은 주차할 수 없습니다." << endl;
        return true; // 경차가 아닌 차량의 전용 자리 주차 방지
    }
    if (parking[row][col] == ELECTRIC && type != ELECTRIC) {
        cout << "이 자리는 전기차 충전 전용 자리입니다. 다른 차량은 주차할 수 없습니다." << endl;
        return true; // 전기차가 아닌 차량의 전용 자리 주차 방지
    }

    return false;
}

// 차량 주차 함수
void Park(int row, int col, int type, bool warning) { 
    // 경고문이 출력되었을 경우 주차하지 않고 return
    if (warning) {
        return;
    }

    // 대형 차량일 경우 두 칸 확인 및 처리
    if (type == LARGE) {
        if (col < COLS - 1 && parking_status[row][col] == VACANT && parking_status[row][col + 1] == VACANT) {
            // 채워짐 처리 후
            parking_status[row][col] = OCCUPIED;
            parking_status[row][col + 1] = OCCUPIED;
            // 주차
            parking[row][col] = LARGE;
            parking[row][col + 1] = LARGE;
        }
    }
    // 오토바이 주차 처리
    else if (type == MOTORBIKE) {
        motorbike_count[row][col]++; // 채워진 오토바이 수 증가
        if (motorbike_count[row][col] == 2) {
            // 채워짐 처리 후
            parking_status[row][col] = OCCUPIED;
            // 주차
            parking[row][col] = MOTORBIKE;
        }
        // 오토바이 한 대 주차
        parking[row][col] = MOTORBIKE;
    } 
    else {
        // 다른 차량 주차 처리
        parking_status[row][col] = OCCUPIED;
        parking[row][col] = type;
    }

    DisplayMap(); // 주차 후 맵 출력
}

// 주차장이 포화 상태인지 확인하는 함수
bool IsParkingFull() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (parking_status[i][j] == VACANT) {
                return false; // 한 칸이라도 빈칸이 있으면 포화 false
            }
        }
    }
    return true; // 모든 칸이 주차 완료 상태
}

int main() {
    // 상태 대입
    // 행의 크기만큼 열을 제작하여 push
    // 비어있는 상태(EMPTY)로 초기화
    for (int i = 0; i < ROWS; ++i) {
        vector<int> row(COLS, EMPTY);   // 주차 공간 타입 초기화
        vector<int> status_row(COLS, VACANT); // 주차 상태 초기화
        vector<int> motorbike_row(COLS, 0); // 오토바이 개수 초기화
        parking.push_back(row);
        parking_status.push_back(status_row);
        motorbike_count.push_back(motorbike_row);
    }
    SetParking();   // 초기 설정
    DisplayMap();   // 맵 출력

    // mud game처럼 종료신호를 받기 전까지 무한 루프
    while(true) {

        // #0. 주차장의 포화상태 확인하기
        if (IsParkingFull()) {
            cout << "주차장의 모든 칸이 포화 상태입니다. 시스템을 종료합니다." << endl;
            break;
        } 

        // #1. 명령어 입력받기
        string command;
        cout << "차량의 종류나 옵션을 입력해주세요 (motorbike, compact, regular, large, electric, map, exit) : ";
        cin >> command;

        // #2. exit 명령어가 들어온 경우
        if (command == "exit") 
            break;  // 프로그램 종료

        // #3. map 명령어가 들어온 경우
        else if (command == "map") {
            DisplayMap(); // 현재 상태의 map 출력
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
        vector<pair<int, int>> recommended_spots;    // 추천된 주차 공간(주소)를 저장할 벡터

        // 각 차량 타입을 확인하고 이에 따라 자리 추천
        if (command == "motorbike") {
            recommended_spots = recommend_spots(MOTORBIKE);
        }
        else if (command == "compact") {
            recommended_spots = recommend_spots(COMPACT);
        }
        else if (command == "regular") {
            recommended_spots = recommend_spots(REGULAR);
        }
        else if (command == "large") {
            recommended_spots = recommend_spots(LARGE);
        }
        // 충전이 필요한 전기차의 경우
        else if (command == "electric" && charging) {
            recommended_spots = recommend_spots(ELECTRIC, true);
        }
        // 충전이 필요없는 전기차의 경우
        else if (command == "electric") {
            recommended_spots = recommend_spots(ELECTRIC);
        }
        // 잘못된 입력의 경우
        else {
            cout << "잘못된 입력입니다." << endl;
            continue;
        }

        // #6. 추천된 자리 출력 및 주차
        if (recommended_spots.empty()) {
            cout << "해당 차량의 추천 자리가 없습니다." << endl;
            continue;
        }

        cout << "추천 주차 자리 : ";
        for (auto spot : recommended_spots) {
            cout << "(" << spot.first << ", " << spot.second << ") ";
        }
        cout << endl;

        // 자리 선택
        int select_r, select_c;
        cout << "주차할 자리를 선택하세요 (행, 열) : ";
        cin >> select_r >> select_c;

        // 추천 자리를 선택했는지 확인
        bool is_recommended_spot = false;
        for (auto spot : recommended_spots) {
            if (spot.first == select_r && spot.second == select_c) {
                is_recommended_spot = true;
                break;
            }
        }

        // 경고문 출력 여부 확인
        bool warning = false;

        // 차량의 종류에 맞게 주차 처리
        if (command == "motorbike") {
            // (해당될 경우) 경고문 출력 후 여부를 true로
            warning = Warning(select_r, select_c, MOTORBIKE);
            Park(select_r, select_c, MOTORBIKE, warning); 
        }
        else if (command == "compact") {
            // (해당될 경우) 경고문 출력 후 여부를 true로
            warning = Warning(select_r, select_c, COMPACT);
            Park(select_r, select_c, COMPACT, warning);
        }
        else if (command == "regular") {
            // (해당될 경우) 경고문 출력 후 여부를 true로
            warning = Warning(select_r, select_c, REGULAR);
            Park(select_r, select_c, REGULAR, warning);
        }
        else if (command == "large") {
            // (해당될 경우) 경고문 출력 후 여부를 true로
            warning = Warning(select_r, select_c, LARGE);
            Park(select_r, select_c, LARGE, warning);
        }
        else if (command == "electric") {
            // (해당될 경우) 경고문 출력 후 여부를 true로
            warning = Warning(select_r, select_c, ELECTRIC);
            Park(select_r, select_c, ELECTRIC, warning);
        }

        // 추천 자리를 선택하지 않은 경우 경고문 출력 여부를 확인하고 안내문 출력
        if (!is_recommended_spot && !warning) {
            cout << "추천 자리가 아닙니다." << endl;
        }
    }
}