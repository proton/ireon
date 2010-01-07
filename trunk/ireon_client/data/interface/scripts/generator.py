# -*- coding: utf-8 -*-
#####Counter, returns new value every time
def counter():
    static_counter = 0
    while True:
        static_counter += 1
        yield static_counter
# define first generator for using in names of PopupWindow on today day
count = counter().next 
# define second generator for using in names references on object method
countMethod = counter().next
countDefault = counter().next