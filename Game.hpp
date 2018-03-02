//
//  Game.hpp
//  Summer_Practice
//
//  Created by Максим Бачурин on 20/08/2017.
//  Copyright © 2017 Максим Бачурин. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <utility>
#include <set>
#include <vector>
#include <map>
using namespace std;

class Game {
    
    bool in_one_cell; //Возможность игроков вставать на одну клетку
    pair<int, int> user, computer; //Позиции игроков
    char **chessboad; //Поле игры
    //int amount_pawn;
    //const int inf = 1e9;
    int user_count;     // Счёт
    int computer_count; //  игроков
    set<pair<int, int>> pawns; // Координаты пешек
    vector<pair<int, int>> moves; // Стек ходов
    map<pair<int, int>, int> distance_to_user, distance_to_computer; // Расстояния до пешек
    map<pair<int, int>, pair<int, int>> parent; // Массив предков
    char one_game();
    void user_move();
    void computer_move();
    void breadth_first_search(map<pair<int, int>, int> &distance,
             pair<int, int> start_position,
             pair<int, int> second_position,
             bool need_parent); // Вычисление расстояний до других клеток
    void update_chessboard(); // Обновление поля игры
    void print(); // Вывод поля
public:
    Game();
    void play(); // Главная функция игры
};

#endif /* Game_hpp */
