//=========
// File:    AllocateIterator.hpp
// Author:  Shane Neph & Scott Kuehn
// Date:    Tue Aug 28 09:36:24 PDT 2007
// Project: Allocate, no-destroy iterator to pointer type
// ID: $Id:$
//=========




// Macro Guard
#ifndef SPECIAL_ALLOCATE_NEW_ITERATOR_HPP
#define SPECIAL_ALLOCATE_NEW_ITERATOR_HPP

#include <iterator>
#include <cstddef>
#include <cstdio>

namespace Ext {

template <class _Tp>
class allocate_iterator;

template <class _Tp>
class allocate_iterator<_Tp*> {

public:
  typedef std::input_iterator_tag  iterator_category;
  typedef _Tp*                     value_type;
  typedef std::ptrdiff_t           difference_type;
  typedef _Tp**                    pointer;
  typedef _Tp*&                    reference;

  allocate_iterator() : _M_ok(false), fp_(NULL), _M_value(0) {}
  allocate_iterator(FILE* fp) /* this ASSUMES fp is open and meaningful */
    : _M_ok(fp && !std::feof(fp)), fp_(fp), _M_value(_M_ok ? new _Tp(fp) : 0)
    {
      _M_ok = (_M_ok && fp && !std::feof(fp));
      if ( !_M_ok && fp_ )
        fp_ = NULL;
    }

  reference operator*() { return _M_value; }
  pointer operator->() { return &(operator*()); }

  allocate_iterator& operator++() { 
    if ( _M_ok ) {
      _M_value = (new _Tp(fp_));
      _M_ok = !std::feof(fp_);
    }
    return *this;
  }

  allocate_iterator operator++(int)  {
    allocate_iterator __tmp = *this;
    if ( _M_ok ) {
      _M_value = (new _Tp(fp_));
      _M_ok = !std::feof(fp_);
    }
    return __tmp;
  }

  bool _M_equal(const allocate_iterator& __x) const
    { return ( (_M_ok == __x._M_ok) && (!_M_ok || fp_ == __x.fp_) ); }


private:
  bool _M_ok;
  FILE* fp_;
  _Tp* _M_value;
};

template <class _Tp>
inline bool 
operator==(const allocate_iterator<_Tp>& __x,
           const allocate_iterator<_Tp>& __y) {
  return __x._M_equal(__y);
}

template <class _Tp>
inline bool 
operator!=(const allocate_iterator<_Tp>& __x,
           const allocate_iterator<_Tp>& __y) {
  return !__x._M_equal(__y);
}

} // namespace Ext

#endif // SPECIAL_ALLOCATE_NEW_ITERATOR_HPP