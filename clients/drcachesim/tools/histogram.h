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

/* histogram: a memory trace histogram analysis tool.
 */

#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_ 1

#include <map>
#include <string>
#include "../analysis_tool.h"
#include "../common/memref.h"

class histogram_t : public analysis_tool_t
{
 public:
    histogram_t();
    virtual ~histogram_t();
    virtual bool process_memref(const memref_t &memref);
    virtual bool print_results();

 protected:
    /* FIXME i#2020: use unsorted_map (C++11) for faster lookup */
    std::map<addr_t, uint64_t> icache_map;
    std::map<addr_t, uint64_t> dcache_map;

    size_t line_size;
    size_t line_size_bits;
    size_t report_top;  /* most accessed lines */
    static const std::string TOOL_NAME;
};

#endif /* _HISTOGRAM_H_ */
