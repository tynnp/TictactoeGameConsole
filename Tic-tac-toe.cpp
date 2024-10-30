#include <iostream>
#include <algorithm>
#include <map>
#include <ctime>
#include <cstdlib>
#include <windows.h>

#define GRAY 7
#define BLUE 9
#define RED 12
#define YELLOW 14

using namespace std;

class Board {
private:
    char grid[3][3];                        // ma trận chứa bàn cờ
    map<string, pair<int, int>> coordinate; // lưu tọa độ của bàn cờ 
public:
    Board();                                // Khởi tạo bàn cờ
    void display();                         // Hiển thị bàn cờ liên màn hình
    bool full();                            // Kiểm tra xem bàn cờ đã được đánh hết chưa
    bool placeMove(string, char);           // Đặt một nước đi trên bàn cờ
    bool checkWin(char);                    // Kiểm tra xem đã thắng hay chưa
    char getGridValue(int, int);            // Lấy giá trị tương ứng với tọa độ
    pair<int, int> getCoor(string);         
};

class Player {
protected:
    char symbol;
public:
    Player();
    Player(char);                           // Khởi tạo người chơi với ký tự của người chơi đó
    virtual bool makeMove(Board&) = 0;      // Thực hiện nước đi trên bàn cờ
    void setSymbol(char);                   
    char getSymbol();
};

class HumanPlayer : public Player {
public:
    HumanPlayer();
    HumanPlayer(char);                      // Khởi tạo người chơi thực với ký tự của người chơi đó
    bool makeMove(Board&);                  // Thực hiện nước đi trên bàn cờ đối với người chơi thực
};

class ComputerPlayer : public Player {
private:
    int level;                              // level dễ hoặc khó (1 hoặc 2)
public:
    ComputerPlayer();
    ComputerPlayer(char, int);              // Khởi tạo player là máy tính với ký tự của máy và độ khó
    bool easyLevel(Board&);                 // Nước đi đối với mức độ dễ
    bool difficultLevel(Board&);            // Nước đi đối với mức độ khó
    bool makeMove(Board&);                  // Thực hiện nước đi trên bàn cờ đối với máy tính
    void setLevel(int);
    int getLevel();
};

class Game {
private:
    Board board;
    Player *player1;
    Player *player2;
    int sleepTime;                          // Giá trị độ chậm của màn hình khi hiển thị
public:
    Game();
    ~Game();
    Game(Player*, Player*);                 // Khởi tạo một game từ hai player
    void run();                             // Bắt chương trình
    void start();                           // Bắt đầu chơi
    void setSleepTime(int);                 // Cài đặt thời gian hiển thị chậm
    int getSleepTime();
};

// HÀM MAIN CHƯƠNG TRÌNH //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    Game game;
    game.start();
    return 0;
}

// CLASS BOARD //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Tạo bảng trò chơi và cập nhật tọa độ 
Board::Board() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[i][j] = ' ';

    coordinate["a1"] = {0 , 0}; coordinate["a2"] = {0 , 1}; coordinate["a3"] = {0 , 2};
    coordinate["b1"] = {1 , 0}; coordinate["b2"] = {1 , 1}; coordinate["b3"] = {1 , 2};
    coordinate["c1"] = {2 , 0}; coordinate["c2"] = {2 , 1}; coordinate["c3"] = {2 , 2};
}

// Kiểm tra bảng đã được đánh hết hay chưa
bool Board::full() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i][j] == ' ') return false;
    return true;
}

// Hiển thị bàn cờ
void Board::display() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "    1   2   3" << endl;
    cout << "  +---+---+---+" << endl;

    for (int i = 0; i < 3; i++) {
        cout << char('a' + i) << " | ";

        // Nếu là X thì màu đỏ, O thì màu xanh
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == 'X') SetConsoleTextAttribute(hConsole, RED);
            else if (grid[i][j] == 'O') SetConsoleTextAttribute(hConsole, BLUE);
            else SetConsoleTextAttribute(hConsole, GRAY);

            cout << grid[i][j];
            SetConsoleTextAttribute(hConsole, GRAY);
            cout << " | ";
        }

        cout << endl;
        cout << "  +---+---+---+" << endl;
    }
}

// Đặt một nước đi trên bàn cờ
bool Board::placeMove(string coord, char symbol) {
    int x = coordinate[coord].first;
    int y = coordinate[coord].second;
    if (x < 0 || x > 2 || y < 0 || y > 2 || grid[x][y] != ' ') return false;
    grid[x][y] = symbol;
    return true;
}

// Kiểm tra xem có người chơi nào thắng hay chưa
bool Board::checkWin(char symbol) {

    // Kiểm tra theo đường dọc và ngang
    for (int i = 0; i < 3; i++) {
        if (grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol) return true;
        if (grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol) return true;
    }

    // Kiểm tra hai đường chéo
    if (grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol) return true;
    if (grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol) return true;

    return false;
}

// Lấy ra cặp tọa độ tương ứng với chuỗi người dùng nhập vào
pair<int, int> Board::getCoor(string coord) {
    auto it = coordinate.find(coord);
    if (it != coordinate.end()) return coordinate[coord];
    else return {-1, -1};
} 

// Lấy ra ký tự dựa trên tọa độ được lấy từ chuỗi
char Board::getGridValue(int i, int j) {
    return grid[i][j];
}

// CLASS TRỪU TƯỢNG PLAYER //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Player::Player() {}
Player::Player(char symbol) : symbol(symbol) {}

void Player::setSymbol(char symbol) {
    this->symbol = symbol;
}

char Player::getSymbol() {
    return symbol;
}

// CLASS HUMANPLAYER //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HumanPlayer::HumanPlayer() {}
HumanPlayer::HumanPlayer(char symbol) : Player(symbol) {}

// Thực hiện một nước đi
bool HumanPlayer::makeMove(Board &board) {
    string coord;
    cout << "[+] Nhap toa do: ";
    getline(cin, coord);

    // xóa khoảng trắng dư
    coord.erase(remove(coord.begin(), coord.end(), ' '), coord.end());

    // nếu nhập <số><chữ> thì đảo ngược tọa độ lại là <chữ><số>
    if (isdigit(coord[0])) reverse(coord.begin(), coord.end());
    for (char &c : coord) c = tolower(c);

    int x = board.getCoor(coord).first;
    int y = board.getCoor(coord).second;

    // Nếu tọa độ vượt khỏi tọa độ bàn cờ thì nén ngoại lệ với thông điệp "không hợp lệ"
    if (x < 0 || x > 2 || y < 0 || y > 2)
        throw invalid_argument("[!] Toa do khong hop le!");

    // Nếu không đặt nước đi tại vị trí đó thì nén ngoại lệ với thông điệp "ô đã được đánh dấu"
    if (!board.placeMove(coord, symbol)) 
        throw invalid_argument("[!] O nay da duoc danh dau!");

    return true;
}

// CLASS COMPUTERPLAYER //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ComputerPlayer::ComputerPlayer() {}
ComputerPlayer::ComputerPlayer(char symbol, int level) : Player(symbol), level(level) {}

void ComputerPlayer::setLevel(int level) {
    this->level = level;
}

int ComputerPlayer::getLevel() {
    return level;
}

// Phương thức máy chơi với mức độ dễ, cho random 
bool ComputerPlayer::easyLevel(Board &board) {
    srand(time(nullptr));

    while (true) {
        int x = rand() % 3;
        int y = rand() % 3;

        if (board.getGridValue(x, y) == ' ') {
            string coord = "";
            coord += ('a' + x);
            coord += ('1' + y);
            
            if (board.placeMove(coord, symbol)) 
                return true;
        }
    }

    return false;
}

// Phương thức máy chơi với mức độ khó
bool ComputerPlayer::difficultLevel(Board &board) {
    int tmpX, tmpY;
    bool isValid;
    string coord = "";
    char opponentSymbol = symbol == 'X' ? 'O' : 'X';

// Kiểm tra đường đi của máy xem đã có đường thắng hay chưa, nếu có thì đánh ô còn lại để thắng
    for (int i = 0; i < 3; i++) {

        // Kiểm tra theo hàng
        if ((board.getGridValue(i, 0) == symbol) + (board.getGridValue(i, 1) == symbol) + (board.getGridValue(i, 2) == symbol) == 2) {
            coord = "";
            isValid = false;
            if (board.getGridValue(i, 0) == ' ') tmpX = i, tmpY = 0, isValid = true;
            if (board.getGridValue(i, 1) == ' ') tmpX = i, tmpY = 1, isValid = true;
            if (board.getGridValue(i, 2) == ' ') tmpX = i, tmpY = 2, isValid = true;
            coord += ('a' + tmpX);
            coord += ('1' + tmpY);
            if (isValid && board.placeMove(coord, symbol)) return true;
        }

        // Kiểm tra theo cột
        if ((board.getGridValue(0, i) == symbol) + (board.getGridValue(1, i) == symbol) + (board.getGridValue(2, i) == symbol) == 2) {
            coord = "";
            isValid = false;
            if (board.getGridValue(0, i) == ' ') tmpX = 0, tmpY = i, isValid = true;
            if (board.getGridValue(1, i) == ' ') tmpX = 1, tmpY = i, isValid = true;
            if (board.getGridValue(2, i) == ' ') tmpX = 2, tmpY = i, isValid = true;
            coord += ('a' + tmpX);
            coord += ('1' + tmpY);
            if (isValid && board.placeMove(coord, symbol)) return true;
        }
    }
    
    // Kiểm tra theo đường chéo chính
    if ((board.getGridValue(0, 0) == symbol) + (board.getGridValue(1, 1) == symbol) + (board.getGridValue(2, 2) == symbol) == 2) {
        coord = "";
        isValid = false;
        if (board.getGridValue(0, 0) == ' ') tmpX = 0, tmpY = 0, isValid = true;
        if (board.getGridValue(1, 1) == ' ') tmpX = 1, tmpY = 1, isValid = true;
        if (board.getGridValue(2, 2) == ' ') tmpX = 2, tmpY = 2, isValid = true;
        coord += ('a' + tmpX);
        coord += ('1' + tmpY);
        if (isValid && board.placeMove(coord, symbol)) return true;
    }

    // Kiểm tra theo đường chéo phụ
    if ((board.getGridValue(0, 2) == symbol) + (board.getGridValue(1, 1) == symbol) + (board.getGridValue(2, 0) == symbol) == 2) {
        coord = "";
        isValid = false;
        if (board.getGridValue(0, 2) == ' ') tmpX = 0, tmpY = 2, isValid = true;
        if (board.getGridValue(1, 1) == ' ') tmpX = 1, tmpY = 1, isValid = true;
        if (board.getGridValue(2, 0) == ' ') tmpX = 2, tmpY = 0, isValid = true;
        coord += ('a' + tmpX);
        coord += ('1' + tmpY);
        if (isValid && board.placeMove(coord, symbol)) return true;
    }

// Nếu chưa có đường thắng thì kiểm tra nước đi của người chơi, nếu có đường thắng thì chặn
    for (int i = 0; i < 3; i++) {

        // Kiểm tra theo hàng
        if ((board.getGridValue(i, 0) == opponentSymbol) + (board.getGridValue(i, 1) == opponentSymbol) + (board.getGridValue(i, 2) == opponentSymbol) == 2) {
            coord = "";
            isValid = false;
            if (board.getGridValue(i, 0) == ' ') tmpX = i, tmpY = 0, isValid = true;
            if (board.getGridValue(i, 1) == ' ') tmpX = i, tmpY = 1, isValid = true;
            if (board.getGridValue(i, 2) == ' ') tmpX = i, tmpY = 2, isValid = true;
            coord += ('a' + tmpX);
            coord += ('1' + tmpY);
            if (isValid && board.placeMove(coord, symbol)) return true;
        }

        // Kiểm tra theo cột
        if ((board.getGridValue(0, i) == opponentSymbol) + (board.getGridValue(1, i) == opponentSymbol) + (board.getGridValue(2, i) == opponentSymbol) == 2) {
            coord = "";
            isValid = false;
            if (board.getGridValue(0, i) == ' ') tmpX = 0, tmpY = i, isValid = true;
            if (board.getGridValue(1, i) == ' ') tmpX = 1, tmpY = i, isValid = true;
            if (board.getGridValue(2, i) == ' ') tmpX = 2, tmpY = i, isValid = true;
            coord += ('a' + tmpX);
            coord += ('1' + tmpY);
            if (isValid && board.placeMove(coord, symbol)) return true;
        }
    }
    
    // Kiểm tra theo đường chéo chính
    if ((board.getGridValue(0, 0) == opponentSymbol) + (board.getGridValue(1, 1) == opponentSymbol) + (board.getGridValue(2, 2) == opponentSymbol) == 2) {
        coord = "";
        isValid = false;
        if (board.getGridValue(0, 0) == ' ') tmpX = 0, tmpY = 0, isValid = true;
        if (board.getGridValue(1, 1) == ' ') tmpX = 1, tmpY = 1, isValid = true;
        if (board.getGridValue(2, 2) == ' ') tmpX = 2, tmpY = 2, isValid = true;
        coord += ('a' + tmpX);
        coord += ('1' + tmpY);
        if (isValid && board.placeMove(coord, symbol)) return true;
    }

    // Kiểm tra theo đường chéo phụ
    if ((board.getGridValue(0, 2) == opponentSymbol) + (board.getGridValue(1, 1) == opponentSymbol) + (board.getGridValue(2, 0) == opponentSymbol) == 2) {
        coord = "";
        isValid = false;
        if (board.getGridValue(0, 2) == ' ') tmpX = 0, tmpY = 2, isValid = true;
        if (board.getGridValue(1, 1) == ' ') tmpX = 1, tmpY = 1, isValid = true;
        if (board.getGridValue(2, 0) == ' ') tmpX = 2, tmpY = 0, isValid = true;
        coord += ('a' + tmpX);
        coord += ('1' + tmpY);
        if (isValid && board.placeMove(coord, symbol)) return true;
    }

// Nếu không nằm trong hai trường hợp trên thì đánh random (thường là 1 hoặc 2 nước đi đầu tiên của máy)
    if (easyLevel(board)) return true;
    return false;
}

// Kiểm tra mức độ, nếu 1 là dễ, 2 là khó
bool ComputerPlayer::makeMove(Board &board) {
    switch (level) {
        case 1: return easyLevel(board);
        case 2: return difficultLevel(board);
    }
}

// CLASS GAME //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Khởi tạo đối tượng game đồng thời in ra màn hình chữ TIC TAC TOE màu vàng
Game::Game() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, YELLOW);
    cout << "\n"
    << " _ _ _ _        _ _ _            _ _ _          \n" 
    << "|_  _ (_) _ _  |_   _|_ _  _ _  |_   _| _   _ _ \n" 
    << "  | | | |/ __|   | |/ _  |/ __|   | |/ _ \\ / _ \\ \n" 
    << "  | | | | (__    | | (_| | (__    | | (_) |  __/ \n" 
    << "  |_| |_|\\___|   |_|\\____|\\___|   |_|\\___/ \\___ \n" 
    << "                                         CaiBang\n";
    SetConsoleTextAttribute(hConsole, GRAY);
} 

Game::~Game() {}
 
Game::Game(Player *player1, Player *player2) {
    this->player1 = player1;
    this->player2 = player2;
}

void Game::setSleepTime(int sleepTime) {
    this->sleepTime = sleepTime;
}

int Game::getSleepTime() {
    return sleepTime;
}

// Bắt đầu chơi
void Game::start() {
    int num;
    string tmp;
    bool isValid;
    char choice, opponentSymbol, choiceRun;
    
    // Cho người chơi chọn chế độ
    cout << "[-] Moi ban chon che do choi:\n";
    cout << "    1. Hai nguoi choi\n";
    cout << "    2. Choi voi may\n";
    cout << "    3. May voi may\n";
    cout << "[+] Nhap lua chon (1/2/3): ";

    isValid = false;
    while (!isValid) {
        getline(cin, tmp);
        tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());

        try {
            // Nếu nhập không phải số thì nén ngoại lệ với thông điệp "phải nhập số tương ứng"
            if (!isdigit(tmp[0]) || tmp.size() > 1) 
                throw invalid_argument("[!] Ban phai nhap so tuong ung voi lua chon: ");

            // nếu nhập số nhưng không phải 1 2 3 thì nén ngoại lệ với thông điệp "không có lựa chọn này, chọn lại"
            else if (isdigit(tmp[0]) && (stoi(tmp) < 1 || stoi(tmp) > 3))
                throw invalid_argument("[!] Khong co lua chon nay, vui long nhap lai: ");

            // nếu đã nhập đúng thì thoát vòng lặp bằng isValid = true
            else isValid = true;

        } catch (invalid_argument &e) {
            cout << e.what();
        }
    }

    num = stoi(tmp);
    switch (num) {

        // Lựa chọn người với người
        case 1: 
            isValid = false;
            cout << "[+] Chon ky tu cua ban (X/O): ";

            while (!isValid) {
                getline(cin, tmp);
                tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    
                if (tmp.size() > 1 || (toupper(tmp[0]) != 'X' && toupper(tmp[0]) != 'O'))
                    cout << "[!] Chi duoc nhap ky tu X hoac O, vui long nhap lai: ";
                else isValid = true;
                
            }

            choice = toupper(tmp[0]);
            opponentSymbol = choice == 'X' ? 'O' : 'X';

            board = Board();
            player1 = new HumanPlayer(choice);
            player2 = new HumanPlayer(opponentSymbol);
            Game(player1, player2);
            sleepTime = 0;
            break;

        // Lựa chọn người với máy
        case 2:
            int level;
            isValid = false;
            cout << "[+] Chon do kho 1-de, 2-kho (1/2): ";

            while (!isValid) {
                getline(cin, tmp);
                tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());

                if (!isdigit(tmp[0]) || (stoi(tmp) != 1 && stoi(tmp) != 2) || tmp.size() > 1)
                    cout << "[!] Chi duoc nhap so 1 hoac so 2, nhap lai: ";
                else isValid = true;
            }

            level = stoi(tmp);
            isValid = false;
            cout << "[+] Chon ky tu cua ban (X/O): ";

            while (!isValid) {
                getline(cin, tmp);
                tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());
    
                if (tmp.size() > 1 || (toupper(tmp[0]) != 'X' && toupper(tmp[0]) != 'O'))
                    cout << "[!] Chi duoc nhap ky tu X hoac O, vui long nhap lai: ";
                else isValid = true;
            }

            choice = toupper(tmp[0]);
            opponentSymbol = choice == 'X' ? 'O' : 'X';

            board = Board();
            player1 = new HumanPlayer(choice);
            player2 = new ComputerPlayer(opponentSymbol, level);
            Game(player1, player2);

            // nếu mức độ dễ thì không chậm hiển thị, còn khó thì chậm 0.4s
            sleepTime = level == 1 ? 0 : 400;
            break;

        // Lựa chọn máy với máy, cho chế độ 2 máy là khó
        case 3:
            board = Board();
            player1 = new ComputerPlayer('X', 2);
            player2 = new ComputerPlayer('O', 2);

            //thời gian chậm mỗi nước là 0.7s
            sleepTime = 700;
            break;
    }

    // sao khi chọn chế độ xong thì xóa màn hình hiện tại, chuyển qua bắt đầu trò chơi
    system("cls");
    run();

    // Sao khi chơi xong thì hỏi người chơi có muốn chơi lại hay không
    isValid = false;
    while (!isValid) {
        cout << "[+] Ban co muon choi lai? (Y/N): ";

        while (!isValid) {
            getline(cin, tmp);
            tmp.erase(remove(tmp.begin(), tmp.end(), ' '), tmp.end());

            if (toupper(tmp[0]) != 'Y' && toupper(tmp[0]) != 'N' || tmp.size() > 1) 
                cout << "[!] Nhap sai, nhap Y de choi lai hoac N de ket thuc: ";
            else choiceRun = toupper(tmp[0]), isValid = true;
        }
    }

    // Nếu lựa chọn chơi lại thì xóa màn hình cũ và cho chọn lại chế độ
    if (choiceRun == 'Y') {
        system("cls");
        delete player1, player2;
        start();

    } else cout << "Tro choi ket thuc!";
    delete player1, player2;
}

// Phương thức bắt đầu ván đấu
void Game::run() {
    board.display();
    Player *curPlayer = player1;
    
    while (true) {
        cout << "[-] Den luot cua " << curPlayer->getSymbol() << endl;
        Sleep(sleepTime);

        try {
            if (curPlayer->makeMove(board)) {
                
                // Xóa màn hình hiện tại và hiện màn hình mới
                system("cls");  
                board.display();

                // Nếu một trong hai thắng thì in ra thông báo thắng
                if (board.checkWin(curPlayer->getSymbol())) {
                    cout << "[-] Chuc mung nguoi choi " << curPlayer->getSymbol() << " da chien thang!" << endl;
                    break;
                }

                // Nếu bảng đã đánh hết thì thông báo hòa
                if (board.full()) {
                    cout << "[-] Hai nguoi choi da hoa nhau!" << endl;
                    break;
                }

                curPlayer = curPlayer == player1 ? player2 : player1;
            } 

        // Nếu một người chơi không đánh được thì in ra thông điệp tương ứng đã nén trước đó
        } catch (invalid_argument &e) {
            cerr << e.what() << endl << endl;
        }
    }
}