# Recovery Model

## Recovery Algorithm

用于保证数据库的一致性，原子性和持久性

包含两个部分：

* 在事务运行时做的操作，确保崩溃能够恢复
* 故障发生后数据库进行恢复的操作，保证原子性一致性和持久性



## In-Memory Database Recovery

比disk-oriented DBMS更要简单，因为：

崩溃后，内存中所有记录都删掉，从磁盘上的checkpoint处加载

* 不用跟踪硬盘上的脏页
* 不需要存储undo log，只需要redo log
* 不需要记录索引修改的log，因为recovery时候是直接在内存中重建索引，不需要从磁盘上加载索引

但是瓶颈仍然是非易失性存储的慢sync速度。



## Logging Schemes

![1589337978357](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589337978357.png)



举个例子，假如对1billion个tuple进行更新操作,第一种方法需要存储1billion个记录，第二种方法只需要存储1项记录即你更新做的操作。

但是第二种方法的缺点是recovery的过程可能会很慢，因为你是在replay操作，不像第一种方法直接更改就行。

所以大部分系统都是用的第一种方法。

## LOG FLUSHING

![1589338510185](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589338510185.png)

* 方法一：事务完全提交之后，一次性把所有log record落盘
* 方法二：允许在事务提交前就写事务的log

假如要更新1billion条数据，第一种方法需要buffer能够支持存储1billion条数据，内存可能不够用。因此通常采用第二种方案



## 组提交

把多组事务的日志刷盘聚集在一个同一个`fsync`进行

* 日志要么在超时之后或者在buffer满了之后才会刷盘

分摊了多个事务的IO平均开销



## EARLY LOCK RELEASE

![1589339383581](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589339383581.png)

For more info: <https://dzone.com/articles/early-lock-release>

![1589347881276](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589347881276.png)

在开始写transaction log的时候就释放锁，而不是写完才释放锁。这样可以提高并发速度。如果落盘失败了，那么该事务及其之后的txn都会被abort



## USE MVCC TIME-TRAVEL TABLE AS RECOVERY LOG

![1589348243531](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589348243531.png)



大意是利用MVCC的历史版本数据来替代WAL中的undo log，尝试把恢复的时间复杂度降低为常数级，即只和需要读取的版本数量有关。

version的存储方式又可以分为两种:

![1589349136727](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589349136727.png)

其中In-row versioning：把事务号和旧的版本数据直接存在原Table的DELTA列中

![1589349031888](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589349031888.png)

## Recovery Protocols

* Phase1:

从checkpoint加载看是哪个transacion开始。

* Redo:

重做，恢复main table与version store的状态为崩溃发生时的状态。

* Undo:

把未提交的事务标记为aborted，然后通过**logical revert**来增量删除老的版本。



## Logical Revert

![1589351516739](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589351516739.png)



## CHECKPOINT PROTOCOL

![1589352829409](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589352829409.png)

checkpoint的理想情况

* 不应该影响正常事务的处理速度
* 不能引入不能接受的延迟
* 不能引入太多的内存开销

## ![1589353700743](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589353700743.png)



## CHECKPOINT MECHANISM

![1589354646596](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589354646596.png)

父进程fork出一个子进程，子进程的内存会拥有和父进程一模一样的拷贝。而且两者的内存视角是隔离的，父进程可以继续进程事务操作修改数据，而子进程是看不到父进程的修改。这就是Redis的RDB做法。



![1589354999756](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589354999756.png)

那我们的checkpoint具体要存储什么呢？

![1589355508616](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589355508616.png)



checkpoint的频率呢？

![1589356066359](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589356066359.png)



## Restart Protocols

![1589356782891](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589356782891.png)

* SHARED MEMORY RESTART

![1589357741908](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589357741908.png)

## In summary

* Physical logging is a general-purpose approach （物理日志存储的是页中改变的字节，逻辑日志存储的是改变的操作）

* Copy-on-update checkpoints are the way to go especially if you are using MVCC

  





