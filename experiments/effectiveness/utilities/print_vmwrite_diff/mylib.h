#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

/**Number of consecutive mutations (it must be a M )**/
#define M_CONS 1000
/**Number of mutations within a test case**/
#define M 10000
/** VMCS fields size Nvmcs**/
#define N_VMCS 140
/** gp regs fields size Nregs**/
#define N_REGS 10
/** N : the total rows**/
#define N 150


#define N_EXIT_REASONS 65
#define DEBUG_GUNFUZZER
#define MAX_BUFFER_SIZE 60000000

// Raw_to_seed MODES
#define INJECTION_MODE 0
#define PRINT_MODE 1
#define get_idx_mutation(field, index) index=hash_mutation_fields_to_idx[field%MOD_HASH_VALUE]

#define MUTATION_SIZE 155
#define MOD_HASH_VALUE 890


/* VMX EXIT REASONS ID */
#define EXIT_REASON_EXCEPTION_NMI       0
#define EXIT_REASON_EXTERNAL_INTERRUPT  1
#define EXIT_REASON_TRIPLE_FAULT        2
#define EXIT_REASON_INIT                3
#define EXIT_REASON_SIPI                4
#define EXIT_REASON_IO_SMI              5
#define EXIT_REASON_OTHER_SMI           6
#define EXIT_REASON_PENDING_VIRT_INTR   7
#define EXIT_REASON_PENDING_VIRT_NMI    8
#define EXIT_REASON_TASK_SWITCH         9
#define EXIT_REASON_CPUID               10
#define EXIT_REASON_GETSEC              11
#define EXIT_REASON_HLT                 12
#define EXIT_REASON_INVD                13
#define EXIT_REASON_INVLPG              14
#define EXIT_REASON_RDPMC               15
#define EXIT_REASON_RDTSC               16
#define EXIT_REASON_RSM                 17
#define EXIT_REASON_VMCALL              18
#define EXIT_REASON_VMCLEAR             19
#define EXIT_REASON_VMLAUNCH            20
#define EXIT_REASON_VMPTRLD             21
#define EXIT_REASON_VMPTRST             22
#define EXIT_REASON_VMREAD              23
#define EXIT_REASON_VMRESUME            24
#define EXIT_REASON_VMWRITE             25
#define EXIT_REASON_VMXOFF              26
#define EXIT_REASON_VMXON               27
#define EXIT_REASON_CR_ACCESS           28
#define EXIT_REASON_DR_ACCESS           29
#define EXIT_REASON_IO_INSTRUCTION      30
#define EXIT_REASON_MSR_READ            31
#define EXIT_REASON_MSR_WRITE           32
#define EXIT_REASON_INVALID_GUEST_STATE 33
#define EXIT_REASON_MSR_LOADING         34
#define EXIT_REASON_MWAIT_INSTRUCTION   36
#define EXIT_REASON_MONITOR_TRAP_FLAG   37
#define EXIT_REASON_MONITOR_INSTRUCTION 39
#define EXIT_REASON_PAUSE_INSTRUCTION   40
#define EXIT_REASON_MCE_DURING_VMENTRY  41
#define EXIT_REASON_TPR_BELOW_THRESHOLD 43
#define EXIT_REASON_APIC_ACCESS         44
#define EXIT_REASON_EOI_INDUCED         45
#define EXIT_REASON_ACCESS_GDTR_OR_IDTR 46
#define EXIT_REASON_ACCESS_LDTR_OR_TR   47
#define EXIT_REASON_EPT_VIOLATION       48
#define EXIT_REASON_EPT_MISCONFIG       49
#define EXIT_REASON_INVEPT              50
#define EXIT_REASON_RDTSCP              51
#define EXIT_REASON_VMX_PREEMPTION_TIMER_EXPIRED 52
#define EXIT_REASON_INVVPID             53
#define EXIT_REASON_WBINVD              54
#define EXIT_REASON_XSETBV              55
#define EXIT_REASON_APIC_WRITE          56
#define EXIT_REASON_INVPCID             58
#define EXIT_REASON_VMFUNC              59
#define EXIT_REASON_PML_FULL            62
#define EXIT_REASON_XSAVES              63
#define EXIT_REASON_XRSTORS             64

/* VMCS AREAS ID */
#define VMCS_CONTROL_AREA 				0
#define VMCS_VM_EXIT_INFORMATION_AREA 	1
#define GUEST_STATE_AREA				2
#define HOST_STATE_AREA					3


/**GP Regs coding**/
#define RAX 0x1FFFFFFF
#define RBX 0x2FFFFFFF
#define RCX 0x3FFFFFFF
#define RDX 0x4FFFFFFF
#define R8  0x5FFFFFFF
#define R9  0x6FFFFFFF
#define R10 0x7FFFFFFF
#define R11 0x8FFFFFFF
#define R12 0x9FFFFFFF
#define R13 0xaFFFFFFF
#define R14 0xbFFFFFFF
#define R15 0xcFFFFFFF
#define RBP 0xdFFFFFFF
#define RSI 0xeFFFFFFF
#define RDI 0x0FFFFFFF



/* VMCS field encodings. */
#define VMCS_HIGH(x) ((x) | 1)
enum vmcs_field {
    VIRTUAL_PROCESSOR_ID            = 0x00000000,
    POSTED_INTR_NOTIFICATION_VECTOR = 0x00000002,
    EPTP_INDEX                      = 0x00000004,
#define GUEST_SEG_SELECTOR(sel) (GUEST_ES_SELECTOR + (sel) * 2) /* ES ... GS */
    GUEST_ES_SELECTOR               = 0x00000800,
    GUEST_CS_SELECTOR               = 0x00000802,
    GUEST_SS_SELECTOR               = 0x00000804,
    GUEST_DS_SELECTOR               = 0x00000806,
    GUEST_FS_SELECTOR               = 0x00000808,
    GUEST_GS_SELECTOR               = 0x0000080a,
    GUEST_LDTR_SELECTOR             = 0x0000080c,
    GUEST_TR_SELECTOR               = 0x0000080e,
    GUEST_INTR_STATUS               = 0x00000810,
    GUEST_PML_INDEX                 = 0x00000812,
    HOST_ES_SELECTOR                = 0x00000c00,
    HOST_CS_SELECTOR                = 0x00000c02,
    HOST_SS_SELECTOR                = 0x00000c04,
    HOST_DS_SELECTOR                = 0x00000c06,
    HOST_FS_SELECTOR                = 0x00000c08,
    HOST_GS_SELECTOR                = 0x00000c0a,
    HOST_TR_SELECTOR                = 0x00000c0c,
    IO_BITMAP_A                     = 0x00002000,
    IO_BITMAP_B                     = 0x00002002,
    MSR_BITMAP                      = 0x00002004,
    VM_EXIT_MSR_STORE_ADDR          = 0x00002006,
    VM_EXIT_MSR_LOAD_ADDR           = 0x00002008,
    VM_ENTRY_MSR_LOAD_ADDR          = 0x0000200a,
    PML_ADDRESS                     = 0x0000200e,
    TSC_OFFSET                      = 0x00002010,
    VIRTUAL_APIC_PAGE_ADDR          = 0x00002012,
    APIC_ACCESS_ADDR                = 0x00002014,
    PI_DESC_ADDR                    = 0x00002016,
    VM_FUNCTION_CONTROL             = 0x00002018,
    EPT_POINTER                     = 0x0000201a,
    EOI_EXIT_BITMAP0                = 0x0000201c,
#define EOI_EXIT_BITMAP(n) (EOI_EXIT_BITMAP0 + (n) * 2) /* n = 0...3 */
    EPTP_LIST_ADDR                  = 0x00002024,
    VMREAD_BITMAP                   = 0x00002026,
    VMWRITE_BITMAP                  = 0x00002028,
    VIRT_EXCEPTION_INFO             = 0x0000202a,
    XSS_EXIT_BITMAP                 = 0x0000202c,
    TSC_MULTIPLIER                  = 0x00002032,
    GUEST_PHYSICAL_ADDRESS          = 0x00002400,
    VMCS_LINK_POINTER               = 0x00002800,
    GUEST_IA32_DEBUGCTL             = 0x00002802,
    GUEST_PAT                       = 0x00002804,
    GUEST_EFER                      = 0x00002806,
    GUEST_PERF_GLOBAL_CTRL          = 0x00002808,
    GUEST_PDPTE0                    = 0x0000280a,
#define GUEST_PDPTE(n) (GUEST_PDPTE0 + (n) * 2) /* n = 0...3 */
    GUEST_BNDCFGS                   = 0x00002812,
    HOST_PAT                        = 0x00002c00,
    HOST_EFER                       = 0x00002c02,
    HOST_PERF_GLOBAL_CTRL           = 0x00002c04,
    PIN_BASED_VM_EXEC_CONTROL       = 0x00004000,
    CPU_BASED_VM_EXEC_CONTROL       = 0x00004002,
    EXCEPTION_BITMAP                = 0x00004004,
    PAGE_FAULT_ERROR_CODE_MASK      = 0x00004006,
    PAGE_FAULT_ERROR_CODE_MATCH     = 0x00004008,
    CR3_TARGET_COUNT                = 0x0000400a,
    VM_EXIT_CONTROLS                = 0x0000400c,
    VM_EXIT_MSR_STORE_COUNT         = 0x0000400e,
    VM_EXIT_MSR_LOAD_COUNT          = 0x00004010,
    VM_ENTRY_CONTROLS               = 0x00004012,
    VM_ENTRY_MSR_LOAD_COUNT         = 0x00004014,
    VM_ENTRY_INTR_INFO              = 0x00004016,
    VM_ENTRY_EXCEPTION_ERROR_CODE   = 0x00004018,
    VM_ENTRY_INSTRUCTION_LEN        = 0x0000401a,
    TPR_THRESHOLD                   = 0x0000401c,
    SECONDARY_VM_EXEC_CONTROL       = 0x0000401e,
    PLE_GAP                         = 0x00004020,
    PLE_WINDOW                      = 0x00004022,
    VM_INSTRUCTION_ERROR            = 0x00004400,
    VM_EXIT_REASON                  = 0x00004402,
    VM_EXIT_INTR_INFO               = 0x00004404,
    VM_EXIT_INTR_ERROR_CODE         = 0x00004406,
    IDT_VECTORING_INFO              = 0x00004408,
    IDT_VECTORING_ERROR_CODE        = 0x0000440a,
    VM_EXIT_INSTRUCTION_LEN         = 0x0000440c,
    VMX_INSTRUCTION_INFO            = 0x0000440e,
#define GUEST_SEG_LIMIT(sel) (GUEST_ES_LIMIT + (sel) * 2) /* ES ... GS */
    GUEST_ES_LIMIT                  = 0x00004800,
    GUEST_CS_LIMIT                  = 0x00004802,
    GUEST_SS_LIMIT                  = 0x00004804,
    GUEST_DS_LIMIT                  = 0x00004806,
    GUEST_FS_LIMIT                  = 0x00004808,
    GUEST_GS_LIMIT                  = 0x0000480a,
    GUEST_LDTR_LIMIT                = 0x0000480c,
    GUEST_TR_LIMIT                  = 0x0000480e,
    GUEST_GDTR_LIMIT                = 0x00004810,
    GUEST_IDTR_LIMIT                = 0x00004812,
#define GUEST_SEG_AR_BYTES(sel) (GUEST_ES_AR_BYTES + (sel) * 2) /* ES ... GS */
    GUEST_ES_AR_BYTES               = 0x00004814,
    GUEST_CS_AR_BYTES               = 0x00004816,
    GUEST_SS_AR_BYTES               = 0x00004818,
    GUEST_DS_AR_BYTES               = 0x0000481a,
    GUEST_FS_AR_BYTES               = 0x0000481c,
    GUEST_GS_AR_BYTES               = 0x0000481e,
    GUEST_LDTR_AR_BYTES             = 0x00004820,
    GUEST_TR_AR_BYTES               = 0x00004822,
    GUEST_INTERRUPTIBILITY_INFO     = 0x00004824,
    GUEST_ACTIVITY_STATE            = 0x00004826,
    GUEST_SMBASE                    = 0x00004828,
    GUEST_SYSENTER_CS               = 0x0000482a,
    GUEST_PREEMPTION_TIMER          = 0x0000482e,
    HOST_SYSENTER_CS                = 0x00004c00,
    CR0_GUEST_HOST_MASK             = 0x00006000,
    CR4_GUEST_HOST_MASK             = 0x00006002,
    CR0_READ_SHADOW                 = 0x00006004,
    CR4_READ_SHADOW                 = 0x00006006,
    CR3_TARGET_VALUE0               = 0x00006008,
#define CR3_TARGET_VALUE(n) (CR3_TARGET_VALUE0 + (n) * 2) /* n < CR3_TARGET_COUNT */
    EXIT_QUALIFICATION              = 0x00006400,
    GUEST_LINEAR_ADDRESS            = 0x0000640a,
    GUEST_CR0                       = 0x00006800,
    GUEST_CR3                       = 0x00006802,
    GUEST_CR4                       = 0x00006804,
#define GUEST_SEG_BASE(sel) (GUEST_ES_BASE + (sel) * 2) /* ES ... GS */
    GUEST_ES_BASE                   = 0x00006806,
    GUEST_CS_BASE                   = 0x00006808,
    GUEST_SS_BASE                   = 0x0000680a,
    GUEST_DS_BASE                   = 0x0000680c,
    GUEST_FS_BASE                   = 0x0000680e,
    GUEST_GS_BASE                   = 0x00006810,
    GUEST_LDTR_BASE                 = 0x00006812,
    GUEST_TR_BASE                   = 0x00006814,
    GUEST_GDTR_BASE                 = 0x00006816,
    GUEST_IDTR_BASE                 = 0x00006818,
    GUEST_DR7                       = 0x0000681a,
    GUEST_RSP                       = 0x0000681c,
    GUEST_RIP                       = 0x0000681e,
    GUEST_RFLAGS                    = 0x00006820,
    GUEST_PENDING_DBG_EXCEPTIONS    = 0x00006822,
    GUEST_SYSENTER_ESP              = 0x00006824,
    GUEST_SYSENTER_EIP              = 0x00006826,
    HOST_CR0                        = 0x00006c00,
    HOST_CR3                        = 0x00006c02,
    HOST_CR4                        = 0x00006c04,
    HOST_FS_BASE                    = 0x00006c06,
    HOST_GS_BASE                    = 0x00006c08,
    HOST_TR_BASE                    = 0x00006c0a,
    HOST_GDTR_BASE                  = 0x00006c0c,
    HOST_IDTR_BASE                  = 0x00006c0e,
    HOST_SYSENTER_ESP               = 0x00006c10,
    HOST_SYSENTER_EIP               = 0x00006c12,
    HOST_RSP                        = 0x00006c14,
    HOST_RIP                        = 0x00006c16,
};
// /**Structs**/
// extern uint64_t vmcs_fields_vett[];
// extern const uint64_t vmcs_fields_to_idx[];
extern const uint64_t vmcs_fields_to_idx[];
extern uint64_t vmcs_fields_vett[];
extern const char* exit_reason_name[N_EXIT_REASONS];
extern const uint64_t vmcs_fields_to_idx[];
extern const int VMCS_FIELDS_NEW[];
extern const char* SEED_TYPES[3];
extern const char* VMCS_FIELDS[27671];
extern int8_t reason_blacklist[N_EXIT_REASONS];
extern int8_t fields_whitelist[MUTATION_SIZE];


struct bin_data {
    uint64_t  field;
    uint64_t  value;
    uint64_t  type;
};
typedef struct bin_data bin_data_t;

// List of items related to one vmexit (vmread access, vmwrite access, reg access)
struct seed {
    unsigned long exit_reason;
    unsigned long  id;
    unsigned long  size;
    bin_data_t* seed_items;
};
typedef struct seed seed_t;

// List of seeds related to multiple vmexit
struct seeds {
    unsigned long  size;
    seed_t* seeds_items ;
};
typedef struct seeds seeds_t;

struct seeds_exit_reason{
    unsigned long  size;
    int* idx_seeds;
};

typedef struct seeds_exit_reason seeds_exit_reason_t;


void get_seeds(seeds_t* seeds_all, seeds_exit_reason_t* seeds_exit_reason);
int raw_to_seeds(int size_buffer, uint64_t*buffer, seeds_t* seeds_all, seeds_exit_reason_t* seeds, int mode);
unsigned long random_bit_flip(unsigned long in_value) ;
void include_gp_regs(int8_t*fields_whitelist);
void print_index_regs();





#define hash_association { \
    [0]=0, \
    [2]=1, \
    [4]=2, \
    [268]=3, \
    [270]=4, \
    [272]=5, \
    [274]=6, \
    [276]=7, \
    [278]=8, \
    [280]=9, \
    [282]=10, \
    [284]=11, \
    [286]=12, \
    [402]=13, \
    [404]=14, \
    [406]=15, \
    [408]=16, \
    [410]=17, \
    [412]=18, \
    [414]=19, \
    [182]=20, \
    [184]=21, \
    [186]=22, \
    [188]=23, \
    [190]=24, \
    [192]=25, \
    [196]=26, \
    [198]=27, \
    [200]=28, \
    [202]=29, \
    [204]=30, \
    [206]=31, \
    [208]=32, \
    [210]=33, \
    [218]=34, \
    [220]=35, \
    [222]=36, \
    [224]=37, \
    [226]=38, \
    [232]=39, \
    [316]=40, \
    [450]=41, \
    [452]=42, \
    [454]=43, \
    [456]=44, \
    [458]=45, \
    [460]=46, \
    [468]=47, \
    [584]=48, \
    [586]=49, \
    [588]=50, \
    [364]=51, \
    [366]=52, \
    [368]=53, \
    [370]=54, \
    [372]=55, \
    [374]=56, \
    [376]=57, \
    [378]=58, \
    [380]=59, \
    [382]=60, \
    [384]=61, \
    [386]=62, \
    [388]=63, \
    [390]=64, \
    [392]=65, \
    [394]=66, \
    [396]=67, \
    [398]=68, \
    [498]=69, \
    [500]=70, \
    [502]=71, \
    [504]=72, \
    [506]=73, \
    [508]=74, \
    [510]=75, \
    [512]=76, \
    [632]=77, \
    [634]=78, \
    [636]=79, \
    [638]=80, \
    [640]=81, \
    [642]=82, \
    [644]=83, \
    [646]=84, \
    [648]=85, \
    [650]=86, \
    [652]=87, \
    [654]=88, \
    [656]=89, \
    [658]=90, \
    [660]=91, \
    [662]=92, \
    [664]=93, \
    [666]=94, \
    [668]=95, \
    [670]=96, \
    [672]=97, \
    [674]=98, \
    [678]=99, \
    [766]=100, \
    [546]=101, \
    [548]=102, \
    [550]=103, \
    [552]=104, \
    [554]=105, \
    [680]=106, \
    [690]=107, \
    [814]=108, \
    [816]=109, \
    [818]=110, \
    [820]=111, \
    [822]=112, \
    [824]=113, \
    [826]=114, \
    [828]=115, \
    [830]=116, \
    [832]=117, \
    [834]=118, \
    [836]=119, \
    [838]=120, \
    [840]=121, \
    [842]=122, \
    [844]=123, \
    [846]=124, \
    [848]=125, \
    [850]=126, \
    [852]=127, \
    [58]=128, \
    [60]=129, \
    [62]=130, \
    [64]=131, \
    [66]=132, \
    [68]=133, \
    [70]=134, \
    [72]=135, \
    [74]=136, \
    [76]=137, \
    [78]=138, \
    [80]=139, \
    [661]=140, \
    [547]=141, \
    [433]=142, \
    [319]=143, \
    [205]=144, \
    [91]=145, \
    [867]=146, \
    [753]=147, \
    [639]=148, \
    [525]=149, \
    [411]=150, \
    [297]=151, \
    [183]=152, \
    [69]=153, \
    [775]=154, \
};

static const uint16_t hash_mutation_fields_to_idx[] = hash_association
