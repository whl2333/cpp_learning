// Snake_WuhaoLun.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
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
//����ͷ�ļ������ﲹ��
using namespace std;
#define HH 50  //����
#define WW 80  //����
#define MAXLEN 50  //����
#define MAXH 50
#define MAXL 80

// -------------------- ������� --------------------
/*
    map[i][j]��ʾ��Ϸ�����һ�����꣬
    ����Ǳ߽���0��ǣ�
    �����������1��ǣ�
    �����ʳ����2��ǣ�
    ����Ǻõ�����3��ǣ�
    ����ǻ�������4���
    ����ǿյ���5��ǡ�
*/
int map[HH][WW];

//��Ҫ����һ������ע���û����ļ�user.txt��һ��������Ϸ��¼�Ŀ��ļ�record.txt

/*
    snakebody[i]��ʾ��i���ߵ����������
    MAXLEN��ʾ�ߵ���󳤶ȣ��±�0�洢Ҫ�ƶ�������һ��λ�õ����꣬�����᷽�����ƶ���ʵ��
    ���統ǰ�ߵĳ�����3����Ӧ�ľ����±�1~3���������꣬���±�4��ʼ������Ͷ���ֵΪ-1
*/
int snakeBody[MAXLEN + 1][2] = { 0 };
//snakeBody[1][0] = 40;
// snakeBody[1][1] = 25;
/*
    ��ʾ��������ƶ����򣬴����ҷֱ��Ӧ��������
*/
const int direction[4][2] = { {-1, 0}, {1, 0} ,{0, -1}, {0, 1} };

/*
    ��ʾ�ƶ���������������ݽṹ���Է���ض�����ǰ������ƶ��ģ�����W��Ӧ0����direciton[0]���ö�Ӧ���ϵ������ƶ�
*/
enum EnumDirection {
    W = 0, S, A, D
};

/*
    ��¼ÿһ�س��ֵ�������Ҫ�ļ���ִΣ�Խ������Ĺؿ�����ִ�Խ��
*/
const int gap[] = { 0, 5, 4, 3 };

/*
    ��¼ͨ��ÿһ������Ҫ��ʳ������,
*/
const int passStandard[] = { 0, 5, 10, 15 };

/*
    ��¼������ÿһ�صĳ�ʼ�ϰ�������
*/
const int barrierNum[] = { 0, 0, 2, 4 };

/*
    ��¼������ÿһ�صĳ�ʼ�ٶ�
*/
const float initSpeed[] = { 0, 1, 1.5, 2 };

/*
    ����ϵ����Ĭ��Ϊ1���������ý������
*/
int acc = 1;

/*
    ������Ϸ����ʱ���ڱ�ʶ����ͣ�����Ļ�����������ģ���Ϊ����Ӧ��ͬ�ĳ�ʼ��������Ĭ�������������
*/
int continueFlag = 0;

/*
    ��¼ĳ�����ĳ����Ϸ�����е��ۼƵ÷֣����ڻָ�Player����
*/
int gameScore = 0;

/*
    ��¼ĳ�����ĳ����Ϸ�����е��ۼƺ�ʱ�����ڻָ�Player����
*/
int gameTime = 0;


/*
    len,x,y,eatenfood,direction����ȫ�ֱ��������ڼ�¼�ߵ�״̬��������Щ״̬�Ϳ��ڼ�����Ϸ��ָ�ԭ������
*/
int curLength = 3;
int curX = WW / 2, curY = HH / 2;
int curEatenFood = 0;
EnumDirection curSnakeDirection = EnumDirection::D; // Ĭ�Ϸ�������

bool ifFirstTime, ifEaten = false;


/*
    �����û�������û��������룬���ں�������Ļ�ϵ���ʾ
*/
std::string usernameToPrint;
std::string passwordToPrint;
wstring sToWs(const string& s) {
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(s);
}
/*
    pageflag��ȫ�ֱ��������ڱ�ʶҪ�����ҳ��,����HomePage�ʹ�������ҳ
*/
enum EnumPages {
    HOME_PAGE = 0, REG_PAGE, LOG_PAGE, MENU_PAGE, INFO_PAGE,
    RANK_PAGE, STAGE_PAGE, SET_PAGE, GAME_PAGE, TOOL_PAGE, END_PAGE
}; EnumPages pageFlag;

/*
    textflag��ȫ�ֱ��������ڰ����Ϸ����б���isLegal�������ֵ�ǰ������ַ������˺Ż������룬
    ��Ϊ�˺ź�����ĺϷ����б�����ǲ�ͬ�ġ�
*/
enum EnumTextAttr { USERNAME = 0, PASSWORD }; EnumTextAttr textFlag;

/*
    curstageȫ�ֱ��������ڱ�ʶҪ����ڼ���,��Ϊÿһ�صĲ�����ͬ������Ҫ��ʶ
*/
enum EnumCurStage { FIRST_STAGE = 1, SECOND_STAGE, THIRD_STAGE }; EnumCurStage curStage;

constexpr auto BACKGROUND_IMAGE = L"background.jpg";
LPCTSTR bkHome = _T("./������.jpg");
LPCTSTR bkLog = _T("./��¼����.jpg");
LPCTSTR bkMenu = _T("./�˵�����.jpg");
LPCTSTR bkReg = _T("./ע�����.jpg");
LPCTSTR bkChoice = _T("./ѡ�ؽ���.jpg");
LPCTSTR bkSet = _T("./��Ϸ���ý���.jpg");
LPCTSTR bkRank = _T("./���а����.jpg");
LPCTSTR bkInfo = _T("./��Ϸ���ܽ���.jpg");
LPCTSTR bkOne = _T("./ɳĮ����.jpg");
LPCTSTR bkTwo = _T("./��ɫ����.jpg");
LPCTSTR bkThree = _T("./���ǻ���.jpg");
LPCTSTR bkPause = _T("./��ͣ����.jpg");
LPCTSTR bkEnd = _T("./��Ϸ��������.jpg");

class Widget
{
public:
    Widget(int x = 0, int y = 0, int width = 1024, int height = 640);
    void setRect(int x, int y, int width, int height);  // ���ÿؼ�����
    virtual void show() const = 0;                      // ��ʾ����(���麯��)

    int getX() const;
    int getY() const;
    int getHeight() const;
    int getWidth() const;
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);

protected:
    int x;        // X ����
    int y;        // Y ����
    int width;    // ���
    int height;   // �߶�
};

class Button :public Widget {
public:
    Button(int x = 0, int y = 0, int width = 100, int height = 60, const std::wstring& text = L"��ť");
    void show() const;                      // ��ʾ��ť
    bool state(const ExMessage& msg);       // ��ť״̬��true ��ʾ��ť����
    const std::wstring& getText() const;    // ��ȡ��ť�ı�
    void setText(const std::wstring& text); // ���ð�ť�ı�

protected:
    bool isIn(const ExMessage& msg) const;  // �ж�����Ƿ��ڰ�ť����
    std::wstring text;                      // ��ť�ı�
    bool isChange;                          // ��ť״̬�Ƿ�ı�
};

class Window :public Widget {
public:
    Window(int width = 1024, int height = 640);
    ~Window();

    void show() const;                          // ��ʾ����
    //void messageLoop();                         // ��Ϣѭ��
    void showHomePage();

    void showMenuView();
    void showRegPage();
    void showLogPage();
    void showChoicePage();
    void showSetPage();
    void showRankPage();
    void showInfoPage();
    void gameView(int gameScore, int gameTime, EnumCurStage cur_stage);
    void toolView();//��Ϸ��ͣ����
    void endPage();//��Ϸ��������
    void printText(std::wstring text, int x, int y, int size);

public:
    // ����״̬��ʶ��ÿ��״̬������һ������
    //enum class WindowState
    //{
    //	homePage,//������
    //	menuView,//�˵�����
    //	regPage,//ע�����
    //	logPage,//��¼����
    //	infoPage,//��Ϸ���ܽ���
    //	rankPage,//���а����
    //	setPage,//��Ϸ���ý���
    //	stagePage,//ѡ��ؿ�����
    //	pausePage,//��Ϸ��ͣ����
    //	gamePage,//��Ϸ����
    //	endPage,//��Ϸ��������
    //	//onePage,//�ؿ�һ����
    //	//twoPage,//�ؿ�������
    //	//threePage,//�ؿ�������
    //};

private:
    // home��ť
    Button* homePageLog;
    Button* homePageReg;
    Button* homePageExit;
    //��¼���水ť
    Button* logPageAccount;
    Button* logPagePassword;
    Button* logPageLog;
    Button* logPageQuit;
    // ע����水ť
    Button* regPageAccount;
    Button* regPagePassword;
    Button* regPageSure;
    Button* regPageQuit;
    //�˵����水ť
    Button* menuViewInfo;
    Button* menuViewSet;
    Button* menuViewChoice;
    Button* menuViewRank;
    Button* menuViewQuit;
    //�ؿ�ѡ����水ť
    Button* choicePageOne;
    Button* choicePageTwo;
    Button* choicePageThree;
    Button* choicePageN;
    Button* choicePageQuit;
    //��Ϸ���ý��水ť
    Button* setPageSpeed1;
    Button* setPageSpeed2;
    Button* setPageSpeed3;
    Button* setPageQuit;
    //���а���水ť
    Button* rankPageQuit;
    //��Ϸ���ܽ��水ť
    Button* infoPageQuit;
    //��Ϸ���水ť
    Button* gamePageScore;
    Button* gamePageTime;
    Button* gamePagePause;
    //��Ϸ��ͣ���水ť
    Button* pausePageBack;
    Button* pausePageQuit;
    //��Ϸ�������水ť
    Button* endPageRe;
    Button* endPageQuit;

    EnumPages state;  // ����״̬
    HWND hWnd;          // ���ھ��
};

Window w(1024, 640);

/*
    ��ͬ�Ĺؿ���Ӧ�Ų�ͬ��ͨ�ر�׼�ͳ�ʼ�ƶ��ٶȵȣ������б�Ҫ�Բ�ͬ�ؿ��Ĳ������й���
*/
class Stage
{
    friend class Snake;
    /*
    �����ˣ������� && ���Ժ�
     ���ģ����Ҫ����������ؿ������ݣ�����Ǳ��޸�Ϊ��һ��ʱִ�е�һ�ص����ݣ�������ʱִ�е��������ݡ�
     ����ʱ�ǰ�һ��ʱ���ĺ�һ��ʱ���ٵĸ��Ƿ��䵽һ�����������˿������ɷ��䡣
    */
private:
    int curStage; // ��ǰ���ڵĹؿ�
    int propGap; // ÿ�������ִ�����һ������
    int curLoop; //�ؿ��ڵ��ִΣ���ÿ�Ե�һ������/ʳ��ִξ�Ҫ��1
    int stagePassStandard; //ͨ����ǰ�ؿ�����Ե���ʳ������
    int stageSpeed; //�ؿ���ʼ���ƶ��ٶ�
    int stageBarrierNum; //��ǰ�ؿ����ϰ�����������ͬ�ؿ��ĳ�ʼֵ��ͬ��������Ҫ�������ֵ����

public:
    //���캯�������ڹ���ؿ�����
    Stage(int stage, int gap, int standard, int speed, int barNum)
        :curStage(stage), propGap(gap), stagePassStandard(standard), stageSpeed(speed), stageBarrierNum(barNum)
    {};

    //����ʵ�ֹؿ������ĳ�ʼ��
    void InitStage()
    {
        //stageScore = 0;
        //stageTimeConsuming = 0;
        curLoop = 0;
    }

    /*
        �����ˣ������� && ���Ժ�
        ���ܣ���ȡ��ǰ�ؿ��ĳ�ʼ�ٶ�
        ����: void
        �����int
    */
    int getStageSpeed()
    {
        return stageSpeed;
    }

    /*
        �����ˣ������� && ���Ժ�
        ���ܣ�����ͨ����ǰ�ؿ�Ҫ�Լ���ʳ��
        ����: void
        �����int
    */
    int getStagePassStandard()
    {
        return stagePassStandard;
    }

    /*
        �����ˣ������� && ���Ժ�
        ���ܣ���ȡ��ǰ�ؿ�������һ�������������ִ�
        ����: void
        �����int
    */
    int getPropGap()
    {
        return propGap;
    }

    /*
        �����ˣ������� && ���Ժ�
        ���ܣ����������ϰ���
        ����: �ϰ����������
        �����void
    */
    void barrierIncrease()
    {
        stageBarrierNum += 2; //�޸��ϰ��������
    }

    /*
        �����ˣ������� && ���Ժ�
        ���ܣ������ִ�
        ����: void
        �����void
    */
    void loopIncrease()
    {
        curLoop++;
    }

    /*
        �����ˣ������� && ���Ժ�
        ���ܣ��ı䵱ǰ�ؿ��ĵ÷�
        ����: int
        �����void
    */
    /* void changeScore(int change)
     {
         stageScore += change;
     }*/

     /*
         �����ˣ������� && ���Ժ�
         ���ܣ����Ӻ�ʱ
         ����: void
         �����void
     */
     //void increaseTime()
     //{
     //    stageTimeConsuming++;
     //}

     /*
         �����ˣ������� && ���Ժ�
         ���ܣ����ص�ǰ�ؿ��ĵ÷�
         ����: void
         �����int
     */
     //int getStageScore();

     /*
         �����ˣ������� && ���Ժ�
         ���ܣ����ص�ǰ�ؿ��ĺ�ʱ
         ����: void
         �����void
      */
      //int getStageTimeConsuming();


      /*
          �����ˣ������� && ���Ժ�
          ���ܣ��ж�Ҫ���ɵĵ��߻���ʳ�������λ���Ƿ���5���ǵĻ��ͺϷ������򲻺Ϸ�
          ����: ����λ��x��y
          �����true/false
      */
    bool isLegal(int x, int y) {
        if (map[x][y] == 5)
            return true;
        else return false;
    }

    /*
        �����ˣ������� && ���Ժ�
        ���ܣ�
            1.���ݵ�ǰ�ִ�CurLoop�����ɵ��ߵļ��PropGap�ж��ǹ�Ҫ���ɵ���
                �����Ҫ���ɾ������������ʹ��isLegal�ж��Ƿ�Ϸ������Ϸ�Ҫ��������
                �����ɵĺϷ������������map�����������עΪ3����4.
            1.����ʳ���������꣬ʹ��isLegal�ж��Ƿ�Ϸ������Ϸ�Ҫ��������
            2.�����ɵĺϷ������������map�����ϱ�עΪ2
            3.�����ϰ��������������Ӧ������������꣬ÿ����һ������Ҫ�ж��Ƿ�Ϸ������Ϸ�Ҫ�������ɡ�
            3.�����ɵĺϷ������������map�����ϱ�עΪ0
            3.��snakeBody�����¼�������עΪ1
        ���룺void
        �����void
    */
    void mapGenerate(bool ifEaten, bool ifFirstTime)
    {
        if (ifEaten == true || ifFirstTime == true)
        {
            memset(map, 0, sizeof(map)); //ÿ��Ҫ���³�ʼ��map
            for (int i = 1; i < HH - 1; i++)
            {
                for (int j = 1; j < WW - 1; j++)
                {
                    map[i][j] = 5;
                }
            }

            int x = rand() % WW;   //����x
            int y = rand() % HH;   //����y
            int num = rand() % 2 + 3;   //�����3|4

            //��������
            if ((this->curLoop) % (this->propGap) == 1)
            {
                while (!isLegal(x, y))
                {
                    x = rand() % WW;
                    y = rand() % HH;
                }
                map[x][y] = num;
            }

            //ʳ������
            while (!isLegal(x, y))
            {
                x = rand() % WW;
                y = rand() % HH;
            }
            map[x][y] = 2;

            //�ϰ�������
            for (int i = 0; i < this->stageBarrierNum; i++)
            {
                while (!isLegal(x, y))
                {
                    x = rand() % WW;
                    y = rand() % HH;
                }
                map[x][y] = 0;
            }
            //��������
            for (int i = 1; i <= curLength; i++)
            {
                map[snakeBody[i][0]][snakeBody[i][1]] = 1;
            }
        }

        else {
            //�������
            for (int i = 1; i < HH - 1; i++)
            {
                for (int j = 1; j < WW - 1; j++)
                {
                    if (map[i][j] == 1) map[i][j] = 5;
                }
            }
            //���´�ӡ����
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
    �������Ҫ�Ǽ���û��˻��Ϸ��Եģ�Ȼ���鳤ʱ��Ƚ϶�����ƽʱ��Ƚ�����������������
    */
private:
    std::string username; //���û����û���
    std::string password; //���û�������
    int curTotalScore; //�ô���Ϸ���ܵ÷�
    int curTotalTime; //�ô���Ϸ���ܺ�ʱ

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
        �����ˣ���� && ����ƽ
        ���ܣ���ȡע���ļ�user.txt���жϵ�ǰ���û����������ڸ��ļ����Ƿ���ڡ�
        ���룺void
        �����true/false
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
        �����ˣ���� && ����ƽ
        ���ܣ��жϵ�ǰ���û���/�������Ƿ�Ϸ�������textflag��PassWord����UserName���ò�ͬ�ĺϷ��ж��߼�
        ���룺�ַ���str���ַ�������textFlag
        �����true/false
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
        �����ˣ���� && ����ƽ
        ���ܣ�����ǰ��Player������û����������¼д��user.txt�У�
        ���룺void
        �����void
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
       �����ˣ���� && ����ƽ
       ����:����ȫ�ֱ���gameScore��gameTime��¼�ĵ�����Ϸ���ܵ÷ֺ���ʱ����¸���Ҹô���Ϸ���ܵ÷ֺ��ܺ�ʱ(����������ȫ�ֱ���)
       ����:��ǰ��Ϸ����ʱ�ĵ÷�score��TimeConsuming
       �����void
   */
    void playerDataUpdate(int score, int timeConsuming) {
        if (score >= 0 && timeConsuming >= 0) {
            curTotalScore = score;
            curTotalTime = timeConsuming;
        }
    }


    /*
* ���ܣ���������û��ܵ÷���
*/
    int getTotalScore() {
        return this->curTotalScore;
    }

    /*
    *
    *
    * ���ܣ������û��ܹ�ʹ�õ�ʱ��
    *
    */
    int getTotalTime() {
        return this->curTotalTime;
    }

    /*
    * ���ܣ������û�������
    */
    std::string getUsername()
    {
        return this->username;
    }
};

class Snake
{
private:
    int eatenFood = 0;//�Ѿ��Ե���ʳ��
    int length = 3; //�ߵ�ǰ�ĳ���,��ʼ����Ϊ1
    int x = WW / 2, y = HH / 2; //��ͷ��ǰ��λ��


public:
    EnumDirection dir = EnumDirection::D; //��ͷ���ƶ����򣬳�ʼ����
    /*
        �����ˣ���˳��
        ���ܣ�����Snake�������snakebody����ĳ�ʼ��
        ���룺�ߵĳ��ȣ���ͷ����x��y��ǰ������d
        �����
    */
    Snake(int eatenfood, int len, int x, int y, EnumDirection d)
        :eatenFood(eatenfood), length(len), x(x), y(y), dir(d)
    {

    };

    /**
    Ĭ�Ϲ��캯��
    */
    Snake() = default;

    /*
        �����ˣ���˳��
        ���ܣ������ߵĵ�ǰ������ȫ�ֱ���curLength,curX,curY,curEatenFood,curSnakeDirection
        ���룺void
        �����void
    */
    void saveSnake();

    /*
        �����ˣ���˳��
        ���ܣ���������ķ��������ͷҪ�ƶ�������һ��λ�õ�����snakebody[0][0]��snakebody[0][1]:
                    snakeBody[0][0] = snakeBody[0][0]+direction[d][0] * stage.stageSpeed * acc��
                    snakeBody[0][1] = snakeBody[0][1]+direction[d][1] * stage.stageSpeed * acc��
              ͬʱ����ǰ������snake.d = d;
        ���룺����d,�ؿ�����stage
        �����void
    */
    void getDir(EnumDirection d, Stage& stage);


    /*
        �����ˣ���˳��
        ���ܣ�
            ���ݷ������ͷ��λ��ʵ��snakebody����ĸ��£�
            ���·�ʽ��snakebody�ĵ����ڶ������긳ֵ�����һ�����꣬�������������긳ֵ�������ڶ�������...��0�����긳ֵ����1������
            ��ʱ+1
        ���룺�ؿ�����stage
        �����void
    */
    void normalMove(Stage& stage, Player& player);

    /*
        �����ˣ���˳��
        ���ܣ����ݳԵ��ĵ��ߣ�ǰ���������ͷ��λ��ʵ��snakebody����ĸ��£�ע�������ȼ�1����
              ��ǰ����+3,ͬʱ��ǰ�ؿ����ִ�+1����ʱ+1���ִθ��º��ж��Ƿ���Ҫ�����ϰ�������Ҫ�����ӣ�
              ����stage.barrierIncrease()���ɡ�
        ���룺��ǰ���ڵĹؿ�����stage
        �����void
    */
    void eatGoodProp(Stage& stage, Player& player);

    /*
        �����ˣ���˳��
        ���ܣ����ݳԵ��ĵ��ߣ�ǰ���������ͷ��λ��ʵ��snakeBody����ĸ��£�ע�������ȼ�1����
              ��ǰ����-3,ͬʱ��ǰ�ؿ����ִ�+1����ʱ+1���ִθ��º��ж��Ƿ���Ҫ�����ϰ�������Ҫ�����ӣ�
              ����stage.barrierIncrease()���ɡ�
        ���룺
        �����
    */
    void eatBadProp(Stage& stage, Player& player);

    /*
        �����ˣ���˳��
        ���ܣ�
            ���ݷ������ͷ��λ��ʵ��snakeBody����ĸ���,������+1������+1����ʱ+1����ǰ�ؿ����ִ�+1��
            �ִθ��º��ж��Ƿ���Ҫ�����ϰ�������Ҫ�����ӣ�����stage.barrierIncrease()���ɡ�
        ���룺��ǰ���ڵĹؿ�����stage
        �����void
    */
    void eatFood(Stage& stage, Player& player);

    /*
        �����ˣ���˳��
        ���ܣ������ߵ�ǰ�ĳ���
        ���룺void
        �����void
    */
    int getLength() {
        return length;
    }
    /*
        �����ˣ���˳��
        ���ܣ������ߵ�ǰ���ƶ�����
        ���룺void
        �����void
    */
    EnumDirection getHeadDirection() {
        return dir;
    }

    /*
        �����ˣ���˳��
        ���ܣ������ߵ�ǰ�Ե���ʳ��
        ���룺void
        �����void
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

    // �����ߵķ���
    this->dir = d;

    // �ж���ͷӦ���ڵ���һ��λ��
    /*snakeBody[0][0] = snakeBody[1][0] + direction[d][0] * stage.getStageSpeed() * acc;
    snakeBody[0][1] = snakeBody[1][1] + direction[d][1] * stage.getStageSpeed() * acc;*/

    snakeBody[0][0] = snakeBody[1][0] + direction[d][0];
    snakeBody[0][1] = snakeBody[1][1] + direction[d][1];

    //std::cout << snakebody[0][0] << ' ' << snakebody[0][1] << '\n';
}

void Snake::normalMove(Stage& stage, Player& player) {
    getDir(dir, stage);
    // ��ʱ+1
    player.increaseTime();
    // ++stage.stageTimeConsuming;
    // gameTime += stage.stageTimeConsuming;

    int before_head_x = snakeBody[0][0];
    int before_head_y = snakeBody[0][1];
    int tail_x = snakeBody[length][0];
    int tail_y = snakeBody[length][1];

    // �����ߵ�λ��
    for (int i = length; i > 0; --i) {
        snakeBody[i][0] = snakeBody[i - 1][0];
        snakeBody[i][1] = snakeBody[i - 1][1];
    }

    // ������ͷλ��
    x = snakeBody[1][0];
    y = snakeBody[1][1];

    // ���ߵ�λ�÷�ӳ��map��ȥ
    map[before_head_x][before_head_y] = 1;
    map[tail_x][tail_y] = 5;

    // �����ߵ�״̬
    saveSnake();
}
//void Snake::normalMove(Stage& stage, Player& player) {
//    getDir(dir, stage);
//    // ��ʱ+1
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
//    // �����ߵ�λ��
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
//    // ������ͷλ��
//    x = snakeBody[1][0];
//    y = snakeBody[1][1];
//    snakeBody[1][0] = snakeBody[0][0];
//    snakeBody[1][1] = snakeBody[0][1];
//
//    // ���ߵ�λ�÷�ӳ��map��ȥ
//    //map[before_head_x][before_head_y] = 1;
//    //map[tail_x][tail_y] = 5;
//}



void Snake::eatFood(Stage& stage, Player& player) {

    // �����ߵ��ƶ�
    int tail_x = snakeBody[length][0];
    int tail_y = snakeBody[length][1];
    normalMove(stage, player);
    ++length;
    ++eatenFood;
    // �ߵĳ���+1�����һ��λ��Ӧ����ԭ�������һ��λ��
    snakeBody[length][0] = tail_x;
    snakeBody[length][1] = tail_y;

    // �����ݽ��и���
    // ++stage.stageScore;
    player.changeScore(1);
    stage.loopIncrease();

    // ���ߵ�λ�÷�ӳ��map��ȥ
    map[snakeBody[length][0]][snakeBody[length][1]] = 1;

    //std::cout << "stage.CurLoop:" << stage.CurLoop << '\n';
    // �Ƿ���Ҫ�����ϰ���
    if (stage.curLoop % 3 == 0) {
        //std::cout << "������һ���ϰ���\n";
        stage.barrierIncrease();
    }
    saveSnake();
}

void Snake::eatGoodProp(Stage& stage, Player& player) {
    eatFood(stage, player);
    // stage.stageScore += 2;
    player.changeScore(2);
    saveSnake();
    //std::cout << "��ǰ������" << stage.score << " �ؿ����ִΣ�"<< stage.CurLoop << " ��ʱ�� " << stage.TimeConsuming << '\n';
}

void Snake::eatBadProp(Stage& stage, Player& player) {
    eatFood(stage, player);
    // stage.stageScore -= 4;
    gameScore -= 4;
    saveSnake();
    //std::cout << "��ǰ������" << stage.score << " �ؿ����ִΣ�" << stage.CurLoop << " ��ʱ�� " << stage.TimeConsuming << '\n';

}

// -------------------- ������� --------------------





// -------------------- view --------------------
/*
    �����ˣ������� && ��ΰ��

    ��Ϊ���������ȷʵ���٣��ͷ��������ˡ�Ȼ��ΰ����ǲ���Ҫ������ĩ���ʱ��Ͷ����㣬ûʱ��Ͷ�д�㡣

    ���ܣ�
          ��ʾ��Ϸ���ܰ�ť
          ��ʾ��Ϸ���ð�ť
          ��ʾ�ؿ�ѡ��ť
          ��ʾ���а�ť
          ��ʾ�˳���½��ť
    ���룺void
    �����void
*/
void menuView();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ�չʾ��½��ť��ע���û���ť���˳���Ϸ��ť
*/
void homePage();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ�չʾ�����˺ŵĿ���������Ŀ�,ע�ᰴť�ͷ��ذ�ť
    ���룺void
    �����void
*/
void regPage();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ�չʾ�����˺ŵĿ���������Ŀ򣬵�½��ť�ͷ��ذ�ť
    ���룺void
    �����void
*/
void logPage();


/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ���ʾ��Ϸ���ܺͿ�����Ա����,�������ؼ�
    ���룺void
    �����void
*/
void infoPage();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ���ȡrecord.txt�ļ�����ʾǰ8�����û����û�������ʱ�͵÷֣���ʾ���ؼ�
    ���룺void
    �����void
*/
void rankPage();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ���ʾ���ý��棬�ֱ���*1.0,*1.5,��*2.0�ļ��ٰ�ť�Լ�1���������˵��İ�ť
    ���룺void
    �����void
*/
void setPage();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ���ʾ��һ�صİ�ť���ڶ��صİ�ť�������صİ�ť�ͷ������˵��İ�ť
    ���룺void
    �����void
*/
void stagePage();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ�
        ��ʾ��������ť��ʹ��printText(...)��ʾ��ǰ��ҵķ����ͺ�ʱ������map��Ⱦ����Ϸ���棬
        map��ȫ�����飬����ֱ�ӵ���
    ���룺��ǰ�ܵ÷�gameScore����ǰ�ܺ�ʱgameTime
    �����void
*/
//void gameView(int gameScore, int gameTime);

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ���ʾһ��д��"������Ϸ"�İ�ť��һ��д��"�������˵�"�İ�ť
    ���룺void
    �����void
*/
void toolView();
/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ�
        1.��ʾ"�������˵�"��ť
        2.��ʾ"���¿�ʼ"��ť
    ���룺void
    �����void
*/
void gamePage();

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ���ָ��λ������ض��ı�
    ���룺�ַ�������text����ʼд������x��y�������Сsize
    �����void
*/
//void printText(std::string text, int x, int y, int size);

/*
    �����ˣ������� && ��ΰ�񣨾���ѡ��
    ���ܣ�
        1.��ʾ"�������˵�"��ť
        2.��ʾ"���¿�ʼ"��ť
    ���룺void
    �����void
*/
void endPage();
// -------------------- view --------------------

// -------------------- service --------------------

/*
    �����ˣ������ && ������(������)

    �����������Ҫ�Ǹ��������ת�ģ�ֻҪ�ɱ��Ϊ�޸Ĺ������С�������������
    ���ܣ�
        1.�㵽ע�ᰴť��pageFlag = REG_PAGE��
        2.�㵽��½��ť��pageFlag = LOG_PAGE��
        3.�㵽�˳���ť��exit��0��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void homeClickCheck();

/*
    �����ˣ������ && ����������������
    ���ܣ�
        ������ĵ�����������
        1.���а�ť����pageFlag = RANK_PAGE��
        2.�ؿ���ť����pageFlag = STAGE_PAGE��
        3.���ܰ�ť����pageFlag = INFO_PAGE��
        4.�˳���½��ť����pageFlag = HOME_PAGE��
        5.���ð�ť����pageFlag = SET_PAGE��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void menuClickCheck();

/*
    �����ˣ������ && ����������������
    ���ܣ�
        Player player();
        while(1){
            1.�����⵽��������˺ſ�textFlag=USERNAME,����ʼ���ܼ������룬ÿ����һ���ַ�����һ��usernameToPrint��
            Ȼ�����һ��printText��ʾuserNameToPrint��������̳���������س���Ϊֹ��
            ֮������Player.isLegal(usernameToPrint��textFlag)�ж��˺����Ƿ�Ϸ���
            ���Ϸ���������벢Ҫ���������룬�Ϸ�����player.setUsername(usernameToPrint);

            2.�����⵽������������textFlag=PASSWORD,����ʼ���ܼ������룬ÿ����һ���ַ�����һ��passwordToPrint��
            Ȼ�����һ��printText��ʾpasswordPrint��������̳���������س���Ϊֹ
            Ȼ�����Player.isLegal(passwordToPrint��textFlag)�ж������Ƿ�Ϸ������Ϸ���������벢Ҫ����������,
            �Ϸ���player.setPassword(passwordToPrint);

            3.�����⵽������˷��ذ�ť��pageflag = HOME_PAGE��

            4.�����⵽�������ע�ᰴť������player.ifExit()�����˺��Ƿ��Ѿ���user.txt�д���
            �������Ϊfalse�����player.createAccout()��user.txt�д�����¼������pageFlag = MENU_PAGE��
        }
    ���룺void
    �����void
*/
void regClickCheck();

/*
    ������:����� && ����������������
    ����:
        ����һ����ʱPlayer����player()
        while(1){
            1.�����⵽��������˺ſ�,��ʼ���ܼ������룬ÿ����һ���ַ�����һ��usernameToPrint��
            Ȼ�����һ��printText(...)��ʾusernameToPrint��������̳���������س���Ϊֹ��
            Ȼ����player.setUsername(usernameToPrint);

            2.�����⵽������������,��ʼ���ܼ������룬ÿ����һ���ַ�����һ��passwordToPrint��
            Ȼ�����һ��printText(...)��ʾpasswordToPrint��������̳���������س���Ϊֹ��
            Ȼ����player.setPassword(passwordToPrint);

            3.�����⵽������˷��ذ�ť��pageFlag = HOME_PAGE��
            4.�����⵽������˵�½��ť������Player.ifExit()���������Ϊtrue��pageFlag = MENU_PAGE��
        }
    ���룺void
    �����void
*/
void logClickCheck();


/*
    �����ˣ������ && ����������������
    ���ܣ�����Ƿ���������Ϣ�����еķ��ؼ�������ǣ���ôpageFlag = MENU_PAGE��
     *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void infoClickCheck();

/*
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        1.����ǵ�һ�ذ�ť��curStage = FIRST_STAGE��
                            pageFlag = GAME_PAGE��
        2.����ǵڶ��ذ�ť��curStage = SECOND_STAGE��
                            pageFlag = GAME_PAGE��
        3.����ǵ����ذ�ť��curStage = THIRD_STAGE��
                            pageFlag = GAME_PAGE��
        4.����Ƿ��ذ�ť��  pageFlag = MENU_PAGE��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void stageClickCheck();

/*
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        1.��������˳���ť��pageFlag = MENU_PAGE;
        2.����������¿�ʼ��ť��pageFlag = GAME_PAGE;curStage = FIRST_STAGE��Ҳ�������´ӵ�һ�ؿ�ʼ��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void endClickCheck();


/*
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        ����Ƿ���������Ϣ�����еķ��ؼ��������pageFlag = MENU_PAGE��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void rankClickCheck();

/*
   �����ˣ������ && ������������ѡ��
   ���ܣ�
        ������������:
        1.����Ƿ��ذ�ť��pageflag = MenuPage��
        2.������ٶ����ð�ť�������尴�����ĸ����ʣ�Ȼ��ȫ�ֱ�����acc�޸�Ϊ��Ӧ��ֵ
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
   ���룺void
   �����void
*/
void setClickCheck();

/*
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        ��ʼ���ƶ�����Ϊ�ߵ�ǰ���ƶ�����EnumDirection d = snake.getHeadDirection();
        ��ͣ0.5s;
        ������û��ļ���/������룺
            if(��wsad�ķ����)  �����"w"��ֱ����snake.getDir()�ﴫ��W,����ͬ����ʲô������ֱ��ȥ��˫���Ŵ���д��ĸ�ͺ�
            if(����������¼��ҵ����˹������İ�ť��)  pageFlag = TOOL_PAGE;return;
        ���û�м���/������룺
            snake.getDir(d)
         *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺Snake& snake,Stage& stage
    �����void
*/
void gameEventCheck(Snake& snake, Stage& stage);

/*
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        �������¼���
        1.��������Ƿ��ز˵���ť��pageFlag = MENU_PAGE��
        2.��������Ǽ�����Ϸ��ť��pageFlag = GAME_PAGE��
    ���룺void
    �����void
*/
void toolClickCheck();

/*
    �����ˣ������ && ������������ѡ��
    ���ܣ�����һ���¼�¼д��record.txt��,��¼������ǰplayer���û����������ͺ�ʱ��
          Ȼ���Է���Ϊ��һ�ؼ��֣���ʱΪ�ڶ��ؼ��ֶԸ���¼ʵ�ֽ�������
    ���룺Player& player
    �����void
*/
void fileWriteAndRank(Player& player);


/*
    �����ˣ������ && ������������ѡ��
    ���ܣ�ͨ���Ա����Ѿ��Ե���ʳ��������ͨ����ǰ�ؿ�����Ե���ʳ���������ж��Ƿ����
    ���룺�ؿ�����stage���߶���snake
    �����true/false
*/
bool isWin(const Snake& snake, const Stage& stage);


/*
    ������:�����
    ����:������Ϸ���棬��ɸ��ؿ�֮�����ת,ͳ�Ƶ�����Ϸ���ֺܷ��ܺ�ʱ����¼��ȫ�ֱ�������
    ����:void
    ����ֵ:void
*/
void runGame();

/*
    ������:�����
    ����:��ɸ�����֮�����ת
    ����:void
    ����ֵ:void
*/
void controller();

/*
    ������:�����
    ����:����++,ʵ��ö�����͵��Լ�
    ����:ö������curStage
    ����ֵ: EnumCurStage
*/
inline EnumCurStage& operator++(EnumCurStage& curStage);
// -------------------- service --------------------
inline EnumCurStage& operator++(EnumCurStage& curStage)
{
    curStage = static_cast<EnumCurStage>(curStage + 1); //ʵ��++curStage
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
                pageFlag = STAGE_PAGE;//��ʼ
                break;
            }

            if (x > 425 && x < 600 && y > 180 && y < 240) {
                pageFlag = SET_PAGE;//����
                break;
            }

            if (x > 425 && x < 600 && y > 280 && y < 340) {
                pageFlag = RANK_PAGE;//���а�
                break;
            }



            if (x > 425 && x < 600 && y > 380 && y < 440) {
                pageFlag = INFO_PAGE;//����
                break;
            }

            if (x > 425 && x < 600 && y > 480 && y < 540) {
                pageFlag = HOME_PAGE;//�˳�
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
        if (m.uMsg == WM_LBUTTONDOWN)    //��⵽�˺ſ���
        {
            int x = m.x;
            int y = m.y;
            if (m.x > 435 && m.x < 635 && m.y > 159 && m.y < 210) {//�˺ſ�
                /*
                textFlag = USERNAME;//�޸�flag
                char ch{};
                while ((ch = getchar()) != '\n') {
                    usernameToPrint += ch;
                    w.printText(sToWs(usernameToPrint), 435, 159, 30);
                }//�س�ֹͣ
                */
                textFlag = USERNAME;

                wchar_t s[20];
                InputBox(s, 13, L"�������˺ţ�");
                wstring wstring1(s);
                w.printText(wstring1, 435, 159, 30);
                usernameToPrint = converter.to_bytes(wstring1);

                Sleep(500);
                if (player.ifLegal(usernameToPrint, textFlag)) {
                    player.setUsername(usernameToPrint); //�Ϸ��ж�
                }

                else {
                    MessageBox(NULL, _T("�û������Ϸ�!"), _T("����"), MB_OK | MB_ICONWARNING);
                    usernameToPrint = "";//���Ϸ����
                    Button* regPageAccount = new Button(330, 164, 82, 50, L"�˺�:");
                    regPageAccount->show();
                    setfillcolor(WHITE);//��������������ɫ
                    setfillstyle(BS_SOLID);//�����������䷽ʽ
                    fillrectangle(412, 164, 412 + 200, 164 + 50);//�����˺���������
                }
            }

            if (m.x > 453 && m.x < 653 && m.y > 246 && m.y < 296) {//�����
                /*
                textFlag = PASSWORD;//�޸�flag
                char ch{};
                while ((ch = getchar()) != '\n') {
                    passwordToPrint += ch;
                    w.printText(sToWs(passwordToPrint), 435, 246, 30);
                */
                textFlag = PASSWORD;

                wchar_t s[20];
                InputBox(s, 13, L"���������룺");
                wstring wstring1(s);
                w.printText(wstring1, 435, 246, 30);
                passwordToPrint = converter.to_bytes(wstring1);


                if (player.ifLegal(passwordToPrint, textFlag)) {
                    player.setPassword(passwordToPrint); //�Ϸ��ж�
                }
                else {
                    MessageBox(NULL, _T("���벻�Ϸ�!"), _T("����"), MB_OK | MB_ICONWARNING);
                    passwordToPrint = "";//���Ϸ����
                    Button* regPagePassword = new Button(330, 250, 82, 50, L"����:");
                    regPagePassword->show();
                    setfillcolor(WHITE);
                    setfillstyle(BS_SOLID);
                    fillrectangle(412, 250, 412 + 200, 250 + 50);//����������������

                }
            }

            if (m.x > 412 && m.x < 612 && m.y > 480 && m.y < 530) {//���ذ�ť
                pageFlag = HOME_PAGE;
                break;
            }

            //  4.�����⵽�������ע�ᰴť������player.ifExit()�����˺��Ƿ��Ѿ���user.txt�д���
            //  �������Ϊfalse�����player.createAccout()��user.txt�д�����¼������pageFlag = MENU_PAGE��

            if (m.x > 412 && m.x < 612 && m.y > 400 && m.y < 450) {//ע��
                //char ch{};
                //while ((ch = getchar()) != '\n') {
                //    usernameToPrint += ch;
                //    w.printText(sToWs(usernameToPrint),435, 159, 30);
                //}//�س�ֹͣ
                if (usernameToPrint == "" || passwordToPrint == "")
                {
                    MessageBox(NULL, _T("�û��������벻��Ϊ��!"), _T("����"), MB_OK | MB_ICONWARNING);
                    //cout << "�û��������벻��Ϊ�գ�" << endl;
                    continue;
                }
                if (!player.ifExist()) {
                    player.createAccount();
                    MessageBox(NULL, _T("ע��ɹ�!"), _T("��Ϣ"), MB_OK | MB_ICONINFORMATION);
                    pageFlag = HOME_PAGE;
                    break;
                }
                else {
                    MessageBox(NULL, _T("���û��Ѵ���!"), _T("����"), MB_OK | MB_ICONWARNING);
                    //cout << "�û����Ѵ��ڣ�����������" << endl;
                    usernameToPrint = "";
                    passwordToPrint = "";

                    Button* regPageAccount = new Button(330, 164, 82, 50, L"�˺�:");
                    regPageAccount->show();
                    setfillcolor(WHITE);//��������������ɫ
                    setfillstyle(BS_SOLID);//�����������䷽ʽ
                    fillrectangle(412, 164, 412 + 200, 164 + 50);//�����˺���������

                    Button* regPagePassword = new Button(330, 250, 82, 50, L"����:");
                    regPagePassword->show();
                    setfillcolor(WHITE);
                    setfillstyle(BS_SOLID);
                    fillrectangle(412, 250, 412 + 200, 250 + 50);//����������������
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
            if (m.x > 412 && m.x < 612 && m.y > 164 && m.y < 214) {//�˺ſ���ռ�������
                /*
                char ch{};
                while ((ch = getchar()) != '\n') {

                    usernameToPrint += ch;
                    w.printText(sToWs(usernameToPrint), 435, 159, 30);
                }
                */
                wchar_t s[20];
                InputBox(s, 13, L"�������˺ţ�");
                wstring wstring1(s);
                w.printText(wstring1, 435, 159, 30);
                usernameToPrint = converter.to_bytes(wstring1);
            }

            if (m.x > 412 && m.x < 612 && m.y > 250 && m.y < 300) {//�������ռ�������

                char ch{};
                /*
                while ((ch = getchar()) != '\n') {
                    passwordToPrint += ch;
                    w.printText(sToWs(passwordToPrint), 435, 250, 30);
                }
                */
                wchar_t s[20];
                InputBox(s, 13, L"���������룺");
                wstring wstring1(s);
                w.printText(wstring1, 435, 250, 30);
                passwordToPrint = converter.to_bytes(wstring1);

            }

            if (m.x > 412 && m.x < 612 && m.y > 487 && m.y < 537) {//���ذ�ť
                pageFlag = HOME_PAGE;
                break;
            }

            if (m.x > 412 && m.x < 612 && m.y > 399 && m.y < 450) {//��¼��ť
                if (usernameToPrint == "" || passwordToPrint == "")
                {
                    MessageBox(NULL, _T("�������û���������!"), _T("����"), MB_OK | MB_ICONWARNING);
                    // cout << "�������û��������룡" << endl;
                    continue;
                }
                player.setUsername(usernameToPrint);
                player.setPassword(passwordToPrint);

                if (player.ifExist()) {
                    //cout << usernameToPrint << endl;
                    //cout << passwordToPrint << endl;
                    MessageBox(NULL, _T("��½�ɹ�!"), _T("��Ϣ"), MB_OK | MB_ICONINFORMATION);
                    pageFlag = MENU_PAGE;
                    break;
                }
                else {
                    MessageBox(NULL, _T("�û����������������!"), _T("����"), MB_OK | MB_ICONWARNING);
                    //std::cerr << "Error:username error !!! \n";
                    usernameToPrint = "";
                    passwordToPrint = "";
                    Button* logPageAccount = new Button(330, 164, 82, 50, L"�˺�:");
                    logPageAccount->show();
                    setfillcolor(WHITE);//��������������ɫ
                    setfillstyle(BS_SOLID);//�����������䷽ʽ
                    fillrectangle(412, 164, 412 + 200, 164 + 50);//�����˺���������

                    Button* logPagePassword = new Button(330, 250, 82, 50, L"����:");
                    logPagePassword->show();
                    setfillcolor(WHITE);//��������������ɫ
                    setfillstyle(BS_SOLID);//�����������䷽ʽ
                    fillrectangle(412, 250, 412 + 200, 250 + 50);//����������������
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
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        1.����ǵ�һ�ذ�ť��curStage = FIRST_STAGE��
                            pageFlag = GAME_PAGE��
        2.����ǵڶ��ذ�ť��curStage = SECOND_STAGE��
                            pageFlag = GAME_PAGE��
        3.����ǵ����ذ�ť��curStage = THIRD_STAGE��
                            pageFlag = GAME_PAGE��
        4.����Ƿ��ذ�ť��  pageFlag = MENU_PAGE��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void stageClickCheck()
{
    while (true) {
        MOUSEMSG m = GetMouseMsg();  // ��ȡ�����Ϣ

        if (m.uMsg == WM_LBUTTONDOWN)  // �����������¼�
        {
            int x = m.x;  // ����¼�����ʱ�ĺ�����
            int y = m.y;  // ����¼�����ʱ��������

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
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        1.��������˳���ť��pageFlag = MENU_PAGE;
        2.����������¿�ʼ��ť��pageFlag = GAME_PAGE;curStage = FIRST_STAGE��Ҳ�������´ӵ�һ�ؿ�ʼ��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void endClickCheck()
{
    int restartTop, restartBottom, restartLeft, restartRight;//������ǵ�һ�صĴ��ڴ�С
    int returnTop, returnBottom, returnLeft, returnRight;//���ؼ��������С

    while (true) {
        MOUSEMSG m = GetMouseMsg();  // ��ȡ�����Ϣ

        if (m.uMsg == WM_LBUTTONDOWN)  // �����������¼�
        {
            int x = m.x;  // ����¼�����ʱ�ĺ�����
            int y = m.y;  // ����¼�����ʱ��������

            if (m.x > 425 && m.x < 600 && m.y > 242 && m.y < 302)
            {
                curStage = FIRST_STAGE;
                pageFlag = GAME_PAGE;
                curLength = 3;
                curX = WW / 2, curY = HH / 2;
                curEatenFood = 0;
                EnumDirection curSnakeDirection = EnumDirection::D; // Ĭ�Ϸ�������
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
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        ����Ƿ���������Ϣ�����еķ��ؼ��������pageFlag = MENU_PAGE��
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺void
    �����void
*/
void rankClickCheck()
{
    int returnTop, returnBottom, returnLeft, returnRight;//���ؼ��������С

    while (true) {
        MOUSEMSG m = GetMouseMsg();  // ��ȡ�����Ϣ

        if (m.uMsg == WM_LBUTTONDOWN)  // �����������¼�
        {
            int x = m.x;  // ����¼�����ʱ�ĺ�����
            int y = m.y;  // ����¼�����ʱ��������


            if (m.x > 431 && m.x < 593 && m.y > 540 && m.y < 607)
            {
                pageFlag = MENU_PAGE;
                break;
            }

        }
    }
};

/*
   �����ˣ������ && ������������ѡ��
   ���ܣ�
        ������������:
        1.����Ƿ��ذ�ť��pageflag = MenuPage��
        2.������ٶ����ð�ť�������尴�����ĸ����ʣ�Ȼ��ȫ�ֱ�����acc�޸�Ϊ��Ӧ��ֵ
        *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
   ���룺void
   �����void
*/
void setClickCheck()//BIG QUESTION
{


    while (true) {
        MOUSEMSG m = GetMouseMsg();  // ��ȡ�����Ϣ

        if (m.uMsg == WM_LBUTTONDOWN)  // �����������¼�
        {
            int x = m.x;  // ����¼�����ʱ�ĺ�����
            int y = m.y;  // ����¼�����ʱ��������

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
     �����ˣ������ && ������������ѡ��
    ���ܣ�
        ��ʼ���ƶ�����Ϊ�ߵ�ǰ���ƶ�����EnumDirection d = snake.getHeadDirection();
        ��ͣ0.5s;
        ������û��ļ���/������룺
            if(��wsad�ķ����)  �����"w"��ֱ����snake.getDir()�ﴫ��W,����ͬ����ʲô������ֱ��ȥ��˫���Ŵ���д��ĸ�ͺ�
            if(����������¼��ҵ����˹������İ�ť��)  pageFlag = TOOL_PAGE;return;
        ���û�м���/������룺
            snake.getDir(d)
         *������ť������һ����Σ����������λ����Ҫ�Ҹ�����ؽ�����Ƶ�ͬѧ��ͨ
    ���룺Snake& snake
    �����void
*/
void gameEventCheck(Snake& snake, Stage& stage)
{
    EnumDirection d = snake.getHeadDirection();
    Sleep(100 * (4 - acc));
    //Sleep(500);
    int top, left, right, bottom;
    MOUSEMSG m = GetMouseMsg();  // ��ȡ�����Ϣ
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
    �����ˣ������ && ������������ѡ��
    ���ܣ�
        �������¼���
        1.��������Ƿ��ز˵���ť��pageFlag = MENU_PAGE��
        2.��������Ǽ�����Ϸ��ť��pageFlag = GAME_PAGE��
    ���룺void
    �����void
*/
void toolClickCheck()
{
    int firstTop, firstBottom, firstLeft, firstRight;//������ǵ�һ�صĴ��ڴ�С
    int returnTop, returnBottom, returnLeft, returnRight;//���ؼ��������С

    while (true)
    {
        MOUSEMSG m = GetMouseMsg();  // ��ȡ�����Ϣ

        if (m.uMsg == WM_LBUTTONDOWN)  // �����������¼�
        {
            int x = m.x;  // ����¼�����ʱ�ĺ�����
            int y = m.y;  // ����¼�����ʱ��������

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
    �����ˣ������
    ���ܣ���record.txt�ļ���ÿһ�м�¼����һ���û������ݡ�ÿ���û������ݸ�ʽ�ǣ��û��� �û����� ���ѵ�ʱ��
    ��player��������record.txt�ж������û����ݽ���������������˳�������û�����Ϊ�ؼ��֣�������ͬ���˰��ջ���ʱ������
    ֮������õ������Ը��ǵ���ʽд��record.txt��
    ���룺Player& player
    �����void
*/
void fileWriteAndRank(Player& player)
{
    // ��ȡrecord.txt�ļ��е��û�����
    std::ifstream file("record.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    // ��player��������ӵ�lines��
    std::string playerData = player.getUsername() + " " + std::to_string(player.getTotalScore()) + " " + std::to_string(player.getTotalTime());
    lines.push_back(playerData);

    // ��lines��������
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

    // ������õ�����д��record.txt��
    std::ofstream outputFile("record.txt");
    for (const std::string& line : lines)
    {
        outputFile << line << std::endl;
    }
    outputFile.close();
}


/*
    �����ˣ������
    ���ܣ�ͨ���Ա����Ѿ��Ե���ʳ��������ͨ����ǰ�ؿ�����Ե���ʳ���������ж��Ƿ����
    ���룺�ؿ�����stage���߶���snake
    �����true/false
*/
bool isWin(Snake& snake, Stage& stage)
{
    if (snake.getEatenFood() == stage.getStagePassStandard())
        return true;
    return false;
};


//�鳤����
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
/**************************Button�ຯ��ʵ��****************************************************************************************/
//
//
Button::Button(int x, int y, int width, int height, const std::wstring& text) : Widget(x, y, width, height), text(text), isChange(false)
{
}

void Button::show() const
{
    // ������ʽ
    setlinestyle(PS_SOLID, 2);
    setfillcolor(CYAN);//CYAN��ɫ
    settextstyle(25, 0, L"΢���ź�");

    // ���ư�ť
    fillrectangle(x, y, x + width, y + height);

    // �����ı�
    RECT rect = { x, y, x + width, y + height };
    drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool Button::state(const ExMessage& msg)
{
    if (msg.message == WM_MOUSEMOVE && isIn(msg))	// ��ť����
    {
        // ������ʽ
        setlinestyle(PS_SOLID, 2);
        setfillcolor(LIGHTBLUE);
        settextstyle(25, 0, L"΢���ź�");

        // ���ư�ť
        fillrectangle(x, y, x + width, y + height);

        // �����ı�
        RECT rect = { x, y, x + width, y + height };
        drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        isChange = true;
        return false;
    }
    else if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONUP) && isIn(msg))	// ��ť�����
    {
        // ������ʽ
        setlinestyle(PS_SOLID, 2);
        setfillcolor(LIGHTBLUE);
        settextstyle(25, 0, L"΢���ź�");

        // ���ư�ť
        fillrectangle(x, y, x + width, y + height);

        // �����ı�
        RECT rect = { x, y, x + width, y + height };
        drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        isChange = true;
        return true;
    }
    else	// �ڰ�ť״̬�ı��ʱ�򣬻ָ���ťԭ����״̬����ֹ����
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
/**************************Window�ຯ��ʵ��****************************/
//
//
//
Window::Window(int width, int height) :
    Widget(0, 0, width, height),
    hWnd(initgraph(width, height))
{
    SetWindowText(hWnd, L"̰����--������ʦ");//�޸Ĵ��ڱ���������
    setbkcolor(WHITE);
    cleardevice();
    ShowWindow(hWnd, SW_HIDE);//ָ�����ڵ���ʾ״̬

    // ���û�ͼ��ʽ
    LOGFONT f;
    gettextstyle(&f);
    f.lfQuality = DEFAULT_QUALITY;
    settextstyle(&f);
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    setlinecolor(BLACK);

    // ������ť
    //home��ť
    homePageLog = new Button(412, 220, 200, 100, L"��¼");
    homePageReg = new Button(412, 350, 200, 100, L"ע��");
    homePageExit = new Button(412, 478, 200, 100, L"�˳�");
    //��¼����
    logPageAccount = new Button(330, 164, 82, 50, L"�˺�:");
    logPagePassword = new Button(330, 250, 82, 50, L"����:");
    logPageLog = new Button(412, 399, 200, 50, L"��¼");
    logPageQuit = new Button(412, 487, 200, 50, L"�˳���¼");
    //ע�����
    regPageAccount = new Button(330, 164, 82, 50, L"�˺�:");
    regPagePassword = new Button(330, 250, 82, 50, L"����:");
    regPageSure = new Button(412, 400, 200, 50, L"ע��");
    regPageQuit = new Button(412, 480, 200, 50, L"�˳�ע��");
    //�˵�����
    menuViewInfo = new Button(425, 380, 174, 59, L"��Ϸ����");
    menuViewSet = new Button(425, 180, 174, 59, L"��Ϸ����");
    menuViewChoice = new Button(425, 80, 174, 59, L"��ʼ��Ϸ");
    menuViewRank = new Button(425, 280, 174, 59, L"���а�");
    menuViewQuit = new Button(425, 480, 174, 59, L"�˳���¼");
    //�ؿ�ѡ�����
    choicePageOne = new Button(363, 100, 299, 59, L"�ؿ�һ��ɳĮ����");
    choicePageTwo = new Button(363, 200, 299, 59, L"�ؿ�������ɫ����");
    choicePageThree = new Button(363, 300, 299, 59, L"�ؿ��������ǻ���");
    choicePageN = new Button(363, 400, 299, 59, L"�ؿ�n�������ڴ�");
    choicePageQuit = new Button(363, 520, 299, 59, L"���ز˵�");
    //��Ϸ���ý���
    setPageSpeed1 = new Button(412, 45, 200, 100, L"�ٶ�*1.0");
    setPageSpeed2 = new Button(412, 195, 200, 100, L"�ٶ�*1.5");
    setPageSpeed3 = new Button(412, 349, 200, 100, L"�ٶ�*2.0");
    setPageQuit = new Button(412, 501, 200, 100, L"�˳�����");
    //���а����
    rankPageQuit = new Button(431, 540, 162, 66, L"���ز˵�");
    //��Ϸ���ܽ���
    infoPageQuit = new Button(423, 510, 162, 66, L"���ز˵�");
    //��Ϸ����
    gamePageScore = new Button(105, 68, 70, 34, L"�÷�:");
    gamePageTime = new Button(344, 68, 70, 34, L"��ʱ:");
    gamePagePause = new Button(949, 25, 50, 50, L"��ͣ");
    //��Ϸ��ͣ����
    pausePageBack = new Button(425, 242, 174, 59, L"������Ϸ");
    pausePageQuit = new Button(425, 375, 174, 59, L"���ز˵�");
    //��Ϸ������ť
    endPageRe = new Button(425, 242, 174, 59, L"���¿�ʼ");
    endPageQuit = new Button(425, 375, 174, 59, L"���ز˵�");
    showHomePage();	// ��ʾ������

}

Window::~Window()
{
    // ����
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
//    // ������Ϣѭ��
//    ExMessage msg;
//    while (true)
//    {
//        // ��ȡ���ж���Ϣ
//        msg = getmessage();
//
//        // �ж���ʾ����
//        if (state == EnumPages::HOME_PAGE)		// ��������ʾ
//        {
//            if (homePageLog->state(msg) && msg.message != WM_LBUTTONUP)
//            {//�����¼����
//                showLogPage();
//            }
//            else if (homePageReg->state(msg) && msg.message != WM_LBUTTONUP)
//            {//����ע�����
//                showRegPage();
//            }
//            else if (homePageExit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//�˳�
//                return;
//            }
//        }
//        else if (state == EnumPages::LOG_PAGE)//��¼����
//        {
//            if (logPageLog->state(msg))
//            {//����˵�����
//                showMenuView();
//            }
//            else if (logPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//����������
//                showHomePage();
//            }
//        }
//        else if (state == EnumPages::REG_PAGE) //ע�����
//        {
//            if (regPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//����������
//                showHomePage();
//            }
//            else if (regPageSure->state(msg) && msg.message != WM_LBUTTONUP)
//            {//���ע��
//
//            }
//        }
//        else if (state == EnumPages::MENU_PAGE) //�˵�����
//        {
//            if (menuViewInfo->state(msg) && msg.message != WM_LBUTTONUP)
//            {//��Ϸ���ܽ���
//                showInfoPage();
//            }
//            else if (menuViewSet->state(msg) && msg.message != WM_LBUTTONUP)
//            {//������Ϸ���ý���
//                showSetPage();
//            }
//            else if (menuViewChoice->state(msg) && msg.message != WM_LBUTTONUP)
//            {//����ؿ�ѡ�����
//                showChoicePage();
//            }
//            else if (menuViewRank->state(msg) && msg.message != WM_LBUTTONUP)
//            {//�������а����
//                showRankPage();
//            }
//            else if (menuViewQuit->state(msg) && msg.message != WM_LBUTTONUP)	// ����
//            {
//                // ��ʾ������
//                showHomePage();
//            }
//        }
//        else if (state == EnumPages::STAGE_PAGE)//ѡ��ؿ�
//        {
//            if (choicePageOne->state(msg) && msg.message != WM_LBUTTONUP)
//            {//����ؿ�1
//                gameView(50, 5000);
//            }
//            else if (choicePageTwo->state(msg) && msg.message != WM_LBUTTONUP)
//            {//����ؿ�2
//                gameView(1, 0);
//            }
//            else if (choicePageThree->state(msg) && msg.message != WM_LBUTTONUP)
//            {//����ؿ�3
//                gameView(1, 0);
//            }
//            else if (choicePageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//���ز˵�
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::SET_PAGE)//���ý���
//        {
//            if (setPageSpeed1->state(msg) && msg.message != WM_LBUTTONUP)
//            {//�����ɫѡ��ť
//
//            }
//            else if (setPageSpeed2->state(msg) && msg.message != WM_LBUTTONUP)
//            {//�����ʼ�ٶȰ�ť
//
//            }
//            else if (setPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//������ز˵���ť
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::RANK_PAGE)//���а����
//        {
//            if (rankPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//������ز˵���ť
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::INFO_PAGE)
//        {
//            if (infoPageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//������ز˵���ť
//                showMenuView();
//            }
//        }
//        else if (state == EnumPages::GAME_PAGE)//��Ϸ����
//        {
//            if (gamePagePause->state(msg) && msg.message != WM_LBUTTONUP)
//            {//������ͣ����
//                toolView();
//            }
//        }
//        else if (state == EnumPages::TOOL_PAGE)
//        {
//            if (pausePageBack->state(msg) && msg.message != WM_LBUTTONUP)
//            {//������Ϸ
//                gameView(1, 0);
//            }
//            else if (pausePageQuit->state(msg) && msg.message != WM_LBUTTONUP)
//            {//�������˵�
//                showMenuView();
//            }
//        }
//    }
//}

void Window::showHomePage()//������
{

    state = EnumPages::HOME_PAGE;
    cleardevice();

    // ���ر���ͼƬ
    loadimage(NULL, bkHome, 1024, 640);

    // ������ʾ����
    RECT rect = { 0, 0, width, 200 };
    settextstyle(50, 0, L"΢���ź�");
    drawtext(L"̰����--������ʦ", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // ��ʾ��ť
    homePageLog->show();
    homePageReg->show();
    homePageExit->show();
}
void Window::showLogPage()//��¼����
{
    state = EnumPages::LOG_PAGE;//����Ŀǰ���ĸ�����
    cleardevice();

    loadimage(NULL, bkLog, 1024, 640);//���ر���ͼƬ

    setfillcolor(WHITE);//��������������ɫ
    setfillstyle(BS_SOLID);//�����������䷽ʽ
    fillrectangle(412, 164, 412 + 200, 164 + 50);//�����˺���������
    fillrectangle(412, 250, 412 + 200, 250 + 50);//����������������
    //��ʾ��ť
    logPageAccount->show();
    logPageLog->show();
    logPagePassword->show();
    logPageQuit->show();

}
void Window::showRegPage()//ע�����
{
    state = EnumPages::REG_PAGE;
    cleardevice();

    // ���ر���ͼƬ
    loadimage(NULL, bkReg, 1024, 640);

    setfillcolor(WHITE);//��������������ɫ
    setfillstyle(BS_SOLID);//�����������䷽ʽ
    fillrectangle(412, 164, 412 + 200, 164 + 50);//�����˺���������
    fillrectangle(412, 250, 412 + 200, 250 + 50);//����������������
    // ��ʾ�ؼ�
    regPageAccount->show();
    regPagePassword->show();
    regPageSure->show();
    regPageQuit->show();
}
void Window::showMenuView()//�˵�����
{
    state = EnumPages::MENU_PAGE;
    cleardevice();

    // ���ر���ͼƬ
    loadimage(NULL, bkMenu, 1024, 640);

    // ��ʾ�ؼ�
    menuViewInfo->show();
    menuViewSet->show();
    menuViewChoice->show();
    menuViewRank->show();
    menuViewQuit->show();
}
void Window::showChoicePage()//ѡ��ؿ�����
{
    state = EnumPages::STAGE_PAGE;
    cleardevice();

    // ���ر���ͼƬ
    loadimage(NULL, bkChoice, 1024, 640);

    //��ʾ�ؼ�
    choicePageOne->show();
    choicePageTwo->show();
    choicePageThree->show();
    choicePageN->show();
    choicePageQuit->show();
}
void Window::showSetPage()//��Ϸ���ý���
{
    state = EnumPages::SET_PAGE;
    cleardevice();

    // ���ر���ͼƬ
    loadimage(NULL, bkSet, 1024, 640);
    //��ʾ�ؼ�
    setPageSpeed1->show();
    setPageSpeed2->show();
    setPageSpeed3->show();
    setPageQuit->show();
}
void Window::showRankPage()//���а����
{
    state = EnumPages::RANK_PAGE;
    cleardevice();
    // ���ر���ͼƬ
    loadimage(NULL, bkRank, 1024, 640);

    setbkmode(TRANSPARENT);//��������򱳾�
    setfillstyle(BS_NULL);//�����������䷽ʽ
    fillrectangle(274, 75, 274 + 477, 75 + 54);//���ƿ�1
    fillrectangle(274, 129, 274 + 477, 129 + 54);//���ƿ�2
    fillrectangle(274, 183, 274 + 477, 183 + 54);//���ƿ�3
    fillrectangle(274, 237, 274 + 477, 237 + 54);//���ƿ�4
    fillrectangle(274, 291, 274 + 477, 291 + 54);//���ƿ�5
    fillrectangle(274, 345, 274 + 477, 345 + 54);//���ƿ�6
    fillrectangle(274, 399, 274 + 477, 399 + 54);//���ƿ�7
    fillrectangle(274, 453, 274 + 477, 453 + 54);//���ƿ�8
    setfillstyle(BS_SOLID);//�����������䷽ʽ
    //��ʾ�ؼ�
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
void Window::showInfoPage()//��Ϸ���ܽ���
{
    state = EnumPages::INFO_PAGE;
    cleardevice();
    // ���ر���ͼƬ
    loadimage(NULL, bkInfo, 1024, 640);

    setlinestyle(PS_NULL);
    rectangle(111, 79, 111 + 104, 79 + 29);
    RECT rect = { 111,79,111 + 104,79 + 29 };
    drawtext(L"��Ϸ����:", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rectangle(215, 79, 215 + 579, 79 + 141);
    rect = { 215, 79, 215 + 579, 79 + 141 };
    drawtext(L"ĳһ��ʱ�䣬�������κ�һ��ʱ�䣬�����ǹ�ȥ�����ڻ����ξ��ĳһ��ʱ�䣬���¸��ܱ����������������ֵ�û�ζ��������¼�˶�ʮ��ƪ��ƪ���£�ÿһƪͼ�Ķ�����ɸѡ���ţ���ʫ��ɢ����ʽ���ֳ��ںܾúܾ���ǰ��ĳһ��ʱ�䣬���ڰ��顢����Ļû����º���������",
        &rect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

    rectangle(111, 228, 111 + 104, 228 + 29);
    rect = { 111, 228, 111 + 104, 228 + 29 };
    drawtext(L"�������:", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rectangle(215, 228, 215 + 579, 228 + 29);
    rect = { 215, 228, 215 + 579, 228 + 29 };
    drawtext(L"̰���߹������ؿ�����ֵ�ߵĽ������а���ͬ��ֵ�Ƚ�ʱ�䡣", &rect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

    rectangle(111, 278, 111 + 104, 278 + 29);
    rect = { 111, 278, 111 + 104, 278 + 29 };
    drawtext(L"������Ա:", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    rectangle(215, 278, 215 + 579, 278 + 92);
    rect = { 215, 278, 215 + 579, 278 + 92 };
    drawtext(L"������-�鳤�������-���鳤�����-���鳤����˳��-�����٣�������-�����٣�����ƽ-�����٣�������-��Ʒ�������Ժ�-��Ʒ���������-��Ϣ�٣���ΰ��-�ල��",
        &rect, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
    infoPageQuit->show();
}
void Window::gameView(int gameScore, int gameTime, EnumCurStage cur_stage)
{
    state = EnumPages::GAME_PAGE;
    cleardevice();
    // ���ر���ͼƬ
    if (cur_stage == FIRST_STAGE)//������
        loadimage(NULL, bkOne, 1024, 640);
    else if (cur_stage == SECOND_STAGE)//������
        loadimage(NULL, bkTwo, 1024, 640);
    else if (cur_stage == THIRD_STAGE)//������
        loadimage(NULL, bkThree, 1024, 640);

    std::wostringstream oss;
    oss << gameScore;
    std::wstring wstr = oss.str();
    printText(wstr, 190, 68, 40);
    std::wostringstream oss2;
    oss2 << gameTime;
    std::wstring wstr1 = oss2.str();
    printText(wstr1, 430, 68, 40);


    setorigin(105, 107);//������Ϸ��ͼ���Ͻ�Ϊԭ��
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
            {//����
                setfillcolor(RED);
                //putpixel(j, i, RED);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 2)
            {//ʳ��
                setfillcolor(YELLOW);
                //putpixel(j, i, YELLOW);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 3)
            {//�õ���
                setfillcolor(LIGHTBLUE);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 4)
            {//������
                setfillcolor(LIGHTGRAY);
                fillrectangle(j * 10 - 5, i * 10 - 5, j * 10 + 5, i * 10 + 5);
            }
            else if (map[i][j] == 5)
            {
            }
        }
    }
    //setaspectratio(1, 1);
    setorigin(0, 0);//�践������
    gamePageScore->show();
    gamePageTime->show();
    //  gamePagePause->show();
}
void Window::toolView()//��Ϸ��ͣ����
{
    state = EnumPages::TOOL_PAGE;
    cleardevice();
    // ���ر���ͼƬ
    loadimage(NULL, bkPause, 1024, 640);

    pausePageBack->show();
    pausePageQuit->show();

}
void Window::endPage()//��Ϸ��������
{
    state = EnumPages::END_PAGE;
    cleardevice();
    // ���ر���ͼƬ
    loadimage(NULL, bkEnd, 1024, 640);

    endPageRe->show();
    endPageQuit->show();
}
void Window::printText(std::wstring text, int x, int y, int size)
{
    settextstyle(size, 0, L"΢���ź�");
    outtextxy(x, y, text.c_str());
}

void runGame() {
    Player player; //��ʼ��Ϸ������player����
    player.setUsername(usernameToPrint); //������ҵ��û���
    player.setPassword(passwordToPrint); //������ҵ�����
    player.playerDataUpdate(gameScore, gameTime); //������ҵķ����ͺ�ʱ
    snakeBody[1][0] = HH / 2;
    snakeBody[1][1] = WW / 2;

    for (EnumCurStage i = curStage; i <= THIRD_STAGE; ++i) {
        Stage stage(i, gap[i], passStandard[i], initSpeed[i], barrierNum[i]); //������i�صĹؿ�����
        if (!continueFlag) stage.InitStage(); // ����ͨ����ͣ��������ģ���Ҫ��ʼ���ؿ�
        Snake snake(curEatenFood, curLength, curX, curY, curSnakeDirection); //�ָ��߶���

        /*    cout << " snakeBody[0][0]: " << snakeBody[0][0] << " snakeBody[0][1]: " << snakeBody[0][1] << '\n';
            cout << " snakeBody[1][0]: " << snakeBody[1][0] << " snakeBody[1][1]: " << snakeBody[1][1] << '\n';
            cout << "map[snakeBody[1][0]][snakeBody[1][1]] : " << map[snakeBody[1][0]][snakeBody[1][1]] << '\n';
            cout << "map[snakeBody[0][0]][snakeBody[0][1]] : " << map[snakeBody[0][0]][snakeBody[0][1]] << '\n';*/
        ifFirstTime = true;
        while (1)
        {
            stage.mapGenerate(ifEaten, ifFirstTime); //���ɱ߽磬�ϰ��ʳ����ߣ��ߵ����壬��map����
            //cout << "mapGenerate has done" << endl;

            w.gameView(player.getTotalScore(), player.getTotalTime(), i);//��ʾ��ͼ���ߣ�ʳ��/���ߣ��������͵÷ֺ�ʱ
            //cout << "gameview has done" << endl;

            gameEventCheck(snake, stage); //����û����룬�����ͣ/�����ߵķ���
            //cout << "gameeventCheck has done" << endl;

            if (pageFlag == TOOL_PAGE)//�ж��Ƿ���Ҫ��ת��������ҳ��
            {
                curStage = i; //��¼��ǰ�ڵڼ���
                snake.saveSnake();//����ǰ���ߵĲ������浽ȫ�ֱ���
                gameScore = player.getTotalScore(); //������Ϸ����
                gameTime = player.getTotalTime(); //������Ϸʱ��
                continueFlag = 1; //��������־�޸�Ϊ1
                return;
            }

            //cout << " snake.dir : " << snake.dir << '\n';

            /*cout << " snakeBody[0][0]: " << snakeBody[0][0] << " snakeBody[0][1]: " << snakeBody[0][1] << '\n';
            cout << " snakeBody[1][0]: " << snakeBody[1][0] << " snakeBody[1][1]: " << snakeBody[1][1] << '\n';
            cout << "map[snakeBody[1][0]][snakeBody[1][1]] : " << map[snakeBody[1][0]][snakeBody[1][1]] << '\n';
            cout << "map[snakeBody[0][0]][snakeBody[0][1]] : " << map[snakeBody[0][0]][snakeBody[0][1]] << '\n';*/
            //cout << "yeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;

            snake.getDir(snake.dir, stage);
            switch (map[snakeBody[0][0]][snakeBody[0][1]]) //������һ��Ҫ�ƶ�����λ����ʲô������snake,stage,player��״̬
            {
            case 0:
                pageFlag = END_PAGE; // ׼�������������
                //cout << "�����߽� \n";
                break;
            case 1:
                pageFlag = END_PAGE; // ׼�������������
                //cout << "�������� \n";
                break;
            case 2:
                snake.eatFood(stage, player); //�Ե�ʳ�����snake,stage��player��״̬
                ifEaten = true;
                break;
            case 3:
                snake.eatGoodProp(stage, player);//�Ե��õ��ߣ�����snake,stage��player��״̬
                ifEaten = true;
                break;
            case 4:
                snake.eatBadProp(stage, player);//�Ե������ߣ�����snake,stage��player��״̬
                ifEaten = true;
                break;
            case 5:
                snake.normalMove(stage, player);//ɶ��û�Ե�������snake,stage��player��״̬
                ifEaten = false;
                //cout << "normalMove �����ƶ���һ��\n";
                break;
            }
            // cout << "Last1" << endl;
            if (isWin(snake, stage)) {
                //cout << "now we are in isWin" << endl;
                break; //��ǰ�Ƿ���أ�����������whileѭ���Խ�����һ��
            }
            if (pageFlag == END_PAGE) {
                //cout << "now we are in endPage" << endl;
                fileWriteAndRank(player);//��Ϸ���������¼�¼
                gameScore = 0;//��Ϸ��������¼����������
                gameTime = 0;//��Ϸ��������¼����������
                return; //�л���Ϸ��������
            }
            ifFirstTime = false;
            //cout << "Last3" << endl;
        }
    }
    fileWriteAndRank(player); //˳��ͨ�أ������ļ��еļ�¼
    gameScore = 0;//˳��ͨ�أ���¼����������
    gameTime = 0; //˳��ͨ�أ���¼����ʱ����
    pageFlag = END_PAGE; //˳��ͨ�أ�׼�������������
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
                homeClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != HOME_PAGE) break;
            }
            break;
        case LOG_PAGE:
            while (1) {
                w.showLogPage();
                logClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != LOG_PAGE) break;
            }
            break;
        case REG_PAGE:
            while (1) {
                w.showRegPage();
                regClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != REG_PAGE) break;
            }
            break;
        case MENU_PAGE:
            while (1) {
                w.showMenuView();
                menuClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != MENU_PAGE) break;
            }
            break;
        case INFO_PAGE:
            while (1) {
                w.showInfoPage();
                infoClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != INFO_PAGE) break;
            }
            break;
        case SET_PAGE:
            while (1) {
                w.showSetPage();
                setClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != SET_PAGE) break;
            }
            break;
        case STAGE_PAGE:
            while (1) {
                w.showChoicePage();
                stageClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != STAGE_PAGE) break;
            }
            break;
        case GAME_PAGE:
            runGame();
            break;
        case TOOL_PAGE:
            while (1) {
                w.toolView();
                toolClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != LOG_PAGE) break;
            }
            break;
        case END_PAGE:
            while (1) {
                w.endPage();
                endClickCheck(); //�����ڲ���Ҫѭ��
                if (pageFlag != END_PAGE) break;
            }
            break;
        case RANK_PAGE:
            while (1) {
                w.showRankPage();
                rankClickCheck(); //�����ڲ���Ҫѭ��
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