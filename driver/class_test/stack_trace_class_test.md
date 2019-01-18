- open
```c
CPU: 0 PID: 708 Comm: char_user_test Tainted: G           O    4.0.0+ #134
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543578>] (dump_stack+0x80/0x90)
[<80543578>] (dump_stack) from [<7f0000a8>] (hello_open+0x24/0x2c [class_test])
[<7f0000a8>] (hello_open [class_test]) from [<8011b5a4>] (chrdev_open+0xdc/0x19c)
[<8011b5a4>] (chrdev_open) from [<80115020>] (do_dentry_open.isra.2+0x204/0x31c)
[<80115020>] (do_dentry_open.isra.2) from [<801151d4>] (vfs_open+0x54/0x58)
[<801151d4>] (vfs_open) from [<80124a74>] (do_last.isra.17+0x458/0xc8c)
[<80124a74>] (do_last.isra.17) from [<80125334>] (path_openat+0x8c/0x5f8)
[<80125334>] (path_openat) from [<80126818>] (do_filp_open+0x3c/0x98)
[<80126818>] (do_filp_open) from [<80116290>] (do_sys_open+0x120/0x1e4)
[<80116290>] (do_sys_open) from [<8011637c>] (SyS_open+0x28/0x2c)
[<8011637c>] (SyS_open) from [<8000ed34>] (__sys_trace_return+0x0/0x2c)
```
- write
```c
CPU: 0 PID: 708 Comm: char_user_test Tainted: G           O    4.0.0+ #134
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543578>] (dump_stack+0x80/0x90)
[<80543578>] (dump_stack) from [<7f000050>] (hello_write+0x24/0x2c [class_test])
[<7f000050>] (hello_write [class_test]) from [<80117368>] (vfs_write+0xb0/0x1c0)
[<80117368>] (vfs_write) from [<801179fc>] (SyS_write+0x4c/0xa0)
[<801179fc>] (SyS_write) from [<8000ed34>] (__sys_trace_return+0x0/0x2c)
```
- read
```c
CPU: 0 PID: 708 Comm: char_user_test Tainted: G           O    4.0.0+ #134
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543578>] (dump_stack+0x80/0x90)
[<80543578>] (dump_stack) from [<7f000024>] (hello_read+0x24/0x2c [class_test])
[<7f000024>] (hello_read [class_test]) from [<801177c8>] (__vfs_read+0x28/0x5c)
[<801177c8>] (__vfs_read) from [<80117884>] (vfs_read+0x88/0x114)
[<80117884>] (vfs_read) from [<8011795c>] (SyS_read+0x4c/0xa0)
[<8011795c>] (SyS_read) from [<8000ed34>] (__sys_trace_return+0x0/0x2c)
```
- close
```c
CPU: 0 PID: 708 Comm: char_user_test Tainted: G           O    4.0.0+ #134
Hardware name: ARM-Versatile Express
[<80016cb0>] (unwind_backtrace) from [<80012bc4>] (show_stack+0x20/0x24)
[<80012bc4>] (show_stack) from [<80543578>] (dump_stack+0x80/0x90)
[<80543578>] (dump_stack) from [<7f00007c>] (hello_close+0x24/0x2c [class_test])
[<7f00007c>] (hello_close [class_test]) from [<80118570>] (__fput+0x90/0x1d8)
[<80118570>] (__fput) from [<80118728>] (____fput+0x18/0x1c)
[<80118728>] (____fput) from [<80040c04>] (task_work_run+0xbc/0xf4)
[<80040c04>] (task_work_run) from [<800341e4>] (ptrace_notify+0xa8/0xb8)
[<800341e4>] (ptrace_notify) from [<80011440>] (syscall_trace_exit+0x84/0x110)
[<80011440>] (syscall_trace_exit) from [<8000ed40>] (__sys_trace_return+0xc/0x2c)
```
