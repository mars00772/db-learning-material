## Scheduling 

我们先回顾下operator/task的概念

![1589525616533](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589525616533.png)

我们今天讨论的调度：就是讨论task时怎么调度的。

![1589525679933](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589525679933.png)

### Process Model

进程模型决定了系统如何支持多用户的并发请求

**worker**是DBMS中负责执行task并且向用户返回结果的组件

第一节课要求读的paper《Architecture of DB》有讲到三种进程模型：

* Approach1 Process per DBMS worker

Fork to stat a new process,所以是操作系统来承担worker调度的工作

![1589526505736](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589526505736.png)

* Approach2 Process Pool

单个worker的socket可能要应付不同的并发读写请求，socket是有缓存的, 那就可能会导致维护缓存一致性导致的性能瓶颈,如cache miss等。

![1589526652549](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589526652549.png)



* Approach3 Thread per DBMS Worker

最常见的模型

![1589526770421](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589526770421.png)



好处：

* 上下文切换开销更少
* 不需要去管理共享内存



### Data Placement

worker都是对local data做操作的，所以worker必须要意识到硬件的内存管理

#### SMP （Uniform Memory Access）

多个CPU共享内存地址空间

总线带宽是瓶颈

![1589527549930](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589527549930.png)



#### NUMA

避免对内存共享系统总线的占用

![1589527766889](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589527766889.png)



一个Node内不同的CPU的L1cache是独占的(通常分为数据cache和指令cache),而L2,L3Cache共享

![1589527947507](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589527947507.png)



DBMS应该是负责把memory做划分，并且分配给不同的CPU

然后把operator调度到距离其最近的CPU核上

![1589531796836](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589531796836.png)



内存分配的策略：

* Interleaving

内存分配平均分布在不同的CPU(socket)上。

* First-Touch

分配在引起Page Fault的那个线程对应的CPU(socket)上

#### Partition & Placement

前者指的是分区：

分区是把DBMS的数据按照某种策略水平划分出不同部分：

* 哈希
* Round-Robin
* Range

后者指的是怎么把这些数据放置到合适的地方

* Roung-Robin
* 均分(Interleaving)

现在我们来讨论如何把Logical Query Plan转化成实际运行的Task？

### Scheduling

#### 静态调度

在逻辑计划生成的时候固定好执行query所需要线程数量

![1589533027883](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589533027883.png)



#### 动态调度

下面的模型是HyPer采取的一种pull模型

如果是dispatcher的话则为push模型，像这种由worker来去选择执行的task则为pull模型

![1589533398668](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589533398668.png)

而下面是SAP HANA选择的策略

![1589534616562](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589534616562.png)



![1589535353344](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589535353344.png)



#### SQLOS(Used by SQL SERVER)

用户态的,NUMA-aware并且**运行在数据库内核的操作系统层**

决定task分配到什么线程

管理IO的调度与数据库的锁



### FLOW CONTROL

如果请求到达速度远快于DBMS可以执行的速度，数据库很容易进入满载状态。



![1589536438684](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589536438684.png)

