#include <iostream>
#include <vector>
#include <stdlib.h>
#include <limits>
#include <string>
#include <random>

using namespace std;

int ABcounter = 0;
int minimaxcounter = 0;
int maxDepth;


class TTT {
private:
    int boardSize;
    vector<vector<string>> board;

public:
    TTT(int size) : boardSize(size), board(size, vector<string>(size, " ")) {}

    void printBoard() // wyświetlanie planszy
    {
        cout << "PLANSZA " << boardSize << "x" << boardSize << ":\n\n";

        for (int i = 0; i < boardSize; i++)
        {
            cout << "\t";
            for (int j = 0; j < boardSize; j++)
            {
                string content = board[i][j];
                if (content == " ")
                {
                    int fieldNum = i * boardSize + j + 1;
                    if (fieldNum < 10)
                        cout << " " << fieldNum;
                    else
                        cout << fieldNum;
                }
                else
                {
                    cout << " " << content;
                }

                if (j < boardSize - 1)
                    cout << " |";
            }
            cout << "\n";

            if (i < boardSize - 1)
            {
                cout << "\t";
                for (int j = 0; j < boardSize; j++)
                {
                    cout << "---";
                    if (j < boardSize - 1)
                        cout << "+";
                }
                cout << "\n";
            }
        }
        cout << "\n";
    }

    // zabezpieczenie przed błędnymi danymi
    int getInt(bool playerMove = false)
    {
        int val;
        cin >> val;
        //dopóki są błędne dane lub dopóki jest ruch gracza i:
        //(wartość nie jest mniejsza od zera lub
        // wartość nie jest większa od ostatniego pola lub
        // jeśli wpisane pole nie jest puste) lub
        // jeżeli to nie ruch gracza i wartość jest mniejsza od zera
        while (cin.fail() || (playerMove && (val < 1 || val > boardSize * boardSize || board[getField(val).first][getField(val).second] != " ")) || (!playerMove && val < 0))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nieprawidlowa wartosc, sprobuj ponownie\n>> ";
            cin >> val;
        }
        return val;
    }

    // sprawdzamy czy można wykonać jakikolwiek ruch
    bool canMove()
    {
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (board[i][j] == " ")
                    return true;
            }
        }
        return false;
    }

    // sprawdzamy czy gra się zakończyła i zwracamy kto wygrał
    int isEnd()
    {
        for (int i = 0; i < boardSize; i++)
        {
            bool rowWin = true;
            bool colWin = true;
            string rSymbol = board[i][0];
            string cSymbol = board[0][i];

            for (int j = 0; j < boardSize; j++)
            {
                //sprawdzamy wygrane w wierszach
                if (board[i][j] != rSymbol)
                    rowWin = false;

                //sprawdzamy wygrane w kolumnach
                if (board[j][i] != cSymbol)
                    colWin = false;
            }

            //sprawdzamy KTO jest zwycięzcą
            if (rowWin && rSymbol != " ")
            {
                if (rSymbol == "X")
                    return 1;
                else return -1;
            }

            if (colWin && cSymbol != " ")
            {
                if (cSymbol == "X")
                    return 1;
                else return -1;
            }
        }

        // ----Sprawdzamy dla przekątnych----
        bool leftDiag = true;
        bool rightDiag = true;
        string ld = board[0][0];
        string rd = board[0][boardSize - 1];

        for (int i = 0; i < boardSize; i++)
        {
            if (board[i][i] != ld)
                leftDiag = false;
            if (board[i][boardSize - 1 - i] != rd)
                rightDiag = false;
        }

        //sprawdzamy zwycięzce
        if (leftDiag && ld != " ")
        {
            if (ld == "X")
                return 1;
            else
                return -1;
        }

        if (rightDiag && rd != " ")
        {
            if (rd == "X")
                return 1;
            else
                return -1;
        }

        //sprawdzamy czy można wykonać jeszcze jakiś ruch
        if (canMove())
            return 2;
        else
            return 0;
    }

    // do wyboru pola
    pair<int, int> getField(int num)
    {
        return { (num - 1) / boardSize, (num - 1) % boardSize };
    }

    // podstawowy
    int minimax(vector<vector<string>> state, bool aiMove, int depth)
    {
        minimaxcounter++;
        int score = isEndState(state);
        if (score != 2)
        {
            if (score == 1)
                return 10 - depth;
            else if (score == -1)
                return -10 + depth;
            else return 0;
        }

        //sprawdzamy, czy osiągnięto maksymalnej głębokości
        if (depth >= maxDepth) {
            return 0; //jeśli tak, zwracamy neutralną ocene
        }

        int best;
        if (aiMove)
            best = numeric_limits<int>::min();
        else
            best = numeric_limits<int>::max();

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (state[i][j] == " ")
                {
                    if (aiMove)
                        state[i][j] = "X";
                    else state[i][j] = "O";

                    int val = minimax(state, !aiMove, depth + 1);
                    state[i][j] = " ";

                    if (aiMove)
                        best = max(best, val);
                    else
                        best = min(best, val);
                }
            }
        }

        return best;
    }

    // alpha-beta
    int AB_minimax(vector<vector<string>> state, bool aiMove, int alpha, int beta, int depth)
    {
        ABcounter++;
        int score = isEndState(state);
        if (score != 2)
        {
            if (score == 1)
                return 10 - depth;
            else if (score == -1)
                return -10 + depth; 
            else return 0;
        }

        if (depth >= maxDepth) {
            return 0; 
        }

        int best;
        if (aiMove)
        {
            best = numeric_limits<int>::min();
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (state[i][j] == " ")
                    {
                        state[i][j] = "X";
                        int val = AB_minimax(state, false, alpha, beta, depth + 1);
                        state[i][j] = " ";
                        best = max(best, val);
                        alpha = max(alpha, best);
                        if (beta <= alpha)
                            return best;
                    }
                }
            }
            return best;
        }
        else
        {
            best = numeric_limits<int>::max();
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (state[i][j] == " ")
                    {
                        state[i][j] = "O";
                        int val = AB_minimax(state, true, alpha, beta, depth + 1); 
                        state[i][j] = " ";
                        best = min(best, val);
                        beta = min(beta, best);
                        if (beta <= alpha)
                            return best;
                    }
                }
            }
            return best;
        }
    }

    pair<int, int> bestMove()
    {
        int bestVal = numeric_limits<int>::min();
        pair<int, int> move = { -1, -1 };

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (board[i][j] == " ")
                {
                    board[i][j] = "X";
                    int val = minimax(board, false, 0);
                    board[i][j] = " ";

                    if (val > bestVal)
                    {
                        bestVal = val;
                        move = { i, j };
                    }
                }
            }
        }
        return move;
    }

    pair<int, int> AB_bestmove()
    {
        int bestVal = numeric_limits<int>::min();
        pair<int, int> move = { -1, -1 };

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (board[i][j] == " ")
                {
                    board[i][j] = "X";
                    int val = AB_minimax(board, false, numeric_limits<int>::min(), numeric_limits<int>::max(), 0);
                    board[i][j] = " ";

                    if (val > bestVal)
                    {
                        bestVal = val;
                        move = { i, j };
                    }
                }
            }
        }
        return move;
    }

    void play(bool isExpanded = false, int whoStart = 0)
    {

        bool playerMove = false;
        if (whoStart == 1) // kto zaczyna - losowo
        {
            int random = rand() % 2 + 1;
            if (random == 2)
            {
                playerMove = true;
            }
        }
        else if (whoStart == 3) // zaczyna gracz
        {
            playerMove = true;
        }


        while (isEnd() == 2)
        {
            system("CLS");
            printBoard();

            if (playerMove)
            {
                if (whoStart == 2)
                {
                    cout << "(wywolania minimax: " << minimaxcounter << ")" << endl;
                    cout << "wywolania minimax AB: " << ABcounter << ")" << endl;
                    whoStart = -999;
                }
                cout << "Twoj ruch, wybierz spomiedzy (1-" << boardSize * boardSize << " pol): ";
                int chosenField = getInt(true);
                auto [row, column] = getField(chosenField);
                board[row][column] = "O";
            }
            else
            {
                pair<int, int> aiMove;
                if (isExpanded)
                {
                    aiMove = AB_bestmove();
                }
                else
                {
                    aiMove = bestMove();
                }

                int aiRow = aiMove.first;
                int aiColumn = aiMove.second;
                board[aiRow][aiColumn] = "X";
            }
            playerMove = !playerMove;
        }

        system("CLS");
        printBoard();
        int result = isEnd();
        if (result == 1)
            cout << "WYGRALO AI!\n";
        else if (result == -1)
            cout << "GRATULACJE WYGRALES\n";
        else
            cout << "REMIS!\n";

        // żeby program się od razu nie wyłączał
        cout << endl;
        cout << "WYWOLANIA MINIMAX: " << minimaxcounter << endl;
        cout << "WYWOLANIA AB..: " << ABcounter << endl;
        char pause;
        cout << "Wprowadz cokolwiek aby zamknac program\n";
        cin >> pause;
    }

private:
    // to nam sprawdza koniec na DOWOLNEJ planszy, czyli np. tej która jest testowana w minimax
    // natomiast isEnd sprawdza koniec AKTUALNEJ
    int isEndState(vector<vector<string>> state)
    {
        for (int i = 0; i < boardSize; i++)
        {
            bool rowWin = true;
            bool colWin = true;
            string rSymbol = state[i][0];
            string cSymbol = state[0][i];

            for (int j = 0; j < boardSize; j++)
            {
                //sprawdzamy wygrane w wierszach
                if (state[i][j] != rSymbol)
                    rowWin = false;

                //sprawdzamy wygrane w kolumnach
                if (state[j][i] != cSymbol)
                    colWin = false;
            }

            //sprawdzamy KTO jest zwycięzcą
            if (rowWin && rSymbol != " ")
            {
                if (rSymbol == "X")
                    return 1;
                else return -1;
            }

            if (colWin && cSymbol != " ")
            {
                if (cSymbol == "X")
                    return 1;
                else return -1;
            }
        }

        // ----Sprawdzamy dla przekątnych----
        bool leftDiag = true;
        bool rightDiag = true;
        string ld = state[0][0];
        string rd = state[0][boardSize - 1];

        for (int i = 0; i < boardSize; i++)
        {
            if (state[i][i] != ld)
                leftDiag = false;
            if (state[i][boardSize - 1 - i] != rd)
                rightDiag = false;
        }

        //sprawdzamy zwycięzce
        if (leftDiag && ld != " ")
        {
            if (ld == "X")
                return 1;
            else
                return -1;
        }

        if (rightDiag && rd != " ")
        {
            if (rd == "X")
                return 1;
            else
                return -1;
        }

        //sprawdzamy czy można wykonać jeszcze jakiś ruch
        for (auto row : state)
        {
            for (auto cell : row)
            {
                if (cell == " ")
                    return 2;
            }
        }
        return 0;
    }
};

int _getInt(bool isChoice = false, bool isPick = false)
{
    int val;
    if (isPick)
    {
        cin >> val;
        while (cin.fail() || val < 1 || val > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nieprawidlowa wartosc, sprobuj ponownie\n>> ";
            cin >> val;
        }
        return val;
    }
    if (isChoice)
    {
        cin >> val;
        while (cin.fail() || val < 1 || val > 2)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nieprawidlowa wartosc, sprobuj ponownie\n>> ";
            cin >> val;
        }
        return val;
    }
    else
    {
        int val;
        cin >> val;
        while (cin.fail() || val < 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nieprawidlowa wartosc, sprobuj ponownie\n>> ";
            cin >> val;
        }
        return val;
    }
}

void selectDifficulty() {
    int choiced;
    cout << "Wybierz poziom trudnosci AI:\n";
    cout << "1. Latwy (glebokosc 2)\n2. Sredni (glebokosc 6)\n3. Trudny (glebokosc 16)\n4. Niemozliwy (glebokosc 32)\n";
    cout << "Wprowadz wybor (1-4): ";
    cin >> choiced;

    switch (choiced) {
    case 1:
        maxDepth = 2;
        cout << "Poziom trudnosci ustawiony na Latwy (glebokosc = 2)\n \n";
        break;
    case 2:
        maxDepth = 8;
        cout << "Poziom trudnosci ustawiony na Sredni (glebokosc = 8)\n \n";
        break;
    case 3:
        maxDepth = 16;
        cout << "Poziom trudnosci ustawiony na Trudny (glebokosc = 16)\n \n";
        break;
    case 4:
        maxDepth = 32;
        cout << "Poziom trudnosci ustawiony na Niemozliwy (glebokosc = 32)\n \n";
        break;
    default:
        maxDepth = 8;
        cout << "Nieprawidlowy wybor, ustawiono Sredni (glebokosc = 8)\n \n";
    }
}
int main()
{

    srand(time(NULL));
    int choice;
    int pickWhoStart;
    int depthchoice;
    cout << "===GRA W KOLKO I KRZYZYK===" << endl << endl;
    cout << "Dokonaj wyboru:" << endl;
    cout << "1 - Podstawowy minimax (zalecane maksymalnie dla planszy 3x3)\n2 - Rozszerzony minimax" << endl;
    choice = _getInt(true);
    selectDifficulty();

    cout << "Chcesz wybrac kto zaczyna?\n1 - Tak\n2 - Nie" << endl;
    pickWhoStart = _getInt(true);
    if (pickWhoStart == 1)
    {
        cout << "Kto ma zaczac gre?\n1 - Losowo\n2 - Komputer\n3 - Gracz" << endl;
        pickWhoStart = _getInt(false, true);
    }
    else pickWhoStart = 0;
    if (choice == 1)
    {
        int size;
        cout << "Podaj rozmiar planszy: ";
        size = _getInt();
        TTT game(size);
        game.play(false, pickWhoStart);
    }
    else
    {
        int size;
        cout << "Podaj rozmiar planszy: ";
        size = _getInt();
        TTT game(size);
        game.play(true, pickWhoStart);
    }

}