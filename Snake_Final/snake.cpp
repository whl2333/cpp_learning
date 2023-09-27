// Snake_WuhaoLun.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <graphics.h>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <conio.h>
#include <winddi.h>
#include <locale>
#include <codecvt>
#include <locale>
//其它头文件在这里补充
using namespace std;
#define HH 50  //待定
#define WW 80  //待定
#define MAXLEN 50  //待定
#define MAXH 50
#define MAXL 80

// -------------------- 数据设计 --------------------
/*
    map[i][j]表示游戏界面的一个坐标，
    如果是边界用0标记，
    如果是蛇身用1标记，
    如果是食物用2标记，
    如果是好道具用3标记，
    如果是坏道具用4标记
    如果是空地用5标记。
*/
int map[HH][WW];

//需要创建一个保存注册用户的文件user.txt，一个保存游戏记录的空文件record.txt

/*
    snakebody[i]表示第i节蛇的身体的坐标
    MAXLEN表示蛇的最大长度，下标0存储要移动到的下一个位置的坐标，这样会方便蛇移动的实现
    比如当前蛇的长度是3，对应的就是下标1~3的数组坐标，从下标4开始的数组就都赋值为-1
*/
int snakeBody[MAXLEN + 1][2] = { 0 };
//snakeBody[1][0] = 40;
// snakeBody[1][1] = 25;
/*
    表示蛇坐标的移动方向，从左到右分别对应上下左右
*/
const int direction[4][2] = { {-1, 0}, {1, 0} ,{0, -1}, {0, 1} };

/*
    表示移动方向，利用这个数据结构可以方便地读出当前是如何移动的，比如W对应0，而direciton[0]正好对应向上的坐标移动
*/
enum EnumDirection {
    W = 0, S, A, D
};

/*
    记录每一关出现道具所需要的间隔轮次，越到后面的关卡间隔轮次越少
*/
const int gap[] = { 0, 5, 4, 3 };

/*
    记录通过每一关所需要的食物数量,
*/
const int passStandard[] = { 0, 5, 10, 15 };

/*
    记录三关里每一关的初始障碍物数量
*/
const int barrierNum[] = { 0, 0, 2, 4 };

/*
    记录三关里每一关的初始速度
*/
const float initSpeed[] = { 0, 1, 1.5, 2 };

/*
    加速系数，默认为1，可在设置界面调整
*/
int acc = 1;

/*
    进入游戏界面时用于标识是暂停后进入的还是正常进入的，因为这会对应不同的初始化操作，默认是正常进入的
*/
int continueFlag = 0;

/*
    记录某个玩家某次游戏过程中的累计得分，用于恢复Player对象
*/
int gameScore = 0;

/*
    记录某个玩家某次游戏过程中的累计耗时，用于恢复Player对象
*/
int gameTime = 0;


/*
    len,x,y,eatenfood,direction都是全局变量，用于记录蛇的状态，有了这些状态就可在继续游戏后恢复原来的蛇
*/
int curLength = 3;
int curX = WW / 2, curY = HH / 2;
int curEatenFood = 0;
EnumDirection curSnakeDirection = EnumDirection::D; // 默认方向向右

bool ifFirstTime, ifEaten = false;


/*
    接受用户输入的用户名和密码，用于后续在屏幕上的显示
*/
std::string usernameToPrint;
std::string passwordToPrint;
wstring sToWs(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}
/*
    pageflag是全局变量，用于标识要进入的页面,比如HomePage就代表了首页
*/
enum EnumPages {
    HOME_PAGE = 0, REG_PAGE, LOG_PAGE, MENU_PAGE, INFO_PAGE,
    RANK_PAGE, STAGE_PAGE, SET_PAGE, GAME_PAGE, TOOL_PAGE, END_PAGE
}; EnumPages pageFlag;

/*
    textflag是全局变量，用于帮助合法性判别函数isLegal（）区分当前输入的字符串是账号还是密码，
    因为账号和密码的合法性判别规则是不同的。
*/
enum EnumTextAttr { USERNAME = 0, PASSWORD }; EnumTextAttr textFlag;

/*
    curstage全局变量，用于标识要进入第几关,因为每一关的参数不同所以需要标识
*/
enum EnumCurStage { FIRST_STAGE = 1, SECOND_STAGE, THIRD_STAGE }; EnumCurStage curStage;

constexpr auto BACKGROUND_IMAGE = L"background.jpg";
LPCTSTR bkHome = _T("./主界面.jpg");
LPCTSTR bkLog = _T("./登录界面.jpg");
LPCTSTR bkMenu = _T("./菜单界面.jpg");
LPCTSTR bkReg = _T("./注册界面.jpg");
LPCTSTR bkChoice = _T("./选关界面.jpg");
LPCTSTR bkSet = _T("./游戏设置界面.jpg");
LPCTSTR bkRank = _T("./排行榜界面.jpg");
LPCTSTR bkInfo = _T("./游戏介绍界面.jpg");
LPCTSTR bkOne = _T("./沙漠绿洲.jpg");
LPCTSTR bkTwo = _T("./黑色城镇.jpg");
LPCTSTR bkThree = _T("./卫星基地.jpg");
LPCTSTR bkPause = _T("./暂停界面.jpg");
LPCTSTR bkEnd = _T("./游戏结束界面.jpg");

class Widget
{
public:
    Widget(int x = 0, int y = 0, int width = 1024, int height = 640);
    void setRect(int x, int y, int width, int height);  // 设置控件矩形
    virtual void show() const = 0;                      // 显示函数(纯虚函数)

    int getX() const;
    int getY() const;
    int getHeight() const;
    int getWidth() const;
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);

protected:
    int x;        // X 坐标
    int y;        // Y 坐标
    int width;    // 宽度
    int height;   // 高度
};

class Button :public Widget {
public:
    Button(int x = 0, int y = 0, int width = 100, int height = 60, const std::wstring& text = L"按钮");
    void show() const;                      // 显示按钮
    bool state(const ExMessage& msg);       // 按钮状态，true 表示按钮按下
    const std::wstring& getText() const;    // 获取按钮文本
    void setText(const std::wstring& text); // 设置按钮文本

protected:
    bool isIn(const ExMessage& msg) const;  // 判断鼠标是否在按钮里面
    std::wstring text;                      // 按钮文本
    bool isChange;                          // 按钮状态是否改变
};

class Window :public Widget {
public:
    Window(int width = 1024, int height = 640);
    ~Window();

    void show() const;                          // 显示窗口
    //void messageLoop();                         // 消息循环
    void showHomePage();

    void showMenuView();
    void showRegPage();
    void showLogPage();
    void showChoicePage();
    void showSetPage();
    void showRankPage();
    void showInfoPage();
    void gameView(int gameScore, int gameTime, EnumCurStage cur_stage);
    void toolView();//游戏暂停界面
    void endPage();//游戏结束界面
    void printText(std::wstring text, int x, int y, int size);

public:
    // 窗口状态标识，每个状态代表了一个界面
    //enum class WindowState
    //{
    //	homePage,//主界面
    //	menuView,//菜单界面
    //	regPage,//注册界面
    //	logPage,//登录界面
    //	infoPage,//游戏介绍界面
    //	rankPage,//排行榜界面
    //	setPage,//游戏设置界面
    //	stagePage,//选择关卡界面
    //	pausePage,//游戏暂停界面
    //	gamePage,//游戏界面
    //	endPage,//游戏结束界面
    //	//onePage,//关卡一界面
    //	//twoPage,//关卡二界面
    //	//threePage,//关卡三界面
    //};

private:
    // home按钮
    Button* homePageLog;
    Button* homePageReg;
    Button* homePageExit;
    //登录界面按钮
    Button* logPageAccount;
    Button* logPagePassword;
    Button* logPageLog;
    Button* logPageQuit;
    // 注册界面按钮
    Button* regPageAccount;
    Button* regPagePassword;
    Button* regPageSure;
    Button* regPageQuit;
    //菜单界面按钮
    Button* menuViewInfo;
    Button* menuViewSet;
    Button* menuViewChoice;
    Button* menuViewRank;
    Button* menuViewQuit;
    //关卡选择界面按钮
    Button* choicePageOne;
    Button* choicePageTwo;
    Button* choicePageThree;
    Button* choicePageN;
    Button* choicePageQuit;
    //游戏设置界面按钮
    Button* setPageSpeed1;
    Button* setPageSpeed2;
    Button* setPageSpeed3;
    Button* setPageQuit;
    //排行榜界面按钮
    Button* rankPageQuit;
    //游戏介绍界面按钮
    Button* infoPageQuit;
    //游戏界面按钮
    Button* gamePageScore;
    Button* gamePageTime;
    Button* gamePagePause;
    //游戏暂停界面按钮
    Button* pausePageBack;
    Button* pausePageQuit;
    //游戏结束界面按钮
    Button* endPageRe;
    Button* endPageQuit;

    EnumPages state;  // 窗口状态
    HWND hWnd;          // 窗口句柄
};

Window w(1024, 640);

/*
    不同的关卡对应着不同的通关标准和初始移动速度等，所以有必要对不同关卡的参数进行管理
*/
class Stage
{
    friend class Snake;
    /*
    负责人：王海新 && 程自豪
     这个模块主要是设计三个关卡的内容，当标记被修改为第一关时执行第一关的内容，第三关时执行第三关内容。
     分配时是把一个时间多的和一个时间少的哥们分配到一起，这样两个人可以自由分配。
    */
private:
    int curStage; // 当前所在的关卡
    int propGap; // 每隔多少轮次生成一个道具
    int curLoop; //关卡内的轮次，蛇每吃掉一个道具/食物，轮次就要加1
    int stagePassStandard; //通过当前关卡所需吃到的食物数量
    int stageSpeed; //关卡初始的移动速度
    int stageBarrierNum; //当前关卡的障碍物数量，不同关卡的初始值不同，后续需要根据这个值设置

public:
    //构造函数，用于构造关卡对象
    Stage(int stage, int gap, int standard, int speed, int barNum)
        :curStage(stage), propGap(gap), stagePassStandard(standard), stageSpeed(speed), stageBarrierNum(barNum)
    {};

    //用于实现关卡参数的初始化
    void InitStage()
    {
        //stageScore = 0;
        //stageTimeConsuming = 0;
        curLoop = 0;
    }

    /*
        负责人：王海新 && 程自豪
        功能：获取当前关卡的初始速度
        输入: void
        输出：int
    */
    int getStageSpeed()
    {
        return stageSpeed;
    }

    /*
        负责人：王海新 && 程自豪
        功能：返回通过当前关卡要吃几个食物
        输入: void
        输出：int
    */
    int getStagePassStandard()
    {
        return stagePassStandard;
    }

    /*
        负责人：王海新 && 程自豪
        功能：获取当前关卡中生成一个道具所隔的轮次
        输入: void
        输出：int
    */
    int getPropGap()
    {
        return propGap;
    }

    /*
        负责人：王海新 && 程自豪
        功能：用于增加障碍物
        输入: 障碍物的增加量
        输出：void
    */
    void barrierIncrease()
    {
        stageBarrierNum += 2; //修改障碍物的数量
    }

    /*
        负责人：王海新 && 程自豪
        功能：增加轮次
        输入: void
        输出：void
    */
    void loopIncrease()
    {
        curLoop++;
    }

    /*
        负责人：王海新 && 程自豪
        功能：改变当前关卡的得分
        输入: int
        输出：void
    */
    /* void changeScore(int change)
     {
         stageScore += change;
     }*/

     /*
         负责人：王海新 && 程自豪
         功能：增加耗时
         输入: void
         输出：void
     */
     //void increaseTime()
     //{
     //    stageTimeConsuming++;
     //}

     /*
         负责人：王海新 && 程自豪
         功能：返回当前关卡的得分
         输入: void
         输出：int
     */
     //int getStageScore();

     /*
         负责人：王海新 && 程自豪
         功能：返回当前关卡的耗时
         输入: void
         输出：void
      */
      //int getStageTimeConsuming();


      /*
          负责人：王海新 && 程自豪
          功能：判断要生成的道具或者食物的坐标位置是否是5，是的话就合法，否则不合法
          输入: 坐标位置x和y
          输出：true/false
      */
    bool isLegal(int x, int y) {
        if (map[x][y] == 5)
            return true;
        else return false;
    }

    /*
        负责人：王海新 && 程自豪
        功能：
            1.根据当前轮次CurLoop和生成道具的间隔PropGap判断是够要生成道具
                如果需要生成就生成随机坐标使用isLegal判断是否合法，不合法要重新生成
                将生成的合法的随机坐标在map数组上随机标注为3或者4.
            1.生成食物的随机坐标，使用isLegal判断是否合法，不合法要重新生成
            2.将生成的合法的随机坐标在map数组上标注为2
            3.根据障碍物的数量生成相应数量的随机坐标，每生成一个坐标要判断是否合法，不合法要重新生成。
            3.将生成的合法的随机坐标在map数组上标注为0
            3.将snakeBody数组记录的坐标标注为1
        输入：void
        输出：void
    */
    void mapGenerate(bool ifEaten, bool ifFirstTime)
    {
        if (ifEaten == true || ifFirstTime == true)
        {
            memset(map, 0, sizeof(map)); //每次要重新初始化map
            for (int i = 1; i < HH - 1; i++)
            {
                for (int j = 1; j < WW - 1; j++)
                {
                    map[i][j] = 5;
                }
            }

            int x = rand() % WW;   //坐标x
            int y = rand() % HH;   //坐标y
            int num = rand() % 2 + 3;   //随机数3|4

            //道具坐标
            if ((this->curLoop) % (this->propGap) == 1)
            {
                while (!isLegal(x, y))
                {
                    x = rand() % WW;
                    y = rand() % HH;
                }
                map[x][y] = num;
            }

            //食物坐标
            while (!isLegal(x, y))
            {
                x = rand() % WW;
                y = rand() % HH;
            }
            map[x][y] = 2;

            //障碍物坐标
            for (int i = 0; i < this->stageBarrierNum; i++)
            {
                while (!isLegal(x, y))
                {
                    x = rand() % WW;
                    y = rand() % HH;
                }
                map[x][y] = 0;
            }
            //更新蛇身
            for (int i = 1; i <= curLength; i++)
            {
                map[snakeBody[i][0]][snakeBody[i][1]] = 1;
            }
        }

        else {
            //清空蛇身
            for (int i = 1; i < HH - 1; i++)
            {
                for (int j = 1; j < WW - 1; j++)
                {
                    if (map[i][j] == 1) map[i][j] = 5;
                }
            }
            //重新打印蛇身
            for (int i = 1; i <= curLength; i++)
            {
                map[snakeBody[i][0]][snakeBody[i][1]] = 1;
            }
        }
    }
};

class Player
{
    /*
    这个类主要是检测用户账户合法性的，然后副组长时间比较多严主平时间比较少嘛，你们两个看这分
    */
private:
    std::string username; //该用户的用户名
    std::string password; //该用户的密码
    int curTotalScore; //该次游戏的总得分
    int curTotalTime; //该次游戏的总耗时

public:
    Player()
    {};

    void setUsername(std::string& username) {
        this->username = username;
    }

    void setPassword(std::string& password) {
        this->password = password;
    }

    /*
        负责人：杨竞博 && 严主平
        功能：读取注册文件user.txt，判断当前的用户名和密码在该文件中是否存在。
        输入：void
        输出：true/false
    */
    bool ifExist() {
        std::ifstream userFile("user.txt");
        std::string line;
        while (std::getline(userFile, line)) {
            std::size_t position = line.find(' ');
            if (position != std::string::npos) {
                std::string storedName = line.substr(0, position);
                std::string storedPassword = line.substr(position + 1);
                if (this->username == storedName) {
                    /*std::cerr << "Error:username already exist!!!";*/
                    return true;
                }
            }
        }
        userFile.close();
        return false;
    }

    void changeScore(int change)
    {
        curTotalScore += change;
    }

    void increaseTime()
    {
        curTotalTime++;
    }

    /*
        负责人：杨竞博 && 严主平
        功能：判断当前的用户名/密码在是否合法，根据textflag是PassWord还是UserName调用不同的合法判断逻辑
        输入：字符串str，字符串属性textFlag
        输出：true/false
    */
    bool ifLegal(std::string str, EnumTextAttr textFlag) {
        if (textFlag == EnumTextAttr::USERNAME) {
            if (str.length() >= 8 && str.length() <= 12) {
                return true;
            }
            else {
                std::cerr << "Error:The username input must be between 8 and 12 characters.\n";
                return false;
            }
        }
        if (textFlag == EnumTextAttr::PASSWORD) {
            if (str.length() >= 8 && str.length() <= 16) {
                return true;
            }
            else {
                std::cerr << "Error:The password input must be between 8 and 16 characters.\n";
                return false;
            }
        }
        return false;
    }

    /*
        负责人：杨竞博 && 严主平
        功能：将当前的Player对象的用户名，密码记录写入user.txt中，
        输入：void
        输出：void
    */
    void createAccount() const {
        std::ofstream userFile("user.txt", std::ios_base::app);
        if (userFile.is_open()) {
            userFile << this->username << " ";
            userFile << this->password << std::endl;
            //std::cout << "creat account successfully!";
            userFile.close();
        }
        else {
            //std::cerr << "failed to open file!";
        }
    }

    /*
       负责人：杨竞博 && 严主平
       功能:根据全局变量gameScore和gameTime记录的当局游戏的总得分和总时间更新该玩家该次游戏的总得分和总耗时(俩变量都是全局变量)
       输入:当前游戏结束时的得分score和TimeConsuming
       输出：void
   */
    void playerDataUpdate(int score, int timeConsuming) {
        if (score >= 0 && timeConsuming >= 0) {
            curTotalScore = score;
            curTotalTime = timeConsuming;
        }
    }


    /*
* 功能：返回这个用户总得分数
*/
    int getTotalScore() {
        return this->curTotalScore;
    }

    /*
    *
    *
    * 功能：返回用户总共使用的时间
    *
    */
    int getTotalTime() {
        return this->curTotalTime;
    }

    /*
    * 功能：返回用户的名字
    */
    std::string getUsername()
    {
        return this->username;
    }
};

class Snake
{
private:
    int eatenFood = 0;//已经吃到的食物
    int length = 3; //蛇当前的长度,初始长度为1
    int x = WW / 2, y = HH / 2; //蛇头当前的位置


public:
    EnumDirection dir = EnumDirection::D; //蛇头的移动方向，初始向右
    /*
        负责人：侯顺智
        功能：创建Snake对象，完成snakebody数组的初始化
        输入：蛇的长度，蛇头坐标x和y，前进方向d
        输出：
    */
    Snake(int eatenfood, int len, int x, int y, EnumDirection d)
        :eatenFood(eatenfood), length(len), x(x), y(y), dir(d)
    {

    };

    /**
    默认构造函数
    */
    Snake() = default;

    /*
        负责人：侯顺智
        功能：保存蛇的当前参数到全局变量curLength,curX,curY,curEatenFood,curSnakeDirection
        输入：void
        输出：void
    */
    void saveSnake();

    /*
        负责人：侯顺智
        功能：根据输入的方向更新蛇头要移动到的下一个位置的坐标snakebody[0][0]和snakebody[0][1]:
                    snakeBody[0][0] = snakeBody[0][0]+direction[d][0] * stage.stageSpeed * acc；
                    snakeBody[0][1] = snakeBody[0][1]+direction[d][1] * stage.stageSpeed * acc；
              同时更新前进方向snake.d = d;
        输入：方向d,关卡对象stage
        输出：void
    */
    void getDir(EnumDirection d, Stage& stage);


    /*
        负责人：侯顺智
        功能：
            根据方向和蛇头的位置实现snakebody数组的更新，
            更新方式：snakebody的倒数第二个坐标赋值给最后一个坐标，倒数第三个坐标赋值给倒数第二个坐标...第0个坐标赋值给第1个坐标
            耗时+1
        输入：关卡对象stage
        输出：void
    */
    void normalMove(Stage& stage, Player& player);

    /*
        负责人：侯顺智
        功能：根据吃到的道具，前进方向和蛇头的位置实现snakebody数组的更新（注意蛇身长度加1），
              当前分数+3,同时当前关卡内轮次+1，耗时+1，轮次更新后判断是否需要增加障碍物，如果需要就增加，
              调用stage.barrierIncrease()即可。
        输入：当前所在的关卡对象stage
        输出：void
    */
    void eatGoodProp(Stage& stage, Player& player);

    /*
        负责人：侯顺智
        功能：根据吃到的道具，前进方向和蛇头的位置实现snakeBody数组的更新（注意蛇身长度加1），
              当前分数-3,同时当前关卡内轮次+1，耗时+1，轮次更新后判断是否需要增加障碍物，如果需要就增加，
              调用stage.barrierIncrease()即可。
        输入：
        输出：
    */
    void eatBadProp(Stage& stage, Player& player);

    /*
        负责人：侯顺智
        功能：
            根据方向和蛇头的位置实现snakeBody数组的更新,蛇身长度+1、分数+1，耗时+1，当前关卡内轮次+1，
            轮次更新后判断是否需要增加障碍物，如果需要就增加，调用stage.barrierIncrease()即可。
        输入：当前所在的关卡对象stage
        输出：void
    */
    void eatFood(Stage& stage, Player& player);

    /*
        负责人：侯顺智
        功能：返回蛇当前的长度
        输入：void
        输出：void
    */
    int getLength() {
        return length;
    }
    /*
        负责人：侯顺智
        功能：返回蛇当前的移动方向
        输入：void
        输出：void
    */
    EnumDirection getHeadDirection() {
        return dir;
    }

    /*
        负责人：侯顺智
        功能：返回蛇当前吃掉的食物
        输入：void
        输出：void
    */
    int getEatenFood() {
        return eatenFood;
    };
};

void Snake::saveSnake() {
    curLength = length;
    //curX = snakeBody[1][0];
    //curY = snakeBody[1][1];
    curX = x;
    curY = y;
    curEatenFood = eatenFood;
    curSnakeDirection = dir;
}

void Snake::getDir(EnumDirection d, Stage& stage) {
    //std::cout << snakebody[0][0] << ' ' << snakebody[0][1] << '\n';
    if (dir == EnumDirection::W && d == EnumDirection::S) {
        return;
    }
    else if (dir == EnumDirection::S && d == EnumDirection::W) {
        return;
    }
    else if (dir == EnumDirection::A && d == EnumDirection::D) {
        return;
    }
    else if (dir == EnumDirection::D && d == EnumDirection::A) {
        return;
    }

    // 更改蛇的方向
    this->dir = d;

    // 判断蛇头应该在的下一个位置
    /*snakeBody[0][0] = snakeBody[1][0] + direction[d][0] * stage.getStageSpeed() * acc;
    snakeBody[0][1] = snakeBody[1][1] + direction[d][1] * stage.getStageSpeed() * acc;*/

    snakeBody[0][0] = snakeBody[1][0] + direction[d][0];
    snakeBody[0][1] = snakeBody[1][1] + direction[d][1];

    //std::cout << snakebody[0][0] << ' ' << snakebody[0][1] << '\n';
}

void Snake::normalMove(Stage& stage, Player& player) {
    getDir(dir, stage);
    // 耗时+1
    player.increaseTime();
    // ++stage.stageTimeConsuming;
    // gameTime += stage.stageTimeConsuming;

    int before_head_x = snakeBody[0][0];
    int before_head_y = snakeBody[0][1];
    int tail_x = snakeBody[length][0];
    int tail_y = snakeBody[length][1];

    // 更新蛇的位置
    for (int i = length; i > 0; --i) {
        snakeBody[i][0] = snakeBody[i - 1][0];
        snakeBody[i][1] = snakeBody[i - 1][1];
    }

    // 更新蛇头位置
    x = snakeBody[1][0];
    y = snakeBody[1][1];

    // 将蛇的位置反映到map中去
    map[before_head_x][before_head_y] = 1;
    map[tail_x][tail_y] = 5;

    // 更新蛇的状态
    saveSnake();
}
//void Snake::normalMove(Stage& stage, Player& player) {
//    getDir(dir, stage);
//    // 耗时+1
//    player.increaseTime();
//    // ++stage.stageTimeConsuming;
//    // gameTime += stage.stageTimeConsuming;
//
//    int before_head_x = snakeBody[0][0];
//    int before_head_y = snakeBody[0][1];
//    int tail_x = snakeBody[length][0];
//    int tail_y = snakeBody[length][1];
//
//    int shift = int(stage.getStageSpeed() * acc);
//
//    // 更新蛇的位置
//    for (int i = length; i > 1; --i) {
//        if (i - shift <= 0)
//        {
//            switch (dir)
//            {
//            case W:
//                snakeBody[i][0] = snakeBody[0][0];
//                snakeBody[i][1] = snakeBody[0][1] - (i - 1);
//                break;
//            case S:
//                snakeBody[i][0] = snakeBody[0][0];
//                snakeBody[i][1] = snakeBody[0][1] + (i - 1);
//                break;
//            case A:
//                snakeBody[i][0] = snakeBody[0][0] + (i - 1);
//                snakeBody[i][1] = snakeBody[0][1];
//                break;
//            case D:
//                snakeBody[i][0] = snakeBody[0][0] - (i - 1);
//                snakeBody[i][1] = snakeBody[0][1];
//                break;
//            }
//        }
//        else
//        {
//            snakeBody[i][0] = snakeBody[i - shift][0];
//            snakeBody[i][1] = snakeBody[i - shift][1];
//        }
//    }
//    // 更新蛇头位置
//    x = snakeBody[1][0];
//    y = snakeBody[1][1];
//    snakeBody[1][0] = snakeBody[0][0];
//    snakeBody[1][1] = snakeBody[0][1];
//
//    // 将蛇的位置反映到map中去
//    //map[before_head_x][before_head_y] = 1;
//    //map[tail_x][tail_y] = 5;
//}



void Snake::eatFood(Stage& stage, Player& player) {

    // 进行蛇的移动
    int tail_x = snakeBody[length][0];
    int tail_y = snakeBody[length][1];
    normalMove(stage, player);
    ++length;
    ++eatenFood;
    // 蛇的长度+1，最后一个位置应该是原来的最后一个位置
    snakeBody[length][0] = tail_x;
    snakeBody[length][1] = tail_y;

    // 对数据进行更新
    // ++stage.stageScore;
    player.changeScore(1);
    stage.loopIncrease();

    // 将蛇的位置反映到map中去
    map[snakeBody[length][0]][snakeBody[length][1]] = 1;

    //std::cout << "stage.CurLoop:" << stage.CurLoop << '\n';
    // 是否需要增加障碍物
    if (stage.curLoop % 3 == 0) {
        //std::cout << "增加了一个障碍物\n";
        stage.barrierIncrease();
    }
    saveSnake();
}

void Snake::eatGoodProp(Stage& stage, Player& player) {
    eatFood(stage, player);
    // stage.stageScore += 2;
    player.changeScore(2);
    saveSnake();
    //std::cout << "当前分数：" << stage.score << " 关卡内轮次："<< stage.CurLoop << " 耗时： " << stage.TimeConsuming << '\n';
}

void Snake::eatBadProp(Stage& stage, Player& player) {
    eatFood(stage, player);
    // stage.stageScore -= 4;
    gameScore -= 4;
    saveSnake();
    //std::cout << "当前分数：" << stage.score << " 关卡内轮次：" << stage.CurLoop << " 耗时： " << stage.TimeConsuming << '\n';

}

// -------------------- 数据设计 --------------------





// -------------------- view --------------------
/*
    负责人：蒋恒新 && 丁伟民

    因为这个层内容确实不少，就分了两个人。然后丁伟民哥们不是要面临期末嘛，有时间就多做点，没时间就多写点。

    功能：
          显示游戏介绍按钮
          显示游戏设置按钮
          显示关卡选择按钮
          显示排行榜按钮
          显示退出登陆按钮
    输入：void
    输出：void
*/
void menuView();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：展示登陆按钮，注册用户按钮和退出游戏按钮
*/
void homePage();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：展示输入账号的框，输入密码的框,注册按钮和返回按钮
    输入：void
    输出：void
*/
void regPage();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：展示输入账号的框，输入密码的框，登陆按钮和返回按钮
    输入：void
    输出：void
*/
void logPage();


/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：显示游戏介绍和开发人员介绍,制作返回键
    输入：void
    输出：void
*/
void infoPage();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：读取record.txt文件并显示前8名的用户的用户名，耗时和得分，显示返回键
    输入：void
    输出：void
*/
void rankPage();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：显示设置界面，分别有*1.0,*1.5,和*2.0的加速按钮以及1个返回主菜单的按钮
    输入：void
    输出：void
*/
void setPage();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：显示第一关的按钮，第二关的按钮，第三关的按钮和返回主菜单的按钮
    输入：void
    输出：void
*/
void stagePage();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：
        显示工具栏按钮，使用printText(...)显示当前玩家的分数和耗时，根据map渲染出游戏界面，
        map是全局数组，可以直接调用
    输入：当前总得分gameScore，当前总耗时gameTime
    输出：void
*/
//void gameView(int gameScore, int gameTime);

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：显示一个写有"继续游戏"的按钮和一个写有"返回主菜单"的按钮
    输入：void
    输出：void
*/
void toolView();
/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：
        1.显示"返回主菜单"按钮
        2.显示"重新开始"按钮
    输入：void
    输出：void
*/
void gamePage();

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：在指定位置输出特定文本
    输入：字符串对象text，开始写的坐标x和y，字体大小size
    输出：void
*/
//void printText(std::string text, int x, int y, int size);

/*
    负责人：蒋恒新 && 丁伟民（具体选择）
    功能：
        1.显示"返回主菜单"按钮
        2.显示"重新开始"按钮
    输入：void
    输出：void
*/
void endPage();
// -------------------- view --------------------

// -------------------- service --------------------

/*
    负责人：佐瑞峰 && 吴相龙(吴相龙)

    这个界面他主要是负责界面跳转的，只要吧标记为修改过来就行。咱俩分着来。
    功能：
        1.点到注册按钮，pageFlag = REG_PAGE；
        2.点到登陆按钮，pageFlag = LOG_PAGE；
        3.点到退出按钮，exit（0）
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void homeClickCheck();

/*
    负责人：佐瑞峰 && 吴相龙（吴相龙）
    功能：
        检测鼠标的点击区域，如果是
        1.排行榜按钮区域，pageFlag = RANK_PAGE；
        2.关卡按钮区域，pageFlag = STAGE_PAGE；
        3.介绍按钮区域，pageFlag = INFO_PAGE；
        4.退出登陆按钮区域，pageFlag = HOME_PAGE；
        5.设置按钮区域，pageFlag = SET_PAGE；
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void menuClickCheck();

/*
    负责人：佐瑞峰 && 吴相龙（吴相龙）
    功能：
        Player player();
        while(1){
            1.如果检测到鼠标点击到账号框，textFlag=USERNAME,并开始接受键盘输入，每输入一个字符更新一次usernameToPrint，
            然后调用一次printText显示userNameToPrint，这个过程持续到读入回车键为止，
            之后令用Player.isLegal(usernameToPrint，textFlag)判断账号名是否合法，
            不合法就清空输入并要求重新输入，合法就用player.setUsername(usernameToPrint);

            2.如果检测到鼠标点击到密码框，textFlag=PASSWORD,并开始接受键盘输入，每输入一个字符更新一次passwordToPrint，
            然后调用一次printText显示passwordPrint，这个过程持续到读入回车键为止
            然后调用Player.isLegal(passwordToPrint，textFlag)判断密码是否合法，不合法就清空输入并要求重新输入,
            合法就player.setPassword(passwordToPrint);

            3.如果检测到点击到了返回按钮，pageflag = HOME_PAGE；

            4.如果检测到点击到了注册按钮，调用player.ifExit()看该账号是否已经在user.txt中存在
            如果返回为false则调用player.createAccout()在user.txt中创建记录，并让pageFlag = MENU_PAGE；
        }
    输入：void
    输出：void
*/
void regClickCheck();

/*
    负责人:佐瑞峰 && 吴相龙（吴相龙）
    功能:
        创建一个临时Player对象player()
        while(1){
            1.如果检测到鼠标点击到账号框,开始接受键盘输入，每输入一个字符更新一次usernameToPrint，
            然后调用一次printText(...)显示usernameToPrint，这个过程持续到读入回车键为止，
            然后用player.setUsername(usernameToPrint);

            2.如果检测到鼠标点击到密码框,开始接受键盘输入，每输入一个字符更新一次passwordToPrint，
            然后调用一次printText(...)显示passwordToPrint，这个过程持续到读入回车键为止，
            然后用player.setPassword(passwordToPrint);

            3.如果检测到点击到了返回按钮，pageFlag = HOME_PAGE；
            4.如果检测到点击到了登陆按钮，调用Player.ifExit()，如果返回为true则pageFlag = MENU_PAGE；
        }
    输入：void
    输出：void
*/
void logClickCheck();


/*
    负责人：佐瑞峰 && 吴相龙（吴相龙）
    功能：检测是否点击到了信息界面中的返回键，如果是，那么pageFlag = MENU_PAGE；
     *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void infoClickCheck();

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        1.如果是第一关按钮：curStage = FIRST_STAGE；
                            pageFlag = GAME_PAGE；
        2.如果是第二关按钮：curStage = SECOND_STAGE；
                            pageFlag = GAME_PAGE；
        3.如果是第三关按钮：curStage = THIRD_STAGE；
                            pageFlag = GAME_PAGE；
        4.如果是返回按钮：  pageFlag = MENU_PAGE；
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void stageClickCheck();

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        1.点击到了退出按钮，pageFlag = MENU_PAGE;
        2.点击到了重新开始按钮，pageFlag = GAME_PAGE;curStage = FIRST_STAGE，也就是重新从第一关开始玩
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void endClickCheck();


/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        检测是否点击到了信息界面中的返回键，如果是pageFlag = MENU_PAGE；
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void rankClickCheck();

/*
   负责人：佐瑞峰 && 吴相龙（具体选择）
   功能：
        检测鼠标点击区域:
        1.如果是返回按钮，pageflag = MenuPage；
        2.如果是速度设置按钮，看具体按到了哪个倍率，然后将全局变量的acc修改为相应的值
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
   输入：void
   输出：void
*/
void setClickCheck();

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        初始化移动方向为蛇当前的移动方向EnumDirection d = snake.getHeadDirection();
        暂停0.5s;
        如果有用户的键盘/鼠标输入：
            if(是wsad的方向键)  如果是"w"，直接往snake.getDir()里传入W,其他同理是什么按键就直接去掉双引号传大写字母就好
            if(输入是鼠标事件且点在了工具栏的按钮上)  pageFlag = TOOL_PAGE;return;
        如果没有键盘/鼠标输入：
            snake.getDir(d)
         *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：Snake& snake,Stage& stage
    输出：void
*/
void gameEventCheck(Snake& snake, Stage& stage);

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        检测鼠标事件：
        1.点击区域是返回菜单按钮，pageFlag = MENU_PAGE；
        2.点击区域是继续游戏按钮，pageFlag = GAME_PAGE；
    输入：void
    输出：void
*/
void toolClickCheck();

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：接受一个新记录写到record.txt中,记录包括当前player的用户名，分数和耗时，
          然后以分数为第一关键字，耗时为第二关键字对各记录实现进行排序。
    输入：Player& player
    输出：void
*/
void fileWriteAndRank(Player& player);


/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：通过对比蛇已经吃到的食物数量和通过当前关卡所需吃到的食物数量来判断是否过关
    输入：关卡对象stage和蛇对象snake
    输出：true/false
*/
bool isWin(const Snake& snake, const Stage& stage);


/*
    负责人:吴昊伦
    功能:运行游戏界面，完成各关卡之间的跳转,统计单次游戏的总分和总耗时，记录在全局变量当中
    参数:void
    返回值:void
*/
void runGame();

/*
    负责人:吴昊伦
    功能:完成各界面之间的跳转
    参数:void
    返回值:void
*/
void controller();

/*
    负责人:吴昊伦
    功能:重载++,实现枚举类型的自加
    参数:枚举类型curStage
    返回值: EnumCurStage
*/
inline EnumCurStage& operator++(EnumCurStage& curStage);
// -------------------- service --------------------
inline EnumCurStage& operator++(EnumCurStage& curStage)
{
    curStage = static_cast<EnumCurStage>(curStage + 1); //实现++curStage
    return curStage;
}

void homeClickCheck() {
    while (1) {
        MOUSEMSG m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN)
        {
            int x = m.x;
            int y = m.y;
            if (x > 412 && x < 612 && y > 200 && y < 320) {
                pageFlag = LOG_PAGE;
                break;
            }

            if (x > 412 && x < 612 && y > 350 && y < 450) {
                pageFlag = REG_PAGE;
                break;
            }

            if (x > 412 && x < 612 && y > 478 && y < 578) {
                exit(0);
            }
        }
    }
}


void menuClickCheck() {
    while (1) {
        MOUSEMSG m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN)
        {
            int x = m.x;
            int y = m.y;

            if (x > 425 && x < 600 && y > 80 && y < 140) {
                pageFlag = STAGE_PAGE;//开始
                break;
            }

            if (x > 425 && x < 600 && y > 180 && y < 240) {
                pageFlag = SET_PAGE;//设置
                break;
            }

            if (x > 425 && x < 600 && y > 280 && y < 340) {
                pageFlag = RANK_PAGE;//排行榜
                break;
            }



            if (x > 425 && x < 600 && y > 380 && y < 440) {
                pageFlag = INFO_PAGE;//介绍
                break;
            }

            if (x > 425 && x < 600 && y > 480 && y < 540) {
                pageFlag = HOME_PAGE;//退出
                break;
            }


        }
    }
}
wstring_convert<std::codecvt_utf8<wchar_t>> converter;
void regClickCheck() {
    Player player;
    usernameToPrint = "";
    passwordToPrint = "";
    while (1) {
        MOUSEMSG m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN)    //检测到账号框点击
        {
            int x = m.x;
            int y = m.y;
            if (m.x > 435 && m.x < 635 && m.y > 159 && m.y < 210) {//账号框
                /*
                textFlag = USERNAME;//修改flag
                char ch{};
                while ((ch = getchar()) != '\n') {
                    usernameToPrint += ch;
                    w.printText(sToWs(usernameToPrint), 435, 159, 30);
                }//回车停止
                */
                textFlag = USERNAME;

                wchar_t s[20];
                InputBox(s, 13, L"请输入账号：");
                wstring wstring1(s);
                w.printText(wstring1, 435, 159, 30);
                usernameToPrint = converter.to_bytes(wstring1);

                Sleep(500);
                if (player.ifLegal(usernameToPrint, textFlag)) {
                    player.setUsername(usernameToPrint); //合法判断
                }

                else {
                    MessageBox(NULL, _T("用户名不合法!"), _T("错误"), MB_OK | MB_ICONWARNING);
                    usernameToPrint = "";//不合法清空
                    Button* regPageAccount = new Button(330, 164, 82, 50, L"账号:");
                    regPageAccount->show();
                    setfillcolor(WHITE);//设置输入框填充颜色
                    setfillstyle(BS_SOLID);//设置输入框填充方式
                    fillrectangle(412, 164, 412 + 200, 164 + 50);//绘制账号输入框矩形
                }
            }

            if (m.x > 453 && m.x < 653 && m.y > 246 && m.y < 296) {//密码框
                /*
                textFlag = PASSWORD;//修改flag
                char ch{};
                while ((ch = getchar()) != '\n') {
                    passwordToPrint += ch;
                    w.printText(sToWs(passwordToPrint), 435, 246, 30);
                */
                textFlag = PASSWORD;

                wchar_t s[20];
                InputBox(s, 13, L"请输入密码：");
                wstring wstring1(s);
                w.printText(wstring1, 435, 246, 30);
                passwordToPrint = converter.to_bytes(wstring1);


                if (player.ifLegal(passwordToPrint, textFlag)) {
                    player.setPassword(passwordToPrint); //合法判断
                }
                else {
                    MessageBox(NULL, _T("密码不合法!"), _T("错误"), MB_OK | MB_ICONWARNING);
                    passwordToPrint = "";//不合法清空
                    Button* regPagePassword = new Button(330, 250, 82, 50, L"密码:");
                    regPagePassword->show();
                    setfillcolor(WHITE);
                    setfillstyle(BS_SOLID);
                    fillrectangle(412, 250, 412 + 200, 250 + 50);//绘制密码输入框矩形

                }
            }

            if (m.x > 412 && m.x < 612 && m.y > 480 && m.y < 530) {//返回按钮
                pageFlag = HOME_PAGE;
                break;
            }

            //  4.如果检测到点击到了注册按钮，调用player.ifExit()看该账号是否已经在user.txt中存在
            //  如果返回为false则调用player.createAccout()在user.txt中创建记录，并让pageFlag = MENU_PAGE；

            if (m.x > 412 && m.x < 612 && m.y > 400 && m.y < 450) {//注册
                //char ch{};
                //while ((ch = getchar()) != '\n') {
                //    usernameToPrint += ch;
                //    w.printText(sToWs(usernameToPrint),435, 159, 30);
                //}//回车停止
                if (usernameToPrint == "" || passwordToPrint == "")
                {
                    MessageBox(NULL, _T("用户名和密码不能为空!"), _T("错误"), MB_OK | MB_ICONWARNING);
                    //cout << "用户名和密码不能为空！" << endl;
                    continue;
                }
                if (!player.ifExist()) {
                    player.createAccount();
                    MessageBox(NULL, _T("注册成功!"), _T("信息"), MB_OK | MB_ICONINFORMATION);
                    pageFlag = HOME_PAGE;
                    break;
                }
                else {
                    MessageBox(NULL, _T("该用户已存在!"), _T("错误"), MB_OK | MB_ICONWARNING);
                    //cout << "用户名已存在，请重新输入" << endl;
                    usernameToPrint = "";
                    passwordToPrint = "";

                    Button* regPageAccount = new Button(330, 164, 82, 50, L"账号:");
                    regPageAccount->show();
                    setfillcolor(WHITE);//设置输入框填充颜色
                    setfillstyle(BS_SOLID);//设置输入框填充方式
                    fillrectangle(412, 164, 412 + 200, 164 + 50);//绘制账号输入框矩形

                    Button* regPagePassword = new Button(330, 250, 82, 50, L"密码:");
                    regPagePassword->show();
                    setfillcolor(WHITE);
                    setfillstyle(BS_SOLID);
                    fillrectangle(412, 250, 412 + 200, 250 + 50);//绘制密码输入框矩形
                }
            }
        }
    }
}
#include <codecvt>

void logClickCheck() {
    Player player;
    usernameToPrint = "";
    passwordToPrint = "";
    while (1) {
        MOUSEMSG m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            int x = m.x;
            int y = m.y;
            if (m.x > 412 && m.x < 612 && m.y > 164 && m.y < 214) {//账号框接收键盘输入
                /*
                char ch{};
                while ((ch = getchar()) != '\n') {

                    usernameToPrint += ch;
                    w.printText(sToWs(usernameToPrint), 435, 159, 30);
                }
                */
                wchar_t s[20];
                InputBox(s, 13, L"请输入账号：");
                wstring wstring1(s);
                w.printText(wstring1, 435, 159, 30);
                usernameToPrint = converter.to_bytes(wstring1);
            }

            if (m.x > 412 && m.x < 612 && m.y > 250 && m.y < 300) {//密码框接收键盘输入

                char ch{};
                /*
                while ((ch = getchar()) != '\n') {
                    passwordToPrint += ch;
                    w.printText(sToWs(passwordToPrint), 435, 250, 30);
                }
                */
                wchar_t s[20];
                InputBox(s, 13, L"请输入密码：");
                wstring wstring1(s);
                w.printText(wstring1, 435, 250, 30);
                passwordToPrint = converter.to_bytes(wstring1);

            }

            if (m.x > 412 && m.x < 612 && m.y > 487 && m.y < 537) {//返回按钮
                pageFlag = HOME_PAGE;
                break;
            }

            if (m.x > 412 && m.x < 612 && m.y > 399 && m.y < 450) {//登录按钮
                if (usernameToPrint == "" || passwordToPrint == "")
                {
                    MessageBox(NULL, _T("请输入用户名和密码!"), _T("错误"), MB_OK | MB_ICONWARNING);
                    // cout << "请输入用户名和密码！" << endl;
                    continue;
                }
                player.setUsername(usernameToPrint);
                player.setPassword(passwordToPrint);

                if (player.ifExist()) {
                    //cout << usernameToPrint << endl;
                    //cout << passwordToPrint << endl;
                    MessageBox(NULL, _T("登陆成功!"), _T("信息"), MB_OK | MB_ICONINFORMATION);
                    pageFlag = MENU_PAGE;
                    break;
                }
                else {
                    MessageBox(NULL, _T("用户名或密码输入错误!"), _T("错误"), MB_OK | MB_ICONWARNING);
                    //std::cerr << "Error:username error !!! \n";
                    usernameToPrint = "";
                    passwordToPrint = "";
                    Button* logPageAccount = new Button(330, 164, 82, 50, L"账号:");
                    logPageAccount->show();
                    setfillcolor(WHITE);//设置输入框填充颜色
                    setfillstyle(BS_SOLID);//设置输入框填充方式
                    fillrectangle(412, 164, 412 + 200, 164 + 50);//绘制账号输入框矩形

                    Button* logPagePassword = new Button(330, 250, 82, 50, L"密码:");
                    logPagePassword->show();
                    setfillcolor(WHITE);//设置输入框填充颜色
                    setfillstyle(BS_SOLID);//设置输入框填充方式
                    fillrectangle(412, 250, 412 + 200, 250 + 50);//绘制密码输入框矩形
                }
            }
        }
    }
}

void infoClickCheck() {
    while (1) {
        MOUSEMSG m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN)
        {
            int x = m.x;
            int y = m.y;
            if (1) {
                pageFlag = MENU_PAGE;
                break;
            }

        }
    }
}
/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        1.如果是第一关按钮：curStage = FIRST_STAGE；
                            pageFlag = GAME_PAGE；
        2.如果是第二关按钮：curStage = SECOND_STAGE；
                            pageFlag = GAME_PAGE；
        3.如果是第三关按钮：curStage = THIRD_STAGE；
                            pageFlag = GAME_PAGE；
        4.如果是返回按钮：  pageFlag = MENU_PAGE；
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void stageClickCheck()
{
    while (true) {
        MOUSEMSG m = GetMouseMsg();  // 获取鼠标消息

        if (m.uMsg == WM_LBUTTONDOWN)  // 鼠标左键按下事件
        {
            int x = m.x;  // 鼠标事件发生时的横坐标
            int y = m.y;  // 鼠标事件发生时的纵坐标

            if (m.x > 363 && m.x < 663 && m.y > 100 && m.y < 160)
            {
                curStage = FIRST_STAGE;
                pageFlag = GAME_PAGE;
                break;

            }
            else if (m.x > 363 && m.x < 663 && m.y > 200 && m.y < 260)
            {
                curStage = SECOND_STAGE;
                pageFlag = GAME_PAGE;
                break;
            }
            else if (m.x > 363 && m.x < 663 && m.y > 300 && m.y < 360)
            {
                curStage = SECOND_STAGE;
                pageFlag = GAME_PAGE;
                break;
            }
            else if (m.x > 363 && m.x < 663 && m.y > 520 && m.y < 580)
            {
                pageFlag = MENU_PAGE;

                break;
            }

        }
    }

};

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        1.点击到了退出按钮，pageFlag = MENU_PAGE;
        2.点击到了重新开始按钮，pageFlag = GAME_PAGE;curStage = FIRST_STAGE，也就是重新从第一关开始玩
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void endClickCheck()
{
    int restartTop, restartBottom, restartLeft, restartRight;//定义的是第一关的窗口大小
    int returnTop, returnBottom, returnLeft, returnRight;//返回键的区域大小

    while (true) {
        MOUSEMSG m = GetMouseMsg();  // 获取鼠标消息

        if (m.uMsg == WM_LBUTTONDOWN)  // 鼠标左键按下事件
        {
            int x = m.x;  // 鼠标事件发生时的横坐标
            int y = m.y;  // 鼠标事件发生时的纵坐标

            if (m.x > 425 && m.x < 600 && m.y > 242 && m.y < 302)
            {
                curStage = FIRST_STAGE;
                pageFlag = GAME_PAGE;
                curLength = 3;
                curX = WW / 2, curY = HH / 2;
                curEatenFood = 0;
                EnumDirection curSnakeDirection = EnumDirection::D; // 默认方向向右
                gameScore = 0;
                gameTime = 0;
                break;
            }
            else if (m.x > 425 && m.x < 600 && m.y > 375 && m.y < 435)
            {
                pageFlag = MENU_PAGE;
                break;
            }


        }
    }
};

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        检测是否点击到了信息界面中的返回键，如果是pageFlag = MENU_PAGE；
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：void
    输出：void
*/
void rankClickCheck()
{
    int returnTop, returnBottom, returnLeft, returnRight;//返回键的区域大小

    while (true) {
        MOUSEMSG m = GetMouseMsg();  // 获取鼠标消息

        if (m.uMsg == WM_LBUTTONDOWN)  // 鼠标左键按下事件
        {
            int x = m.x;  // 鼠标事件发生时的横坐标
            int y = m.y;  // 鼠标事件发生时的纵坐标


            if (m.x > 431 && m.x < 593 && m.y > 540 && m.y < 607)
            {
                pageFlag = MENU_PAGE;
                break;
            }

        }
    }
};

/*
   负责人：佐瑞峰 && 吴相龙（具体选择）
   功能：
        检测鼠标点击区域:
        1.如果是返回按钮，pageflag = MenuPage；
        2.如果是速度设置按钮，看具体按到了哪个倍率，然后将全局变量的acc修改为相应的值
        *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
   输入：void
   输出：void
*/
void setClickCheck()//BIG QUESTION
{


    while (true) {
        MOUSEMSG m = GetMouseMsg();  // 获取鼠标消息

        if (m.uMsg == WM_LBUTTONDOWN)  // 鼠标左键按下事件
        {
            int x = m.x;  // 鼠标事件发生时的横坐标
            int y = m.y;  // 鼠标事件发生时的纵坐标

            if (m.x > 412 && m.x < 612 && m.y > 45 && m.y < 145)
            {
                acc = 1;
                break;
            }
            else if (m.x > 412 && m.x < 612 && m.y > 195 && m.y < 295)
            {
                acc = 2;
                break;
            }
            else if (m.x > 412 && m.x < 612 && m.y > 349 && m.y < 449)
            {
                acc = 3;
                break;
            }
            else if (m.x > 412 && m.x < 612 && m.y > 501 && m.y < 601)
            {
                pageFlag = MENU_PAGE;
                break;
            }
        }
    }
}; //

/*
     负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        初始化移动方向为蛇当前的移动方向EnumDirection d = snake.getHeadDirection();
        暂停0.5s;
        如果有用户的键盘/鼠标输入：
            if(是wsad的方向键)  如果是"w"，直接往snake.getDir()里传入W,其他同理是什么按键就直接去掉双引号传大写字母就好
            if(输入是鼠标事件且点在了工具栏的按钮上)  pageFlag = TOOL_PAGE;return;
        如果没有键盘/鼠标输入：
            snake.getDir(d)
         *各个按钮区域是一块矩形，具体的坐标位置需要找负责相关界面绘制的同学沟通
    输入：Snake& snake
    输出：void
*/
void gameEventCheck(Snake& snake, Stage& stage)
{
    EnumDirection d = snake.getHeadDirection();
    Sleep(100 * (4 - acc));
    //Sleep(500);
    int top, left, right, bottom;
    MOUSEMSG m = GetMouseMsg();  // 获取鼠标消息
    char ch;
    //while (true) {
    //cout << "gameEventCheckgameEventCheckgameEventCheckgameEventCheckgameEventCheck\n";
    if (_kbhit()) {
        ch = _getch();
        switch (ch)
        {
        case'w':
        case'W':
            snake.getDir(EnumDirection::W, stage); break;
        case'a':
        case'A':
            snake.getDir(EnumDirection::A, stage); break;
        case'd':
        case'D':
            snake.getDir(EnumDirection::D, stage); break;
        case's':
        case'S':
            snake.getDir(EnumDirection::S, stage); break;
        }
        // cout << "getDirTest:" << ch << '\n';
         //break;
    }
};

/*
    负责人：佐瑞峰 && 吴相龙（具体选择）
    功能：
        检测鼠标事件：
        1.点击区域是返回菜单按钮，pageFlag = MENU_PAGE；
        2.点击区域是继续游戏按钮，pageFlag = GAME_PAGE；
    输入：void
    输出：void
*/
void toolClickCheck()
{
    int firstTop, firstBottom, firstLeft, firstRight;//定义的是第一关的窗口大小
    int returnTop, returnBottom, returnLeft, returnRight;//返回键的区域大小

    while (true)
    {
        MOUSEMSG m = GetMouseMsg();  // 获取鼠标消息

        if (m.uMsg == WM_LBUTTONDOWN)  // 鼠标左键按下事件
        {
            int x = m.x;  // 鼠标事件发生时的横坐标
            int y = m.y;  // 鼠标事件发生时的纵坐标

            if (m.x > 425 && m.x < 599 && m.y > 242 && m.y < 302)
            {
                pageFlag = GAME_PAGE;
                break;
            }

            else if (m.x > 425 && m.x < 599 && m.y > 375 && m.y < 435)
            {
                pageFlag = MENU_PAGE;
                break;
            }

        }
    }
};

/*
    负责人：佐瑞峰
    功能：在record.txt文件中每一行记录的是一个用户的数据。每个用户的数据格式是：用户名 用户分数 花费的时间
    将player的数据与record.txt中读出的用户数据进行重新排序，排序顺序先以用户分数为关键字，分数相同的人按照花费时间排序
    之后将排序好的数据以覆盖的形式写入record.txt中
    输入：Player& player
    输出：void
*/
void fileWriteAndRank(Player& player)
{
    // 读取record.txt文件中的用户数据
    std::ifstream file("record.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    // 将player的数据添加到lines中
    std::string playerData = player.getUsername() + " " + std::to_string(player.getTotalScore()) + " " + std::to_string(player.getTotalTime());
    lines.push_back(playerData);

    // 对lines进行排序
    std::sort(lines.begin(), lines.end(), [](const std::string& a, const std::string& b) {
        std::istringstream issA(a);
        std::istringstream issB(b);
        std::string usernameA, usernameB;
        int scoreA, scoreB;
        int timeA, timeB;
        issA >> usernameA >> scoreA >> timeA;
        issB >> usernameB >> scoreB >> timeB;
        if (scoreA == scoreB)
        {
            return timeA < timeB;
        }
        return scoreA > scoreB;
        });

    // 将排序好的数据写入record.txt中
    std::ofstream outputFile("record.txt");
    for (const std::string& line : lines)
    {
        outputFile << line << std::endl;
    }
    outputFile.close();
}


/*
    负责人：佐瑞峰
    功能：通过对比蛇已经吃到的食物数量和通过当前关卡所需吃到的食物数量来判断是否过关
    输入：关卡对象stage和蛇对象snake
    输出：true/false
*/
bool isWin(Snake& snake, Stage& stage)
{
    if (snake.getEatenFood() == stage.getStagePassStandard())
        return true;
    return false;
};


//组长负责
Widget::Widget(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
{
}

int Widget::getX() const
{
    return x;
}

int Widget::getY() const
{
    return y;
}

int Widget::getWidth() const
{
    return width;
}

int Widget::getHeight() const
{
    return height;
}

void Widget::setX(int x)
{
    this->x = x;
    show();
}

void Widget::setY(int y)
{
    this->y = y;
    show();
}

void Widget::setWidth(int width)
{
    this->width = width;
    show();
}

void Widget::setHeight(int height)
{
    this->height = height;
    show();
}

void Widget::setRect(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    show();
}
/**************************Button类函数实现****************************************************************************************/
//
//
Button::Button(int x, int y, int width, int height, const std::wstring& text) : Widget(x, y, width, height), text(text), isChange(false)
{
}

void Button::show() const
{
    // 设置样式
    setlinestyle(PS_SOLID, 2);
    setfillcolor(CYAN);//CYAN青色
    settextstyle(25, 0, L"微软雅黑");

    // 绘制按钮
    fillrectangle(x, y, x + width, y + height);

    // 绘制文本
    RECT rect = { x, y, x + width, y + height };
    drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool Button::state(const ExMessage& msg)
{
    if (msg.message == WM_MOUSEMOVE && isIn(msg))	// 按钮悬浮
    {
        // 设置样式
        setlinestyle(PS_SOLID, 2);
        setfillcolor(LIGHTBLUE);
        settextstyle(25, 0, L"微软雅黑");

        // 绘制按钮
        fillrectangle(x, y, x + width, y + height);

        // 绘制文本
        RECT rect = { x, y, x + width, y + height };
        drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        isChange = true;
        return false;
    }
    else if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONUP) && isIn(msg))	// 按钮被点击
    {
        // 设置样式
        setlinestyle(PS_SOLID, 2);
        setfillcolor(LIGHTBLUE);
        settextstyle(25, 0, L"微软雅黑");

        // 绘制按钮
        fillrectangle(x, y, x + width, y + height);

        // 绘制文本
        RECT rect = { x, y, x + width, y + height };
        drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        isChange = true;
        return true;
    }
    else	// 在按钮状态改变的时候，恢复按钮原来的状态，防止屏闪
    {
        if (isChange)
        {
            show();
            isChange = false;
        }
        return false;
    }
}

const std::wstring& Button::getText() const
{
    return text;
}

void Button::setText(const std::wstring& text)
{
    this->text = text;
}

bool Button::isIn(const ExMessage& msg) const
{
    if (msg.x >= this->x && msg.x <= this->x + width && msg.y >= this->y && msg.y <= this->y + height)
    {
        return true;
    }
    return false;
}
/**************************Window类函数实现****************************/
//
//
//
Window::Window(int width, int height) :
    Widget(0, 0, width, height),
    hWnd(initgraph(width, height))
{
    SetWindowText(hWnd, L"贪吃蛇--击剑大师");//修改窗口标题栏内容
    setbkcolor(WHITE);
    cleardevice();
    ShowWindow(hWnd, SW_HIDE);//指定窗口的显示状态

    // 设置绘图样式
    LOGFONT f;
    gettextstyle(&f);
    f.lfQuality = DEFAULT_QUALITY;
    settextstyle(&f);
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    setlinecolor(BLACK);

    // 创建按钮
    //home按钮
    homePageLog = new Button(412, 220, 200, 100, L"登录");
    homePageReg = new Button(412, 350, 200, 100, L"注册");
    homePageExit = new Button(412, 478, 200, 100, L"退出");
    //登录界面
    logPageAccount = new Button(330, 164, 82, 50, L"账号:");
    logPagePassword = new Button(330, 250, 82, 50, L"密码:");
    logPageLog = new Button(412, 399, 200, 50, L"登录");
    logPageQuit = new Button(412, 487, 200, 50, L"退出登录");
    //注册界面
    regPageAccount = new Button(330, 164, 82, 50, L"账号:");
    regPagePassword = new Button(330, 250, 82, 50, L"密码:");
    regPageSure = new Button(412, 400, 200, 50, L"注册");
    regPageQuit = new Button(412, 480, 200, 50, L"退出注册");
    //菜单界面
    menuViewInfo = new Button(425, 380, 174, 59, L"游戏介绍");
    menuViewSet = new Button(425, 180, 174, 59, L"游戏设置");
    menuViewChoice = new Button(425, 80, 174, 59, L"开始游戏");
    menuViewRank = new Button(425, 280, 174, 59, L"排行榜");
    menuViewQuit = new Button(425, 480, 174, 59, L"退出登录");
    //关卡选择界面
    choicePageOne = new Button(363, 100, 299, 59, L"关卡一：沙漠绿洲");
    choicePageTwo = new Button(363, 200, 299, 59, L"关卡二：黑色城镇");
    choicePageThree = new Button(363, 300, 299, 59, L"关卡三：卫星基地");
    choicePageN = new Button(363, 400, 299, 59, L"关卡n：敬请期待");
    choicePageQuit = new Button(363, 520, 299, 59, L"返回菜单");
    //游戏设置界面
    setPageSpeed1 = new Button(412, 45, 200, 100, L"速度*1.0");
    setPageSpeed2 = new Button(412, 195, 200, 100, L"速度*1.5");
    setPageSpeed3 = new Button(412, 349, 200, 100, L"速度*2.0");
    setPageQuit = new Button(412, 501, 200, 100, L"退出设置");
    //排行榜界面
    rankPageQuit = new Button(431, 540, 162, 66, L"返回菜单");
    //游戏介绍界面
    infoPageQuit = new Button(423, 510, 162, 66, L"返回菜单");
    //游戏界面
    gamePageScore = new Button(105, 68, 70, 34, L"得分:");
    gamePageTime = new Button(344, 68, 70, 34, L"用时:");
    gamePagePause = new Button(949, 25, 50, 50, L"暂停");
    //游戏暂停界面
    pausePageBack = new Button(425, 242, 174, 59, L"继续游戏");
    pausePageQuit = new Button(425, 375, 174, 59, L"返回菜单");
    //游戏结束按钮
    endPageRe = new Button(425, 242, 174, 59, L"重新开始");
    endPageQuit = new Button(425, 375, 174, 59, L"返回菜单");
    showHomePage();	// 显示主界面

}

Window::~Window()
{
    // 销毁
    delete homePageLog;
    delete homePageReg;
    delete homePageExit;

    delete regPageSure;
    delete regPageQuit;

    delete menuViewInfo;
    delete menuViewSet;
    delete menuViewChoice;
    delete menuViewRank;
    delete menuViewQuit;


}

void Window::show() const
{
    ShowWindow(hWnd, SW_SHOW);
}

//void Window::messageLoop()
//{
//    // 开启消息循环
//    ExMessage msg;
//    while (true)
//    {
//        // 获取并判断消息
//        msg = getmessage();
//
//        // 判断显示界面
//        if (state == EnumPages::HOME_PAGE)		// 主窗口显示
//        {
//            if (homePageLog->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入登录界面
//                showLogPage();
//            }
//            else if (homePageReg->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入注册界面
//                showRegPage();
//            }
//            else if (homePageExit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//退出
//                return;
//            }
//        }
//        else if (state == EnumPages::LOG_PAGE)//登录界面
//        {
//            if (logPageLog->state(msg))
//            {//进入菜单界面
//                showMenuView();
//            }
//            else if (logPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//返回主界面
//                showHomePage();
//            }
//        }
//        else if (state == EnumPages::REG_PAGE) //注册界面
//        {
//            if (regPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//返回主界面
//                showHomePage();
//            }
//            else if (regPageSure->state(msg) && msg.message != WM_LBUTTONUP)
//            {//点击注册
//
//            }
//        }
//        else if (state == EnumPages::MENU_PAGE) //菜单界面
//        {
//            if (menuViewInfo->state(msg) && msg.message != WM_LBUTTONUP)
//            {//游戏介绍界面
//                showInfoPage();
//            }
//            else if (menuViewSet->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入游戏设置界面
//                showSetPage();
//            }
//            else if (menuViewChoice->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入关卡选择界面
//                showChoicePage();
//            }
//            else if (menuViewRank->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入排行榜界面
//                showRankPage();
//            }
//            else if (menuViewQuit->state(msg) && msg.message != WM_LBUTTONUP)	// 返回
//            {
//                // 显示主窗口
//                showHomePage();
//            }
//        }
//        else if (state == EnumPages::STAGE_PAGE)//选择关卡
//        {
//            if (choicePageOne->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入关卡1
//                gameView(50, 5000);
//            }
//            else if (choicePageTwo->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入关卡2
//                gameView(1, 0);
//            }
//            else if (choicePageThree->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入关卡3
//                gameView(1, 0);
//            }
//            else if (choicePageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//返回菜单
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::SET_PAGE)//设置界面
//        {
//            if (setPageSpeed1->state(msg) && msg.message != WM_LBUTTONUP)
//            {//点击颜色选择按钮
//
//            }
//            else if (setPageSpeed2->state(msg) && msg.message != WM_LBUTTONUP)
//            {//点击初始速度按钮
//
//            }
//            else if (setPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//点击返回菜单按钮
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::RANK_PAGE)//排行榜界面
//        {
//            if (rankPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//点击返回菜单按钮
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::INFO_PAGE)
//        {
//            if (infoPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//点击返回菜单按钮
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::GAME_PAGE)//游戏界面
//        {
//            if (gamePagePause->state(msg) && msg.message != WM_LBUTTONUP)
//            {//进入暂停界面
//                toolView();
//            }
//        }
//        else if (state == EnumPages::TOOL_PAGE)
//        {
//            if (pausePageBack->state(msg) && msg.message != WM_LBUTTONUP)
//            {//返回游戏
//                gameView(1, 0);
//            }
//            else if (pausePageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//返回主菜单
//                showMenuView();
//            }
//        }
//    }
//}

void Window::showHomePage()//主界面
{

    state = EnumPages::HOME_PAGE;
    cleardevice();

    // 加载背景图片
    loadimage(NULL, bkHome, 1024, 640);

    // 绘制提示文字
    RECT rect = { 0, 0, width, 200 };
    settextstyle(50, 0, L"微软雅黑");
    drawtext(L"贪吃蛇--击剑大师", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 显示按钮
    homePageLog->show();
    homePageReg->show();
    homePageExit->show();
}
void Window::showLogPage()//登录界面
{
    state = EnumPages::LOG_PAGE;//更新目前是哪个界面
    cleardevice();

    loadimage(NULL, bkLog, 1024, 640);//加载背景图片

    setfillcolor(WHITE);//设置输入框填充颜色
    setfillstyle(BS_SOLID);//设置输入框填充方式
    fillrectangle(412, 164, 412 + 200, 164 + 50);//绘制账号输入框矩形
    fillrectangle(412, 250, 412 + 200, 250 + 50);//绘制密码输入框矩形
    //显示按钮
    logPageAccount->show();
    logPageLog->show();
    logPagePassword->show();
    logPageQuit->show();

}
void Window::showRegPage()//注册界面
{
    state = EnumPages::REG_PAGE;
    cleardevice();

    // 加载背景图片
    loadimage(NULL, bkReg, 1024, 640);

    setfillcolor(WHITE);//设置输入框填充颜色
    setfillstyle(BS_SOLID);//设置输入框填充方式
    fillrectangle(412, 164, 412 + 200, 164 + 50);//绘制账号输入框矩形
    fillrectangle(412, 250, 412 + 200, 250 + 50);//绘制密码输入框矩形
    // 显示控件
    regPageAccount->show();
    regPagePassword->show();
    regPageSure->show();
    regPageQuit->show();
}
void Window::showMenuView()//菜单界面
{
    state = EnumPages::MENU_PAGE;
    cleardevice();

    // 加载背景图片
    loadimage(NULL, bkMenu, 1024, 640);

    // 显示控件
    menuViewInfo->show();
    menuViewSet->show();
    menuViewChoice->show();
    menuViewRank->show();
    menuViewQuit->show();
}
void Window::showChoicePage()//选择关卡界面
{
    state = EnumPages::STAGE_PAGE;
    cleardevice();

    // 加载背景图片
    loadimage(NULL, bkChoice, 1024, 640);

    //显示控件
    choicePageOne->show();
    choicePageTwo->show();
    choicePageThree->show();
    choicePageN->show();
    choicePageQuit->show();
}
void Window::showSetPage()//游戏设置界面
{
    state = EnumPages::SET_PAGE;
    cleardevice();

    // 加载背景图片
    loadimage(NULL, bkSet, 1024, 640);
    //显示控件
    setPageSpeed1->show();
    setPageSpeed2->show();
    setPageSpeed3->show();
    setPageQuit->show();
}
void Window::showRankPage()//排行榜界面
{
    state = EnumPages::RANK_PAGE;
    cleardevice();
    // 加载背景图片
    loadimage(NULL, bkRank, 1024, 640);

    setbkmode(TRANSPARENT);//设置输入框背景
    setfillstyle(BS_NULL);//设置输入框填充方式
    fillrectangle(274, 75, 274 + 477, 75 + 54);//绘制框1
    fillrectangle(274, 129, 274 + 477, 129 + 54);//绘制框2
    fillrectangle(274, 183, 274 + 477, 183 + 54);//绘制框3
    fillrectangle(274, 237, 274 + 477, 237 + 54);//绘制框4
    fillrectangle(274, 291, 274 + 477, 291 + 54);//绘制框5
    fillrectangle(274, 345, 274 + 477, 345 + 54);//绘制框6
    fillrectangle(274, 399, 274 + 477, 399 + 54);//绘制框7
    fillrectangle(274, 453, 274 + 477, 453 + 54);//绘制框8
    setfillstyle(BS_SOLID);//设置输入框填充方式
    //显示控件
    rankPageQuit->show();
    std::ifstream file("record.txt");
    std::string line;
    int i = 0;
    while (std::getline(file, line))
    {
        ++i;
        if (i > 8)  break;
        printText(sToWs(line), 280, 75 + (i - 1) * 54, 40);
    }

    file.close();

}
void Window::showInfoPage()//游戏介绍界面
{
    state = EnumPages::INFO_PAGE;
    cleardevice();
    // 加载背景图片
    loadimage(NULL, bkInfo, 1024, 640);

    setlinestyle(PS_NULL);
    rectangle(111, 79, 111 + 104, 79 + 29);
    RECT rect = { 111,79,111 + 104,79 + 29 };
    drawtext(L"游戏背景:", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rectangle(215, 79, 215 + 579, 79 + 141);
    rect = { 215, 79, 215 + 579, 79 + 141 };
    drawtext(L"某一个时间，可以是任何一个时间，不论是过去、现在或是梦境里。某一个时间，故事更能表达真理，甚至比真理还值得回味。书中收录了二十二篇短篇故事，每一篇图文都精心筛选编排，以诗歌散文形式呈现出在很久很久以前的某一个时间，关于爱情、生活的幻化故事和永恒真理。",
        &rect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

    rectangle(111, 228, 111 + 104, 228 + 29);
    rect = { 111, 228, 111 + 104, 228 + 29 };
    drawtext(L"规则介绍:", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rectangle(215, 228, 215 + 579, 228 + 29);
    rect = { 215, 228, 215 + 579, 228 + 29 };
    drawtext(L"贪吃蛇共三个关卡，分值高的进入排行榜，相同分值比较时间。", &rect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

    rectangle(111, 278, 111 + 104, 278 + 29);
    rect = { 111, 278, 111 + 104, 278 + 29 };
    drawtext(L"开发成员:", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rectangle(215, 278, 215 + 579, 278 + 92);
    rect = { 215, 278, 215 + 579, 278 + 92 };
    drawtext(L"蒋恒新-组长，佐瑞峰-副组长，杨竞博-副组长，侯顺智-技术官，吴相龙-技术官，严主平-技术官，王海新-产品经理，程自豪-产品经理，吴昊伦-信息官，丁伟民-监督官",
        &rect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
    infoPageQuit->show();
}
void Window::gameView(int gameScore, int gameTime, EnumCurStage cur_stage)
{
    state = EnumPages::GAME_PAGE;
    cleardevice();
    // 加载背景图片
    if (cur_stage == FIRST_STAGE)//填条件
        loadimage(NULL, bkOne, 1024, 640);
    else if (cur_stage == SECOND_STAGE)//填条件
        loadimage(NULL, bkTwo, 1024, 640);
    else if (cur_stage == THIRD_STAGE)//填条件
        loadimage(NULL, bkThree, 1024, 640);

    std::wostringstream oss;
    oss << gameScore;
    std::wstring wstr = oss.str();
    printText(wstr, 190, 68, 40);
    std::wostringstream oss2;
    oss2 << gameTime;
    std::wstring wstr1 = oss2.str();
    printText(wstr1, 430, 68, 40);


    setorigin(105, 107);//设置游戏地图左上角为原点
    //setaspectratio(10, 10);
    for (int i = 0; i < MAXH; i++)
    {
        for (int j = 0; j < MAXL; j++)
        {
            if (map[i][j] == 0)
            {
                setfillcolor(BLACK);
                //putpixel(j, i, BLACK);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 1)
            {//蛇身
                setfillcolor(RED);
                //putpixel(j, i, RED);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 2)
            {//食物
                setfillcolor(YELLOW);
                //putpixel(j, i, YELLOW);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 3)
            {//好道具
                setfillcolor(LIGHTBLUE);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 4)
            {//坏道具
                setfillcolor(LIGHTGRAY);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 5)
            {
            }
        }
    }
    //setaspectratio(1, 1);
    setorigin(0, 0);//设返回设置
    gamePageScore->show();
    gamePageTime->show();
    //  gamePagePause->show();
}
void Window::toolView()//游戏暂停界面
{
    state = EnumPages::TOOL_PAGE;
    cleardevice();
    // 加载背景图片
    loadimage(NULL, bkPause, 1024, 640);

    pausePageBack->show();
    pausePageQuit->show();

}
void Window::endPage()//游戏结束界面
{
    state = EnumPages::END_PAGE;
    cleardevice();
    // 加载背景图片
    loadimage(NULL, bkEnd, 1024, 640);

    endPageRe->show();
    endPageQuit->show();
}
void Window::printText(std::wstring text, int x, int y, int size)
{
    settextstyle(size, 0, L"微软雅黑");
    outtextxy(x, y, text.c_str());
}

void runGame() {
    Player player; //开始游戏，创建player对象
    player.setUsername(usernameToPrint); //载入玩家的用户名
    player.setPassword(passwordToPrint); //载入玩家的密码
    player.playerDataUpdate(gameScore, gameTime); //载入玩家的分数和耗时
    snakeBody[1][0] = HH / 2;
    snakeBody[1][1] = WW / 2;

    for (EnumCurStage i = curStage; i <= THIRD_STAGE; ++i) {
        Stage stage(i, gap[i], passStandard[i], initSpeed[i], barrierNum[i]); //创建第i关的关卡对象
        if (!continueFlag) stage.InitStage(); // 不是通过暂停界面回来的，需要初始化关卡
        Snake snake(curEatenFood, curLength, curX, curY, curSnakeDirection); //恢复蛇对象

        /*    cout << " snakeBody[0][0]: " << snakeBody[0][0] << " snakeBody[0][1]: " << snakeBody[0][1] << '\n';
            cout << " snakeBody[1][0]: " << snakeBody[1][0] << " snakeBody[1][1]: " << snakeBody[1][1] << '\n';
            cout << "map[snakeBody[1][0]][snakeBody[1][1]] : " << map[snakeBody[1][0]][snakeBody[1][1]] << '\n';
            cout << "map[snakeBody[0][0]][snakeBody[0][1]] : " << map[snakeBody[0][0]][snakeBody[0][1]] << '\n';*/
        ifFirstTime = true;
        while (1)
        {
            stage.mapGenerate(ifEaten, ifFirstTime); //生成边界，障碍物，食物，道具，蛇的身体，即map数组
            //cout << "mapGenerate has done" << endl;

            w.gameView(player.getTotalScore(), player.getTotalTime(), i);//显示地图，蛇，食物/道具，工具栏和得分耗时
            //cout << "gameview has done" << endl;

            gameEventCheck(snake, stage); //检查用户输入，完成暂停/更新蛇的方向
            //cout << "gameeventCheck has done" << endl;

            if (pageFlag == TOOL_PAGE)//判断是否需要跳转到工具栏页面
            {
                curStage = i; //记录当前在第几关
                snake.saveSnake();//将当前的蛇的参数保存到全局变量
                gameScore = player.getTotalScore(); //保存游戏分数
                gameTime = player.getTotalTime(); //保存游戏时间
                continueFlag = 1; //将继续标志修改为1
                return;
            }

            //cout << " snake.dir : " << snake.dir << '\n';

            /*cout << " snakeBody[0][0]: " << snakeBody[0][0] << " snakeBody[0][1]: " << snakeBody[0][1] << '\n';
            cout << " snakeBody[1][0]: " << snakeBody[1][0] << " snakeBody[1][1]: " << snakeBody[1][1] << '\n';
            cout << "map[snakeBody[1][0]][snakeBody[1][1]] : " << map[snakeBody[1][0]][snakeBody[1][1]] << '\n';
            cout << "map[snakeBody[0][0]][snakeBody[0][1]] : " << map[snakeBody[0][0]][snakeBody[0][1]] << '\n';*/
            //cout << "yeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;

            snake.getDir(snake.dir, stage);
            switch (map[snakeBody[0][0]][snakeBody[0][1]]) //根据下一个要移动到的位置有什么来更新snake,stage,player的状态
            {
            case 0:
                pageFlag = END_PAGE; // 准备进入结束界面
                //cout << "碰到边界 \n";
                break;
            case 1:
                pageFlag = END_PAGE; // 准备进入结束界面
                //cout << "碰到蛇身 \n";
                break;
            case 2:
                snake.eatFood(stage, player); //吃到食物，更新snake,stage和player的状态
                ifEaten = true;
                break;
            case 3:
                snake.eatGoodProp(stage, player);//吃到好道具，更新snake,stage和player的状态
                ifEaten = true;
                break;
            case 4:
                snake.eatBadProp(stage, player);//吃到坏道具，更新snake,stage和player的状态
                ifEaten = true;
                break;
            case 5:
                snake.normalMove(stage, player);//啥都没吃到，更新snake,stage和player的状态
                ifEaten = false;
                //cout << "normalMove 正常移动了一下\n";
                break;
            }
            // cout << "Last1" << endl;
            if (isWin(snake, stage)) {
                //cout << "now we are in isWin" << endl;
                break; //当前是否过关，过关了跳出while循环以进入下一关
            }
            if (pageFlag == END_PAGE) {
                //cout << "now we are in endPage" << endl;
                fileWriteAndRank(player);//游戏结束，更新记录
                gameScore = 0;//游戏结束，记录完后分数清零
                gameTime = 0;//游戏结束，记录完后分数清零
                return; //切换游戏结束界面
            }
            ifFirstTime = false;
            //cout << "Last3" << endl;
        }
    }
    fileWriteAndRank(player); //顺利通关，更新文件中的记录
    gameScore = 0;//顺利通关，记录完后分数清零
    gameTime = 0; //顺利通关，记录完后耗时清零
    pageFlag = END_PAGE; //顺利通关，准备进入结束界面
}

void controller()
{
    w.show();
    while (1)
    {
        switch (pageFlag)
        {
        case HOME_PAGE:
            while (1) {
                w.showHomePage();
                homeClickCheck(); //函数内部需要循环
                if (pageFlag != HOME_PAGE) break;
            }
            break;
        case LOG_PAGE:
            while (1) {
                w.showLogPage();
                logClickCheck(); //函数内部需要循环
                if (pageFlag != LOG_PAGE) break;
            }
            break;
        case REG_PAGE:
            while (1) {
                w.showRegPage();
                regClickCheck(); //函数内部需要循环
                if (pageFlag != REG_PAGE) break;
            }
            break;
        case MENU_PAGE:
            while (1) {
                w.showMenuView();
                menuClickCheck(); //函数内部需要循环
                if (pageFlag != MENU_PAGE) break;
            }
            break;
        case INFO_PAGE:
            while (1) {
                w.showInfoPage();
                infoClickCheck(); //函数内部需要循环
                if (pageFlag != INFO_PAGE) break;
            }
            break;
        case SET_PAGE:
            while (1) {
                w.showSetPage();
                setClickCheck(); //函数内部需要循环
                if (pageFlag != SET_PAGE) break;
            }
            break;
        case STAGE_PAGE:
            while (1) {
                w.showChoicePage();
                stageClickCheck(); //函数内部需要循环
                if (pageFlag != STAGE_PAGE) break;
            }
            break;
        case GAME_PAGE:
            runGame();
            break;
        case TOOL_PAGE:
            while (1) {
                w.toolView();
                toolClickCheck(); //函数内部需要循环
                if (pageFlag != LOG_PAGE) break;
            }
            break;
        case END_PAGE:
            while (1) {
                w.endPage();
                endClickCheck(); //函数内部需要循环
                if (pageFlag != END_PAGE) break;
            }
            break;
        case RANK_PAGE:
            while (1) {
                w.showRankPage();
                rankClickCheck(); //函数内部需要循环
                if (pageFlag != RANK_PAGE) break;
            }
            break;
        }
    }
}

int main()
{
    controller();
    return 0;
};