#include "game.h"

int main() {
	srand (time(NULL));
    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);
   

    sqlite3 *db;
    int rc ; 
    const char* sql; 
    rc = sqlite3_open("players.db", &db);
	
    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

		//Get location 
        const char * uri = FCGX_GetParam("REQUEST_URI", request.envp);
		
		//Get parameter
        string content = get_request_content(request);
        db_build();
        sqlite3_open("players.db", &db);
	//-------------------------------------------------------------------
		
        
		string inp[4];
		for(int i=0;i<4;i++)
			inp[i] = "";

		
		string s = content;
		string delimiter = ":";

		//tokenize 
		size_t pos = 0;
		string token;
		int i=0;

		while ((pos = s.find(delimiter)) != string::npos) {
    		token = s.substr(0, pos);
    		inp[i] = token;
    		s.erase(0, pos + delimiter.length());
    		i++;
		}

		inp[i] = s;
		
		user = inp[0];
		pass = inp[1];

		if(inp[2] != "")
			req = inp[2];

		if(req == "MOV")
			place = inp[3];


		brd = getTable();

		if(checkUserExistence(user,pass)){
			Log_Succ = 'T';
			if(req == "MOV"){
				if((amountOfUsers() < 2) || (getTurn() != user) || getter("GAME_STATE","WIN","") != "-" || !isBrdEmpty()){
					MOV_A = 'F';
				}
				else{
					MOV_A = mov();
					if(checkWIn()){
						setWinner(user);
					}
					
				}

			}

			if(req == "REG"){
				Log_Succ = 'F';
			}

			if(req == "CLR"){
				PC_flag = false;
				resetGameStat();
				clearUsers();
			}

			if(req == "PC"){
				if(amountOfUsers()>1){
					MOV_A = 'F';

				}
				else{
					MOV_A = 'T';
					PC_flag = true;
					if(checkUserExistence("PC","")){
						addUser("_PC","100597");
					}
					else{
						addUser("PC","100597");
					}
				}
			}

			
		}
		else{
			
			if(req == "REG" && amountOfUsers() < 2){
				if(checkUserExistence(user,"")){
					Log_Succ = 'F';
				}else{
					addUser(user,pass);
					Log_Succ = 'T';
				}
			}
			else{
				Log_Succ = 'F';
			}
		}
		
		TRN_A = getTurn();
		

		WIN_A = getter("GAME_STATE","WIN","");
		

		cout << "Content-type: Json\r\n\r\n"
			 << "{\"Log_Succ\":\"" << Log_Succ << "\","
			 << "\"MOV\":\"" << MOV_A << "\","
			 << "\"WIN\":\"" << WIN_A << "\","
			 << "\"TRN\":\"" << TRN_A << "\","
			 << BrdToJson() <<"}"<<endl;
        
        //-------------------------------------------------------------------
        sqlite3_close(db);
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;

}
