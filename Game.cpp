//
//  Game.cpp
//  Summer_Practice
//
//  Created by Максим Бачурин on 20/08/2017.
//  Copyright © 2017 Максим Бачурин. All rights reserved.
//

#include <iostream>
#include "Game.hpp"
#include <ctime>
#include <queue>
#include <utility>

using namespace std;

Game::Game() {
    in_one_cell = false;
    user = computer = {0,0};
    chessboad = NULL;
    user_count = computer_count = 0;
}

void Game::update_chessboard() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            chessboad[i][j] = '.';
            if(i == user.first && j == user.second)
                chessboad[i][j] = 'Y';
            if(i == computer.first && j == computer.second)
                chessboad[i][j] = 'C';
            if(i == user.first && j == user.second && i == computer.first && j == computer.second)
                chessboad[i][j] = 'B';
            if(pawns.count({i,j}))
                chessboad[i][j] = 'P';
        }
    }
}

char Game::one_game() {
    pawns.clear();
    parent.clear();
    moves.clear();
    chessboad = new char *[8];
    for(int i = 0; i < 8; i++)
        chessboad[i] = new char [8];
    user = {0,0};
    computer = {7,7};
    int amount_pawn = 0;
    user_count = computer_count = 0;
    cout << "Введите количество пешек (1-62): ";
    while(amount_pawn < 1 || amount_pawn > 62)
        cin >> amount_pawn;
    srand((int)time(NULL));
    while (pawns.size() < amount_pawn) {
        pair<int, int> new_pawn = {rand()%8, rand()%8};
        if(new_pawn != user && new_pawn != computer)
            pawns.insert(new_pawn);
    }
    cout << "Могут ли игроки вставать одновременно\nна одну и ту же клетку? (Y/N): ";
    char c;
    cin >> c;
    if(c == 'Y' || c == 'y')
        in_one_cell = true;
    else
        in_one_cell = false;
    update_chessboard();
    cout << "Y - Вы\nC - Компьютер\nP - Пешка\n";
    if(in_one_cell)
        cout << "B - Оба\n";
    if(~rand()&1)
        user_move();
    while(true) {
        computer_move();
        if(pawns.empty()) {
            if(user_count > computer_count)
                return '2';
            if(user_count < computer_count)
                return '1';
            return '3';
        }
        user_move();
        if(pawns.empty()) {
            if(user_count > computer_count)
                return '2';
            if(user_count < computer_count)
                return '1';
            return '3';
        }
    }
    return '0';
}

void Game::user_move() {
    print();
    cout << "Ваш ход. Введите координаты (A-H)(1-8)\n (Например: B8 или c3): ";
    char x = ' ', y = ' ';
    string s;
    while(!(((abs(x - 'A' - user.first) == 1 &&
              abs(y - '1' - user.second) == 2) ||
       (abs(x - 'A' - user.first) == 2 &&
        abs(y - '1' - user.second) == 1)) &&
          (in_one_cell || !(((x - 'A') == computer.first) &&
                            (y - '1') == computer.second)) &&
            x >= 'A' && x <= 'H' && y >= '1' && y <= '8')) {
        cin >> s;
        if(s.size() != 2)
            continue;
        x = s[0], y = s[1];
        if(x >= 'a' && x <= 'z')
            x += 'A' - 'a';
    }
    user = {x - 'A', y - '1'};
    if(pawns.count(user)) {
        user_count++;
        pawns.erase(user);
    }
    update_chessboard();
}

void Game::computer_move() {
    if(moves.empty()) {
        breadth_first_search(distance_to_user, user, computer, false);
        breadth_first_search(distance_to_computer, computer, user, true);
        int minimal = 1e9;
        pair<int, int> nearest_pawn;
        for(auto x : pawns) {
            if(distance_to_computer[x] <= distance_to_user[x] && distance_to_computer[x] < minimal) {
                minimal = distance_to_computer[x];
                nearest_pawn = x;
            }
        }
        if(minimal == 1e9) {
            for(auto x : pawns) {
                if(distance_to_computer[x] < minimal) {
                    minimal = distance_to_computer[x];
                    nearest_pawn = x;
                }
            }
            pair<int, int> move = nearest_pawn;
            while (parent[move] != computer)
                move = parent[move];
            computer = move;
        }
        else {
            pair<int, int> move = nearest_pawn;
            while (parent[move] != computer) {
                moves.push_back(move);
                move = parent[move];
            }
            if(pawns.count(move)) {
                computer_count++;
                pawns.erase(move);
            }
            computer = move;
        }
    }
    else {
        pair<int, int> move = moves.back();
        if(move == user && !in_one_cell) {
            moves.clear();
            computer_move();
            return;
        }
        moves.pop_back();
        if(pawns.count(move)) {
            computer_count++;
            pawns.erase(move);
        }
        computer = move;
    }
    update_chessboard();
}

void Game::breadth_first_search(map<pair<int, int>, int> &distance,
               pair<int, int> start_position,
               pair<int, int> second_position,
               bool need_parent) {
    distance.clear();
    if(need_parent)
        parent.clear();
    map<pair<int, int>, bool> used;
    used.clear();
    vector<pair<int, int>> direction{{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
    distance[start_position] = 0;
    used[start_position] = true;
    queue<pair<int, int>> q;
    q.push(start_position);
    while(!q.empty()) {
        pair<int, int> k = q.front();
        q.pop();
        for(auto x : direction) {
            pair<int, int> new_k = {k.first + x.first, k.second + x.second};
            if(k == start_position && new_k == second_position && !in_one_cell)
                continue;
            if(0 <= new_k.first && new_k.first < 8 &&
               0 <= new_k.second && new_k.second < 8 && !used.count(new_k)) {
                used[new_k] = true;
                q.push(new_k);
                distance[new_k] = distance[k] + 1;
                if(need_parent)
                    parent[new_k] = k;
            }
        }
    }
}

void Game::print() {
    cout << "  1 2 3 4 5 6 7 8 \n";
    for(int i = 0; i < 8; i++) {
        cout << " .-.-.-.-.-.-.-.-.\n";
        cout << char('A' + i) << '|';
        for(int j = 0; j < 8; j++) {
            cout << chessboad[i][j] << '|';
        }
        cout << endl;
    }
    cout << " .-.-.-.-.-.-.-.-.\n";
    cout << "Счёт:\nВы " << user_count << " : " << computer_count << " Компьютер. "<< endl;
}

void Game::play() {
    char solution;
    do {
        char result = one_game();
        print();
        for(int i = 0; i < 8; i++)
            delete [] chessboad[i];
        delete [] chessboad;
        if(result == '1')
            cout << "Вы проиграли :(" << endl;
        if(result == '2')
            cout << "Вы выиграли!!!" << endl;
        if(result == '3')
            cout << "Ничья." << endl;
        cout << "Хотите сыграть ещё? (Y/N): ";
        cin >> solution;
    }while(solution == 'Y' || solution == 'y');
}
