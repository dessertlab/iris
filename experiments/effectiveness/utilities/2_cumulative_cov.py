# Desc: this script evaluates cumulative coverage processing parsed coverage data (.info)
# Where to execute: anywhere
# Usage example: python2 2_cumulative_cov.py /data/CPU_workload
# @param: path to the folder containing the workload data 



import copy
import sys
import os
import operator
import itertools


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


def get_exit_reason(filename):
    with open(filename, 'r') as file:
        list = file.readlines()
        list = [line.rstrip() for line in list]
        idx = list.index("4402")
        reason = list[idx+1]
        return exit_reasons_name[reason]




dir_rec = sys.argv[1] + "parsed_coverage/cov_recorded"
list_cov_rec = [os.path.abspath(os.path.join(dir_rec, p)) for p in os.listdir(dir_rec)]
list_cov_rec.sort(key=lambda x: int(''.join(filter(str.isdigit, x))))

dir_rep = sys.argv[1] + "parsed_coverage/cov_replayed"
list_cov_rep = [os.path.abspath(os.path.join(dir_rep, p)) for p in os.listdir(dir_rep)]
list_cov_rep.sort(key=lambda x: int(''.join(filter(str.isdigit, x))))



# Create temp vector
cov_temp = order_info(list_cov_rec[0])
cov_temp = merge_info(cov_temp)
cumulative_vect_rec = []
cumulative_vect_rep = []
for instr_file in cov_temp:
    cumulative_vect_rec.append([])
    cumulative_vect_rep.append([])



list_cov_rec = list_cov_rec[1:]
list_cov_rep = list_cov_rep[1:]

iter = 0
file = open("results_cumulative", "w")
file.write ("filename" + "\t\t" + "count_cov_rec" + "\t" + "count_cumulative_rec" + "\t" + "count_cov_rep" + "\t" + "count_cumulative_rep" +"\n")
for filename_rec, filename_rep in zip(list_cov_rec, list_cov_rep):


    count_cov_rec = 0
    count_cov_rep = 0

    # Order and filter coverage data
    cov_data_rec = order_info(filename_rec)
    cov_data_rec = merge_info(cov_data_rec)
    cov_data_rec = filter_info(cov_data_rec)

    cov_data_rep = order_info(filename_rep)
    cov_data_rep = merge_info(cov_data_rep)
    cov_data_rep = filter_info(cov_data_rep)

    # For each instrumented file within coverage data
    idx_instr_file=0
    for instr_file_rec, instr_file_rep in zip(cov_data_rec, cov_data_rep):
        for cov_line_rec, cov_line_rep in zip(instr_file_rec, instr_file_rep):
            if (type(cov_line_rec) is tuple):
                if (cov_line_rec[1]>0):
                    cumulative_vect_rec[idx_instr_file].append(cov_line_rec)
                    count_cov_rec+=1
                if (cov_line_rec[1]>0 and cov_line_rep[1]>0):
                    cumulative_vect_rep[idx_instr_file].append(cov_line_rep)
                    count_cov_rep+=1
        idx_instr_file+=1


    count_cumulative_rec = 0
    for elem_rec in cumulative_vect_rec:
        elem_rec.sort(key=lambda elem_rec: int(''.join(filter(str.isdigit, elem_rec[0]))))
        elem_rec = list(accumulate(elem_rec))
        for line_rec in elem_rec:
             count_cumulative_rec+=1

    count_cumulative_rep = 0
    for elem_rep in cumulative_vect_rep:
        elem_rep.sort(key=lambda elem_rep: int(''.join(filter(str.isdigit, elem_rep[0]))))
        elem_rep = list(accumulate(elem_rep))
        for line_rep in elem_rep:
             count_cumulative_rep+=1

    file.write(str(filename_rec.split("/")[-1]) + "\t" + str(count_cov_rec) + "\t\t" + str(count_cumulative_rec) + "\t\t\t" + str(count_cov_rep) + "\t\t" + str(count_cumulative_rep) +"\n")
    print(filename_rec.split("/")[-1], count_cov_rec, count_cov_rep, count_cumulative_rec, count_cumulative_rep)


file.close()



