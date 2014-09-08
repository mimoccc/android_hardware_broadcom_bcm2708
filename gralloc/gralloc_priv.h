/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GRALLOC_PRIV_H_
#define GRALLOC_PRIV_H_

#include <stdint.h>
#include <limits.h>
#include <sys/cdefs.h>
#include <hardware/gralloc.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <gralloc_brcm.h>
#include <utils/Log.h>
#include <cutils/native_handle.h>

#include <linux/fb.h>

/*****************************************************************************/


enum {
    /* Gralloc perform enums
    */
    GRALLOC_MODULE_PERFORM_GET_DISPMANX_HANDLE = 1,
    GRALLOC_MODULE_PERFORM_GET_STRIDE,
    GRALLOC_MODULE_PERFORM_GET_CUSTOM_STRIDE_FROM_HANDLE,
};


struct private_module_t;



/*****************************************************************************/

#ifdef __cplusplus
struct private_handle_t;
struct private_handle_t : public native_handle {
#else
typedef struct private_handle_t private_handle_t;
struct private_handle_t {
    struct native_handle nativeHandle;
#endif
 enum {
        PRIV_FLAGS_FRAMEBUFFER = 0x00000001
    };

    enum {
        READ_LOCK = 0x00000001,
        WRITE_LOCK = 0x00000002
    };

    // file-descriptors
    int     fd;
    // ints
    int     magic;
    int     flags;
       int     lock;
    int     size;
    int     offset;
        int     format;
        int     width;
        int     height;
    // FIXME: the attributes below should be out-of-line
    int     base;
    
    int     pid;
    struct gralloc_private_handle_t* brcm_handle;
#ifdef __cplusplus
    static const int sNumInts = 6;
    static const int sNumFds = 1;
    static const int sMagic = 0x3141592;

    private_handle_t(int fd, int size, int flags, int format,int width, int height) :
        fd(fd), magic(sMagic), flags(flags), size(size),offset(0),format(format), width(width), height(height),base(0), pid(getpid())
    {
        version = sizeof(native_handle);
        numInts = sNumInts;
        numFds = sNumFds;
    }
    ~private_handle_t() {
        magic = 0;
    }

    static int validate(const native_handle* h) {
        const private_handle_t* hnd = (const private_handle_t*)h;
        if (!h || h->version != sizeof(native_handle) ||
                h->numInts != sNumInts || h->numFds != sNumFds ||
                hnd->magic != sMagic)
        {
            ALOGE("invalid gralloc handle (at %p)", h);
            return -EINVAL;
        }
        return 0;
    }
#endif
};

struct private_module_t {
    gralloc_module_t base;

    private_handle_t* framebuffer;
    uint32_t fbFormat;
    uint32_t flags;
    uint32_t numBuffers;
    uint32_t bufferMask;
    pthread_mutex_t lock;
    buffer_handle_t currentBuffer;
    int     format;
    struct fb_var_screeninfo info;
    struct fb_fix_screeninfo finfo;
    float xdpi;
    float ydpi;
    float fps;
};
#endif /* GRALLOC_PRIV_H_ */
