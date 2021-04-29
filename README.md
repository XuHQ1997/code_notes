# code_notes
Some notes about Data structure and Algorithm.

> 总担心在代码里面写中文注释会因为编码问题而产生乱码，所以都写到README里，QAQ。

##### bitree_traversal（二叉树遍历）

非递归二叉树遍历有多种实现方式，这里使用模拟栈的方式来实现，尽量将三种深度优先遍历的代码统一起来。

##### sort（排序）

各种简单的排序算法，select sort、bubble sort、heap sort、merge sort、quick sort、radix sort。这里实现的radix sort仅支持非负数，所以test时生成的样本也都是非负的。

##### bisearch（二分查找）

实现了find，lower_bound，upper_bound。之前总想用类似于lower_bound的方式实现find，就是不对`nums[mid]==target`做单独的判断，等跳出循环后再根据`nums[left]==target`来判断找没找到，但后来发现不合适，因为还得处理`nums[left]`有没有越界的问题。

##### bilist（双向链表）

看到链表就想到fake head，就像说起西红柿就想到炒鸡蛋一样自然。双向链表还要加个fake tail。

##### 01packages（01背包）

朴素的01背包求最大价值问题，dfs、dp、空间优化的dp。这次写代码错的两个问题：

1. 朴素dp中，`dp[i][j]`里面的`i`是从1开始的，不能直接索引物品体积数组和价值数组。
2. 空间优化的dp中，对`j`的遍历需要反着来，因为`dp[i][j]`依赖于`dp[i-1][0:j+1]`。

##### math_tools（一些数学问题）

最小公倍数gcd、不溢出地计算组合数、线性素数筛。

##### permutation（按字典序的下一个排列）

next_permutation、prev_permutation。和STL使用相同的算法，但是行为没有和STL完全一致。当没有next permutaion时，`std::next_permutation`返回`false`，并将输入变为单调递增数列，而这里的实现直接返回。

##### topk（最大的k个数）

三种算法，思路分别类似于bubble sort、quick sort、heap sort。实现中的其实是“top k+1”，即保证`nums[k]`是第k+1大的数，`nums[0:k]`都大于等于`nums[k]`，`nums[k+1:]`都小于等于`nums[k]`。