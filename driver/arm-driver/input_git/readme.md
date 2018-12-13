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


```
