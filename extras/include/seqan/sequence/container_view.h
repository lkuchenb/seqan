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
// Author: David Weese <david.weese@fu-berlin.de>
// Author: Enrico Siragusa <enrico.siragusa@fu-berlin.de>
// ==========================================================================

#ifndef SEQAN_EXTRAS_SEQUENCE_CONTAINER_VIEW_H
#define SEQAN_EXTRAS_SEQUENCE_CONTAINER_VIEW_H

namespace seqan {

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ----------------------------------------------------------------------------
// Tag Resizable
// ----------------------------------------------------------------------------

template <typename TSpec = void>
struct Resizable;

// ----------------------------------------------------------------------------
// Class ContainerView
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec = void>
class ContainerView
{
public:
    typedef typename Iterator<ContainerView, Standard>::Type        TIterator;

    TIterator _begin;
    TIterator _end;

    // ------------------------------------------------------------------------
    // ContainerView Constructors
    // ------------------------------------------------------------------------

    SEQAN_FUNC
    ContainerView() :
        _begin(),
        _end()
    {}

    template <typename TOtherContainer>
    SEQAN_FUNC
    ContainerView(TOtherContainer & cont):
        _begin(begin(cont, Standard())),
        _end(end(cont, Standard()))
    {}

    template <typename TOtherContainer>
    SEQAN_FUNC
    ContainerView(TOtherContainer const & cont):
        _begin(begin(cont, Standard())),
        _end(end(cont, Standard()))
    {}

    SEQAN_FUNC
    ContainerView(TIterator const & begin, TIterator const & end):
        _begin(begin),
        _end(end)
    {}

    // ------------------------------------------------------------------------
    // Operator =
    // ------------------------------------------------------------------------

    template <typename TOtherContainer>
    SEQAN_FUNC
    ContainerView &
    operator= (TOtherContainer & other)
    {
        assign(*this, other);
        return *this;
    }

    // ------------------------------------------------------------------------
    // Operator []
    // ------------------------------------------------------------------------

    template <typename TPos>
    SEQAN_FUNC
    typename Reference<ContainerView>::Type
    operator[] (TPos pos)
    {
        return value(*this, pos);
    }

    template <typename TPos>
    SEQAN_FUNC
    typename GetValue<ContainerView>::Type
    operator[] (TPos pos) const
    {
        return getValue(*this, pos);
    }
};

// ----------------------------------------------------------------------------
// Class Resizable ContainerView
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
class ContainerView<TContainer, Resizable<TSpec> > :
    public ContainerView<TContainer, TSpec>
{
public:
    typedef ContainerView<TContainer, TSpec>    TBase;
    typedef typename TBase::TIterator           TIterator;
    typedef typename Size<TContainer>::Type     TSize;

    TSize _capacity;

    // ------------------------------------------------------------------------
    // ContainerView Constructors
    // ------------------------------------------------------------------------

    SEQAN_FUNC
    ContainerView() :
        TBase(),
        _capacity(0)
    {}

    template <typename TOtherContainer>
    SEQAN_FUNC
    ContainerView(TOtherContainer & cont) :
        TBase(cont),
        _capacity(capacity(cont))
    {}

    template <typename TOtherContainer>
    SEQAN_FUNC
    ContainerView(TOtherContainer const & cont) :
        TBase(cont),
        _capacity(capacity(cont))
    {}

    SEQAN_FUNC
    ContainerView(TIterator const & begin, TIterator const & end) :
        TBase(begin, end),
        _capacity(end - begin)
    {}
};

// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction View
// ----------------------------------------------------------------------------

template <typename TValue, typename TAlloc>
struct View<String<TValue, TAlloc> >
{
    typedef ContainerView<String<TValue, TAlloc> >  Type;
};

// ----------------------------------------------------------------------------
// Metafunction RemoveView
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct RemoveView<ContainerView<TContainer, TSpec> >
{
    typedef TContainer  Type;
};

// ----------------------------------------------------------------------------
// Metafunction Value
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct Value<ContainerView<TContainer, TSpec> >
{
    typedef typename Value<TContainer>::Type Type;
};

template <typename TContainer, typename TSpec>
struct Value<ContainerView<TContainer, TSpec> const> :
    public Value<ContainerView<TContainer const, TSpec> > {};

// ----------------------------------------------------------------------------
// Metafunction GetValue
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct GetValue<ContainerView<TContainer, TSpec> >
{
    typedef ContainerView<TContainer, TSpec>                        TContainerView_;
    typedef typename Iterator<TContainerView_, Standard>::Type      TIterator_;
    typedef typename GetValue<TIterator_>::Type                     Type;
};

template <typename TContainer, typename TSpec>
struct GetValue<ContainerView<TContainer, TSpec> const>
{
    typedef ContainerView<TContainer, TSpec> const                  TContainerView_;
    typedef typename Iterator<TContainerView_, Standard>::Type      TIterator_;
    typedef typename GetValue<TIterator_>::Type                     Type;
};

// ----------------------------------------------------------------------------
// Metafunction Iterator
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct Iterator<ContainerView<TContainer, TSpec>, Standard>:
    public Iterator<TContainer, Standard> {};

template <typename TContainer, typename TSpec>
struct Iterator<ContainerView<TContainer, TSpec> const, Standard>:
    public Iterator<TContainer const, Standard> {};

#ifdef PLATFORM_CUDA
template <typename TContainer, typename TAlloc, typename TSpec>
struct Iterator<ContainerView<thrust::device_vector<TContainer, TAlloc>, TSpec>, Standard>
{
    typedef typename thrust::device_vector<TContainer, TAlloc>::pointer         TIterator_;
    typedef typename thrust::detail::pointer_traits<TIterator_>::raw_pointer    Type;
};

template <typename TContainer, typename TAlloc, typename TSpec>
struct Iterator<ContainerView<thrust::device_vector<TContainer, TAlloc>, TSpec> const, Standard>
{
    typedef typename thrust::device_vector<TContainer const, TAlloc>::pointer   TIterator_;
    typedef typename thrust::detail::pointer_traits<TIterator_>::raw_pointer    Type;
};
#endif

// ----------------------------------------------------------------------------
// Metafunction Difference
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct Difference<ContainerView<TContainer, TSpec> >
{
    typedef typename Difference<TContainer>::Type Type;
};

// ----------------------------------------------------------------------------
// Metafunction Size
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct Size<ContainerView<TContainer, TSpec> >
{
    typedef typename Difference<TContainer>::Type       TDifference;
    typedef typename MakeUnsigned<TDifference>::Type    Type;
};

// ----------------------------------------------------------------------------
// Metafunction IsSequence
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct IsSequence<ContainerView<TContainer, TSpec> >:
    public IsSequence<TContainer> {};

// ----------------------------------------------------------------------------
// Metafunction Infix
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
struct Infix<ContainerView<TContainer, TSpec> >
{
    typedef ContainerView<TContainer, TSpec>    Type;
};

template <typename TContainer, typename TSpec>
struct Infix<ContainerView<TContainer, TSpec> const> :
    Infix<ContainerView<TContainer, TSpec> > {};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function begin()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
SEQAN_FUNC
typename Iterator<ContainerView<TContainer, TSpec>, Standard>::Type
begin(ContainerView<TContainer, TSpec> & view, Standard)
{
    return view._begin;
}

template <typename TContainer, typename TSpec>
SEQAN_FUNC
typename Iterator<ContainerView<TContainer, TSpec> const, Standard>::Type
begin(ContainerView<TContainer, TSpec> const & view, Standard)
{
    return view._begin;
}

// ----------------------------------------------------------------------------
// Function end()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
SEQAN_FUNC
typename Iterator<ContainerView<TContainer, TSpec>, Standard>::Type
end(ContainerView<TContainer, TSpec> & view, Standard)
{
    return view._end;
}

template <typename TContainer, typename TSpec>
SEQAN_FUNC
typename Iterator<ContainerView<TContainer, TSpec> const, Standard>::Type
end(ContainerView<TContainer, TSpec> const & view, Standard)
{
    return view._end;
}

// ----------------------------------------------------------------------------
// Function value()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec, typename TPos>
SEQAN_FUNC
typename Reference<ContainerView<TContainer, TSpec> >::Type
value(ContainerView<TContainer, TSpec> & view, TPos pos)
{
    return *(view._begin + pos);
}

template <typename TContainer, typename TSpec, typename TPos>
SEQAN_FUNC
typename Reference<ContainerView<TContainer, TSpec> const>::Type
value(ContainerView<TContainer, TSpec> const & view, TPos pos)
{
    return *(view._begin + pos);
}

// ----------------------------------------------------------------------------
// Function getValue()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec, typename TPos>
SEQAN_FUNC
typename GetValue<ContainerView<TContainer, TSpec> >::Type
getValue(ContainerView<TContainer, TSpec> & view, TPos pos)
{
    return getValue(view._begin + pos);
}

template <typename TContainer, typename TSpec, typename TPos>
SEQAN_FUNC
typename GetValue<ContainerView<TContainer, TSpec> const>::Type
getValue(ContainerView<TContainer, TSpec> const & view, TPos pos)
{
    return getValue(view._begin + pos);
}

// ----------------------------------------------------------------------------
// Function length()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
SEQAN_FUNC
typename Difference<ContainerView<TContainer, TSpec> >::Type
length(ContainerView<TContainer, TSpec> const & view)
{
    return view._end - view._begin;
}

// ----------------------------------------------------------------------------
// Function resize()
// ----------------------------------------------------------------------------

// this function doesn't do anything as we are not allowed to change the host (only its elements)
// it is, however, implemented for algorithms that get a sequence to work on
// and need to make sure that it has a certain length

template <typename TContainer, typename TSpec, typename TSize, typename TValue, typename TExpand>
inline typename Size< ContainerView<TContainer, TSpec> >::Type
resize(ContainerView<TContainer, TSpec> & me, TSize new_length, TValue /* val */, Tag<TExpand>)
{
    ignoreUnusedVariableWarning(new_length);

    SEQAN_ASSERT_EQ(new_length, (TSize)length(me));
    return length(me);
}

template <typename TContainer, typename TSpec, typename TSize, typename TExpand>
inline typename Size< ContainerView<TContainer, TSpec> >::Type
resize(ContainerView<TContainer, TSpec> & me, TSize new_length, Tag<TExpand> tag)
{
    return resize(me, new_length, Nothing(), tag);
}

// ----------------------------------------------------------------------------
// Function resize(); Resizable ContainerView
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec, typename TSize, typename TValue, typename TExpand>
SEQAN_FUNC typename Size< ContainerView<TContainer, Resizable<TSpec> > >::Type
resize(ContainerView<TContainer, Resizable<TSpec> > & me, TSize new_length, TValue /* val */, Tag<TExpand>)
{
    SEQAN_ASSERT_LEQ(new_length, (TSize)capacity(me));

    me._end = me._begin + new_length;

    return length(me);
}

template <typename TContainer, typename TSpec, typename TSize, typename TExpand>
SEQAN_FUNC typename Size< ContainerView<TContainer, Resizable<TSpec> > >::Type
resize(ContainerView<TContainer, Resizable<TSpec> > & me, TSize new_length, Tag<TExpand> tag)
{
    return resize(me, new_length, Nothing(), tag);
}

// NOTE(esiragusa): It is not necessary to overload it, but otherwise I had to qualify the generic one :(
template <typename TContainer, typename TSpec, typename TSize>
SEQAN_FUNC typename Size< ContainerView<TContainer, Resizable<TSpec> > >::Type
resize(ContainerView<TContainer, Resizable<TSpec> > & me, TSize new_length)
{
    typedef ContainerView<TContainer, Resizable<TSpec> >    TView;

    return resize(me, new_length, Nothing(), typename DefaultOverflowImplicit<TView>::Type());
}

// ----------------------------------------------------------------------------
// Function infix()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec, typename TPosBegin, typename TPosEnd>
SEQAN_FUNC typename Infix<ContainerView<TContainer, TSpec> >::Type
infix(ContainerView<TContainer, TSpec> & view, TPosBegin pos_begin, TPosEnd pos_end)
{
    return typename Infix<ContainerView<TContainer, TSpec> >::Type(view._begin + pos_begin, view._begin + pos_end);
}

template <typename TContainer, typename TSpec, typename TPosBegin, typename TPosEnd>
SEQAN_FUNC typename Infix<ContainerView<TContainer, TSpec> const>::Type
infix(ContainerView<TContainer, TSpec> const & view, TPosBegin pos_begin, TPosEnd pos_end)
{
    return typename Infix<ContainerView<TContainer, TSpec> const>::Type(view._begin + pos_begin, view._begin + pos_end);
}

// TODO(esiragusa): infix of view pointer?
//template <typename T, typename TPosBegin, typename TPosEnd>
//inline typename Infix<T *>::Type
//infix(T * t, TPosBegin pos_begin, TPosEnd pos_end)
//{
//    return typename Infix<T *>::Type (t, pos_begin, pos_end);
//}

// ----------------------------------------------------------------------------
// Function assign()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec, typename TOtherContainer>
inline void assign(ContainerView<TContainer, TSpec> & view, TOtherContainer const & cont)
{
    view._begin = begin(cont, Standard());
    view._end = end(cont, Standard());
}

template <typename TContainer, typename TSpec, typename TOtherContainer>
inline void assign(ContainerView<TContainer, Resizable<TSpec> > & view, TOtherContainer const & cont)
{
    view._begin = begin(cont, Standard());
    view._end = end(cont, Standard());
    view._capacity = capacity(cont, Standard());
}

// ----------------------------------------------------------------------------
// Function appendValue()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec, typename TValue, typename TExpand>
SEQAN_FUNC void
appendValue(ContainerView<TContainer, Resizable<TSpec> > & view, TValue const & _value, Tag<TExpand> tag)
{
    resize(view, length(view) + 1, tag);
    value(view, length(view) - 1) = _value;
}

// NOTE(esiragusa): It is not necessary to overload it, but otherwise I had to qualify the generic one :(
template <typename TContainer, typename TSpec, typename TValue>
SEQAN_FUNC void
appendValue(ContainerView<TContainer, Resizable<TSpec> > & view, TValue const & _value)
{
    typedef ContainerView<TContainer, Resizable<TSpec> >    TView;

    appendValue(view, _value, typename DefaultOverflowImplicit<TView>::Type());
}
// ----------------------------------------------------------------------------
// Function clear()
// ----------------------------------------------------------------------------

template <typename TContainer, typename TSpec>
SEQAN_FUNC void
clear(ContainerView<TContainer, Resizable<TSpec> > & view)
{
    resize(view, 0, Exact());
}

// ----------------------------------------------------------------------------
// Operator<<
// ----------------------------------------------------------------------------

template <typename TStream, typename TContainer, typename TSpec>
inline TStream &
operator<<(TStream & target, ContainerView<TContainer, TSpec> const & source)
{
    write(target, source);
    return target;
}

// ----------------------------------------------------------------------------
// Pipe interface
// ----------------------------------------------------------------------------

//template < typename TContainer,
//           typename TSpec,
//           typename TInput,
//           typename TPipeSpec >
//inline void assign(ContainerView<TContainer, TSpec> &dest, Pipe<TInput, TPipeSpec> &src)
//{
//    typedef typename Iterator<ContainerView<TContainer, TSpec>, Standard>::Type TDestIter;
//    resize(dest, length(src));
//    beginRead(src);
//    for (TDestIter _cur = begin(dest, Standard()), _end = end(dest, Standard()); _cur != _end; ++_cur, ++src)
//        *_cur = *src;
//    endRead(src);
//}
//
//template < typename TContainer,
//           typename TSpec,
//           typename TInput,
//           typename TPipeSpec >
//inline void operator << (ContainerView<TContainer, TSpec> &dest, Pipe<TInput, TPipeSpec> &src)
//{
//    assign(dest, src);
//}

}  // namespace seqan

#endif  // #ifndef SEQAN_EXTRAS_SEQUENCE_CONTAINER_VIEW_H