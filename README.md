# DataBases & Distributed Systems

数据库与分布式系统学习资料总结,有缘人进来点个star,祝您一生平安靴靴 Welcome to star! 

## <a name='TOC'>Table of Contents</a>

  1. [Course Material](#CourseMaterial)
  2. [Papers](#Papers)
  3. [OpenSource](#OpenSource)
  4. [PaperReading Notes](#PaperReading)
  5. [Code](#Code)
  6. [Lecture-Notes](#LectureNotes)
 
### <a name='CourseMaterial'> Course Material
  
[CMU-15-445/645](https://15445.courses.cs.cmu.edu/fall2019/) Intro to Database Systems (Fall 2019)

[CMU-15-721-英字Video](https://www.bilibili.com/video/BV1Wz411b7sD?from=search&seid=1785395184520069316)

[CMU-15-721-Course-website](https://15721.courses.cs.cmu.edu/spring2020/)  Advanced Database Systems(Spring 2020)

这个repo中的CMU-15-721包含了每一个课堂的Slides,我的课堂笔记以及每堂课必读的论文

[MIT.6.824 英字Video](https://www.bilibili.com/video/av91748150) Distributed Systems

[MIT.6.830 Database Systems](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-830-database-systems-fall-2010/index.htm) 接下来的计划应该就是上这门课，写SimpleDB


### <a name='Papers'> Papers Reading List

[rxin的数据库经典论文](https://github.com/rxin/db-readings#data-parallel)

[PingCAP的数据库入门总结](https://github.com/pingcap/awesome-database-learning)

### <a name='OpenSource'> OpenSource Project


### <a name='PaperReading'> My paper reading notes
 
[What's new with NewSQL](https://github.com/AlexanderChiuluvB/db-learning-material/tree/master/CMU-15-721/Lec-01-History-Of-Databases)

这篇是15-721第一门课要求读的论文，讲述了New SQL的特性&HTAP是趋势

[原始Paper](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-01-History-Of-Databases/pavlo-newsql-sigmodrec2016.pdf)

[Architecture of a Database System](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-01-History-Of-Databases/DBArchitectureLearningNotes.md)

数据库入门必备，让你对关系型数据库有一个整体架构的理解。这篇论文PingCAP在B站有导读。

[原始Paper](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-01-History-Of-Databases/fntdb07-architecture.pdf)

[An Evaluation of Concurrency Control with One Thousand Cores](https://blog.csdn.net/weixin_38499215/article/details/105883971)

探讨多种并发控制策略在多核CPU下的性能瓶颈

[原始Paper](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-02-In-Memory-Databases/p209-yu.pdf)

[An Empirical Evaluation of In-Memory MVCC](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-03-MVCC-design/MVCC-LEC01.pdf)

Andy口中世界上南波万的关于MVCC的paper..读就完事了

[原始Paper](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-03-MVCC-design/wu-vldb2017.pdf)

### <a name='Code'> Code from scratch

尝试实现一些数据库与分布式系统领域常见的数据结构,一些太复杂的以阅读别人实现好的轮子代替.

* skipList

  [Simple skipList with Java](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/Data-structure/src/main/java/skipList/skipList.java)

* Trie Tree

* Radix Tree

* B/B+ Tree

* LSM Tree

* LRU

* LFU

* Hashmap

* Red-Black Tree

* Bloom Filter

* Roaring BitMap

* BitMap

  [A very simple BitMap demo based on JDK BitSet](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/Data-structure/src/main/java/BitMap/BitMap.java)

  参考BitSet写的一个很弱智的bitmap

  [BitSet源码解读](https://blog.csdn.net/weixin_38499215/article/details/105996592)
  
  
  
 ### <a name='LectureNotes'> LectureNotes
  
 #### CMU 15-721
 
 [Lec-01 History Of DataBases](https://github.com/AlexanderChiuluvB/db-learning-material/tree/master/CMU-15-721/Lec-01-History-Of-Databases)
 
 [Lec-02 In-memory Databases](https://github.com/AlexanderChiuluvB/db-learning-material/tree/master/CMU-15-721/Lec-02-In-Memory-Databases)
 
 [Lec-03 MVCC-design](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-03-MVCC-design/MVCC-LEC01.pdf)
 
 [Lec-04 MVCC-Protocol](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-04-MVCC-Protocol/04-mvcc2-notes.pdf)
 
 [Lec-05 MVCC-GC](https://github.com/AlexanderChiuluvB/db-learning-material/tree/master/CMU-15-721/Lec-05-MVCC-GC)
 
 [Lec-06 OLTP-BTree](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-06-OLTP-BTree/lecture-notes.pdf)
 
 [Lec-07 OLTP-Trie](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-07-OLTP-Trie/Lec-note.pdf)
 
 [Lec-08 Storage Model](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-08-Storage-Model/README.pdf)

 [Lec-09 Data Compression](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-09-Database-Compression/lec-notes.pdf)
 
[Lec-10 Recovery Model](https://github.com/AlexanderChiuluvB/db-learning-material/blob/master/CMU-15-721/Lec-10-Recovery-Model/lec-notes.pdf)
  
  
