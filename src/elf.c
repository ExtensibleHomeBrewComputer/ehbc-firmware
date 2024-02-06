#include <ehbc/elf.h>
#include <ehbc/string.h>

static memberof(ELFObject, shdr32)* find_shdr(ELFObject* self, const char* name)
{
    for (unsigned int i = 0; i < self->header->shnum; i++) {
        if (i == self->header->shstrndx) {
            continue;
        } else if (strncmp((const char*)self->shstrtab + self->sheader[i].name, name, 512) == 0) {
            return &self->sheader[i];
        }
    }
    return NULL;
}

int memberof(ELFObject, construct)(ELFObject* self, file_t* file)
{
    self->header = mem_alloc(sizeof(*self->header));
    self->pheader = NULL;
    self->sheader = NULL;
    self->shstrtab = NULL;
    self->file = file;

    file->fstrait->seek_file(file->fs, file, 0, FS_SEEK_SET);
    file->fstrait->read_file(file->fs, file, self->header, sizeof(*self->header), 1);

    if (strncmp((const char*)self->header->ident, "\x7F" "ELF", 4) != 0) {
        return -1;
    }

    if (self->header->ident[4] != EI_CLASS_ELF32) {
        return -1;
    }

    if (self->header->ident[5] != EI_DATA_BIGENDIAN) {
        return -1;
    }

    if (self->header->type != ET_EXEC) {
        return -1;
    }

    if (self->header->machine != EM_M68K) {
        return -1;
    }

    uint32_t phsize = self->header->phentsize * self->header->phnum;
    uint32_t shsize = self->header->shentsize * self->header->shnum;

    self->pheader = mem_alloc(phsize);
    self->sheader = mem_alloc(shsize);

    file->fstrait->seek_file(file->fs, file, self->header->phoff, FS_SEEK_SET);
    file->fstrait->read_file(file->fs, file, self->pheader, phsize, 1);

    file->fstrait->seek_file(file->fs, file, self->header->shoff, FS_SEEK_SET);
    file->fstrait->read_file(file->fs, file, self->sheader, shsize, 1);

    for (unsigned int i = 0; i < self->header->phnum; i++) {
        printf("PHDR %u: type=%08lX, vaddr=%08lX, paddr=%08lX, size=%08lX\r\n", i, self->pheader[i].type, self->pheader[i].vaddr, self->pheader[i].paddr, self->pheader[i].memsz);
    }

    self->shstrtab = mem_alloc(self->sheader[self->header->shstrndx].size);
    self->shstrtabsz = self->sheader[self->header->shstrndx].size;
    file->fstrait->seek_file(file->fs, file, self->sheader[self->header->shstrndx].offset, FS_SEEK_SET);
    file->fstrait->read_file(file->fs, file, self->shstrtab, self->shstrtabsz, 1);

    return 0;
}

int memberof(ELFObject, destruct)(ELFObject* self)
{
    mem_free(self->header);
    mem_free(self->pheader);
    mem_free(self->sheader);
    mem_free(self->shstrtab);
    return 0;
}

int memberof(ELFObject, get_section_data)(ELFObject* self, const char* name, void* buf, size32_t len)
{
    const memberof(ELFObject, shdr32)* shdr = find_shdr(self, name);
    if (shdr != NULL) {
        if (shdr->offset < len) {
            len = shdr->offset;
        }
        self->file->fstrait->seek_file(self->file->fs, self->file, shdr->offset, FS_SEEK_SET);
        self->file->fstrait->read_file(self->file->fs, self->file, buf, len, 1);
        return 0;
    } else {
        return -1;
    }
}

int memberof(ELFObject, get_section_header)(ELFObject* self, const char* name, memberof(ELFObject, shdr32)* shdr_buf)
{
    const memberof(ELFObject, shdr32)* shdr = find_shdr(self, name);
    if (shdr != NULL) {
        memcpy(shdr_buf, shdr, sizeof(*shdr_buf));
        return 0;
    } else {
        return -1;
    }
}
