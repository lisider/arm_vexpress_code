```c

suweishuai@vexpress:/work # ls /dev/input/
event0  event1  mice    mouse0
suweishuai@vexpress:/work # insmod  input_driver1.ko 
input: I am simplest input subsystem as /devices/virtual/input/input3
OK,input subsystem inited!
suweishuai@vexpress:/work # mdev -s
suweishuai@vexpress:/work # ls /dev/input/
event0  event1  event2  mice    mouse0
suweishuai@vexpress:/work # ./evtest /dev/input/event2 




suweishuai@vexpress:/work # ls /dev/input/
event0  event1  mice    mouse0
suweishuai@vexpress:/work # insmod  input_driver2.ko 
input: test_input as /devices/virtual/input/input3
suweishuai@vexpress:/work # mdev -s
suweishuai@vexpress:/work # ls /dev/input/
event0  event1  event2  mice    mouse0
suweishuai@vexpress:/work # ./evtest /dev/input/event2 


##### 设置 hotplug 程序
wit@vexpress:/ # echo "/sbin/mdev" > /proc/sys/kernel/hotplug
wit@vexpress:/ # cat /proc/sys/kernel/hotplug
/sbin/mdev

##### 当 device_create 的时候 会 调用 call_usermodehelper 来 调用 /sbin/mdev 来在 /dev 创建设备
wit@vexpress:/work # ls /dev/input/
event0  event1  mice    mouse0
wit@vexpress:/work # ls
input_driver1.ko
wit@vexpress:/work # insmod input_driver1.ko 
input: I am simplest input subsystem as /devices/virtual/input/input3
OK,input subsystem inited!
OK,input subsystem inited!
wit@vexpress:/work # ls /dev/input/
event0  event1  event2  mice    mouse0
wit@vexpress:/work # hexdump /dev/input/event2 
0000000 6dae 5c2b 9d64 0006 0003 0009 000a 0000
0000010 6dae 5c2b 9d64 0006 0000 0000 0000 0000
0000020 6db0 5c2b bf10 0006 0003 0009 000b 0000
0000030 6db0 5c2b bf10 0006 0000 0000 0000 0000


```
