# usage example: python2 4_process_single_cov.py cov_recorded.info cov_replayed.info

import copy
import sys
import os
import operator
import itertools


def read_blocks(filename):
    block = []
    with open(filename) as fh:
        for line in fh:
            if line.startswith('SF:'):
                if block:
                    yield block
                block = []
            block.append(line)
    if block:
        yield block

def order_info(filename):
    blacklist = ['TN', 'FN', 'end_of', 'FNDA']
    blocks = sorted(list(read_blocks(filename)))
    new_blocks=[]
    for block in blocks:
        block = [word for word in block if not any(bad in word for bad in blacklist)]
        new_blocks.append(block)
    new_blocks = list(filter(None, new_blocks))
    return new_blocks

def accumulate(l):
    it = itertools.groupby(l, operator.itemgetter(0))
    for key, subiter in it:
       yield key, sum(item[1] for item in subiter) 

def merge_info(blocks):
    merged = []
    list_filename = []
    for block in blocks:
        if block[0] not in list_filename:
            list_filename.append(block[0])
    for file in list_filename:
        temp = []
        for block in blocks:
            if (block[0] == file and len(block)>1):
                for elem in block[1:]:
                    elem = elem.replace("\n", "");
                    elem = (elem.split(",")[0], int(elem.split(",")[1]))
                    temp.append(elem)
        #temp.sort(key=lambda y:y[0])
        temp.sort(key=lambda x: int(''.join(filter(str.isdigit, x[0]))))
        temp = list(accumulate(temp))
        temp.insert(0,file)
        merged.append(temp)
    return merged

def filter_list(list, idx_file, start_line, end_line):
    filename = list[idx_file].pop(0)
    list[idx_file] = [x for x in list[idx_file] if not (start_line <= int(x[0].split(":")[1]) <= end_line)]
    list[idx_file].insert(0,filename)
    return list

def filter_info(list):

    # step0 - filter prototype error
    idx_file = 38 #vmx.c
    list = filter_list(list, idx_file, 3918, 4115)

    idx_file = 70 #vmx.h
    list = filter_list(list, idx_file, 377, 397)

    idx_file = 70 #vmx.h
    list = filter_list(list, idx_file, 478, 492)

    idx_file = 110 #xmalloc.h
    list = filter_list(list, idx_file, 83, 94)


    return list


mon_file = sys.argv[1]
inj_file = sys.argv[2]

cov_mon = order_info(mon_file)
cov_mon = merge_info(cov_mon)
cov_mon = filter_info(cov_mon)
cov_inj = order_info(inj_file)
cov_inj = merge_info(cov_inj)
cov_inj = filter_info(cov_inj)


count_diff = 0
for file_mon, file_inj in zip(cov_mon, cov_inj):

    temp_file = []
    for item_mon, item_inj in zip(file_mon, file_inj):
        if (type(item_mon) is str):
            temp_file.append(item_mon.split("\n")[0])
        else:
            if (item_mon[1]>0 and item_inj[1]==0):
                temp_file.append(item_mon[0])
                count_diff+=1
    if len(temp_file)>1:
        for item in temp_file:
            print(item)

print("number of diff: ", count_diff)

