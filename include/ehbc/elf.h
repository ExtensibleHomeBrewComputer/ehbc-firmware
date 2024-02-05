#ifndef _EHBC_ELF_H__
#define _EHBC_ELF_H__

#include <ehbc/fs/filesystem.h>

#define EI_NIDENT 16

#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4
#define EI_DATA         5
#define EI_VERSION      6
#define EI_OSABI        7
#define EI_ABIVERSION   8

#define EI_CLASS_NONE   0
#define EI_CLASS_ELF32  1
#define EI_CLASS_ELF64  2

#define EI_DATA_NONE            0
#define EI_DATA_LITTLEENDIAN    1
#define EI_DATA_BIGENDIAN       2

#define EI_OSABI_SYSTEMV    0
#define EI_OSABI_HPUX       1
#define EI_OSABI_NETBSD     2
#define EI_OSABI_LINUX      3
#define EI_OSABI_GNUHURD    4
#define EI_OSABI_86OPEN     5
#define EI_OSABI_SOLARIS    6
#define EI_OSABI_AIX        7
#define EI_OSABI_IRIX       8
#define EI_OSABI_FREEBSD    9
#define EI_OSABI_TRU64      10
#define EI_OSABI_MODESTO    11
#define EI_OSABI_OPENBSD    12
#define EI_OSABI_OPENVMS    13
#define EI_OSABI_NSK        14
#define EI_OSABI_AROS       15
#define EI_OSABI_FENIXOS    16
#define EI_OSABI_CLOUDABI   17
#define EI_OSABI_OPENVOS    18
#define EI_OSABI_ARM_AEABI  64
#define EI_OSABI_ARM        97
#define EI_OSABI_STANDALONE 255

#define ET_NONE             0
#define ET_RELOC            1
#define ET_EXEC             2
#define ET_DYNAMIC          3
#define ET_CORE             4
#define ET_LOOS             0xFE00
#define ET_HIOS             0xFEFF
#define ET_LOPROC           0xFF00
#define ET_HIPROC           0xFFFF

#define EM_NONE             0
#define EM_M68K             4

#define PHT_NULL            0
#define PHT_LOAD            1
#define PHT_DYNAMIC         2
#define PHT_INTERP          3
#define PHT_NOTE            4
#define PHT_SHLIB           5
#define PHT_PHDR            6
#define PHT_TLS             7
#define PHT_LOOS            0x60000000
#define PHT_HIOS            0x6FFFFFFF
#define PHT_LOPROC          0x70000000
#define PHT_HIPROC          0x7FFFFFFF

#define PHF_EXEC            1
#define PHF_WRITE           2
#define PHF_READ            4

#define SHT_NULL            0
#define SHT_PROGBITS        1
#define SHT_SYMTAB          2
#define SHT_STRTAB          3
#define SHT_RELA            4
#define SHT_HASH            5
#define SHT_DYNAMIC         6
#define SHT_NOTE            7
#define SHT_NOBITS          8
#define SHT_REL             9
#define SHT_SHLIB           10
#define SHT_DYNSYM          11
#define SHT_INIT_ARRAY      14
#define SHT_FINI_ARRAY      15
#define SHT_PREINIT_ARRAY   16
#define SHT_GROUP           17
#define SHT_SYMTAB_SHNDX    18
#define SHT_NUM             19
#define SHT_LOOS            0x60000000
#define SHT_HIOS            0x6FFFFFFF
#define SHT_LOPROC          0x70000000
#define SHT_HIPROC          0x7FFFFFFF
#define SHT_LOUSER          0x80000000
#define SHT_HIUSER          0x8FFFFFFF

#define SHF_WRITE               0x00000001
#define SHF_ALLOC               0x00000002
#define SHF_EXECINSTR           0x00000004
#define SHF_MERGE               0x00000010
#define SHF_STRINGS             0x00000020
#define SHF_INFO_LINK           0x00000040
#define SHF_LINK_ORDER          0x00000080
#define SHF_OS_NONCONFORMING    0x00000100
#define SHF_GROUP               0x00000200
#define SHF_TLS                 0x00000400
#define SHF_COMPRESSED          0x00000800
#define SHF_MASKOS              0x0FF00000
#define SHF_MASKPROC            0xF0000000


typedef uint32_t elf32_addr_t;
typedef uint32_t elf32_offset_t;

typedef struct {
    uint8_t     ident[EI_NIDENT];
    uint16_t    type;
    uint16_t    machine;
    uint32_t    version;
    uint32_t    entry;
    uint32_t    phoff;
    uint32_t    shoff;
    uint32_t    flags;
    uint16_t    ehsize;
    uint16_t    phentsize;
    uint16_t    phnum;
    uint16_t    shentsize;
    uint16_t    shnum;
    uint16_t    shstrndx;
} memberof(ELFObject, header32);

typedef struct {
    uint32_t    type;
    uint32_t    offset;
    uint32_t    vaddr;
    uint32_t    paddr;
    uint32_t    filesz;
    uint32_t    memsz;
    uint32_t    flags;
    uint32_t    align;
} memberof(ELFObject, phdr32);

typedef struct {
    uint32_t    name;
    uint32_t    type;
    uint32_t    flags;
    uint32_t    addr;
    uint32_t    offset;
    uint32_t    size;
    uint32_t    link;
    uint32_t    info;
    uint32_t    addralign;
    uint32_t    entsize;
} memberof(ELFObject, shdr32);

typedef struct {
    uint32_t    name;
    uint32_t    value;
    uint32_t    size;
    uint8_t     info;
    uint8_t     other;
    uint8_t     shndx;
} memberof(ELFObject, symbol32);

typedef struct {
    uint32_t    offset;
    uint32_t    info;
} memberof(ELFObject, reloc32);

typedef struct {
    file_t* file;
    memberof(ELFObject, header32)* header;
    memberof(ELFObject, phdr32)* pheader;
    memberof(ELFObject, shdr32)* sheader;
    void* shstrtab;
    size32_t shstrtabsz;
} ELFObject;

int memberof(ELFObject, construct)(ELFObject* self, file_t* file);
int memberof(ELFObject, destruct)(ELFObject* self);
int memberof(ELFObject, get_section_data)(ELFObject* self, const char* name, void* buf, size32_t len);
int memberof(ELFObject, get_section_header)(ELFObject* self, const char* name, memberof(ELFObject, shdr32)* shdr_buf);

static const struct {
    int (*construct)(ELFObject* self, file_t* file);
    int (*destruct)(ELFObject* self);
    int (*get_section_data)(ELFObject* self, const char* name, void* buf, size32_t len);
    int (*get_section_header)(ELFObject* self, const char* name, memberof(ELFObject, shdr32)* shdr_buf);
} ftableof(ELFObject) = {
    .construct = memberof(ELFObject, construct),
    .destruct = memberof(ELFObject, destruct),
    .get_section_data = memberof(ELFObject, get_section_data),
    .get_section_header = memberof(ELFObject, get_section_header),
};

#endif  // _EHBC_ELF_H__
