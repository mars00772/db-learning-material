### SIMD

Single Instruction Multiple Data

以加法指令为例，单指令单数据（[SISD](https://baike.baidu.com/item/SISD)）的CPU对加法指令译码后，执行部件先访问内存，取得第一个[操作数](https://baike.baidu.com/item/%E6%93%8D%E4%BD%9C%E6%95%B0)；之后再一次访问内存，取得第二个操作数；随后才能进行求和运算。而在SIMD型的CPU中，指令译码后几个执行部件同时访问内存，一次性获得所有操作数进行运算。这个特点使SIMD特别适合于多媒体应用等数据密集型运算。

![1589858393563](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589858393563.png)

我的理解是SIMD的大型寄存器允许单周期内把多个数加载到register内

但是往往需要人工来把算法改成SIMD模式

![1589859004861](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589859004861.png)

### VECTORIZATION

* Automatic Vectorization

编译器来自动负责把循环内部的instruction重写为向量化版本。

但是只能适用于简单的循环，并且需要硬件支持

而且有时候vectorize也是不安全的:

![1589872018984](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589872018984.png)



* Compiler Hints

给编译器提供额外信息，告知什么时候才能安全地向量化

![1589872293448](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589872293448.png)

方法一：

![1589872395619](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589872395619.png)



方法二：

![1589872478152](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589872478152.png)

* Explicit Vectorization

![1589872735632](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589872735632.png)



![1589873220738](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589873220738.png)





### Vectorization Direction

* Horizontal

Perform operation on all elements together within a single vector.

* Vertical

Perform operation in an elementwise manner on elements of each vector



![1589879013052](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589879013052.png)



### EXPLICIT VECTORIZATION METHOD

![1589880565504](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589880565504.png)



接下来介绍向量化的一些算法

![1589880695631](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589880695631.png)

### VECTORIZED OPERATORS

* Selection Scans

![1589953372038](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589953372038.png)

* Hash Tables

线性探测法解决哈希冲突的性能瓶颈在于:线性探测部分，即冲突之后在哈希表遍历的部分

向量化的线性探测：128-bit的寄存器允许一次性加载四个key/value,分摊掉原来线性探测的开销。

![1589953782003](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589953782003.png)



* Partitioning / Histograms

![1589955272031](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589955272031.png)



对于OLAP数据库来说，向量化执行是其提高性能的关键。

![1589955079949](C:\Users\AlexanderChiu\AppData\Roaming\Typora\typora-user-images\1589955079949.png)







