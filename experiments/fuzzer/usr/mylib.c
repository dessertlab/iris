#include <stdint.h>
#ifndef MYLIB_H
#include "mylib.h"
#endif
const char* exit_reason_name[65] = {
    [0]="EXCEPTION_NMI",
    [1]="EXTERNAL_INTERRUPT",
    [2]="TRIPLE_FAULT",
    [3]="INIT",
    [4]="SIPI",
    [5]="IO_SMI",
    [6]="OTHER_SMI",
    [7]="PENDING_VIRT_INTR",
    [8]="PENDING_VIRT_NMI",
    [9]="TASK_SWITCH",
    [10]="CPUID",
    [11]="GETSEC",
    [12]="HLT",
    [13]="INVD",
    [14]="INVLPG",
    [15]="RDPMC",
    [16]="RDTSC",
    [17]="RSM",
    [18]="VMCALL",
    [19]="VMCLEAR",
    [20]="VMLAUNCH",
    [21]="VMPTRLD",
    [22]="VMPTRST",
    [23]="VMREAD",
    [24]="VMRESUME",
    [25]="VMWRITE",
    [26]="VMXOFF",
    [27]="VMXON",
    [28]="CR_ACCESS",
    [29]="DR_ACCESS",
    [30]="IO_INSTRUCTION",
    [31]="MSR_READ",
    [32]="MSR_WRITE",
    [33]="INVALID_GUEST_STATE",
    [34]="MSR_LOADING",
    [36]="MWAIT_INSTRUCTION",
    [37]="MONITOR_TRAP_FLAG",
    [39]="MONITOR_INSTRUCTION",
    [40]="PAUSE_INSTRUCTION",
    [41]="MCE_DURING_VMENTRY",
    [43]="TPR_BELOW_THRESHOLD",
    [44]="APIC_ACCESS",
    [45]="EOI_INDUCED",
    [46]="ACCESS_GDTR_OR_IDTR",
    [47]="ACCESS_LDTR_OR_TR",
    [48]="EPT_VIOLATION",
    [49]="EPT_MISCONFIG",
    [50]="INVEPT",
    [51]="RDTSCP",
    [52]="VMX_PREEMPTION_TIMER_EXPIRED",
    [53]="INVVPID",
    [54]="WBINVD",
    [55]="XSETBV",
    [56]="APIC_WRITE",
    [58]="INVPCID",
    [59]="VMFUNC",
    [62]="PML_FULL",
    [63]="XSAVES",
    [64]="XRSTORS"
};


uint64_t vmcs_fields_vett[] = {
	0x00000000, // = "VIRTUAL_PROCESSOR_ID",
	0x00000002, // = "POSTED_INTR_NOTIFICATION_VECTOR",
	0x00000004, // = "EPTP_INDEX",
	//GUEST_SEG_SELECTOR(sel) (GUEST_ES_SELECTOR + (sel) * 2) /* ES ... GS */
	0x00000800, // = "GUEST_ES_SELECTOR",
	0x00000802, // = "GUEST_CS_SELECTOR",
	0x00000804, // = "GUEST_SS_SELECTOR",
    0x00000806, // = "GUEST_DS_SELECTOR",
	0x00000808, // = "GUEST_FS_SELECTOR",
    0x0000080a, // = "GUEST_GS_SELECTOR",
    0x0000080c, // = "GUEST_LDTR_SELECTOR",
    0x0000080e, // = "GUEST_TR_SELECTOR",
    0x00000810, // = "GUEST_INTR_STATUS",
    0x00000812, // = "GUEST_PML_INDEX",
    0x00000c00, // = "HOST_ES_SELECTOR",
    0x00000c02, // = "HOST_CS_SELECTOR",
    0x00000c04, // = "HOST_SS_SELECTOR",
    0x00000c06, // = "HOST_DS_SELECTOR",
    0x00000c08, // = "HOST_FS_SELECTOR",
    0x00000c0a, // = "HOST_GS_SELECTOR",
    0x00000c0c, // = "HOST_TR_SELECTOR",
    0x00002000, // = "IO_BITMAP_A",
    0x00002002, // = "IO_BITMAP_B",
    0x00002004, // = "MSR_BITMAP",
    0x00002006, // = "VM_EXIT_MSR_STORE_ADDR",
    0x00002008, // = "VM_EXIT_MSR_LOAD_ADDR",
    0x0000200a, // = "VM_ENTRY_MSR_LOAD_ADDR",
    0x0000200e, // = "PML_ADDRESS",
    0x00002010, // = "TSC_OFFSET",
    0x00002012, // = "VIRTUAL_APIC_PAGE_ADDR",
    0x00002014, // = "APIC_ACCESS_ADDR",
    0x00002016, // = "PI_DESC_ADDR",
    0x00002018, // = "VM_FUNCTION_CONTROL",
    0x0000201a, // = "EPT_POINTER",
    0x0000201c, // = "EOI_EXIT_BITMAP0",
    //EOI_EXIT_BITMAP(n) (EOI_EXIT_BITMAP0 + (n) * 2) /* n = 0...3 */
    0x00002024, // = "EPTP_LIST_ADDR",
    0x00002026, // = "VMREAD_BITMAP",
    0x00002028, // = "VMWRITE_BITMAP",
    0x0000202a, // = "VIRT_EXCEPTION_INFO",
    0x0000202c, // = "XSS_EXIT_BITMAP",
    0x00002032, // = "TSC_MULTIPLIER",
    0x00002400, // = "GUEST_PHYSICAL_ADDRESS",
    0x00002800, // = "VMCS_LINK_POINTER",
    0x00002802, // = "GUEST_IA32_DEBUGCTL",
    0x00002804, // = "GUEST_PAT",
    0x00002806, // = "GUEST_EFER",
    0x00002808, // = "GUEST_PERF_GLOBAL_CTRL",
    0x0000280a, // = "GUEST_PDPTE0",
    //GUEST_PDPTE(n) (GUEST_PDPTE0 + (n) * 2) /* n = 0...3 */
    0x00002812, // = "GUEST_BNDCFGS",
    0x00002c00, // = "HOST_PAT",
    0x00002c02, // = "HOST_EFER",
    0x00002c04, // = "HOST_PERF_GLOBAL_CTRL",
    0x00004000, // = "PIN_BASED_VM_EXEC_CONTROL",
    0x00004002, // = "CPU_BASED_VM_EXEC_CONTROL",
    0x00004004, // = "EXCEPTION_BITMAP",
    0x00004006, // = "PAGE_FAULT_ERROR_CODE_MASK",
    0x00004008, // = "PAGE_FAULT_ERROR_CODE_MATCH",
    0x0000400a, // = "CR3_TARGET_COUNT",
    0x0000400c, // = "VM_EXIT_CONTROLS",
    0x0000400e, // = "VM_EXIT_MSR_STORE_COUNT",
    0x00004010, // = "VM_EXIT_MSR_LOAD_COUNT",
    0x00004012, // = "VM_ENTRY_CONTROLS",
    0x00004014, // = "VM_ENTRY_MSR_LOAD_COUNT",
    0x00004016, // = "VM_ENTRY_INTR_INFO",
    0x00004018, // = "VM_ENTRY_EXCEPTION_ERROR_CODE",
    0x0000401a, // = "VM_ENTRY_INSTRUCTION_LEN",
    0x0000401c, // = "TPR_THRESHOLD",
    0x0000401e, // = "SECONDARY_VM_EXEC_CONTROL",
    0x00004020, // = "PLE_GAP",
    0x00004022, // = "PLE_WINDOW",
    0x00004400, // = "VM_INSTRUCTION_ERROR",
    0x00004402, // = "VM_EXIT_REASON",
    0x00004404, // = "VM_EXIT_INTR_INFO",
    0x00004406, // = "VM_EXIT_INTR_ERROR_CODE",
    0x00004408, // = "IDT_VECTORING_INFO",
    0x0000440a, // = "IDT_VECTORING_ERROR_CODE",
    0x0000440c, // = "VM_EXIT_INSTRUCTION_LEN",
    0x0000440e, // = "VMX_INSTRUCTION_INFO",
    //GUEST_SEG_LIMIT(sel) (GUEST_ES_LIMIT + (sel) * 2) /* ES ... GS */
    0x00004800, // = "GUEST_ES_LIMIT",
    0x00004802, // = "GUEST_CS_LIMIT",
    0x00004804, // = "GUEST_SS_LIMIT",
    0x00004806, // = "GUEST_DS_LIMIT",
    0x00004808, // = "GUEST_FS_LIMIT",
    0x0000480a, // = "GUEST_GS_LIMIT",
    0x0000480c, // = "GUEST_LDTR_LIMIT",
    0x0000480e, // = "GUEST_TR_LIMIT",
    0x00004810, // = "GUEST_GDTR_LIMIT",
    0x00004812, // = "GUEST_IDTR_LIMIT",
    //GUEST_SEG_AR_BYTES(sel) (GUEST_ES_AR_BYTES + (sel) * 2) /* ES ... GS */
    0x00004814, // = "GUEST_ES_AR_BYTES",
    0x00004816, // = "GUEST_CS_AR_BYTES",
    0x00004818, // = "GUEST_SS_AR_BYTES",
    0x0000481a, // = "GUEST_DS_AR_BYTES",
    0x0000481c, // = "GUEST_FS_AR_BYTES",
    0x0000481e, // = "GUEST_GS_AR_BYTES",
    0x00004820, // = "GUEST_LDTR_AR_BYTES",
    0x00004822, // = "GUEST_TR_AR_BYTES",
    0x00004824, // = "GUEST_INTERRUPTIBILITY_INFO",
    0x00004826, // = "GUEST_ACTIVITY_STATE",
    0x00004828, // = "GUEST_SMBASE",
    0x0000482a, // = "GUEST_SYSENTER_CS",
    0x0000482e, // = "GUEST_PREEMPTION_TIMER",
    0x00004c00, // = "HOST_SYSENTER_CS",
    0x00006000, // = "CR0_GUEST_HOST_MASK",
    0x00006002, // = "CR4_GUEST_HOST_MASK",
    0x00006004, // = "CR0_READ_SHADOW",
    0x00006006, // = "CR4_READ_SHADOW",
    0x00006008, // = "CR3_TARGET_VALUE0",
    //CR3_TARGET_VALUE(n) (CR3_TARGET_VALUE0 + (n) * 2) /* n < CR3_TARGET_COUNT */
    0x00006400, // = "EXIT_QUALIFICATION",
    0x0000640a, // = "GUEST_LINEAR_ADDRESS",
    0x00006800, // = "GUEST_CR0",
    0x00006802, // = "GUEST_CR3",
    0x00006804, // = "GUEST_CR4",
    //GUEST_SEG_BASE(sel) (GUEST_ES_BASE + (sel) * 2) /* ES ... GS */
    0x00006806, // = "GUEST_ES_BASE",
    0x00006808, // = "GUEST_CS_BASE",
    0x0000680a, // = "GUEST_SS_BASE",
    0x0000680c, // = "GUEST_DS_BASE",
    0x0000680e, // = "GUEST_FS_BASE",
    0x00006810, // = "GUEST_GS_BASE",
    0x00006812, // = "GUEST_LDTR_BASE",
    0x00006814, // = "GUEST_TR_BASE",
    0x00006816, // = "GUEST_GDTR_BASE",
    0x00006818, // = "GUEST_IDTR_BASE",
    0x0000681a, // = "GUEST_DR7",
    0x0000681c, // = "GUEST_RSP",
    0x0000681e, // = "GUEST_RIP",
    0x00006820, // = "GUEST_RFLAGS",
    0x00006822, // = "GUEST_PENDING_DBG_EXCEPTIONS",
    0x00006824, // = "GUEST_SYSENTER_ESP",
    0x00006826, // = "GUEST_SYSENTER_EIP",
    0x00006c00, // = "HOST_CR0",
    0x00006c02, // = "HOST_CR3",
    0x00006c04, // = "HOST_CR4",
    0x00006c06, // = "HOST_FS_BASE",
    0x00006c08, // = "HOST_GS_BASE",
    0x00006c0a, // = "HOST_TR_BASE",
    0x00006c0c, // = "HOST_GDTR_BASE",
    0x00006c0e, // = "HOST_IDTR_BASE",
    0x00006c10, // = "HOST_SYSENTER_ESP",
    0x00006c12, // = "HOST_SYSENTER_EIP",
    0x00006c14, // = "HOST_RSP",
    0x00006c16, // = "HOST_RIP"
    RAX,
    RBX ,
    RCX,
    RDX ,
    R8 ,
    R9 ,
    R10 ,
    R11 ,
    R12,
    R13 ,
    R14 ,
    R15 ,
    RBP,
    RSI,
    RDI



};

const uint64_t vmcs_fields_to_idx[] = {
	[0x00000000]=0, // = "VIRTUAL_PROCESSOR_ID",
	[0x00000002]=1, // = "POSTED_INTR_NOTIFICATION_VECTOR"]=,
	[0x00000004]=2, // = "EPTP_INDEX"]=,
	//GUEST_SEG_SELECTOR(sel) (GUEST_ES_SELECTOR + (sel) * 2) /* ES ... GS */
	[0x00000800]=3, // = "GUEST_ES_SELECTOR"]=,
	[0x00000802]=4, // = "GUEST_CS_SELECTOR"]=,
	[0x00000804]=5, // = "GUEST_SS_SELECTOR"]=,
    [0x00000806]=6, // = "GUEST_DS_SELECTOR"]=,
	[0x00000808]=7, // = "GUEST_FS_SELECTOR"]=,
    [0x0000080a]=8, // = "GUEST_GS_SELECTOR"]=,
    [0x0000080c]=9, // = "GUEST_LDTR_SELECTOR"]=,
    [0x0000080e]=10, // = "GUEST_TR_SELECTOR"]=,
    [0x00000810]=11, // = "GUEST_INTR_STATUS"]=,
    [0x00000812]=12, // = "GUEST_PML_INDEX"]=,
    [0x00000c00]=13, // = "HOST_ES_SELECTOR"]=,
    [0x00000c02]=14, // = "HOST_CS_SELECTOR"]=,
    [0x00000c04]=15, // = "HOST_SS_SELECTOR"]=,
    [0x00000c06]=16, // = "HOST_DS_SELECTOR"]=,
    [0x00000c08]=17, // = "HOST_FS_SELECTOR"]=,
    [0x00000c0a]=18, // = "HOST_GS_SELECTOR"]=,
    [0x00000c0c]=19, // = "HOST_TR_SELECTOR"]=,
    [0x00002000]=20, // = "IO_BITMAP_A"]=,       **
    [0x00002002]=21, // = "IO_BITMAP_B"]=,
    [0x00002004]=22, // = "MSR_BITMAP"]=,
    [0x00002006]=23, // = "VM_EXIT_MSR_STORE_ADDR"]=,
    [0x00002008]=24, // = "VM_EXIT_MSR_LOAD_ADDR"]=,
    [0x0000200a]=25, // = "VM_ENTRY_MSR_LOAD_ADDR"]=,
    [0x0000200e]=26, // = "PML_ADDRESS"]=,
    [0x00002010]=27, // = "TSC_OFFSET"]=,
    [0x00002012]=28, // = "VIRTUAL_APIC_PAGE_ADDR"]=,
    [0x00002014]=29, // = "APIC_ACCESS_ADDR"]=,
    [0x00002016]=30, // = "PI_DESC_ADDR"]=,
    [0x00002018]=31, // = "VM_FUNCTION_CONTROL"]=,
    [0x0000201a]=32, // = "EPT_POINTER"]=,
    [0x0000201c]=33, // = "EOI_EXIT_BITMAP0"]=,
    //EOI_EXIT_BITMAP(n) (EOI_EXIT_BITMAP0 + (n) * 2) /* n = 0...3 */
    [0x00002024]=34, // = "EPTP_LIST_ADDR"]=,
    [0x00002026]=35, // = "VMREAD_BITMAP"]=,
    [0x00002028]=36, // = "VMWRITE_BITMAP"]=,
    [0x0000202a]=37, // = "VIRT_EXCEPTION_INFO"]=,
    [0x0000202c]=38, // = "XSS_EXIT_BITMAP"]=,
    [0x00002032]=39, // = "TSC_MULTIPLIER"]=,
    [0x00002400]=40, // = "GUEST_PHYSICAL_ADDRESS"]=,
    [0x00002800]=41, // = "VMCS_LINK_POINTER"]=,
    [0x00002802]=42, // = "GUEST_IA32_DEBUGCTL"]=,
    [0x00002804]=43, // = "GUEST_PAT"]=,
    [0x00002806]=44, // = "GUEST_EFER"]=,
    [0x00002808]=45, // = "GUEST_PERF_GLOBAL_CTRL"]=,
    [0x0000280a]=46, // = "GUEST_PDPTE0"]=,
    //GUEST_PDPTE(n) (GUEST_PDPTE0 + (n) * 2) /* n = 0...3 */
    [0x00002812]=47, // = "GUEST_BNDCFGS"]=,
    [0x00002c00]=48, // = "HOST_PAT"]=,
    [0x00002c02]=49, // = "HOST_EFER"]=,
    [0x00002c04]=50, // = "HOST_PERF_GLOBAL_CTRL"]=,
    [0x00004000]=51, // = "PIN_BASED_VM_EXEC_CONTROL"]=,
    [0x00004002]=52, // = "CPU_BASED_VM_EXEC_CONTROL"]=,
    [0x00004004]=53, // = "EXCEPTION_BITMAP"]=,
    [0x00004006]=54, // = "PAGE_FAULT_ERROR_CODE_MASK"]=,
    [0x00004008]=55, // = "PAGE_FAULT_ERROR_CODE_MATCH"]=,
    [0x0000400a]=56, // = "CR3_TARGET_COUNT"]=,
    [0x0000400c]=57, // = "VM_EXIT_CONTROLS"]=,
    [0x0000400e]=58, // = "VM_EXIT_MSR_STORE_COUNT"]=,
    [0x00004010]=59, // = "VM_EXIT_MSR_LOAD_COUNT"]=,
    [0x00004012]=60, // = "VM_ENTRY_CONTROLS"]=,
    [0x00004014]=61, // = "VM_ENTRY_MSR_LOAD_COUNT"]=,
    [0x00004016]=62, // = "VM_ENTRY_INTR_INFO"]=,
    [0x00004018]=63, // = "VM_ENTRY_EXCEPTION_ERROR_CODE"]=, **
    [0x0000401a]=64, // = "VM_ENTRY_INSTRUCTION_LEN"]=,
    [0x0000401c]=65, // = "TPR_THRESHOLD"]=,
    [0x0000401e]=66, // = "SECONDARY_VM_EXEC_CONTROL"]=,
    [0x00004020]=67, // = "PLE_GAP"]=,
    [0x00004022]=68, // = "PLE_WINDOW"]=,
    [0x00004400]=69, // = "VM_INSTRUCTION_ERROR"]=,
    [0x00004402]=70, // = "VM_EXIT_REASON"]=,
    [0x00004404]=71, // = "VM_EXIT_INTR_INFO"]=,
    [0x00004406]=72, // = "VM_EXIT_INTR_ERROR_CODE"]=,
    [0x00004408]=73, // = "IDT_VECTORING_INFO"]=,
    [0x0000440a]=74, // = "IDT_VECTORING_ERROR_CODE"]=,
    [0x0000440c]=75, // = "VM_EXIT_INSTRUCTION_LEN"]=,
    [0x0000440e]=76, // = "VMX_INSTRUCTION_INFO"]=,
    //GUEST_SEG_LIMIT(sel) (GUEST_ES_LIMIT + (sel) * 2) /* ES ... GS */
    [0x00004800]=77, // = "GUEST_ES_LIMIT"]=,
    [0x00004802]=78, // = "GUEST_CS_LIMIT"]=,
    [0x00004804]=79, // = "GUEST_SS_LIMIT"]=,
    [0x00004806]=80, // = "GUEST_DS_LIMIT"]=,
    [0x00004808]=81, // = "GUEST_FS_LIMIT"]=,
    [0x0000480a]=82, // = "GUEST_GS_LIMIT"]=,
    [0x0000480c]=83, // = "GUEST_LDTR_LIMIT"]=,
    [0x0000480e]=84, // = "GUEST_TR_LIMIT"]=,
    [0x00004810]=85, // = "GUEST_GDTR_LIMIT"]=,
    [0x00004812]=86, // = "GUEST_IDTR_LIMIT"]=,
    //GUEST_SEG_AR_BYTES(sel) (GUEST_ES_AR_BYTES + (sel) * 2) /* ES ... GS */
    [0x00004814]=87, // = "GUEST_ES_AR_BYTES"]=,
    [0x00004816]=88, // = "GUEST_CS_AR_BYTES"]=,
    [0x00004818]=89, // = "GUEST_SS_AR_BYTES"]=,
    [0x0000481a]=90, // = "GUEST_DS_AR_BYTES"]=,
    [0x0000481c]=91, // = "GUEST_FS_AR_BYTES"]=,
    [0x0000481e]=92, // = "GUEST_GS_AR_BYTES"]=,
    [0x00004820]=93, // = "GUEST_LDTR_AR_BYTES"]=,
    [0x00004822]=94, // = "GUEST_TR_AR_BYTES"]=,
    [0x00004824]=95, // = "GUEST_INTERRUPTIBILITY_INFO"]=,
    [0x00004826]=96, // = "GUEST_ACTIVITY_STATE"]=,
    [0x00004828]=97, // = "GUEST_SMBASE"]=,
    [0x0000482a]=98, // = "GUEST_SYSENTER_CS"]=,
    [0x0000482e]=99, // = "GUEST_PREEMPTION_TIMER"]=,
    [0x00004c00]=100, // = "HOST_SYSENTER_CS"]=,
    [0x00006000]=101, // = "CR0_GUEST_HOST_MASK"]=,
    [0x00006002]=102, // = "CR4_GUEST_HOST_MASK"]=,
    [0x00006004]=103, // = "CR0_READ_SHADOW"]=,
    [0x00006006]=104, // = "CR4_READ_SHADOW"]=,
    [0x00006008]=105, // = "CR3_TARGET_VALUE0"]=,
    //CR3_TARGET_VALUE(n) (CR3_TARGET_VALUE0 + (n) * 2) /* n < CR3_TARGET_COUNT */
    [0x00006400]=106, // = "EXIT_QUALIFICATION"]=,
    [0x0000640a]=107, // = "GUEST_LINEAR_ADDRESS"]=,
    [0x00006800]=108, // = "GUEST_CR0"]=,
    [0x00006802]=109, // = "GUEST_CR3"]=,
    [0x00006804]=110, // = "GUEST_CR4"]=,
    //GUEST_SEG_BASE(sel) (GUEST_ES_BASE + (sel) * 2) /* ES ... GS */
    [0x00006806]=111, // = "GUEST_ES_BASE"]=,
    [0x00006808]=112, // = "GUEST_CS_BASE"]=,
    [0x0000680a]=113, // = "GUEST_SS_BASE"]=,
    [0x0000680c]=114, // = "GUEST_DS_BASE"]=,
    [0x0000680e]=115, // = "GUEST_FS_BASE"]=,
    [0x00006810]=116, // = "GUEST_GS_BASE"]=,
    [0x00006812]=117, // = "GUEST_LDTR_BASE"]=,
    [0x00006814]=118, // = "GUEST_TR_BASE"]=,
    [0x00006816]=119, // = "GUEST_GDTR_BASE"]=,
    [0x00006818]=120, // = "GUEST_IDTR_BASE"]=,
    [0x0000681a]=121, // = "GUEST_DR7"]=,
    [0x0000681c]=122, // = "GUEST_RSP"]=,
    [0x0000681e]=123, // = "GUEST_RIP"]=,
    [0x00006820]=124, // = "GUEST_RFLAGS"]=,
    [0x00006822]=125, // = "GUEST_PENDING_DBG_EXCEPTIONS"]=,
    [0x00006824]=126, // = "GUEST_SYSENTER_ESP"]=,
    [0x00006826]=127, // = "GUEST_SYSENTER_EIP"]=,
    [0x00006c00]=128, // = "HOST_CR0"]=,
    [0x00006c02]=129, // = "HOST_CR3"]=,
    [0x00006c04]=130, // = "HOST_CR4"]=,
    [0x00006c06]=131, // = "HOST_FS_BASE"]=,
    [0x00006c08]=132, // = "HOST_GS_BASE"]=,
    [0x00006c0a]=133, // = "HOST_TR_BASE"]=,
    [0x00006c0c]=134, // = "HOST_GDTR_BASE"]=,
    [0x00006c0e]=135, // = "HOST_IDTR_BASE"]=,
    [0x00006c10]=136, // = "HOST_SYSENTER_ESP"]=,
    [0x00006c12]=137, // = "HOST_SYSENTER_EIP"]=,
    [0x00006c14]=138, // = "HOST_RSP"]=,
    [0x00006c16]=139 // = "HOST_RIP"
    
    // /**aggiungere i registri**/
    // [RAX]=140,
    // [RBX]=141,
    // [RCX]=143,
    // [RDX]=144,
    // [R8]=145,
    // [R9]=146,
    // [R10]=147,
    // [R11]=148,
    // [R12]=149,
    // [R13]=150,
    // [R14]=151,
    // [R15]=152




};


#define MOD_HASH_VALUE 890


/* VMCS FIELDS names*/
const int VMCS_FIELDS_NEW[] = {
	[0x00000000] = 0,
	[0x00000002] = 1,
	[0x00000004] = 2,
	//GUEST_SEG_SELECTOR(sel) (GUEST_ES_SELECTOR + (sel) * 2) /* ES ... GS */
	[0x00000800] = 3,
	[0x00000802] = 4,
	[0x00000804] = 5
};

/* SEED TYPE NAMES*/
const char* SEED_TYPES[3] = {
    [0] = "VMWRITE",
    [1] = "VMREAD",
    [2] = "REG"
};


/* VMCS FIELDS names*/
const char* VMCS_FIELDS[27671] = {
	[0x00000000] = "VIRTUAL_PROCESSOR_ID",
	[0x00000002] = "POSTED_INTR_NOTIFICATION_VECTOR",
	[0x00000004] = "EPTP_INDEX",
	//GUEST_SEG_SELECTOR(sel) (GUEST_ES_SELECTOR + (sel) * 2) /* ES ... GS */
	[0x00000800] = "GUEST_ES_SELECTOR",
	[0x00000802] = "GUEST_CS_SELECTOR",
	[0x00000804] = "GUEST_SS_SELECTOR",
    [0x00000806] = "GUEST_DS_SELECTOR",
	[0x00000808] = "GUEST_FS_SELECTOR",
    [0x0000080a] = "GUEST_GS_SELECTOR",
    [0x0000080c] = "GUEST_LDTR_SELECTOR",
    [0x0000080e] = "GUEST_TR_SELECTOR",
    [0x00000810] = "GUEST_INTR_STATUS",
    [0x00000812] = "GUEST_PML_INDEX",
    [0x00000c00] = "HOST_ES_SELECTOR",
    [0x00000c02] = "HOST_CS_SELECTOR",
    [0x00000c04] = "HOST_SS_SELECTOR",
    [0x00000c06] = "HOST_DS_SELECTOR",
    [0x00000c08] = "HOST_FS_SELECTOR",
    [0x00000c0a] = "HOST_GS_SELECTOR",
    [0x00000c0c] = "HOST_TR_SELECTOR",
    [0x00002000] = "IO_BITMAP_A",
    [0x00002002] = "IO_BITMAP_B",
    [0x00002004] = "MSR_BITMAP",
    [0x00002006] = "VM_EXIT_MSR_STORE_ADDR",
    [0x00002008] = "VM_EXIT_MSR_LOAD_ADDR",
    [0x0000200a] = "VM_ENTRY_MSR_LOAD_ADDR",
    [0x0000200e] = "PML_ADDRESS",
    [0x00002010] = "TSC_OFFSET",
    [0x00002012] = "VIRTUAL_APIC_PAGE_ADDR",
    [0x00002014] = "APIC_ACCESS_ADDR",
    [0x00002016] = "PI_DESC_ADDR",
    [0x00002018] = "VM_FUNCTION_CONTROL",
    [0x0000201a] = "EPT_POINTER",
    [0x0000201c] = "EOI_EXIT_BITMAP0",
    //EOI_EXIT_BITMAP(n) (EOI_EXIT_BITMAP0 + (n) * 2) /* n = 0...3 */
    [0x00002024] = "EPTP_LIST_ADDR",
    [0x00002026] = "VMREAD_BITMAP",
    [0x00002028] = "VMWRITE_BITMAP",
    [0x0000202a] = "VIRT_EXCEPTION_INFO",
    [0x0000202c] = "XSS_EXIT_BITMAP",
    [0x00002032] = "TSC_MULTIPLIER",
    [0x00002400] = "GUEST_PHYSICAL_ADDRESS",
    [0x00002800] = "VMCS_LINK_POINTER",
    [0x00002802] = "GUEST_IA32_DEBUGCTL",
    [0x00002804] = "GUEST_PAT",
    [0x00002806] = "GUEST_EFER",
    [0x00002808] = "GUEST_PERF_GLOBAL_CTRL",
    [0x0000280a] = "GUEST_PDPTE0",
    //GUEST_PDPTE(n) (GUEST_PDPTE0 + (n) * 2) /* n = 0...3 */
    [0x00002812] = "GUEST_BNDCFGS",
    [0x00002c00] = "HOST_PAT",
    [0x00002c02] = "HOST_EFER",
    [0x00002c04] = "HOST_PERF_GLOBAL_CTRL",
    [0x00004000] = "PIN_BASED_VM_EXEC_CONTROL",
    [0x00004002] = "CPU_BASED_VM_EXEC_CONTROL",
    [0x00004004] = "EXCEPTION_BITMAP",
    [0x00004006] = "PAGE_FAULT_ERROR_CODE_MASK",
    [0x00004008] = "PAGE_FAULT_ERROR_CODE_MATCH",
    [0x0000400a] = "CR3_TARGET_COUNT",
    [0x0000400c] = "VM_EXIT_CONTROLS",
    [0x0000400e] = "VM_EXIT_MSR_STORE_COUNT",
    [0x00004010] = "VM_EXIT_MSR_LOAD_COUNT",
    [0x00004012] = "VM_ENTRY_CONTROLS",
    [0x00004014] = "VM_ENTRY_MSR_LOAD_COUNT",
    [0x00004016] = "VM_ENTRY_INTR_INFO",
    [0x00004018] = "VM_ENTRY_EXCEPTION_ERROR_CODE",
    [0x0000401a] = "VM_ENTRY_INSTRUCTION_LEN",
    [0x0000401c] = "TPR_THRESHOLD",
    [0x0000401e] = "SECONDARY_VM_EXEC_CONTROL",
    [0x00004020] = "PLE_GAP",
    [0x00004022] = "PLE_WINDOW",
    [0x00004400] = "VM_INSTRUCTION_ERROR",
    [0x00004402] = "VM_EXIT_REASON",
    [0x00004404] = "VM_EXIT_INTR_INFO",
    [0x00004406] = "VM_EXIT_INTR_ERROR_CODE",
    [0x00004408] = "IDT_VECTORING_INFO",
    [0x0000440a] = "IDT_VECTORING_ERROR_CODE",
    [0x0000440c] = "VM_EXIT_INSTRUCTION_LEN",
    [0x0000440e] = "VMX_INSTRUCTION_INFO",
    //GUEST_SEG_LIMIT(sel) (GUEST_ES_LIMIT + (sel) * 2) /* ES ... GS */
    [0x00004800] = "GUEST_ES_LIMIT",
    [0x00004802] = "GUEST_CS_LIMIT",
    [0x00004804] = "GUEST_SS_LIMIT",
    [0x00004806] = "GUEST_DS_LIMIT",
    [0x00004808] = "GUEST_FS_LIMIT",
    [0x0000480a] = "GUEST_GS_LIMIT",
    [0x0000480c] = "GUEST_LDTR_LIMIT",
    [0x0000480e] = "GUEST_TR_LIMIT",
    [0x00004810] = "GUEST_GDTR_LIMIT",
    [0x00004812] = "GUEST_IDTR_LIMIT",
    //GUEST_SEG_AR_BYTES(sel) (GUEST_ES_AR_BYTES + (sel) * 2) /* ES ... GS */
    [0x00004814] = "GUEST_ES_AR_BYTES",
    [0x00004816] = "GUEST_CS_AR_BYTES",
    [0x00004818] = "GUEST_SS_AR_BYTES",
    [0x0000481a] = "GUEST_DS_AR_BYTES",
    [0x0000481c] = "GUEST_FS_AR_BYTES",
    [0x0000481e] = "GUEST_GS_AR_BYTES",
    [0x00004820] = "GUEST_LDTR_AR_BYTES",
    [0x00004822] = "GUEST_TR_AR_BYTES",
    [0x00004824] = "GUEST_INTERRUPTIBILITY_INFO",
    [0x00004826] = "GUEST_ACTIVITY_STATE",
    [0x00004828] = "GUEST_SMBASE",
    [0x0000482a] = "GUEST_SYSENTER_CS",
    [0x0000482e] = "GUEST_PREEMPTION_TIMER",
    [0x00004c00] = "HOST_SYSENTER_CS",
    [0x00006000] = "CR0_GUEST_HOST_MASK",
    [0x00006002] = "CR4_GUEST_HOST_MASK",
    [0x00006004] = "CR0_READ_SHADOW",
    [0x00006006] = "CR4_READ_SHADOW",
    [0x00006008] = "CR3_TARGET_VALUE0",
    //CR3_TARGET_VALUE(n) (CR3_TARGET_VALUE0 + (n) * 2) /* n < CR3_TARGET_COUNT */
    [0x00006400] = "EXIT_QUALIFICATION",
    [0x0000640a] = "GUEST_LINEAR_ADDRESS",
    [0x00006800] = "GUEST_CR0",
    [0x00006802] = "GUEST_CR3",
    [0x00006804] = "GUEST_CR4",
    //GUEST_SEG_BASE(sel) (GUEST_ES_BASE + (sel) * 2) /* ES ... GS */
    [0x00006806] = "GUEST_ES_BASE",
    [0x00006808] = "GUEST_CS_BASE",
    [0x0000680a] = "GUEST_SS_BASE",
    [0x0000680c] = "GUEST_DS_BASE",
    [0x0000680e] = "GUEST_FS_BASE",
    [0x00006810] = "GUEST_GS_BASE",
    [0x00006812] = "GUEST_LDTR_BASE",
    [0x00006814] = "GUEST_TR_BASE",
    [0x00006816] = "GUEST_GDTR_BASE",
    [0x00006818] = "GUEST_IDTR_BASE",
    [0x0000681a] = "GUEST_DR7",
    [0x0000681c] = "GUEST_RSP",
    [0x0000681e] = "GUEST_RIP",
    [0x00006820] = "GUEST_RFLAGS",
    [0x00006822] = "GUEST_PENDING_DBG_EXCEPTIONS",
    [0x00006824] = "GUEST_SYSENTER_ESP",
    [0x00006826] = "GUEST_SYSENTER_EIP",
    [0x00006c00] = "HOST_CR0",
    [0x00006c02] = "HOST_CR3",
    [0x00006c04] = "HOST_CR4",
    [0x00006c06] = "HOST_FS_BASE",
    [0x00006c08] = "HOST_GS_BASE",
    [0x00006c0a] = "HOST_TR_BASE",
    [0x00006c0c] = "HOST_GDTR_BASE",
    [0x00006c0e] = "HOST_IDTR_BASE",
    [0x00006c10] = "HOST_SYSENTER_ESP",
    [0x00006c12] = "HOST_SYSENTER_EIP",
    [0x00006c14] = "HOST_RSP",
    [0x00006c16] = "HOST_RIP"
};


void get_seeds(char* input_filename, seeds_t* seeds_all, seeds_exit_reason_t* seeds_exit_reason){
    int max_buffer_size = MAX_BUFFER_SIZE;
    uint64_t *buffer = malloc(sizeof(uint64_t)*max_buffer_size);
    int size_buffer;
    FILE* fp;
    size_buffer=0;

    if((fp=fopen(input_filename, "r"))==NULL) {
        printf("Errore nell'apertura del file'");
        exit(1);
    }
    while((!feof(fp)) && size_buffer<max_buffer_size){
        fscanf(fp,"%"PRIx64"\n", &buffer[size_buffer]);
        size_buffer++;
    }
    
    if(size_buffer == max_buffer_size) printf("The size buffer is not enough\n");
    fclose(fp);
    if(raw_to_seeds(size_buffer, buffer, seeds_all, seeds_exit_reason,INJECTION_MODE)) printf("error:rawa_to_seeds\n");
    free(buffer);
}

#define MAX_NUMBER_EXIT 1000000

int raw_to_seeds(int size_buffer, uint64_t*buffer,seeds_t* seeds_all, seeds_exit_reason_t* seeds_exit_reason, int mode){
	int i,j,i_count;
	int size = size_buffer;
	unsigned long *sizes = malloc(sizeof(unsigned long)*MAX_NUMBER_EXIT);
	unsigned long *indexes = malloc(sizeof(unsigned long)*MAX_NUMBER_EXIT);
    
	//printf("Raw to seeds \n");
	if (size%3==1) {
        printf("Error: buffer size, size %d\n",size);
        return -1;
    }
	i_count=-1;	
	
	for (i=0;i<size;i=i+3){
		if(buffer[i]==0xFFFFFFFF && buffer[i+1]==0xFFFFFFFF && buffer[i+2]==0){
			i_count++;			//number of exit reasons
			indexes[i_count]=i;		//index of first row related to an exit		
			sizes[i_count]=0;		//number of reads/writes within an exit reason
		}
		else if(i_count>=0) { 
			sizes[i_count]++;
		}
		//printf("VMREAD FIELD(%"PRIx64"), VALUE(%"PRIx64"), type(%"PRIx64") \n", buffer[i], buffer[i+1], buffer[i+2]);
	}
	i_count++;
	
	printf("Number of exit reasons %d\n",i_count);	
	seeds_all->size = i_count; 
	seed_t* seeds_items = malloc(sizeof(seed_t)*i_count);
	
	for (i=0;i<i_count;i++){			//for each exit reason
		//printf("\nSEED #%d\n",i);
		int count = 0;
		int type = 0;
		seeds_items[i].id = i;
		bin_data_t* data = malloc(sizeof(bin_data_t)*sizes[i]);
		
		for (j=0;j<sizes[i]*3;j=j+3){				// foreach read/write within an exit reason
			if (mode == INJECTION_MODE) type = buffer[indexes[i]+j+5];
			else if (mode == PRINT_MODE) type = 999;
			if (type != 0){
				data[count].field=buffer[indexes[i]+j+3];
				data[count].value=buffer[indexes[i]+j+4];
				data[count].type=buffer[indexes[i]+j+5];
                if (data[count].field == 0x0004402) {
                    seeds_items[i].exit_reason = data[count].value;
                }
				//printf("VMREAD FIELD(%"PRIx64"), VALUE(%"PRIx64"), TYPE(%"PRIx64") \n",data[count].field,data[count].value, data[count].type);
				count++;
			}
		}
		seeds_items[i].seed_items=data;
		seeds_items[i].size=count;
	}
    //free(sizes);
    //free(indexes);

	seeds_all->seeds_items=seeds_items;
    //memset(seeds_exit_reason,0,sizeof(seeds_exit_reason));
    for (i=0;i<seeds_all->size;i++){
        int idx_exit_reason=seeds_all->seeds_items[i].exit_reason;
        seeds_exit_reason[idx_exit_reason].size++;
    }
    int count_exit_reasons[N_EXIT_REASONS];
    memset(count_exit_reasons,0, sizeof(count_exit_reasons));
    for (i=0;i<seeds_all->size;i++){
        int idx_exit_reason=seeds_all->seeds_items[i].exit_reason;
        if (count_exit_reasons[idx_exit_reason]==0){
            seeds_exit_reason[idx_exit_reason].idx_seeds = malloc(sizeof(int)*seeds_exit_reason[idx_exit_reason].size);
        }  
        seeds_exit_reason[idx_exit_reason].idx_seeds[count_exit_reasons[idx_exit_reason]]=i;
        count_exit_reasons[idx_exit_reason]++;
   }
	return 0;
}


// Random bit flipping of a field
unsigned long random_bit_flip(unsigned long in_value) {    
    unsigned long out_value = in_value;    
    int msb = 31;    
    int lsb = 0;        
    int pos = rand() % (msb + 1 - lsb) + lsb;    
    out_value ^= 1UL << pos;        
    return out_value;
}

void include_gp_regs(int8_t*fields_whitelist){
    int index=0;
    get_idx_mutation(RAX,index);
    fields_whitelist[index]=1;
    get_idx_mutation(RBX,index);
    fields_whitelist[index]=1;
    get_idx_mutation(RCX,index);
    fields_whitelist[index]=1;
    get_idx_mutation(RDX,index);
    fields_whitelist[index]=1;
    get_idx_mutation(R8,index);
    fields_whitelist[index]=1;
    get_idx_mutation(R9,index);
    fields_whitelist[index]=1;
    get_idx_mutation(R10,index);
    fields_whitelist[index]=1;
    get_idx_mutation(R11,index);
    fields_whitelist[index]=1;
    get_idx_mutation(R12,index);
    fields_whitelist[index]=1;
    get_idx_mutation(RDI,index);
    fields_whitelist[index]=1;
    get_idx_mutation(RSI,index);
    fields_whitelist[index]=1;
    get_idx_mutation(RBP,index);
    fields_whitelist[index]=1;
}

void not_include_gp_regs(int8_t*fields_whitelist){
    int index=0;
    memset(fields_whitelist,1, sizeof(fields_whitelist));
    get_idx_mutation(RAX,index);
    fields_whitelist[index]=0;
    get_idx_mutation(RBX,index);
    fields_whitelist[index]=0;
    get_idx_mutation(RCX,index);
    fields_whitelist[index]=0;
    get_idx_mutation(RDX,index);
    fields_whitelist[index]=0;
    get_idx_mutation(R8,index);
    fields_whitelist[index]=0;
    get_idx_mutation(R9,index);
    fields_whitelist[index]=0;
    get_idx_mutation(R10,index);
    fields_whitelist[index]=0;
    get_idx_mutation(R11,index);
    fields_whitelist[index]=0;
    get_idx_mutation(R12,index);
    fields_whitelist[index]=0;
    get_idx_mutation(RDI,index);
    fields_whitelist[index]=0;
    get_idx_mutation(RSI,index);
    fields_whitelist[index]=0;
    get_idx_mutation(RBP,index);
    fields_whitelist[index]=0;
}
