~~~bash
    cmake -S . -B build
    cmake --build build
    cmake -G "Visual Studio 17 2022" -S . -B build
    cmake --build build --config Release
    cmake --install build --prefix ./data_structurelib
~~~

# 简介
本来想着, 具备通用性, 能够在其他语言灵活应用. 但是却忘了, 缺少泛型, 必然不具备通用性. 如果增加泛型, 有少了原始的数据结构的韵味.  
所以, 灵活转移到其它语言, 单靠这本书的内容, 肯定是不行的. 所以需要转变思想, 只是单纯地学习数据结构, 不考虑泛型的问题. 明白其解决的实际问题即可, 后续复杂算法, 再专门移植就行. 
