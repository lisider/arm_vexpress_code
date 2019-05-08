```c

# ls /sys/ -R |grep hello
hello_class
/sys/class/hello_class:
    hello_class
    hello_class
/sys/devices/virtual/hello_class:
        hello_class
        /sys/devices/virtual/hello_class/hello_class:
            /sys/devices/virtual/hello_class/hello_class/power:


# cat /proc/devices  |grep hello
                252 hello_dev

```
