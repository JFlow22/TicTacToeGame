#ifndef _HELLO_H
#define _HELLO_H

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "fcgio.h"
#include <sqlite3.h>
//#include <stdio.h>
#include <time.h>

using namespace std;

// Maximum bytes
const unsigned long STDIN_MAX = 1000000;

char MOV_A = 'F',Log_Succ = 'F';
string TRN_A = "NULL",WIN_A = "-";
bool PC_flag = false;
string user,pass,req = "",place = "";
string* brd;

sqlite3 *db; 

char* db_build();
string get_request_content(const FCGX_Request & request);
void OpenDB();
void create_table();
void setter(string table,string col,string value,bool resetTable);
string getter(string table, string value, string condition);
bool checkUserExistence(string user,string pwd);
void closeDB();
string* getTokens(string content,string splitter);
string* getTable();
void deleteTable();
void resetGameStat();
void clearUsers();
bool addUser(string name, string pwd);
int amountOfUsers();
string getTurn();
string getUserMark(string user);
void setWinner(string winner);
void setTable(string * table);
void setTurn(string turn);
void switchTurn();
void resetBrd();
bool checkWIn();
char mov();
string BrdToJson();
bool isBrdEmpty();

#endif
