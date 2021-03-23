# code_notes
Some notes about Data structure and Algorithm.

> 总担心在代码里面写中文注释会因为编码问题而产生乱码，所以都写到README里，QAQ。

##### bitree_traversal（二叉树遍历）

非递归二叉树遍历有多种实现方式，这里使用模拟栈的方式来实现，尽量将三种深度优先遍历的代码统一起来。

##### sort（排序）

各种简单的排序算法，select sort、bubble sort、heap sort、merge sort、quick sort、radix sort。

##### bisearch（二分查找）

实现了find，lower_bound，upper_bound。之前总想用类似于lower_bound的方式实现find，就是不对`nums[mid]==target`做单独的判断，等跳出循环后再根据`nums[left]==target`来判断找没找到，但后来发现不合适，因为还得处理`nums[left]`有没有越界的问题。

##### list（双向链表）

看到链表就想到fake head，就像说起西红柿就想到炒鸡蛋一样自然。双向链表还要加个fake tail。

##### 01packages（01背包）

朴素的01背包求最大价值问题，dfs、dp、空间优化的dp。这次写代码错的两个问题：

1. 朴素dp中，`dp[i][j]`里面的`i`是从1开始的，不能直接索引物品体积数组和价值数组。
2. 空间优化的dp中，对`j`的遍历需要反着来，因为`dp[i][j]`依赖于`dp[i-1][0:j+1]`。

##### math_tools（一些数学问题）

最小公倍数gcd、不溢出地计算组合数、线性素数筛。