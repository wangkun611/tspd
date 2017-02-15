//////////////////////////////////////////////////////////////////////////
// aprallocator.h : 
//
//	Author:		Eric Wang
//	E-mail:		bulepaper@msn.com
//
//
//	This code may be used in compiled form in any way you desire. This
//	file may be redistributed unmodified by any means PROVIDING it is 
//	not sold for profit without the authors written consent, and 
//	providing that this notice and the authors name and all copyright 
//	notices remains intact. 
//
//	An email letting me know how you are using it would be nice as well. 
//
//	This file is provided "as is" with no expressed or implied warranty.
//	The author accepts no liability for any damage/loss of business that
//	this product may cause.
//
////////////////////////////////////////////////////////////////////////////
#ifndef __APRALLOCATOR_H__
#define __APRALLOCATOR_H__


typedef struct apr_pool_t apr_pool_t;

template<typename _T1, typename _T2>
void _Construct(_T1 _FARQ *_P, const _T2& _V)
{new ((void _FARQ *)_P) _T1(_V); }
inline void _Destroy(char _FARQ *_P)
{}

template <typename _Ty>
class aprStdAllocator
{
public:
	explicit aprStdAllocator(apr_pool_t* p):pool(p)
	{
	}
public:
	typedef _SIZT size_type;
	typedef _PDFT difference_type;
	typedef _Ty _FARQ *pointer;
	typedef const _Ty _FARQ *const_pointer;
	typedef _Ty _FARQ& reference;
	typedef const _Ty _FARQ& const_reference;
	typedef _Ty value_type;
	pointer address(reference _X) const
	{
		return (&_X);
	}
	const_pointer address(const_reference _X) const
	{
		return (&_X); 
	}
	pointer Allocate(_PDFT _N, _Ty _FARQ *)
	{
		if (pool)
		{
			return (pointer)apr_palloc(pool, (_SIZT)_N * sizeof (_Ty));
		}
		return NULL;
	}
	pointer allocate(size_type _N, const void *)
	{
		return (Allocate((difference_type)_N, (pointer)0));
	}
	char _FARQ *_Charalloc(size_type _N)
	{
		return (Allocate((difference_type)_N,
			(char _FARQ *)0)); 
	}
	void deallocate(void _FARQ *_P, size_type)
	{
		if (pool)
		{
		}
	}
	
	void construct(pointer _P, const _Ty& _V)
	{
		_Construct(_P, _V);
	}
	void destroy(pointer _P)
	{
		_Destroy(_P);
	}
	_SIZT max_size() const
	{
		_SIZT _N = (_SIZT)(-1) / sizeof (_Ty);
		return (0 < _N ? _N : 1); 
	}
	bool operator==(aprStdAllocator<_Ty>& rhs)
	{
		return pool == rhs.pool	;
	}
protected:
	apr_pool_t* pool;
};



#endif //__APRALLOCATOR_H__