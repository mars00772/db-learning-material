### Query Optimization

定义：

给定Query,寻找一个代价最小的执行计划。但这是一个NP完全问题，并没有一个优化器能够找到最优的执行计划:

* 使用estimation techniques来估算计划的成本
* 使用启发式搜索来限制搜索空间

讨论主题：

* Optimizer实现
* Query的重写
* 执行计划枚举
* adaptive query optimization
* 代价模型



![1590765737091](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1590765737091.png)



1.客户端发送SQL Query到服务端的SQL Rewriter进行重写

2.重写后的SQL Query经过Parser进行词法分析和语法分析，生成一棵抽象语法树，语法树的Token分别对应Table或者Column的Name

3.AST传入Binder,结合Catalog把Token转化为内部的Identifer，这时候生成的是逻辑计划

4.然后逻辑计划经过Tree Rewriter进行重写，应用一些改写规则如重写View，谓词下移，最后会生成优化后的Logical Plan

5.最后逻辑计划经过Optimizer生成Physical Plan.

### Logical vs. Physical Plan区别

Optimizer生成的是从逻辑代数表达式到物理代数表达式的一个映射。

eg:访问Table foo

逻辑计划的内容是: access Table foo

物理计划的内容是: access Table foo using index XYZ or using sequential scan/binary search

逻辑计划强调的是What your results look like

物理计划强调的是How you get your results

Physical operators 使用`access path` 来决定具体的执行策略。

* 依赖于处理数据的物理格式
* 并不总是1:1的映射

### OLTP PLANNING IS EASY

![1590822613074](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1590822613074.png)



sargable的意思是可以利用索引来加速数据的查找，一般来说对于OLTP的查询优化一般只需要选取最佳的索引，然后JOIN操作使用外键，采取一些简单的策略即可。

启发式搜索：

如尽可能进行谓词下移，那么在进行JOIN的时候涉及到的数据量就会减少

![1590823362243](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1590823362243.png)

### COST ESTIMATION

所需要考虑的因素有：

* 在DBMS中与其他因素交互
* 中间结果的大小
* 访问方法与算法的选择
* CPU,IO,Network的资源利用
* 数据的属性(skew,order,placement)



## 优化器的知识点

### 优化粒度

单查询

更小的搜索空间。
DBMS不能在查询中重复使用结果。
为了解决资源争夺问题，成本模型必须考虑当前正在运行的内容。
多查询

如果有很多类似的查询，效率会更高。
搜索空间要大得多。
用于扫描共享（之后的文章会提到，大致就是多个查询共用一个扫描表的结果，而不是各自扫描一次）

### 优化时间

**静态优化**

在执行之前选择最佳计划。
计划质量取决于成本模型的准确性。
可以使用prepared statements分摊执行。

**动态优化**

主要在流式数据的时候使用
将重新优化多次执行。
难以实现/调试（非确定性）

**混合优化**

使用静态算法编译。
如果估计>阈值的误差，则重新优化。

### 准备好的声明

![1590828472938](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1590828472938.png)

假设有这么一条SQL

![1590828495280](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1590828495280.png)

可以把这个SQL转化为prepared statements,其中A,B,C的val参数可能会改变

![1590828545486](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1590828545486.png)



那么怎么去决定这个prepared statements的执行计划？

![1590828724032](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1590828724032.png)







### 计划稳定性

**提示**

运行数据库系统管理员帮助优化器进行决策，比如，我希望用hash表进行JOIN等
mysql、sqlserver似乎都支持这种方法

**固定优化器版本**

设置优化器版本号并将查询逐个迁移到新优化器。比如，虽然用了新版本的数据库，但是对于某种特定的查询，我们发现了旧版本的优化器似乎更快，则可以将版本调回去。

**向后兼容的计划**

从旧版本保存备份查询计划，并将其提供给新的DBMS。


### 搜索停止







