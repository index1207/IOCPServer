#pragma once
#include "Types.h"

template<class... T>
struct TypeList;

template<class T, class U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

template<class T, class... U>
struct TypeList<T, U...>
{
	using Head = T;
	using Tail = TypeList<U...>;
};

template<class T>
struct Length;

template<>
struct Length<TypeList<>>
{
	enum { value = 0 };
};

template<class T, class... U>
struct Length<TypeList<T, U...>>
{
	enum { value = 1 + Length<TypeList<U...>>::value };
};

template<class TL, int32 index>
struct TypeAt;

template<class Head, class... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0>
{
	using Type = Head;
};

template<class Head, class... Tail, int32 index>
struct TypeAt<TypeList<Head, Tail...>, index>
{
	using Type = typename TypeAt<TypeList<Tail...>, index - 1>::Type;
};

template<class TL, class T>
struct IndexOf;

template<class T>
struct IndexOf<TypeList<>, T>
{
	enum { value = -1 };
};

template<class... Tail, class T>
struct IndexOf<TypeList<T, Tail...>, T>
{
	enum { value = 0 };
};

template<class Head, class... Tail, class T>
class IndexOf<TypeList<Head, Tail...>, T>
{
	enum { t = IndexOf<TypeList<Tail...>, T>::value };
public:
	enum { value = t == -1 ? -1 : t + 1 };
};

template<class From, class To>
class Convertion
{
	//���� �ٸ� ������ �־� �����ϱ� ����
	using Small = __int8;
	using Big = int32;

	// ĳ������ �����ϴٸ� SmallŸ���� ����
	static Small Test(const To&) { }

	// ĳ������ �ȵȴٸ� BigŸ���� ����
	static Big Test(...) { }

	// FromŸ���� �ν��Ͻ��� ����
	static From MakeFrom() { }
public:
	enum { exist = sizeof(Test(MakeFrom())) == sizeof(Small) };
};

template<int32 v>
struct Int
{ 
	enum { value = v };
};

template<class TL>
struct TypeConvertion
{
	enum { length = Length<TL>::value };
public:
	TypeConvertion()
	{
		MakeConvertion(Int<0>(), Int<0>());
	}

	template<int32 i, int32 j>
	static void MakeConvertion(Int<i>, Int<j>)
	{
		using From = typename TypeAt<TL, i>::Type;
		using To = typename TypeAt<TL, j>::Type;

		if (Convertion<const From*, const To*>::exist)
			convertion_table[i][j] = true;
		else
			convertion_table[i][j] = false;

		MakeConvertion(Int<i>(), Int<j + 1>());
	}

	template<int32 i>
	static void MakeConvertion(Int<i>, Int<length>)
	{
		MakeConvertion(Int<i + 1>(), Int<0>());
	}

	template<int32 j>
	static void MakeConvertion(Int<length>, Int<j>)
	{
	}

	static inline bool CanConvert(int32 from, int32 to)
	{
		static TypeConvertion conv;
		return convertion_table[from][to];
	}
public:
	static bool convertion_table[length][length];
};

template<class TL>
bool TypeConvertion<TL>::convertion_table[length][length];

template<class To, class From>
To checked_cast(From* ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConvertion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_cast<To>(ptr);

	return nullptr;
}

template<class To, class From>
shared_ptr<From> checked_cast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConvertion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_pointer_cast<To>(ptr);

	return nullptr;
}


template<typename To, typename From>
bool CanCast(From* ptr)
{
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;

	if (TypeConvertion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return true;

	return false;
}

template<typename To, typename From>
bool CanCast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;

	if (TypeConvertion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return true;

	return false;
}

#define CASTABLE_CLASS using TL = TL; int32 _typeId;
#define INIT_ID(type) _typeId = IndexOf<TL, type>::value;