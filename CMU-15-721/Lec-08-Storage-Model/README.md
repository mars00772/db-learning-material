### DataBase Storage

#### Type Representation

使用FLOAT,REAL/DOUBLE 与 使用 NUMERIC,DECIMAL,VARCHAR的区别

FLOAT是根据IEEE-754 Standard用二进制来对浮点数进行编码，上过ICS都知道其实这种方式表示浮点数是有误差的。因此在一些不能容忍误差的场合如金融统计等精度要求极高的场景，不能使用float/double，而应该使用numeric/decimal。当然使用float/double速度会快很多

#### Data Layout / Alignment

* 如何表示NULL DATA TYPES IN DB？

choice 1：Special Values

如用INT_MIN来表示NULL

choice 2：Null Column Bitmap Header

在tuple的header存一个bitmap,用单个bit来表示哪些属性是null

choice 3：Per Attribute Null Flag

用一个Flag来表示Null,但是会比较浪费存储空间，并且会有word alignment问题

* Unalignment data reading issue

下图所示：如果要读取cdate这个TS，由于其不是对齐存放的，跨越了第一个64bit word和第二个64bit word，导致读一个64bit的数据可能要读取两个64bit的word。这很影响性能

![1589178048869](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589178048869.png)



Possible Solution:

填充一些0字符，使得强制对齐。

![1589178197120](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589178197120.png)



也可以尝试先排序，尽量把字段对齐地放在一个64bit word里面。但可能也会需要padding

![1589178237850](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589178237850.png)



测试结果：可以看到如果解决好字段存储的对齐问题，性能提高非常明显

![1589178357804](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589178357804.png)

#### Storage Models

* N-ARY STORAGE MODEL(NSM)

行式存储

一个tuple中所有的属性都连续存储在一起。适合OLTP因为OLTP通常涉及少量的tuple及tuple的所有attributes.

![1589179321528](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589179321528.png)

但是不适合OLAP场景，因为OLAP通常是需要某几个列，而不是全部列，举个极端例子如果你只需要海量数据某一个列的数据来做分析，那么你需要把海量数据所有的tuple的所有attribute都加载出来。

![1589179556516](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589179556516.png)

* DECOMPOSITION STORAGE MODEL

列式存储，海量数据的某一列属性连续存储在一起，减少了把数据从索引加载到内存的waste work,因为你只需要读取你所用到的数据

OLAP场景是查询多，但对于增删改会比较慢

![1589179752718](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589179752718.png)

具体实现细节：

* Tuple Identification

![1589180478837](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589180478837.png)

* Data Organization

![1589180706449](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589180706449.png)

#### Insert Order

![1589180882075](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589180882075.png)

#### Sorted Order

![1589181677371](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589181677371.png)

插入的时候需要保证Sorted Table的全局有序

这个时候可能需要把该tuple插入位置及之后的数据全部后移

![1589181861039](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589181861039.png)

#### Partitioned

一个二级索引分别指向不同paritition(可以按照Key来分)，插入数据的时候会定位到具体的partition.

那么寻找数据的时候只需要在这个数据的partition内查找，使得OLAP的查询效率得到提高。

![1589182043662](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589182043662.png)

但事实上我们可以结合两种storage model,因为可以把数据库的数据进行冷热划分:

hot data：刚刚insert到DB的数据, 很有可能最近还会被更新，适合增删改的OLTP，即行式存储。

cold data: 已经insert到DB很久的数据，被更新的几率已经不大，适合查的OLAP，即列式存储。

阿里云的AnalyticDB就是采用这种行列混存的形式存储冷热数据。



![1589182566102](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589182566102.png)

方案一：对行列存储分别使用不同的执行引擎

所有由事务插入到数据库的数据主要都是**行式存储**。

仅当分析性的Query来到要查询数据库数据的时候，要把行式存储的数据复制成**列式存储**的格式

![1589182724655](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589182724655.png)



方案二：只用一套执行引擎

关于delta store可以参考这篇<https://aboutsqlserver.com/2014/05/06/clustered-columnstore-indexes-exploring-delta-store-and-delete-bitmap/>

![1589183293457](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589183293457.png)

回忆一下第三节课讲的Delta Storage: main table只存储最新版本的数据，然后通过指针把历史版本数据连接在一起。

![1589184155405](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589184155405.png)



#### System Catalogs

数据库的Catalog指的是什么？一个数据库系统会包括多个Catalog，每个Catalog下面又有多个Schema。每个Schema下面包含了DB的实例如table,view. 可以理解为命名空间层面上用于进行租户隔离的机制。



#### Schema Changes

NSM:行存 DSM:列存

![1589185233186](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589185233186.png)













