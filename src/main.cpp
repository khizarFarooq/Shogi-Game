#include<windows.h>
#include<conio.h>
#include<math.h>
#include<iostream>
#include<fstream>
using namespace std;
void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput,scrn);
}
char A3darray[1000][9][9];
int counter=0;
int c=0;
char  threed[1000][9][9];
// For printing box of board
void Print_box(int rdim, int cdim,int row,int col,char ch)
{
    for(int r=1; r<=rdim; r++)
    {
        for(int c=1; c<=cdim; c++)
        {
            if(r==1|| r==rdim || c==1 || c==cdim)
            {
                gotoRowCol((r-1)+row,(c-1)+col);
                cout<<ch;
            }
        }
    }
}
//For print board and character
void Print_board(int rdim, int cdim,char board[9][9])
{

    for(int row=0; row<9; row++)
    {
        for(int col=0; col<9; col++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4+row);
            Print_box(rdim,cdim,row*rdim,col*cdim,-37);
        }
    }
    for(int r=3,rows=0; r<75,rows<9; r+=8,rows++)
    {
        for(int c=5,cols=0; c<96,cols<9; c+=10,cols++)
        {
            gotoRowCol(r,c);
            if(board[rows][cols]>=64&&board[rows][cols]<=90)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
            else if(board[rows][cols]>=97&&board[rows][cols]<=122)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
            else  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),255);
            cout<<board[rows][cols];
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
        }
    }
}
void initiallize3d(char board[9][9])
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            threed[c][i][j]=board[i][j];
        }
    }
    c++;
}
void undo(char board[9][9],int r_dim,int c_dim)
{
    c=c-2;
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            board[i][j]=threed[c][i][j];
        }
    }
    Print_board(r_dim,c_dim,board);
    c++;
}
void redo(char board[9][9],int r_dim,int c_dim)
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            board[i][j]=threed[c][i][j];
        }
    }
    Print_board(r_dim,c_dim,board);
    c++;
}
void replay(char board[9][9],int r_dim,int c_dim)
{
    for(int count=0; count<c; count++)
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                board[i][j]=threed[c][i][j];
            }
        }
        Print_board(r_dim,c_dim,board);
        Sleep(600.0);
    }
}
void replayredo(char board[9][9],int sc[2],int r_dim,int c_dim)
{
    if(sc[0]==1)
    {
        undo(board,r_dim,c_dim);
    }
    if(sc[0]==2)
    {
        redo(board,r_dim,c_dim);
    }
    if(sc[0]==3)
    {
        replay(board,r_dim,c_dim);
    }
}

void getRowColbyLC(int &rpos,int &cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if(InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    }
    while(true);
}

// For check x-axis move
bool is_x_axis_move(int sc[2],int ec[2])
{
    return sc[0]==ec[0];
}
// For check y-axis move
bool is_y_axis_move(int sc[2],int ec[2])
{
    return (sc[1]==ec[1]);
}
// For check diagonal move
bool is_diagonal_move(int sc[2],int ec[2])
{
    return ( abs(sc[0]-ec[0])==abs(sc[1]-ec[1]));
}
// For validation of piece is capital
bool iscapital(int sc[2],char board[9][9])
{
    return (board[sc[0]][sc[1]]>='A'&&board[sc[0]][sc[1]]<='Z');
}
// For validation of piece is small
bool issmall(int sc[2],char board[9][9])
{
    return (board[sc[0]][sc[1]]>='a'&& board[sc[0]][sc[1]]<='z');
}
// For check clean pat
bool is_x_axis_path_clean(char board[9][9],int sc[2],int ec[2])
{
    int min_c,max_c;
    if(sc[1]<ec[1])//Left to right
    {
        min_c=sc[1];
        max_c=ec[1];
    }
    else//Right to left
    {
        min_c=ec[1];
        max_c=sc[1];
    }
    int time=abs(min_c-max_c)-1;
    for(int i=1; i<=time; i++)
    {
        if(board[sc[0]][min_c+i]!='-')
        {
            return  false;
        }
    }
    return true;
}
// For check y-axis path clean
bool is_y_axis_clean(char board[9][9],int sc[2],int ec[2])
{
    int min_r,max_r;

    if(sc[0]<ec[0])//Up to down
    {
        min_r=sc[0];
        max_r=ec[0];
    }
    else//Down to up
    {
        min_r=ec[0];
        max_r=sc[0];
    }
    int time=(max_r-min_r)-1;
    for(int i=1; i<=time; i++)
    {
        if(board[ min_r+i][sc[1]]!='-')
        {
            return false;
        }
    }
    return true;
}
//For cherck diagonal path clean
bool is_diagonal_path_clean(char board[9][9],int sc[2],int ec[2])
{
    int min_r,min_c,max_r,max_c,time_r;
    if(sc[0]<ec[0]&&sc[1]<ec[1])//up to down
    {
        min_r=sc[0];
        min_c=sc[1];
        max_r=ec[0];
        max_c=ec[1];
        time_r=abs(min_r-max_r)-1;
        for (int i=1; i<=time_r; i++)
        {
            if(board[min_r+i][min_c+i]!='-')
            {
                return false;
            }
        }
    }
    if(sc[0]>ec[0]&&sc[1]>ec[1])//down to up
    {

        min_r=ec[0];
        max_r=sc[0];
        min_c=ec[1];
        max_c=sc[1];
        time_r=(max_r-min_r)-1;
        for (int i=1; i<=time_r; i++)
        {
            if(board[min_r+i][min_c+i]!='-')
            {
                return false;
            }
        }
    }
    if(sc[0]>ec[0]&&sc[1]<ec[1])
    {
        min_r=ec[0];
        max_r=sc[0];
        min_c=sc[1];
        max_c=ec[1];
        time_r=(max_r-min_r)-1;
        for(int i=1; i<=time_r; i++)
        {
            if(board[max_r-i][min_c+i]!='-')
            {
                return false;
            }
        }

    }
    if(sc[0]<ec[0]&&sc[1]>ec[1])
    {
        min_r=sc[0];
        max_r=ec[0];
        min_c=ec[1];
        max_c=sc[1];
        time_r=(max_r-min_r)-1;
        for(int i=1; i<=time_r; i++)
        {
            if(board[min_r+i][max_c-i]!='-')
            {
                return false;
            }
        }

    }
    return true;
}
// For validation of pawn move
bool is_valid_pawn_move(int sc[2],int ec[2],char board[9][9],int turn)
{
    if(turn==1&&(is_y_axis_move(sc,ec)==true &&(sc[0]-ec[0]==-1)))
    {
        return true;
    }
    if(turn==2&&(is_y_axis_move(sc,ec)==true&&(sc[0]-ec[0]==1) ))
    {
        return true;
    }
    return false;
}
//For validation of rukh move
bool is_valid_rukh_move(int sc[2],int ec[2],char board[9][9])
{
    if((is_y_axis_move(sc,ec)==true&&is_y_axis_clean(board,sc,ec)==true)||(is_x_axis_move(sc,ec)==true&&is_x_axis_path_clean(board,sc,ec)==true))
    {
        return true;
    }
    return false;
}
// For validation of knight  move
bool  is_valid_knight_move(int sc[2],int ec[2],int turn)
{
    if(turn==1)
    {
        int rowdif=sc[0]-ec[0];
        int coldif=sc[1]-ec[1];
        if(rowdif==-2&&(coldif==1||coldif==-1))
        {
            return true;
        }
        else return false;
    }
    if(turn==2)
    {
        int rowdif=sc[0]-ec[0];
        int coldif=sc[1]-ec[1];
        if(rowdif==2&&(coldif==1||coldif==-1))
        {
            return true;
        }
        else return false;
    }
}
//For validation of bishup move
bool   is_valid_bishup_move(int sc[2],int ec[2],char board[9][9])
{
    return (( is_diagonal_path_clean( board,sc, ec))==true&&(is_diagonal_move( sc,ec))==true);
}
// For validation of silver move
bool  is_valid_silver_move(int sc[2],int ec[2],char board[9][9],int turn)
{
    if(turn==1)
    {
        return (sc[0]-ec[0]==-1&&sc[1]==ec[1]||abs(sc[1]-ec[1])==1&&abs(sc[0]-ec[0])==1);
    }
    if(turn==2)
    {
        return ((sc[0]-ec[0]==1&&sc[1]==ec[1])||abs(sc[1]-ec[1])==1&&abs(sc[0]-ec[0])==1);
    }
}
// For validation of gold move
bool  is_valid_Gold_move(int sc[2],int ec[2],char board[9][9],int& turn)
{
    if(turn==1)
    {
        if((is_x_axis_move(sc,ec)==true &&(sc[1]-ec[1]==1||sc[1]-ec[1]==-1))||(is_y_axis_move(sc,ec)==true&&(sc[0]-ec[0]==1||sc[0]-ec[0]==-1))||(sc[0]-ec[0]==-1&&sc[1]-ec[1]==-1)||(sc[0]-ec[0]==-1&&sc[1]-ec[1]==1))
        {
            return true;
        }
        return false;
    }
    if(turn==2)
    {
        if((is_x_axis_move(sc,ec)==true&&(sc[1]-ec[1]==1||sc[1]-ec[1]==-1))||(is_y_axis_move(sc,ec)==true&&(sc[0]-ec[0]==1||sc[0]-ec[0]==-1))||(sc[0]-ec[0]==1&&sc[1]-ec[1]==-1)||(sc[0]-ec[0]==1&&sc[1]-ec[1]==1))
        {
            return true;
        }
        return false;
    }
}
//For validation of lance move
bool  is_valid_lance_move(int sc[2],int ec[2],char board[9][9],int turn)
{
    if(turn==1)
    {
        if(sc[1]==ec[1]&&sc[0]<ec[0])
        {
            if(is_y_axis_clean( board, sc, ec)==true)
            {
                return true;
            }
        }
        return false;
    }
    if(turn==2)
    {
        if(sc[1]==ec[1]&&sc[0]>ec[0])
        {
            if(is_y_axis_clean( board, sc, ec)==true)
            {
                return true;
            }
        }
        return false;
    }
}
//For validation of king move
bool is_valid_king_move(int sc[2],int ec[2],char board[9][9])
{
    if(abs(sc[1]-ec[1])==1&&sc[0]==ec[0])
    {
        return true;
    }
    if(sc[1]==ec[1]&&abs(sc[0]-ec[0])==1)
    {
        return true;
    }
    if(abs(sc[1]-ec[1])==1&&abs(sc[0]-ec[0])==1)
    {
        if( is_diagonal_path_clean( board, sc, ec)==true)
        {
            return true;
        }
    }
    return false;
}
// For validation of move of every piece
bool is_valid_move(char board[9][9],int sc[2],int ec[2],int turn)
{
    char p_sym=board[sc[0]][sc[1]];
    switch (p_sym)
    {
    case 'P':
    case  'p':
        return    is_valid_pawn_move(sc,ec,board,turn);
        break;
    case 'r':
    case 'R':
        return  is_valid_rukh_move(sc,ec,board);
        break;
    case 'n':
    case 'N':
        return  is_valid_knight_move(sc,ec,turn);
        break;
    case 'b':
    case 'B':
        return is_valid_bishup_move(sc,ec,board);
        break;
    case 's':
    case 'S':
        return is_valid_silver_move(sc,ec,board,turn);
        break;
    case 'g':
    case 'G':
        return  is_valid_Gold_move(sc,ec,board,turn);
        break;
    case 'l':
    case 'L':
        return  is_valid_lance_move(sc,ec,board,turn);
        break;
    case 'k':
    case 'K':
        return is_valid_king_move(sc,ec,board);
        break;
    case 'w':
    case 'W':
    case 'a':
    case 'A':
    case 'v':
    case 'V':
    case 'T':
    case 't':
        return  is_valid_Gold_move(sc,ec,board,turn);
        break;
    case 'o':
    case 'O':
        return  is_valid_rukh_move(sc,ec,board)||is_valid_king_move(sc,ec,board);
        break;
    case 'H':
    case 'h':
        return is_valid_bishup_move(sc,ec,board)||is_valid_king_move(sc,ec,board);
        break;
    }
}
//For Check
bool check(char board[9][9],int turn,int kc[2],int mpc[2])
{
    if(turn==1)
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                mpc[0]=i;
                mpc[1]=j;
                if(iscapital(mpc,board)==true)
                {
                    for(int k=0; k<9; k++)
                    {
                        for(int l=0; l<9; l++)
                        {
                            kc[0]=k;
                            kc[1]=l;
                            if(is_valid_move(board,mpc,kc,turn) && board[k][l]=='k')
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
    if(turn==2)
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                mpc[0]=i;
                mpc[1]=j;
                if(issmall(mpc,board))
                {
                    for(int k=0; k<9; k++)
                    {
                        for(int l=0; l<9; l++)
                        {
                            kc[0]=k;
                            kc[1]=l;
                            if(is_valid_move(board,mpc,kc,turn) && board[k][l]=='K')
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
}
//For Capturing
void capturing1(char &sym,char capturing_array1[20],int  &counter1, int &counter2,int &counter3, int &counter4, int &counter5, int &counter6,int& counter7)
{
    int i=0;
    switch(sym)
    {
    case 'p':
    case 'w':
        sym='P';
        capturing_array1[i]=sym;
        i++;
        gotoRowCol(20,142);
        cout<<++counter1<<" ";
        break;
    case 'r':
    case 'o':
        sym='R';
        capturing_array1[i]=sym;
        i++;
        gotoRowCol(28,142);
        cout<<++counter2<<" ";
        break;
    case 'b':
    case 'h':
        sym='B';
        capturing_array1[i]=sym;
        i++;
        gotoRowCol(36,142);
        cout<<++counter3<<" ";
        break;
    case 'n':
    case 't':
        sym='N';
        capturing_array1[i]=sym;
        i++;
        gotoRowCol(43,142);
        cout<<++counter4<<" ";
        break;
    case 'g':
        sym='G';
        capturing_array1[i]=sym;
        i++;
        gotoRowCol(52,142);
        cout<<++counter5<<" ";
        break;
    case 'l':
    case 'a':
        sym='L';
        capturing_array1[i]=sym;
        i++;
        gotoRowCol(60,142);
        cout<<++counter6<<" ";
        break;
    case 's':
    case 'v':
        sym='S';
        capturing_array1[i]=sym;
        i++;
        gotoRowCol(68,142);
        cout<<++counter7<<" ";
        break;
    }
    cout<<capturing_array1[i-1];

}
//For Capturing
void capturing2(char &sym,char capturing_array2[20],int  &count1, int &count2, int &count3,  int &count4,int &count5,int &count6, int &count7)
{
    int i=0;
    char array2[20]= {' '};
    switch(sym)
    {
    case 'P':
    case 'W':
        sym='p';
        capturing_array2[i]=sym;
        i++;
        gotoRowCol(20,152);
        cout<<++count1<<" ";
        break;
    case 'R':
    case 'O':
        sym='r';
        capturing_array2[i]=sym;
        i++;
        gotoRowCol(28,152);
        cout<<++count2<<" ";
        break;
    case 'B':
    case 'H':
        sym='b';
        capturing_array2[i]=sym;
        i++;
        gotoRowCol(36,152);
        cout<<++count3<<" ";
        break;
    case 'N':
    case 'T':
        sym='n';
        capturing_array2[i]=sym;
        i++;
        gotoRowCol(43,152);
        cout<<++count4<<" ";
        break;
    case 'G':
        sym='g';
        capturing_array2[i]=sym;
        i++;
        gotoRowCol(52,152);
        cout<<++count5<<" ";
    case 'L':
    case 'A':
        sym='l';
        capturing_array2[i]=sym;
        i++;
        gotoRowCol(60,152);
        cout<<++count6<<" ";
        break;
    case 'S':
    case 'V':
        sym='s';
        capturing_array2[i]=sym;
        i++;
        gotoRowCol(68,152);
        cout<<++count7<<" ";
        break;
    }
    cout<<capturing_array2[i-1];
}
//For chang position
void changeposition(int sc[2],int ec[2],char board[9][9],int turn,char capturring_array1[20],char capturring_array2[20],char &sym1,char& sym2, int  &counter1, int &counter2,int &counter3,  int &counter4, int &counter5, int &counter6, int& counter7, int  &count1, int &count2,int &count3,int &count4, int &count5,int &count6,int &count7)
{
    if(turn==1)
    {

        char sym1=board[ec[0]][ec[1]];
        capturing1(sym1,capturring_array1,counter1,counter2,counter3, counter4,counter5,counter6,counter7);
        board [ec[0]][ec[1]]=board[sc[0]][sc[1]];
        board [sc[0]][sc[1]]='-';
    }
    if(turn==2)
    {
        char sym2=board[ec[0]][ec[1]];
        capturing2(sym2,capturring_array2,count1,count2,count3, count4,count5,count6,count7);
        board [ec[0]][ec[1]]=board[sc[0]][sc[1]];
        board [sc[0]][sc[1]]='-';
    }
}
bool check_mate(char board[9][9],int turn)
{
    if(turn==1)
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                if(board[i][j]=='k')
                {
                    return false;
                }
            }
        }
    }
    else if(turn==2)
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                if(board[i][j]=='K')
                {
                    return false;
                }
            }
        }
    }
    return true;
}
// For Promotion
void promotion (char board[9][9],int end_coordiinate[2],int turn)
{
    char sym;
    if(turn==1)
    {
        if(end_coordiinate[0]==6||end_coordiinate[0]==7||end_coordiinate[0]==8)
        {
            for(int i=6; i<9; i++)
            {
                for(int j=0; j<9; j++)
                {
                    if(board[i][j]=='S'||board[i][j]=='R'||board[i][j]=='B')
                    {
                        gotoRowCol(82,45);
                        cout<<"Y || y for promotion and n || N for not";
                        gotoRowCol(82,95);
                        cin>>sym;
                        if(sym=='Y'||sym=='y')
                        {
                            switch (board[i][j])
                            {
                            case 'R':
                                board[i][j]='O';
                                break;
                            case 'B':
                                board[i][j]='H';
                                break;
                            case 'S':
                                board[i][j]='V';
                                break;
                            }
                        }
                        else gotoRowCol(82,45);
                        cout<<"                                       ";
                        gotoRowCol(82,95);
                        cout<<"  ";
                    }
                }
            }
        }
        if(end_coordiinate[0]==6||end_coordiinate[0]==7)
        {
            for(int r=6; r<8; r++)
            {
                for(int c=0; c<9; c++)
                {
                    if(board[r][c]=='P'||board[r][c]=='L')
                    {
                        gotoRowCol(82,45);
                        cout<<"Y || y for promotion and n || N for not";
                        gotoRowCol(82,95);
                        cin>>sym;
                        if(sym=='Y'||sym=='y')
                        {
                            switch (board[r][c])
                            {
                            case 'P':
                                board[r][c]='W';
                                break;
                            case 'L':
                                board [r][c]='A';
                                break;
                            }
                        }
                    }
                    else gotoRowCol(82,45);
                    cout<<"                                       ";
                    gotoRowCol(82,95);
                    cout<<" ";
                }
            }
        }
        if(end_coordiinate[0]==6)
        {
            for(int c=0; c<9; c++)
            {
                if(board[6][c]=='N')
                {
                    gotoRowCol(82,45);
                    cout<<"Y || y for promotion and n || N for not";
                    gotoRowCol(82,95);
                    cin>>sym;
                    if(sym=='Y'||sym=='y')
                    {
                        switch (board[6][c])
                        {
                        case 'N':
                            board[6][c]='T';
                            break;
                        }
                    }
                }
                else gotoRowCol(82,45);
                cout<<"                                       ";
                gotoRowCol(82,95);
                cout<<" ";
            }
        }
        for(int c=0; c<9; c++)
        {
            if(board[8][c]=='L'||board[8][ c]=='P'||board[8][ c]=='N'||board[7][ c]=='N')
            {
                {
                    switch(board[8][c])
                    {
                    case 'P':
                        board[8][c]='W';
                        break;
                    case 'L':
                        board [8][c]='A';
                        break;
                    case 'N':
                        board[8][c]='T';
                        break;
                    }
                    switch(board[7][c])
                    {
                    case 'N':
                        board[7][c]='T';
                        break;
                    }

                }
            }
        }
    }
    if(turn==2)
    {
        if(end_coordiinate[0]==0||end_coordiinate[0]==1||end_coordiinate[0]==2)
        {
            for(int r=0; r<3; r++)
            {
                for(int c=0; c<9; c++)
                {
                    if(board[r][c]=='s'||board[r][c]=='r'||board[r][c]=='b')
                    {
                        gotoRowCol(82,45);
                        cout<<"Y || y for promotion and n || N for not";
                        gotoRowCol(82,95);
                        cin>>sym;
                        if(sym=='Y'||sym=='y')
                        {
                            switch (board[r][c])
                            {
                            case 'r':
                                board[r][c]='o';
                                break;
                            case 'b':
                                board[r][c]='h';
                                break;
                            case 's':
                                board[r][c]='v';
                                break;
                            }
                        }
                    }
                    else gotoRowCol(82,45);
                    cout<<"                                        ";
                    gotoRowCol(82,95);
                    cout<<" ";
                }
            }
        }
        if(end_coordiinate[0]==1||end_coordiinate[0]==2)
        {
            for(int r=1; r<3; r++)
            {
                for(int c=0; c<9; c++)
                {
                    if(board[r][c]=='p'||board[r][c]=='l')
                    {
                        gotoRowCol(82,45);
                        cout<<"Y || y for promotion and n || N for not";
                        gotoRowCol(82,95);
                        cin>>sym;
                        if(sym=='Y'||sym=='y')
                        {
                            switch (board[r][c])
                            {
                            case 'p':
                                board[r][c]='w';
                                break;
                            case 'l':
                                board [r][c]='a';
                                break;
                            }
                        }

                    }
                    else gotoRowCol(82,45);
                    cout<<"                                        ";
                    gotoRowCol(82,95);
                    cout<<" ";
                }
            }
        }
        if(end_coordiinate[0]==2)
        {
            for(int c=0; c<9; c++)
            {
                if(board[2][c]=='n')
                {
                    gotoRowCol(82,45);
                    cout<<"Y || y for promotion and n || N for not";
                    gotoRowCol(82,95);
                    cin>>sym;
                    if(sym=='Y'||sym=='y')
                    {
                        switch (board[2][c])
                        {
                        case 'n':
                            board[2][c]='t';
                            break;
                        }
                    }

                }
                else gotoRowCol(82,45);
                cout<<"                                       ";
                gotoRowCol(82,95);
                cout<<" ";
            }
        }
        for(int c=0; c<9; c++)
        {
            if(board[0][c]=='l'||board[0][ c]=='p'||board[0][c]=='n'||board[1][c]=='n')
            {
                switch(board[0][c])
                {
                case 'p':
                    board[0][c]='w';
                    break;
                case 'l':
                    board [0][c]='a';
                    break;
                case 'n':
                    board[0][c]='t';
                    break;
                }
                switch(board[1][c])
                {
                case 'n':
                    board[1][c]='t';
                    break;
                }
            }
        }
    }
}
// For change turn of player
void turnchange(int &turn,string player_name[2])
{
    switch (turn)
    {
    case 1:
        turn=2;
        gotoRowCol(2,100);
        cout<<"Now,Player"<<" "<<player_name[1]<<" "<<"turn"<<endl<<endl;
        break;
    case 2:
        turn=1;
        gotoRowCol(2,100);
        cout<<"Now,Player"<<" "<<player_name[0]<<" "<<"turn"<<endl<<endl;
        break;
    }
}
//For check himself piece
bool ismypiece(int& turn,int startcoordinate[2],char board[9][9])
{
    if(turn==1&&iscapital(startcoordinate,board)==true)
    {
        return true;
    }
    if(turn==2&&issmall(startcoordinate,board)==true)
    {
        return true;
    }
    else return false;
}
// For check end-co-ordinate
bool is_valid_destination(int turn,int endcoordinate[2],char board[9][9])
{
    if(turn==1&&iscapital(endcoordinate,board)==true)
    {
        return false;
    }
    if(turn==2&&issmall(endcoordinate,board)==true)
    {
        return false;
    }
    else return true;
}
bool is_pawn_dropping_move(char board[9][9],int turn,int col)
{
    if(turn==1)
    {
        for(int i=0; i<9; i++)
        {
            if(board[i][col]=='P')
            {
                return false;
            }
        }
        return true;
    }
    if(turn==2)
    {
        for(int j=0; j<9; j++)
        {
            if(board[j][col]=='p')
            {
                return false;
            }
        }
        return true;
    }
}


//For print cad board
void print_cad_board(int r_dim,int c_dim,char cad[8][2])
{
    for(int row=1; row<9; row++)
    {
        for(int col=13; col<16; col++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4+col);
            Print_box(r_dim,c_dim,row*r_dim,col*c_dim,-37);

        }
    }
    gotoRowCol(10,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Piece";
    gotoRowCol(20,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Pawn";
    gotoRowCol(28,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Rook";
    gotoRowCol(36,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Bishup";
    gotoRowCol(43,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Knight";
    gotoRowCol(52,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Gold";
    gotoRowCol(60,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Lance";
    gotoRowCol(68,133);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"silver";
    gotoRowCol(10,142);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
    cout<<"Player1";
    gotoRowCol(10,152);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    cout<<"player2";
    gotoRowCol(11,90);
    cout<<"Undo";
    gotoRowCol(19,90);
    cout<<"Redo";
    gotoRowCol(28,90);
    cout<<"Replay";
}
//For Dropping
void dropping(char board[9][9],int &turn,char capturing_array1[20],char capturing_array2[20],string player_name[2],int rdim,int cdim,char cad[8][2],bool &drop)
{
    if(turn==1)
    {
        char piece;
        int piece_quardinate[2];
        static int  counter1=1,counter2=1,counter3=1, counter4=1,counter5=1,counter6=1,counter7=1;
        gotoRowCol(82,100);
        cout<<"player"<<player_name[0]<<"which piece do you want to drop it";
        cin>>piece;
        gotoRowCol(82,100);
        cout<<"                                                             ";
        for(int i=0; i<20; i++)
        {
            if(capturing_array1[i]==piece)
            {
                gotoRowCol(82,100);
                cout<<"please enter co-ordinate which you want to put it";
                cin>>piece_quardinate[0]>>piece_quardinate[1];
                gotoRowCol(82,100);
                cout<<"                                                    ";
                if(piece=='P'&&piece_quardinate[0]<=7&&board[piece_quardinate[0]][piece_quardinate[1]]=='-'&&is_pawn_dropping_move(board,turn,piece_quardinate[1])==true)
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]=piece;
                    drop=true;
                    gotoRowCol(20,142);
                    cout<<--counter1;
                }
                if(piece=='R'&& board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(28,142);
                    cout<<--counter2;
                }
                if(piece=='N' && piece_quardinate[0]<=6 && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(43,142);
                    cout<<--counter4;
                }
                if(piece=='S' && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(68,142);
                    cout<<--counter7;
                }
                if(piece=='G' && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(52,142);
                    cout<<--counter5;
                }
                if(piece=='L' && piece_quardinate[0]<=7 && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(60,142);
                    cout<<--counter6;
                }
                if(piece=='B' && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(36,142);
                    cout<<--counter3;
                }
            }
        }
    }
    if(turn==2)
    {
        char piece;
        int piece_quardinate[2];
        static int  counter1=1,counter2=1,counter3=1, counter4=1,counter5=1,counter6=1,counter7=1;
        gotoRowCol(82,100);
        cout<<"player"<<player_name[1]<<"which piece do you want to drop it";
        cin>>piece;
        gotoRowCol(82,100);
        cout<<"                                                             ";
        for(int i=0; i<20; i++)
        {
            if(capturing_array2[i]==piece)
            {
                gotoRowCol(82,100);
                cout<<"please enter co-ordinate which you want to put it";
                cin>>piece_quardinate[0]>>piece_quardinate[1];
                gotoRowCol(82,100);
                cout<<"                                                      ";
                if(piece=='p'&&piece_quardinate[0]>=1&&board[piece_quardinate[0]][piece_quardinate[1]]=='-'&&is_pawn_dropping_move(board,turn,piece_quardinate[1])==true)
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]=piece;
                    drop=true;
                    gotoRowCol(20,152);
                    cout<<--counter1;
                }
                if(piece=='r'&& board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(28,152);
                    cout<<--counter2;
                }
                if(piece=='n' && piece_quardinate[0]>=2&& board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(43,152);
                    cout<<--counter4;
                }
                if(piece=='s' && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(68,152);
                    cout<<--counter7;
                }
                if(piece=='g' && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(52,152);
                    cout<<--counter5;
                }
                if(piece=='l' && piece_quardinate[0]>=1 && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(60,152);
                    cout<<--counter6;
                }
                if(piece=='b' && board[piece_quardinate[0]][piece_quardinate[1]]=='-')
                {
                    board[piece_quardinate[0]][piece_quardinate[1]]= piece;
                    drop=true;
                    gotoRowCol(36,152);
                    cout<<--counter3;
                }
            }
        }
    }
}
// For initiallize the array
void initialize(char board[9][9],char capturing_array1[20],char capturing_array2[20])
{
    //For Pawn
    for(int col=0; col<9; col++)
    {
        board [2][col]='P';
        board [6][col]='p';
    }
//    For Slashes
    for(int row=3 ; row<=5 ; row++ )
    {
        for(int col=0 ; col<9 ; col++ )
        {
            board [row][col]='-';
        }
    }
    capturing_array1[20]= {' '};
    capturing_array2[20]= {' '};
    board [0][0]=board [0][8]='L';
    board [0][1]=board [0][7]='N';
    board [0][2]=board [0][6]='S';
    board [0][3]=board [0][5]='G';
    board [0][4]='K';
    board [1][7]='B';
    board [1][1]='R';
    board [8][2]=board [8][6]='s';
    board [8][0]=board [8][8]='l';
    board [8][3]=board [8][5]='g';
    board [8][1]=board [8][7]='n';
    board [8][4]='k';
    board [7][1]='b';
    board [7][7]='r';
    board[1][0]=board[1][2]=board[1][2]=board[1][3]=board[1][4]=board[1][5]=board[1][6]=board[1][8]=board[7][0]=board[7][2]=board[7][2]=board[7][3]=board[7][4]=board[7][5]=board[7][6]=board[7][8]='-';
}
//For Highlight Box
void highlightbox( char board[9][9],int sc[2],int ec[2],int turn,int r_dim,int c_dim)
{
    for(int r=0; r<9; r++)
    {
        for(int c=0; c<9; c++)
        {
            ec[0]=r;
            ec[1]=c;
            if(!ismypiece(turn,ec,board)&&is_valid_move(board,sc,ec,turn))
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
               Print_box(r_dim,c_dim,r*r_dim,c*c_dim,-37);
            }
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);

}
// For set arrow keys
void selectPiece(int stcor[2],int &cr,int &cc,int rdim,int cdim,char board[9][9],int& turn,char capturing_array1[20],char capturing_array2[20],string player_name[2],char cad[8][2])
{
    bool pieceselect=false;
    bool drop=false;
    int counter=0;
    int counter1=0;
    while(pieceselect==false)
    {
        if (kbhit())
        {
            int key=getch();
            if(key==224)
            {
                key=getch();
                switch(key)
                {
                case 72:    //For Move Up
                {
                    cr--;
                    if(cr<0)
                        cr=8;
                    break;
                }
                case 75:        //For Move Left
                {
                    cc--;
                    if(cc<0)
                        cc=8;
                    break;
                }
                case 77:        //For Move Right
                {
                    cc++;
                    if(cc>8)
                        cc=0;
                    break;
                }
                case 80:       //For Move Down
                {
                    cr++;
                    if(cr>8)
                        cr=0;
                    break;
                }
                }
                gotoRowCol(8,100);
                cout<<"Current Row:"<<cr;
                gotoRowCol(10,100);
                cout<<"Current Col:"<<cc;
            }
            if(key==13)
                pieceselect=true;
            if(key=='D'||key=='d')
            {
                dropping(board,turn,capturing_array1,capturing_array2,player_name,rdim,cdim,cad,drop);
                if(drop==true)
                {
                    promotion(board,stcor,turn);
                    Print_board( rdim, cdim, board);
                    turnchange(turn,player_name);
                }
            }
            if((key=='U'||key=='u'))
            {
                undo(board,rdim,cdim);
                counter++;
            }
            if(key=='R'||key=='r')
            {
                while(counter>counter1)
                {
                    redo(board,rdim,cdim);
                    counter1++;
                }
            }
        }
        stcor[0]=cr;
        stcor[1]=cc;
        gotoRowCol((cr)*rdim + rdim/2,(cc)*cdim + cdim/2);
    }
}
//For Load Game
void load_game(char board[9][9],int &turn,string player_name[2])
{
    ifstream reader("file.txt");
    reader>>turn;
    turnchange(turn,player_name);
    char sym;
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            reader>>sym;
            board[i][j]=sym;
        }
    }
}
//For Save Game
void save_game(char board[9][9],int &turn)
{
    ofstream writer("file.txt");
    writer<<turn<<endl;
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            writer << board[i][j] ;
        }
        writer<< endl;
    }
}
int main()
{
    int rows=85,cols=100;
    int r_dim=rows/10,c_dim=cols/10;
    string  player_names[2];
    int turn=2,cr=4,cc=4;
    int start_coordinate[2];
    int end_coordinate[2];
    char sym,sym1,sym2;
    char board[9][9];
    char cad[8][2];
    int kc[2];
    int i=0;
    int j=0;
    bool drop=false;
    int mpc[2];
    char capturring_array1[20]= {' '};
    char capturring_array2[20]= {' '};
    static int  counter1=0,counter2=0,counter3=0, counter4=0,counter5=0,counter6=0,counter7=0;
    static int  count1=0,count2=0,count3=0, count4=0,count5=0,count6=0,count7=0;\
     gotoRowCol(35,70);
    cout<<"Welcom to the Shogi game";
    Sleep(1800);
    system("cls");
    gotoRowCol(0,100);
    cout<<"please enter First player Name";
    cin>>player_names[0];
    gotoRowCol(4,100);
    cout<<"please enter Second player Name";
    cin>>player_names[1];
    gotoRowCol(8,100);
    cout<<"L || l for load game and n ||N for new game";
    gotoRowCol(8,144);
    cin>>sym;
    system("cls");
    if((sym=='l')||(sym=='L'))
    {
        load_game(board,turn,player_names);
        Print_board(r_dim,c_dim,board);
        print_cad_board(r_dim,c_dim,cad);
    }
    else if(sym=='N'||sym=='n')
    {
        gotoRowCol(16,100);
        cout<<"                                           ";
        gotoRowCol(18,100);
        cout<<" ";
        initialize(board,capturring_array1,capturring_array2);
        print_cad_board(r_dim,c_dim,cad);
    }
    do
    {
        Print_board(r_dim,c_dim,board);
        initiallize3d( board);
        turnchange(turn,player_names);
//
        do
        {
            gotoRowCol(5,100);
            cout<<"Please enter first co-ordinates";
//            getRowColbyLC(i,j);
//            start_coordinate[0]=i/r_dim;
//            start_coordinate[1]=j/c_dim;
             selectPiece(start_coordinate,cr,cc,r_dim,c_dim, board,turn,capturring_array1,capturring_array2,player_names,cad);
            gotoRowCol(5,100);
            cout<<"                               ";
            if(start_coordinate[1]==9)
            {
                replayredo(board,start_coordinate,r_dim,c_dim);
            }
        }
        while((ismypiece(turn,start_coordinate,board))==false);
        highlightbox(board,start_coordinate,end_coordinate,turn,r_dim,c_dim);
        do
        {
            gotoRowCol(5,100);
            cout<<"Please enter end co-ordinates";
//            getRowColbyLC(i,j);
//            end_coordinate[0]=i/r_dim;
//            end_coordinate[1]=j/c_dim;
            selectPiece(end_coordinate,cr,cc,r_dim,c_dim,board,turn,capturring_array1,capturring_array2,player_names,cad);
            gotoRowCol(5,100);
            cout<<"                             ";
        }
        while(( is_valid_destination(turn,end_coordinate,board)==false));
        if(is_valid_move(board,start_coordinate,end_coordinate,turn)==false)
        {
            turnchange(turn,player_names);
            continue;
        }
//        if(self_check(board,start_coordinate,end_coordinate,turn,capturring_array1,capturring_array2,sym1,sym2,counter1,counter2,counter3, counter4,counter5,counter6,counter7, count1,count2,count3, count4,count5,count6,count7,kc,mpc)==false)
//        {
//           turnchange(turn,player_names);
//           continue;
//        }
        else
            changeposition(start_coordinate,end_coordinate,board,turn,capturring_array1,capturring_array2,sym1,sym2,counter1,counter2,counter3, counter4,counter5,counter6,counter7, count1,count2,count3, count4,count5,count6,count7);
        promotion(board,end_coordinate,turn);
        save_game(board,turn);
        check(board,turn,kc,mpc);
        if(check(board,turn,mpc,kc)==true&&turn==1)
        {
            gotoRowCol(3,118);
            cout<<"Now,player 2 king is in check";
        }
        else
        {
            gotoRowCol(3,118);
            cout<<"                             ";
        }



        if(check(board,turn,mpc,kc)==true&&turn==2)
        {
            gotoRowCol(2,118);
            cout<<"Now,player 1 king is in check";
        }
        else
        {
            gotoRowCol(2,118);
            cout<<"                             ";
        }
    }
    while (check_mate(board,turn)==false);
    if(check_mate(board,turn)==true)
    {
        system("cls");
        if(turn==1)
        {
            gotoRowCol(rows/2,cols/2+20);
            cout<<"Player"<<" "<<turn<<" "<<"is won";
        }
        if(turn==2)
        {
            gotoRowCol(rows/2,cols/2+20);
            cout<<"Player"<<" "<<turn<<" "<<"is won";
        }
    }
}

