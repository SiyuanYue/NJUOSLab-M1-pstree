 **One process may has many children_processes,but has only one parent pid. So in struct pidinfo only strore this process**

**name and pid and ppid.**

```c
struct pidinfo{
	char *name;
	__pid_t pid;
	__pid_t ppid;
};
```
