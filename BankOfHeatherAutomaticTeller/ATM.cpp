/* Bank of Heather Automatic Teller
 * @input:
 * Enter maximun size of queue; maxSize;
 * Enter thr number of simulation hours: simulation hours;
 * Enter the average number of customers per hour: customer number;
 * @output:
 * ATM::printResult();
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <random>
#include <ctime>
#include <cstdlib>
using std::cout; 
using std::endl;

class ATM {
private:
    struct Customer{
        unsigned long comingTime;
        int processingTime;
        bool served;
        Customer* next;
        Customer(unsigned int comingTime): 
            comingTime(comingTime),
            processingTime(getProcessingTime()),
            served(false), 
            next(nullptr) {
        }

        int getProcessingTime() {
            return rand() % 3 + 1;
        }
    };
    const unsigned int maxSize;         //队列长度最大值
    enum { MIN_PER_HOUR = 60 };
    unsigned int customerAccepted;      //入队多少人
    unsigned int customerServed;        //服务了多少人
    unsigned int customerTrunaways;     //离开多少人
    unsigned int size;                  //当前队列长度
    unsigned int cNumPerHour;             //每分钟来多少人

    long long sizeSum;              //队列总长度
    long long waitingTimeSum;       //等待时间之和

    double simulationHour;          //总模拟时间
    double simulationMin;           //总模拟分钟
    double minPerCustomer;          //几分钟来一个人
    double averageSize;             //平均队列长度 = 每分钟的队列长度之和 / 分钟数
    double averageTime;             //平均等待时间 = 每个人的等待时间长度 / 人数

    Customer* head;
    Customer* tail;

    std::vector<int> turnawayTime;
public:
    ATM(unsigned int maxSize, double simulationHour, double cNumPerHour):
        maxSize(maxSize),
        customerAccepted(0),
        customerServed(0),
        customerTrunaways(0),
        size(0),
        cNumPerHour(cNumPerHour),
        minPerCustomer(MIN_PER_HOUR / cNumPerHour),

        sizeSum(0),
        waitingTimeSum(0),

        simulationHour(simulationHour),
        simulationMin(simulationHour * MIN_PER_HOUR),
        averageSize(0),
        averageTime(0),

        head(nullptr),
        tail(nullptr) {

    }

    /* 析构函数
     * 删除链表中的所有结点
     */
    ~ATM() {
        Customer* pre = head;
        for (Customer* p = head; p != nullptr; ) {
            pre = p ->next;
            delete p;
            p = pre;
        }
    }

    /* 判断队列是否为空
     * @return 1:为空, 0:不为空
     */
    bool empty() {
        return !size;
    }


    /* 判断队列是否已满
     * @return 1:已满, 0:未满
     */
    bool full() {
        return size == maxSize;
    }

    /*从队尾入队
     */
    bool insert(int comingTime) {
        if (this ->full()) {
            return false;
        }
        Customer* p = new Customer(comingTime);
        if (tail) tail ->next = p;
        if (!head) head = p;
        size++;
        customerAccepted++;
        tail = p;
        return true;
    }

    /*离队
     */
    bool leave() {
        if (empty()) return false;
        Customer* p = head ->next;
        delete head;
        size--;
        head = p;
        if (empty()) tail = nullptr;
        return true;
    }

    /* 判断这一分钟是否有新客服加入
     * @return 1:有客户加入 0:没有客户加入
     */
    bool newCustomer() {
        return (rand() * minPerCustomer / RAND_MAX) < 1;
    }


    /* 打印离开的人和离开发生的时间在总时间的百分比。
     */
    void turnawayOccurredAt() {
        cout << "customer turnaways: " << customerTrunaways << endl;
        if (customerTrunaways) {
            cout << "First turnaway occurred at: " << turnawayTime[0] << "Min. @" << turnawayTime[0] / simulationMin * 100 << '%' << ' ' << endl;
            double ans;
            for (std::vector<int>::iterator it = turnawayTime.begin(); it !=        turnawayTime.end(); ++it){
                ans += (*it / simulationMin);
            }
            ans /= turnawayTime.size();
            cout << "turnaway occurred at: " << ans * 100 << '%' << endl;
        }
    }

    /* 返回队列在模拟中的平均长度
     */
    double getAveSize() {
        return sizeSum / simulationMin;
    }

    /* 返回队列中人的平均等待时间
     */
    double getAveWaitTime() {
        return waitingTimeSum / (double)customerServed;
    }

    /* 循环每分钟的事件
     * @parameter 
     * @return
     */
    void play() {
        unsigned int waitingTime = 0;
        Customer* p = nullptr;
        for (int i = 0; i < simulationMin; ++i) {
            //cout << "min: " << i << " size: " << size << endl;
            //printf("min: %d size: %d\n", i, size);
            sizeSum += size;
            if (newCustomer()) {
                if (full()) {
                    customerTrunaways++;
                    turnawayTime.push_back(i);
                }
                else insert(i);
            }
            if (waitingTime <= 0 && !empty()) {
                if (head ->served) {
                    waitingTimeSum += i - head ->comingTime;
                    leave();    //如果队头已经服务完毕，离队
                }
                //serve
                else {
                    waitingTime = head ->processingTime;
                    head ->served = true;
                    customerServed++;
                    if (empty()) continue;
                }
            }
            if (waitingTime > 0) waitingTime--;
        }
        printResult();
    }

    void printResult() {
        using std::ios_base;
        putchar('\n');
        cout << "the length limit of queue: " << maxSize << endl;
        cout << "the simulationHour is: " << simulationHour << " Hour" << ((simulationHour > 1) ? 's' : ' ') << endl;
        cout << "the number of customer per hour is: " << cNumPerHour << endl;
        putchar('\n');
        cout << "customer accepted: " << customerAccepted << endl;
        cout << "customer served: " << customerServed << endl;
        cout.precision(2);
        cout.setf(ios_base::fixed, ios_base::floatfield);
        turnawayOccurredAt();
        putchar('\n');
        cout << "average queue size: " << getAveSize() << endl;
        cout << "average wait time: " << getAveWaitTime() << endl;
        cout.precision(6);
        cout.unsetf(ios_base::fixed);
        cout.unsetf(ios_base::floatfield);
        cout << "Done!\n=====" << endl;
        //printf("customer accepted: %d\n", customerAccepted);
        //printf("customer served:%d\n", customerServed);
    }
};

void BankOfHeatherAutomaticTeller() {
    printf("Please input the maxSize of queue: ");
    unsigned int maxSize;
    double simulationHour, cNumPerHour;
    scanf("%u", &maxSize);
    printf("Please input the simulationHour: ");
    scanf("%lf", &simulationHour);
    printf("Please input the number of customer per hour: ");
    scanf("%lf", &cNumPerHour);
    ATM demo(maxSize, simulationHour, cNumPerHour);
    demo.play();
}

int main(){
    srand(time(NULL));
    freopen("ATM.txt", "r", stdin);
    int cas = 0;
    char c;
    while (~scanf("%c", &c)) {
        ungetc(c,stdin);
        printf("Case %d:\n", ++cas);
        BankOfHeatherAutomaticTeller();
    }
    return 0;
    system("pause");
}