# code_notes
Some notes about Data structure and Algorithm.

> 总担心在代码里面写中文注释会因为编码问题而产生乱码，所以都写到README里，QAQ。

##### bitree_traversal（二叉树遍历）

非递归二叉树遍历有多种实现方式，这里使用模拟栈的方式来实现，尽量将三种深度优先遍历的代码统一起来。

##### sort（排序）

各种简单的排序算法，select sort、bubble sort、heap sort、merge sort、quick sort。

##### bisearch（二分查找）

实现了find，lower_bound，upper_bound。之前总想用类似于lower_bound的方式实现find，就是不对`nums[mid]==target`做单独的判断，等跳出循环后再根据`nums[left]==target`来判断找没找到，但后来发现不合适，因为还得处理数组越界的问题。