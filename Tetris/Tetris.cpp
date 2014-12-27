/* 
    Title : Tetris 
    Author: nyist_xiaod 
    Date  : 2013.1.3 
*/  
#include <vector>  
#include <time.h>  
#include <string>  
#include <conio.h>  
#include <iostream>  
#include <windows.h>  
using namespace std;  
  
const int N = 22;                       //游戏栏&&信息栏的行数  
const int M = 12;                       //游戏栏的列数  
const int MM = 6;                       //信息栏的列数  
  
const string square = "■";  
const string space  = "  ";  
const string line[] = {"┄","┆"};  
const string corner[] = {"┌","┐","└" ,"┘"};  
  
string g[N][M],gg[N][MM];               //g为游戏栏,gg为信息栏  
  
struct Point  
{  
    int x,y;  
    Point(){}  
    Point(int _x,int _y):x(_x),y(_y){}  
    Point operator++(int)  
    {  
        y++;  
        return (*this);  
    }  
    Point operator--(int)  
    {  
        y--;  
        return (*this);  
    }  
    Point operator++()  
    {  
        x++;  
        return (*this);  
    }  
    bool operator==(const Point& B)  
    {  
        return x==B.x && y==B.y;  
    }  
};  
  
void SetCursor(int x,int y)             //设置光标位置  
{  
    COORD cd = {x,y};  
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),cd);  
}  
  
void SetCursor(const Point& p)          //将数组中的位置映射到屏幕上  
{  
    SetCursor(2*p.y,p.x);  
}  
  
void HideCursor()                       //隐藏光标  
{  
     HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);  
     CONSOLE_CURSOR_INFO ConsoleCursorInfo;  
     GetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);  
     ConsoleCursorInfo.bVisible = FALSE;  
     SetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);  
}  
  
template<typename T>  
void PrintAt(const Point& p,T data,int wid=0)                   //在点p处输出data  
{  
    SetCursor(p);  
    if(wid)  
        cout.width(wid);  
    cout<< data;  
}  
  
void MagicShow(const Point& p,bool flag=false,bool info=false)  //在点p处输出/擦除方块  
{  
    if(info || g[p.x][p.y]==space)  
        PrintAt(p ,flag ? square : space);  
}  
  
class Game;  
  
class Block  
{  
    friend class Game;                  //声明Game类为友元类  
    private:  
        static string all_state[7][4];  //标记所有方块的状态  
        int id,dir;                     //id表示种类,dir表示方向  
    public:  
        Block();  
        Point p;                        //标记4*4数组的左上角坐标  
        bool state[4][4];               //用4*4的数组标记自己的状态  
        static void State_Init();       //初始化所有方块的状态  
        void StateCpy();                //将自己的状态与id和dir对应  
        void Rotate();                  //旋转  
        void Show(bool);                //将方块在屏幕上输出  
};  
  
string Block::all_state[7][4];          //初始化静态变量  
  
void Block::State_Init()  
{  
    //I  
    all_state[0][0] = all_state[0][2] = "0100 0100 0100 0100";  
    all_state[0][1] = all_state[0][3] = "0000 1111 0000 0000";  
    //O  
    all_state[1][0] = all_state[1][1] = "0000 0110 0110 0000";  
    all_state[1][2] = all_state[1][3] = "0000 0110 0110 0000";  
    //S  
    all_state[2][0] = all_state[2][2] = "0000 0110 1100 0000";  
    all_state[2][1] = all_state[2][3] = "0100 0110 0010 0000";  
    //Z  
    all_state[3][0] = all_state[3][2] = "0000 1100 0110 0000";  
    all_state[3][1] = all_state[3][3] = "0100 1100 1000 0000";  
    //T  
    all_state[4][0] = "0000 1110 0100 0000";  
    all_state[4][1] = "0100 1100 0100 0000";  
    all_state[4][2] = "0100 1110 0000 0000";  
    all_state[4][3] = "0100 0110 0100 0000";  
    //J  
    all_state[5][0] = "0100 0100 1100 0000";  
    all_state[5][1] = "1000 1110 0000 0000";  
    all_state[5][2] = "1100 1000 1000 0000";  
    all_state[5][3] = "0000 1110 0010 0000";  
    //L  
    all_state[6][0] = "0100 0100 0110 0000";  
    all_state[6][1] = "0000 1110 1000 0000";  
    all_state[6][2] = "1100 0100 0100 0000";  
    all_state[6][3] = "0010 1110 0000 0000";  
}  
  
Block::Block()  
{  
    id = rand()%7;                      //随机生成块的种类  
    dir = rand()%4;                     //随机生成块的方向  
    p = Point(4,13);                    //初始化点p的位置  
    StateCpy();  
}  
  
void Block::StateCpy()  
{  
    for(int i=0,add=0;i<4;i++,add++)  
        for(int j=0;j<4;j++)  
            state[i][j] = all_state[id][dir][i*4+j+add]=='1';   //给每个方块的状态赋值  
}  
  
void Block::Rotate()  
{  
    dir = (dir+1)%4;  
    StateCpy();  
}  
  
void Block::Show(bool info=false)           //info表示是否是信息栏,下同  
{  
    for(int i=0;i<4;i++)  
        for(int j=0;j<4;j++)  
            MagicShow(Point(p.x+i,p.y+j),state[i][j],info);  
}  
  
class Game  
{  
    private:  
        static int level_up[6];             //各level对应的分数  
        static int score_up[5];             //消除各行对应要加的分数  
        static int tick_cnt[6];             //各level对应的cnt数  
        int level,score;  
    public:  
        Block runB,nextB;  
        Game();  
        int GetTickCnt();                   //得到当前level的cnt数  
        void ShowFrame();                   //输出游戏框架  
        void CheckBoard();                  //检查键盘响应  
        bool AllSquare(int);                //检查xx行是否可消  
        void DropDown(int);                 //将xx行以上的全部下移一格  
        void CheckLine();                   //方块安放后检查是否有可消行  
        void PlaceOn();                     //方块安放好  
        void Update(int);                   //更新信息  
        void GetNext();                     //得到下一个方块  
        inline bool OutBoard(int,int);      //检查坐标是否出界  
        bool CanChange(int,int);            //检查是否可以发生这样的改变  
        bool ChangePos(int,int);            //检查并改变下落方块的位置  
        void Rotate();                      //旋转  
        void Begin();                       //游戏欢迎界面  
        void Over();                        //游戏结束界面  
};  
  
int Game::level_up[6]={0,80,180,290,410,666};  
int Game::score_up[5]={0,10,30,60,100};  
int Game::tick_cnt[6]={0,50,40,30,20,10};  
  
Game::Game():level(1),score(0)  
{  
    for(int i=0;i<N;i++)  
    {  
        for(int j=0;j<M;j++)  
            g[i][j] = "  ";  
        for(int j=0;j<MM;j++)  
            gg[i][j] = "  ";  
    }  
    for(int i=1;i<M;i++)  
        g[0][i] = g[N-1][i] = line[0];  
    for(int i=1;i<MM;i++)  
        gg[0][i] = gg[N-1][i] = line[0];  
    for(int i=1;i<N;i++)  
        g[i][0] = g[i][M-1] = gg[i][0] = gg[i][MM-1] = line[1];  
    g[0][0] = gg[0][0] = corner[0];  
    g[0][M-1] = gg[0][MM-1] = corner[1];  
    g[N-1][0] = gg[N-1][0] = corner[2];  
    g[N-1][M-1] = gg[N-1][MM-1] = corner[3];  
}  
  
int Game::GetTickCnt()  
{  
    return tick_cnt[level];  
}  
  
void Game::ShowFrame()  
{  
    for(int i=0;i<N;i++)  
    {  
        for(int j=0;j<M;j++)  
            cout << g[i][j];  
        for(int j=0;j<MM;j++)  
            cout << gg[i][j];  
        cout << endl;  
    }  
    PrintAt(Point(2,13)," Next");  
    PrintAt(Point(9,13)," Level");  
    PrintAt(Point(10,14),level,2);  
    PrintAt(Point(12,13)," Score");  
    PrintAt(Point(13,14),score,2);  
    PrintAt(Point(20,13)," @Xiaod");  
}  
  
bool Game::AllSquare(int xx)  
{  
    for(int j=1;j<M-1;j++)  
        if(g[xx][j] != square)  
            return false;  
    return true;  
}  
  
void Game::DropDown(int xx)  
{  
    for(int i=xx;i>1;i--)  
        for(int j=1;j<M-1;j++)  
            g[i][j] = g[i-1][j];  
    for(int j=1;j<M-1;j++)  
        g[1][j] = space;  
}  
  
void Game::Update(int flash_line_cnt)  
{  
    score += score_up[flash_line_cnt];  
    PrintAt(Point(13,14),score);  
    if(level<6 && score>=level_up[level])  
        PrintAt(Point(10,14),++level,2);  
}  
  
void Game::CheckLine()  
{  
    vector<int> flash_line;                                 //用来储存要消去的行号  
    vector<int>::iterator it;  
    for(int i=0;i<4;i++)  
        if(AllSquare(i+runB.p.x))  
            flash_line.push_back(i+runB.p.x);  
    if(flash_line.empty())  
        return ;  
    int flash_times = 5;  
    while(flash_times--)                                    //闪  
    {  
        for(it=flash_line.begin();it!=flash_line.end();it++)  
            for(int j=1;j<M-1;j++)  
                MagicShow(Point(*it,j),flash_times&1,true);  
        Sleep(60);  
    }  
    for(it=flash_line.begin();it!=flash_line.end();it++)    //消去某行后，将上面的信息向下移动  
        DropDown(*it);  
    it = flash_line.end()-1;  
    for(int i=1;i<=*it;i++)  
        for(int j=1;j<M-1;j++)  
            MagicShow(Point(i,j),g[i][j]==square,true);     //更新要消去的最后一行上方的信息  
    Update(flash_line.size());  
    flash_line.clear();  
}  
  
void Game::PlaceOn()  
{  
    for(int i=0;i<4;i++)  
        for(int j=0;j<4;j++)  
            if(runB.state[i][j])  
                g[runB.p.x+i][runB.p.y+j] = square;  
}  
  
void Game::GetNext()  
{  
    runB = nextB;  
    runB.p = Point(1,5);  
    nextB = Block();  
    nextB.Show(true);  
    runB.Show();  
}  
  
inline bool Game::OutBoard(int x,int y)  
{  
    return x<=0 || x>=N-1 || y<=0 || y>=M-1;  
}  
  
void Game::CheckBoard()  
{  
    char ch[2];  
    for(int i=0;i<2;i++)  
        if(kbhit())                                     //检测键盘是否按下  
        {  
            ch[i] = getch();                            //只能用getch函数接收  
            if(ch[0] != -32)  
            {  
                i--;  
                if(ch[0] == ' ')                        //按空格暂停游戏  
                    getch();  
                continue;  
            }  
            if(i == 1)  
            {  
                switch(ch[1])  
                {  
                    case 72:Rotate();break;             //上  
                    case 80:ChangePos(1,0);break;       //下  
                    case 75:ChangePos(0,-1);break;      //左  
                    case 77:ChangePos(0,1);break;       //右  
                }  
            }  
        }  
}  
  
bool Game::CanChange(int dx=0,int dy=0)  
{  
    const int x = runB.p.x , y = runB.p.y;  
    for(int i=0;i<4;i++)  
        for(int j=0;j<4;j++)  
            if(runB.state[i][j] && (OutBoard(i+x+dx,j+y+dy) || g[i+x+dx][j+y+dy]==square))  
                return false;  
    return true;  
}  
  
void Game::Rotate()  
{  
    runB.Rotate();  
    if(!CanChange())  
        for(int i=0;i<3;i++)  
            runB.Rotate();  
    runB.Show();  
}  
  
bool Game::ChangePos(int dx,int dy)  
{  
    if(!CanChange(dx,dy))  
        return false;  
    const int x = runB.p.x , y = runB.p.y;  
    if(dx == 1)                                     //处理最后那行的显示  
    {  
        for(int j=0;j<4;j++)  
            MagicShow(Point(x,y+j));  
        ++runB.p;  
    }  
    else if(dy == 1)                                //+1  
    {  
        for(int i=0;i<4;i++)  
            MagicShow(Point(x+i,y));  
        runB.p++;  
    }  
    else if(dy == -1)                               //+2  
    {  
        for(int i=0;i<4;i++)  
            MagicShow(Point(x+i,y+3));  
        runB.p--;  
    }  
    runB.Show();  
    return true;  
}  
  
void Game::Begin()  
{  
    ShowFrame();  
    nextB = Block();  
    GetNext();  
    int flash_times = 1000;  
    while(!kbhit() && flash_times--)  
    {  
        PrintAt( Point(9,1),flash_times&1 ? "                   " : " * * * * * * * * * ");  
        PrintAt(Point(10,1),flash_times&1 ? "  Anykey to start  " : "* Anykey to start *");  
        PrintAt(Point(11,1),flash_times&1 ? "                   " : " * * * * * * * * * ");  
        Sleep(100);  
    }  
    getch();                                        //anykey to start!  
    PrintAt( Point(9,1),"                   ");  
    PrintAt(Point(10,1),"                   ");  
    PrintAt(Point(11,1),"                   ");  
}  
  
void Game::Over()  
{  
    int flash_times = 12;  
    while(flash_times--)  
    {  
        PrintAt( Point(9,3),flash_times&1 ? "             " : " * * * * * * ");  
        PrintAt(Point(10,3),flash_times&1 ? "  Game over  " : "* Game over *");  
        PrintAt(Point(11,3),flash_times&1 ? "             " : " * * * * * * ");  
        Sleep(200);  
    }  
    PrintAt(Point(21,0),"");                        //把光标移到最后  
}  
  
int main()  
{  
    HideCursor();  
    srand(time(NULL));  
    Block::State_Init();  
    Game* pG = new Game();  
    pG->Begin();  
    while(1)  
    {  
        int tick_times = 0;  
        while(++tick_times < pG->GetTickCnt())      //控制下落时间间隔以及控制输出,控制输出的方法很奇妙  
        {  
            pG->CheckBoard();  
            Sleep(10);  
        }  
        if(!pG->ChangePos(1,0))  
        {  
            if(pG->runB.p == Point(1,5))            //如果一开始就下不去,game over.  
            {  
                pG->Over();  
                break;  
            }  
            pG->PlaceOn();  
            pG->CheckLine();  
            pG->GetNext();  
        }  
    }  
    delete pG;  
    return 0;  
}  