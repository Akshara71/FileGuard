/*
 * main.c - FileGuard command-line interface (Core Module-I)
 *
 * Commands implemented so far:
 *   init    create the virtual disks
 *   status  show the state of every virtual disk
 */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "disk.h"

static void print_usage(const char *prog)
{
    printf("FileGuard - user-space RAID-based virtual storage system\n\n");
    printf("Usage:\n");
    printf("  %s init     Create %d virtual disks in %s/\n",
           prog, NUM_DISKS, DISK_DIR);
    printf("  %s status   Show the status of every virtual disk\n", prog);
}

static int cmd_init(void)
{
    char path[128];
    int i;

    printf("Initializing %d virtual disks...\n", NUM_DISKS);
    for (i = 0; i < NUM_DISKS; i++) {
        if (disk_create(i) != DISK_OK) {
            fprintf(stderr, "Failed to create disk %d\n", i + 1);
            return 1;
        }
        disk_path(i, path, sizeof(path));
        printf("  Created %s (%d data blocks of %d bytes)\n",
               path, DISK_DATA_BLOCKS, BLOCK_SIZE);
    }
    printf("Done. Run './fileguard status' to check the disks.\n");
    return 0;
}

static int cmd_status(void)
{
    char path[128];
    char label[16];
    const char *state;
    struct stat st;
    DiskHeader hdr;
    long long size;
    unsigned blocks;
    int online = 0;
    int rc, i;

    printf("FileGuard - Virtual Disk Status\n");
    printf("--------------------------------------------------------------\n");
    printf("%-8s %-18s %12s %8s  %s\n",
           "Disk", "File", "Size(bytes)", "Blocks", "State");

    for (i = 0; i < NUM_DISKS; i++) {
        disk_path(i, path, sizeof(path));
        snprintf(label, sizeof(label), "Disk %d", i + 1);

        /* stat() asks the kernel for file information, including size */
        size = (stat(path, &st) == 0) ? (long long)st.st_size : 0;

        rc = disk_read_header(i, &hdr);
        blocks = 0;
        if (rc == DISK_OK) {
            state = "ONLINE";
            blocks = hdr.data_blocks;
            online++;
        } else if (rc == DISK_ERR_MISSING) {
            state = "MISSING";
        } else if (rc == DISK_ERR_INVALID) {
            state = "CORRUPT";
        } else {
            state = "ERROR";
        }

        printf("%-8s %-18s %12lld %8u  %s\n",
               label, path, size, blocks, state);
    }

    printf("--------------------------------------------------------------\n");
    printf("Disks online: %d of %d\n", online, NUM_DISKS);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "init") == 0)
        return cmd_init();
    if (strcmp(argv[1], "status") == 0)
        return cmd_status();

    fprintf(stderr, "Unknown command: %s\n\n", argv[1]);
    print_usage(argv[0]);
    return 1;
}
