import copy
import sys
import os
import operator
import itertools
import collections

exit_reasons_name={

    '0':'EXCEPTION_NMI',
    '1':'EXTERNAL_INTERRUPT',
    '2':'TRIPLE_FAULT',
    '3':'INIT',
    '4':'SIPI',
    '5':'IO_SMI',
    '6':'OTHER_SMI',
    '7':'PENDING_VIRT_INTR',
    '8':'PENDING_VIRT_NMI',
    '9':'TASK_SWITCH',
    'a':'CPUID',
    'b':'GETSEC',
    'c':'HLT',
    'd':'INVD',
    'e':'INVLPG',
    'f':'RDPMC',
    '10':'RDTSC',
    '11':'RSM',
    '12':'VMCALL',
    '13':'VMCLEAR',
    '14':'VMLAUNCH',
    '15':'VMPTRLD',
    '16':'VMPTRST',
    '17':'VMREAD',
    '18':'VMRESUME',
    '19':'VMWRITE',
    '1a':'VMXOFF',
    '1b':'VMXON',
    '1c':'CR_ACCESS',
    '1d':'DR_ACCESS',
    '1e':'IO_INSTRUCTION',
    '1f':'MSR_READ',
    '20':'MSR_WRITE',
    '21':'INVALID_GUEST_STATE',
    '22':'MSR_LOADING',
    '24':'MWAIT_INSTRUCTION',
    '25':'MONITOR_TRAP_FLAG',
    '27':'MONITOR_INSTRUCTION',
    '28':'PAUSE_INSTRUCTION',
    '29':'MCE_DURING_VMENTRY',
    '2b':'TPR_BELOW_THRESHOLD',
    '2c':'APIC_ACCESS',
    '2d':'EOI_INDUCED',
    '2e':'ACCESS_GDTR_OR_IDTR',
    '2f':'ACCESS_LDTR_OR_TR',
    '30':'EPT_VIOLATION',
    '31':'EPT_MISCONFIG',
    '32':'INVEPT',
    '33':'RDTSCP',
    '34':'VMX_PREEMPTION_TIMER_EXPIRED',
    '35':'INVVPID',
    '36':'WBINVD',
    '37':'XSETBV',
    '38':'APIC_WRITE',
    '39':'INVPCID',
    '3b':'VMFUNC',
    '3e':'PML_FULL',
    '3f':'XSAVES',
    '40':'XRSTORS'
}



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
        temp.sort(key=lambda x: int(''.join(filter(str.isdigit, x[0]))))
        temp = list(accumulate(temp))
        temp.insert(0,file)
        merged.append(temp)
    return merged

def merge_diff(diff_list):
    merged = []
    list_filename = []
    for diff in diff_list:
        if diff[0] not in list_filename:
            list_filename.append(diff[0])
    for file in list_filename:
        temp = []
        for diff in diff_list:
            if (diff[0] == file):
                for elem in diff[1:]:
                    temp.append(int(elem.split(":")[1]))
        temp.sort()
        temp = collections.Counter(temp).most_common()
        temp = sorted(temp, key=lambda tup: tup[0])
        temp.insert(0, file)

        merged.append(temp)
    return merged


def filter_list(list, idx_file, start_line, end_line):
    filename = list[idx_file].pop(0)
    list[idx_file] = [x for x in list[idx_file] if not (start_line <= int(x[0].split(":")[1]) <= end_line)]
    list[idx_file].insert(0,filename)
    return list

def filter_info(list):
    # vmx.c
    idx_file = 38
    start_line = 3918
    end_line = 4115
    list = filter_list(list, idx_file, start_line, end_line)
    # vmx.h
    idx_file = 70
    start_line = 377
    end_line = 397
    list = filter_list(list, idx_file, start_line, end_line)
    # vmx.h
    idx_file = 70
    start_line = 478
    end_line = 492
    list = filter_list(list, idx_file, start_line, end_line)
    #xmalloc.h
    idx_file = 110
    start_line = 83
    end_line = 94
    list = filter_list(list, idx_file, start_line, end_line)
    return list


def get_exit_reason(filename):
    with open(filename, 'r') as file:
        list = file.readlines()
        list = [line.rstrip() for line in list]
        idx = list.index("4402")
        reason = list[idx+1]
        return exit_reasons_name[reason]


dir1 = sys.argv[1]
dir2 = sys.argv[2]

list_mon = [os.path.abspath(os.path.join(dir1, p)) for p in os.listdir(dir1)]

list_inj = [os.path.abspath(os.path.join(dir2, p)) for p in os.listdir(dir2)]


iter = 0
list_diff = []
for mon_file, inj_file in zip(list_mon, list_inj):


    cov_mon = order_info(mon_file)
    cov_mon = merge_info(cov_mon)
    cov_mon = filter_info(cov_mon)
    cov_inj = order_info(inj_file)
    cov_inj = merge_info(cov_inj)
    cov_inj = filter_info(cov_inj)

    count_diff = 0
    temp_seed=[]
    for file_mon, file_inj in zip(cov_mon, cov_inj):
        temp_file = []
        for item_mon, item_inj in zip(file_mon, file_inj):
            if (type(item_mon) is str):
                temp_file.append(item_mon.split("\n")[0])
            else:
                if (item_mon[1]>0 and item_inj[1]==0):
                    temp_file.append(item_mon[0])
                    count_diff+=1
        if (len(temp_file)>1):
            temp_seed.append(temp_file)

    if (count_diff > 0):
         for file in temp_seed:
             list_diff.append(file)



res = merge_diff(list_diff)
for seed in res:
    for file in seed:
        print(file)
