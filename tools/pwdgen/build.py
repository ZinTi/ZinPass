#!/usr/bin/python3
import os
print("编译PasswordGenerator")
os.system("gcc pwdgen.c color.c -o pwdgen -I. -Os")
