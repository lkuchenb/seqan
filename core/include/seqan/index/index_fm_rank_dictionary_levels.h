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

#ifndef INDEX_FM_RANK_DICTIONARY_LEVELS_H_
#define INDEX_FM_RANK_DICTIONARY_LEVELS_H_

namespace seqan {

// ============================================================================
// Forwards
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryBitMask_
// ----------------------------------------------------------------------------

template <typename TSpec>
struct RankDictionaryBitMask_;

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryWordSize_
// ----------------------------------------------------------------------------

template <typename TSpec>
struct RankDictionaryWordSize_;

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryBitsPerBlock_
// ----------------------------------------------------------------------------

template <typename TSpec>
struct RankDictionaryBitsPerBlock_;

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryBlock_
// ----------------------------------------------------------------------------

template <typename TSpec>
struct RankDictionaryBlock_;

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryValues_
// ----------------------------------------------------------------------------

template <typename TSpec>
struct RankDictionaryValues_;

// ----------------------------------------------------------------------------
// Struct RankDictionaryEntry_
// ----------------------------------------------------------------------------

template <typename TSpec>
struct RankDictionaryEntry_;

// ============================================================================
// Tags
// ============================================================================

// ----------------------------------------------------------------------------
// Tag FibreRanks
// ----------------------------------------------------------------------------

struct FibreRanks_;

typedef Tag<FibreRanks_>
const FibreRanks;

// ----------------------------------------------------------------------------
// Tag TwoLevels
// ----------------------------------------------------------------------------

template <typename TValue = bool, typename TSpec = void>
struct TwoLevels {};

// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction Value                                          [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct Value<RankDictionary<TwoLevels<TValue, TSpec> > >
{
    typedef TValue  Type;
};

template <typename TValue, typename TSpec>
struct Value<RankDictionary<TwoLevels<TValue, TSpec> > const> :
    Value<RankDictionary<TwoLevels<TValue, TSpec> > > {};

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryBitMask_
// ----------------------------------------------------------------------------

template <>
struct RankDictionaryBitMask_<__uint32>
{
    static const __uint32 VALUE = 0x55555555;
};

template <>
struct RankDictionaryBitMask_<__uint64>
{
    static const __uint64 VALUE = 0x5555555555555555;
};

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryWordSize_                             [TwoLevels]
// ----------------------------------------------------------------------------

//#ifdef __CUDACC__
template <typename TValue, typename TSpec>
struct RankDictionaryWordSize_<TwoLevels<TValue, TSpec> > :
    BitsPerValue<__uint32> {};
//#else
//template <typename TValue, typename TSpec>
//struct RankDictionaryWordSize_<TwoLevels<TValue, TSpec> > :
//    BitsPerValue<unsigned long> {};
//#endif

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryBitsPerBlock_                         [TwoLevels]
// ----------------------------------------------------------------------------
// The number of bits per block equals the number of bits of the block summary.

template <typename TValue, typename TSpec>
struct RankDictionaryBitsPerBlock_<TwoLevels<TValue, TSpec> > :
    BitsPerValue<typename RankDictionaryBlock_<TwoLevels<TValue, TSpec> >::Type> {};

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryBlock_                                [TwoLevels]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct RankDictionaryBlock_<TwoLevels<TValue, TSpec> >
{
    typedef RankDictionary<TwoLevels<TValue, TSpec> >               TRankDictionary_;
    typedef typename Size<TRankDictionary_>::Type                   TSize_;

    typedef Tuple<TSize_, ValueSize<TValue>::VALUE>                 Type;
};

template <typename TSpec>
struct RankDictionaryBlock_<TwoLevels<bool, TSpec> >
{
    typedef RankDictionary<TwoLevels<bool, TSpec> >                 TRankDictionary_;

    typedef typename Size<TRankDictionary_>::Type                   Type;
};

// ----------------------------------------------------------------------------
// Metafunction RankDictionaryValues_                               [TwoLevels]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct RankDictionaryValues_<TwoLevels<TValue, TSpec> >
{
    typedef RankDictionary<TwoLevels<TValue, TSpec> >                       TRankDictionary_;
    
    typedef Tuple<TValue, TRankDictionary_::_VALUES_PER_WORD, BitPacked<> > TValues;
    typedef typename TValues::TBitVector                                    TWord;
    typedef Tuple<TValues, TRankDictionary_::_WORDS_PER_BLOCK>              Type;
};

// ----------------------------------------------------------------------------
// Metafunction Fibre                                          [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct Fibre<RankDictionary<TwoLevels<TValue, TSpec> >, FibreRanks>
{
    typedef TwoLevels<TValue, TSpec>                                   TRankDictionarySpec_;
    typedef RankDictionary<TRankDictionarySpec_>                       TRankDictionary_;
    typedef RankDictionaryEntry_<TRankDictionarySpec_>                 TRankDictionaryEntry_;
    typedef typename RankDictionaryFibreSpec<TRankDictionary_>::Type   TRankDictionaryFibreSpec_;

    typedef String<TRankDictionaryEntry_, TRankDictionaryFibreSpec_>   Type;
};

// ============================================================================
// Classes
// ============================================================================

// ----------------------------------------------------------------------------
// Struct RankDictionaryEntry_
// ----------------------------------------------------------------------------

template <typename TSpec = TwoLevels<> >
struct RankDictionaryEntry_ {};

// ----------------------------------------------------------------------------
// Struct TwoLevels RankDictionaryEntry_
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct RankDictionaryEntry_<TwoLevels<TValue, TSpec> >
{
    typedef TwoLevels<TValue, TSpec>    TRankDictionarySpec;

    // A bit-compressed block of TValue symbols.
    typename RankDictionaryValues_<TRankDictionarySpec>::Type   values;

    // A summary of counts for each block of TValue symbols.
    typename RankDictionaryBlock_<TRankDictionarySpec>::Type    block;
};

// ----------------------------------------------------------------------------
// Class TwoLevels RankDictionary
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct RankDictionary<TwoLevels<TValue, TSpec> >
{
    // ------------------------------------------------------------------------
    // Constants
    // ------------------------------------------------------------------------

    static const unsigned _BITS_PER_VALUE   = BitsPerValue<TValue>::VALUE;
    static const unsigned _BITS_PER_BLOCK   = RankDictionaryBitsPerBlock_<TwoLevels<TValue, TSpec> >::VALUE;
    static const unsigned _BITS_PER_WORD    = Min<RankDictionaryWordSize_<TwoLevels<TValue, TSpec> >::VALUE, _BITS_PER_BLOCK>::VALUE;
    static const unsigned _VALUES_PER_WORD  = _BITS_PER_WORD  / _BITS_PER_VALUE;
    static const unsigned _VALUES_PER_BLOCK = _BITS_PER_BLOCK / _BITS_PER_VALUE;
    static const unsigned _WORDS_PER_BLOCK  = _BITS_PER_BLOCK / _BITS_PER_WORD;

    // ------------------------------------------------------------------------
    // Fibres
    // ------------------------------------------------------------------------

    typename Fibre<RankDictionary, FibreRanks>::Type    ranks;
    typename Size<RankDictionary>::Type                 _length;

    // ------------------------------------------------------------------------
    // Constructors
    // ------------------------------------------------------------------------

    // NOTE(esiragusa): NVCC cyclic SEQAN_FUNC problem.
    RankDictionary() {};

//    RankDictionary() : _length(0) {};

    template <typename TText>
    RankDictionary(TText const & text)
    {
        createRankDictionary(*this, text);
    }
};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function loadAndCache()
// ----------------------------------------------------------------------------
// TODO(esiragusa): move loadAndCache() in misc_cuda.h

template <typename TValue>
SEQAN_FUNC TValue
loadAndCache(TValue const & value)
{
#if __CUDA_ARCH__ >= 350
    return __ldg(&value);
#else
    return value;
#endif
}

// ----------------------------------------------------------------------------
// Function loadAndCache()                                              [Tuple]
// ----------------------------------------------------------------------------
// TODO(esiragusa): move loadAndCache() in misc_cuda.h

template <typename TValue, unsigned SIZE, typename TSpec>
SEQAN_FUNC Tuple<TValue, SIZE, TSpec>
loadAndCache(Tuple<TValue, SIZE, TSpec> const & tuple)
{
#if __CUDA_ARCH__ >= 350
    typedef Tuple<TValue, SIZE, TSpec>  TTuple;

    const unsigned UINTS = BytesPerValue<TTuple>::VALUE / 4;

    union { TTuple x; uint4 y[UINTS]; } tmp;

    for (unsigned u = 0; u < UINTS; ++u)
        tmp.y[u] = __ldg(reinterpret_cast<uint4 const *>(&tuple) + u);

    return tmp.x;
#else
    return tuple;
#endif
}

// ----------------------------------------------------------------------------
// Function loadAndCache()                               [RankDictionaryEntry_]
// ----------------------------------------------------------------------------
//
//template <typename TValue, typename TSpec>
//SEQAN_FUNC RankDictionaryEntry_<TwoLevels<TValue, TSpec> >
//loadAndCache(RankDictionaryEntry_<TwoLevels<TValue, TSpec> > const & entry)
//{
//#if __CUDA_ARCH__ >= 350
//    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
//    typedef RankDictionaryEntry_<TRankDictionarySpec>   TEntry;
//
//    const unsigned UINTS = BytesPerValue<TEntry>::VALUE / 4;
//
//    union { TEntry x; uint4 y[UINTS]; } tmp;
//
//    for (unsigned u = 0; u < UINTS; ++u)
//        tmp.y[u] = __ldg(reinterpret_cast<uint4 const *>(&entry) + u);
//
//    return tmp.x;
//#else
//    return entry;
//#endif
//}

//template <unsigned SIZE, typename TSpec>
//SEQAN_FUNC Tuple<Tuple<Dna, SIZE, BitPacked<> >, 4, TSpec>
//loadAndCache(Tuple<Tuple<Dna, SIZE, BitPacked<> >, 4, TSpec> const & values)
//{
//#if __CUDA_ARCH__ >= 350
//    Tuple<Tuple<Dna, SIZE, BitPacked<> >, 4, TSpec> tmp;
//
//    uint4 t = __ldg((uint4 *)values.i);
//    tmp.i[0].i = t.x;
//    tmp.i[1].i = t.y;
//    tmp.i[2].i = t.z;
//    tmp.i[3].i = t.w;
//
//    return tmp;
//#else
//    return values;
//#endif
//}

//template <unsigned SIZE, typename TSpec>
//SEQAN_FUNC Tuple<Tuple<bool, SIZE, BitPacked<> >, 1, TSpec>
//loadAndCache(Tuple<Tuple<bool, SIZE, BitPacked<> >, 1, TSpec> const & values)
//{
//#if __CUDA_ARCH__ >= 350
//    Tuple<Tuple<bool, SIZE, BitPacked<> >, 1, TSpec> tmp;
//
//    uint2 t = __ldg((uint2 *)values.i);
//    tmp.i[0].i = t.x;
//    tmp.i[1].i = t.y;
//
//    return tmp;
//#else
//    return values;
//#endif
//}

// ----------------------------------------------------------------------------
// Function getFibre()                                         [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
SEQAN_FUNC typename Fibre<RankDictionary<TwoLevels<TValue, TSpec> >, FibreRanks>::Type &
getFibre(RankDictionary<TwoLevels<TValue, TSpec> > & dict, FibreRanks)
{
    return dict.ranks;
}

template <typename TValue, typename TSpec>
SEQAN_FUNC typename Fibre<RankDictionary<TwoLevels<TValue, TSpec> >, FibreRanks>::Type const &
getFibre(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, FibreRanks)
{
    return dict.ranks;
}

// ----------------------------------------------------------------------------
// Function _toPosInWord()                                     [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
_toPosInWord(RankDictionary<TwoLevels<TValue, TSpec> > const & /* dict */, TPos posInBlock)
{
    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>         TRankDictionary;

    return posInBlock % TRankDictionary::_VALUES_PER_WORD;
}

// ----------------------------------------------------------------------------
// Function _toWordPos()                                       [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
_toWordPos(RankDictionary<TwoLevels<TValue, TSpec> > const & /* dict */, TPos posInBlock)
{
    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>         TRankDictionary;

    return posInBlock / TRankDictionary::_VALUES_PER_WORD;
}

// ----------------------------------------------------------------------------
// Function _toPosInBlock()                                    [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
_toPosInBlock(RankDictionary<TwoLevels<TValue, TSpec> > const & /* dict */, TPos pos)
{
    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>         TRankDictionary;

    return pos % TRankDictionary::_VALUES_PER_BLOCK;
}

// ----------------------------------------------------------------------------
// Function _toBlockPos()                                      [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
_toBlockPos(RankDictionary<TwoLevels<TValue, TSpec> > const & /* dict */, TPos pos)
{
    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>         TRankDictionary;

    return pos / TRankDictionary::_VALUES_PER_BLOCK;
}

// ----------------------------------------------------------------------------
// Function _toPos()                                           [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TBlockPos>
inline typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
_toPos(RankDictionary<TwoLevels<TValue, TSpec> > const & /* dict */, TBlockPos blockPos)
{
    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>         TRankDictionary;

    return blockPos * TRankDictionary::_VALUES_PER_BLOCK;
}

// ----------------------------------------------------------------------------
// Function _valuesAt()                                        [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TBlockPos, typename TWordPos>
SEQAN_FUNC typename RankDictionaryValues_<TwoLevels<TValue, TSpec> >::TValues &
_valuesAt(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TBlockPos blockPos, TWordPos wordPos)
{
    return dict.ranks[blockPos].values[wordPos];
}

template <typename TValue, typename TSpec, typename TBlockPos, typename TWordPos>
SEQAN_FUNC typename RankDictionaryValues_<TwoLevels<TValue, TSpec> >::TValues const &
_valuesAt(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, TBlockPos blockPos, TWordPos wordPos)
{
    return dict.ranks[blockPos].values[wordPos];
}

// ----------------------------------------------------------------------------
// Function _valuesAt()                                        [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename RankDictionaryValues_<TwoLevels<TValue, TSpec> >::Type &
_valuesAt(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TPos pos)
{
    return dict.ranks[_toBlockPos(dict, pos)].values;
}

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename RankDictionaryValues_<TwoLevels<TValue, TSpec> >::Type const &
_valuesAt(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, TPos pos)
{
    return dict.ranks[_toBlockPos(dict, pos)].values;
}

// ----------------------------------------------------------------------------
// Function _blockAt()                                         [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename RankDictionaryBlock_<TwoLevels<TValue, TSpec> >::Type &
_blockAt(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TPos pos)
{
    return dict.ranks[_toBlockPos(dict, pos)].block;
}

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename RankDictionaryBlock_<TwoLevels<TValue, TSpec> >::Type const &
_blockAt(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, TPos pos)
{
    return dict.ranks[_toBlockPos(dict, pos)].block;
}

// ----------------------------------------------------------------------------
// Function _clearBlockAt()                                    [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
inline void _clearBlockAt(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TPos pos)
{
    clear(_blockAt(dict, pos));
}

// ----------------------------------------------------------------------------
// Function _clearBlockAt(bool)                                [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TSpec, typename TPos>
inline void _clearBlockAt(RankDictionary<TwoLevels<bool, TSpec> > & dict, TPos pos)
{
    _blockAt(dict, pos) = 0u;
}

// ----------------------------------------------------------------------------
// Function _getBlockRank()                                    [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TBlock, typename TPos, typename TChar>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > const>::Type
_getBlockRank(RankDictionary<TwoLevels<TValue, TSpec> > const & /* dict */, TBlock const & block, TPos /* pos */, TChar c)
{
    return loadAndCache(block[ordValue(c)]);
//    return block[ordValue(c)];
}

// ----------------------------------------------------------------------------
// Function _getBlockRank(bool)                                [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TSpec, typename TBlock, typename TPos>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<bool, TSpec> > const>::Type
_getBlockRank(RankDictionary<TwoLevels<bool, TSpec> > const & dict, TBlock const & block, TPos pos, bool c)
{
    TBlock rank = loadAndCache(block);

    // If c == false then return the complementary rank.
    return c ? rank : pos - _toPosInBlock(dict, pos) - rank;

//    return c ? block : pos - _toPosInBlock(dict, pos) - block;
}

// ----------------------------------------------------------------------------
// Function _getValueRank()                                    [RankDictionary]
// ----------------------------------------------------------------------------
// NOTE(esiragusa): This version is generic but absymally slow.

//template <typename TValue, typename TSpec, typename TValues, typename TPosInWord>
//inline typename Size<RankDictionary<TwoLevels<TValue, TSpec> > const>::Type
//_getValueRank(RankDictionary<TwoLevels<TValue, TSpec> > const & dict,
//              TValues values,
//              TPosInWord posInWord,
//              TValue c)
//{
//    typedef TwoLevels<TValue, TSpec>                                    TRankDictionarySpec;
//    typedef RankDictionary<TRankDictionarySpec>                         TRankDictionary;
//    typedef typename Size<TRankDictionary>::Type                        TSize;
//
//    TSize valueRank = 0;
//
//    for (TSize i = 0; i <= posInWord; ++i)
//        valueRank += isEqual(_valuesAt(dict, blockPos, wordPos)[i], c);
//
//    return valueRank;
//}

// ----------------------------------------------------------------------------
// Function _getWordRank(Dna)                                  [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TSpec, typename TWord, typename TPosInWord>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<Dna, TSpec> > const>::Type
_getWordRank(RankDictionary<TwoLevels<Dna, TSpec> > const & /* dict */,
             TWord const & values,
             TPosInWord posInWord,
             Dna c)
{
    typedef TwoLevels<Dna, TSpec>                                       TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                         TRankDictionary;
    typedef typename Size<TRankDictionary>::Type                        TSize;

    // Clear the last positions.
    TWord word = hiBits(values, TRankDictionary::_BITS_PER_VALUE * (posInWord + 1));

    // And matches when c == G|T.
    TWord odd  = ((ordValue(c) & ordValue(Dna('G'))) ? word : ~word) >> 1;

    // And matches when c == C|T.
    TWord even = ((ordValue(c) & ordValue(Dna('C'))) ? word : ~word);

    // Apply the interleaved mask.
    TWord mask = odd & even & RankDictionaryBitMask_<TWord>::VALUE;

    // The rank is the sum of the bits on.
    TSize valueRank = popCount(mask);

    // If c == A then masked character positions must be subtracted from the count.
    if (c == Dna('A')) valueRank -= TRankDictionary::_VALUES_PER_WORD - (posInWord + 1);

    return valueRank;
}

// ----------------------------------------------------------------------------
// Function _getWordRank(bool)                                 [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TSpec, typename TWord, typename TPosInWord>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<bool, TSpec> > const>::Type
_getWordRank(RankDictionary<TwoLevels<bool, TSpec> > const & /* dict */,
             TWord const & values,
             TPosInWord posInWord,
             bool c)
{
    // Negate the values to compute the rank of zero.
    TWord word = c ? values : ~values;

    // Clear the last positions.
    TWord mask = hiBits(word, posInWord + 1);

    // Get the sum of the bits on.
    return popCount(mask);
}

// ----------------------------------------------------------------------------
// Function _getWordRank()                                     [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TWord>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > const>::Type
_getWordRank(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, TWord const & values, TValue c)
{
    typedef TwoLevels<TValue, TSpec>                                TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                     TRankDictionary;

    return _getWordRank(dict, values, TRankDictionary::_VALUES_PER_WORD - 1, c);
}

// ----------------------------------------------------------------------------
// Function _getValueRank()                                    [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TValues, typename TPosInBlock>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > const>::Type
_getValueRank(RankDictionary<TwoLevels<TValue, TSpec> > const & dict,
              TValues const & values,
              TPosInBlock posInBlock,
              TValue c)
{
    typedef TwoLevels<TValue, TSpec>                                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                         TRankDictionary;
    typedef typename Size<TRankDictionary>::Type                        TSize;

    TSize wordPos    = _toWordPos(dict, posInBlock);
    TSize posInWord  = _toPosInWord(dict, posInBlock);

    TSize valueRank = 0;

    for (TSize wordPrevPos = 0; wordPrevPos < wordPos; ++wordPrevPos)
      valueRank += _getWordRank(dict, values[wordPrevPos].i, c);

    valueRank += _getWordRank(dict, values[wordPos].i, posInWord, c);

    return valueRank;
}

// ----------------------------------------------------------------------------
// Function _getValuesRanks()                                  [RankDictionary]
// ----------------------------------------------------------------------------
// TODO(esiragusa): Specialize _getValuesRanks() for Dna.

template <typename TValue, typename TSpec, typename TPos>
inline typename RankDictionaryBlock_<TwoLevels<TValue, TSpec> >::Type
_getValuesRanks(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, TPos pos)
{
    typedef TwoLevels<TValue, TSpec>                                    TRankDictionarySpec;
    typedef typename RankDictionaryBlock_<TRankDictionarySpec>::Type    TBlock;
    typedef typename ValueSize<TValue>::Type                            TValueSize;

    TBlock blockRank;

    for (TValueSize c = 0; c < ValueSize<TValue>::VALUE; ++c)
        assignValue(blockRank, c, _getValueRank(dict, _valuesAt(dict, pos), _toPosInBlock(dict, pos), TValue(c)));

    return blockRank;
}

// ----------------------------------------------------------------------------
// Function _getValuesRanks(bool)                              [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TSpec, typename TPos>
inline typename RankDictionaryBlock_<TwoLevels<bool, TSpec> >::Type
_getValuesRanks(RankDictionary<TwoLevels<bool, TSpec> > const & dict, TPos pos)
{
    return _getValueRank(dict, _valuesAt(dict, pos), _toPosInBlock(dict, pos), true);
}

// ----------------------------------------------------------------------------
// Function getRank()                                          [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos, typename TChar>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<TValue, TSpec> > const>::Type
getRank(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, TPos pos, TChar c)
{
    typedef TwoLevels<TValue, TSpec>                                        TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec> const                       TRankDictionary;
    typedef typename Fibre<TRankDictionary, FibreRanks>::Type               TFibreRanks;
    typedef typename Value<TFibreRanks>::Type                               TRankEntry;
    typedef typename RankDictionaryBlock_<TRankDictionarySpec>::Type        TRankBlock;
    typedef typename RankDictionaryValues_<TRankDictionarySpec>::Type       TRankValues;
    typedef typename Size<TRankDictionary>::Type                            TSize;

    TSize blockPos   = _toBlockPos(dict, pos);
    TSize posInBlock = _toPosInBlock(dict, pos);

//    TRankEntry entry = loadAndCache(dict.ranks[blockPos]);
    TRankEntry const & entry = dict.ranks[blockPos];

//    TRankBlock block = loadAndCache(entry.block);
    TRankValues values = loadAndCache(entry.values);

    return _getBlockRank(dict, entry.block, pos, static_cast<TValue>(c)) +
           _getValueRank(dict, values, posInBlock, static_cast<TValue>(c));
}

// ----------------------------------------------------------------------------
// Function getRank(bool)                                      [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TSpec, typename TPos>
SEQAN_FUNC typename Size<RankDictionary<TwoLevels<bool, TSpec> > const>::Type
getRank(RankDictionary<TwoLevels<bool, TSpec> > const & dict, TPos pos)
{
    return getRank(dict, pos, true);
}

// ----------------------------------------------------------------------------
// Function getValue()                                         [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename Value<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
getValue(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TPos pos)
{
    typedef TwoLevels<TValue, TSpec>                                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                         TRankDictionary;
    typedef typename Size<TRankDictionary>::Type                        TSize;

    TSize blockPos   = _toBlockPos(dict, pos);
    TSize posInBlock = _toPosInBlock(dict, pos);
    TSize wordPos    = _toWordPos(dict, posInBlock);
    TSize posInWord  = _toPosInWord(dict, posInBlock);

    return _valuesAt(dict, blockPos, wordPos)[posInWord];
}

template <typename TValue, typename TSpec, typename TPos>
SEQAN_FUNC typename Value<RankDictionary<TwoLevels<TValue, TSpec> > const>::Type
getValue(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, TPos pos)
{
    typedef TwoLevels<TValue, TSpec>                                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                         TRankDictionary;
    typedef typename Size<TRankDictionary>::Type                        TSize;

    TSize blockPos   = _toBlockPos(dict, pos);
    TSize posInBlock = _toPosInBlock(dict, pos);
    TSize wordPos    = _toWordPos(dict, posInBlock);
    TSize posInWord  = _toPosInWord(dict, posInBlock);

    return _valuesAt(dict, blockPos, wordPos)[posInWord];
}

// ----------------------------------------------------------------------------
// Function setValue()                                         [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TPos, typename TChar>
inline void setValue(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TPos pos, TChar c)
{
    typedef TwoLevels<TValue, TSpec>                                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                         TRankDictionary;
    typedef typename Size<TRankDictionary>::Type                        TSize;

    TSize blockPos   = _toBlockPos(dict, pos);
    TSize posInBlock = _toPosInBlock(dict, pos);
    TSize wordPos    = _toWordPos(dict, posInBlock);
    TSize posInWord  = _toPosInWord(dict, posInBlock);

    assignValue(_valuesAt(dict, blockPos, wordPos), posInWord, static_cast<TValue>(c));
}

// ----------------------------------------------------------------------------
// Function appendValue()                                      [RankDictionary]
// ----------------------------------------------------------------------------
// NOTE(esiragusa): Better not to have appendValue() - it is not efficient - and thus neither length().

template <typename TValue, typename TSpec, typename TChar, typename TExpand>
inline void appendValue(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TChar c, Tag<TExpand> const tag)
{
    resize(dict, length(dict) + 1, tag);
    setValue(dict, length(dict) - 1, c);
}

// ----------------------------------------------------------------------------
// Function updateRanks()                                      [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline void updateRanks(RankDictionary<TwoLevels<TValue, TSpec> > & dict)
{
    typedef TwoLevels<TValue, TSpec>                                TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                     TRankDictionary;
    typedef typename Size<TRankDictionary>::Type                    TSize;
    typedef typename Fibre<TRankDictionary, FibreRanks>::Type       TFibreRanks;
    typedef typename Iterator<TFibreRanks, Standard>::Type          TFibreRanksIter;

    if (empty(dict)) return;

    TFibreRanksIter ranksBegin = begin(dict.ranks, Standard());
    TFibreRanksIter ranksEnd = end(dict.ranks, Standard());

    // Insures the first block ranks start from zero.
    _clearBlockAt(dict, 0u);

    // Iterate through the blocks.
    for (TFibreRanksIter ranksIt = ranksBegin; ranksIt != ranksEnd - 1; ++ranksIt)
    {
        TSize blockPos = ranksIt - ranksBegin;
        TSize curr = _toPos(dict, blockPos);
        TSize next = _toPos(dict, blockPos + 1);

        _blockAt(dict, next) = _blockAt(dict, curr) + _getValuesRanks(dict, next - 1);
    }
}

// ----------------------------------------------------------------------------
// Function createRankDictionary()                             [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TText>
inline void
createRankDictionary(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TText const & text)
{
    typedef TwoLevels<TValue, TSpec>                                TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>                     TRankDictionary;
    typedef typename Size<TRankDictionary>::Type                    TSize;
    typedef typename Iterator<TText const, Standard>::Type          TTextIterator;

    // Resize the RankDictionary.
    resize(dict, length(text), Exact());

    // Assign the text value by value.
    TTextIterator textBegin = begin(text, Standard());
    TTextIterator textEnd = end(text, Standard());
    for (TTextIterator textIt = textBegin; textIt != textEnd; ++textIt)
        setValue(dict, textIt - textBegin, value(textIt));

    // Update all ranks.
    updateRanks(dict);
}

// ----------------------------------------------------------------------------
// Function clear()                                            [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline void clear(RankDictionary<TwoLevels<TValue, TSpec> > & dict)
{
    clear(dict.ranks);
}

// ----------------------------------------------------------------------------
// Function empty()                                            [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
SEQAN_FUNC bool empty(RankDictionary<TwoLevels<TValue, TSpec> > const & dict)
{
    return empty(dict.ranks);
}

// ----------------------------------------------------------------------------
// Function length()                                           [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
length(RankDictionary<TwoLevels<TValue, TSpec> > const & dict)
{
    return dict._length;
}

// ----------------------------------------------------------------------------
// Function reserve()                                          [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TSize, typename TExpand>
inline typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
reserve(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TSize newCapacity, Tag<TExpand> const tag)
{
    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>         TRankDictionary;

   return reserve(dict.ranks, std::ceil(newCapacity / static_cast<double>(TRankDictionary::_VALUES_PER_BLOCK)), tag);
}

// ----------------------------------------------------------------------------
// Function resize()                                           [RankDictionary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TSize, typename TExpand>
inline typename Size<RankDictionary<TwoLevels<TValue, TSpec> > >::Type
resize(RankDictionary<TwoLevels<TValue, TSpec> > & dict, TSize newLength, Tag<TExpand> const tag)
{
    typedef TwoLevels<TValue, TSpec>                    TRankDictionarySpec;
    typedef RankDictionary<TRankDictionarySpec>         TRankDictionary;

    dict._length = newLength;
    return resize(dict.ranks, std::ceil(newLength / static_cast<double>(TRankDictionary::_VALUES_PER_BLOCK)), tag);
}

// ----------------------------------------------------------------------------
// Function open()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline bool open(RankDictionary<TwoLevels<TValue, TSpec> > & dict, const char * fileName, int openMode)
{
    // TODO(esiragusa): Open _length or remove it.
    return open(dict.ranks, fileName, openMode);
}

template <typename TValue, typename TSpec>
inline bool open(RankDictionary<TwoLevels<TValue, TSpec> > & dict, const char * fileName)
{
    return open(dict, fileName, DefaultOpenMode<RankDictionary<TwoLevels<TValue, TSpec> > >::VALUE);
}

// ----------------------------------------------------------------------------
// Function save()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline bool save(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, const char * fileName, int openMode)
{
    return save(dict.ranks, fileName, openMode);
}

template <typename TValue, typename TSpec>
inline bool save(RankDictionary<TwoLevels<TValue, TSpec> > const & dict, const char * fileName)
{
    // TODO(esiragusa): Save _length or remove it.
    return save(dict, fileName, DefaultOpenMode<RankDictionary<TwoLevels<TValue, TSpec> > >::VALUE);
}

}

#endif  // INDEX_FM_RANK_DICTIONARY_LEVELS_H_
