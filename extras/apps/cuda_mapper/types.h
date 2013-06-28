// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2013 NVIDIA Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of NVIDIA Corporation nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Enrico Siragusa <enrico.siragusa@fu-berlin.de>
// ==========================================================================
// This file contains type definitions.
// ==========================================================================

#ifndef SEQAN_EXTRAS_CUDAMAPPER_TYPES_H_
#define SEQAN_EXTRAS_CUDAMAPPER_TYPES_H_

// ============================================================================
// Prerequisites
// ============================================================================

#include <seqan/basic_extras.h>
#include <seqan/sequence_extras.h>
#include <seqan/index_extras.h>

using namespace seqan;

// ============================================================================
// Tags
// ============================================================================

// ----------------------------------------------------------------------------
// Execution Space Tags
// ----------------------------------------------------------------------------

struct CPU_;
typedef Tag<CPU_>     CPU;

#ifdef SEQAN_CUDA
struct GPU_;
typedef Tag<GPU_>     GPU;
#endif

// ============================================================================
// Global Types
// ============================================================================

// ----------------------------------------------------------------------------
// Fragment Store Configuration
// ----------------------------------------------------------------------------

struct CUDAStoreConfig
{
    typedef String<Dna>             TReadSeq;
    typedef String<Dna>             TContigSeq;
    typedef Owner<ConcatDirect<> >  TContigSpec;

    typedef double                  TMean;
    typedef double                  TStd;
    typedef signed char             TMappingQuality;

    typedef void                    TReadStoreElementSpec;
    typedef Owner<ConcatDirect<> >  TReadSeqStoreSpec;
    typedef Alloc<>                 TReadNameSpec;
    typedef Owner<ConcatDirect<> >  TReadNameStoreSpec;
    typedef void                    TMatePairStoreElementSpec;
    typedef void                    TLibraryStoreElementSpec;
    typedef void                    TContigStoreElementSpec;
    typedef void                    TContigFileSpec;
    typedef void                    TAlignedReadStoreElementSpec;
    typedef Owner<ConcatDirect<> >  TAlignedReadTagStoreSpec;
    typedef void                    TAnnotationStoreElementSpec;
};

// ============================================================================
// Other Store Types
// ============================================================================

typedef StringSet<CUDAStoreConfig::TContigSeq, CUDAStoreConfig::TContigSpec>        TContigs;
typedef StringSet<CUDAStoreConfig::TReadSeq, CUDAStoreConfig::TReadSeqStoreSpec>    TReadSeqs;

// ----------------------------------------------------------------------------
// ReadSeqs Size
// ----------------------------------------------------------------------------
// TODO(esiragusa): Check if register usage decreases.

namespace seqan {
template <>
struct Size<TReadSeqs>
{
    typedef __uint32 Type;
};

#ifdef __CUDACC__
template <>
struct Size<typename Device<TReadSeqs>::Type>
{
    typedef __uint32 Type;
};

template <>
struct Size<typename View<typename Device<TReadSeqs>::Type>::Type>
{
    typedef __uint32 Type;
};
#endif
}

// ----------------------------------------------------------------------------
// ContainerView Size
// ----------------------------------------------------------------------------
// TODO(esiragusa): Check if register usage decreases.

namespace seqan {
#ifdef __CUDACC__
template <typename TValue, typename TAlloc, typename TViewSpec>
struct Size<ContainerView<thrust::device_vector<TValue, TAlloc>, TViewSpec> >
{
    typedef __uint32 Type;
};

template <typename TValue, typename TAlloc, typename TViewSpec, typename TSSetSpec>
struct Size<StringSet<ContainerView<thrust::device_vector<TValue, TAlloc>, TViewSpec>, TSSetSpec> >
{
    typedef __uint32 Type;
};
#endif
}

// ============================================================================
// Index Types
// ============================================================================

// ----------------------------------------------------------------------------
// Suffix Array Value Type
// ----------------------------------------------------------------------------

namespace seqan {
template <>
struct SAValue<TContigs>
{
    typedef Pair<__uint8, __uint32, Pack> Type;
};
}

// ----------------------------------------------------------------------------
// FM Index Fibres
// ----------------------------------------------------------------------------

typedef FMIndex<>                           TGenomeIndexSpec;
typedef Index<TContigs, TGenomeIndexSpec>   TGenomeIndex;

// ----------------------------------------------------------------------------
// FM Index Size
// ----------------------------------------------------------------------------

namespace seqan {
template <>
struct Size<TGenomeIndex>
{
    typedef __uint32 Type;
};

#ifdef __CUDACC__
template <>
struct Size<typename Device<TGenomeIndex>::Type>
{
    typedef __uint32 Type;
};

template <>
struct Size<typename View<typename Device<TGenomeIndex>::Type>::Type>
{
    typedef __uint32 Type;
};
#endif

// ----------------------------------------------------------------------------
// Rank Dictionary Size
// ----------------------------------------------------------------------------

template <typename TSpec>
struct Size<RankDictionary<TwoLevels<Dna, TSpec> > >
{
    typedef __uint32 Type;
};

template <typename TSpec>
struct Size<RankDictionary<TwoLevels<bool, TSpec> > >
{
    typedef __uint32 Type;
};

template <typename TSpec>
struct Size<RankDictionary<Naive<bool, TSpec> > >
{
    typedef __uint32 Type;
};
}

// ----------------------------------------------------------------------------
// Shape Size
// ----------------------------------------------------------------------------

namespace seqan {
template <typename TValue, unsigned q>
struct Value<Shape<TValue, UngappedShape<q> > >
{
    typedef __uint32    Type;
};
}

#endif  // #ifndef SEQAN_EXTRAS_CUDAMAPPER_TYPES_H_