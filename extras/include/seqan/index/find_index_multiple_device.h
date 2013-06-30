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

#ifndef SEQAN_EXTRAS_INDEX_FIND_INDEX_MULTIPLE_DEVICE_H
#define SEQAN_EXTRAS_INDEX_FIND_INDEX_MULTIPLE_DEVICE_H

namespace seqan {

// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction FinderCTASize_
// ----------------------------------------------------------------------------

template <typename TText, typename TPattern, typename TSpec>
struct FinderCTASize_;

template <typename TText, typename TIndexSpec, typename TPattern, typename TSpec>
struct FinderCTASize_<Index<TText, TIndexSpec>, TPattern, Multiple<TSpec> >
{
    static const unsigned VALUE = 256;
};

// ============================================================================
// Kernels
// ============================================================================

// ----------------------------------------------------------------------------
// Kernel _computeHashesKernel()
// ----------------------------------------------------------------------------

template <typename TFinderView, typename TPatternsView>
__global__ void
_computeHashesKernel(TFinderView finder, TPatternsView patterns)
{
    typedef typename Value<TPatternsView>::Type         TPatternView;
    typedef typename Value<TPatternView>::Type          TAlphabet;
    typedef Shape<TAlphabet, UngappedShape<10> >        TShape;

    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // Return silently if there is no job left.
    if (idx >= length(patterns)) return;

    // Compute the hash of a read.
    TShape shape;
    finder._hashes[idx] = hash(shape, begin(patterns[idx], Standard()));

    // Fill idxs with the identity permutation.
    finder._idxs[idx] = idx;
}

// ----------------------------------------------------------------------------
// Kernel _findKernel()
// ----------------------------------------------------------------------------

template <typename TFinderView, typename TPatternsView, typename TDelegateView>
__global__ void
_findKernel(TFinderView finder, TPatternsView patterns, TDelegateView delegate)
{
    typedef Proxy<TFinderView>                              TFinderProxy;
    typedef Delegator<TFinderProxy, TDelegateView>          TDelegator;
    typedef typename FinderSerial_<TFinderView>::Type       TSerialFinder;

    unsigned threadId = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned gridThreads = gridDim.x * blockDim.x;

    // Instantiate a serial finder.
    TSerialFinder serialFinder = getObject(finder._factory, threadId);

    // Instantiate a finder proxy to be delegated.
    TFinderProxy finderProxy(serialFinder);

    // Instantiate a delegator object to delegate the finder proxy instead of the serial finder.
    TDelegator delegator(finderProxy, delegate);

    unsigned patternsCount = length(patterns);

	for (unsigned patternId = threadId; patternId < patternsCount; patternId += gridThreads)
    {
//        finderProxy._patternIt = begin(patterns, Standard()) + patternId;
        finderProxy._patternIt = patternId;

        // Find a single pattern.
        find(serialFinder, patterns[patternId], delegator);
//        find(finderFlyweight, patterns[finder._idxs[patternId]], delegator);
    }
}

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function preprocess()
// ----------------------------------------------------------------------------

//template <typename TText, typename TIndexSpec, typename TPattern, typename TSpec, typename TPatterns>
//SEQAN_FUNC void
//preprocess(Finder2<Index<TText, TIndexSpec>, TPattern, TSpec> & finder, TPatterns const & patterns)
//{
//    _resize(finder, length(patterns));
//    _computeHashes(finder, patterns);
//    _fillIdxsWithIdentity(finder);
//    _sortIdxsByHashes(finder);
//    cudaDeviceSynchronize();
//}

// ----------------------------------------------------------------------------
// Function _find(); ExecDevice
// ----------------------------------------------------------------------------

template <typename TText, typename TPattern, typename TSpec, typename TDelegate>
inline void
_find(Finder2<TText, TPattern, Multiple<TSpec> > & finder,
      TPattern /* const */ & pattern,
      TDelegate & delegate,
      ExecDevice const & /* tag */)
{
    typedef Finder2<TText, TPattern, Multiple<TSpec> >  TFinder;
    typedef typename View<TFinder>::Type                TFinderView;
    typedef typename View<TPattern>::Type               TPatternView;
    typedef typename View<TDelegate>::Type              TDelegateView;

    // Preprocess patterns.
//    _preprocess(finder, patterns);

    // Compute grid size.
    unsigned ctaSize = FinderCTASize_<TText, TPattern, Multiple<TSpec> >::VALUE;
    unsigned activeBlocks = cudaMaxActiveBlocks(_findKernel<TFinderView, TPatternView, TDelegateView>, ctaSize, 0);

    std::cout << "CTA Size:\t\t\t" << ctaSize << std::endl;
    std::cout << "Active Blocks:\t\t\t" << activeBlocks << std::endl;

    // Initialize the iterator factory.
    setMaxHistoryLength(finder._factory, length(back(pattern)));
    setMaxObjects(finder._factory, activeBlocks * ctaSize);
    build(finder._factory);

    // Launch the find kernel.
    _findKernel<<<activeBlocks, ctaSize>>>(view(finder), view(pattern), view(delegate));
}

}

#endif  // #ifndef SEQAN_EXTRAS_INDEX_FIND_INDEX_MULTIPLE_DEVICE_H