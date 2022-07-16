# OSLAB M1: 打印进程树 (pstree)

这是南京大学JYY讲授的的操作系统课的实验M1的实现

实验要求与手册请看：[http://jyywiki.cn/OS/2022/labs/M1](http://jyywiki.cn/OS/2022/labs/M1)

实现一个三种选项（-V   -n   -p ）和默认的pstree命令，通过读取Linux中 /proc/   下的内容打印出进程树

  /proc/pid/stat 文件中有name和ppid

### 缺陷列表:

> 1. 当进程名称出现空格 `“ ”` 时，如：（tmux：sever），存在bug
>    因为读取/proc/1234(对应进程pid)/stat文件出现空格读取错位存在bug需要解决。
> 2. 考虑更贴合实际pstree的输出：  对于叶子节点 /proc/1234/task/  文件夹下会存在叶子节点开启的子进程( 还会有个该进程的副本文件夹 )文件夹，里面stat文件记录pid和进程名称，且这种进程不会在/proc/ 下出现，pstree中这种进程输出样式为 ``[{   ***  }}]``
> 3. **[已实现]** ~~对错误输入返回非0，对合法输入返回0            如果你的 main 函数返回了非 0 的数值，我们将认为程序报告了错误——在非法的输入上返回 0，以及在合法的输入上返回非 0 都将导致 Wrong Answer。  (实验手册提到的要求)~~
> 4. **[已实现]**  ~~错误检查：open malloc失败时的检查    (实验手册提到的要求)~~
> 5. 思考：(实验手册提到的)
>
>    1. 万一我得到进程号以后，进去发现文件没了 (进程终止了)，怎么办？会不会有这种情况？万一有我的程序会不会 crash……？
>    2. 进程的信息一直在变，文件的内容也一直在变 (两次 `cat` 的结果不同)。那我会不会读到不一致的信息(前一半是旧信息、新一半是新信息)？这两个问题都是 race condition 导致的；我们将会在并发部分回到这个话题。
