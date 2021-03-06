/* **********************************************************
 * Copyright (c) 2016 Google, Inc.  All rights reserved.
 * **********************************************************/

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Google, Inc. nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL VMWARE, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include <assert.h>
#include <fstream>
#include <map>
#include "file_reader.h"
#include "../common/memref.h"
#include "../common/utils.h"

#ifdef VERBOSE
# include <iostream>
#endif

file_reader_t::file_reader_t()
{
    /* Empty. */
}

file_reader_t::file_reader_t(const char *file_name) :
    fstream(file_name, std::ifstream::binary)
{
    /* Empty. */
}

bool
file_reader_t::init()
{
    at_eof = false;
    if (!fstream)
        return false;
    trace_entry_t *first_entry = read_next_entry();
    if (first_entry == NULL)
        return false;
    if (first_entry->type != TRACE_TYPE_HEADER ||
        first_entry->addr != TRACE_ENTRY_VERSION) {
        ERRMSG("missing header or version mismatch\n");
        return false;
    }
    ++*this;
    return true;
}

file_reader_t::~file_reader_t()
{
    fstream.close();
}

trace_entry_t *
file_reader_t::read_next_entry()
{
    if (!fstream.read((char*)&entry_copy, sizeof(entry_copy)))
        return NULL;
    return &entry_copy;
}

bool
file_reader_t::is_complete()
{
    if (!fstream)
        return false;
    bool res = false;
    std::streampos pos = fstream.tellg();
    fstream.seekg(-(int)sizeof(trace_entry_t), fstream.end);
    // Avoid reaching eof b/c we can't seek away from it.
    if (fstream.read((char*)&entry_copy.type, sizeof(entry_copy.type)) &&
        entry_copy.type == TRACE_TYPE_FOOTER)
        res = true;
    fstream.seekg(pos);
    return res;
}
