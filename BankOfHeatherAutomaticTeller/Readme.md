#Bank of Heather Automatic Teller

模拟单台ATM机的处理，观察人流量对用户等待时间等数据的影响。可以推广到其他类似设备的模型。

``` c++
/* Bank of Heather Automatic Teller
 * @input:
 * Enter maximun size of queue; maxSize;
 * Enter thr number of simulation hours: simulation hours;
 * Enter the average number of customers per hour: customer number;
 * @output:
 * ATM::printResult();
 */
```

------

输入队列最大长度，模拟的小时数，每个小时到达的人数。
程序打印在这段过程中：
ATM机服务的人数，
因队列满离开的人数，
第一次出现离开的时间，以及其在总模拟时间的百分多少位置，
平均离开发生的时间在总模拟时间的百分之多少，
平均排队的长度，两位小数
平均每个接受服务的用户所等待的时间。

------

可以发现单台ATM机在一般人流量下，翻倍的人流量带来的是多出5-10倍人均等待时间，特别是人流量超过其处理时间阈值后，其性能会数百倍的下降。