## Parallel Join Algorithms

### Two main parallel join algorithms:

* Hash Join
* Sort-Merge Join

许多OLTP DBMS是不会实现hash join的，因为没有必要build hashtable，有现成的index

**index nested-loop join** with small number of target tuples 可以大概等效于一个hash join

大概意思是说，hash join的hashtable是query查询临时构建的，查询完毕之后hashtable就会销毁。而index是本身就已经存在了的。

### Join Algorithm Design Goals

* 减少同步开销

在执行过程中尽量避免用latches

* 最小化内存访问的开销

当data还在cpu cache的时候，尽最大可能重用

确保数据和工作线程的局部性

#### 影响DBMS中cache misses的因素

* Cache + TLB容量(TLB就是快表，存储virtual address到physical address的mapping)
* 时空局部性

#### Non-Random Access:(sequential scan)

* Clustering data to a cache line.
* 那么可以对cache line的数据执行尽可能多的操作

#### Random Access: (lookups)

* partition data to fit in cache + TLB

### 什么是Hash Join？

可以划分为三个阶段

![1594540752949](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594540752949.png)

#### Partition Phase

![1594540911369](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594540911369.png)

如果是行存：整个tuple

如果是列存：只需要存储部分列的数据

![1594541687854](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594541687854.png)

* Non-Blocking Partitioning

![1594542258079](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594542258079.png)



* Shared Partitions

![1594542287102](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594542287102.png)

性能开销在于latch同步比较影响性能

* Private Partitions

![1594542412698](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594542412698.png)

性能开销主要是copy data了两次，如果是行存而且一行tuple很大的话，copy开销其实不小。

* Blocking Partitioning（Radix Partitioning）

![1594543017679](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594543017679.png)

![1594543159223](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594543159223.png)



#### Build Phase

![1594543761432](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594543761432.png)





* hash table

![1594544655342](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594544655342.png)



![1594545216568](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594545216568.png)

### Probe Phase

![1594551036975](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594551036975.png)



* Bloom Filter

![1594551112175](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1594551112175.png)



