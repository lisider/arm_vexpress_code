```c

系统调用的入口是 vector_swi

```

```c
进入vector_swi后的执行流程：
        (1)在栈上备份中断前的寄存器。
        (2)获取syscall number(r7)。
        (3)进入具体syscall函数的准备工作
            r8-->tbl,r9-->tsk,lr-->新的返回地址（ret_fast_syscall)
            r0-r5 6个C函数的参数放在对应位置
        (4)通过tbl记录的table入口和r7记录的偏移量，进入相应的系统调用。
        (5)返回到ret_fast_syscall,具体来说原理都一样，不再分析了


```
```c
像每一个异常处理程序一样，要做的第一件事当然就是保护现场了。
紧接着是获得系统调用的系统调用号。
然后以系统调用号作为索引来查找系统调用表，
如果系统调用号正常的话，就会调用相应的处理例程来处理，就是上面的那个ldrcc  pc, [tbl, scno, lsl #2]语句
然后通过例程ret_fast_syscall来返回。

```

- open
```c
CPU: 0 PID: 704 Comm: char_user_test Tainted: G           O    4.0.0+ #62
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543528>] (dump_stack+0x80/0x90)
[<80543528>] (dump_stack) from [<7f0040a8>] (hello_open+0x24/0x2c [class_test])
[<7f0040a8>] (hello_open [class_test]) from [<8011b5a4>] (chrdev_open+0xdc/0x19c)
[<8011b5a4>] (chrdev_open) from [<80115020>] (do_dentry_open.isra.2+0x204/0x31c)
[<80115020>] (do_dentry_open.isra.2) from [<801151d4>] (vfs_open+0x54/0x58)
[<801151d4>] (vfs_open) from [<80124a74>] (do_last.isra.17+0x458/0xc8c)
[<80124a74>] (do_last.isra.17) from [<80125334>] (path_openat+0x8c/0x5f8)
[<80125334>] (path_openat) from [<80126818>] (do_filp_open+0x3c/0x98)
[<80126818>] (do_filp_open) from [<80116290>] (do_sys_open+0x120/0x1e4)
[<80116290>] (do_sys_open) from [<8011637c>] (SyS_open+0x28/0x2c)
[<8011637c>] (SyS_open) from [<8000ebc0>] (ret_fast_syscall+0x0/0x34)
```

- write
```c
CPU: 0 PID: 704 Comm: char_user_test Tainted: G           O    4.0.0+ #62
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543528>] (dump_stack+0x80/0x90)
[<80543528>] (dump_stack) from [<7f004050>] (hello_write+0x24/0x2c [class_test])
[<7f004050>] (hello_write [class_test]) from [<80117368>] (vfs_write+0xb0/0x1c0)
[<80117368>] (vfs_write) from [<801179fc>] (SyS_write+0x4c/0xa0)
[<801179fc>] (SyS_write) from [<8000ebc0>] (ret_fast_syscall+0x0/0x34)
```
- read
```c
CPU: 0 PID: 704 Comm: char_user_test Tainted: G           O    4.0.0+ #62
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543528>] (dump_stack+0x80/0x90)
[<80543528>] (dump_stack) from [<7f004024>] (hello_read+0x24/0x2c [class_test])
[<7f004024>] (hello_read [class_test]) from [<801177c8>] (__vfs_read+0x28/0x5c)
[<801177c8>] (__vfs_read) from [<80117884>] (vfs_read+0x88/0x114)
[<80117884>] (vfs_read) from [<8011795c>] (SyS_read+0x4c/0xa0)
[<8011795c>] (SyS_read) from [<8000ebc0>] (ret_fast_syscall+0x0/0x34)
```

- close
```c
CPU: 0 PID: 704 Comm: char_user_test Tainted: G           O    4.0.0+ #62
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543528>] (dump_stack+0x80/0x90)
[<80543528>] (dump_stack) from [<7f00407c>] (hello_close+0x24/0x2c [class_test])
[<7f00407c>] (hello_close [class_test]) from [<80118570>] (__fput+0x90/0x1d8)
[<80118570>] (__fput) from [<80118728>] (____fput+0x18/0x1c)
[<80118728>] (____fput) from [<80040c04>] (task_work_run+0xbc/0xf4)
[<80040c04>] (task_work_run) from [<8001241c>] (do_work_pending+0x8c/0xb4)
[<8001241c>] (do_work_pending) from [<8000ec04>] (work_pending+0xc/0x20)
```
