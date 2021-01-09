/*
 * driver_vec.h
 *
 * Interface for exposure of BaS drivers to the OS
 *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 24.10.2013
 *      Author: Markus Fröschle
 */

#ifndef DRIVER_VEC_H
#define DRIVER_VEC_H

#include <stdint.h>
#include <stdlib.h>

#include "fb.h"

enum driver_type
{
    BLOCKDEV_DRIVER,
    CHARDEV_DRIVER,
    XHDI_DRIVER,
    MCD_DRIVER,
    VIDEO_DRIVER,
    PCI_DRIVER,
    MMU_DRIVER,
    PCI_NATIVE_DRIVER,
    END_OF_DRIVERS = 0xffffffffL,        /* marks end of driver list */
};

struct generic_driver_interface
{
    uint32_t (*init)(void);
    uint32_t (*read)(void *buf, size_t count);
    uint32_t (*write)(const void *buf, size_t count);
    uint32_t (*ioctl)(uint32_t request, ...);
};

struct dma_driver_interface
{
    int32_t version;
    int32_t magic;

    /* ... */
};

struct xhdi_driver_interface
{
    uint32_t (*xhdivec)(void);
};



struct framebuffer_driver_interface
{
    struct fb_info **framebuffer_info;  /* pointer to an fb_info struct (defined in include/fb.h) */
};

struct pci_bios_interface
{
    uint32_t subjar;
    uint32_t version;

    /* ... */
};

struct mmu_driver_interface
{
    uint32_t (*map_page_locked)(uint32_t address, uint32_t length, int asid);
    uint32_t (*unlock_page)(uint32_t address, uint32_t length, int asid);
    uint32_t (*report_locked_pages)(uint32_t *num_itlb, uint32_t *num_dtlb);
    uint32_t (*report_pagesize)(void);
};

struct pci_native_driver_interface_0_1
{
    uint32_t (*pci_read_config_longword)(int32_t handle, int offset);
    uint16_t (*pci_read_config_word)(int32_t handle, int offset);
    uint8_t (*pci_read_config_byte)(int32_t handle, int offset);

    int32_t (*pci_write_config_longword)(int32_t handle, int offset, uint32_t value);
    int32_t (*pci_write_config_word)(int32_t handle, int offset, uint16_t value);
    int32_t (*pci_write_config_byte)(int32_t handle, int offset, uint8_t value);
    int32_t (*pci_hook_interrupt)(int32_t handle, void *handler, void *parameter);
    int32_t (*pci_unhook_interrupt)(int32_t handle);

    struct pci_rd * (*pci_get_resource)(int32_t handle);
};

struct pci_native_driver_interface
{
    uint32_t (*pci_read_config_longword)(int32_t handle, int offset);
    uint16_t (*pci_read_config_word)(int32_t handle, int offset);
    uint8_t (*pci_read_config_byte)(int32_t handle, int offset);

    int32_t (*pci_write_config_longword)(int32_t handle, int offset, uint32_t value);
    int32_t (*pci_write_config_word)(int32_t handle, int offset, uint16_t value);
    int32_t (*pci_write_config_byte)(int32_t handle, int offset, uint8_t value);
    int32_t (*pci_hook_interrupt)(int32_t handle, void *handler, void *parameter);
    int32_t (*pci_unhook_interrupt)(int32_t handle);
    int32_t (*pci_find_device)(uint16_t device_id, uint16_t vendor_id, int index);
    int32_t (*pci_find_classcode)(uint32_t classcode, int index);
    struct pci_rd * (*pci_get_resource)(int32_t handle);
};

union interface
{
    struct generic_driver_interface *gdi;
    struct xhdi_driver_interface *xhdi;
    struct dma_driver_interface *dma;
    struct framebuffer_driver_interface *fb;
    struct pci_bios_interface *pci;
    struct mmu_driver_interface *mmu;
    struct pci_native_driver_interface_0_1 *pci_native_0_1;
    struct pci_native_driver_interface *pci_native;
};

struct generic_interface
{
    long type;
    char name[16];
    char description[64];
    long version;
    long revision;
    union interface interface;
};

struct driver_table
{
    uint32_t bas_version;
    uint32_t bas_revision;
    void (*remove_handler)(void);           /* calling this will disable the BaS' hook into trap #0 */
    struct generic_interface *interfaces;
};


#endif /* _DRIVER_VEC_H_ */
