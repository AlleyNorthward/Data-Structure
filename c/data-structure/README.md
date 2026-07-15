&emsp;&emsp;@author 巷北  
&emsp;&emsp;@time 2026-07-13 13:55:51  

基本结构划分就是
element - container-owner-element (实际上是个环, 表示owner可以通过容器控制element, 也可是直接控制element, 但是不同语言通过container控制element程度不同, C++可以完全控制, C语言是部分控制, 数据释放必须通过上层用户), 
但是更完全是
element-n*container-owner-element, 后者表示泛型container的element, 也可以是container, 这样会产生一条责任链, 用户侧掌管最近的container, 如果是C++, 内部的element(不论是否是container), 完全不用用户管理, 而如果是C, 那就很凄惨了, 因为element释放必须经由用户之手, 如果element是container, 那么就会有一条长长的责任链. 

所以, *arg是必须的, 就是为了避免element-n*container-owner的情况诞生, 使得用户只需要关注自己使用container最近的element即可.












