#! /usr/bin/python

#
# Description:
# ================================================================
# Time-stamp: "2022-09-27 13:19:48 trottar"
# ================================================================
#
# Author:  Richard L. Trotta III <trotta@cua.edu>
#
# Copyright (c) trottar
#


def savefile(inp_f):
    with open(inp_f,"r") as f:
        string_f = f.read()
    return string_f

def general_utility():
    return savefile(general_utility.cpp)

def print_data():
    return savefile(print_data.cpp)

def read_setting():
    return savefile(read_setting.cpp)

def root_ana_pl():
    return savefile(root_ana_pl.cpp)
