#include <iostream>
#include <vector>
#include <stdlib.h>
#include <limits>
#include <string>
#include <random>

using namespace std;

class TTT{
private:
    int boardSize;
    vector<vector<string>> board;

public:
    TTT(int size) : boardSize(size), board(size, vector<string>(size, " ")) {}

    // wyświetlamy planszę
    void printBoard()
    {
        cout << "PLANSZA " << boardSize << "x" << boardSize <<":\n";
        for(int i = 0; i<boardSize; i++)
        {
            cout << "\t";
            for(int j = 0; j < boardSize; j++)
            {
                if(board[i][j] == " ")
                {
                    cout << (i *  boardSize + j + 1); // numerujemy pola
                }
                else
                {
                    cout << board[i][j]; // jesli cos jest postawione na polu, wypisujemy
                }
                if(j < boardSize-1)
                    cout << " | ";
            }
            cout << "\n";
            if(i < boardSize-1)
            {
                cout << "\t";
                for (int j = 0; j < boardSize; j++)
                {
                    cout << "---";
                    if(j < boardSize-1)
                        cout << "+";
                    else cout << " ";
                }
                cout << "\n";
            }
        } 
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
        while(cin.fail() || (playerMove && (val < 1 || val > boardSize * boardSize || board[getField(val).first][getField(val).second] != " ")) || (!playerMove && val < 0))
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
        for(int i = 0; i < boardSize; i++)
        {
            for(int j = 0; j < boardSize; j++)
            {
                if(board[i][j] == " ")
                    return true;
            }
        }
        return false;
    }

    // sprawdzamy czy gra się zakończyła i zwracamy kto wygrał
    int isEnd()
    {
        for(int i = 0; i < boardSize; i++)
        {
            bool rowWin = true;
            bool colWin = true;
            string rSymbol = board[i][0];
            string cSymbol = board[0][i];

            for(int j = 0; j < boardSize; j++)
            {
                //sprawdzamy wygrane w wierszach
                if(board[i][j] != rSymbol)
                    rowWin = false;
                
                //sprawdzamy wygrane w kolumnach
                if(board[j][i] != cSymbol)
                    colWin = false;
            }

            //sprawdzamy KTO jest zwycięzcą
            if(rowWin && rSymbol != " ")
            {
                if(rSymbol == "X")
                    return 1;
                else return -1;
            }

            if(colWin && cSymbol != " ")
            {
                if(cSymbol == "X")
                    return 1;
                else return -1;
            }
        }


//         ----Sprawdzamy dla przekątnych----
        bool leftDiag = true;
        bool rightDiag = true;
        string ld = board[0][0];
        string rd = board[0][boardSize-1];

        for(int i = 0; i < boardSize; i++)
        {
            if(board[i][i] != ld)
                leftDiag = false;
            if(board[i][boardSize-1 - i] != rd)
                rightDiag = false;
        }

        //sprawdzamy zwycięzce
        if(leftDiag && ld != " ")
        {
            if(ld == "X")
                return 1;
            else
                return -1;
        }

        if(rightDiag && rd != " ")
        {
            if(rd == "X")
                return 1;
            else
                return -1;
        }
        
        //sprawdzamy czy można wykonać jeszcze jakiś ruch
        if(canMove())
            return 2;
        else
            return 0;
    }

    // do wyboru pola
    pair<int, int> getField(int num)
    {
        return {(num-1) / boardSize, (num-1) % boardSize};
    }

    int minimax(vector<vector<string>> state, bool aiMove)
    {
        int score = isEndState(state);
        if (score != 2)
            return score;
        
        int best;
        if(aiMove)
            best = numeric_limits<int>::min();
        else
            best = numeric_limits<int>::max();

        for(int i = 0; i < boardSize; i++)
        {
            for(int j = 0; j < boardSize; j++)
            {
                if(state[i][j] == " ")
                {
                    if(aiMove)
                        state[i][j] = "X";
                    else state[i][j] = "O";
                    
                    int val = minimax(state, !aiMove);
                    state[i][j] = " ";

                    if(aiMove)
                        best = max(best,val);
                    else
                        best = min(best,val);
                }
            }
        }

        return best;
    }

    pair<int, int> bestMove()
    {
        int bestVal = numeric_limits<int>::min();
        pair<int,int> move = {-1, -1};
        
        for(int i = 0; i < boardSize; i++)
        {
            for(int j = 0; j < boardSize; j++)
            {
                if(board[i][j] == " ")
                {
                    board[i][j] = "X";
                    int val = minimax(board, false);
                    board[i][j] = " ";

                    if(val > bestVal)
                    {
                        bestVal = val;
                        move = {i, j};
                    }
                }
            }
        }
        return move;
    }

    void play()
    {
        bool playerMove = false;
        // losujemy kto zaczyna
        int random = rand() % 2 + 1;
        cout << "Zaczyna: ";
        if(random == 2)
        {
            cout << "Gracz" << endl;
            playerMove = true;
        }else
        {
            cout << "AI" << endl;
        }
        

        while(isEnd() == 2)
        {
            system("CLS");
            printBoard();

            if(playerMove)
            {
                cout << "Twoj ruch, wybierz spomiedzy (1-"<< boardSize * boardSize <<" pol): ";
                int chosenField = getInt(true);
                auto [row, column] = getField(chosenField);
                board[row][column] = "O";
            }
            else
            {
                auto [aiRow, aiColumn] = bestMove();
                board[aiRow][aiColumn] = "X";
            }
            playerMove = !playerMove;
        }

        system("CLS");
        printBoard();
        int result = isEnd();
        if(result == 1)
            cout << "WYGRALO AI!\n";
        else if(result == -1)
            cout << "GRATULACJE WYGRALES\n";
        else
            cout << "REMIS!\n";

        // żeby program się od razu nie wyłączał
        char pause;
        cout << "Wprowadz cokolwiek aby zamknac program\n";
        cin >> pause;
    }

//jest tak nisko, bo to funkcja, żeby w miarę pogrupować
private:

    // to nam sprawdza koniec na dowolnej planszy, czyli np. tej która jest testowana w minimax
    // natomiast isEnd sprawdza koniec aktualnej
    int isEndState(vector<vector<string>> state)
    {
        for(int i = 0; i < boardSize; i++)
        {
            bool rowWin = true;
            bool colWin = true;
            string rSymbol = state[i][0];
            string cSymbol = state[0][i];

            for(int j = 0; j < boardSize; j++)
            {
                //sprawdzamy wygrane w wierszach
                if(state[i][j] != rSymbol)
                    rowWin = false;
                
                //sprawdzamy wygrane w kolumnach
                if(state[j][i] != cSymbol)
                    colWin = false;
            }

            //sprawdzamy KTO jest zwycięzcą
            if(rowWin && rSymbol != " ")
            {
                if(rSymbol == "X")
                    return 1;
                else return -1;
            }

            if(colWin && cSymbol != " ")
            {
                if(cSymbol == "X")
                    return 1;
                else return -1;
            }
        }


//         ----Sprawdzamy dla przekątnych----
        bool leftDiag = true;
        bool rightDiag = true;
        string ld = state[0][0];
        string rd = state[0][boardSize-1];

        for(int i = 0; i < boardSize; i++)
        {
            if(state[i][i] != ld)
                leftDiag = false;
            if(state[i][boardSize-1 - i] != rd)
                rightDiag = false;
        }

        //sprawdzamy zwycięzce
        if(leftDiag && ld != " ")
        {
            if(ld == "X")
                return 1;
            else
                return -1;
        }

        if(rightDiag && rd != " ")
        {
            if(rd == "X")
                return 1;
            else
                return -1;
        }
        
        //sprawdzamy czy można wykonać jeszcze jakiś ruch
        for(auto row : state)
        {
            for(auto cell : row)
            {
                if(cell == " ")
                    return 2;
            }
        }
        return 0;
    }
};

int _getInt(bool isChoice = false)
{
    if(isChoice)
    {
        int val;
        cin >> val;
        while(cin.fail() || val < 1 || val > 2)
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
        while(cin.fail() || val < 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nieprawidlowa wartosc, sprobuj ponownie\n>> ";
            cin >> val;
        }
        return val;
    }
}

int main()
{
    int choice;
    cout << "===GRA W KOLKO I KRZYZYK==="<< endl << endl;
    cout << "Dokonaj wyboru:" << endl;
    cout << "1 - Podstawowy minimax\n2 - Rozszerzony minimax" << endl;   
    choice = _getInt(true);
    if(choice == 1)
    {
        int size;
        cout << "Podaj rozmiar planszy: ";
        size = _getInt();
        TTT game(size);
        game.play();
    }    
    else
    {
        cout << "[INFO] Jeszcze nie zaimplementowane";
        // int size;
        // cout << "Podaj rozmiar planszy: ";
        // size = _getInt();
        // TTT game(size);
        // game.play();
    }
}