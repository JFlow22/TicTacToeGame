/*#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include "fcgio.h"
#include <sqlite3.h>
#include <stdio.h>
#include <time.h>*/
#include "game.h"





bool isBrdEmpty(){
	for(int i=0;i<9;i++)
		if(brd[i] == "-") 
			return true;
	return false;
}

bool checkTblExistence(string TblName){
	OpenDB();
	string sName;
	string dbdata;
	dbdata = "SELECT name FROM sqlite_master WHERE type='table' AND name='"+TblName+"'";
	sqlite3_stmt *stmt; 
	bool exsits;
  	int rc = sqlite3_prepare_v2(db, dbdata.c_str(), -1, &stmt, NULL );  
  	if(rc != SQLITE_OK) { 
    	cout << "Error: " << sqlite3_errmsg(db) << endl;
    	return false; 
	}
	if (sqlite3_step(stmt) == SQLITE_ROW)
		exsits = true;
	else
		exsits =  false;
	sqlite3_finalize(stmt);
    closeDB();  
    return exsits;
}

//Get option after -d
string get_request_content(const FCGX_Request & request) {
    char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
    unsigned long content_length = STDIN_MAX;

    if (content_length_str) {
        content_length = strtol(content_length_str, &content_length_str, 10);
        if (*content_length_str) {
            cerr << "Can't Parse 'CONTENT_LENGTH='"
                 << FCGX_GetParam("CONTENT_LENGTH", request.envp)
                 << "'. Consuming stdin up to " << STDIN_MAX << endl;
        }

        if (content_length > STDIN_MAX) {
            content_length = STDIN_MAX;
        }
    } else {
        content_length = 0;
    }

    char * content_buffer = new char[content_length];
    cin.read(content_buffer, content_length);
    content_length = cin.gcount();

    // Chew up any remaining stdin - this shouldn't be necessary
    // but is because mod_fastcgi doesn't handle it correctly.

    // ignore() doesn't set the eof bit in some versions of glibc++
    // so use gcount() instead of eof()...
    do cin.ignore(1024); while (cin.gcount() == 1024);

    string content(content_buffer, content_length);
    delete [] content_buffer;
    return content;
}

//open database
void OpenDB() { 
	int rc; 
	string dbName = "players.db"; 
	rc = sqlite3_open_v2(dbName.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); 
	if(rc != SQLITE_OK)
		cerr << "Error???" << endl; 
} 

//Close database
void closeDB() { 
  int rc=sqlite3_close(db); 
  if (rc != SQLITE_OK) { 
    cerr << "Error: " << sqlite3_errmsg(db) << endl; 
  } 
} 

//Delete table
void deleteTable(string table){
	OpenDB();
	string Name; 
  	sqlite3_stmt *stmt; 
	string dbdata = "DELETE from " + table;
	int rc = sqlite3_prepare_v2(db, dbdata.c_str(),dbdata.length(),&stmt,NULL); 
	if(rc != SQLITE_OK) { 
		cerr << "Error: " << sqlite3_errmsg(db) << endl; 
	} 
	rc = sqlite3_step(stmt); 
	if(rc != SQLITE_DONE) { 
		cerr << "Error: " << sqlite3_errmsg(db) << endl;
	} 
	sqlite3_finalize(stmt); 
	closeDB();
}

//Setter
void setter(string table,string col,string value,bool resetTable) { 
	if (resetTable)
		deleteTable(table);
	OpenDB();
	string Name; 
	string dbdata;
	sqlite3_stmt *stmt;
	int rc; 
	dbdata = "INSERT INTO " + table + " (" + col + ") VALUES (" + value + ")";
	rc = sqlite3_prepare_v2(db, dbdata.c_str(),dbdata.length(),&stmt,NULL); 
	if(rc != SQLITE_OK)
		cerr << "Error: " << sqlite3_errmsg(db) << endl;  
	rc = sqlite3_step(stmt); 
	if(rc != SQLITE_DONE) 
		cerr << "Error: " << sqlite3_errmsg(db) << endl; 
	sqlite3_finalize(stmt); 
	closeDB();
} 

void setWinner(string winner) { 
	string currentBRD = getter("GAME_STATE","BRD","");
	string currentTrn = getter("GAME_STATE","TRN","");
	setter("GAME_STATE","BRD,TRN,WIN","'" + currentBRD + "','" + currentTrn + "','"+winner+"'",true);
} 

//Getter
//Condition can be empty
string getter(string table, string col, string condition) { 
  OpenDB();
  string value; 
  string dbdata;
  if (!condition.empty())
  	dbdata = "SELECT " + col + " from " + table + " Where " + condition;
  else
  	dbdata = "SELECT " + col + " from " + table;
  sqlite3_stmt *stmt; 
  int rc = sqlite3_prepare_v2(db, dbdata.c_str(), -1, &stmt, NULL ); 
  if(rc != SQLITE_OK) { 
    cout << "Error: " << sqlite3_errmsg(db) << endl;
    closeDB(); 
    return "Error!"; 
  } 
  while (sqlite3_step(stmt) == SQLITE_ROW) { 
    value = (char*)sqlite3_column_text(stmt, 0); 
  } 
  sqlite3_finalize(stmt); 
  closeDB(); 
  return value;
} 

// ------------- Functions for amit -------------

//Check user existence
//pwd can be empty if you want to check only user name
bool checkUserExistence(string user,string pwd){
	OpenDB();
	string sName;
	string dbdata;
	if(!pwd.empty()){
		dbdata = "SELECT * from PLAYERS where USER='" + user + "' and PWD='" + pwd +"'";
	}
	else
		dbdata = "SELECT * from PLAYERS where USER='" + user + "'";
	sqlite3_stmt *stmt; 
	bool exsits;
  	int rc = sqlite3_prepare_v2(db, dbdata.c_str(), -1, &stmt, NULL );  
  	if(rc != SQLITE_OK) { 
    	cout << "Error: " << sqlite3_errmsg(db) << endl;
    	return false; 
	}
	if (sqlite3_step(stmt) == SQLITE_ROW)
		exsits = true;
	else
		exsits =  false;
	sqlite3_finalize(stmt);
    closeDB();  
    return exsits;
}

//Split text with given splitter
string* getTokens(string content,string splitter){
	string s = content;
	string delimiter = splitter;
	string* input = new string[9];
	int i = 0;
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
    	token = s.substr(0, pos);
    	input[i] = token;
    	s.erase(0, pos + delimiter.length());
    	i++;
	}
	input[i] = s;
	return input;
}

//Get the board array
string* getTable(){
	string tempTable = getter("GAME_STATE","BRD","");
	return getTokens(tempTable,",");
	
}

//Set the board with given array
void setTable(string * table){
	string sTable = "";
	string currentTurn;
	//Copty table to single string with ,
	for (int i=0;i<9;i++){
		if(i==0)
			sTable += table[i];
		else 
			sTable += ("," + table[i]);
	}
	//Get current turn
	currentTurn = getter("GAME_STATE","TRN","");
	//Set the table with new table stats and current turn
	//send true for delete old table stats
	setter("GAME_STATE","BRD,TRN,WIN","'" + sTable + "','" + currentTurn + "','-'",true);
}

//Set turn with given username
void setTurn(string turn){
	//Get current board
	string currentBRD = getter("GAME_STATE","BRD","");
	setter("GAME_STATE","BRD,TRN,WIN","'" + currentBRD + "','" + turn + "','-'",true);
}

//Reset game stats --> board will be -,...,-
//                 --> turn will be -
//				   --> win will be -
void resetGameStat(){
	setter("GAME_STATE","BRD,TRN,WIN","'-,-,-,-,-,-,-,-,-','-','-'",true);
}

//Remove all users
void clearUsers(){
	deleteTable("PLAYERS");
}

//Add user with given username and pass
//return false if username exists or there are more then 2 users
bool addUser(string name, string pwd){
	int amount = amountOfUsers();
	//Check if user exists
	if (checkUserExistence(name,""))
		return false;
	//If not, check how many users are registered
	if (amount == 0){
		setter("PLAYERS","USER,PWD,MRK","'" + name +"','" + pwd + "','x'", false);
		setTurn(name);
		return true;
	}
	if (amount == 1){
		setter("PLAYERS","USER,PWD,MRK","'" + name +"','" + pwd + "','o'", false);
		return true;
	}
	//return false if there are too many players
	return false;
}

//Get amount of users
int amountOfUsers(){
	OpenDB();
  	string value; 
  	string dbdata;
  	int counter = 0;
  	dbdata = "SELECT * from PLAYERS";
  	sqlite3_stmt *stmt; 
  	int rc = sqlite3_prepare_v2(db, dbdata.c_str(), -1, &stmt, NULL ); 
  	if(rc != SQLITE_OK) { 
    	cout << "Error: " << sqlite3_errmsg(db) << endl;
    	closeDB(); 
    	return -1; 
  	} 
  	while (sqlite3_step(stmt) == SQLITE_ROW) { 
    	value = (char*)sqlite3_column_text(stmt, 0); 
    	counter++;
  	} 
  	sqlite3_finalize(stmt); 
  	closeDB(); 
  	return counter;
}

//Get current turn
string getTurn(){;
	return getter("GAME_STATE","TRN","");
}

//Get maek by given username
string getUserMark(string user){
	return getter("PLAYERS","MRK", "USER='" + user + "'");
}

//switch turns
//Work only with 2 players!!!
void switchTurn(){
	setTurn(getter("PLAYERS","USER","USER<>'"+ getTurn() + "'"));
}

void resetBrd(){
	brd = new string[9];
	for(int i=0;i<9;i++)
		brd[i] = "-";
}


char* db_build(){
   sqlite3 *db;
   
    int rc ; // This line
    const char* sql; // This line
    rc = sqlite3_open("players.db", &db);
	if(!checkTblExistence("players")){
   		/* Create SQL statement */
   		sql = "CREATE TABLE PLAYERS("  
         	"USER TEXT PRIMARY		KEY		NOT NULL," 
         	"PWD  TEXT						NOT NULL,"
         	"MRK  TEXT 					NOT NULL)" ;

   /* Execute SQL statement */
   		rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    }
    if(!checkTblExistence("GAME_STATE")){
   
    sql = "CREATE TABLE GAME_STATE("  
         "BRD  TEXT PRIMARY		KEY		NOT NULL," 
         "TRN  TEXT						NOT NULL,"
         "WIN  TEXT						NOT NULL )" ;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    sqlite3_close(db);
   	string tmp[] = {"-","-","-","-","-","-","-","-","-"};
   	setTurn("-");
   	setTable(tmp);
   }
   return 0;
}

bool checkWIn(){
	brd = getTable();
	
	if(brd[0] == brd[1] && brd[1] == brd[2] && brd[2] != "-") return true;
	if(brd[3] == brd[4] && brd[4] == brd[5] && brd[5] != "-") return true;
	if(brd[6] == brd[7] && brd[7] == brd[8] && brd[8] != "-") return true;

	if(brd[0] == brd[3] && brd[3] == brd[6] && brd[6] != "-") return true;
	if(brd[1] == brd[4] && brd[4] == brd[7] && brd[7] != "-") return true;
	if(brd[2] == brd[5] && brd[5] == brd[8] && brd[8] != "-") return true;

	if(brd[0] == brd[4] && brd[4] == brd[8] && brd[8] != "-") return true;
	if(brd[2] == brd[4] && brd[4] == brd[6] && brd[6] != "-") return true;

	return false;

}

char mov(){
	int i = 0;
	brd = new string[9];
	if(place.length() != 2) return 'F';
	char l = place[0];
	switch(l){
		case 'A':
			i = 0;
			break;
		case 'B':
			i = 3;
			break;
		case 'C':
			i = 6;
			break;
		default:
			return 'F';
	}
	l = place[1];
	switch(l){
		case '1':
			i += 0;
			break;
		case '2':
			i += 1;
			break;
		case '3':
			i += 2;
			break;
		default:
			return 'F';
	}
	
	brd = getTable();
	if(brd[i] == "-"){
		brd[i] = getUserMark(user);
		setTable(brd);
	}


	else
		return 'F';

	if(PC_flag){
		if((!isBrdEmpty()) || !checkWIn()){
			int ind = rand() % 9;
			while(brd[ind] != "-") ind = rand() % 9;
			if(checkUserExistence("PC","")){
				brd[ind] = getUserMark("PC");
			}
			else{
				brd[ind] = getUserMark("_PC");
			}
			setTable(brd);
		}

	}
	else{
		switchTurn();
	}
	
	return 'T';
	
}


string BrdToJson(){
	brd = getTable();
	string j = "\"BRD\":[\"";
	j += brd[0];
	j += "\"";
	for(int i=1;i<9;i++){
		j +=",";
		j += "\"";
		j += brd[i];
		j += "\"";
	}
	j+= "]";
	return j;

}



