//  System-H - A C++ library inspired by the .Net Framework
//	Copyright(C) 2021 Kris De Greve
//
//	This program is free software : you can redistribute it and /or modify
//	it under the terms of the GNU Affero General Public License as
//	published by the Free Software Foundation, either version 3 of the
//	License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//	GNU Affero General Public License for more details.
//
//	You should have received a copy of the GNU Affero General Public License
//	along with this program.If not, see < https://www.gnu.org/licenses/>.

#ifndef System_H
#define System_H

#ifdef __GNUC__
#define tlocal thread_local
#else
#define tlocal __declspec(thread)
#endif

/*/ // switch static-dynamic, also change this in project properties

// __declspec(dllexport) tells the linker that you want this object to be made available for other DLL's to import. It is used when creating a DLL that others can link to.
// __declspec(dllimport) imports the implementation from a DLL so your application can use it.
// Two different use cases:
// dllexport: You are defining a class implementation within a dll. You want another program to use the class. Here you use dllexport as you are creating a class that you wish the dll to expose.
// dllimport: You are using a function provided by a dll. You include a header supplied with the dll. Here the header uses dllimport to bring in the implementation to be used by the current program.
// Often the same header file is used in both cases and a macro defined. The build configuration defines the macro to be import or export depending which it needs.

#ifdef WIN32
#ifdef SYSTEM_EXPORTS
#define System_API __declspec(dllexport)
#else
#define System_API __declspec(dllimport)
#endif
#else
#define System_API
#endif
/*/
#define System_API 
/**/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <locale>
#include <vector>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <assert.h>
#include <functional>
#include <stdint.h>
#include <algorithm>
#include <string>
#include <codecvt>
#include <queue>
#include <regex>

#if UINTPTR_MAX == 0xffffffff
/* 32-bit */
#define size_t uint32_t
#define ptrshifter 2
#define ptrsize 4
#elif UINTPTR_MAX == 0xffffffffffffffff
/* 64-bit */
#define size_t uint64_t
#define ptrshifter 3
#define ptrsize 8
#else
/* wtf */
#endif


#ifdef __GNUC__
#ifdef ARM
		//#include <arm_neon.h>
	#define __lzcnt32 __builtin_clz
	#define PAUSE() __yield();
#elif defined(ESP32)
	//#include "AsyncUDP.h"
	//using ESP32_IPAddress = IPAddress;
	extern "C" {
		#include "lwip/opt.h"
		#include "lwip/inet.h"
		#include "lwip/udp.h"
		#include "lwip/tcp.h"
		#include "lwip/igmp.h"
		#include "lwip/ip_addr.h"
		#include "lwip/mld6.h"
		#include "lwip/prot/ethernet.h"
		#include <esp_err.h>
		#include <esp_wifi.h>
	}
	#include "lwip/priv/tcpip_priv.h"

	#define __lzcnt32 __builtin_clz
	#define PAUSE() yield();
	bool InterlockedCompareExchange128(long long* Destination, long long ExchangeHigh, long long ExchangeLow, long long* ComparandResult) {
		// TODO
		return false;
	}
#elif defined __MINGW32__
	#include <x86intrin.h>
	#include <Winsock2.h>
	#include <ws2tcpip.h>
	#include <Windows.h>
	#define PAUSE() __builtin_ia32_pause()
	//typedef __int128 int128_t;
	//typedef unsigned __int128 uint128_t;
	#undef Yield // get rid of 'evil' macros

#else

// x86-64 linux, most likely
#define __lzcnt32 __builtin_clz
#define PAUSE() Yield();

#endif
#endif


#if defined(_MSC_VER)
	#define PAUSE() _mm_pause()
	#define sprintf sprintf_s	
	#include <intrin.h>
	#include <vcruntime_exception.h>
//	#include <fileapi.h>
	#define __lzcnt32 __lzcnt	
	#include <Winsock2.h>
	#include <ws2tcpip.h>
	#include <Windows.h>
	#include <psapi.h>

	#undef Yield // get rid of 'evil' macros
#else
	#include <dirent.h>
#endif


#define short int16_t
#define int int32_t
#define long int64_t
#define byte uint8_t
#define ushort uint16_t
#define uint uint32_t
#define ulong uint64_t
#define string System::String
//#define null 0

//#define override virtual

#if defined(__clang__)
#if __has_feature(cxx_rtti)
#define RTTI_ENABLED
#endif
#elif defined(__GNUC__)
#if defined(__GXX_RTTI)
#define RTTI_ENABLED
#endif
#elif defined(_MSC_VER)
#if defined(_CPPRTTI)
#define RTTI_ENABLED
#endif
#endif


namespace System
{
	class Helpers {
	private:

		static int getFirstSetBitPos(uint n)
		{
			byte b = 0;

			if ((n & 0xFFFF0000) != 0)
			{
				b += 16;
				n >>= 16;
			}
			if ((n & 0xFF00) != 0)
			{
				b += 8;
				n >>= 8;
			}
			if ((n & 0xF0) != 0)
			{
				b += 4;
				n >>= 4;
			}
			if ((n & 0xC) != 0)
			{
				b += 2;
				n >>= 2;
			}
			if ((n & 0x2) != 0)
			{
				b += 1;
			}

			return b;
		}

		static int getFirstSetBitPos(ulong n)
		{
			int b = 0;

			if ((n & 0xFFFFFFFF00000000) != 0)
			{
				b += 32;
				n >>= 32;
			}
			b += getFirstSetBitPos((uint)n);

			return b;
		}

	public:
		static uint GetMSBPos(uint n)
		{
			//#ifdef WIN32
			//	return 31 - __lzcnt(n);
			//#endif
			return getFirstSetBitPos(n);
		}

		static uint GetMSBPos(ulong n)
		{
		//	ulong ret = getFirstSetBitPos(n);
			//#ifdef WIN32
			//	return 63 - __lzcnt64(n);
			//#endif
			return getFirstSetBitPos(n);
		}

		//static uint GetLSBPos(uint n)
		//{
		//	n = n ^ (n - 1);
		//	return getFirstSetBitPos(n);
		//}

		//static uint GetLSBPos(ulong n)
		//{
		//	n = n ^ (n - 1);
		//	return getFirstSetBitPos(n);
		//}

		//// returns the smallest power-of-2 number greater than or equal to the given number
		//static uint GetPowerOf2(uint x)
		//{
		//	x -= 1;
		//	x |= x >> 1;
		//	x |= x >> 2;
		//	x |= x >> 4;
		//	x |= x >> 8;
		//	x |= x >> 16;

		//	return x + 1;
		//}

		//// returns the smallest power-of-2 number greater than or equal to the given number
		//static ulong GetPowerOf2(ulong x)
		//{
		//	x -= 1;
		//	x |= x >> 1;
		//	x |= x >> 2;
		//	x |= x >> 4;
		//	x |= x >> 8;
		//	x |= x >> 16;
		//	x |= x >> 32;

		//	return x + 1;
		//}
	};
}
#ifndef System_MemoryPool_H
#define System_MemoryPool_H

//#include "stdafx.h"
//#include <intrin.h>

namespace System
{
	class System_API SimpleFixedSizeMemoryPool
	{
	private:
		byte* head;// recycle stack
		std::atomic<byte*> headTS;
	public:
		size_t SizeObj;
		size_t SizePtr;
		size_t Size;
		size_t Count;



		SimpleFixedSizeMemoryPool() : head(nullptr), headTS(nullptr), SizeObj(0), SizePtr(sizeof(byte*)), Size(0), Count(0) {
		}

		~SimpleFixedSizeMemoryPool() {
			Collect();
		}

		void Create(size_t SizeObj) {
			this->SizeObj = SizeObj;
			this->Size = SizePtr + SizePtr + SizeObj; // we store a pointer to this pool/linked list pointer + ref counter + the actual data
		}

		byte* Get(size_t const & size)
		{
			if (!SizeObj)
				Create(size);

			byte* ret;

			if (head)
			{
				ret = head;
				byte** tmp = (byte**)(ret - SizePtr - SizePtr);
				head = *tmp;
				SimpleFixedSizeMemoryPool** sfsmp = (SimpleFixedSizeMemoryPool**)tmp;
				*sfsmp = this;
				return ret;
			}
			byte* tmpheadts = headTS.load(std::memory_order_relaxed);
			if (tmpheadts) {
				ret = tmpheadts;
				while (!headTS.compare_exchange_weak(tmpheadts, NULL, std::memory_order_acquire, std::memory_order_relaxed)) {
					ret = tmpheadts;
				}
				byte** tmp = (byte**)(ret - SizePtr - SizePtr);
				head = *tmp; // we know head is null bc threadlocal
				SimpleFixedSizeMemoryPool** sfsmp = (SimpleFixedSizeMemoryPool**)tmp;
				*sfsmp = this;
				return ret;
			}

			ret = (byte*)malloc(Size);
			if (ret)
			{
				if (((size_t) ret) & 3) { // make sure the new pointer is a multiple of 4 so we can use the 2 LSBs for tagged pointers, if not we want to know about it -> throw an exception
					std::bad_alloc exception;
					throw exception;
				}

				SimpleFixedSizeMemoryPool** sfsmp = (SimpleFixedSizeMemoryPool**)ret;
				*sfsmp = this;
				ret += SizePtr;
				size_t* szt = (size_t*)ret;
				*szt = (size_t)0;
				ret += SizePtr;

				++Count;
				return ret;
			}
			else {
				std::bad_alloc exception;
				throw exception;
			}
		}

		void Put(byte* value)
		{
			*((byte**)(value - SizePtr - SizePtr)) = head;
			head = value;
		}

		void PutTS(byte* value)
		{
			byte* ptrheadts = headTS.load(std::memory_order_relaxed);
			do {
				*((byte**)(value - SizePtr - SizePtr)) = ptrheadts;
			} while (!headTS.compare_exchange_weak(ptrheadts, value, std::memory_order_release, std::memory_order_relaxed));
		}

		size_t Collect()
		{
			if (!SizeObj)
				return 0;

			byte* cur = head; // take a local copy
			while (cur)
			{
				byte* ptr = *((byte**)(cur - SizePtr - SizePtr));
				byte* tmp = cur - SizePtr - SizePtr;
				free(tmp);
				cur = ptr;
				--Count;
			}
			head = NULL;

			cur = headTS.load();
			if (cur) {
				while (!headTS.compare_exchange_weak(cur, NULL)) {
				}
				while (cur)
				{
					byte* ptr = *((byte**)(cur - SizePtr - SizePtr));
					byte* tmp = cur - SizePtr - SizePtr;
					free(tmp);
					cur = ptr;
					--Count;
				}
			}

			return Count;
		}

	};

	class System_API SimpleVariableSizeMemoryPool {
	private:

	public:
#ifdef ESP32
		static tlocal SimpleFixedSizeMemoryPool* simplepools; // [sizeof(byte*) << 3] ;
		static tlocal bool simplepoolsinited;
#else
		static tlocal SimpleFixedSizeMemoryPool simplepools[sizeof(byte*) << 3] ;
#endif
		~SimpleVariableSizeMemoryPool()
		{
		}

		byte* Get(const size_t& size)
		{
#ifdef ESP32
			if (!simplepoolsinited) {
				simplepools = (SimpleFixedSizeMemoryPool*)malloc(sizeof(SimpleFixedSizeMemoryPool) * (sizeof(byte*) << 3));
				SimpleFixedSizeMemoryPool* tmp = simplepools;
				for (int i = 0; i < (sizeof(byte*) << 3); i++) {
					new (tmp) SimpleFixedSizeMemoryPool();
					tmp++;
				}
				simplepoolsinited = true;
			}
#endif
			int poolindex;
			if (sizeof(byte*) == 8) {
				poolindex = 31 - (int) __lzcnt32((int)size + 8 + 8 - 1);
			}
			else {
				poolindex = 31 - __lzcnt32((int)size + 4 + 4 - 1);
			}
			SimpleFixedSizeMemoryPool* pool = &simplepools[poolindex];
			byte* ret = pool->Get((size_t) (1ULL << (poolindex + 1)));

			return ret;
		}


		void Put(byte* value)
		{
			SimpleFixedSizeMemoryPool** ptr = (SimpleFixedSizeMemoryPool**)(value - sizeof(byte*)-sizeof(byte*));
			if (*ptr >= &simplepools[0] && *ptr <= &simplepools[sizeof(byte*) << 3]) {
				(*ptr)->Put(value);
			}
			else {
				(*ptr)->PutTS(value);
			}
		}

		// thread local!
		size_t Collect() {
			size_t remaining = 0;
			for (size_t i = 0; i < (sizeof(byte*) << 3); i++)
			{
				remaining += simplepools[i].Collect();
			}

			return remaining;
		}
	};

#ifndef SYSTEM_EXPORTS
#ifdef ESP32
	tlocal SimpleFixedSizeMemoryPool* SimpleVariableSizeMemoryPool::simplepools;// [sizeof(byte*) << 3] ;
	tlocal bool SimpleVariableSizeMemoryPool::simplepoolsinited;
#else
	tlocal SimpleFixedSizeMemoryPool SimpleVariableSizeMemoryPool::simplepools[sizeof(byte*) << 3] ;
#endif
#endif



}


#endif

namespace System
{
	class String;
	class Exception;
	namespace Collections {
		namespace Generic {
			template<class T> class System_API IEnumerable;
			template<class T> class System_API Array;
			template<class T> class System_API List;
		}
	}



	template<typename T>
	struct identity { typedef T type; };



	template<class R, class T, R(T::* GetMethod)() const> class System_API PropGenGet
	{
	private:
		T* target;

		void Init(T* target)
		{
			this->target = target;
		}
	public:

		PropGenGet() {
			target = nullptr;
		}

		PropGenGet(T* target)
		{
			Init(target);
		}

		operator R() const {
			return (target->*GetMethod)();
		}

		R operator->() const {
			return (target->*GetMethod)();
		}

		bool operator!=(R const & other) const {
			return other != (target->*GetMethod)();
		}
	};

	template<class R, R(GetMethod)()> class System_API PropGenGetStatic
	{
	public:

		operator R() {
			return (GetMethod)();
		}
	};

	template<class R, class I, class T, R& (T::* GetMethod)()> class System_API PropGenGetArray
	{
	private:
		T* target;

		void Init(T* target)
		{
			this->target = target;
		}
	public:
		PropGenGetArray(T* target) {
			Init(target);
		}


		operator R& () {
			return (target->*GetMethod)();
		}

		I& operator[](int index)
		{
			return (target->*GetMethod)()[index];
		}
	};

	template<class R, class S, class T, R& (T::* GetMethod)() const, void (T::* SetMethodL)(const S&), void (T::* SetMethodR)(S&&)> class System_API PropGen
	{
	private:
		T* target;

		void Init(T* target)
		{
			this->target = target;
		}
	public:
		PropGen() {
			this->target = nullptr;
		}

		PropGen(T* target) {
			Init(target);
		}

		operator R& () const {
			return (target->*GetMethod)();
		}

		R& operator->() const {
			return (target->*GetMethod)();
		}

		// lvalue setter
		void operator=(S const & value) {
			(target->*SetMethodL)(value);
		}

		//rvalue setter
		void operator=(S&& value) {
			(target->*SetMethodR)((S&&)value);
		}

		void operator=(std::nullptr_t const& value) {
			(target->*SetMethodL)(value);
		}

		bool operator!=(R const& other) const {
			return other != (target->*GetMethod)();
		}

		bool operator==(R const& other) const {
			return other == (target->*GetMethod)();
		}
	};


	template<class R, class S, class T, R(T::* GetMethod)(size_t), void (T::* SetMethod)(size_t, S)> class System_API IndexerGen
	{
	private:
		T* target;
		size_t index;
	public:
		IndexerGen() {}

		IndexerGen(const IndexerGen& other) {
			this->target = other.target;
			this->index = other.index;
		}

		void Init(T* target, size_t index)
		{
			this->target = target;
			this->index = index;
		}

		operator R() {
			return (target->*GetMethod)(index);
		}

		void operator= (S value) {
			(target->*SetMethod)(index, value);
		}
	};

	//#ifndef SYSTEM_EXPORTS
	SimpleVariableSizeMemoryPool MPool;
	//#endif

	template <typename T = char>
	inline T* AlignUp(void* pointer, intptr_t mask) {
		intptr_t value = reinterpret_cast<intptr_t>(pointer);
		value += (-value) & mask;
		return reinterpret_cast<T*>(value);
	}


}

#if ESP32
bool UseMemoryPool = false;
#endif

void* operator new(size_t size) //, const std::nothrow_t& nothrow_value) noexcept
//throw ()      // Don't let NULL return throw an exception
{
#if ESP32
	if (!UseMemoryPool) {
		return (void*)malloc(size);
	}
#endif

	void* ret = (void*)System::MPool.Get(size);
	return ret;
}

void operator delete(void* mem)
{
#if ESP32
	if (!UseMemoryPool) {
		free(mem);
		return;
	}
#endif
	System::MPool.Put((byte*)mem);
}



void* operator new[](size_t size)//, const std::nothrow_t& nothrow_value) noexcept
//throw ()      // Don't let NULL return throw an exception
{
#if ESP32
	if (!UseMemoryPool) {
		return (void*)malloc(size);
	}
#endif
	size_t* ret = (size_t*)System::MPool.Get(size + ptrsize);
	*ret = size;
	ret++;
	return (void*)ret;
}

void operator delete[](void* mem)
{
#if ESP32
	if (!UseMemoryPool) {
		free(mem);
		return;
	}
#endif
	size_t* ptrmem = (size_t*)mem;
	ptrmem--;

	System::MPool.Put((byte*)ptrmem);
}

namespace System {

	template<class T> class Out {
	private:
		T& base;
	public:
		explicit Out(T& obj) : base(obj) {}
		T& operator*() { return base; }
		T* operator->() { return &base; }
		operator T() const { return base; }
		T& operator=(T& newval) {
			base = newval;
			return base;
		}

		T& operator=(T&& newval) {
			base = std::move(newval);
			return base;
		}
	};

	template<class T> Out<T> out(T& obj) {
		return Out<T>(obj);
	}


	template<class T> class Ref {
	private:
		T& base;
	public:
		explicit Ref(T& obj) : base(obj) {}
		T& operator*() const { return base; }
		T* operator->() const { return &base; }
		operator T() const { return base; }
		T& operator=(T& newval) {
			base = newval;
			return base;
		}

		T& operator=(T&& newval) {
			base = std::move(newval);
			return base;
		}
	};

	template<class T> Ref<T> ref(T& obj) {
		return Ref<T>(obj);
	}

#define null nullptr

	class System_API Object
	{
	private:

#ifdef RTTI_ENABLED
		template<class R> bool is(identity<R> dummy) const {
			if (!this->od)
				return false;
			typename R::ObjectData* dd = dynamic_cast<typename R::ObjectData*>(this->od);
			return dd != nullptr;
		}

		bool is(identity<short> dummy) const;
		bool is(identity<int> dummy) const;
		bool is(identity<long> dummy) const;

		template<class R> R as(identity<R> dummy) const {
			return R(dynamic_cast<typename R::ObjectData*>(this->od));
		}

		short as(identity<short> dummy) const;
		int as(identity<int> dummy) const;
		long as(identity<long> dummy) const;
#endif

		//	protected:
		//		template<class R> R* GetNewObjectData();

	public:
		class System_API ObjectData
		{
		protected:

		public:
			ObjectData(size_t refcnt) {
				// we assume there is a size_t in front of the actual data which we'll use as a reference counter i.e. smart pointers
				size_t* ptr = ((size_t*)this) - 1;
				*ptr = refcnt;
			}

			ObjectData() : ObjectData(1) {

			}

			// for value types...
			virtual ObjectData* DeepCopy() {
				return nullptr;
			}

			virtual ~ObjectData()
			{
				//		std::cout << "destroy called in objectd, od = " << (size_t)this << std::endl;
			}

			virtual int GetHashCode() {
				size_t szthis = (size_t)this;
				int ret = (int)szthis;
				if (sizeof(byte*) == 8) {
					ret ^= (int)(((ulong)szthis) >> 32);
				}
				return ret;
			}

			virtual String ToString() const;
		};

		mutable ObjectData* od = nullptr;

		Object() : od(nullptr) {
		}

		Object(std::nullptr_t const & n) : od(nullptr) {
		}

		Object(Object const & other) : od(other.od) {
			if (od) {
				size_t refcnt = *(((size_t*)od) - 1);
				if (refcnt == 0) {
					// value type, copy
					od = other.od->DeepCopy();
				}
				else {
					AddRef();
				}
			}
		}

		Object(Object&& other) noexcept : od(other.od) {
			other.od = null;
		}

		~Object() {
			Release();
		}

		Object(Object* pValue) : od(pValue->od) {
			if (!od)
				od = new ObjectData();
			::operator delete((void*)pValue);
		}

		Object(ObjectData* pValue) : od(pValue) {
			AddRef();
		}

		/// <summary>Increments the reference counter of this Object.</summary>
		/// <returns></returns>
		size_t AddRef() const
		{
			if (!od)
				return 0;
			std::atomic<size_t>* ato = (std::atomic<size_t>*) (((size_t*)od) - 1);
			size_t ret = ++(*ato);

			return ret;
		}

		/// <summary>Decrements the reference counter of this Object.</summary>
		/// <returns></returns>
		size_t Release() const
		{
			if (!od)
				return 0;

			size_t* ptr = (((size_t*)od) - 1);
			std::atomic<size_t>* ato = (std::atomic<size_t>*) ptr;
			if (ato->load() == 1)
			{
				delete od;
				return 0;
			}
			size_t ret = --(*ato);
			if (!ret) {
				delete od;
			}

			return ret;
		}

		/// <summary>Gets the current value of the reference counter of this Object.</summary>
		/// <returns>The current value of the reference counter.</returns>
		size_t GetRef() const {
			size_t* ptr = (((size_t*)od) - 1);
			return *ptr;
		}

		/// <summary>Sets the reference counter of this Object.</summary>
		/// <param name="value">The value to assign to the reference counter.</param>
		/// <returns></returns>
		void SetRef(size_t value) const {
			size_t* ptr = (((size_t*)od) - 1);
			*ptr = value;
		}

		Object& operator=(std::nullptr_t const& other) {
			Release();
			this->od = null;

			return *this;
		}

		Object& operator=(Object const& sp) {
			if (this->od == sp.od) // Avoid self assignment
				return *this;
			Release();
			od = sp.od;
			AddRef();

			return *this;
		}

		Object& operator=(Object&& sp) noexcept {
			if (this->od == sp.od) // Avoid self assignment
				return *this;
			Release();
			od = sp.od;
			sp.od = nullptr;

			return *this;
		}

		bool operator==(std::nullptr_t const& other) const {
			return (this->od == nullptr);
		}

		bool operator==(Object const& other) const {
			//	std::cout << "operator overloaded == " << std::endl;
			return (this->od == other.od);
		}

		bool operator!=(std::nullptr_t const& other) const {
			return !operator==(other);
		}

		bool operator!=(Object const& other) const {
			return !operator==(other);
		}

		//bool operator!=(const int other) {
		//	return !operator==(other);
		//}

		Object* operator->() {
			return this;
		}

		int GetHashCode();

		String ToString() const;

#ifdef RTTI_ENABLED
		template<class R> bool is() const {
			return is(identity<R>());
		}

		template<class R> R as() const {
			return as(identity<R>());
		}
#endif

				Object(short value);
		explicit operator short();
		Object& operator=(short value);
		bool operator==(const short other);
		bool operator!=(const short other) { return !operator==(other);}

					Object(int value);
		explicit operator int();
		Object& operator=(int value);
		bool operator==(const int other);
		bool operator!=(const int other) { return !operator==(other);}

					Object(long value);
		explicit operator long();
		Object& operator=(long value);
		bool operator==(const long other);
		bool operator!=(const long other) { return !operator==(other);}

	};


	
	
	class System_API String : public Object
	{
	private:
		int GetLength() const {
			ObjectData* dd = (ObjectData*)(od);
			return dd->Length;
		}

		void InternalReplace(String& str, const String& oldValue, const String& newValue, int startpos, int totlen) const {

			ObjectData* sd = (ObjectData*)(this->od);


			int ovpos = IndexOf(oldValue, startpos);
			if (ovpos > 0) {
				ObjectData* sdo = (ObjectData*)(oldValue.od);
				ObjectData* sdn = (ObjectData*)(newValue.od);
				InternalReplace(str, oldValue, newValue, ovpos + sdo->Length, totlen + ovpos - startpos + sdn->Length);
				ObjectData* sdstr = (ObjectData*)(str.od);
				char16_t* ptr = (char16_t*)(((byte*)sdstr) + sizeof(ObjectData));
				memcpy(ptr + totlen, ((byte*)sd) + sizeof(ObjectData) + (startpos << 1), ((size_t)ovpos - startpos) << 1);
				memcpy(ptr + totlen + ovpos - startpos, ((byte*)sdn) + sizeof(ObjectData), (size_t)sdn->Length << 1);
			}
			else {
				ObjectData* sdstr = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)totlen + Length - startpos + 1) << 1));
				new (sdstr) ObjectData();
				char16_t* ptr = (char16_t*)(((byte*)sdstr) + sizeof(ObjectData));

				memcpy(ptr + totlen, ((byte*)sd) + sizeof(ObjectData) + (startpos << 1), ((size_t)Length - startpos) << 1);
				ptr[totlen + sd->Length - startpos] = 0;
				sdstr->Length = totlen + sd->Length - startpos;
				str.od = sdstr;
			}
		}

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:
			void Clear()
			{
				Length = 0;
			}
		public:
			int Length = 0;
			//char16_t* chrs = nullptr;


			virtual ~ObjectData() {
				Clear();
			}

			//override String ToString() {
			//	return NULL;
			//}

			int GetHashCode() override {
				// TODO: consider randomized hashing to prevent 'hash flooding' (DOS type) attacks
				//uint ret = 0;
				if (Length == 0)
					return 0;
				char16_t* ptr = (char16_t*)(((byte*)this) + sizeof(ObjectData));

				std::u16string wstr(ptr, Length);
				size_t str_hash = std::hash<std::u16string>{}(wstr);
				if (sizeof(byte*) == 8)
				{
					str_hash ^= (((ulong)str_hash) >> 32);
				}

				return (int)str_hash;
			}


		};

		PropGenGet<int, String, &String::GetLength> Length{ this };

		String() {
		}

		static int utf8_to_utf16(const char* value, int count, char* dst) {

			char16_t* sdchrs = (char16_t*)dst;


			int len16 = 0;
			for (int i = 0; i < count; i++) {
				unsigned char ch = (unsigned char)value[i];
				int todo = 0;
				int uni;
				if (ch <= 0x7F) {
					uni = ch;
					todo = 0;
				}
				else if (ch <= 0xBF) {
					// ignore or throw?
				}
				else if (ch <= 0xDF) {
					uni = ch & 0x1F;
					todo = 1;
				}
				else if (ch <= 0xEF) {
					uni = ch & 0x0F;
					todo = 2;
				}
				else if (ch <= 0xF7) {
					uni = ch & 0x07;
					todo = 3;
				}
				else {
					// ignore or throw?
				}
				for (int j = 0; j < todo; j++) {
					i++;
					if (i == count) {
						// ignore or throw?
					}
					else {
						ch = value[i];
						if (ch < 0x80 || ch > 0xBF) {
							// ignore or throw?
						}
						else {
							uni <<= 6;
							uni += ch & 0x3F;
						}
					}
				}
				if ((uni >= 0xD800 && uni <= 0xDFFF) || (uni > 0x10FFFF)) {
					// ignore or throw?
				}
				else {
					if (uni <= 0xFFFF) {
						sdchrs[len16++] = (char16_t)uni;
					}
					else {
						uni -= 0x10000;
						sdchrs[len16++] = (char16_t)((uni >> 10) + 0xD800);
						sdchrs[len16++] = (char16_t)((uni & 0x3FF) + 0xDC00);
					}
				}
			}
			sdchrs[len16] = (char16_t)0;

			return len16;
		}

		static inline size_t utf32_codepoint_utf8_length(char32_t srcChar)
		{
			if (srcChar < 0x00000080) {
				return 1;
			}
			else if (srcChar < 0x00000800) {
				return 2;
			}
			else if (srcChar < 0x00010000) {
				if ((srcChar < 0x0000D800) || (srcChar > 0x0000DFFF)) {
					return 3;
				}
				else {
					return 0;
				}
			}
			else if (srcChar <= 0x0010FFFF) {
				return 4;
			}
			else {
				return 0;
			}
		}

		static void utf32_codepoint_to_utf8(uint8_t* dstP, char32_t srcChar, size_t bytes)
		{
			const char32_t kFirstByteMark[] = {
0x00000000, 0x00000000, 0x000000C0, 0x000000E0, 0x000000F0
			};

			dstP += bytes;
			switch (bytes)
			{
			case 4: *--dstP = (uint8_t)((srcChar | 0x00000080) & 0x000000BF); srcChar >>= 6;
			case 3: *--dstP = (uint8_t)((srcChar | 0x00000080) & 0x000000BF); srcChar >>= 6;
			case 2: *--dstP = (uint8_t)((srcChar | 0x00000080) & 0x000000BF); srcChar >>= 6;
			case 1: *--dstP = (uint8_t)(srcChar | kFirstByteMark[bytes]);
			}
		}

		static int utf16_to_utf8(char16_t const* src, int src_len, char* dst) {

			if (src == NULL || src_len == 0 || dst == NULL) {
				return 0;
			}
			const char16_t* cur_utf16 = src;
			const char16_t* const end_utf16 = src + src_len;
			char* cur = dst;
			while (cur_utf16 < end_utf16) {
				char32_t utf32;
				if ((*cur_utf16 & 0xFC00) == 0xD800 && (cur_utf16 + 1) < end_utf16
					&& (*(cur_utf16 + 1) & 0xFC00) == 0xDC00) {
					utf32 = (*cur_utf16++ - 0xD800) << 10;
					utf32 |= *cur_utf16++ - 0xDC00;
					utf32 += 0x10000;
				}
				else {
					utf32 = (char32_t)*cur_utf16++;
				}
				const size_t len = utf32_codepoint_utf8_length(utf32);
				utf32_codepoint_to_utf8((uint8_t*)cur, utf32, len);
				cur += len;
			}
			*cur = '\0';

			return (int)(cur - dst);
		}




		String(char const* value) {
			// assume value is UTF8
			/*/
			int len = (int)std::strlen(value);
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
			std::u16string wstr = converter.from_bytes(value, value + len);

			const char16_t* chrs16 = wstr.data();
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((wstr.length() + 1) << 1));
			new (sd) ObjectData();
			sd->Length = (int32_t)wstr.length();
			//sd->chrs = (char16_t*)MPool.Get(((uint64_t)sd->Length + 1) << 1);

			char16_t* ptr = (char16_t*)(((byte*)sd) + sizeof(ObjectData));
			memcpy(ptr, chrs16, ((uint64_t)sd->Length) << 1);
			ptr[sd->Length] = 0;

			this->od = sd;
			/*/
			int len = (int)std::strlen(value);
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + 1) << 1));
			new (sd) ObjectData();
			sd->Length = utf8_to_utf16(value, len, ((char*)sd) + sizeof(ObjectData));

			this->od = sd;

			/**/
		}

		String(char const* value, int count) {
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((count + 1) << 1));
			new (sd) ObjectData();
			sd->Length = utf8_to_utf16(value, count, ((char*)sd) + sizeof(ObjectData));

			this->od = sd;
		}


		String(char16_t const* value) {
			int len = std::wcslen((wchar_t const*)value);
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)(len)+1) << 1));
			new (sd) ObjectData();
			sd->Length = len;
			memcpy(((byte*)sd) + sizeof(ObjectData), value, (len + 1) << 1);
			this->od = sd;


			//char16_t const* ptr = value;
			//while (*ptr) {
			//	ptr++;
			//};
			//ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)(ptr - value) + 1) << 1));
			//new (sd) ObjectData();
			//sd->Length = (int32_t)(ptr - value);
			//memcpy(((byte*)sd) + sizeof(ObjectData), value, ((size_t)sd->Length + 1) << 1);
			//this->od = sd;
		}

		String(char16_t const* value, int len) {
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)(len)+1) << 1));
			new (sd) ObjectData();
			sd->Length = len;
			memcpy(((byte*)sd) + sizeof(ObjectData), value, (len + 1) << 1);
			this->od = sd;


			//char16_t const* ptr = value;
			//while (*ptr) {
			//	ptr++;
			//};
			//ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)(ptr - value) + 1) << 1));
			//new (sd) ObjectData();
			//sd->Length = (int32_t)(ptr - value);
			//memcpy(((byte*)sd) + sizeof(ObjectData), value, ((size_t)sd->Length + 1) << 1);
			//this->od = sd;
		}

		String(bool const value) : String(value ? u"True" : u"False") {
		}

		String(String* pValue) {
			this->od = pValue->od;
			pValue->od = nullptr;

			delete pValue;
		}

		String(int const value) {
			char tempStr[32] = { 0 };
			sprintf(tempStr, "%d", value);

			int len = (int)std::strlen(tempStr);
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + 1) << 1));
			new (sd) ObjectData();
			sd->Length = utf8_to_utf16(tempStr, len, ((char*)sd) + sizeof(ObjectData));

			this->od = sd;
		}

		String(uint const value) {
			char tempStr[32] = { 0 };
			sprintf(tempStr, "%u", value);

			int len = (int)std::strlen(tempStr);
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + 1) << 1));
			new (sd) ObjectData();
			sd->Length = utf8_to_utf16(tempStr, len, ((char*)sd) + sizeof(ObjectData));

			this->od = sd;
		}

		template<class R, class T, R(T::* GetMethod)() const>
		String(PropGenGet<R, T, GetMethod> const& pgg) : String((R)pgg) {}

		template<class R, class S, class T, R& (T::* GetMethod)() const, void (T::* SetMethodL)(const S&), void (T::* SetMethodR)(S&&)>
		String(PropGen<R, S, T, GetMethod, SetMethodL, SetMethodR> const& pg) : String((R)pg) {}

		String(long const value) {
			char tempStr[32] = { 0 };
			sprintf(tempStr, "%lld", value);

			int len = (int)std::strlen(tempStr);
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + 1) << 1));
			new (sd) ObjectData();
			sd->Length = utf8_to_utf16(tempStr, len, ((char*)sd) + sizeof(ObjectData));

			this->od = sd;
		}

		String(ulong const value) {
			char tempStr[32] = { 0 };
			sprintf(tempStr, "%llu", value);

			int len = (int)std::strlen(tempStr);
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + 1) << 1));
			new (sd) ObjectData();
			sd->Length = utf8_to_utf16(tempStr, len, ((char*)sd) + sizeof(ObjectData));

			this->od = sd;
		}

		String(char16_t const value) {
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + 4);
			new (sd) ObjectData();
			sd->Length = 1;
			char16_t* ptr = (char16_t*)((byte*)sd + sizeof(ObjectData));
			ptr[0] = value;
			ptr[1] = 0;
			this->od = sd;
		}

		String(String const& value) {
			if (value.od != NULL) {
				int len = ((ObjectData*)value.od)->Length;
				ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)len + 1) << 1));
				new (sd) ObjectData();
				sd->Length = len;
				memcpy(((byte*)sd) + sizeof(ObjectData), ((byte*)value.od) + sizeof(ObjectData), ((size_t)len + 1) << 1);
				this->od = sd;
			}
			else {
				this->od = NULL;
			}
		}

		String(String&& value) noexcept {
			this->od = value.od;
			value.od = nullptr;
		}

		String(std::nullptr_t const& other) {
			this->od = null;
		}

		String* operator->() {
			return this;
		}

		String& operator=(String const& o)
		{
			if (o == nullptr) {
				return operator=(nullptr);
			}
			if (this->od) {
				delete this->od;
			}
			int len = o.Length;
			ObjectData* sd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)len + 1) << 1));
			new (sd) ObjectData();

			sd->Length = len;
			memcpy(((byte*)sd) + sizeof(ObjectData), ((byte*)o.od) + sizeof(ObjectData), ((size_t)len + 1) << 1);
			this->od = sd;

			return *this;
		}

		String& operator=(String&& o) noexcept {
			if (o == nullptr) {
				return operator=(nullptr);
			}
			if (this->od == o.od) // Avoid self assignment
				return *this;
			if (this->od)
				delete this->od;
			this->od = o.od;
			o.od = nullptr;

			return *this;
		}


		//String& operator=(const char* value)
		//{
		//	return operator=(String(value));
		//}

		//String& operator=(const char16_t* value)
		//{
		//	return operator=(String(value));
		//}

		String& operator=(std::nullptr_t const& value) {
			if (this->od)
			{
				delete this->od;
				this->od = nullptr;
			}

			return *this;
		}

		//Object& operator=(String* o)
		//{
		//	return operator=(*o);
		//}

		String& operator+=(String const& value) {
			ObjectData* sd = (ObjectData*)(this->od);
			int vlen = ((ObjectData*)value.od)->Length;
			int len = sd->Length;

			ObjectData* nsd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + (size_t)vlen + 1) << 1));
			new (nsd) ObjectData();
			memcpy(((byte*)nsd) + sizeof(ObjectData), ((byte*)sd) + sizeof(ObjectData), (size_t)(len << 1));
			memcpy(((byte*)nsd) + sizeof(ObjectData) + (len << 1), ((byte*)value.od) + sizeof(ObjectData), ((size_t)vlen + 1) << 1);
			nsd->Length = len + vlen;

			delete this->od;
			this->od = nsd;

			return *this;
		}

		String& operator+=(char16_t const value) {
			ObjectData* sd = (ObjectData*)(this->od);

			int len = sd->Length;
			ObjectData* nsd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)len + 1 + 1) << 1));
			new (nsd) ObjectData();
			memcpy(((byte*)nsd) + sizeof(ObjectData), ((byte*)sd) + sizeof(ObjectData), (size_t)len << 1);
			char16_t* ptr = (char16_t*)(((byte*)nsd) + sizeof(ObjectData));
			ptr[len] = value;
			nsd->Length = sd->Length + 1;

			delete this->od;
			this->od = nsd;

			return *this;
		}

		String& operator+=(int const value) {
			return operator+=(String(value));
		}

		String& operator+=(uint const value) {
			return operator+=(String(value));
		}

		String& operator+=(long const value) {
			return operator+=(String(value));
		}

		String& operator+=(ulong const value) {
			return operator+=(String(value));
		}

		bool operator==(String const& value) const
		{
			if (value.od == nullptr || this->od == nullptr)
				return false;
			ObjectData* sd = (ObjectData*)(this->od);
			ObjectData* vsd = (ObjectData*)(value.od);
			if (sd->Length != vsd->Length)
				return false;
			if (sd->Length == 0)
				return true;

			return memcmp(((byte*)vsd) + sizeof(ObjectData), ((byte*)sd) + sizeof(ObjectData), ((size_t)sd->Length) << 1) == 0;
		}

		bool operator==(std::nullptr_t const& other) const {
			return (this->od == null);
		}

		bool operator!=(String const& o) const
		{
			return !(operator==(o));
		}

		bool operator!=(std::nullptr_t const& other) const {
			return (this->od != null);
		}



		char16_t operator[](int const index) const
		{
			char16_t* ptr = (char16_t*)(((byte*)this->od) + sizeof(ObjectData));
			return ptr[index];
		}

		char16_t* ToCharArray() const
		{
			char16_t* ptr = (char16_t*)(((byte*)this->od) + sizeof(ObjectData));
			return ptr;
		}

		int IndexOf(String const& value, int const startIndex = 0) const {
			char16_t* ptr = (char16_t*)(((byte*)this->od) + sizeof(ObjectData));

			std::u16string wstr = std::u16string(ptr, GetLength());
			int ret = (int)wstr.find((char16_t*)(((byte*)value.od) + sizeof(ObjectData)), (size_t)startIndex);

			return ret;
		}

		int LastIndexOf(String const& value) const {
			char16_t* ptr = (char16_t*)(((byte*)this->od) + sizeof(ObjectData));

			std::u16string wstr = std::u16string(ptr, GetLength());
			int ret = (int)wstr.rfind((char16_t*)(((byte*)value.od) + sizeof(ObjectData)));

			return ret;
		}

		bool Contains(String const& value) const {
			return IndexOf(value) >= 0;
		}

		String Substring(int const startIndex, int const length) const {
			char16_t* ptr = (char16_t*)(((byte*)this->od) + sizeof(ObjectData));

			ObjectData* nsd = (ObjectData*) ::operator new(sizeof(ObjectData) + (((size_t)length + 1) << 1));
			new (nsd) ObjectData();

			char16_t* nptr = (char16_t*)(((byte*)nsd) + sizeof(ObjectData));
			std::memcpy(nptr, ptr + startIndex, ((size_t)length) << 1);
			nptr[length] = 0;
			nsd->Length = length;

			String ret{};
			ret.od = nsd;

			return ret;
		}

		String Substring(int const startIndex) const {
			return Substring(startIndex, Length - startIndex);
		}

		String ToLower() const {
			char16_t* ptr = (char16_t*)(((byte*)this->od) + sizeof(ObjectData));
			int len = GetLength();

			ObjectData* nsd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + 1) << 1));
			new (nsd) ObjectData();
			char16_t* nptr = (char16_t*)(((byte*)nsd) + sizeof(ObjectData));
			nsd->Length = len;

			if (Length > 0) {
				std::u16string wstr(ptr, len);
				std::transform(wstr.begin(), wstr.end(), nptr, towlower);
				nptr[len] = 0;
			}

			String ret{};
			ret.od = nsd;

			return ret;
		}

		String ToUpper() const {
			char16_t* ptr = (char16_t*)(((byte*)this->od) + sizeof(ObjectData));
			int len = GetLength();

			ObjectData* nsd = (ObjectData*) ::operator new(sizeof(ObjectData) + ((len + 1) << 1));
			new (nsd) ObjectData();
			char16_t* nptr = (char16_t*)(((byte*)nsd) + sizeof(ObjectData));
			nsd->Length = len;

			if (Length > 0) {
				std::u16string wstr(ptr, len);
				std::transform(wstr.begin(), wstr.end(), nptr, towupper);
				nptr[len] = 0;
			}

			String ret{};
			ret.od = nsd;

			return ret;
		}

		String Replace(String const& oldValue, String const& newValue) const {
			String str;
			InternalReplace(str, oldValue, newValue, 0, 0);
			return str;
		}

		String TrimStart() {
			// TODO: reimplement, needs unicode support...
			int pos = 0;
			char16_t* rawBytes = ToCharArray();
			for (int i = 0; i < Length; i++) {
				char16_t chr = rawBytes[i];
				if (chr == '\r' || chr == '\n' || chr == ' ' || chr == '\t') {
					pos++;
				}
				else {
					break;
				}
			}

			return Substring(pos);
		}

		String TrimEnd() {
			// TODO: reimplement, needs unicode support...
			int cnt = 0;
			char16_t* rawBytes = ToCharArray();
			for (int i = Length; i --> 0;) {
				char16_t chr = rawBytes[i];
				if (chr == '\r' || chr == '\n' || chr == ' ' || chr == '\t') {
					cnt++;
				}
				else {
					break;
				}
			}

			return Substring(0, Length - cnt);
		}

		String Trim() {
			String ret = TrimStart();
			return ret.TrimEnd();
		}



		System::Collections::Generic::Array<string> Split(char const c);
	};



	template<class T> class DefaultGenerator {
		static T def;
	};

#ifndef SYSTEM_EXPORTS
	template<class T> T DefaultGenerator<T>::def{};
#endif // !SYSTEM_EXPORTS


	template<class T> T& _default() {
		return DefaultGenerator<T>::def;
	}

	class Number {
	private:
	public:

		template<class T>
		static String Format(T value, String const& format) {
			char p[] = "%088d";

			char tempStr[32] = { 0 };
			char specifier = (char)format[0];
			if (specifier <= 90) {
				if (specifier == 'X') {
				}
				else {
					specifier += 32;
				}
			}


			switch (format.Length) {
			case 1: {
				p[1] = specifier;
				p[2] = '\0';
				break;
			}
			case 2: {
				p[2] = (char)format[1];
				p[3] = specifier;
				p[4] = '\0';
				break;
			}
			case 3: {
				p[2] = (char)format[1];
				p[3] = (char)format[2];
				p[4] = specifier;
				break;
			}
			};

			if (specifier == 'f') {
				p[1] = '.';
				sprintf(tempStr, p, (double)value);
			}
			else {
				sprintf(tempStr, p, value);
			}

			return tempStr;
		}

		static String FormatInt16(short value, String const& format) {
			return Format<short>(value, format);
		}

		static String FormatInt32(int value, String const& format) {
			return Format<int>(value, format);
		}

		static String FormatInt64(long value, String const& format) {
			return Format<long>(value, format);
		}

		static String FormatDouble(double value, String const& format) {
			return Format<double>(value, format);
		}

	};

	class System_API Int16 {
	private:
		short value;
	public:
		static const short MaxValue = (short)0x7fff;
		static const short MinValue = (short)0x8000;

		class System_API ObjectData : public Object::ObjectData {
		public:
			short value;

			ObjectData(short value) : Object::ObjectData(0), value(value) {
				// activate copy-value by setting the reference count to 0
			}

			int GetHashCode() override {
				uint ret = (uint)value;
				return *reinterpret_cast<int*>(&ret);
			}

			Object::ObjectData* DeepCopy() override {
				return new ObjectData(value);
			}

			string ToString() const override {
				return System::Number::FormatInt16(value, "D");
			}

		};

		Int16(short const value) : value(value) {}

		String ToString(String const& format) const {
			return System::Number::FormatInt16(value, format);
		}

	};

	class System_API Int32 {
	private:
		int value;
	public:
		static const int MaxValue = 0x7fffffff;
		static const int MinValue = 0x80000000;

		class System_API ObjectData : public Object::ObjectData {
		public:
			int value;

			ObjectData(int value) : Object::ObjectData(0), value(value) {
				// activate copy-value by setting the reference count to 0
			}

			int GetHashCode() override {
				uint ret = (uint)value;
				return *reinterpret_cast<int*>(&ret);
			}

			Object::ObjectData* DeepCopy() override {
				return new ObjectData(value);
			}

			string ToString() const override {
				return System::Number::FormatInt32(value, "D");
			}

		};

		Int32(int const value) : value(value) {}

		String ToString(String const& format) const {
			return Number::FormatInt32(value, format);
		}

		static bool TryParse(string s, Out<int> result) {
			// TODO: Performance!
			int res = 0;
			char16_t* rawBytes = s.ToCharArray();
			for (int i = 0; i < s.Length; i++) {
				char16_t chr = rawBytes[i];
				if (chr < 48 || chr > 57)
					return false;
				res = (res * 10) + (chr - 48);
			}

			result = res;
			return true;
		}


	};

	class System_API Int64 {
	private:
		long value;
	public:
		static long const MaxValue = 0x7fffffffffffffffll; // 9223372036854775807;
		static long const MinValue = 0x8000000000000000ll; // -9223372036854775808;

		class System_API ObjectData : public Object::ObjectData {
		public:
			long value;

			ObjectData(long value) : Object::ObjectData(0), value(value) {
				// activate copy-value by setting the reference count to 0
			}

			int GetHashCode() override {
				uint ret = (uint)value;
				ret ^= (uint)(value >> 32);
				return *reinterpret_cast<int*>(&ret);
			}

			Object::ObjectData* DeepCopy() override {
				return new ObjectData(value);
			}

			string ToString() const override {
				return System::Number::FormatInt64(value, "D");
			}
		};

		Int64(long value) : value(value) {}

		String ToString(String const & format) const {
			return Number::FormatInt64(value, format);
		}
	};

	class System_API IDisposable {
	public:
		virtual void Dispose() {}
	};


	class System_API GC : public Object
	{
	public:
		static size_t Collect() {
			size_t remaining = MPool.Collect();
			return remaining;
		}
	};

	class System_API Environment : public Object::ObjectData {
	private:
		static int Get_ProcessorCount() {
			return std::thread::hardware_concurrency();
		}

		static long StartTicks;

		static long Get_RealTickCount() {
			std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
			long nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();

			return (621355968000000000 + (nanos / 100));
		}

		static int Get_TickCount() {
			return (int)((Get_RealTickCount() - StartTicks) / 10000);
		}

	public:
		static PropGenGetStatic<int, &Environment::Get_ProcessorCount> ProcessorCount;
		static PropGenGetStatic<int, &Environment::Get_TickCount> TickCount;
	};

#ifndef SYSTEM_EXPORTS
	PropGenGetStatic<int, &Environment::Get_ProcessorCount> Environment::ProcessorCount{};
	PropGenGetStatic<int, &Environment::Get_TickCount> Environment::TickCount{};
	long Environment::StartTicks{ Environment::Get_RealTickCount() };
#endif


	String operator+(String const & c1, String const & c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String const & c1, char16_t const c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String const & c1, char const c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String const & c1, int const c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String const & c1, uint const c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String const & c1, long const c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String const & c1, ulong const c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String && c1, ulong const c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String&& c1, String const & c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	bool operator<(String const & lhs, String const & rhs)
	{
		if (lhs == null || rhs == null)
			return false;
		int len = rhs.Length;
		if (len > lhs.Length) {
			len = lhs.Length;
		}
		char16_t* ptr = (char16_t*)(((byte*)lhs.od) + sizeof(String::ObjectData));
		char16_t* ptr2 = (char16_t*)(((byte*)rhs.od) + sizeof(String::ObjectData));
		for (int i = 0; i < len; i++) {
			if (*ptr != *ptr2) {
				return *ptr < *ptr2;
			}
		}

		return false;
	}

	String operator+(String&& c1, const char16_t c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	String operator+(String&& c1, const char c2)
	{
		String ret(c1);
		ret += c2;
		return ret;
	}

	class System_API Exception : public Object, std::exception {
	public:
		class System_API ObjectData : public Object::ObjectData {
		public:
			String Message;

			String ToString() const override {
				return Message;
			}
		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Exception(){}

		Exception(std::nullptr_t const & n) : System::Object(n) {
		}

		Exception(Exception* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Exception(Exception const & other) : System::Object(other) { }

		Exception(Exception&& other) noexcept : System::Object(std::move(other)) { }

		Exception(Object::ObjectData* other) : System::Object(other) {
		}

		Exception& operator=(Exception const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Exception& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Exception& operator=(Exception&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Exception& operator=(Exception* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Exception* operator->() {
			return this;
		}



			Exception(String const& str) {
			ObjectData* ed = new ObjectData();
			ed->Message = str;
			od = ed;
		}

		Exception(String&& str) {
			ObjectData* ed = new ObjectData();
			ed->Message = str;
			od = ed;
		}
	};

	class System_API NullReferenceException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		NullReferenceException(){}

		NullReferenceException(std::nullptr_t const & n) : Exception(n) {
		}

		NullReferenceException(NullReferenceException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		NullReferenceException(NullReferenceException const & other) : Exception(other) { }

		NullReferenceException(NullReferenceException&& other) noexcept : Exception(std::move(other)) { }

		NullReferenceException(Object::ObjectData* other) : Exception(other) {
		}

		NullReferenceException& operator=(NullReferenceException const & other) {
			Exception::operator=(other);
			return *this;
		}

		NullReferenceException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		NullReferenceException& operator=(NullReferenceException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		NullReferenceException& operator=(NullReferenceException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		NullReferenceException* operator->() {
			return this;
		}



			NullReferenceException(System::String const& str) : Exception(str) {

		}

		NullReferenceException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API NotImplementedException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		NotImplementedException(){}

		NotImplementedException(std::nullptr_t const & n) : Exception(n) {
		}

		NotImplementedException(NotImplementedException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		NotImplementedException(NotImplementedException const & other) : Exception(other) { }

		NotImplementedException(NotImplementedException&& other) noexcept : Exception(std::move(other)) { }

		NotImplementedException(Object::ObjectData* other) : Exception(other) {
		}

		NotImplementedException& operator=(NotImplementedException const & other) {
			Exception::operator=(other);
			return *this;
		}

		NotImplementedException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		NotImplementedException& operator=(NotImplementedException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		NotImplementedException& operator=(NotImplementedException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		NotImplementedException* operator->() {
			return this;
		}



			NotImplementedException(System::String const& str) : Exception(str) {

		}

		NotImplementedException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API IndexOutOfRangeException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		IndexOutOfRangeException(){}

		IndexOutOfRangeException(std::nullptr_t const & n) : Exception(n) {
		}

		IndexOutOfRangeException(IndexOutOfRangeException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		IndexOutOfRangeException(IndexOutOfRangeException const & other) : Exception(other) { }

		IndexOutOfRangeException(IndexOutOfRangeException&& other) noexcept : Exception(std::move(other)) { }

		IndexOutOfRangeException(Object::ObjectData* other) : Exception(other) {
		}

		IndexOutOfRangeException& operator=(IndexOutOfRangeException const & other) {
			Exception::operator=(other);
			return *this;
		}

		IndexOutOfRangeException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		IndexOutOfRangeException& operator=(IndexOutOfRangeException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		IndexOutOfRangeException& operator=(IndexOutOfRangeException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		IndexOutOfRangeException* operator->() {
			return this;
		}



			IndexOutOfRangeException(System::String const& str) : Exception(str) {

		}

		IndexOutOfRangeException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API TimeoutException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		TimeoutException(){}

		TimeoutException(std::nullptr_t const & n) : Exception(n) {
		}

		TimeoutException(TimeoutException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		TimeoutException(TimeoutException const & other) : Exception(other) { }

		TimeoutException(TimeoutException&& other) noexcept : Exception(std::move(other)) { }

		TimeoutException(Object::ObjectData* other) : Exception(other) {
		}

		TimeoutException& operator=(TimeoutException const & other) {
			Exception::operator=(other);
			return *this;
		}

		TimeoutException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		TimeoutException& operator=(TimeoutException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		TimeoutException& operator=(TimeoutException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		TimeoutException* operator->() {
			return this;
		}



			TimeoutException(System::String const& str) : Exception(str) {

		}

		TimeoutException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API InvalidOperationException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		InvalidOperationException(){}

		InvalidOperationException(std::nullptr_t const & n) : Exception(n) {
		}

		InvalidOperationException(InvalidOperationException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		InvalidOperationException(InvalidOperationException const & other) : Exception(other) { }

		InvalidOperationException(InvalidOperationException&& other) noexcept : Exception(std::move(other)) { }

		InvalidOperationException(Object::ObjectData* other) : Exception(other) {
		}

		InvalidOperationException& operator=(InvalidOperationException const & other) {
			Exception::operator=(other);
			return *this;
		}

		InvalidOperationException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		InvalidOperationException& operator=(InvalidOperationException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		InvalidOperationException& operator=(InvalidOperationException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		InvalidOperationException* operator->() {
			return this;
		}



			InvalidOperationException(System::String const& str) : Exception(str) {

		}

		InvalidOperationException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API ArgumentNullException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		ArgumentNullException(){}

		ArgumentNullException(std::nullptr_t const & n) : Exception(n) {
		}

		ArgumentNullException(ArgumentNullException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		ArgumentNullException(ArgumentNullException const & other) : Exception(other) { }

		ArgumentNullException(ArgumentNullException&& other) noexcept : Exception(std::move(other)) { }

		ArgumentNullException(Object::ObjectData* other) : Exception(other) {
		}

		ArgumentNullException& operator=(ArgumentNullException const & other) {
			Exception::operator=(other);
			return *this;
		}

		ArgumentNullException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		ArgumentNullException& operator=(ArgumentNullException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		ArgumentNullException& operator=(ArgumentNullException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		ArgumentNullException* operator->() {
			return this;
		}



			ArgumentNullException(System::String const& str) : Exception(str) {

		}

		ArgumentNullException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API ArgumentOutOfRangeException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		ArgumentOutOfRangeException(){}

		ArgumentOutOfRangeException(std::nullptr_t const & n) : Exception(n) {
		}

		ArgumentOutOfRangeException(ArgumentOutOfRangeException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		ArgumentOutOfRangeException(ArgumentOutOfRangeException const & other) : Exception(other) { }

		ArgumentOutOfRangeException(ArgumentOutOfRangeException&& other) noexcept : Exception(std::move(other)) { }

		ArgumentOutOfRangeException(Object::ObjectData* other) : Exception(other) {
		}

		ArgumentOutOfRangeException& operator=(ArgumentOutOfRangeException const & other) {
			Exception::operator=(other);
			return *this;
		}

		ArgumentOutOfRangeException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		ArgumentOutOfRangeException& operator=(ArgumentOutOfRangeException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		ArgumentOutOfRangeException& operator=(ArgumentOutOfRangeException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		ArgumentOutOfRangeException* operator->() {
			return this;
		}



			ArgumentOutOfRangeException(System::String const& str) : Exception(str) {

		}

		ArgumentOutOfRangeException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API OutOfMemoryException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		OutOfMemoryException(){}

		OutOfMemoryException(std::nullptr_t const & n) : Exception(n) {
		}

		OutOfMemoryException(OutOfMemoryException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		OutOfMemoryException(OutOfMemoryException const & other) : Exception(other) { }

		OutOfMemoryException(OutOfMemoryException&& other) noexcept : Exception(std::move(other)) { }

		OutOfMemoryException(Object::ObjectData* other) : Exception(other) {
		}

		OutOfMemoryException& operator=(OutOfMemoryException const & other) {
			Exception::operator=(other);
			return *this;
		}

		OutOfMemoryException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		OutOfMemoryException& operator=(OutOfMemoryException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		OutOfMemoryException& operator=(OutOfMemoryException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		OutOfMemoryException* operator->() {
			return this;
		}



			OutOfMemoryException(System::String const& str) : Exception(str) {

		}

		OutOfMemoryException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API FormatException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		FormatException(){}

		FormatException(std::nullptr_t const & n) : Exception(n) {
		}

		FormatException(FormatException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		FormatException(FormatException const & other) : Exception(other) { }

		FormatException(FormatException&& other) noexcept : Exception(std::move(other)) { }

		FormatException(Object::ObjectData* other) : Exception(other) {
		}

		FormatException& operator=(FormatException const & other) {
			Exception::operator=(other);
			return *this;
		}

		FormatException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		FormatException& operator=(FormatException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		FormatException& operator=(FormatException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		FormatException* operator->() {
			return this;
		}



			FormatException(System::String const& str) : Exception(str) {

		}

		FormatException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API OverflowException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		OverflowException(){}

		OverflowException(std::nullptr_t const & n) : Exception(n) {
		}

		OverflowException(OverflowException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		OverflowException(OverflowException const & other) : Exception(other) { }

		OverflowException(OverflowException&& other) noexcept : Exception(std::move(other)) { }

		OverflowException(Object::ObjectData* other) : Exception(other) {
		}

		OverflowException& operator=(OverflowException const & other) {
			Exception::operator=(other);
			return *this;
		}

		OverflowException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		OverflowException& operator=(OverflowException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		OverflowException& operator=(OverflowException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		OverflowException* operator->() {
			return this;
		}



			OverflowException(System::String const& str) : Exception(str) {

		}

		OverflowException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	class System_API InvalidCastException : public Exception {
	public:
		class System_API ObjectData : public Exception::ObjectData {

		};
				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		InvalidCastException(){}

		InvalidCastException(std::nullptr_t const & n) : Exception(n) {
		}

		InvalidCastException(InvalidCastException* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		InvalidCastException(InvalidCastException const & other) : Exception(other) { }

		InvalidCastException(InvalidCastException&& other) noexcept : Exception(std::move(other)) { }

		InvalidCastException(Object::ObjectData* other) : Exception(other) {
		}

		InvalidCastException& operator=(InvalidCastException const & other) {
			Exception::operator=(other);
			return *this;
		}

		InvalidCastException& operator=(std::nullptr_t const & n) {
			Exception::operator=(n);
			return *this;
		}

		InvalidCastException& operator=(InvalidCastException&& other) noexcept {
			Exception::operator=(std::move(other));
			return *this;
		}

		InvalidCastException& operator=(InvalidCastException* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		InvalidCastException* operator->() {
			return this;
		}



			InvalidCastException(System::String const& str) : Exception(str) {

		}

		InvalidCastException(System::String&& str) noexcept : Exception((System::String&&)str) {

		}
	};

	//template<class R> R* Object::GetNewObjectData() {
	//	R* ptr = new R();
	//	if (!ptr)
	//		throw OutOfMemoryException();
	//	return ptr;
	//}

	int Object::GetHashCode() {
		if (od)
			return od->GetHashCode();
		else {
			throw NullReferenceException();
		}
	}

	String Object::ObjectData::ToString() const {
		return String(u"");
	}

	String Object::ToString() const {
		if (this->od)
			return this->od->ToString();
		return String(u"");
	}

#ifdef RTTI_ENABLED
		Object::Object(short value) {
		od = new Int16::ObjectData(value);
	}

	Object::operator short() {
		if (!od)
			throw NullReferenceException();
		Int16::ObjectData* dd = dynamic_cast<Int16::ObjectData*>(od);
		if (!dd)
			throw InvalidCastException();
		return dd->value;
	}

	Object& Object::operator=(short value) {
		if (this->od) {
			Int16::ObjectData* dd = dynamic_cast<Int16::ObjectData*>(this->od);
			if (dd) {
				dd->value = value;
				return *this;
			}
		}
		Release();
		this->od = new Int16::ObjectData(value);
		return *this;
	}

	bool Object::operator==(const short other) {
		Int16::ObjectData* iod = dynamic_cast<Int16::ObjectData*>(od);
		return iod && iod->value == other;
	}

	bool Object::is(identity<short> dummy) const {
		if (!this->od)
			return false;
		Int16::ObjectData* dd = dynamic_cast<Int16::ObjectData*>(this->od);
		return dd != nullptr;
	}

	short Object::as(identity<short> dummy) const {
		if (!this->od)
			throw NullReferenceException();
		Int16::ObjectData* dd = dynamic_cast<Int16::ObjectData*>(this->od);
		if (!dd)
			throw InvalidCastException();
		return dd->value;
	}


			Object::Object(int value) {
		od = new Int32::ObjectData(value);
	}

	Object::operator int() {
		if (!od)
			throw NullReferenceException();
		Int32::ObjectData* dd = dynamic_cast<Int32::ObjectData*>(od);
		if (!dd)
			throw InvalidCastException();
		return dd->value;
	}

	Object& Object::operator=(int value) {
		if (this->od) {
			Int32::ObjectData* dd = dynamic_cast<Int32::ObjectData*>(this->od);
			if (dd) {
				dd->value = value;
				return *this;
			}
		}
		Release();
		this->od = new Int32::ObjectData(value);
		return *this;
	}

	bool Object::operator==(const int other) {
		Int32::ObjectData* iod = dynamic_cast<Int32::ObjectData*>(od);
		return iod && iod->value == other;
	}

	bool Object::is(identity<int> dummy) const {
		if (!this->od)
			return false;
		Int32::ObjectData* dd = dynamic_cast<Int32::ObjectData*>(this->od);
		return dd != nullptr;
	}

	int Object::as(identity<int> dummy) const {
		if (!this->od)
			throw NullReferenceException();
		Int32::ObjectData* dd = dynamic_cast<Int32::ObjectData*>(this->od);
		if (!dd)
			throw InvalidCastException();
		return dd->value;
	}


			Object::Object(long value) {
		od = new Int64::ObjectData(value);
	}

	Object::operator long() {
		if (!od)
			throw NullReferenceException();
		Int64::ObjectData* dd = dynamic_cast<Int64::ObjectData*>(od);
		if (!dd)
			throw InvalidCastException();
		return dd->value;
	}

	Object& Object::operator=(long value) {
		if (this->od) {
			Int64::ObjectData* dd = dynamic_cast<Int64::ObjectData*>(this->od);
			if (dd) {
				dd->value = value;
				return *this;
			}
		}
		Release();
		this->od = new Int64::ObjectData(value);
		return *this;
	}

	bool Object::operator==(const long other) {
		Int64::ObjectData* iod = dynamic_cast<Int64::ObjectData*>(od);
		return iod && iod->value == other;
	}

	bool Object::is(identity<long> dummy) const {
		if (!this->od)
			return false;
		Int64::ObjectData* dd = dynamic_cast<Int64::ObjectData*>(this->od);
		return dd != nullptr;
	}

	long Object::as(identity<long> dummy) const {
		if (!this->od)
			throw NullReferenceException();
		Int64::ObjectData* dd = dynamic_cast<Int64::ObjectData*>(this->od);
		if (!dd)
			throw InvalidCastException();
		return dd->value;
	}


#endif



		class System_API Uri : public Object {
		private:
		public:
			static bool IsHexDigit(char16_t character) {
				return ((character >= '0') && (character <= '9'))
					|| ((character >= 'A') && (character <= 'F'))
					|| ((character >= 'a') && (character <= 'f'));
			}
	};


	class System_API IPv4AddressHelper {
	public:
		static const long Invalid = -1;
		static const long MaxIPv4Value = (uint)0xffffffff;
		static const int Decimal = 10;
		static const int Octal = 8;
		static const int Hex = 16;
		static const int NumberOfLabels = 4;
		//
// IsValidCanonical
//
//  Checks if the substring is a valid canonical IPv4 address or an IPv4 address embedded in an IPv6 literal
//  This is an attempt to parse ABNF productions from RFC3986, Section 3.2.2:
//     IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
//     IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
//     dec-octet   = DIGIT                 ; 0-9
//                 / %x31-39 DIGIT         ; 10-99
//                 / "1" 2DIGIT            ; 100-199
//                 / "2" %x30-34 DIGIT     ; 200-249
//                 / "25" %x30-35          ; 250-255
//
		static bool IsValidCanonical(char16_t* name, int start, Ref<int> end, bool allowIPv6, bool notImplicitFile) {
			int dots = 0;
			int number = 0;
			bool haveNumber = false;
			bool firstCharIsZero = false;

			while (start < end) {
				char16_t ch = name[start];
				if (allowIPv6) {
					// for ipv4 inside ipv6 the terminator is either ScopeId, prefix or ipv6 terminator
					if (ch == ']' || ch == '/' || ch == '%') break;
				}
				else if (ch == '/' || ch == '\\' || (notImplicitFile && (ch == ':' || ch == '?' || ch == '#'))) {
					break;
				}

				if (ch <= '9' && ch >= '0') {
					if (!haveNumber && (ch == '0')) {
						if ((start + 1 < end) && name[start + 1] == '0') {
							// 00 is not allowed as a prefix.
							return false;
						}

						firstCharIsZero = true;
					}

					haveNumber = true;
					number = number * 10 + (name[start] - '0');
					if (number > 255) {
						return false;
					}
				}
				else if (ch == '.') {
					if (!haveNumber || (number > 0 && firstCharIsZero)) {
						// 0 is not allowed to prefix a number.
						return false;
					}
					++dots;
					haveNumber = false;
					number = 0;
					firstCharIsZero = false;
				}
				else {
					return false;
				}
				++start;
			}
			bool res = (dots == 3) && haveNumber;
			if (res) {
				end = start;
			}
			return res;
		}

		// Parse any canonical or noncanonical IPv4 formats and return a long between 0 and MaxIPv4Value.
		// Return Invalid (-1) for failures.
		// If the address has less than three dots, only the rightmost section is assumed to contain the combined value for
		// the missing sections: 0xFF00FFFF == 0xFF.0x00.0xFF.0xFF == 0xFF.0xFFFF
		static long ParseNonCanonical(char16_t* name, int start, Ref<int> end, bool notImplicitFile)
		{
			int numberBase = Decimal;
			char16_t ch;
			long parts[4];
			long currentValue = 0;
			bool atLeastOneChar = false;

			// Parse one dotted section at a time
			int dotCount = 0; // Limit 3
			int current = start;
			for (; current < end; current++)
			{
				ch = name[current];
				currentValue = 0;

				// Figure out what base this section is in
				numberBase = Decimal;
				if (ch == '0')
				{
					numberBase = Octal;
					current++;
					atLeastOneChar = true;
					if (current < end)
					{
						ch = name[current];
						if (ch == 'x' || ch == 'X')
						{
							numberBase = Hex;
							current++;
							atLeastOneChar = false;
						}
					}
				}

				// Parse this section
				for (; current < end; current++)
				{
					ch = name[current];
					int digitValue;

					if ((numberBase == Decimal || numberBase == Hex) && '0' <= ch && ch <= '9')
					{
						digitValue = ch - '0';
					}
					else if (numberBase == Octal && '0' <= ch && ch <= '7')
					{
						digitValue = ch - '0';
					}
					else if (numberBase == Hex && 'a' <= ch && ch <= 'f')
					{
						digitValue = ch + 10 - 'a';
					}
					else if (numberBase == Hex && 'A' <= ch && ch <= 'F')
					{
						digitValue = ch + 10 - 'A';
					}
					else
					{
						break; // Invalid/terminator
					}

					currentValue = (currentValue * numberBase) + digitValue;

					if (currentValue > MaxIPv4Value) // Overflow
					{
						return Invalid;
					}

					atLeastOneChar = true;
				}

				if (current < end && name[current] == '.')
				{
					if (dotCount >= 3 // Max of 3 dots and 4 segments
						|| !atLeastOneChar // No empty segmets: 1...1
						// Only the last segment can be more than 255 (if there are less than 3 dots)
						|| currentValue > 0xFF)
					{
						return Invalid;
					}
					parts[dotCount] = currentValue;
					dotCount++;
					atLeastOneChar = false;
					continue;
				}
				// We don't get here unless We find an invalid character or a terminator
				break;
			}

			// Terminators
			if (!atLeastOneChar)
			{
				return Invalid;  // Empty trailing segment: 1.1.1.
			}
			else if (current >= end)
			{
				// end of string, allowed
			}
			else if ((ch = name[current]) == '/' || ch == '\\' || (notImplicitFile && (ch == ':' || ch == '?' || ch == '#')))
			{
				end = current;
			}
			else
			{
				// not a valid terminating character
				return Invalid;
			}

			parts[dotCount] = currentValue;

			// Parsed, reassemble and check for overflows
			switch (dotCount)
			{
			case 0: // 0xFFFFFFFF
				if (parts[0] > MaxIPv4Value)
				{
					return Invalid;
				}
				return parts[0];
			case 1: // 0xFF.0xFFFFFF
				if (parts[1] > 0xffffff)
				{
					return Invalid;
				}
				return (parts[0] << 24) | (parts[1] & 0xffffff);
			case 2: // 0xFF.0xFF.0xFFFF
				if (parts[2] > 0xffff)
				{
					return Invalid;
				}
				return (parts[0] << 24) | ((parts[1] & 0xff) << 16) | (parts[2] & 0xffff);
			case 3: // 0xFF.0xFF.0xFF.0xFF
				if (parts[3] > 0xff)
				{
					return Invalid;
				}
				return (parts[0] << 24) | ((parts[1] & 0xff) << 16) | ((parts[2] & 0xff) << 8) | (parts[3] & 0xff);
			default:
				return Invalid;
			}
		}

		static bool IsValid(char16_t* name, int start, Ref<int> end, bool allowIPv6, bool notImplicitFile, bool unknownScheme) {
			// IPv6 can only have canonical IPv4 embedded. Unknown schemes will not attempt parsing of non-canonical IPv4 addresses.
			if (allowIPv6 || unknownScheme)
			{
				return IsValidCanonical(name, start, end, allowIPv6, notImplicitFile);
			}
			else
			{
				return ParseNonCanonical(name, start, end, notImplicitFile) != Invalid;
			}
		}


	};


	class System_API IPv6AddressHelper {
	private:
		static bool InternalIsValid(char16_t* name, int start, Ref<int> end, bool validateStrictAddress) {
			int sequenceCount = 0;
			int sequenceLength = 0;
			bool haveCompressor = false;
			bool haveIPv4Address = false;
			bool havePrefix = false;
			bool expectingNumber = true;
			int lastSequence = 1;

			// Starting with a colon character is only valid if another colon follows.
			if (name[start] == ':' && (start + 1 >= end || name[start + 1] != ':')) { // && ServicePointManager.UseStrictIPv6AddressParsing) {
				return false;
			}

			int i;
			for (i = start; i < end; ++i) {
				if (havePrefix ? (name[i] >= '0' && name[i] <= '9') : Uri::IsHexDigit(name[i])) {
					++sequenceLength;
					expectingNumber = false;
				}
				else {
					if (sequenceLength > 4) {
						return false;
					}
					if (sequenceLength != 0) {
						++sequenceCount;
						lastSequence = i - sequenceLength;
					}
					switch (name[i]) {
					case '%':   while (true) {
						//accept anything in scopeID
						if (++i == end) {
							// no closing ']', fail
							return false;
						}
						if (name[i] == ']') {
							goto explicit_endbracket;
						}
						else if (name[i] == '/') {
							goto explicit_slash;
						}
					}
					case ']':
					explicit_endbracket:
						start = i;
						i = end;
						//this will make i = end+1
						continue;
					case ':':
						if ((i > 0) && (name[i - 1] == ':')) {
							if (haveCompressor) {

								//
								// can only have one per IPv6 address
								//

								return false;
							}
							haveCompressor = true;
							expectingNumber = false;
						}
						else {
							expectingNumber = true;
						}
						break;

					case '/':
					explicit_slash:
						if (validateStrictAddress) {
							return false;
						}
						if ((sequenceCount == 0) || havePrefix) {
							return false;
						}
						havePrefix = true;
						expectingNumber = true;
						break;

					case '.':
						if (haveIPv4Address) {
							return false;
						}

						i = end;
						if (!IPv4AddressHelper::IsValid(name, lastSequence, ref(i), true, false, false)) {
							return false;
						}
						// ipv4 address takes 2 slots in ipv6 address, one was just counted meeting the '.'
						++sequenceCount;
						haveIPv4Address = true;
						--i;            // it will be incremented back on the next loop
						break;

					default:
						return false;
					}
					sequenceLength = 0;
				}
			}

			//
			// if the last token was a prefix, check number of digits
			//

			if (havePrefix && ((sequenceLength < 1) || (sequenceLength > 2))) {
				return false;
			}

			//
			// these sequence counts are -1 because it is implied in end-of-sequence
			//

			int expectedSequenceCount = 8 + (havePrefix ? 1 : 0);

			if (!expectingNumber && (sequenceLength <= 4) && (haveCompressor ? (sequenceCount < expectedSequenceCount) : (sequenceCount == expectedSequenceCount)))
			{
				if (i == end + 1)
				{
					// ']' was found
					end = start + 1;
					return true;
				}
				return false;
			}
			return false;
		}

	public:
		static bool IsValidStrict(char16_t* name, int start, Ref<int> end) {
			return InternalIsValid(name, start, end, true);
		}

	};
}



namespace System {

template<class ... A> class VAction : public System::Object {
public:
	typedef void (ExecuteActionPtr)(System::Object::ObjectData*, A ...);

	class ObjectData : public System::Object::ObjectData {
	public:
		ExecuteActionPtr* exec;
	};

	template<class OT> class ObjectDataOT : public ObjectData {
	public:
		OT obj;
		void(OT::*act)(A ...) const;

		ObjectDataOT(const OT& obj) : obj(obj), act(&OT::operator()) {
			ObjectData::exec = &ExecuteAction;
		}

		static void ExecuteAction(System::Object::ObjectData* od, A ... args){
			ObjectDataOT* ptr = static_cast<ObjectDataOT<OT>*>(od);
			(ptr->obj.*(ptr->act))(args...);
		}

	};

	
	ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

	VAction(){}

	VAction(std::nullptr_t const & n) : System::Object(n) {
	}

	VAction(VAction* pValue) {
		if (!pValue->od) {
			ObjectData* dd = new ObjectData();
			od = dd;
		}
		else {
			od = pValue->od;
			pValue->od = nullptr;
		}
		delete pValue;
	}

	VAction(VAction const & other) : System::Object(other) { }

	VAction(VAction&& other) noexcept : System::Object(std::move(other)) { }

	VAction(Object::ObjectData* other) : System::Object(other) {
	}

	VAction& operator=(VAction const & other) {
		System::Object::operator=(other);
		return *this;
	}

	VAction& operator=(std::nullptr_t const & n) {
		System::Object::operator=(n);
		return *this;
	}

	VAction& operator=(VAction* other) {
		if (od == other->od)
			return *this;
		Release();
		od = other->od;
		::operator delete((void*)other);
		return *this;
	}

	VAction& operator=(VAction&& other) noexcept {
		System::Object::operator=(std::move(other));
		return *this;
	}

	VAction* operator->() {
		return this;
	}


	
	//ExecuteActionPtr* exec;

	template<typename F>
	VAction(const F& f) : Object() {
		ObjectDataOT<F>* d = new ObjectDataOT<F>(f);
		//exec = &ObjectDataOT<F>::ExecuteAction;
		this->od = d;
	}

	void operator() (A ... args) const {
		GOD()->exec(od, args...);
	}

};


template<typename IG0 = void, typename IG1 = void, typename IG2 = void, typename IG3 = void, typename IG4 = void, typename IG5 = void, typename IG6 = void, typename IG7 = void, typename IG8 = void, typename IG9 = void, typename IG10 = void, typename IG11 = void, typename IG12 = void, typename IG13 = void, typename IG14 = void, typename IG15 = void, typename IG16 = void> class System_API Action : public System::Object {};

template<typename IG0 = void, typename IG1 = void, typename IG2 = void, typename IG3 = void, typename IG4 = void, typename IG5 = void, typename IG6 = void, typename IG7 = void, typename IG8 = void, typename IG9 = void, typename IG10 = void, typename IG11 = void, typename IG12 = void, typename IG13 = void, typename IG14 = void, typename IG15 = void, typename IG16 = void, typename IG17 = void> class System_API Func : public System::Object {};

	template<> class System_API Action<void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void()> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void()>(f);
			od = d;
		}

		void operator()() const {
			GOD()->f();
		}
	};



	template<typename TResult> class System_API Func<TResult, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult()> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult()>(f);
			od = d;
		}

		TResult operator()() const {
			return GOD()->f();
		}


	};


	template<typename T0> class System_API Action<T0, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0) const {
			GOD()->f(arg0);
		}
	};



	template<typename T0, typename TResult> class System_API Func<T0, TResult, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0) const {
			return GOD()->f(arg0);
		}


	};


	template<typename T0,typename T1> class System_API Action<T0,T1, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1) const {
			GOD()->f(arg0,arg1);
		}
	};



	template<typename T0,typename T1, typename TResult> class System_API Func<T0,T1, TResult, void, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1) const {
			return GOD()->f(arg0,arg1);
		}


	};


	template<typename T0,typename T1,typename T2> class System_API Action<T0,T1,T2, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2) const {
			GOD()->f(arg0,arg1,arg2);
		}
	};



	template<typename T0,typename T1,typename T2, typename TResult> class System_API Func<T0,T1,T2, TResult, void, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2) const {
			return GOD()->f(arg0,arg1,arg2);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3> class System_API Action<T0,T1,T2,T3, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3) const {
			GOD()->f(arg0,arg1,arg2,arg3);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3, typename TResult> class System_API Func<T0,T1,T2,T3, TResult, void, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3) const {
			return GOD()->f(arg0,arg1,arg2,arg3);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4> class System_API Action<T0,T1,T2,T3,T4, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4, typename TResult> class System_API Func<T0,T1,T2,T3,T4, TResult, void, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5> class System_API Action<T0,T1,T2,T3,T4,T5, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5, TResult, void, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6> class System_API Action<T0,T1,T2,T3,T4,T5,T6, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6, TResult, void, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7, TResult, void, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8, TResult, void, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9, TResult, void, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10, TResult, void, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11, TResult, void, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12, TResult, void, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12,T13 const & arg13) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13, TResult, void, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12,T13 const & arg13) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13,typename T14> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12,T13 const & arg13,T14 const & arg14) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13,typename T14, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14, TResult, void, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12,T13 const & arg13,T14 const & arg14) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14);
		}


	};


	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13,typename T14,typename T15> class System_API Action<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &,T15 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Action(){}

		Action(std::nullptr_t const & n) : System::Object(n) {
		}

		Action(Action* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Action(Action const & other) : System::Object(other) { }

		Action(Action&& other) noexcept : System::Object(std::move(other)) { }

		Action(Object::ObjectData* other) : System::Object(other) {
		}

		Action& operator=(Action const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Action& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Action& operator=(Action* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Action& operator=(Action&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Action* operator->() {
			return this;
		}



		template<class F>
		Action(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<void(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &,T15 const &)>(f);
			od = d;
		}

		void operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12,T13 const & arg13,T14 const & arg14,T15 const & arg15) const {
			GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15);
		}
	};



	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13,typename T14,typename T15, typename TResult> class System_API Func<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15, TResult, void> : public Object
	{
	public:
		class ObjectData : public System::Object::ObjectData {
		public:
			std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &,T15 const &)> f;
		};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Func(){}

		Func(std::nullptr_t const & n) : System::Object(n) {
		}

		Func(Func* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Func(Func const & other) : System::Object(other) { }

		Func(Func&& other) noexcept : System::Object(std::move(other)) { }

		Func(Object::ObjectData* other) : System::Object(other) {
		}

		Func& operator=(Func const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Func& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Func& operator=(Func* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Func& operator=(Func&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Func* operator->() {
			return this;
		}



		template <class F>
		Func(F f) : Object() {
			ObjectData* d = new ObjectData();
			//if (!d)
			//	throw OutOfMemoryException();
			d->f = std::function<TResult(T0 const &,T1 const &,T2 const &,T3 const &,T4 const &,T5 const &,T6 const &,T7 const &,T8 const &,T9 const &,T10 const &,T11 const &,T12 const &,T13 const &,T14 const &,T15 const &)>(f);
			od = d;
		}

		TResult operator()(T0 const & arg0,T1 const & arg1,T2 const & arg2,T3 const & arg3,T4 const & arg4,T5 const & arg5,T6 const & arg6,T7 const & arg7,T8 const & arg8,T9 const & arg9,T10 const & arg10,T11 const & arg11,T12 const & arg12,T13 const & arg13,T14 const & arg14,T15 const & arg15) const {
			return GOD()->f(arg0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15);
		}


	};


} // namespace System

namespace System {
template<typename IG0 = void, typename IG1 = void, typename IG2 = void, typename IG3 = void, typename IG4 = void, typename IG5 = void, typename IG6 = void, typename IG7 = void, typename IG8 = void> class System_API Tuple : public System::Object {};


	template<typename T1> class System_API Tuple<T1, void, void, void, void, void, void, void> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			ObjectData(T1 const & item1) : m_Item1(item1){ }
							ObjectData(T1&& item1) : m_Item1(std::move(item1)){ }
						};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1>, &Tuple<T1>::GetItem1, &Tuple<T1>::SetItem1, &Tuple<T1>::SetItem1> Item1{this};


			Tuple(T1 const & item1) {
				this->od = new ObjectData(item1);
			}
						Tuple(T1&& item1) {
				this->od = new ObjectData(std::move(item1));
			}
			
	};

	template<typename T1, typename T2> class System_API Tuple<T1, T2, void, void, void, void, void, void> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			T2 m_Item2;
			ObjectData(T1 const & item1, T2 const & item2) : m_Item1(item1), m_Item2(item2){ }
							ObjectData(T1 const & item1, T2&& item2) : m_Item1(item1), m_Item2(std::move(item2)){ }
							ObjectData(T1&& item1, T2 const & item2) : m_Item1(std::move(item1)), m_Item2(item2){ }
							ObjectData(T1&& item1, T2&& item2) : m_Item1(std::move(item1)), m_Item2(std::move(item2)){ }
						};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1, T2>, &Tuple<T1, T2>::GetItem1, &Tuple<T1, T2>::SetItem1, &Tuple<T1, T2>::SetItem1> Item1{this};

		T2& GetItem2() const { return GOD()->m_Item2; }
				
		void SetItem2(T2 const & value) { 
			GOD()->m_Item2 = value; 
		}
			
		void SetItem2(T2&& value) { 
			GOD()->m_Item2 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T2, T2, Tuple<T1, T2>, &Tuple<T1, T2>::GetItem2, &Tuple<T1, T2>::SetItem2, &Tuple<T1, T2>::SetItem2> Item2{this};


			Tuple(T1 const & item1, T2 const & item2) {
				this->od = new ObjectData(item1, item2);
			}
						Tuple(T1 const & item1, T2&& item2) {
				this->od = new ObjectData(item1, std::move(item2));
			}
						Tuple(T1&& item1, T2 const & item2) {
				this->od = new ObjectData(std::move(item1), item2);
			}
						Tuple(T1&& item1, T2&& item2) {
				this->od = new ObjectData(std::move(item1), std::move(item2));
			}
			
	};

	template<typename T1, typename T2, typename T3> class System_API Tuple<T1, T2, T3, void, void, void, void, void> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			T2 m_Item2;
			T3 m_Item3;
			ObjectData(T1 const & item1, T2 const & item2, T3 const & item3) : m_Item1(item1), m_Item2(item2), m_Item3(item3){ }
							ObjectData(T1 const & item1, T2 const & item2, T3&& item3) : m_Item1(item1), m_Item2(item2), m_Item3(std::move(item3)){ }
							ObjectData(T1 const & item1, T2&& item2, T3 const & item3) : m_Item1(item1), m_Item2(std::move(item2)), m_Item3(item3){ }
							ObjectData(T1 const & item1, T2&& item2, T3&& item3) : m_Item1(item1), m_Item2(std::move(item2)), m_Item3(std::move(item3)){ }
							ObjectData(T1&& item1, T2 const & item2, T3 const & item3) : m_Item1(std::move(item1)), m_Item2(item2), m_Item3(item3){ }
							ObjectData(T1&& item1, T2 const & item2, T3&& item3) : m_Item1(std::move(item1)), m_Item2(item2), m_Item3(std::move(item3)){ }
							ObjectData(T1&& item1, T2&& item2, T3 const & item3) : m_Item1(std::move(item1)), m_Item2(std::move(item2)), m_Item3(item3){ }
							ObjectData(T1&& item1, T2&& item2, T3&& item3) : m_Item1(std::move(item1)), m_Item2(std::move(item2)), m_Item3(std::move(item3)){ }
						};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1, T2, T3>, &Tuple<T1, T2, T3>::GetItem1, &Tuple<T1, T2, T3>::SetItem1, &Tuple<T1, T2, T3>::SetItem1> Item1{this};

		T2& GetItem2() const { return GOD()->m_Item2; }
				
		void SetItem2(T2 const & value) { 
			GOD()->m_Item2 = value; 
		}
			
		void SetItem2(T2&& value) { 
			GOD()->m_Item2 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T2, T2, Tuple<T1, T2, T3>, &Tuple<T1, T2, T3>::GetItem2, &Tuple<T1, T2, T3>::SetItem2, &Tuple<T1, T2, T3>::SetItem2> Item2{this};

		T3& GetItem3() const { return GOD()->m_Item3; }
				
		void SetItem3(T3 const & value) { 
			GOD()->m_Item3 = value; 
		}
			
		void SetItem3(T3&& value) { 
			GOD()->m_Item3 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T3, T3, Tuple<T1, T2, T3>, &Tuple<T1, T2, T3>::GetItem3, &Tuple<T1, T2, T3>::SetItem3, &Tuple<T1, T2, T3>::SetItem3> Item3{this};


			Tuple(T1 const & item1, T2 const & item2, T3 const & item3) {
				this->od = new ObjectData(item1, item2, item3);
			}
						Tuple(T1 const & item1, T2 const & item2, T3&& item3) {
				this->od = new ObjectData(item1, item2, std::move(item3));
			}
						Tuple(T1 const & item1, T2&& item2, T3 const & item3) {
				this->od = new ObjectData(item1, std::move(item2), item3);
			}
						Tuple(T1 const & item1, T2&& item2, T3&& item3) {
				this->od = new ObjectData(item1, std::move(item2), std::move(item3));
			}
						Tuple(T1&& item1, T2 const & item2, T3 const & item3) {
				this->od = new ObjectData(std::move(item1), item2, item3);
			}
						Tuple(T1&& item1, T2 const & item2, T3&& item3) {
				this->od = new ObjectData(std::move(item1), item2, std::move(item3));
			}
						Tuple(T1&& item1, T2&& item2, T3 const & item3) {
				this->od = new ObjectData(std::move(item1), std::move(item2), item3);
			}
						Tuple(T1&& item1, T2&& item2, T3&& item3) {
				this->od = new ObjectData(std::move(item1), std::move(item2), std::move(item3));
			}
			
	};

	template<typename T1, typename T2, typename T3, typename T4> class System_API Tuple<T1, T2, T3, T4, void, void, void, void> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			T2 m_Item2;
			T3 m_Item3;
			T4 m_Item4;
			ObjectData(T1 const & item1, T2 const & item2, T3 const & item3, T4 const & item4) : m_Item1(item1), m_Item2(item2), m_Item3(item3), m_Item4(item4){ }
							ObjectData(T1 const & item1, T2 const & item2, T3 const & item3, T4&& item4) : m_Item1(item1), m_Item2(item2), m_Item3(item3), m_Item4(std::move(item4)){ }
							ObjectData(T1 const & item1, T2 const & item2, T3&& item3, T4 const & item4) : m_Item1(item1), m_Item2(item2), m_Item3(std::move(item3)), m_Item4(item4){ }
							ObjectData(T1 const & item1, T2 const & item2, T3&& item3, T4&& item4) : m_Item1(item1), m_Item2(item2), m_Item3(std::move(item3)), m_Item4(std::move(item4)){ }
							ObjectData(T1 const & item1, T2&& item2, T3 const & item3, T4 const & item4) : m_Item1(item1), m_Item2(std::move(item2)), m_Item3(item3), m_Item4(item4){ }
							ObjectData(T1 const & item1, T2&& item2, T3 const & item3, T4&& item4) : m_Item1(item1), m_Item2(std::move(item2)), m_Item3(item3), m_Item4(std::move(item4)){ }
							ObjectData(T1 const & item1, T2&& item2, T3&& item3, T4 const & item4) : m_Item1(item1), m_Item2(std::move(item2)), m_Item3(std::move(item3)), m_Item4(item4){ }
							ObjectData(T1 const & item1, T2&& item2, T3&& item3, T4&& item4) : m_Item1(item1), m_Item2(std::move(item2)), m_Item3(std::move(item3)), m_Item4(std::move(item4)){ }
							ObjectData(T1&& item1, T2 const & item2, T3 const & item3, T4 const & item4) : m_Item1(std::move(item1)), m_Item2(item2), m_Item3(item3), m_Item4(item4){ }
							ObjectData(T1&& item1, T2 const & item2, T3 const & item3, T4&& item4) : m_Item1(std::move(item1)), m_Item2(item2), m_Item3(item3), m_Item4(std::move(item4)){ }
							ObjectData(T1&& item1, T2 const & item2, T3&& item3, T4 const & item4) : m_Item1(std::move(item1)), m_Item2(item2), m_Item3(std::move(item3)), m_Item4(item4){ }
							ObjectData(T1&& item1, T2 const & item2, T3&& item3, T4&& item4) : m_Item1(std::move(item1)), m_Item2(item2), m_Item3(std::move(item3)), m_Item4(std::move(item4)){ }
							ObjectData(T1&& item1, T2&& item2, T3 const & item3, T4 const & item4) : m_Item1(std::move(item1)), m_Item2(std::move(item2)), m_Item3(item3), m_Item4(item4){ }
							ObjectData(T1&& item1, T2&& item2, T3 const & item3, T4&& item4) : m_Item1(std::move(item1)), m_Item2(std::move(item2)), m_Item3(item3), m_Item4(std::move(item4)){ }
							ObjectData(T1&& item1, T2&& item2, T3&& item3, T4 const & item4) : m_Item1(std::move(item1)), m_Item2(std::move(item2)), m_Item3(std::move(item3)), m_Item4(item4){ }
							ObjectData(T1&& item1, T2&& item2, T3&& item3, T4&& item4) : m_Item1(std::move(item1)), m_Item2(std::move(item2)), m_Item3(std::move(item3)), m_Item4(std::move(item4)){ }
						};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1, T2, T3, T4>, &Tuple<T1, T2, T3, T4>::GetItem1, &Tuple<T1, T2, T3, T4>::SetItem1, &Tuple<T1, T2, T3, T4>::SetItem1> Item1{this};

		T2& GetItem2() const { return GOD()->m_Item2; }
				
		void SetItem2(T2 const & value) { 
			GOD()->m_Item2 = value; 
		}
			
		void SetItem2(T2&& value) { 
			GOD()->m_Item2 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T2, T2, Tuple<T1, T2, T3, T4>, &Tuple<T1, T2, T3, T4>::GetItem2, &Tuple<T1, T2, T3, T4>::SetItem2, &Tuple<T1, T2, T3, T4>::SetItem2> Item2{this};

		T3& GetItem3() const { return GOD()->m_Item3; }
				
		void SetItem3(T3 const & value) { 
			GOD()->m_Item3 = value; 
		}
			
		void SetItem3(T3&& value) { 
			GOD()->m_Item3 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T3, T3, Tuple<T1, T2, T3, T4>, &Tuple<T1, T2, T3, T4>::GetItem3, &Tuple<T1, T2, T3, T4>::SetItem3, &Tuple<T1, T2, T3, T4>::SetItem3> Item3{this};

		T4& GetItem4() const { return GOD()->m_Item4; }
				
		void SetItem4(T4 const & value) { 
			GOD()->m_Item4 = value; 
		}
			
		void SetItem4(T4&& value) { 
			GOD()->m_Item4 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T4, T4, Tuple<T1, T2, T3, T4>, &Tuple<T1, T2, T3, T4>::GetItem4, &Tuple<T1, T2, T3, T4>::SetItem4, &Tuple<T1, T2, T3, T4>::SetItem4> Item4{this};


			Tuple(T1 const & item1, T2 const & item2, T3 const & item3, T4 const & item4) {
				this->od = new ObjectData(item1, item2, item3, item4);
			}
						Tuple(T1 const & item1, T2 const & item2, T3 const & item3, T4&& item4) {
				this->od = new ObjectData(item1, item2, item3, std::move(item4));
			}
						Tuple(T1 const & item1, T2 const & item2, T3&& item3, T4 const & item4) {
				this->od = new ObjectData(item1, item2, std::move(item3), item4);
			}
						Tuple(T1 const & item1, T2 const & item2, T3&& item3, T4&& item4) {
				this->od = new ObjectData(item1, item2, std::move(item3), std::move(item4));
			}
						Tuple(T1 const & item1, T2&& item2, T3 const & item3, T4 const & item4) {
				this->od = new ObjectData(item1, std::move(item2), item3, item4);
			}
						Tuple(T1 const & item1, T2&& item2, T3 const & item3, T4&& item4) {
				this->od = new ObjectData(item1, std::move(item2), item3, std::move(item4));
			}
						Tuple(T1 const & item1, T2&& item2, T3&& item3, T4 const & item4) {
				this->od = new ObjectData(item1, std::move(item2), std::move(item3), item4);
			}
						Tuple(T1 const & item1, T2&& item2, T3&& item3, T4&& item4) {
				this->od = new ObjectData(item1, std::move(item2), std::move(item3), std::move(item4));
			}
						Tuple(T1&& item1, T2 const & item2, T3 const & item3, T4 const & item4) {
				this->od = new ObjectData(std::move(item1), item2, item3, item4);
			}
						Tuple(T1&& item1, T2 const & item2, T3 const & item3, T4&& item4) {
				this->od = new ObjectData(std::move(item1), item2, item3, std::move(item4));
			}
						Tuple(T1&& item1, T2 const & item2, T3&& item3, T4 const & item4) {
				this->od = new ObjectData(std::move(item1), item2, std::move(item3), item4);
			}
						Tuple(T1&& item1, T2 const & item2, T3&& item3, T4&& item4) {
				this->od = new ObjectData(std::move(item1), item2, std::move(item3), std::move(item4));
			}
						Tuple(T1&& item1, T2&& item2, T3 const & item3, T4 const & item4) {
				this->od = new ObjectData(std::move(item1), std::move(item2), item3, item4);
			}
						Tuple(T1&& item1, T2&& item2, T3 const & item3, T4&& item4) {
				this->od = new ObjectData(std::move(item1), std::move(item2), item3, std::move(item4));
			}
						Tuple(T1&& item1, T2&& item2, T3&& item3, T4 const & item4) {
				this->od = new ObjectData(std::move(item1), std::move(item2), std::move(item3), item4);
			}
						Tuple(T1&& item1, T2&& item2, T3&& item3, T4&& item4) {
				this->od = new ObjectData(std::move(item1), std::move(item2), std::move(item3), std::move(item4));
			}
			
	};

	template<typename T1, typename T2, typename T3, typename T4, typename T5> class System_API Tuple<T1, T2, T3, T4, T5, void, void, void> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			T2 m_Item2;
			T3 m_Item3;
			T4 m_Item4;
			T5 m_Item5;
	
		template <class U> ObjectData(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5) : item1(std::forward<U>(item1)), item2(std::forward<U>(item2)), item3(std::forward<U>(item3)), item4(std::forward<U>(item4)), item5(std::forward<U>(item5)){ }
				};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1, T2, T3, T4, T5>, &Tuple<T1, T2, T3, T4, T5>::GetItem1, &Tuple<T1, T2, T3, T4, T5>::SetItem1, &Tuple<T1, T2, T3, T4, T5>::SetItem1> Item1{this};

		T2& GetItem2() const { return GOD()->m_Item2; }
				
		void SetItem2(T2 const & value) { 
			GOD()->m_Item2 = value; 
		}
			
		void SetItem2(T2&& value) { 
			GOD()->m_Item2 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T2, T2, Tuple<T1, T2, T3, T4, T5>, &Tuple<T1, T2, T3, T4, T5>::GetItem2, &Tuple<T1, T2, T3, T4, T5>::SetItem2, &Tuple<T1, T2, T3, T4, T5>::SetItem2> Item2{this};

		T3& GetItem3() const { return GOD()->m_Item3; }
				
		void SetItem3(T3 const & value) { 
			GOD()->m_Item3 = value; 
		}
			
		void SetItem3(T3&& value) { 
			GOD()->m_Item3 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T3, T3, Tuple<T1, T2, T3, T4, T5>, &Tuple<T1, T2, T3, T4, T5>::GetItem3, &Tuple<T1, T2, T3, T4, T5>::SetItem3, &Tuple<T1, T2, T3, T4, T5>::SetItem3> Item3{this};

		T4& GetItem4() const { return GOD()->m_Item4; }
				
		void SetItem4(T4 const & value) { 
			GOD()->m_Item4 = value; 
		}
			
		void SetItem4(T4&& value) { 
			GOD()->m_Item4 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T4, T4, Tuple<T1, T2, T3, T4, T5>, &Tuple<T1, T2, T3, T4, T5>::GetItem4, &Tuple<T1, T2, T3, T4, T5>::SetItem4, &Tuple<T1, T2, T3, T4, T5>::SetItem4> Item4{this};

		T5& GetItem5() const { return GOD()->m_Item5; }
				
		void SetItem5(T5 const & value) { 
			GOD()->m_Item5 = value; 
		}
			
		void SetItem5(T5&& value) { 
			GOD()->m_Item5 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T5, T5, Tuple<T1, T2, T3, T4, T5>, &Tuple<T1, T2, T3, T4, T5>::GetItem5, &Tuple<T1, T2, T3, T4, T5>::SetItem5, &Tuple<T1, T2, T3, T4, T5>::SetItem5> Item5{this};


	template <class U> Tuple(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5) {
		this->od = new ObjectData(std::forward<U>(item1), std::forward<U>(item2), std::forward<U>(item3), std::forward<U>(item4), std::forward<U>(item5));
	}
	
	};

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6> class System_API Tuple<T1, T2, T3, T4, T5, T6, void, void> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			T2 m_Item2;
			T3 m_Item3;
			T4 m_Item4;
			T5 m_Item5;
			T6 m_Item6;
	
		template <class U> ObjectData(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5, U&& item6) : item1(std::forward<U>(item1)), item2(std::forward<U>(item2)), item3(std::forward<U>(item3)), item4(std::forward<U>(item4)), item5(std::forward<U>(item5)), item6(std::forward<U>(item6)){ }
				};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1, T2, T3, T4, T5, T6>, &Tuple<T1, T2, T3, T4, T5, T6>::GetItem1, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem1, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem1> Item1{this};

		T2& GetItem2() const { return GOD()->m_Item2; }
				
		void SetItem2(T2 const & value) { 
			GOD()->m_Item2 = value; 
		}
			
		void SetItem2(T2&& value) { 
			GOD()->m_Item2 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T2, T2, Tuple<T1, T2, T3, T4, T5, T6>, &Tuple<T1, T2, T3, T4, T5, T6>::GetItem2, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem2, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem2> Item2{this};

		T3& GetItem3() const { return GOD()->m_Item3; }
				
		void SetItem3(T3 const & value) { 
			GOD()->m_Item3 = value; 
		}
			
		void SetItem3(T3&& value) { 
			GOD()->m_Item3 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T3, T3, Tuple<T1, T2, T3, T4, T5, T6>, &Tuple<T1, T2, T3, T4, T5, T6>::GetItem3, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem3, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem3> Item3{this};

		T4& GetItem4() const { return GOD()->m_Item4; }
				
		void SetItem4(T4 const & value) { 
			GOD()->m_Item4 = value; 
		}
			
		void SetItem4(T4&& value) { 
			GOD()->m_Item4 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T4, T4, Tuple<T1, T2, T3, T4, T5, T6>, &Tuple<T1, T2, T3, T4, T5, T6>::GetItem4, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem4, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem4> Item4{this};

		T5& GetItem5() const { return GOD()->m_Item5; }
				
		void SetItem5(T5 const & value) { 
			GOD()->m_Item5 = value; 
		}
			
		void SetItem5(T5&& value) { 
			GOD()->m_Item5 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T5, T5, Tuple<T1, T2, T3, T4, T5, T6>, &Tuple<T1, T2, T3, T4, T5, T6>::GetItem5, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem5, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem5> Item5{this};

		T6& GetItem6() const { return GOD()->m_Item6; }
				
		void SetItem6(T6 const & value) { 
			GOD()->m_Item6 = value; 
		}
			
		void SetItem6(T6&& value) { 
			GOD()->m_Item6 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T6, T6, Tuple<T1, T2, T3, T4, T5, T6>, &Tuple<T1, T2, T3, T4, T5, T6>::GetItem6, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem6, &Tuple<T1, T2, T3, T4, T5, T6>::SetItem6> Item6{this};


	template <class U> Tuple(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5, U&& item6) {
		this->od = new ObjectData(std::forward<U>(item1), std::forward<U>(item2), std::forward<U>(item3), std::forward<U>(item4), std::forward<U>(item5), std::forward<U>(item6));
	}
	
	};

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7> class System_API Tuple<T1, T2, T3, T4, T5, T6, T7, void> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			T2 m_Item2;
			T3 m_Item3;
			T4 m_Item4;
			T5 m_Item5;
			T6 m_Item6;
			T7 m_Item7;
	
		template <class U> ObjectData(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5, U&& item6, U&& item7) : item1(std::forward<U>(item1)), item2(std::forward<U>(item2)), item3(std::forward<U>(item3)), item4(std::forward<U>(item4)), item5(std::forward<U>(item5)), item6(std::forward<U>(item6)), item7(std::forward<U>(item7)){ }
				};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1, T2, T3, T4, T5, T6, T7>, &Tuple<T1, T2, T3, T4, T5, T6, T7>::GetItem1, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem1, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem1> Item1{this};

		T2& GetItem2() const { return GOD()->m_Item2; }
				
		void SetItem2(T2 const & value) { 
			GOD()->m_Item2 = value; 
		}
			
		void SetItem2(T2&& value) { 
			GOD()->m_Item2 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T2, T2, Tuple<T1, T2, T3, T4, T5, T6, T7>, &Tuple<T1, T2, T3, T4, T5, T6, T7>::GetItem2, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem2, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem2> Item2{this};

		T3& GetItem3() const { return GOD()->m_Item3; }
				
		void SetItem3(T3 const & value) { 
			GOD()->m_Item3 = value; 
		}
			
		void SetItem3(T3&& value) { 
			GOD()->m_Item3 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T3, T3, Tuple<T1, T2, T3, T4, T5, T6, T7>, &Tuple<T1, T2, T3, T4, T5, T6, T7>::GetItem3, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem3, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem3> Item3{this};

		T4& GetItem4() const { return GOD()->m_Item4; }
				
		void SetItem4(T4 const & value) { 
			GOD()->m_Item4 = value; 
		}
			
		void SetItem4(T4&& value) { 
			GOD()->m_Item4 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T4, T4, Tuple<T1, T2, T3, T4, T5, T6, T7>, &Tuple<T1, T2, T3, T4, T5, T6, T7>::GetItem4, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem4, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem4> Item4{this};

		T5& GetItem5() const { return GOD()->m_Item5; }
				
		void SetItem5(T5 const & value) { 
			GOD()->m_Item5 = value; 
		}
			
		void SetItem5(T5&& value) { 
			GOD()->m_Item5 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T5, T5, Tuple<T1, T2, T3, T4, T5, T6, T7>, &Tuple<T1, T2, T3, T4, T5, T6, T7>::GetItem5, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem5, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem5> Item5{this};

		T6& GetItem6() const { return GOD()->m_Item6; }
				
		void SetItem6(T6 const & value) { 
			GOD()->m_Item6 = value; 
		}
			
		void SetItem6(T6&& value) { 
			GOD()->m_Item6 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T6, T6, Tuple<T1, T2, T3, T4, T5, T6, T7>, &Tuple<T1, T2, T3, T4, T5, T6, T7>::GetItem6, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem6, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem6> Item6{this};

		T7& GetItem7() const { return GOD()->m_Item7; }
				
		void SetItem7(T7 const & value) { 
			GOD()->m_Item7 = value; 
		}
			
		void SetItem7(T7&& value) { 
			GOD()->m_Item7 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T7, T7, Tuple<T1, T2, T3, T4, T5, T6, T7>, &Tuple<T1, T2, T3, T4, T5, T6, T7>::GetItem7, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem7, &Tuple<T1, T2, T3, T4, T5, T6, T7>::SetItem7> Item7{this};


	template <class U> Tuple(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5, U&& item6, U&& item7) {
		this->od = new ObjectData(std::forward<U>(item1), std::forward<U>(item2), std::forward<U>(item3), std::forward<U>(item4), std::forward<U>(item5), std::forward<U>(item6), std::forward<U>(item7));
	}
	
	};

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8> class System_API Tuple<T1, T2, T3, T4, T5, T6, T7, T8> : public Object {
	private:

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:			
		public:
			T1 m_Item1;
			T2 m_Item2;
			T3 m_Item3;
			T4 m_Item4;
			T5 m_Item5;
			T6 m_Item6;
			T7 m_Item7;
			T8 m_Item8;
	
		template <class U> ObjectData(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5, U&& item6, U&& item7, U&& item8) : item1(std::forward<U>(item1)), item2(std::forward<U>(item2)), item3(std::forward<U>(item3)), item4(std::forward<U>(item4)), item5(std::forward<U>(item5)), item6(std::forward<U>(item6)), item7(std::forward<U>(item7)), item8(std::forward<U>(item8)){ }
				};

		ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Tuple(){}

		Tuple(std::nullptr_t const & n) : System::Object(n) {
		}

		Tuple(Tuple* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Tuple(Tuple const & other) : System::Object(other) { }

		Tuple(Tuple&& other) noexcept : System::Object(std::move(other)) { }

		Tuple(Object::ObjectData* other) : System::Object(other) {
		}

		Tuple& operator=(Tuple const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Tuple& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Tuple& operator=(Tuple&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Tuple& operator=(Tuple* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Tuple* operator->() {
			return this;
		}


		T1& GetItem1() const { return GOD()->m_Item1; }
				
		void SetItem1(T1 const & value) { 
			GOD()->m_Item1 = value; 
		}
			
		void SetItem1(T1&& value) { 
			GOD()->m_Item1 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T1, T1, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem1, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem1, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem1> Item1{this};

		T2& GetItem2() const { return GOD()->m_Item2; }
				
		void SetItem2(T2 const & value) { 
			GOD()->m_Item2 = value; 
		}
			
		void SetItem2(T2&& value) { 
			GOD()->m_Item2 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T2, T2, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem2, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem2, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem2> Item2{this};

		T3& GetItem3() const { return GOD()->m_Item3; }
				
		void SetItem3(T3 const & value) { 
			GOD()->m_Item3 = value; 
		}
			
		void SetItem3(T3&& value) { 
			GOD()->m_Item3 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T3, T3, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem3, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem3, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem3> Item3{this};

		T4& GetItem4() const { return GOD()->m_Item4; }
				
		void SetItem4(T4 const & value) { 
			GOD()->m_Item4 = value; 
		}
			
		void SetItem4(T4&& value) { 
			GOD()->m_Item4 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T4, T4, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem4, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem4, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem4> Item4{this};

		T5& GetItem5() const { return GOD()->m_Item5; }
				
		void SetItem5(T5 const & value) { 
			GOD()->m_Item5 = value; 
		}
			
		void SetItem5(T5&& value) { 
			GOD()->m_Item5 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T5, T5, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem5, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem5, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem5> Item5{this};

		T6& GetItem6() const { return GOD()->m_Item6; }
				
		void SetItem6(T6 const & value) { 
			GOD()->m_Item6 = value; 
		}
			
		void SetItem6(T6&& value) { 
			GOD()->m_Item6 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T6, T6, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem6, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem6, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem6> Item6{this};

		T7& GetItem7() const { return GOD()->m_Item7; }
				
		void SetItem7(T7 const & value) { 
			GOD()->m_Item7 = value; 
		}
			
		void SetItem7(T7&& value) { 
			GOD()->m_Item7 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T7, T7, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem7, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem7, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem7> Item7{this};

		T8& GetItem8() const { return GOD()->m_Item8; }
				
		void SetItem8(T8 const & value) { 
			GOD()->m_Item8 = value; 
		}
			
		void SetItem8(T8&& value) { 
			GOD()->m_Item8 = std::move(value); 
		}

		/// <summary></summary>
		PropGen<T8, T8, Tuple<T1, T2, T3, T4, T5, T6, T7, T8>, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::GetItem8, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem8, &Tuple<T1, T2, T3, T4, T5, T6, T7, T8>::SetItem8> Item8{this};


	template <class U> Tuple(U&& item1, U&& item2, U&& item3, U&& item4, U&& item5, U&& item6, U&& item7, U&& item8) {
		this->od = new ObjectData(std::forward<U>(item1), std::forward<U>(item2), std::forward<U>(item3), std::forward<U>(item4), std::forward<U>(item5), std::forward<U>(item6), std::forward<U>(item7), std::forward<U>(item8));
	}
	
	};
}



namespace System {
	namespace Text {

		class System_API StringBuilder : public System::Object
		{
		private:
		public:
			class System_API ObjectData : public System::Object::ObjectData {
			private:
			public:
				mutable char16_t* chrs;
				mutable int cap;
				int len;

				ObjectData(int const capacity) {
					chrs = new char16_t[capacity];
					cap = capacity;
					len = 0;
				}

				ObjectData() : ObjectData(16) {}

				~ObjectData() override {
					delete[] chrs;
				}

				void EnsureCapacity(int const capacity) const {
					if (capacity <= cap)
						return;

					while (capacity > cap) {
						cap <<= 1;
					}
					char16_t* newchrs = new char16_t[cap];
					memcpy(newchrs, chrs, len << 1);
					delete[] chrs;
					chrs = newchrs;
				}

				void Append(System::String const& value) {
					if (value == null)
						return;

					if (value.Length == 0)
						return;
					EnsureCapacity(len + value.Length);
					memcpy(chrs + len, value.ToCharArray(), value.Length << 1);
					len += value.Length;
				}

				void Append(char16_t const * value) {
					int l = std::wcslen((wchar_t const *)value);
					EnsureCapacity(len + l);
					std::memcpy(chrs + len, value, l << 1);
					len += l;
				}

				void Append(char16_t const value) {
					EnsureCapacity(len + 1);
					chrs[len++] = value;
				}

				void Append(short const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%d", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				void Append(ushort const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%u", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				void Append(int const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%d", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				void Append(uint const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%u", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				void Append(long const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%lld", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				void Append(ulong const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%llu", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				void Append(float const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%f", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					wchar_t const* p = std::wcschr((wchar_t const*)tempStr, u'.');
					if (p != null) {
						while (l > 0 && tempStr[l - 1] == u'0')
							l--;
						if (tempStr[l - 1] == u'.')
							l--;
					}
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				void Append(double const value) {
					char16_t tempStr[32] = { 0 };
					swprintf((wchar_t* const)tempStr, 32, (wchar_t const* const)u"%f", value);
					int l = std::wcslen((wchar_t const*)tempStr);
					wchar_t const * p = std::wcschr((wchar_t const*)tempStr, u'.');
					if (p != null) {
						while (l > 0 && tempStr[l - 1] == u'0')
							l--;
						if (tempStr[l - 1] == u'.')
							l--;
					}
					EnsureCapacity(len + l);
					memcpy(chrs + len, tempStr, l << 1);
					len += l;
				}

				System::String ToString() const override {
					EnsureCapacity(len + 1);
					chrs[len] = u'\0';

					return chrs;
				}

				void Clear() {
					len = 0;
				}

			};


			ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			StringBuilder(){}

			StringBuilder(std::nullptr_t const & n) {
				this->od = nullptr;
			}

			StringBuilder(StringBuilder* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					this->od = dd;
				}
				else {
					this->od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			StringBuilder(StringBuilder const & other) : System::Object(other) { }

			StringBuilder(StringBuilder&& other) noexcept : System::Object(std::move(other)) { }

			StringBuilder(ObjectData* other) {
				this->od = other;
				if (this->od)
					this->AddRef();
			}

			StringBuilder& operator=(StringBuilder const & other) {
				System::Object::operator=(other);
				return *this;
			}

			StringBuilder& operator=(StringBuilder* other) {
				if (this->od == other->od)
					return *this;
				this->Release();
				this->od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			StringBuilder& operator=(StringBuilder&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			StringBuilder* operator->() {
				return this;
			}



			void Append(System::String const& value) {
				GOD()->Append(value);
			}

			void Append(char16_t const* value) {
				GOD()->Append(value);
			}

			void Append(char16_t const value) {
				GOD()->Append(value);
			}

			void Append(short const value) {
				GOD()->Append(value);
			}

			void Append(ushort const value) {
				GOD()->Append(value);
			}

			void Append(int const value) {
				GOD()->Append(value);
			}

			void Append(uint const value) {
				GOD()->Append(value);
			}

			void Append(long const value) {
				GOD()->Append(value);
			}

			void Append(ulong const value) {
				GOD()->Append(value);
			}

			void Append(float const value) {
				GOD()->Append(value);
			}

			void Append(double const value) {
				GOD()->Append(value);
			}

			int EnsureCapacity(int const capacity) {
				if (capacity < 0)
					throw ArgumentOutOfRangeException("capacity");
				ObjectData* od = GOD();
				od->EnsureCapacity(capacity);

				return od->cap;
			}

			void Clear() {
				GOD()->Clear();
			}



			System::String ToString() const {
				return GOD()->ToString();
			}

		};

	}
}
//#include "Func.h"

namespace System {

	class System_API TimeSpan : public Object {
	private:
		static long const TicksPerMillisecond = 10000;
		static double const MillisecondsPerTick;

		static long const TicksPerSecond = TicksPerMillisecond * 1000;   // 10,000,000
		static double const SecondsPerTick;         // 0.0001

		static long const TicksPerMinute = TicksPerSecond * 60;         // 600,000,000
		static double const MinutesPerTick; // 1.6666666666667e-9

		static long const TicksPerHour = TicksPerMinute * 60;        // 36,000,000,000
		static double const HoursPerTick; // 2.77777777777777778e-11

		static long const TicksPerDay = TicksPerHour * 24;          // 864,000,000,000
		static double const DaysPerTick; // 1.1574074074074074074e-12

		static int const MillisPerSecond = 1000;
		static int const MillisPerMinute = MillisPerSecond * 60; //     60,000
		static int const MillisPerHour = MillisPerMinute * 60;   //  3,600,000
		static int const MillisPerDay = MillisPerHour * 24;      // 86,400,000

		static long const MaxSeconds = Int64::MaxValue / TicksPerSecond;
		static long const MinSeconds = Int64::MinValue / TicksPerSecond;

		static long const MaxMilliSeconds = Int64::MaxValue / TicksPerMillisecond;
		static long const MinMilliSeconds = Int64::MinValue / TicksPerMillisecond;

		static long const TicksPerTenthSecond = TicksPerMillisecond * 100;



		long Get_Ticks() const {
			ObjectData* dd = static_cast<ObjectData*>(this->od);
			return dd->Ticks;
		}

		int Get_Days() const {
			return (int)(Get_Ticks() / TicksPerDay);
		}

		int Get_Hours() const {
			return (int)((Get_Ticks() / TicksPerHour) % 24);
		}

		int Get_Milliseconds() const {
			return (int)((Get_Ticks() / TicksPerMillisecond) % 1000);
		}

		int Get_Minutes() const {
			return (int)((Get_Ticks() / TicksPerMinute) % 60);
		}

		int Get_Seconds() const {
			return (int)((Get_Ticks() / TicksPerSecond) % 60);
		}

		double Get_TotalDays() const {
			return ((double)Get_Ticks()) * DaysPerTick;
		}

		double Get_TotalHours() const {
			return ((double)Get_Ticks()) * HoursPerTick;
		}

		double Get_TotalMilliseconds() const {
			double temp = ((double)Get_Ticks()) * MillisecondsPerTick;
			if (temp > MaxMilliSeconds)
				return (double)MaxMilliSeconds;

			if (temp < MinMilliSeconds)
				return (double)MinMilliSeconds;

			return temp;
		}

		double Get_TotalMinutes() const {
			return ((double)Get_Ticks()) * MinutesPerTick;
		}

		double Get_TotalSeconds() const {
			return ((double)Get_Ticks()) * SecondsPerTick;
		}

		static TimeSpan Interval(double const value, int const scale) {
			//if (Double.IsNaN(value))
			//    throw new ArgumentException(Environment.GetResourceString("Arg_CannotBeNaN"));
			//Contract.EndContractBlock();
			double tmp = value * scale;
			double millis = tmp + (value >= 0 ? 0.5 : -0.5);
			if ((millis > Int64::MaxValue / TicksPerMillisecond) || (millis < Int64::MinValue / TicksPerMillisecond))
				throw OverflowException();
			return TimeSpan((long)millis * TicksPerMillisecond);
		}

		static long TimeToTicks(int const hour, int const minute, int const second) {
			// totalSeconds is bounded by 2^31 * 2^12 + 2^31 * 2^8 + 2^31,
			// which is less than 2^44, meaning we won't overflow totalSeconds.
			long totalSeconds = (long)hour * 3600 + (long)minute * 60 + (long)second;
			if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
				throw ArgumentOutOfRangeException();
			return totalSeconds * TicksPerSecond;
		}

		friend class DateTime;
	public:
		class System_API ObjectData : public Object::ObjectData {
		private:
			friend class TimeSpan;
		public:
			ObjectData() : Object::ObjectData(0) {}

			ObjectData(long ticks) : ObjectData() {
				this->Ticks = ticks;
			}

			Object::ObjectData* DeepCopy() override {
				return new ObjectData(Ticks);
			}

			long Ticks = 0;
		};

		static const TimeSpan Zero;

		static const TimeSpan MaxValue;
		static const TimeSpan MinValue;


				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		TimeSpan(){}

		TimeSpan(std::nullptr_t const & n) : System::Object(n) {
		}

		TimeSpan(TimeSpan* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		TimeSpan(TimeSpan const & other) : System::Object(other) { }

		TimeSpan(TimeSpan&& other) noexcept : System::Object(std::move(other)) { }

		TimeSpan(Object::ObjectData* other) : System::Object(other) {
		}

		TimeSpan& operator=(TimeSpan const & other) {
			System::Object::operator=(other);
			return *this;
		}

		TimeSpan& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		TimeSpan& operator=(TimeSpan&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		TimeSpan& operator=(TimeSpan* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		TimeSpan* operator->() {
			return this;
		}


			TimeSpan(long ticks) {
			ObjectData* dd = new ObjectData(ticks);
			this->od = dd;
		}

		TimeSpan(int hours, int minutes, int seconds) {
			ObjectData* dd = new ObjectData(TimeToTicks(hours, minutes, seconds));
			this->od = dd;
		}

		TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds) {
			long totalMilliSeconds = ((long)days * 3600 * 24 + (long)hours * 3600 + (long)minutes * 60 + seconds) * 1000 + milliseconds;
			if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
				throw ArgumentOutOfRangeException();
			ObjectData* dd = new ObjectData((long)totalMilliSeconds * TicksPerMillisecond);
			this->od = dd;
		}

		TimeSpan(int days, int hours, int minutes, int const seconds) : TimeSpan(days, hours, minutes, seconds, 0) {
		}

		TimeSpan Add(TimeSpan const & ts) const {
			long _ticks = Get_Ticks();
			long ts_ticks = ts.Get_Ticks();
			long result = _ticks + ts_ticks;
			// Overflow if signs of operands was identical and result's
			// sign was opposite.
			// >> 63 gives the sign bit (either 64 1's or 64 0's).
			if ((_ticks >> 63 == ts_ticks >> 63) && (_ticks >> 63 != result >> 63))
				throw OverflowException();
			return TimeSpan(result);
		}

		void Set_Ticks(long ticks) {
			ObjectData* dd = static_cast<ObjectData*>(this->od);
			dd->Ticks = ticks;
		}

		// Compares two TimeSpan values, returning an integer that indicates their
		// relationship.
		//
		static int Compare(TimeSpan const& t1, TimeSpan const& t2) {
			long t1_ticks = t1.Get_Ticks();
			long t2_ticks = t2.Get_Ticks();
			if (t1_ticks > t2_ticks) return 1;
			if (t1_ticks < t2_ticks) return -1;
			return 0;
		}

		int CompareTo(TimeSpan const & value) const {
			long _ticks = Get_Ticks();
			long t = value.Get_Ticks();
			if (_ticks > t) return 1;
			if (_ticks < t) return -1;
			return 0;
		}

		static TimeSpan FromDays(double value) {
			return Interval(value, MillisPerDay);
		}

		TimeSpan Duration() const {
			long _ticks = Get_Ticks();
			//if (Ticks==TimeSpan.MinValue.Ticks)
			//    throw new OverflowException(Environment.GetResourceString("Overflow_Duration"));
			return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
		}

		static TimeSpan FromHours(double const value) {
			return Interval(value, MillisPerHour);
		}

		static TimeSpan FromMilliseconds(double const value) {
			return Interval(value, 1);
		}

		static TimeSpan FromMinutes(double const value) {
			return Interval(value, MillisPerMinute);
		}

		TimeSpan Negate() const {
			long _ticks = Get_Ticks();
			//    if (Ticks==TimeSpan.MinValue.Ticks)
			//        throw new OverflowException(Environment.GetResourceString("Overflow_NegateTwosCompNum"));
			return TimeSpan(-_ticks);
		}

		static TimeSpan FromSeconds(double const value) {
			return Interval(value, MillisPerSecond);
		}

		TimeSpan Subtract(TimeSpan const& ts) const {
			long _ticks = Get_Ticks();
			long ts_ticks = ts.Get_Ticks();
			long result = _ticks - ts_ticks;
			// Overflow if signs of operands was different and result's
			// sign was opposite from the first argument's sign.
			// >> 63 gives the sign bit (either 64 1's or 64 0's).
			if ((_ticks >> 63 != ts_ticks >> 63) && (_ticks >> 63 != result >> 63))
				throw OverflowException();
			return TimeSpan(result);
		}

		static TimeSpan FromTicks(long const value) {
			return TimeSpan(value);
		}

		bool operator==(TimeSpan const& t) const {
			return Get_Ticks() == t.Get_Ticks();
		}

		bool operator!=(TimeSpan const& t) const {
			return Get_Ticks() != t.Get_Ticks();
		}

		bool operator<(TimeSpan const& t) const {
			return Get_Ticks() < t.Get_Ticks();
		}

		bool operator <=(TimeSpan const& t) const {
			return Get_Ticks() <= t.Get_Ticks();
		}

		bool operator>(TimeSpan const& t) const {
			return Get_Ticks() > t.Get_Ticks();
		}

		bool operator >=(TimeSpan const& t) const {
			return Get_Ticks() >= t.Get_Ticks();
		}

		TimeSpan operator-() const {
			long t_ticks = Get_Ticks();
			//if (t_ticks==TimeSpan.MinValue._ticks)
			//	throw OverflowException();
			return TimeSpan(-t_ticks);
		}

		TimeSpan const & operator+() const {
			return *this;
		}

		PropGenGet<long, TimeSpan, &TimeSpan::Get_Ticks> Ticks{ this };
		PropGenGet<int, TimeSpan, &TimeSpan::Get_Days> Days{ this };
		PropGenGet<int, TimeSpan, &TimeSpan::Get_Hours> Hours{ this };
		PropGenGet<int, TimeSpan, &TimeSpan::Get_Milliseconds> Milliseconds{ this };
		PropGenGet<int, TimeSpan, &TimeSpan::Get_Seconds> Seconds{ this };
		PropGenGet<double, TimeSpan, &TimeSpan::Get_TotalDays> TotalDays{ this };
		PropGenGet<double, TimeSpan, &TimeSpan::Get_TotalHours> TotalHours{ this };
		PropGenGet<double, TimeSpan, &TimeSpan::Get_TotalMilliseconds> TotalMilliseconds{ this };
		PropGenGet<double, TimeSpan, &TimeSpan::Get_TotalMinutes> TotalMinutes{ this };
		PropGenGet<double, TimeSpan, &TimeSpan::Get_TotalSeconds> TotalSeconds{ this };

	};

	TimeSpan operator-(TimeSpan const & t1, TimeSpan const & t2)
	{
		return t1.Subtract(t2);
	}

	TimeSpan operator+(TimeSpan const & t1, TimeSpan const & t2)
	{
		return t1.Add(t2);
	}

#ifndef SYSTEM_EXPORTS
	double const TimeSpan::MillisecondsPerTick = 1.0 / TimeSpan::TicksPerMillisecond;
	double const TimeSpan::SecondsPerTick = 1.0 / TimeSpan::TicksPerSecond;
	double const TimeSpan::MinutesPerTick = 1.0 / TimeSpan::TicksPerMinute;
	double const TimeSpan::HoursPerTick = 1.0 / TimeSpan::TicksPerHour;
	double const TimeSpan::DaysPerTick = 1.0 / TimeSpan::TicksPerDay;
	TimeSpan const TimeSpan::Zero{ (int64_t)0 };
	TimeSpan const TimeSpan::MaxValue{ Int64::MaxValue };
	TimeSpan const TimeSpan::MinValue{ Int64::MinValue };

#endif

	enum class DateTimeKind {
		Unspecified = 0,
		Utc = 1,
		Local = 2
	};

	class System_API DateTime : public Object
	{
	private:
		//static long DateToTicks(int year, int month, int day) {
		//		//int days[13] = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
		//		//if (day >= 1 && day <= days[month] - days[month - 1]) {
		//		//	int y = year - 1;
		//		//	int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
		//		//	return n * TicksPerDay;
		//		//}
		//}

		static DateTime Get_Now()
		{
			return ObjectData::Get_Now();
		}

		long Get_Ticks() const {
			ObjectData* dd = GOD();
			return dd->Get_Ticks();
		}

		int Get_Year() const {
			ObjectData* dd = GOD();
			return dd->Get_Year();
		}

		int Get_Month() const {
			ObjectData* dd = GOD();
			return dd->Get_Month();
		}

		int Get_Day() const {
			ObjectData* dd = GOD();
			return dd->Get_Day();
		}

		int Get_Hour() const {
			ObjectData* dd = GOD();
			return dd->Get_Hour();
		}

		int Get_Minute() const {
			ObjectData* dd = GOD();
			return dd->Get_Minute();
		}

		int Get_Second() const {
			ObjectData* dd = GOD();
			return dd->Get_Second();
		}

		int Get_Millisecond() const {
			ObjectData* dd = GOD();
			return dd->Get_Millisecond();
		}

		//void Set_Ticks(long& value) {
		//	ObjectData* dd = GOD();
		//	dd->Set_Ticks(value);
		//}

		//void Set_Ticks(long&& value) {
		//	ObjectData* dd = GOD();
		//	dd->Set_Ticks(value);
		//}

	public:
		class System_API ObjectData : public Object::ObjectData {
		private:
			static long const TicksPerMillisecond = 10000;
			static long const TicksPerSecond = TicksPerMillisecond * 1000;
			static long const TicksPerMinute = TicksPerSecond * 60;
			static long const TicksPerHour = TicksPerMinute * 60;
			static long const TicksPerDay = TicksPerHour * 24;

			// Number of milliseconds per time unit
			static int const MillisPerSecond = 1000;
			static int const MillisPerMinute = MillisPerSecond * 60;
			static int const MillisPerHour = MillisPerMinute * 60;
			static int const MillisPerDay = MillisPerHour * 24;

			// Number of days in a non-leap year
			static int const DaysPerYear = 365;
			// Number of days in 4 years
			static int const DaysPer4Years = DaysPerYear * 4 + 1;       // 1461
			// Number of days in 100 years
			static int const DaysPer100Years = DaysPer4Years * 25 - 1;  // 36524
			// Number of days in 400 years
			static int const DaysPer400Years = DaysPer100Years * 4 + 1; // 146097

			// Number of days from 1/1/0001 to 12/31/1600
			static int const DaysTo1601 = DaysPer400Years * 4;          // 584388
			// Number of days from 1/1/0001 to 12/30/1899
			static int const DaysTo1899 = DaysPer400Years * 4 + DaysPer100Years * 3 - 367;
			// Number of days from 1/1/0001 to 12/31/1969
			static int const DaysTo1970 = DaysPer400Years * 4 + DaysPer100Years * 3 + DaysPer4Years * 17 + DaysPerYear; // 719,162
			// Number of days from 1/1/0001 to 12/31/9999
			static int const DaysTo10000 = DaysPer400Years * 25 - 366;  // 3652059

			static long const MinTicks = 0;
			static long const MaxTicks = DaysTo10000 * TicksPerDay - 1;
			static long const MaxMillis = (long)DaysTo10000 * MillisPerDay;

			static int const DatePartYear = 0;
			static int const DatePartDayOfYear = 1;
			static int const DatePartMonth = 2;
			static int const DatePartDay = 3;

			static int const DaysToMonth365[13];// = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
			static int const DaysToMonth366[13];// = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

			static ulong const TicksMask = 0x3FFFFFFFFFFFFFFF;
			static ulong const FlagsMask = 0xC000000000000000;
			static ulong const LocalMask = 0x8000000000000000;
			static long const TicksCeiling = 0x4000000000000000;
			static ulong const KindUnspecified = 0x0000000000000000;
			static ulong const KindUtc = 0x4000000000000000;
			static ulong const KindLocal = 0x8000000000000000;
			static ulong const KindLocalAmbiguousDst = 0xC000000000000000;
			static int const  KindShift = 62;

			// The data is stored as an unsigned 64-bit integer
			//   Bits 00-61: The value of 100-nanosecond ticks where 0 represents 1/1/0001 12:00am, up until the value
			//               12/31/9999 23:59:59.9999999
			//   Bits 62-63: A four-state value that describes the DateTimeKind value of the date time, with a 2nd
			//               value for the rare case where the date time is local, but is in an overlapped daylight
			//               savings time hour and it is in daylight savings time. This allows distinction of these
			//               otherwise ambiguous local times and prevents data loss when round tripping from Local to
			//               UTC time.
			ulong dateData = 0;

			long InternalTicks() const {
				return (long)(dateData & TicksMask);
			}

			static long DateToTicks(int  year, int month, int day) {
				int const* days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
				if (day >= 1 && day <= days[month] - days[month - 1]) {
					int y = year - 1;
					int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
					return n * TicksPerDay;
				}
			}

			// Return the tick count corresponding to the given hour, minute, second.
			// Will check the if the parameters are valid.
			static long TimeToTicks(int hour, int minute, int second)
			{
				//TimeSpan.TimeToTicks is a family access function which does no error checking, so
				//we need to put some error checking out here.
				if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60)
				{
					return (TimeSpan::TimeToTicks(hour, minute, second));
				}
				throw ArgumentOutOfRangeException();
			}

			// Returns a given date part of this DateTime. This method is used
// to compute the year, day-of-year, month, or day part.
			int GetDatePart(int part) const {
				long ticks = InternalTicks();
				// n = number of days since 1/1/0001
				int n = (int)(ticks / TicksPerDay);
				// y400 = number of whole 400-year periods since 1/1/0001
				int y400 = n / DaysPer400Years;
				// n = day number within 400-year period
				n -= y400 * DaysPer400Years;
				// y100 = number of whole 100-year periods within 400-year period
				int y100 = n / DaysPer100Years;
				// Last 100-year period has an extra day, so decrement result if 4
				if (y100 == 4) y100 = 3;
				// n = day number within 100-year period
				n -= y100 * DaysPer100Years;
				// y4 = number of whole 4-year periods within 100-year period
				int y4 = n / DaysPer4Years;
				// n = day number within 4-year period
				n -= y4 * DaysPer4Years;
				// y1 = number of whole years within 4-year period
				int y1 = n / DaysPerYear;
				// Last year has an extra day, so decrement result if 4
				if (y1 == 4) y1 = 3;
				// If year was requested, compute and return it
				if (part == DatePartYear) {
					return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
				}
				// n = day number within year
				n -= y1 * DaysPerYear;
				// If day-of-year was requested, return it
				if (part == DatePartDayOfYear) return n + 1;
				// Leap year calculation looks different from IsLeapYear since y1, y4,
				// and y100 are relative to year 1, not year 0
				bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
				int const* days = leapYear ? DaysToMonth366 : DaysToMonth365;
				// All months have less than 32 days, so n >> 5 is a good conservative
				// estimate for the month
				int m = (n >> 5) + 1;
				// m = 1-based month number
				while (n >= days[m]) m++;
				// If month was requested, return it
				if (part == DatePartMonth) return m;
				// Return 1-based day-of-month
				return n - days[m - 1] + 1;
			}


			friend class DateTime;
		public:
			ObjectData() : Object::ObjectData(0) {}

			ObjectData(long ticks) : ObjectData() {
				dateData = (ulong)ticks;
			}

			ObjectData(ulong dateData) : ObjectData() {
				this->dateData = dateData;
			}

			ObjectData(int year, int month, int day) : ObjectData() {
				dateData = (ulong)DateToTicks(year, month, day);
			}

			ObjectData(int year, int month, int day, int hour, int minute, int second) : ObjectData() {
				dateData = (ulong)DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			}

			ObjectData(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind) : ObjectData() {
				long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
				this->dateData = ((ulong)ticks | ((ulong)kind << KindShift));
			}

			ObjectData(int year, int month, int day, int hour, int minute, int second, int millisecond) : ObjectData() {
				long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
				ticks += millisecond * TicksPerMillisecond;
				dateData = (ulong)ticks;
			}

			ObjectData(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind) : ObjectData() {
				long ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
				ticks += millisecond * TicksPerMillisecond;
				this->dateData = ((ulong)ticks | ((ulong)kind << KindShift));
			}

			Object::ObjectData* DeepCopy() override {
				ObjectData* ret = new ObjectData();
				ret->dateData = this->dateData;
				return ret;
			}

			static DateTime Get_Now()
			{
				DateTime ret;
				std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
				long nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
				return DateTime(621355968000000000 + (nanos / 100));
			}

			long Get_Ticks() const {
				return InternalTicks();
			}

			int Get_Year() const {
				return GetDatePart(DatePartYear);
			}

			int Get_Month() const {
				return GetDatePart(DatePartMonth);
			}

			int Get_Day() const {
				return GetDatePart(DatePartDay);
			}

			int Get_Hour() const {
				return (int)(InternalTicks() / TicksPerHour) % 24;
			}

			int Get_Minute() const {
				return (int)(InternalTicks() / TicksPerMinute) % 60;
			}

			int Get_Second() const {
				return (int)((InternalTicks() / TicksPerSecond) % 60);
			}

			int Get_Millisecond() const {
				return (int)((InternalTicks() / TicksPerMillisecond)) % 1000;
			}

			static bool IsLeapYear(int year) {
				return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
			}

			String ToString() const override {
				return
					((Int32)GetDatePart(DatePartYear)).ToString("D4") + "/" +
					((Int32)GetDatePart(DatePartMonth)).ToString("D2") + "/" +
					((Int32)GetDatePart(DatePartDay)).ToString("D2") + " " +
					((Int32)Get_Hour()).ToString("D2") + ":" +
					((Int32)Get_Minute()).ToString("D2") + ":" +
					((Int32)Get_Second()).ToString("D2")
					;
			}
		};

		static PropGenGetStatic<DateTime, &DateTime::Get_Now> Now;
		PropGenGet<long, DateTime, &DateTime::Get_Ticks> Ticks{ this };
		PropGenGet<int, DateTime, &DateTime::Get_Year> Year{ this };
		PropGenGet<int, DateTime, &DateTime::Get_Month> Month{ this };
		PropGenGet<int, DateTime, &DateTime::Get_Day> Day{ this };
		PropGenGet<int, DateTime, &DateTime::Get_Hour> Hour{ this };
		PropGenGet<int, DateTime, &DateTime::Get_Minute> Minute{ this };
		PropGenGet<int, DateTime, &DateTime::Get_Second> Second{ this };
		PropGenGet<int, DateTime, &DateTime::Get_Millisecond> Millisecond{ this };

				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		DateTime(){}

		DateTime(std::nullptr_t const & n) : System::Object(n) {
		}

		DateTime(DateTime* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		DateTime(DateTime const & other) : System::Object(other) { }

		DateTime(DateTime&& other) noexcept : System::Object(std::move(other)) { }

		DateTime(Object::ObjectData* other) : System::Object(other) {
		}

		DateTime& operator=(DateTime const & other) {
			System::Object::operator=(other);
			return *this;
		}

		DateTime& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		DateTime& operator=(DateTime&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		DateTime& operator=(DateTime* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		DateTime* operator->() {
			return this;
		}



			DateTime(long ticks) : DateTime() {
			if (ticks < ObjectData::MinTicks || ticks > ObjectData::MaxTicks)
				throw ArgumentOutOfRangeException();
			ObjectData* dd = new ObjectData(ticks);
			this->od = dd;
		}

		DateTime(ulong dateData) {
			ObjectData* dd = new ObjectData(dateData);
			this->od = dd;
		}

		DateTime(int year, int month, int day) {
			if (!(year >= 1 && year <= 9999 && month >= 1 && month <= 12)) {
				throw ArgumentOutOfRangeException();
			}
			ObjectData* dd = new ObjectData(year, month, day);
			this->od = dd;
		}

		DateTime(int year, int month, int day, int hour, int minute, int second) {
			ObjectData* dd = new ObjectData(year, month, day, hour, minute, second);
			this->od = dd;
		}

		DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind) {
			ObjectData* dd = new ObjectData(year, month, day, hour, minute, second, kind);
			this->od = dd;
		}

		DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond) {
			if (millisecond < 0 || millisecond >= ObjectData::MillisPerSecond)
				throw ArgumentOutOfRangeException();
			ObjectData* dd = new ObjectData(year, month, day, hour, minute, second, millisecond);
			this->od = dd;
		}

		DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind) {
			if (millisecond < 0 || millisecond >= ObjectData::MillisPerSecond)
				throw ArgumentOutOfRangeException();
			ObjectData* dd = new ObjectData(year, month, day, hour, minute, second, millisecond, kind);
			this->od = dd;
		}


		static bool IsLeapYear(int const year) {
			return ObjectData::IsLeapYear(year);
		}
	};

#ifndef SYSTEM_EXPORTS
	PropGenGetStatic<DateTime, &DateTime::Get_Now> DateTime::Now;
	int const DateTime::ObjectData::DaysToMonth365[13] = {
		0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	int const DateTime::ObjectData::DaysToMonth366[13] = {
		0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
#endif


	///<summary>
	///Initializes a new instance of the System.Random class, using a time-dependent default seed value.
	///</summary>
	class System_API Random : public Object {
	private:

		uint64_t(&Get_Seed())[2]{
			return GOD()->Seed;
		}
	public:
		PropGenGetArray<uint64_t[2], uint64_t, Random, &Random::Get_Seed> Seed{ this };

		class System_API ObjectData : public Object::ObjectData {
		private:
			// http://en.wikipedia.org/wiki/Xorshift
			uint64_t xorshift128plus(void) {
				uint64_t s1 = Seed[0];
				const uint64_t s0 = Seed[1];
				Seed[0] = s0;
				s1 ^= s1 << 23;
				return (Seed[1] = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26))) + s0;
			}

		public:
			uint64_t Seed[2] = { 0, 0 };
			int Length = 0;

			ObjectData(ulong Seed) {
				this->Seed[0] = Seed;
				srand((uint)Seed);
			}

			ObjectData() : ObjectData(time(NULL)) {

			}
		};



				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

		Random(){}

		Random(std::nullptr_t const & n) : System::Object(n) {
		}

		Random(Random* pValue) {
			if (!pValue->od) {
				ObjectData* dd = new ObjectData();
				od = dd;
			}
			else {
				od = pValue->od;
				pValue->od = nullptr;
			}
			delete pValue;
		}

		Random(Random const & other) : System::Object(other) { }

		Random(Random&& other) noexcept : System::Object(std::move(other)) { }

		Random(Object::ObjectData* other) : System::Object(other) {
		}

		Random& operator=(Random const & other) {
			System::Object::operator=(other);
			return *this;
		}

		Random& operator=(std::nullptr_t const & n) {
			System::Object::operator=(n);
			return *this;
		}

		Random& operator=(Random&& other) noexcept {
			System::Object::operator=(std::move(other));
			return *this;
		}

		Random& operator=(Random* other) {
			if (od == other->od)
				return *this;
			Release();
			od = other->od;
			::operator delete((void*)other);
			return *this;
		}

		Random* operator->() {
			return this;
		}



			Random(ulong Seed) : Random() {
			ObjectData* dd = new ObjectData(Seed);
			this->od = dd;
		}



		Random& operator++ ()     // prefix ++
		{

			return *this;
		}

		///<summary>
		///Returns a non-negative random integer.
		///</summary>
		int Next() {
			return rand();
		}

		///<summary>
		///Returns a non-negative random integer that is less than the specified maximum.
		///</summary>
		int Next(int maxValue) {
			return rand() % maxValue;
		}

		///<summary>
		///Returns a random integer that is within a specified range.
		///</summary>
		int Next(int minValue, int maxValue) {
			return minValue + rand() % (maxValue - minValue);
		}

		///<summary>
		///Returns a random floating - point number that is greater than or equal to 0.0, and less than 1.0.
		///</summary>
		double NextDouble() {
			return rand() / (double)RAND_MAX;
		}

	};

	class Math {
	public:
		static constexpr double PI = 3.1415926535897931;

		static double Abs(double d) {
			if (d < 0)
				return 0 - d;
			return d;
		}

		static double Sqrt(double d) {
			return std::sqrt(d);
		}

		static double Atan(double d) {
			return atan(d);
		}

		static double Atan2(double d1, double d2) {
			return atan2(d1, d2);
		}

		template<class T> static T Min(T const & val1, T const & val2) {
			return val1 < val2 ? val1 : val2;
		}

		template<class T> static T Max(T const & val1, T const & val2) {
			return val1 > val2 ? val1 : val2;
		}
	};

	namespace Collections {
		class HashHelpers {
		public:
			static const int primes[];

			static bool IsPrime(int candidate) {
				if ((candidate & 1) != 0)
				{
					int limit = (int)std::sqrt(candidate);
					for (int divisor = 3; divisor <= limit; divisor += 2)
					{
						if ((candidate % divisor) == 0)
							return false;
					}
					return true;
				}
				return (candidate == 2);
			}

			static int GetPrime(int min) {
				for (int i = 0; i < 72; i++)
				{
					int prime = primes[i];
					if (prime >= min) return prime;
				}

				//outside of our predefined table. 
				//compute the hard way. 
				for (int i = (min | 1); i < 0x7fffffff; i += 2)
				{
					if (IsPrime(i) && ((i - 1) % 101 != 0))
						return i;
				}
				return min;
			}

			static int ExpandPrime(int oldSize)
			{
				int newSize = 2 * oldSize;

				// Allow the hashtables to grow to maximum possible size (~2G elements) before encoutering capacity overflow.
				// Note that this check works even when _items.Length overflowed thanks to the (uint) cast
				//if ((uint)newSize > MaxPrimeArrayLength && MaxPrimeArrayLength > oldSize)
				//{
				//	return MaxPrimeArrayLength;
				//}

				return GetPrime(newSize);
			}

		};

		const int HashHelpers::primes[] = {
		   3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
		   1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
		   17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
		   187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
		   1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369 };


		namespace Generic {


			template<class T> class System_API IComparer : public Object {
			private:
			public:
				typedef int (CompareFN)(const T&, const T&);

				class System_API ObjectData {
				private:
				public:
					virtual int Compare(const T& x, const T& y) const {
						if (x < y)
							return -1;
						return x == y ? 0 : 1;
					}

					static int Compare_static(const T& x, const T& y) {
						if (x < y)
							return -1;
						return x == y ? 0 : 1;
					}

					virtual typename IComparer<T>::CompareFN* GetFP_Compare() const {
						return &ObjectData::Compare_static;
					}

				};


								ObjectData* id = nullptr;

				ObjectData* GOD() const { return this->id; };

				IComparer(){}

				IComparer(std::nullptr_t const & n) : System::Object (nullptr), id(nullptr) {
				}

				IComparer(IComparer const & other) : System::Object(other) { this->id = other.id; }

				IComparer(IComparer&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

				IComparer(System::Object::ObjectData* baseod, ObjectData* other) : System::Object(baseod), id(other) {
				}

				IComparer& operator=(IComparer const & other) {
					System::Object::operator=(other);
					id = other.id;
					return *this;
				}

				IComparer& operator=(IComparer&& other) noexcept {
					System::Object::operator=(std::move(other));
					id = other.id;
					return *this;
				}

				IComparer<T>* operator->() {
					return this;
				}




					int Compare(const T& x, const T& y) const {
					return GOD()->Compare(x, y);
				}

				typename IComparer<T>::CompareFN* GetFP_Compare() const {
					return GOD()->GetFP_Compare();
				}
			};
			template<class T> class System_API IEqualityComparer : public Object {
			private:
			public:
				class System_API ObjectData {
				private:
				public:
					virtual bool Equals(const T& x, const T& y) const {
						return x == y;
					}

					virtual int GetHashCode(const T& value) const {
						//Object* ptr = dynamic_cast<Object*>(&value);
						//if (ptr)
						//	return ptr->GetHashCode();
						Object o = value;
						return o.GetHashCode();
					}
				};

								ObjectData* id = nullptr;

				ObjectData* GOD() const { return this->id; };

				IEqualityComparer(){}

				IEqualityComparer(std::nullptr_t const & n) : System::Object (nullptr), id(nullptr) {
				}

				IEqualityComparer(IEqualityComparer const & other) : System::Object(other) { this->id = other.id; }

				IEqualityComparer(IEqualityComparer&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

				IEqualityComparer(System::Object::ObjectData* baseod, ObjectData* other) : System::Object(baseod), id(other) {
				}

				IEqualityComparer& operator=(IEqualityComparer const & other) {
					System::Object::operator=(other);
					id = other.id;
					return *this;
				}

				IEqualityComparer& operator=(IEqualityComparer&& other) noexcept {
					System::Object::operator=(std::move(other));
					id = other.id;
					return *this;
				}

				IEqualityComparer<T>* operator->() {
					return this;
				}



					operator IEqualityComparer<T>() {
					IEqualityComparer<T> ret(this->od);
					return ret;
				}

				bool Equals(const T& x, const T& y) const {
					return GOD()->Equals(x, y);
				}

				int GetHashCode(const T& value) const {
					return GOD()->GetHashCode(value);
				}


			};
			template<class T> class System_API Comparer : public Object {
			private:
			public:
				class System_API ObjectData : public Object::ObjectData, public IComparer<T>::ObjectData {


				};



				static Comparer<T> Default;
				//	static int number;
				//	static bool StaticConstructorCalled;

					//static bool StaticConstructor() {
					//	number = 37;
					//	return true;
					//}


								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				Comparer(){}

				Comparer(std::nullptr_t const & n) : System::Object(n) {
				}

				Comparer(Comparer* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				Comparer(Comparer const & other) : System::Object(other) { }

				Comparer(Comparer&& other) noexcept : System::Object(std::move(other)) { }

				Comparer(Object::ObjectData* other) : System::Object(other) {
				}

				Comparer& operator=(Comparer const & other) {
					System::Object::operator=(other);
					return *this;
				}

				Comparer& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				Comparer& operator=(Comparer&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				Comparer& operator=(Comparer* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				Comparer* operator->() {
					return this;
				}



					operator IComparer<T>() {
					IComparer<T> ret(this->od, GOD());
					return ret;
				}

				int Compare(const T& x, const T& y) const {
					return GOD()->Compare(x, y);
				}

				//virtual int Compare(T x, T y) {
				//	if (x < y)
				//		return -1;
				//	return x > y ? 1 : 0;
				//}
				typename IComparer<T>::CompareFN* GetFP_Compare() const {
					return GOD()->GetFP_Compare();
				}

			};
			template<class T> class System_API EqualityComparer : public Object {
			private:
			public:
				class System_API ObjectData : public Object::ObjectData, public IEqualityComparer<T>::ObjectData {


				};

				static EqualityComparer<T> Default;


								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				EqualityComparer(){}

				EqualityComparer(std::nullptr_t const & n) : System::Object(n) {
				}

				EqualityComparer(EqualityComparer* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				EqualityComparer(EqualityComparer const & other) : System::Object(other) { }

				EqualityComparer(EqualityComparer&& other) noexcept : System::Object(std::move(other)) { }

				EqualityComparer(Object::ObjectData* other) : System::Object(other) {
				}

				EqualityComparer& operator=(EqualityComparer const & other) {
					System::Object::operator=(other);
					return *this;
				}

				EqualityComparer& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				EqualityComparer& operator=(EqualityComparer&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				EqualityComparer& operator=(EqualityComparer* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				EqualityComparer* operator->() {
					return this;
				}



					//virtual bool Equals(T x, T y) {
					//	return x == y;
					//}
					operator IEqualityComparer<T>() {
					IEqualityComparer<T> ret(od, GOD());
					return ret;
				}

			};
#ifndef SYSTEM_EXPORTS
			//		template<class T> int Comparer<T>::number(666);
			template<class T> Comparer<T> Comparer<T>::Default(new Comparer<T>());
			//	template<class T> bool Comparer<T>::StaticConstructorCalled = Comparer<T>::StaticConstructor();
			template<class T> EqualityComparer<T> EqualityComparer<T>::Default(new EqualityComparer<T>());
#endif	
			/// <summary>Supports a simple iteration over a generic collection.</summary>
			template<class T> class System_API IEnumerator : public Object
			{
			private:



			public:
				T& GetCurrent() const { return GOD()->GetCurrent(); }

				typedef T* (MoveNextGetCurrentFN)(void*);

				//typedef void (ResetFN)(void*);
				//typedef bool (MoveNextFN)(void*);

				class System_API ObjectData { //public System::Object::ObjectData {
				public:
					//struct IEnumerator_VTable {
					//	MoveNextGetCurrentFN* MoveNextGetCurrent = nullptr;
					//	ResetFN* Reset = nullptr;
					//	MoveNextFN* MoveNext = nullptr;

					//	IEnumerator_VTable() = default;
					//	IEnumerator_VTable(MoveNextGetCurrentFN* mngc, ResetFN* rst, MoveNextFN* mn) : MoveNextGetCurrent(mngc), Reset(rst), MoveNext(mn) {}
					//};

					//static IEnumerator_VTable vtable;

					//IEnumerator_VTable* vtableptr;


					//System::Object obj;
					//std::function<System::Object()> GetO;
					//std::function<void()> Reset;
					//std::function<bool()> MoveNext;
					//std::function<T& ()> GetCurrent;
					//std::function<void(const T&)> SetCurrent;

					MoveNextGetCurrentFN* mngc;
					T* mngc_cur;

					ObjectData() {
						//vtableptr = &vtable;
					}

					//template <class R>
					//ObjectData(R& o) :
						//obj(other),
						//GetO([o]() { return o; }),
						//Reset([o]() mutable { o.Reset(); }),
						//MoveNext([&o]() mutable -> bool { return o.MoveNext(); }),
						//GetCurrent([&o]()-> T& { return o.GetCurrent(); }),
						//SetCurrent([o](const T& value) mutable { o.SetCurrent(value); })
						//)
					//{
					//	this->other = other;
						//vtableptr = &vtable;
					//}

					~ObjectData() {
						//		ReleaseOther();
					}

					virtual void Reset() = 0;
					virtual bool MoveNext() = 0;
					virtual T* MoveNextGetCurrent() = 0;
					virtual MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const = 0;
					virtual T& GetCurrent() const = 0;
					virtual void SetCurrent(const T& value) = 0;
				};

				// for some reason VC++ needs these to be public (TODO FIX)
				void SetCurrent(T const & value) {
					return GOD()->SetCurrent(value);
				}
				void SetCurrent(T&& value) {
					return GOD()->SetCurrent(std::move(value));
				}

				/// <summary></summary>
				PropGen<T, T, IEnumerator<T>, &IEnumerator<T>::GetCurrent, &IEnumerator<T>::SetCurrent, &IEnumerator<T>::SetCurrent> Current{ this };

								ObjectData* id = nullptr;

				ObjectData* GOD() const { return this->id; };

				IEnumerator(){}

				IEnumerator(std::nullptr_t const & n) : System::Object (nullptr), id(nullptr) {
				}

				IEnumerator(IEnumerator const & other) : System::Object(other) { this->id = other.id; }

				IEnumerator(IEnumerator&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

				IEnumerator(System::Object::ObjectData* baseod, ObjectData* other) : System::Object(baseod), id(other) {
				}

				IEnumerator& operator=(IEnumerator const & other) {
					System::Object::operator=(other);
					id = other.id;
					return *this;
				}

				IEnumerator& operator=(IEnumerator&& other) noexcept {
					System::Object::operator=(std::move(other));
					id = other.id;
					return *this;
				}




					//template<class R>
					//IEnumerator(R& other) {
					//	ObjectData* dd = new ObjectData(other);
					//	this->od = dd;
					//}

					//	= PropGen<T, T, IEnumerator<T>, &IEnumerator<T>::Get_Current, &IEnumerator<T>::Set_Current>(this);

					//T* MoveNextGetCurrent() {
					//	return GOD()->MoveNextGetCurrent(od);
					//}

					//MoveNextGetCurrentFN* GetMoveNextGetCurrentFN() {
					//	ObjectData* god = GOD();
					//	//void** vtbl = *((void***)god);
					//	void** vtbl = *(((void***)god) + 1);
					//	return (MoveNextGetCurrentFN*)vtbl[0];
					//}


					/// <summary>Sets the enumerator to its initial position, which is before the first element in the collection.</summary>
					/// <returns></returns>
					void Reset() { GOD()->Reset(); }

				//ResetFN* GetResetFN() {
				//	ObjectData* god = GOD();
				//	//void** vtbl = *((void***)god);
				//	void** vtbl = *(((void***)god) + 1);
				//	return (ResetFN*)vtbl[1];
				//}

				/// <summary>Advances the enumerator to the next element of the collection.</summary>
				/// <returns>true if the enumerator was successfully advanced to the next element; false if the enumerator has passed the end of the collection.</returns>
				bool MoveNext() {
					return GOD()->MoveNext();
				}

				typename IEnumerator<T>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const {
					return GOD()->GetFP_MoveNextGetCurrent();
				}

				// iterator functions
				// iterator traits
				using difference_type = T;
				using value_type = T;
				using pointer = const T*;
				using reference = const T&;
				using iterator_category = std::forward_iterator_tag;



				IEnumerator<T>& operator++() {
					GOD()->mngc_cur = GOD()->mngc(this->od);
					return *this;
				}
				IEnumerator<T> operator++(int) {
					throw NotImplementedException();
					//IEnumerator<T> retval = *this; 
					//++(*this); 
					//return retval; 
				}
				bool operator==(IEnumerator<T> const & other) const {
					return GOD()->mngc_cur == nullptr; //GOD()->CheckEqual(other.GOD()->GetO());
				}
				bool operator!=(IEnumerator<T> const & other) const {
					return GOD()->mngc_cur != nullptr; //!(*this == other); 
				}
				T& operator*() {
					return *(GOD()->mngc_cur);// GetCurrent();
				}
				T& operator*() const {
					return *(GOD()->mngc_cur); // GetCurrent();
				}
				T* operator->() {
					return GOD()->mngc_cur; //(T*) &GetCurrent();
				}
				T* operator->() const {
					return GOD()->mngc_cur; //(T*) &GetCurrent();
				}
				// end iterator functions

			};
			//#ifndef SYSTEM_EXPORTS
					//	template <class T>
					//	typename IEnumerator<T>::ObjectData::IEnumerator_VTable IEnumerator<T>::ObjectData::vtable{};
			//#endif
						/// <summary>Exposes the enumerator, which supports a simple iteration over a collection of a specified type.</summary>
			template<class T> class System_API IEnumerable : public Object
			{
			public:
				//IEnumerable() = delete;
				class System_API ObjectData
				{

				public:

					virtual IEnumerator<T> GetEnumerator() const = 0;
					virtual IEnumerator<T> GetEnumeratorEnd() const = 0;
					virtual List<T> ToList() const;
					virtual Array<T> ToArray() const;
					virtual IEnumerable<T> Where(const Func<T, bool>& predicate) const;
					template<class R> IEnumerable<R> Select(const Func<T, R>& selector) const;
					template<class R> IEnumerable<R> OrderBy(const Func<T, R>& keySelector, const IComparer<R>& comparer) const;
					template<class R> IEnumerable<R> OrderBy(const Func<T, R>& keySelector) const;
					template<class R> T Max(const Func<T, R>& selector) const;
					template<class R> T Min(const Func<T, R>& selector) const;
				};


								ObjectData* id = nullptr;

				ObjectData* GOD() const { return this->id; };

				IEnumerable(){}

				IEnumerable(std::nullptr_t const & n) : System::Object (nullptr), id(nullptr) {
				}

				IEnumerable(IEnumerable const & other) : System::Object(other) { this->id = other.id; }

				IEnumerable(IEnumerable&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

				IEnumerable(System::Object::ObjectData* baseod, ObjectData* other) : System::Object(baseod), id(other) {
				}

				IEnumerable& operator=(IEnumerable const & other) {
					System::Object::operator=(other);
					id = other.id;
					return *this;
				}

				IEnumerable& operator=(IEnumerable&& other) noexcept {
					System::Object::operator=(std::move(other));
					id = other.id;
					return *this;
				}

				IEnumerable<T>* operator->() {
					return this;
				}



					/// <summary>Returns an enumerator that iterates through the collection.</summary>
					/// <returns></returns>
					IEnumerator<T> GetEnumerator() const { return GOD()->GetEnumerator(); }
				List<T> ToList() const { return od != nullptr ? GOD()->ToList() : nullptr; }
				Array<T> ToArray() const { return od != nullptr ? GOD()->ToArray() : nullptr; }
				IEnumerable<T> Where(const Func<T, bool>& predicate) const { return GOD()->Where(predicate); }
				template<class R> IEnumerable<R> Select(const Func<T, R>& selector) const { return GOD()->Select(selector); }
				template<class R> IEnumerable<R> OrderBy(const Func<T, R>& keySelector, const IComparer<R>& comparer) const { return GOD()->OrderBy(keySelector, comparer); }
				template<class R> IEnumerable<R> OrderBy(const Func<T, R>& keySelector) const { return GOD()->OrderBy(keySelector); }
				template<class R> T Max(const Func<T, R>& selector) const{ return GOD()->Max(selector); }
				template<class R> T Min(const Func<T, R>& selector) const { return GOD()->Min(selector); }

				IEnumerator<T> begin() const {
					IEnumerator<T> ret = GetEnumerator();
					ret.GOD()->mngc = ret.GetFP_MoveNextGetCurrent();
					++ret;
					return ret;
				}
				IEnumerator<T> end() const {
					return GOD()->GetEnumeratorEnd();
				}
			};
			/// <summary></summary>
			template<class T> class System_API SimpleEnumerator : public Object //IEnumerator<T>
			{
			public:
				class System_API ObjectData : public Object::ObjectData, public IEnumerator<T>::ObjectData {
				public:
					//struct SimpleEnumerator_VTable : public IEnumerator<T>::ObjectData::IEnumerator_VTable {
					//	//typename IEnumerator<T>::MoveNextGetCurrentFN* MoveNextGetCurrent;
					//	//typename IEnumerator<T>::ResetFN* Reset;
					//	SimpleEnumerator_VTable() = default;

					//	SimpleEnumerator_VTable(
					//		typename IEnumerator<T>::MoveNextGetCurrentFN* mngc, 
					//		typename IEnumerator<T>::ResetFN rst,
					//		typename IEnumerator<T>::MoveNextFN mn) 
					//		: IEnumerator<T>::ObjectData::IEnumerator_VTable(mngc, rst, mn) {}
					//};

					//static SimpleEnumerator_VTable vtable;

					T* dta;
					T* curptr;
					T* end;

					Object enulst;

					ObjectData() {
						//this->vtableptr = (typename IEnumerator<T>::ObjectData::IEnumerator_VTable*) &vtable;
						this->dta = nullptr;
						this->curptr = nullptr;
						this->end = nullptr;
					}

					ObjectData(Object const & enulst, T* dta, T* curptr, T* end) : dta(dta), curptr(curptr), end(end), enulst(enulst) {
						//this->vtableptr = (typename IEnumerator<T>::ObjectData::IEnumerator_VTable*) &vtable;
					}

					~ObjectData() override
					{
						//	enulst.Release();
					}



					T& GetCurrent() const override {
						return *curptr;
					}

					void SetCurrent(const T& value) override {
						*curptr = value;
					}


					static T* MoveNextGetCurrent(void* _this) {
						ObjectData* o = (ObjectData*)_this;
						if (++o->curptr != o->end)
							return o->curptr;
						else
							return nullptr;
					}

					T* MoveNextGetCurrent() override {
						return MoveNextGetCurrent(this);
					}

					typename IEnumerator<T>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const override {
						return &ObjectData::MoveNextGetCurrent;
					}

					void Reset() override { // (void* _this) {
						//ObjectData* o = (ObjectData*)_this;
						this->curptr = this->dta - 1;
					}

					bool MoveNext() override { //void* _this) {
						//ObjectData* o = (ObjectData*)_this;
						return ++this->curptr != this->end;
					}

				};



								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				SimpleEnumerator(){}

				SimpleEnumerator(std::nullptr_t const & n) : System::Object(n) {
				}

				SimpleEnumerator(SimpleEnumerator* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				SimpleEnumerator(SimpleEnumerator const & other) : System::Object(other) { }

				SimpleEnumerator(SimpleEnumerator&& other) noexcept : System::Object(std::move(other)) { }

				SimpleEnumerator(Object::ObjectData* other) : System::Object(other) {
				}

				SimpleEnumerator& operator=(SimpleEnumerator const & other) {
					System::Object::operator=(other);
					return *this;
				}

				SimpleEnumerator& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				SimpleEnumerator& operator=(SimpleEnumerator&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				SimpleEnumerator& operator=(SimpleEnumerator* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				SimpleEnumerator* operator->() {
					return this;
				}



					SimpleEnumerator(IEnumerable<T> const & enulst, T* dta, ulong length, int pos = -1) {
					ObjectData* d = new ObjectData(enulst, dta, dta + pos, dta + length);
					this->od = d;
				}

				operator IEnumerator<T>() const {
					IEnumerator<T> ret(GOD(), GOD());
					return ret;
				}

				T& GetCurrent() const {
					return GOD()->GetCurrent();
				}

				void SetCurrent(T const & value) {
					GOD()->SetCurrent(value);
				}

				void Reset() {
					GOD()->Reset();
				}

				bool MoveNext() {
					return GOD()->MoveNext();
				}

				typename IEnumerator<T>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const {
					return GOD()->GetFP_MoveNextGetCurrent();
				}

			};

			template<class T, class R> class System_API FuncEnumerator : public Object
			{
			public:
				class System_API ObjectData : public Object::ObjectData, public IEnumerator<T>::ObjectData {
				private:
					mutable T* cur = null;
					Func<T*, typename FuncEnumerator<T, T>::ObjectData*, T*> funcGetNext;

				public:
					R Tag;

					ObjectData() {
					}

					ObjectData(Func<T*, typename FuncEnumerator<T, T>::ObjectData*, T*> const & funcGetNext) : funcGetNext(funcGetNext) {
					}

					~ObjectData() override
					{
					}

					T& GetCurrent() const override {
						return *cur;
					}

					void SetCurrent(T const & value) override {
						*cur = value;
					}

					static T* MoveNextGetCurrent(void* _this) {
						ObjectData* o = (ObjectData*)_this;
						o->cur = o->funcGetNext(o->cur, o);

						return o->cur;
					}

					T* MoveNextGetCurrent() override {
						return MoveNextGetCurrent(this);
					}

					typename IEnumerator<T>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const override {
						return &ObjectData::MoveNextGetCurrent;
					}

					void Reset() override {
						cur = null;
					}

					bool MoveNext() override {
						cur = funcGetNext(cur, this);
						return cur != null;
					}

				};



								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				FuncEnumerator(){}

				FuncEnumerator(std::nullptr_t const & n) : System::Object(n) {
				}

				FuncEnumerator(FuncEnumerator* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				FuncEnumerator(FuncEnumerator const & other) : System::Object(other) { }

				FuncEnumerator(FuncEnumerator&& other) noexcept : System::Object(std::move(other)) { }

				FuncEnumerator(Object::ObjectData* other) : System::Object(other) {
				}

				FuncEnumerator& operator=(FuncEnumerator const & other) {
					System::Object::operator=(other);
					return *this;
				}

				FuncEnumerator& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				FuncEnumerator& operator=(FuncEnumerator&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				FuncEnumerator& operator=(FuncEnumerator* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				FuncEnumerator* operator->() {
					return this;
				}



				FuncEnumerator(Func<T*, typename FuncEnumerator<T, T>::ObjectData*, T*> const & funcGetNext) {
					this->od = new ObjectData(funcGetNext);
				}

				operator IEnumerator<T>() const {
					IEnumerator<T> ret(GOD(), GOD());
					return ret;
				}

				T& GetCurrent() const {
					return GOD()->GetCurrent();
				}

				void SetCurrent(const T& value) {
					GOD()->SetCurrent(value);
				}

				void Reset() {
					GOD()->Reset();
				}

				bool MoveNext() {
					return GOD()->MoveNext();
				}

				typename IEnumerator<T>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const {
					return GOD()->GetFP_MoveNextGetCurrent();
				}

			};


			template<class T> class System_API Array : public Object
			{
			private:
				int32_t Get_Length() const {
					return (int32_t)GOD()->Length;
				}

				//void Set_Length(int value) {
				////	((ArrayD<T>*)od)->Length = value;
				//}

			public:
				class System_API ObjectData : public Object::ObjectData
				{
				public:
					T* arrdta;
					ulong Length;


					ObjectData() {
						Length = 0;
						arrdta = NULL;
					}

					ObjectData(int length) {
						Length = length;
						//arrdta = (T*)ObjectD::Pool.Get((size_t)length * sizeof(T));
						arrdta = new T[length];
					}

					~ObjectData() override
					{
						//std::cout << "destroy called in array " << std::endl;
						if (arrdta) {
							delete[] arrdta;
							//	ObjectD::Pool.Put((byte*)arrdta);
						}
					}

					int32_t Get_Length() const {
						return (int32_t)Length;
					}

				};


				PropGenGet<int32_t, Array<T>, &Array<T>::Get_Length> Length{ this };// = PropGen<int, int, Array<T>, &Array<T>::Get_Length, &Array<T>::Set_Length>(this);

							ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			Array(){}

			Array(std::nullptr_t const & n) : System::Object(n) {
			}

			Array(Array* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			Array(Array const & other) : System::Object(other) { }

			Array(Array&& other) noexcept : System::Object(std::move(other)) { }

			Array(Object::ObjectData* other) : System::Object(other) {
			}

			Array& operator=(Array const & other) {
				System::Object::operator=(other);
				return *this;
			}

			Array& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			Array& operator=(Array&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			Array& operator=(Array* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			Array* operator->() {
				return this;
			}



					Array(const int32_t length) {
					ObjectData* dd = new ObjectData(length);
					od = dd;
				}

				Array(T* pValue) {
					ObjectData* dd = new ObjectData();

					// length in bytes is stored in a size_t right before the actual data...
					size_t* ptrlen = ((size_t*)pValue) - 1;
					dd->Length = (int)(*ptrlen / sizeof(T));
					dd->arrdta = pValue;

					this->od = dd;
				}

				Array(T* pValue, int len) {
					ObjectData* dd = new ObjectData();

					dd->Length = len;
					dd->arrdta = pValue;

					this->od = dd;
				}

				Array(const std::initializer_list<T> args) : Array((int)args.size()) {
					int ind = 0;
					ObjectData* ad = GOD();
					for (auto& arg : args)
					{
						ad->arrdta[ind++] = arg;
					}
				}

				operator const T* () const {
					return GOD()->arrdta;
				}

				operator T* () {
					return GOD()->arrdta;
				}


				T& operator[](const int index) {
					ObjectData* ad = GOD();
					if (index < 0 || index >= (int)ad->Length)
						throw IndexOutOfRangeException();

					return ad->arrdta[index];
				}

				const T& operator[](const int index) const {
					ObjectData* ad = GOD();
					if (index < 0 || index >= (int)ad->Length)
						throw IndexOutOfRangeException();

					return ad->arrdta[index];
				}

				//operator const T* () const {
				//	ArrayD<T>* ad = (ArrayD<T>*)od;
				//	return ad->arrdta;
				//}

			};
			template<class T> class System_API List : public Object
			{
			private:
				int GetCount() const {
					return this->od ? GOD()->Count : 0;
				}

				//void Set_Count(int& value) {
				//	//	((ArrayD<T>*)od)->Length = value;
				//}

			public:
				class System_API ObjectData : public Object::ObjectData, public IEnumerable<T>::ObjectData
				{
				private:
					//void EnsureCapacity(int desired) {
					//
					//}

				public:
					T* arrdta;
					int Count;
					int capacity;


					ObjectData() {
						this->Count = 0;
						this->capacity = 0;
						this->arrdta = NULL;
					}

					~ObjectData() override
					{
						//	std::cout << "destroy called in list " << std::endl;
						if (arrdta) {
							for (int i = 0; i < this->Count; i++) {
								arrdta[i].~T();
							}
							::operator delete(arrdta);
							//arrdta = nullptr;
						}
					}

					void Add(T const & item) {
						Insert(this->Count, item);
					}

					void Add(T&& item) {
						Insert(this->Count, std::move(item));
					}

					void Insert(int index, T const & item) {
						if (capacity == this->Count) {
							int newcapacity = capacity ? (capacity << 1) : 4;
							T* newarrdta = (T*) ::operator new(newcapacity * sizeof(T));
							for (int i = 0; i < index; i++) {
								new (&newarrdta[i]) T(std::move(this->arrdta[i]));
							}
							for (int i = index + 1; i < this->Count + 1; i++) {
								new (&newarrdta[i]) T(std::move(this->arrdta[i - 1]));
							}
							//std::memcpy(newarrdta, arrdta, index * sizeof(T));
							//std::memcpy(newarrdta + index + 1, arrdta + index, ((size_t)this->Count - index) * sizeof(T));
							new (&newarrdta[index]) T(item);

							if (this->arrdta)
								::operator delete(this->arrdta);
							this->arrdta = newarrdta;
							capacity = newcapacity;
						}
						else
						{
							for (int i = this->Count; i > index; i--) {
								this->arrdta[i] = std::move(this->arrdta[i - 1]);
							}
							new (&arrdta[index]) T(item);
						}
						this->Count++;
					}

					void Insert(int index, T&& item) {

						if (capacity == this->Count) {
							int newcapacity = capacity ? (capacity << 1) : 4;
							T* newarrdta = (T*) ::operator new(newcapacity * sizeof(T));
							for (int i = 0; i < index; i++) {
								new (&newarrdta[i]) T(std::move(this->arrdta[i]));
							}
							for (int i = index + 1; i < this->Count + 1; i++) {
								new (&newarrdta[i]) T(std::move(this->arrdta[i - 1]));
							}
							//std::memcpy(newarrdta, arrdta, index * sizeof(T));
							//std::memcpy(newarrdta + index + 1, arrdta + index, ((size_t)this->Count - index) * sizeof(T));
							new (&newarrdta[index]) T(std::move(item));

							if (this->arrdta)
								::operator delete(this->arrdta);
							this->arrdta = newarrdta;
							capacity = newcapacity;
						}
						else
						{
							for (int i = this->Count; i > index; i--) {
								this->arrdta[i] = std::move(this->arrdta[i - 1]);
							}
							new (&arrdta[index]) T(std::move(item));
						}
						this->Count++;
					}

					void AddRange(IEnumerable<T>& collection) {
						throw NotImplementedException();
					}

					void AddRange(IEnumerable<T>&& collection) {
						throw NotImplementedException();
					}

					void Clear() {
						for (int i = 0; i < this->Count; i++) {
							arrdta[i].~T();
						}
						::operator delete(arrdta);
						this->Count = 0;
					}

					IEnumerator<T> GetEnumerator() const override {
						IEnumerable<T> enu(const_cast<ObjectData*>(this), const_cast<ObjectData*>(this));

						SimpleEnumerator<T> ret = SimpleEnumerator<T>(enu, this->arrdta, this->Count);
						return ret;
					}

					IEnumerator<T> GetEnumeratorEnd() const override {
						IEnumerable<T> enu(const_cast<ObjectData*>(this), const_cast<ObjectData*>(this));

						SimpleEnumerator<T> ret = SimpleEnumerator<T>(enu, this->arrdta, this->Count, this->Count);
						return ret;
					}

					List<T> ToList() const override;

					IEnumerable<T> Where(const Func<T, bool>& predicate) const override;

				};

				/// <summary>Gets the number of elements contained in the List&lt;T&gt;.</summary>
				PropGenGet<int, List<T>, &List<T>::GetCount> Count{ this };

								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				List(){}

				List(std::nullptr_t const & n) : System::Object(n) {
				}

				List(List* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				List(List const & other) : System::Object(other) { }

				List(List&& other) noexcept : System::Object(std::move(other)) { }

				List(Object::ObjectData* other) : System::Object(other) {
				}

				List& operator=(List const & other) {
					System::Object::operator=(other);
					return *this;
				}

				List& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				List& operator=(List&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				List& operator=(List* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				List* operator->() {
					return this;
				}



					List(int capacity) {
					ObjectData* dd = new ObjectData();
					dd->Count = 0;
					dd->capacity = capacity;
					dd->arrdta = capacity ? (T*) ::operator new(capacity * sizeof(T)) : 0;
					this->od = dd;
				}

				List(T* pValue) {
					ObjectData* d = new ObjectData();
					// length is (hopefully) stored in a size_t right before the actual data...
					size_t* ptrlen = ((size_t*)pValue) - 1;
					d->Length = (int)*ptrlen;
					d->capacity = d->Length;
					d->arrdta = pValue;
					this->od = d;
				}

				List(std::initializer_list<T> args) {
					ObjectData* d = new ObjectData();
					d->capacity = static_cast<int>(args.size());
					if (d->capacity) {
						d->arrdta = (T*) ::operator new(d->capacity * sizeof(T));
						//if (!d->arrdta)
						//	throw OutOfMemoryException();
						//int ind = 0;
						T* ptr = d->arrdta;
						for (T arg : args)
						{
							//d->arrdta[ind++] = arg;
							//*ptr = arg;
							new (ptr) T(arg);
							ptr++;
						}
						d->Count = d->capacity;
					}

					this->od = d;
				}

				operator IEnumerable<T>() {
					IEnumerable<T> ret(this->od, static_cast<ObjectData*>(this->od));
					return ret;
				}

				operator const T* () const {
					return GOD()->arrdta;
				}

				operator T* () {
					return GOD()->arrdta;
				}

				~List()
				{
					//	std::cout << "List Destructor, od = " <<(size_t) od << std::endl;
				}

				//T operator[](const int index) const
				//{
				//	ObjectData* d = GOD();
				//	return d->arrdta[index];
				//}

				T& operator[](const int index) const
				{
					return GOD()->arrdta[index];
				}

				/// <summary>Adds an object to the end of the List&lt;T&gt;.</summary>
				/// <param name="item">The object to be added to the end of the List&lt;T&gt;. The value can be null for reference types.</param>
				/// <returns></returns>
				void Add(T const & item) const {
					GOD()->Add(item);
				}

				void Add(T&& item) const {
					GOD()->Add(std::move(item));
				}

				/// <summary>Inserts an element in the List&lt;T&gt; at the specified index.</summary>
				/// <param name="index">The zero-based index at which item should be inserted.</param>
				/// <param name="item">The object to insert. The value can be null for reference types.</param>
				/// <returns></returns>
				void Insert(int const index, T& item) const {
					GOD()->Insert(index, item);
				}

				void Insert(int const index, T&& item) const {
					GOD()->Insert(index, std::move(item));
				}

				void AddRange(IEnumerable<T> const & collection) const {
					GOD()->AddRange(collection);
				}

				void Clear() const {
					GOD()->Clear();
				}

				IEnumerator<T> GetEnumerator() const { return GOD()->GetEnumerator(); }
				List<T> ToList() const { return *this; }
				Array<T> ToArray() const { return od != nullptr ? GOD()->ToArray() : nullptr; }
				IEnumerable<T> Where(const Func<T, bool>& predicate) const { return GOD()->Where(predicate); }
				template<class R> IEnumerable<R> Select(const Func<T, R>& selector) const { return GOD()->Select(selector); }
				template<class R> IEnumerable<R> OrderBy(const Func<T, R>& keySelector, const IComparer<R>& comparer) const { return GOD()->OrderBy(keySelector, comparer); }
				template<class R> IEnumerable<R> OrderBy(const Func<T, R>& keySelector) const { return GOD()->OrderBy(keySelector); }
				template<class R> T Max(const Func<T, R>& selector) const { return GOD()->Max(selector); };
				template<class R> T Min(const Func<T, R>& selector) const { return GOD()->Min(selector); };

				IEnumerator<T> begin() const {
					IEnumerator<T> ret = GetEnumerator();
					ret.GOD()->mngc = ret.GetFP_MoveNextGetCurrent();
					++ret;
					return ret;
				}
				IEnumerator<T> end() const {
					return GOD()->GetEnumeratorEnd();
				}
			};
			template<class T> List<T> IEnumerable<T>::ObjectData::ToList() const {
				List<T> ret(0);
				IEnumerator<T> enu = this->GetEnumerator();
				while (enu.MoveNext()) {
					ret.Add(enu.Current);
				}

				return ret;
			};
			template<class T> List<T> List<T>::ObjectData::ToList() const {
				List<T> ret(const_cast<List<T>::ObjectData*>(this));
				return ret;
			};
			template<class T> Array<T> IEnumerable<T>::ObjectData::ToArray() const {
				int count = 0;
				IEnumerator<T> enu = this->GetEnumerator();
				while (enu.MoveNext()) {
					count++;
				}
				enu.Reset();


				Array<T> arr = new Array<T>(count);
				//int len = arr.Length;
				count = 0;
				while (enu.MoveNext()) {
					arr[count++] = std::move(enu.Current);
				}

				return arr;
			};
			template<class T> IEnumerable<T> IEnumerable<T>::ObjectData::Where(Func<T, bool> const & predicate) const {
				List<T> ret(0);

				IEnumerator<T> enu = this->GetEnumerator();
				typename IEnumerator<T>::MoveNextGetCurrentFN* ptr = enu.GetFP_MoveNextGetCurrent();

				T* cur;
				while ((cur = ptr(enu.od)) != null)
				{
					if (predicate(*cur)) {

						ret.Add(*cur);
					}
				}

				return ret;
			}
			template<class T> IEnumerable<T> List<T>::ObjectData::Where(Func<T, bool> const & predicate) const {
				List<T> ret(0);

				IEnumerator<T> enu = this->GetEnumerator();

				for (int i = 0; i < this->Count; i++)
				{
					T& cur = this->arrdta[i];
					if (predicate(cur)) {
						ret.Add(cur);
					}
				}

				return ret;
			}
			template<class T> template<class R> IEnumerable<R> IEnumerable<T>::ObjectData::Select(Func<T, R> const & selector) const {
				List<R> ret(0);
				IEnumerator<T> enu = this->GetEnumerator();
				//int ind = 0;
				while (enu.MoveNext()) {
					ret.Add(selector(enu.Current));
				}

				return ret;
			}
			template<class T, class R> void QSortList(T* lst, int low, int high, Func<T, R> const & keySelector, IComparer<R> const & comparer) {
				if (high - low <= 1)
					return;
				int pivotindex = low + ((high - low) >> 1);
				T pivot = lst[pivotindex];
				R pivotkey = keySelector(pivot);
				T tmp = lst[high - 1];
				lst[high - 1] = pivot;
				lst[pivotindex] = tmp;

				int i = low - 1;
				for (int j = low; j < high - 1; j++) {
					R key = keySelector(lst[j]);
					int cr = comparer.Compare(key, pivotkey);
					if (cr < 0) {
						i++;
						if (i < j) {
							tmp = lst[i];
							lst[i] = lst[j];
							lst[j] = tmp;
							//std::swap_ranges(&lst[i], &lst[i + 1], &lst[j]);
						}
					}
				}
				i++;
				lst[high - 1] = lst[i];
				lst[i] = pivot;


				QSortList(lst, low, i, keySelector, comparer);
				QSortList(lst, i + 1, high, keySelector, comparer);
			}
			template<class T> template<class R> IEnumerable<R> IEnumerable<T>::ObjectData::OrderBy(Func<T, R> const & keySelector, IComparer<R> const & comparer) const {
				struct lcl {
					R key;
					T* ptr;
				};

				List<lcl> arr(0);
				IEnumerator<T> enu = this->GetEnumerator();
				typename IEnumerator<T>::MoveNextGetCurrentFN* mngc = enu.GetFP_MoveNextGetCurrent();

				T* cur;
				while ((cur = mngc(enu.od)) != null)
				{
					arr.Add({ keySelector(*cur), cur });
				}
				typename IComparer<R>::CompareFN* comp = comparer.GetFP_Compare();
				//QSortList<T, R>(ret.GOD()->arrdta, 0, ret.Count, keySelector, comparer);
				std::sort(arr->GOD()->arrdta, arr->GOD()->arrdta + ((int)arr.Count), [&comp](lcl const & a, lcl const & b) -> bool {
					return comp(a.key, b.key) == -1;
					});
				List<T> ret(arr.Count);
				for (int i = 0; i < arr.Count; i++) {
					ret.Add(*arr[i].ptr);
				}

				return ret;
			}
			template<class T> template<class R> IEnumerable<R> IEnumerable<T>::ObjectData::OrderBy(const Func<T, R>& keySelector) const {
				return this->OrderBy(keySelector, (IComparer<R>)Comparer<R>::Default);
			}
			template<class T> template<class R> T IEnumerable<T>::ObjectData::Max(const Func<T, R>& selector) const {
				IEnumerator<T> enu = this->GetEnumerator();
				typename IEnumerator<T>::MoveNextGetCurrentFN* mngc = enu.GetFP_MoveNextGetCurrent();

				bool gotone = false;
				T max;
				R maxval;
				T* cur;
				while ((cur = mngc(enu.od)) != null)
				{
					if (!gotone) {
						max = *cur;
						maxval = selector(*cur);
						gotone = true;
						continue;
					}
					R curval = selector(*cur);
					if (curval > maxval) {
						max = *cur;
						maxval = curval;
					}
				}

				return max;
			}
			template<class T> template<class R> T IEnumerable<T>::ObjectData::Min(const Func<T, R>& selector) const {
				IEnumerator<T> enu = this->GetEnumerator();
				typename IEnumerator<T>::MoveNextGetCurrentFN* mngc = enu.GetFP_MoveNextGetCurrent();

				bool gotone = false;
				T min;
				R minval;
				T* cur;
				while ((cur = mngc(enu.od)) != null)
				{
					if (!gotone) {
						min = *cur;
						minval = selector(*cur);
						gotone = true;
						continue;
					}
					R curval = selector(*cur);
					if (curval < minval) {
						min = *cur;
						minval = curval;
					}
				}

				return min;
			}

			template<class T> class LinkedList;

			template<class T>
			struct ActualLinkedListNode {
			private:
			public:
				T Value;
				ActualLinkedListNode<T>* Next;
				ActualLinkedListNode<T>* Previous;

				ActualLinkedListNode(T const & value, ActualLinkedListNode<T>* next, ActualLinkedListNode<T>* previous) : Value(value), Next(next), Previous(previous) {
				
				}

				ActualLinkedListNode(T&& value, ActualLinkedListNode<T>* next, ActualLinkedListNode<T>* previous) : Value(std::move(value)), Next(next), Previous(previous) {

				}
			};

			template<class T> class System_API LinkedListNode : public Object
			{
			private:
				LinkedListNode<T> GetNext() const {
					return LinkedListNode<T>(GOD()->alln->Next);
				}

				LinkedListNode<T> GetPrevious() const {
					return LinkedListNode<T>(GOD()->alln->Previous);
				}

				T& GetValue() const {
					return GOD()->alln->Value;
				}

				//LinkedList<T> GetList() const {
				//	return GOD()->;
				//}

				template <class U>
				void SetValue(U&& value) {
					GOD()->alln->Value = std::forward<U>(value);
				}

			public:
				class System_API ObjectData : public Object::ObjectData
				{
				private:
				public:
					ActualLinkedListNode<T>* alln;

					ObjectData(ActualLinkedListNode<T>* alln) : alln(alln)  {
					
					}

					~ObjectData() override {
						//std::cout << "LinkedListNode destructor" << std::endl;
					}
				};

			//	PropGen<LinkedListNode<T>, LinkedListNode<T>, LinkedListNode<T>, &LinkedListNode<T>::GetNext, &LinkedListNode<T>::SetNext, &LinkedListNode<T>::SetNext> Next{ this };// = PropGen<bool&, bool, System::Threading::Thread, &System::Threading::Thread::Get_IsBackground, &System::Threading::Thread::Set_IsBackground>(this);
			//	PropGen<LinkedListNode<T>, LinkedListNode<T>, LinkedListNode<T>, &LinkedListNode<T>::GetPrevious, &LinkedListNode<T>::SetPrevious, &LinkedListNode<T>::SetPrevious> Previous{ this };// = PropGen<bool&, bool, System::Threading::Thread, &System::Threading::Thread::Get_IsBackground, &System::Threading::Thread::Set_IsBackground>(this);
				

				/// <summary>Gets the next node in the LinkedList&lt;T&gt;.</summary>
				PropGenGet<LinkedListNode<T>, LinkedListNode<T>, &LinkedListNode<T>::GetNext> Next{ this };
				/// <summary>Gets the previous node in the LinkedList&lt;T&gt;.</summary>
				PropGenGet<LinkedListNode<T>, LinkedListNode<T>, &LinkedListNode<T>::GetPrevious> Previous{ this };
				
				/// <summary>Gets the value contained in the node.</summary>
				PropGen<T, T, LinkedListNode<T>, &LinkedListNode<T>::GetValue, &LinkedListNode<T>::SetValue, &LinkedListNode<T>::SetValue> Value{ this };
				/// <summary>Gets the LinkedList&lt;T&gt; that the LinkedListNode&lt;T&gt; belongs to.</summary>
				//PropGenGet<LinkedList<T>, LinkedList<T>, &LinkedList<T>::GetList> List{ this };

				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				LinkedListNode(){}

				LinkedListNode(std::nullptr_t const & n) : System::Object(n) {
				}

				LinkedListNode(LinkedListNode* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				LinkedListNode(LinkedListNode const & other) : System::Object(other) { }

				LinkedListNode(LinkedListNode&& other) noexcept : System::Object(std::move(other)) { }

				LinkedListNode(Object::ObjectData* other) : System::Object(other) {
				}

				LinkedListNode& operator=(LinkedListNode const & other) {
					System::Object::operator=(other);
					return *this;
				}

				LinkedListNode& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				LinkedListNode& operator=(LinkedListNode&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				LinkedListNode& operator=(LinkedListNode* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				LinkedListNode* operator->() {
					return this;
				}



				LinkedListNode(ActualLinkedListNode<T>* alln) {
					od = new ObjectData(alln);
				}

				//LinkedListNode(T&& value) {
				//	od = new ObjectData(std::move(value));
				//}
			};

			template<class T> class System_API LinkedList : public Object
			{
			private:
				int GetCount() const {
					return this->od ? GOD()->Count : 0;
				}

				LinkedListNode<T> GetFirst() const {
					return LinkedListNode<T>(GOD()->GetFirst());
				}

				LinkedListNode<T> GetLast() const {
					return LinkedListNode<T>(GOD()->GetLast());
				}

			public:
				class System_API Enumerator : public Object
				{
				public:
					class System_API ObjectData : public Object::ObjectData, public IEnumerator<T>::ObjectData {
					private:
						ActualLinkedListNode<T>* alln;
						ActualLinkedListNode<T>* first;
					public:
						ObjectData(ActualLinkedListNode<T>* alln) : alln(null), first(alln) {
						}

						ObjectData() : alln(null) {
						}

						~ObjectData() override
						{
						}

						T& GetCurrent() const override {
							return alln->Value;
						}

						void SetCurrent(T const& value) override {
							alln->Value = value;
						}

						static T* MoveNextGetCurrent(void* _this) {
							ObjectData* o = (ObjectData*)_this;
							if (!o->alln)
								o->alln = o->first;
							else {
								o->alln = o->alln->Next;
								// check for circular lists
								if (o->alln == o->first)
									return null;
							}
							if (o->alln)
								return &o->alln->Value;
							return null;
						}

						T* MoveNextGetCurrent() override {
							return MoveNextGetCurrent(this);
						}

						typename IEnumerator<T>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const override {
							return &ObjectData::MoveNextGetCurrent;
						}

						void Reset() override {
							alln = null;
						}

						bool MoveNext() override {
							if (!alln) {
								alln = first;
								return alln != null;
							}
							alln = alln->Next;
							return alln != null && alln != first;
						}

					};



				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				Enumerator(){}

				Enumerator(std::nullptr_t const & n) : System::Object(n) {
				}

				Enumerator(Enumerator* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				Enumerator(Enumerator const & other) : System::Object(other) { }

				Enumerator(Enumerator&& other) noexcept : System::Object(std::move(other)) { }

				Enumerator(Object::ObjectData* other) : System::Object(other) {
				}

				Enumerator& operator=(Enumerator const & other) {
					System::Object::operator=(other);
					return *this;
				}

				Enumerator& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				Enumerator& operator=(Enumerator&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				Enumerator& operator=(Enumerator* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				Enumerator* operator->() {
					return this;
				}



					Enumerator(ActualLinkedListNode<T>* first) {
						this->od = new ObjectData(first);
					}

					operator IEnumerator<T>() const {
						IEnumerator<T> ret(GOD(), GOD());
						return ret;
					}

					T& GetCurrent() const {
						return GOD()->GetCurrent();
					}

					void SetCurrent(T const & value) const {
						GOD()->SetCurrent(value);
					}

					void Reset() const {
						GOD()->Reset();
					}

					bool MoveNext() const {
						return GOD()->MoveNext();
					}

					typename IEnumerator<T>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const {
						return GOD()->GetFP_MoveNextGetCurrent();
					}

				};



				class System_API ObjectData : public Object::ObjectData, public IEnumerable<T>::ObjectData
				{
				private:
					ActualLinkedListNode<T>* head;
					ActualLinkedListNode<T>* tail;
				public:
					int Count;

					ObjectData() : Count(0), head(null) , tail(null) {
					}

					~ObjectData() override
					{
						Clear();
					}

					LinkedListNode<T> GetFirst() {
						return LinkedListNode<T>(head);
					}

					LinkedListNode<T> GetLast() {
						return LinkedListNode<T>(tail);
					}

					void MakeCircular() {
						tail->Next = head;
						head->Previous = tail;
					}

					LinkedListNode<T> AddFirst(T const & value) {
						ActualLinkedListNode<T>* newhead = new ActualLinkedListNode<T>(value, head, null);
						if (head != null) {
							head->Previous = newhead;
							head = newhead;
						}
						else {
							head = newhead;
							tail = head;
						}
						Count++;

						return LinkedListNode<T>(newhead);
					}

					LinkedListNode<T> AddFirst(T&& value) {
						ActualLinkedListNode<T>* newhead = new ActualLinkedListNode<T>(std::move(value), head, null);
						if (head != null) {
							head->Previous = newhead;
							head = newhead;
						}
						else {
							head = newhead;
							tail = head;
						}
						Count++;

						return LinkedListNode<T>(newhead);
					}

					LinkedListNode<T> AddAfter(LinkedListNode<T> const & llnode, T const& value) {
						ActualLinkedListNode<T>* newnode;
						ActualLinkedListNode<T>* node = llnode.GOD()->alln;

						if (node->Next == null) {
							newnode = new ActualLinkedListNode<T>(value, null, node);
							node->Next = newnode;
							tail = newnode;
						}
						else {
							newnode = new ActualLinkedListNode<T>(value, node->Next, node);
							node->Next->Previous = newnode;
							node->Next = newnode;
						}
						Count++;
						
						return LinkedListNode<T>(newnode);
					}

					LinkedListNode<T> AddAfter(LinkedListNode<T> const & llnode, T&& value) {
						ActualLinkedListNode<T>* newnode;
						ActualLinkedListNode<T>* node = llnode.GOD()->alln;

						if (node->Next == null) {
							newnode = new ActualLinkedListNode<T>(std::move(value), null, node);
							node->Next = newnode;
							tail = newnode;
						}
						else {
							newnode = new ActualLinkedListNode<T>(std::move(value), node->Next, node);
							node->Next->Previous = newnode;
							node->Next = newnode;
						}
						Count++;

						return LinkedListNode<T>(newnode);
					}

					void Clear() {
						ActualLinkedListNode<T>* tmp = head;
						while (tmp) {
							ActualLinkedListNode<T>* tmp2 = tmp;
							tmp = tmp->Next;
							delete tmp2;
							if (tmp == head) // in case the list is circular...
								break;
						}
						head = null;
						// tail = null; // no need to nullify tail
						Count = 0;
					}

					void Remove(LinkedListNode<T> const & llnode) {
						ActualLinkedListNode<T>* node = llnode.GOD()->alln;
						if (node->Next != null) {
							node->Next->Previous = node->Previous;
						}
						if (node->Previous != null) {
							node->Previous->Next = node->Next;
						}
						delete node;
						Count--;
					}

					
					IEnumerator<T> GetEnumerator() const override {
						Enumerator ret = Enumerator(head);
						return ret;
					}

					IEnumerator<T> GetEnumeratorEnd() const override {
						return null;
					}
				};

				/// <summary>Gets the number of elements contained in the LinkedList&lt;T&gt;.</summary>
				PropGenGet<int, LinkedList<T>, &LinkedList<T>::GetCount> Count{ this };
				/// <summary>Gets the first node of the LinkedList&lt;T&gt;.</summary>
				PropGenGet<LinkedListNode<T>, LinkedList<T>, &LinkedList<T>::GetFirst> First{ this };
				/// <summary>Gets the last node of the LinkedList&lt;T&gt;.</summary>
				PropGenGet<LinkedListNode<T>, LinkedList<T>, &LinkedList<T>::GetLast> Last{ this };


				ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				LinkedList(){}

				LinkedList(std::nullptr_t const & n) : System::Object(n) {
				}

				LinkedList(LinkedList* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				LinkedList(LinkedList const & other) : System::Object(other) { }

				LinkedList(LinkedList&& other) noexcept : System::Object(std::move(other)) { }

				LinkedList(Object::ObjectData* other) : System::Object(other) {
				}

				LinkedList& operator=(LinkedList const & other) {
					System::Object::operator=(other);
					return *this;
				}

				LinkedList& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				LinkedList& operator=(LinkedList&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				LinkedList& operator=(LinkedList* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				LinkedList* operator->() {
					return this;
				}



				//List(std::initializer_list<T> args) {
				//	ObjectData* d = new ObjectData();
				//	d->capacity = static_cast<int>(args.size());
				//	if (d->capacity) {
				//		d->arrdta = (T*) ::operator new(d->capacity * sizeof(T));
				//		//if (!d->arrdta)
				//		//	throw OutOfMemoryException();
				//		//int ind = 0;
				//		T* ptr = d->arrdta;
				//		for (T arg : args)
				//		{
				//			//d->arrdta[ind++] = arg;
				//			//*ptr = arg;
				//			new (ptr) T(arg);
				//			ptr++;
				//		}
				//		d->Count = d->capacity;
				//	}

				//	this->od = d;
				//}

				operator IEnumerable<LinkedListNode<T>>() {
					IEnumerable<LinkedListNode<T>> ret(this->od, static_cast<ObjectData*>(this->od));
					return ret;
				}

				/// <summary>Makes the LinkedList&lt;T&gt; circular.</summary>
				/// <returns></returns>
				void MakeCircular() const {
					GOD()->MakeCircular();
				}

				/// <summary>Adds a new node containing the specified value at the start of the LinkedListt&lt;T&gt;.</summary>
				/// <param name="value">The value to add at the start of the LinkedListt&lt;T&gt;.</param>
				/// <returns>The new LinkedListNodet&lt;T&gt; containing value.</returns>
				//LinkedListNode<T> AddFirst(T const & value) const {
				//	return GOD()->AddFirst(value);
				//}

				/// <summary>Adds a new node containing the specified value at the start of the LinkedListt&lt;T&gt;.</summary>
				/// <param name="value">The value to add at the start of the LinkedListt&lt;T&gt;.</param>
				/// <returns>The new LinkedListNodet&lt;T&gt; containing value.</returns>
				template <class U>
				LinkedListNode<T> AddFirst(U&& value) const {
					return GOD()->AddFirst(std::forward<U>(value));
				}

				/// <summary>Adds a new node containing the specified value after the specified existing node in the LinkedListt&lt;T&gt;.</summary>
				/// <param name="node">The LinkedListNodet&lt;T&gt; after which to insert a new LinkedListNodet&lt;T&gt; containing value.</param>
				/// <param name="value">The value to add to the LinkedListt&lt;T&gt;.</param>
				/// <returns>The new LinkedListNodet&lt;T&gt; containing value.</returns>
				LinkedListNode<T> AddAfter(LinkedListNode<T> const & node, T const& value) const {
					return GOD()->AddAfter(node, value);
				}

				/// <summary>Adds a new node containing the specified value after the specified existing node in the LinkedListt&lt;T&gt;.</summary>
				/// <param name="node">The LinkedListNode<T> after which to insert a new LinkedListNodet&lt;T&gt; containing value.</param>
				/// <param name="value">The value to add to the LinkedListt&lt;T&gt;.</param>
				/// <returns>The new LinkedListNodet&lt;T&gt; containing value.</returns>
				LinkedListNode<T> AddAfter(LinkedListNode<T> const & node, T&& value) const {
					return GOD()->AddAfter(node, std::move(value));
				}

				/// <summary>Removes the specified node from the LinkedListt&lt;T&gt;.</summary>
				/// <returns></returns>
				void Remove(LinkedListNode<T> const & node) const {
					GOD()->Remove(node);
				}

				/// <summary>Removes all nodes from the LinkedListt&lt;T&gt;.</summary>
				/// <returns></returns>
				void Clear() const {
					GOD()->Clear();
				}

				IEnumerator<T> GetEnumerator() const { return GOD()->GetEnumerator(); }
				List<T> ToList() const { return od != nullptr ? GOD()->ToList() : nullptr; }
				Array<T> ToArray() const { return od != nullptr ? GOD()->ToArray() : nullptr; }
				IEnumerable<T> Where(Func<T, bool> const & predicate) const { return GOD()->Where(predicate); }
				template<class R> IEnumerable<R> Select(Func<T, R> const & selector) const { return GOD()->Select(selector); }
				template<class R> IEnumerable<R> OrderBy(Func<T, R> const & keySelector, const IComparer<R>& comparer) const { return GOD()->OrderBy(keySelector, comparer); }
				template<class R> IEnumerable<R> OrderBy(Func<T, R> const & keySelector) const { return GOD()->OrderBy(keySelector); }
				template<class R> T Max(Func<T, R> const & selector) const { return GOD()->Max(selector); };
				template<class R> T Min(Func<T, R> const& selector) const { return GOD()->Min(selector); };

				IEnumerator<T> begin() const {
					IEnumerator<T> ret = GetEnumerator();
					ret.GOD()->mngc = ret.GetFP_MoveNextGetCurrent();
					++ret;
					return ret;
				}
				IEnumerator<T> end() const {
					return GOD()->GetEnumeratorEnd();
				}
			};



			template <class TKey, class TValue> struct DictionaryEntry {
				int hashCode = 0;    // Lower 31 bits of hash code, -1 if unused
				int next = 0;        // Index of next entry, -1 if last
				TKey key{};           // Key of entry
				TValue value{};         // Value of entry
			};
			template <class TKey, class TValue> class System_API KeyValuePair : public Object {
			private:
			public:


				class System_API ObjectData : public Object::ObjectData {
				public:
					// same as in Dictionary

					//TKey key;
					//TValue value;
					DictionaryEntry<TKey, TValue>* dta;
				};

				//KeyValuePair(TKey key, TValue value) {
				//	ObjectData* dd = new ObjectData();
				//	dd->key = key;
				//	dd->value = value;
				//	od = dd;
				//}



				TKey& Get_Key() const {
					return GOD()->dta->key;
				}

				void Set_Key(const TKey& value) {
					GOD()->dta->key = value;
				}

				void Set_Key(TKey&& value) {
					GOD()->dta->key = value;
				}

				PropGen<TKey, TKey, System::Collections::Generic::KeyValuePair<TKey, TValue>, &System::Collections::Generic::KeyValuePair<TKey, TValue>::Get_Key, &System::Collections::Generic::KeyValuePair<TKey, TValue>::Set_Key, &System::Collections::Generic::KeyValuePair<TKey, TValue>::Set_Key> Key{ this };

				TValue& Get_Value() const {
					return GOD()->dta->value;
				}

				void Set_Value(const TValue& value) {
					GOD()->dta->value = value;
				}

				void Set_Value(TValue&& value) {
					GOD()->dta->value = value;
				}

				PropGen<TValue, TValue, System::Collections::Generic::KeyValuePair<TKey, TValue>, &System::Collections::Generic::KeyValuePair<TKey, TValue>::Get_Value, &System::Collections::Generic::KeyValuePair<TKey, TValue>::Set_Value, &System::Collections::Generic::KeyValuePair<TKey, TValue>::Set_Value> Value{ this };


							ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			KeyValuePair(){}

			KeyValuePair(std::nullptr_t const & n) : System::Object(n) {
			}

			KeyValuePair(KeyValuePair* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			KeyValuePair(KeyValuePair const & other) : System::Object(other) { }

			KeyValuePair(KeyValuePair&& other) noexcept : System::Object(std::move(other)) { }

			KeyValuePair(Object::ObjectData* other) : System::Object(other) {
			}

			KeyValuePair& operator=(KeyValuePair const & other) {
				System::Object::operator=(other);
				return *this;
			}

			KeyValuePair& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			KeyValuePair& operator=(KeyValuePair&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			KeyValuePair& operator=(KeyValuePair* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			KeyValuePair* operator->() {
				return this;
			}



					KeyValuePair(DictionaryEntry<TKey, TValue>* entry) {
					ObjectData* dd = new ObjectData();
					dd->dta = entry;
					od = dd;
				}

			};

			template<class TKey, class TValue> class System_API KeyValuePairEnumerator : public Object //IEnumerator<T>
			{
			public:
				class System_API ObjectData : public Object::ObjectData, public IEnumerator<KeyValuePair<TKey, TValue>>::ObjectData {
				public:

					KeyValuePair<TKey, TValue>* kvp = nullptr;

					DictionaryEntry<TKey, TValue>* dta;
					DictionaryEntry<TKey, TValue>* curptr;
					DictionaryEntry<TKey, TValue>* end;

					Object enulst;

					ObjectData() {
						//this->vtableptr = (typename IEnumerator<T>::ObjectData::IEnumerator_VTable*) &vtable;
						this->dta = nullptr;
						this->curptr = nullptr;
						this->end = nullptr;
					}

					ObjectData(Object const & enulst, DictionaryEntry<TKey, TValue>* dta, DictionaryEntry<TKey, TValue>* curptr, DictionaryEntry<TKey, TValue>* end) : dta(dta), curptr(curptr), end(end), enulst(enulst) {
						//this->vtableptr = (typename IEnumerator<T>::ObjectData::IEnumerator_VTable*) &vtable;
						if (curptr != end)
							kvp = new KeyValuePair<TKey, TValue>(curptr);
					}

					~ObjectData() override
					{
						//	enulst.Release();
						if (kvp != nullptr) {
							delete kvp;
						}
					}

					KeyValuePair<TKey, TValue>& GetCurrent() const override {
						return *kvp;
					}

					void SetCurrent(KeyValuePair<TKey, TValue> const& value) override {
						kvp->Value = value.Value;
						// not safe to set key
					}


					static KeyValuePair<TKey, TValue>* MoveNextGetCurrent(void* _this) {
						ObjectData* o = (ObjectData*)_this;
						while (++o->curptr != o->end && o->curptr->hashCode < 0) {}
						if (o->curptr != o->end) {
							o->kvp->GOD()->dta = o->curptr;
							return o->kvp;
						}
						else
							return nullptr;
					}

					KeyValuePair<TKey, TValue>* MoveNextGetCurrent() override {
						return MoveNextGetCurrent(this);
					}

					typename IEnumerator<KeyValuePair<TKey, TValue>>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const override {
						return &ObjectData::MoveNextGetCurrent;
					}

					void Reset() override {
						this->curptr = this->dta - 1;
					}

					bool MoveNext() override {
						return ++this->curptr != this->end;
					}
				};

								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				KeyValuePairEnumerator(){}

				KeyValuePairEnumerator(std::nullptr_t const & n) : System::Object(n) {
				}

				KeyValuePairEnumerator(KeyValuePairEnumerator* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				KeyValuePairEnumerator(KeyValuePairEnumerator const & other) : System::Object(other) { }

				KeyValuePairEnumerator(KeyValuePairEnumerator&& other) noexcept : System::Object(std::move(other)) { }

				KeyValuePairEnumerator(Object::ObjectData* other) : System::Object(other) {
				}

				KeyValuePairEnumerator& operator=(KeyValuePairEnumerator const & other) {
					System::Object::operator=(other);
					return *this;
				}

				KeyValuePairEnumerator& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				KeyValuePairEnumerator& operator=(KeyValuePairEnumerator&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				KeyValuePairEnumerator& operator=(KeyValuePairEnumerator* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				KeyValuePairEnumerator* operator->() {
					return this;
				}



					KeyValuePairEnumerator(IEnumerable<KeyValuePair<TKey, TValue>> const & enulst, DictionaryEntry<TKey, TValue>* dta, ulong length, int pos = -1) {
					ObjectData* d = new ObjectData(enulst, dta, dta + pos, dta + length);
					this->od = d;
				}

				operator IEnumerator<KeyValuePair<TKey, TValue>>() const {
					IEnumerator<KeyValuePair<TKey, TValue>> ret(GOD(), GOD());
					return ret;
				}

				KeyValuePair<TKey, TValue>& GetCurrent() const {
					return GOD()->GetCurrent();
				}

				void SetCurrent(const KeyValuePair<TKey, TValue>& value) {
					GOD()->SetCurrent(value);
				}

				void Reset() {
					GOD()->Reset();
				}

				bool MoveNext() {
					return GOD()->MoveNext();
				}

				typename IEnumerator<KeyValuePair<TKey, TValue>>::MoveNextGetCurrentFN* GetFP_MoveNextGetCurrent() const {
					return GOD()->GetFP_MoveNextGetCurrent();
				}

			};

			template<class TKey, class TValue> class System_API Dictionary : public Object {
			public:
				class System_API ObjectData : public Object::ObjectData, public IEnumerable<KeyValuePair<TKey, TValue>>::ObjectData {
				public:

					int* buckets;
					int bucketsLength;
					DictionaryEntry<TKey, TValue>* entries;
					int count;
					int capacity;
					int freeList;
					int freeCount;
					//	int version;
					IEqualityComparer<TKey> comparer;

					void Initialize(int capacity) {
						int size = HashHelpers::GetPrime(capacity);
						bucketsLength = size;
						//buckets = (int*) ObjectD::Pool.Get((size_t)size << 2);
						buckets = new int[size];
						for (int i = 0; i < size; i++)
							buckets[i] = -1;
						//entries = (Entry*)ObjectD::Pool.Get(sizeof(Entry) * size);
						entries = new DictionaryEntry<TKey, TValue>[size];
						freeList = -1;
					}



					ObjectData(int capacity, IEqualityComparer<TKey> comparer) {
						//if (capacity < 0) ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.capacity);
						count = 0;
						//	version = 0;
						freeCount = 0;
						buckets = null;

						if (capacity > 0)
							Initialize(capacity);
						this->comparer = comparer;

					}

					ObjectData(int capacity) : ObjectData(capacity, EqualityComparer<TKey>::Default) {}

					ObjectData(IEqualityComparer<TKey> comparer) : ObjectData(0, comparer) {}

					ObjectData() : ObjectData(0) {}

					~ObjectData() {
						if (buckets) {
							delete[] buckets;
							delete[] entries;
						}
					}

					void Resize() {
						Resize(HashHelpers::ExpandPrime(count), false);
					}

					void Resize(int newSize, bool forceNewHashCodes) {
						int* newBuckets = new int[(size_t)newSize];
						DictionaryEntry<TKey, TValue>* newEntries = new DictionaryEntry<TKey, TValue>[newSize];
						for (int i = 0; i < newSize; i++) {
							newBuckets[i] = -1;
						}
						for (int i = 0; i < count; i++) {
							newEntries[i] = entries[i];
						}
						//if (forceNewHashCodes) {
						//	for (int i = 0; i < count; i++) {
						//		if (newEntries[i].hashCode != -1) {
						//			newEntries[i].hashCode = (comparer.GetHashCode(newEntries[i].key) & 0x7FFFFFFF);
						//		}
						//	}
						//}
						for (int i = 0; i < count; i++) {
							if (newEntries[i].hashCode >= 0) {
								int bucket = newEntries[i].hashCode % newSize;
								newEntries[i].next = newBuckets[bucket];
								newBuckets[bucket] = i;
							}
						}

						delete[] buckets;
						delete[] entries;
						buckets = newBuckets;
						entries = newEntries;
						bucketsLength = newSize;
					}

					int Insert(const TKey& key, const TValue& value, bool add) {
						if (!buckets)
							Initialize(0);

						//				Object o = key;
						int hashCode = comparer.GetHashCode(key) & 0x7FFFFFFF;
						int targetBucket = hashCode % bucketsLength;
						for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next) {
							if (entries[i].hashCode == hashCode && comparer.Equals(entries[i].key, key)) {
								if (add) {
									//ThrowHelper.ThrowArgumentException(ExceptionResource.Argument_AddingDuplicate);
									throw Exception();
								}
								entries[i].value = value;
								//	version++;
								return i;
							}
						}
						int index;
						if (freeCount > 0) {
							index = freeList;
							freeList = entries[index].next;
							freeCount--;
						}
						else {
							if (count == bucketsLength)
							{
								Resize();
								targetBucket = hashCode % bucketsLength;
							}
							index = count;
							count++;
						}

						entries[index].hashCode = hashCode;
						entries[index].next = buckets[targetBucket];
						entries[index].key = key;
						entries[index].value = value;
						buckets[targetBucket] = index;
						//	version++;
						return index;
					}

					int Insert(const TKey& key, TValue&& value, bool add) {
						if (!buckets)
							Initialize(0);

						//				Object o = key;
						int hashCode = comparer.GetHashCode(key) & 0x7FFFFFFF;
						int targetBucket = hashCode % bucketsLength;
						for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next) {
							if (entries[i].hashCode == hashCode && comparer.Equals(entries[i].key, key)) {
								if (add) {
									//ThrowHelper.ThrowArgumentException(ExceptionResource.Argument_AddingDuplicate);
									throw Exception();
								}
								entries[i].value = (TValue&&)value;
								//	version++;
								return i;
							}
						}
						int index;
						if (freeCount > 0) {
							index = freeList;
							freeList = entries[index].next;
							freeCount--;
						}
						else {
							if (count == bucketsLength)
							{
								Resize();
								targetBucket = hashCode % bucketsLength;
							}
							index = count;
							count++;
						}

						entries[index].hashCode = hashCode;
						entries[index].next = buckets[targetBucket];
						entries[index].key = key;
						entries[index].value = value;
						buckets[targetBucket] = index;
						//	version++;
						return index;
					}

					int FindEntry(const TKey& key) {
						//if (key == null) {
						//	throw NullReferenceException();
						//}

						if (buckets) {
							int hashCode = comparer.GetHashCode(key) & 0x7FFFFFFF;
							for (int i = buckets[hashCode % bucketsLength]; i >= 0; i = entries[i].next) {
								if (entries[i].hashCode == hashCode && comparer.Equals(entries[i].key, key)) return i;
							}
						}
						return -1;
					}

					TValue* TryGetValue(const TKey& key) {
						int i = FindEntry(key);
						if (i >= 0) {
							return &(entries[i].value);
							//return true;
						}
						//value = TValue();
						return NULL;
					}

					bool Remove(TKey const& key) {
						if (buckets != null) {
							int hashCode = comparer.GetHashCode(key) & 0x7FFFFFFF;
							int bucket = hashCode % bucketsLength;
							int last = -1;
							for (int i = buckets[bucket]; i >= 0; last = i, i = entries[i].next) {
								if (entries[i].hashCode == hashCode && comparer.Equals(entries[i].key, key)) {
									if (last < 0) {
										buckets[bucket] = entries[i].next;
									}
									else {
										entries[last].next = entries[i].next;
									}
									entries[i].hashCode = -1;
									entries[i].next = freeList;
									entries[i].key = TKey();
									entries[i].value = TValue();
									freeList = i;
									freeCount++;
									//version++;
									return true;
								}
							}
						}
						return false;
					}

					IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator() const override {
						IEnumerable<KeyValuePair<TKey, TValue>> enu(const_cast<ObjectData*>(this), const_cast<ObjectData*>(this));

						KeyValuePairEnumerator<TKey, TValue> ret = KeyValuePairEnumerator<TKey, TValue>(enu, entries, this->count);
						return ret;
					}

					IEnumerator<KeyValuePair<TKey, TValue>> GetEnumeratorEnd() const override {
						IEnumerable<KeyValuePair<TKey, TValue>> enu(const_cast<ObjectData*>(this), const_cast<ObjectData*>(this));

						KeyValuePairEnumerator<TKey, TValue> ret = KeyValuePairEnumerator<TKey, TValue>(enu, entries, this->count, this->count);
						return ret;
					}

					//					List<KeyValuePair<TKey, TValue>> ToList() const override;

					//					IEnumerable<KeyValuePair<TKey, TValue>> Where(const Func<KeyValuePair<TKey, TValue>, bool>& predicate) const override;


				};



							ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			Dictionary(){}

			Dictionary(std::nullptr_t const & n) : System::Object(n) {
			}

			Dictionary(Dictionary* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			Dictionary(Dictionary const & other) : System::Object(other) { }

			Dictionary(Dictionary&& other) noexcept : System::Object(std::move(other)) { }

			Dictionary(Object::ObjectData* other) : System::Object(other) {
			}

			Dictionary& operator=(Dictionary const & other) {
				System::Object::operator=(other);
				return *this;
			}

			Dictionary& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			Dictionary& operator=(Dictionary&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			Dictionary& operator=(Dictionary* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			Dictionary* operator->() {
				return this;
			}



					Dictionary(int capacity) {
					ObjectData* dd = ObjectData(capacity);
					this->od = dd;
				}

				Dictionary(int capacity, IEqualityComparer<TKey> comparer) {
					ObjectData* dd = ObjectData(capacity, comparer);
					this->od = dd;
				}

				Dictionary(IEqualityComparer<TKey> comparer) {
					ObjectData* dd = ObjectData(comparer);
					this->od = dd;
				}

				//operator IEnumerable<T>() {
				//	return IEnumerable<T>(this->od, static_cast<ObjectData*>(this->od));
				//}

				void Add(const TKey& key, TValue& value) {
					ObjectData* dd = GOD();
					dd->Insert(key, value, true);
				}

				void Add(const TKey& key, TValue&& value) {
					ObjectData* dd = GOD();
					dd->Insert(key, (TValue&&)value, true);
				}

				bool TryGetValue(const TKey& key, TValue& value) {
					ObjectData* dd = GOD();
					TValue* ptr = dd->TryGetValue(key);
					if (!ptr)
						return false;
					value = *ptr;
					return true;
				}

				bool Contains(const TKey& key) {
					TValue value;
					return TryGetValue(key, value);
				}

				bool Remove(const TKey& key) {
					ObjectData* dd = GOD();
					return dd->Remove(key);
				}

				TValue& operator[](TKey const& key)
				{
					ObjectData* dd = GOD();
					TValue* val = dd->TryGetValue(key);
					if (val)
						return *val;
					int index = dd->Insert(key, TValue(), true);
					return dd->entries[index].value;
				}

				IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator() const { return GOD()->GetEnumerator(); }

				IEnumerator<KeyValuePair<TKey, TValue>> begin() const {
					IEnumerator<KeyValuePair<TKey, TValue>> ret = GetEnumerator();
					ret.GOD()->mngc = ret.GetFP_MoveNextGetCurrent();
					++ret;
					return ret;
				}
				IEnumerator<KeyValuePair<TKey, TValue>> end() const {
					return GOD()->GetEnumeratorEnd();
				}

			};

			template<class T> class System_API Stack : public Object {
			private:
				int Get_Count() const {
					return GOD()->Get_Count();
				}

			public:
				class System_API ObjectData : public Object::ObjectData {
				private:
					T* _array;
					int _size;
					int _capacity;
					static const int _defaultCapacity = 4;

				public:

					int Get_Count() const {
						return _size;
					}

					ObjectData(int capacity) {
						_array = (T*) ::operator new[](capacity);
						_size = 0;
						_capacity = capacity;
					}

					ObjectData() {
						_array = nullptr;
						_size = 0;
						_capacity = 0;
					}

					~ObjectData() override {
						Clear();
					}

					void Clear() {
						if (_capacity > 0) {
							for (int i = 0; i < _size; i++) {
								_array[i].~T();
							}
							::operator delete[](_array);
							_capacity = 0;
							_size = 0;
						}
					}

					void Push(T const& item) {
						if (_size == _capacity) {
							if (_capacity == 0) {
								_array = (T*) ::operator new[](_defaultCapacity);
								_capacity = _defaultCapacity;
							}
							else {
								T* newArray = (T*) ::operator new[](sizeof(T)* (_capacity << 1));
								memcpy(newArray, _array, sizeof(T) * _capacity);
								::operator delete[](_array);
								_array = newArray;
								_capacity <<= 1;
							}
						}
						new (_array + _size++) T(item);
					}

					T Pop() {
						if (_size == 0) {
							throw InvalidOperationException();
						}
						T ret = _array[--_size];
						//_array[_size] = default<T>();
						_array[_size].~T();

						return ret;
					}

				};

				PropGenGet<int, Stack<T>, &Stack<T>::Get_Count> Count{ this };

							ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			Stack(){}

			Stack(std::nullptr_t const & n) : System::Object(n) {
			}

			Stack(Stack* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			Stack(Stack const & other) : System::Object(other) { }

			Stack(Stack&& other) noexcept : System::Object(std::move(other)) { }

			Stack(Object::ObjectData* other) : System::Object(other) {
			}

			Stack& operator=(Stack const & other) {
				System::Object::operator=(other);
				return *this;
			}

			Stack& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			Stack& operator=(Stack&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			Stack& operator=(Stack* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			Stack* operator->() {
				return this;
			}



					Stack(int capacity) {
					this->od = new ObjectData(capacity);
				}

				void Push(T const& item) {
					GOD()->Push(item);
				}

				T Pop() {
					return GOD()->Pop();
				}
			};


			template<class T> class System_API Queue : public Object {
			private:
				int Get_Count() const {
					return GOD()->Count;
				}

			public:

				class System_API ObjectData : public Object::ObjectData {
				private:
					struct node {
						T value;
						node(T const & value) : value(value) {
						}

						node(T&& value) : value(std::move(value)) {
						}
					};

					static const int c = 7;

					node** wtbl = NULL;
					node** rtbl = NULL;

					size_t fnode;
				public:
					int Count = 0;

					ObjectData() {
						node** nptr = new node * [c + 1];
						memset(nptr, 0, c << ptrshifter);
						nptr[c] = (node*)1; // sentinel
						rtbl = wtbl = nptr;
						fnode = (size_t)(nptr);
					}

					~ObjectData() override {
						while ((((size_t)*rtbl) & 1) == 0)
						{
							if (*rtbl)
								delete (*rtbl);
							rtbl++;
						}
						while (true) {
							node** nexttbl = (node**)(((size_t)*rtbl) ^ 1);
							delete[](rtbl - c);
							if (!nexttbl)
								break;
							rtbl = nexttbl;
							while ((((size_t)*rtbl) & 1) == 0)
							{
								if (*rtbl)
									delete (*rtbl);
								rtbl++;
							}
						}
					}

					void Enqueue(T const & item) {

						node* newnode = new node(item);
						node* cur = *wtbl;
						if (cur == 0) {
							*wtbl = newnode;
							wtbl++;
						}
						else {
							node** nptr = new node * [c + 1];
							memset(nptr, 0, c << ptrshifter);
							nptr[0] = newnode;
							nptr[c] = (node*)1; // sentinel
							*wtbl = (node*)((size_t)nptr | 1);
							wtbl = nptr + 1;
						}
						++Count;
					}

					T Dequeue() {
						//Serial.println("DEQUEUE BEGIN");
						node* n = *rtbl;
						size_t nval = (size_t)n;
						if (nval <= 1)
							throw InvalidOperationException();
						if (nval & 1) {
							//Serial.print("DEQUEUE SENTINEL, DELETE BLOCK AT ");
							//Serial.print((size_t)(rtbl - c));
							//Serial.print(", FNODE = ");
							//Serial.println(fnode);

							delete[](rtbl - c);
							rtbl = (node**)(nval ^ 1);
						}
						//Serial.println("DEQUEUE BEFORE NODE DELETE");
						T ret = (*rtbl)->value;
						delete (*rtbl);
						rtbl++;
						//Serial.println("DEQUEUE END");
						--Count;
						return ret;
					}


				};

				PropGenGet<int, Queue<T>, &Queue<T>::Get_Count> Count{ this };

							ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			Queue(){}

			Queue(std::nullptr_t const & n) : System::Object(n) {
			}

			Queue(Queue* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			Queue(Queue const & other) : System::Object(other) { }

			Queue(Queue&& other) noexcept : System::Object(std::move(other)) { }

			Queue(Object::ObjectData* other) : System::Object(other) {
			}

			Queue& operator=(Queue const & other) {
				System::Object::operator=(other);
				return *this;
			}

			Queue& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			Queue& operator=(Queue&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			Queue& operator=(Queue* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			Queue* operator->() {
				return this;
			}



					void Enqueue(T const& item) {
					GOD()->Enqueue(item);
				}

				T Dequeue() {
					return GOD()->Dequeue();
				}
			};



		}
	}

	System::Collections::Generic::Array<string> String::Split(char const c) {
		System::Collections::Generic::List<string> lst = new System::Collections::Generic::List<string>();
		char16_t* rawBytes = ToCharArray();
		int startpos = 0;
		for (int i = 0; i < Length; i++) {
			if (rawBytes[i] == c) {
				string str(rawBytes + startpos, i - startpos);
				lst.Add(str);
				startpos = i + 1;
			}
		}
		string str(rawBytes + startpos, Length - startpos);
		lst.Add(str);

		return lst.ToArray();
	}


	namespace Dynamic {

		class System_API ExpandoObject : public Object
		{
		private:


		public:
			class System_API ObjectData : public System::Collections::Generic::Dictionary<string, Object>::ObjectData {
			public:

			};
						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			ExpandoObject(){}

			ExpandoObject(std::nullptr_t const & n) : System::Object(n) {
			}

			ExpandoObject(ExpandoObject* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			ExpandoObject(ExpandoObject const & other) : System::Object(other) { }

			ExpandoObject(ExpandoObject&& other) noexcept : System::Object(std::move(other)) { }

			ExpandoObject(Object::ObjectData* other) : System::Object(other) {
			}

			ExpandoObject& operator=(ExpandoObject const & other) {
				System::Object::operator=(other);
				return *this;
			}

			ExpandoObject& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			ExpandoObject& operator=(ExpandoObject&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			ExpandoObject& operator=(ExpandoObject* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			ExpandoObject* operator->() {
				return this;
			}



				System::Object& operator[](string const& key)
			{
				ObjectData* dd = GOD();
				System::Object* val = dd->TryGetValue(key);
				if (val)
					return *val;
				int index = dd->Insert(key, System::Object(), true);
				return dd->entries[index].value;
			}
		};
	}


	namespace Text {
		class UTF8Encoding;

		class System_API Encoding : public Object
		{
		private:

		public:
			//	int tst;

			Encoding() {
				//	tst = 1;
			}

			static UTF8Encoding UTF8;

			//virtual byte* GetBytes(String* s) {
			//	throw NotImplementedException();
			//}

			virtual System::Collections::Generic::Array<byte> GetBytes(String const& s) {
				/*std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
				auto bs = convert.to_bytes(s.ToCharArray());

				byte* bts = new byte[(int)bs.length()];
				const char* chr = bs.c_str();
				while (chr) *bts++ = *chr++;
				return bts;
				*/
				//typedef std::codecvt<char16_t, char, mbstate_t> Convert;
				////std::wstring w = L"\u20ac\u20ab\u20ac";
				//std::locale locale("en_GB.utf8");
				//const Convert& convert = std::use_facet<Convert>(locale);

				//std::mbstate_t state;
				//const char16_t* from_ptr;
				//char* to_ptr;
				//std::vector<char> result(((size_t) s.Length) * 3 + 1, 0);
				//Convert::result convert_result = convert.out(state,
				//	s.ToCharArray(), s.ToCharArray() + s.Length, from_ptr,
				//	result.data(), result.data() + result.size(), to_ptr);

				//if (convert_result == Convert::ok)
				//	std::cout << result.data() << std::endl;
				//else std::cout << "Failure: " << convert_result << std::endl;

				//byte* bts = new byte[strlen(to_ptr)];

				//while (to_ptr) *bts++ = *to_ptr++;
				//return bts;
				throw NotImplementedException();
			}

			virtual String GetString(byte const* bytes, int index, int count) {
				throw NotImplementedException();
			}

		};

		class System_API UTF8Encoding : public Encoding
		{
		private:

		public:
			UTF8Encoding() {
			}

			System::Collections::Generic::Array<byte> GetBytes(String const& s) override {
				//std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
				byte* dst = new byte[(s.Length << 2) + 1];
				char16_t* chrs = s.ToCharArray();
				int len = String::utf16_to_utf8(chrs, s.Length, (char*)dst);

				return System::Collections::Generic::Array<byte>(dst, len);
			}

			String GetString(byte const* bytes, int index, int count) override {
				return String((char const*)(bytes + index), count);
			}

		};

#ifndef SYSTEM_EXPORTS
		UTF8Encoding Encoding::UTF8;
#endif




		namespace RegularExpressions {

			enum class RegexOptions {
				None = 0x0000,
				IgnoreCase = 0x0001,
				Multiline = 0x0002,
				ExplicitCapture = 0x0004,
				Compiled = 0x0008,
				SingleLine = 0x0010,
				IgnorePatternWhitespace = 0x0020,
				RightToLeft = 0x0040,
				ECMAScript = 0x0100,
				CultureInvariant = 0x0200,
			};


			class System_API Regex : public Object {
			private:
			public:






			};



		}
	}

	class System_API Console
	{
	private:
	public:

		static void Write(String const& value) {
#if _MSC_VER
			wchar_t* wchrs = (wchar_t*)value.ToCharArray();
			WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), wchrs, (DWORD)std::wcslen(wchrs), NULL, NULL);
#elif defined ESP32
			Serial.print((char const*)(uint8_t const*)System::Text::Encoding::UTF8.GetBytes(value));
#else	


			//std::cout << (int)value.Length << std::endl;
			System::Collections::Generic::Array<byte> arr = System::Text::Encoding::UTF8.GetBytes(value);
			byte const* chrs = arr;
			//for (int i = 0; i < value.Length; i++)
			//	std::cout << (int)chrs[i] << std::endl;
			std::cout.write((char const*)chrs, arr.Length);
			std::cout.flush();
#endif
		}

		static void WriteLine(String const& value)
		{
			Write(value + u"\r\n");
			//std::cout << std::endl;
		}

	};

	namespace Threading {
		//typedef void(*cfnoparam)(void);

		enum class ApartmentState {
			STA = 0,
			MTA = 1,
			Unknown = 2
		};





		class System_API Thread : public Object
		{
		private:
			bool& Get_IsBackground() const {
				return GOD()->IsBackground;
			}

			void Set_IsBackground(const bool& value) {
				GOD()->IsBackground = value;
			}

			void Set_IsBackground(bool&& value) {
				GOD()->IsBackground = value;
			}

		public:
			class System_API ObjectData : public Object::ObjectData {
			public:
				std::thread* t = nullptr;
				Action<> act;
				ApartmentState apstate;
				bool IsBackground = false;

				ObjectData() : apstate(ApartmentState::MTA) {
				}

				~ObjectData() override {
					if (t) {
						if (IsBackground) {
							t->detach();
							delete t; // Yes, you have to delete it by yourself.Once you called std::thread::detach, the thread will be separated from the thread object and allowed execution to continue independently, and then the thread object will no longer own any thread.
							t = nullptr;
						}
						else {
							if (t->get_id() != std::this_thread::get_id()) {
								t->join();
								delete t;
								t = nullptr;
							}
							else {
								// we're on the t thread trying to delete ourselves, this happens for threads that aren't referenced anymore by anything but itself
								// not sure if this is the correct/best way to handle this but it seems to work at least on MVC
								// we skip calling the std::thread destructor and just release the memory:
								::operator delete(t);
							}

						}
					}
				}
			};


						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			Thread(){}

			Thread(std::nullptr_t const & n) : System::Object(n) {
			}

			Thread(Thread* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			Thread(Thread const & other) : System::Object(other) { }

			Thread(Thread&& other) noexcept : System::Object(std::move(other)) { }

			Thread(Object::ObjectData* other) : System::Object(other) {
			}

			Thread& operator=(Thread const & other) {
				System::Object::operator=(other);
				return *this;
			}

			Thread& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			Thread& operator=(Thread&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			Thread& operator=(Thread* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			Thread* operator->() {
				return this;
			}



			Thread(Action<> const & act)
			{
				ObjectData* dd = new ObjectData();
				dd->act = act;
				od = dd;
			}

			Thread(Action<>&& act)
			{
				ObjectData* dd = new ObjectData();
				dd->act = std::move(act);
				od = dd;
			}


			static void Sleep(int millisecondsTimeout)
			{
				std::chrono::milliseconds dura(millisecondsTimeout);
				std::this_thread::sleep_for(dura);
			}

			static void Yield() {
				std::this_thread::yield();
			}

			static void SpinWait(int iterations) {
				for (int i = 0; i < iterations; i++) {
					PAUSE();
				}
			}

			//Thread(int id)
			//{
			//	ManagedThreadId = id;
			//}

			static Thread _CurrentThread;

			void Start() {
				ObjectData* td = GOD();
				Thread& t = *this;
				//ApartmentState apstate = td->apstate;
				td->t = new std::thread([t]() {

					ObjectData* td = t.GOD();
					//#if _MSC_VER
#ifdef _MSC_VER
					HRESULT comhandle = CoInitializeEx(0, td->apstate == ApartmentState::STA ? COINIT_APARTMENTTHREADED : COINIT_MULTITHREADED);
#endif
					try {
						td->act();
					}
					catch (...) {
						//#if _MSC_VER
#ifdef _MSC_VER
						CoUninitialize();
#endif
						throw;
					}
					//#if _MSC_VER
#ifdef _MSC_VER
					CoUninitialize();
#endif
					});
			}

			//void Start(Object parameter)
			//{
			//	t = std::thread(func, parameter);
			//}

			void Join() {
				ObjectData* td = GOD();
				td->t->join();
				delete td->t;
				td->t = nullptr;
			}

			void SetApartmentState(ApartmentState state) {
				GOD()->apstate = state;
			}



			PropGen<bool, bool, System::Threading::Thread, &System::Threading::Thread::Get_IsBackground, &System::Threading::Thread::Set_IsBackground, &System::Threading::Thread::Set_IsBackground> IsBackground{ this };// = PropGen<bool&, bool, System::Threading::Thread, &System::Threading::Thread::Get_IsBackground, &System::Threading::Thread::Set_IsBackground>(this);
		};

		class System_API Timeout {
		public:
			static const int Infinite = -1;
			static const uint UnsignedInfinite = (uint)-1;

		};


		class System_API SpinLock : public System::Object {
		private:
		public:
			class ObjectData : public System::Object::ObjectData {
			private:
				std::atomic_flag flag;

			public:

				ObjectData() {
					// initing the flag can't be done through the constructor in MSC so we explicitly call clear, which should be portable
					flag.clear();
				}

				void Enter(Ref<bool> lockTaken) {
					while (flag.test_and_set(std::memory_order_acquire));

					lockTaken = true;
				}

				void Exit() {
					flag.clear(std::memory_order_release);
				}

				void TryEnter(Ref<bool> lockTaken) {
					lockTaken = !flag.test_and_set(std::memory_order_acquire);
				}
			};

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			SpinLock(){}

			SpinLock(std::nullptr_t const & n) : System::Object(n) {
			}

			SpinLock(SpinLock* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			SpinLock(SpinLock const & other) : System::Object(other) { }

			SpinLock(SpinLock&& other) noexcept : System::Object(std::move(other)) { }

			SpinLock(Object::ObjectData* other) : System::Object(other) {
			}

			SpinLock& operator=(SpinLock const & other) {
				System::Object::operator=(other);
				return *this;
			}

			SpinLock& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			SpinLock& operator=(SpinLock&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			SpinLock& operator=(SpinLock* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			SpinLock* operator->() {
				return this;
			}



				void Enter(Ref<bool> lockTaken) {
				GOD()->Enter(lockTaken);
			}

			void Exit() {
				GOD()->Exit();
			}

			void TryEnter(Ref<bool> lockTaken) {
				GOD()->TryEnter(lockTaken);
			}
		};


		class System_API SpinWait : System::Object {
		private:
			static const int YIELD_THRESHOLD = 10;
			static const int SLEEP_0_EVERY_HOW_MANY_TIMES = 5;
			static const int SLEEP_1_EVERY_HOW_MANY_TIMES = 20;

			int Get_Count() const {
				return GOD()->m_count;
			}

			bool Get_NextSpinWillYield() const {
				return GOD()->NextSpinWillYield();
			}

		public:
			class System_API ObjectData : System::Object::ObjectData {
			private:
				int m_count = 0;

				friend class SpinWait;
			public:
				ObjectData() : System::Object::ObjectData(0) {}

				Object::ObjectData* DeepCopy() override {
					ObjectData* ret = new ObjectData();
					ret->m_count = m_count;

					return ret;
				}

				bool NextSpinWillYield() {
					return m_count > YIELD_THRESHOLD || Environment::ProcessorCount == 1;
				}

				void SpinOnce() {
					if (NextSpinWillYield()) {
						int yieldsSoFar = m_count >= YIELD_THRESHOLD ? m_count - YIELD_THRESHOLD : m_count;
						if ((yieldsSoFar % SLEEP_1_EVERY_HOW_MANY_TIMES) == SLEEP_1_EVERY_HOW_MANY_TIMES - 1) {
							Thread::Sleep(1);
						}
						else if ((yieldsSoFar % SLEEP_0_EVERY_HOW_MANY_TIMES) == SLEEP_0_EVERY_HOW_MANY_TIMES - 1) {
							Thread::Sleep(0);
						}
						else {
							Thread::Yield();
						}
					}
					else {
						Thread::SpinWait(4 << m_count);
					}

					m_count = m_count == Int32::MaxValue ? YIELD_THRESHOLD : m_count + 1;
				}

				void Reset() {
					m_count = 0;
				}



			};
			PropGenGet<int, SpinWait, &SpinWait::Get_Count> Count{ this };
			PropGenGet<bool, SpinWait, &SpinWait::Get_NextSpinWillYield> NextSpinWillYield{ this };

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			SpinWait(){}

			SpinWait(std::nullptr_t const & n) : System::Object(n) {
			}

			SpinWait(SpinWait* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			SpinWait(SpinWait const & other) : System::Object(other) { }

			SpinWait(SpinWait&& other) noexcept : System::Object(std::move(other)) { }

			SpinWait(Object::ObjectData* other) : System::Object(other) {
			}

			SpinWait& operator=(SpinWait const & other) {
				System::Object::operator=(other);
				return *this;
			}

			SpinWait& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			SpinWait& operator=(SpinWait&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			SpinWait& operator=(SpinWait* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			SpinWait* operator->() {
				return this;
			}



				void SpinOnce() {
				GOD()->SpinOnce();
			}

			void Reset() {
				GOD()->Reset();
			}

			static bool SpinUntil(const Func<bool>& condition, int millisecondsTimeout) {
				if (millisecondsTimeout < Timeout::Infinite)
					throw ArgumentOutOfRangeException();
				if (condition == null)
					throw ArgumentNullException();
				uint startTime = 0;
				if (millisecondsTimeout != 0 && millisecondsTimeout != Timeout::Infinite) {
					startTime = (uint)Environment::TickCount;
				}
				SpinWait spinner{};
				while (!condition()) {
					if (millisecondsTimeout == 0)
						return false;
					spinner.SpinOnce();
					if (millisecondsTimeout != Timeout::Infinite && spinner.NextSpinWillYield) {
						if (millisecondsTimeout <= (Environment::TickCount - (int)startTime))
							return false;
					}
				}

				return true;
			}




		};


		class System_API WaitHandle : public Object
		{
		private:
			//bool Get_IsCompleted() const { return GOD()->Get_IsCompleted(); }
			//bool Get_CompletedSynchronously() const { return GOD()->Get_CompletedSynchronously(); };
			//Object Get_AsyncState() const { return GOD()->Get_AsyncState(); };
			//System::Threading::WaitHandle Get_AsyncWaitHandle() const { return GOD()->Get_AsyncWaitHandle(); };

		public:
			class System_API ObjectData {
			public:
				ObjectData() {
				}

				~ObjectData() {
				}

				//virtual bool Get_IsCompleted() const = 0;
				//virtual bool Get_CompletedSynchronously() const = 0;
				//virtual Object Get_AsyncState() const = 0;
				//virtual System::Threading::WaitHandle Get_AsyncWaitHandle() const = 0;
			};

					ObjectData* id = nullptr;

		ObjectData* GOD() const { return this->id; };

		WaitHandle(){}

		WaitHandle(std::nullptr_t const & n) : System::Object (nullptr), id(nullptr) {
		}

		WaitHandle(WaitHandle const & other) : System::Object(other) { this->id = other.id; }

		WaitHandle(WaitHandle&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

		WaitHandle(System::Object::ObjectData* baseod, ObjectData* other) : System::Object(baseod), id(other) {
		}

		WaitHandle& operator=(WaitHandle const & other) {
			System::Object::operator=(other);
			id = other.id;
			return *this;
		}

		WaitHandle& operator=(WaitHandle&& other) noexcept {
			System::Object::operator=(std::move(other));
			id = other.id;
			return *this;
		}

		WaitHandle* operator->() {
			return this;
		}





		};

	}

	namespace Collections {
		namespace Concurrent {

			template<class T> class System_API LockingStack : public Object {
			private:
			public:

				class System_API ObjectData : public Object::ObjectData {
				private:
					System::Collections::Generic::Stack<T> s = new System::Collections::Generic::Stack<T>{};
					std::mutex mut;


				public:

					ObjectData() {
					}

					~ObjectData() override {

					}

					void Push(T const& item) {

						//mut.lock();
						std::lock_guard<std::mutex> mlock(mut);
						s.Push(item);
						//mut.unlock();
					}

					void Push(T&& item) {
						std::lock_guard<std::mutex> mlock(mut);
						//mut.lock();
						s.Push(std::forward<T>(item));
						//mut.unlock();
					}

					bool TryPop(Out<T> result) {
						std::lock_guard<std::mutex> mlock(mut);
						//mut.lock();
						if (s.Count == 0) {
							//mut.unlock();
							return false;
						}
						result = s.Pop();

						//mut.unlock();
						return true;
					}

					T Pop() {
						std::lock_guard<std::mutex> mlock(mut);
						return s.Pop();
					}

				};

								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				LockingStack(){}

				LockingStack(std::nullptr_t const & n) : System::Object(n) {
				}

				LockingStack(LockingStack* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				LockingStack(LockingStack const & other) : System::Object(other) { }

				LockingStack(LockingStack&& other) noexcept : System::Object(std::move(other)) { }

				LockingStack(Object::ObjectData* other) : System::Object(other) {
				}

				LockingStack& operator=(LockingStack const & other) {
					System::Object::operator=(other);
					return *this;
				}

				LockingStack& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				LockingStack& operator=(LockingStack&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				LockingStack& operator=(LockingStack* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				LockingStack* operator->() {
					return this;
				}



					void Push(const T& item) {
					GOD()->Push(item);
				}

				void Push(T&& item) {
					GOD()->Push(std::forward<T>(item));
				}

				bool TryPop(Out<T> result) {
					return GOD()->TryPop(result);
				}

				T Pop() {
					return GOD()->Pop();
				}

			};


//			class System_API Counter1ms : public Object {
//			private:
//			public:
//
//				static Counter1ms Instance;
//
//				volatile ulong cnt{};
//				System::Threading::Thread t = null;
//
//
//				Counter1ms() {
//					t = new System::Threading::Thread([this]() {
//						while (true) {
//							cnt++;
//							System::Threading::Thread::Sleep(1);
//						}
//						});
//					t.Start();
//				}
//
//				static ulong GetCount() {
//					return Instance.cnt;
//				}
//
//			};
//
//#ifndef SYSTEM_EXPORTS
//			Counter1ms Counter1ms::Instance{};
//#endif
//




			/*
			template<class T> class System_API KConcurrentStack : public Object {
			private:
				int Get_Count() const { return (int) GOD()->Get_Count(); }
			public:
				class System_API ObjectData : public Object::ObjectData {
				private:


					//void TryShiftDelStacks() {
					//	ulong ms = Counter1ms::GetCount();
					//	ulong lastms = at_lastms.load(std::memory_order_relaxed);
					//	if (ms - lastms >= 2) {
					//		if (at_lastms.compare_exchange_strong(lastms, ms, std::memory_order_relaxed)) {
					//			Node* purgatory_tail = lst_at_deltail[1].load(std::memory_order_relaxed);
					//			if (purgatory_tail != nullptr) {
					//				Node* hell_head = lst_at_deltail[2].load(std::memory_order_relaxed);
					//				purgatory_tail->next = hell_head;
					//				Node* purgatory_head = lst_at_delhead[1].load(std::memory_order_relaxed);
					//				while (!lst_at_delhead[2].compare_exchange_weak(purgatory_tail->next, purgatory_head, std::memory_order_relaxed)) {}
					//			}


					//			Node* del_tail = lst_at_deltail[0].load(std::memory_order_relaxed);
					//			if (del_tail != nullptr) {
					//				while (!lst_at_deltail[0].compare_exchange_weak(del_tail, nullptr, std::memory_order_relaxed)) {}
					//				Node* del_head = lst_at_delhead[0].load(std::memory_order_relaxed);
					//				while (!lst_at_delhead[0].compare_exchange_weak(del_head, nullptr, std::memory_order_relaxed)) {}

					//				lst_at_delhead[1].store(del_head, std::memory_order_relaxed);
					//				lst_at_deltail[1].store(del_tail);
					//			}
					//			else {
					//				lst_at_delhead[1].store(nullptr, std::memory_order_relaxed);
					//				lst_at_deltail[1].store(nullptr);
					//			}
					//		}
					//	}
					//}


					//void PushDel(Node* item) {
					//	std::atomic<Node*>* at_delhead = lst_at_delhead + 0;

					//	Node* delhead = at_delhead->load(std::memory_order_relaxed);
					//	item->next = delhead;
					//	while (!at_delhead->compare_exchange_weak(item->next, item, std::memory_order_release)) {
					//	}
					//	if (item->next == nullptr) {
					//		lst_at_deltail[0].store(item, std::memory_order_relaxed);
					//	}
					//}
				public:
					class Node {
					private:
					public:
						T value;
						Node* next;

						Node() {}

						Node(const T& value) : value(value) {}
					};

					class TLDelRow {
					private:
					public:
						Node* first = nullptr;
						Node* last = nullptr;
						//Node* nextlast = nullptr;

						int count = 0;

						TLDelRow() {

						}

						TLDelRow(const TLDelRow& other) : first(other.first), last(other.last), count(other.count) {

						}

						~TLDelRow() {

						}

						int Add(Node* ptr) {
							ptr->next = last;
							//nextlast = last;
							last = ptr;
							if (first == nullptr) {
								first = ptr;
							}

							return ++count;
						}

						Node* TryGet() {
							if (last != nullptr) {
								Node* ret = last;
								last = last->next;
								if (last == nullptr)
									first = nullptr;
								--count;

								return ret;
							}

							return nullptr;
						}

						void Append(TLDelRow& other) {
							if (other.count > 0) {
								other.first->next = last;
								last = other.last;
								count += other.count;
							}
						}

						void Clear() {
							first = nullptr;
							last = nullptr;
							count = 0;
						}

						void DeleteAll() {
							Node* ptr = last;
							while (ptr != null) {
								Node* tmp = ptr;
								ptr = ptr->next;
								delete tmp;
							}
							Clear();
						}

						void DeleteUntil(int cnt) {
							Node* ptr = last;
							while (ptr != null && count > cnt) {
								Node* tmp = ptr;
								ptr = ptr->next;
								delete tmp;
								--count;
							}
							if (!ptr) {
								last = first = null;
							}
							else
								last = ptr;
						}

						TLDelRow& operator=(const TLDelRow& other) {
							first = other.first;
							last = other.last;
							count = other.count;

							return *this;
						}

					};

					Node* at_head{};
					std::atomic<ulong> at_NumPush{};
					std::atomic<ulong> at_NumPop{};
					std::atomic<Node*> at_delhead{}; // [3] = { }; // if there are fewer elements of the initializer list than there are members in the aggregate, then the remaining members shall be initialized from an empty initializer list. In this case, that's all members.
					//std::atomic<Node*> lst_at_deltail[2] = { };

					static tlocal ulong lastms;
					static tlocal TLDelRow hospital;
					static tlocal TLDelRow purgatory;
					static tlocal TLDelRow hell;

					void TryClearHospital() {
						ulong ms = Counter1ms::GetCount();
						if (ms - lastms >= 1) {
							lastms = ms;
							//if (hell.count > 0) {
							//	Node* delhead = at_delhead.load(std::memory_order_relaxed);
							//	hell.first->next = delhead;
							//	while (!at_delhead.compare_exchange_weak(hell.first->next, hell.last, std::memory_order_release)) {}
							//}
							hell.Append(purgatory);
							//hell.DeleteUntil(10000);
							//hell.DeleteAll();
							//hell = purgatory;
							purgatory = hospital;
							hospital.Clear();
						}
					}

					void DeleteStack(Node* ptr) {
						while (ptr != nullptr) {
							Node* tmp = ptr;
							ptr = ptr->next;
							delete tmp;
						}
					}


					~ObjectData() override {
						//for (int i = 0; i < 3; i++) {
						//	std::atomic<Node*>* at_delhead = lst_at_delhead + i;
							//Node* ptr = at_delhead.load(std::memory_order_acquire);
							//while (ptr != nullptr) {
							//	Node* tmp = ptr;
							//	ptr = ptr->next;
							//	delete tmp;
							//}
						//}
						//ptr = at_head; // .load(std::memory_order_acquire);
						DeleteStack(at_head);
						hospital.DeleteAll();
						purgatory.DeleteAll();
						hell.DeleteAll();
					}

					void Push(const T& item) {
						//std::atomic<Node*>* at_delhead = lst_at_delhead + 2;
						//Node* newhead = at_delhead->load(std::memory_order_acquire);
						//while (newhead != null) {
						//	if (at_delhead->compare_exchange_weak(newhead, newhead->next, std::memory_order_acquire))
						//		break;
						//}

						Node* newhead = hell.TryGet();
						//if (newhead == nullptr) {
						//	newhead = at_delhead.load(std::memory_order_acquire);
						//	while (newhead != null) {
						//		if (at_delhead.compare_exchange_weak(newhead, newhead->next, std::memory_order_acquire))
						//			break;
						//	}
						//}
						if (newhead == nullptr)
							newhead = new Node(item);

						//newhead->value = item;
						//newhead->next = at_head; // .load(std::memory_order_relaxed);
						//if (InterlockedCompareExchangePointerRelease((void**)&at_head, (void*)newhead, (void*)newhead->next) == newhead->next) {
						//	return;
						//}


					//	System::Threading::SpinWait spin = new System::Threading::SpinWait();
						do {
						//	spin.SpinOnce();
							newhead->next = at_head;
						} while (InterlockedCompareExchangePointerRelease((void**)&at_head, (void*)newhead, (void*)newhead->next) != newhead->next);


						//void* cur;
						//while ((cur = InterlockedCompareExchangePointerRelease((void**)&at_head, (void*)newhead, (void*)newhead->next)) != newhead->next) {
						//	newhead->next = (Node*)cur;
						//	//while (!at_head.compare_exchange_weak(newhead->next, newhead, std::memory_order_release)) {
						//}
						//if (newhead->next == nullptr) {
						//	lst_at_deltail[0].store(newhead, std::memory_order_release);
						//}
					//	at_NumPush.fetch_add(1, std::memory_order_relaxed);

					}

					ulong Get_Count() {
						return at_NumPush.load(std::memory_order_relaxed) - at_NumPop.load(std::memory_order_relaxed);
					}

					bool TryPop(Out<T> result) {
						if (at_head == nullptr)
							return false;
						Node* head = at_head;

					//	void* cur;
						while (InterlockedCompareExchangePointerAcquire((void**)&at_head, (void*)head->next, (void*)head) != head) {
							//while (!at_head.compare_exchange_weak(head, head->next, std::memory_order_acquire)) {
							if (at_head == nullptr)
								return false;
							head = at_head;
						}

						result = head->value;

						//PushDel(head);
						//ulong numpops = at_NumPop.fetch_add(1, std::memory_order_relaxed);
						//if ((numpops & 4095) == 0) {
						//	TryShiftDelStacks();
						//}

						int cnth = hospital.Add(head);
						if (cnth > 2000) {
							TryClearHospital();
						}


						return true;
					}
				};

				PropGenGet<int, KConcurrentStack, &KConcurrentStack::Get_Count> Count{ this };

					ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

					KConcurrentStack(){}

					KConcurrentStack(std::nullptr_t const & n) : System::Object(n) {
					}

					KConcurrentStack(KConcurrentStack* pValue) {
						if (!pValue->od) {
							ObjectData* dd = new ObjectData();
							od = dd;
						}
						else {
							od = pValue->od;
							pValue->od = nullptr;
						}
						delete pValue;
					}

					KConcurrentStack(KConcurrentStack const & other) : System::Object(other) { }

					KConcurrentStack(KConcurrentStack&& other) noexcept : System::Object(std::move(other)) { }

					KConcurrentStack(Object::ObjectData* other) : System::Object(other) {
					}

					KConcurrentStack& operator=(KConcurrentStack const & other) {
						System::Object::operator=(other);
						return *this;
					}

					KConcurrentStack& operator=(std::nullptr_t const & n) {
						System::Object::operator=(n);
						return *this;
					}

					KConcurrentStack& operator=(KConcurrentStack&& other) noexcept {
						System::Object::operator=(std::move(other));
						return *this;
					}

					KConcurrentStack& operator=(KConcurrentStack* other) {
						if (od == other->od)
							return *this;
						Release();
						od = other->od;
						::operator delete((void*)other);
						return *this;
					}

					KConcurrentStack* operator->() {
						return this;
					}



				void Push(const T& item) {
					return GOD()->Push(item);
				}

				//void Push(T&& item) {
				//	return GOD()->Push(item);
				//}

				bool TryPop(Out<T> result) {
					return GOD()->TryPop(result);
				}

			};

#ifndef SYSTEM_EXPORTS
			template<class T> tlocal ulong KConcurrentStack<T>::ObjectData::lastms{};
			template<class T> tlocal typename KConcurrentStack<T>::ObjectData::TLDelRow KConcurrentStack<T>::ObjectData::hospital{};
			template<class T> tlocal typename KConcurrentStack<T>::ObjectData::TLDelRow KConcurrentStack<T>::ObjectData::purgatory{};
			template<class T> tlocal typename KConcurrentStack<T>::ObjectData::TLDelRow KConcurrentStack<T>::ObjectData::hell{};
#endif
			*/

#if _MSC_VER

			template<class T> class System_API KConcurrentStack2 : public Object {
			private:
				int Get_Count() const { return (int)GOD()->Get_Count(); }
			public:
				class System_API ObjectData : public Object::ObjectData {
				private:
					class Node;

					class alignas(16) CountedNode {
					private:
					public:
						Node* ptr = nullptr;
						ulong external_count = 0;

						CountedNode() {
						}

						~CountedNode() {
						}
					};

					class Node {
					private:
					public:
						T value;
						CountedNode next;
						std::atomic<int> internal_count;

						Node() {}

						Node(const T& value) : value(value), internal_count(0) {}
					};

					CountedNode head{};


					void increase_head_count(CountedNode& old_counter) {
						CountedNode nn;
						do {
							nn = old_counter;
							++nn.external_count;
						} while (!InterlockedCompareExchange128((long*)&head, (long)nn.external_count, (long)nn.ptr, (long*)&old_counter));
						old_counter.external_count = nn.external_count;
					}

					//void fetch_add_128(long* dst, int cnt)
					//{
					//	
					//}

				public:



					~ObjectData() override {

					}

					void Push(const T& item) {
						CountedNode nn;
						nn.ptr = new Node(item);
						nn.external_count = 1;
						nn.ptr->next = head;
						while (!InterlockedCompareExchange128((long*)&head, (long)nn.external_count, (long)nn.ptr, (long*)(&nn.ptr->next))) {}
					}

					ulong Get_Count() {
						return 0;
					}

					bool TryPop(Out<T> result) {
						CountedNode old_head = head;
						for (;;) {
							increase_head_count(old_head);
							Node* const ptr = old_head.ptr;
							if (!ptr)
								return false;
							if (InterlockedCompareExchange128((long*)&head, (long)ptr->next.external_count, (long)ptr->next.ptr, (long*)(&old_head))) {
								result = ptr->value;
								int const count_increase = (int)old_head.external_count - 2;
								if (ptr->internal_count.fetch_add(count_increase) == -count_increase) {
									delete ptr;
								}
								return true;
							}
							else if (ptr->internal_count.fetch_sub(1) == 1) {
								delete ptr;
							}
						}
					}
				};

				PropGenGet<int, KConcurrentStack2, &KConcurrentStack2::Get_Count> Count{ this };

								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				KConcurrentStack2(){}

				KConcurrentStack2(std::nullptr_t const & n) : System::Object(n) {
				}

				KConcurrentStack2(KConcurrentStack2* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				KConcurrentStack2(KConcurrentStack2 const & other) : System::Object(other) { }

				KConcurrentStack2(KConcurrentStack2&& other) noexcept : System::Object(std::move(other)) { }

				KConcurrentStack2(Object::ObjectData* other) : System::Object(other) {
				}

				KConcurrentStack2& operator=(KConcurrentStack2 const & other) {
					System::Object::operator=(other);
					return *this;
				}

				KConcurrentStack2& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				KConcurrentStack2& operator=(KConcurrentStack2&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				KConcurrentStack2& operator=(KConcurrentStack2* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				KConcurrentStack2* operator->() {
					return this;
				}



				void Push(T const& item) {
					return GOD()->Push(item);
				}

				//void Push(T&& item) {
				//	return GOD()->Push(item);
				//}

				bool TryPop(Out<T> result) {
					return GOD()->TryPop(result);
				}
			};
#endif

#if _MSC_VER
			template<class T>
			using ConcurrentStack = KConcurrentStack2<T>; //LockingStack<T>;
#else
			template<class T>
			using ConcurrentStack = LockingStack<T>;
#endif


			template<class T> class System_API KConcurrentQueue : public Object {
			private:
			public:

				class System_API ObjectData : public Object::ObjectData {
				private:
					static const int bitsperdim = 4;
					static const int elsperdim = 1 << bitsperdim;
					static const int dimmask = (1 << (bitsperdim + 1)) - 1;

					static const int maxnumdim = 2;
					static const int bitsdimcounter = 4;

					struct node {
						T* value;
						ulong nextindex;
					};

					node** root;
					std::atomic<ulong> at_index;
					std::atomic<ulong> at_inject;
					//std::atomic<ulong> generationcounter = 0;
				public:

					ObjectData() {
						at_index.store(0);
						at_inject.store(elsperdim);
						node** arr = new node * [elsperdim];
						for (int i = 0; i < elsperdim; i++) {
							arr[i] = new node();
							arr[i]->value = nullptr;
							arr[i]->nextindex = i == elsperdim - 1 ? 0 : i + 1;
						}
						for (int i = 0; i < maxnumdim - 1; i++) {
							node** tmp = new node * [elsperdim];
							memset(tmp, 0, elsperdim * sizeof(node*));
							tmp[0] = (node*)arr;
							arr = tmp;
						}
						root = arr;
					}

					~ObjectData() override {

					}

					ulong ComposeIndex(ulong dimcounter, ulong wi, ulong ri, ulong generation) {
						ulong ret = dimcounter << (64 - bitsdimcounter);
						ulong shifter = 64 - bitsdimcounter - ((bitsperdim + bitsperdim) * (dimcounter + 1));
						ulong generationmask = (1 << shifter) - 1;
						for (int i = 0; i <= dimcounter; i++) {
							ret |= (wi & dimmask) << (shifter + bitsperdim);
							wi >>= bitsperdim;
							ret |= (ri & dimmask) << (shifter);
							ri >>= bitsperdim;
							shifter += (bitsperdim + bitsperdim);
						}
						ret |= (generation & generationmask);

						return ret;
					}


					int ParseDim(ulong indori, ulong dims, ulong& ind, ulong& wi, ulong& ri, node** arr, node** parr, const T& item) {
						ulong wipart = (ind >> (64 - bitsperdim));
						ulong ripart = (ind >> (64 - bitsperdim - bitsperdim)) & dimmask;
						wi = (wi << bitsperdim) | wipart;
						ri = (ri << bitsperdim) | ripart;

						if (dims > 0) {
							parr = arr;
							arr = (node**)arr[wipart];
							ind <<= (bitsdimcounter + bitsdimcounter);
							return ParseDim(indori, dims - 1, ind, wi, ri, arr, parr, item);
						}
						else {
							node* rarr = (node*)arr;
							node* nod = rarr + (wi & dimmask);

							ulong nextwi = nod->nextindex;
							if (nextwi != ri) {

								ulong nextind = ComposeIndex(indori >> 64 - bitsdimcounter, nextwi, ri, indori);
								ulong expected = indori;
								if (at_index.compare_exchange_weak(expected, nextind)) {
									ind = nextind;
									std::atomic<T*>* ato = reinterpret_cast<std::atomic<T*>*>(nod);
									T* newt = new T(item);
									ato->store(newt);

									return 0;
								}
								else {
									// report failure, try again
									return -1;
								}
							}
							else {
								// uh-oh, we caught up with the read index
								Console::WriteLine("Should increase it");

							}
						}

					}


					void Enqueue(T const & item) {
						ulong indori = at_index.load();

						ulong dimcounter = indori >> 64 - bitsdimcounter;
						//T** tmp = root;

						ulong wi = 0;
						ulong ri = 0;
						node** parr = nullptr;
						node** arr = root;
						for (int i = 0; i < maxnumdim - dimcounter - 1; i++) {
							parr = arr;
							arr = (node**)arr[0];
						}
						ulong ind = indori << bitsdimcounter;
						ParseDim(dimcounter, dimcounter, ind, wi, ri, arr, parr, item);
						//ulong ind = indori;
						//ulong shifter = 64;
						//ulong wi = 0;
						//ulong ri = 0;
						//for (int i = 0; i < numdim - 1; i++) {
						//	shifter -= (bitsperdim + bitsperdim);
						//	int wipart = (indori >> (shifter + bitsperdim)) & dimmask;
						//	int ripart = (indori >> shifter) & dimmask;
						//	wi = (wi << bitsperdim) | wipart;
						//	ri = (wi << bitsperdim) | ripart;
						//	tmp = ((T**)tmp)[wipart];
						//}



						//tmp[wi & dimmask] = item;




					}
				};

								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				KConcurrentQueue(){}

				KConcurrentQueue(std::nullptr_t const & n) : System::Object(n) {
				}

				KConcurrentQueue(KConcurrentQueue* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				KConcurrentQueue(KConcurrentQueue const & other) : System::Object(other) { }

				KConcurrentQueue(KConcurrentQueue&& other) noexcept : System::Object(std::move(other)) { }

				KConcurrentQueue(Object::ObjectData* other) : System::Object(other) {
				}

				KConcurrentQueue& operator=(KConcurrentQueue const & other) {
					System::Object::operator=(other);
					return *this;
				}

				KConcurrentQueue& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				KConcurrentQueue& operator=(KConcurrentQueue&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				KConcurrentQueue& operator=(KConcurrentQueue* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				KConcurrentQueue* operator->() {
					return this;
				}




					void Enqueue(const T& item) {
					GOD()->Enqueue(item);
				}
			};



			template<class T> class System_API LockingQueue : public Object {
			private:
			public:

				class System_API ObjectData : public Object::ObjectData {
				private:
					System::Collections::Generic::Queue<T> q = new System::Collections::Generic::Queue<T>{};
					std::mutex mut;


				public:

					ObjectData() {
					}

					~ObjectData() override {

					}

					void Enqueue(T const & item) {
						std::lock_guard<std::mutex> mlock(mut);
						q.Enqueue(item);
					}

					void Enqueue(T&& item) {
						std::lock_guard<std::mutex> mlock(mut);
						q.Enqueue(std::forward<T>(item));
					}

					bool TryDequeue(Out<T> result) {
						std::lock_guard<std::mutex> mlock(mut);

						if (q.Count == 0)
							return false;

						result = q.Dequeue();


						return true;
					}

					int Get_Count() {
						std::lock_guard<std::mutex> mlock(mut);
						return q.Count;
					}
				};

								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				LockingQueue(){}

				LockingQueue(std::nullptr_t const & n) : System::Object(n) {
				}

				LockingQueue(LockingQueue* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				LockingQueue(LockingQueue const & other) : System::Object(other) { }

				LockingQueue(LockingQueue&& other) noexcept : System::Object(std::move(other)) { }

				LockingQueue(Object::ObjectData* other) : System::Object(other) {
				}

				LockingQueue& operator=(LockingQueue const & other) {
					System::Object::operator=(other);
					return *this;
				}

				LockingQueue& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				LockingQueue& operator=(LockingQueue&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				LockingQueue& operator=(LockingQueue* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				LockingQueue* operator->() {
					return this;
				}



				int Get_Count() const {
					return GOD()->Get_Count();
				}

				bool Get_IsEmpty() const {
					return Get_Count() == 0;
				}

				PropGenGet<int, LockingQueue, &LockingQueue::Get_Count> Count{ this };
				PropGenGet<bool, LockingQueue, &LockingQueue::Get_IsEmpty> IsEmpty{ this };

				void Enqueue(T const& item) {
					GOD()->Enqueue(item);
				}

				void Enqueue(T&& item) {
					GOD()->Enqueue(std::forward<T>(item));
				}

				bool TryDequeue(Out<T> result) {
					return GOD()->TryDequeue(result);
				}
			};

			template<class T> class System_API MPMCBoundedQueue : public Object {
			private:

			public:
				class System_API ObjectData : public Object::ObjectData {
				private:
					int size;
					std::atomic<ulong> wi = 0;
					std::atomic<ulong> ri = 0;
					T** data;

				public:
					ObjectData() {}

					ObjectData(int size) : size(size), data(new T* [size]) {
						memset(data, 0, size << ptrshifter);
					}

					~ObjectData() override {

					}

					void Enqueue(const T& item) {





					}

					bool TryDequeue(Out<T>& result) {
						return true;
					}
				};
								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				MPMCBoundedQueue(){}

				MPMCBoundedQueue(std::nullptr_t const & n) : System::Object(n) {
				}

				MPMCBoundedQueue(MPMCBoundedQueue* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				MPMCBoundedQueue(MPMCBoundedQueue const & other) : System::Object(other) { }

				MPMCBoundedQueue(MPMCBoundedQueue&& other) noexcept : System::Object(std::move(other)) { }

				MPMCBoundedQueue(Object::ObjectData* other) : System::Object(other) {
				}

				MPMCBoundedQueue& operator=(MPMCBoundedQueue const & other) {
					System::Object::operator=(other);
					return *this;
				}

				MPMCBoundedQueue& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				MPMCBoundedQueue& operator=(MPMCBoundedQueue&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				MPMCBoundedQueue& operator=(MPMCBoundedQueue* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				MPMCBoundedQueue* operator->() {
					return this;
				}



					MPMCBoundedQueue(int size) {
					od = new ObjectData(size);
				}

				void Enqueue(const T& item) {
					GOD()->Enqueue(item);
				}

				bool TryDequeue(Out<T>& result) {
					return GOD()->TryDequeue(result);
				}

			};



			template<class T>
			using ConcurrentQueue = LockingQueue<T>;


			// A ConcurrentDictionary and Wait-Free Queue from Compare and Swap   William Morriss 2015/05/09
			template<class TKey, class TValue> class System_API WMorrissDictionaryD : public Object::ObjectData {
			private:
				struct node {
					TKey key;
					TValue value;
					int hash;
				};

				static const int b = 4;
				static const int n = 1 << b;
				static const int c = 7; // cache lines are usually 64 bytes, optimizing this for 64-bit architectures that's 8 64-bit pointers, 7 for entries, 1 for the pointer to the next table
				static const int cshift = 3;
				static const int bmask = n - 1;


				struct table_t {
					std::atomic<void*> entries[n * (c + 1)];
				};

				System::Collections::Generic::IEqualityComparer<TKey> comparer;

				table_t* MainTable;

				void DeleteTable(table_t* tbl) {
					for (int i = 0; i < n; i++) {
						void* ptr = tbl->entries[i << cshift].load();
						if (ptr) {
							DeleteTable((table_t*)ptr);
						}
						else {
							for (int j = 0; j < c; j++) {
								ptr = tbl->entries[(i << cshift) + 1 + j].load();
								if (ptr) {
									node* nptr = (node*)ptr;
									//		std::cout << "deletingcout " << nptr->key << std::endl;
									//		std::cerr << "deletingcerr " << nptr->key << std::endl;
									delete nptr;
								}
								else
									break;
							}
						}
					}
					MPool.Put((byte*)tbl);
				}

			public:
				int count;

				void Initialize(int capacity) {
					int numbts = (n * (c + 1)) << ptrshifter;
					MainTable = (table_t*)MPool.Get(numbts);
					memset(MainTable, 0, numbts);
				}

				WMorrissDictionaryD(int capacity, System::Collections::Generic::IEqualityComparer<TKey> comparer) {
					//if (capacity < 0) ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.capacity);
					count = 0;

					Initialize(capacity);
					if (comparer == null)
						this->comparer = System::Collections::Generic::EqualityComparer<TKey>::Default;
					else
						this->comparer = comparer;
				}

				WMorrissDictionaryD(int capacity) : WMorrissDictionaryD(capacity, null) {}

				WMorrissDictionaryD(System::Collections::Generic::IEqualityComparer<TKey> comparer) : WMorrissDictionaryD(0, comparer) {}

				WMorrissDictionaryD() : WMorrissDictionaryD(0) {}

				TValue* TryGetValue(const TKey& key) {
					int hash = comparer.GetHashCode(key);
					table_t* tbl = MainTable;
					int bshift = 0;
					int rowpos;
					do {
						rowpos = ((hash >> bshift) & bmask) << cshift;
						void* ptr = tbl->entries[rowpos].load();
						if (!ptr)
							break;
						bshift += b;
						tbl = (table_t*)ptr;
					} while (1);
					node* newnode = NULL;
					for (int i = 0; i < c; i++) {
						node* ptr = (node*)tbl->entries[rowpos + 1 + i].load();
						if (!ptr) {
							return NULL;
						}
						//if (ptr == TombStone)
						//	continue;
						if (comparer.Equals(key, ptr->key)) {
							return &ptr->value;
						}
					}

					return NULL;
				}

				void Insert(const TKey& key, const TValue& value, bool add) {
					int hash = comparer.GetHashCode(key);
					table_t* tbl = MainTable;
					int bshift = 0;
					int rowpos;
					do {
						do {
							rowpos = ((hash >> bshift) & bmask) << cshift;
							void* ptr = tbl->entries[rowpos].load();
							if (!ptr)
								break;
							bshift += b;
							tbl = (table_t*)ptr;
						} while (1);
						node* newnode = NULL;
						for (int i = 0; i < c; i++) {
							node* ptr = (node*)tbl->entries[rowpos + 1 + i].load();
							if (!ptr) { // || ptr == TombStone) {
								if (!newnode) {
									newnode = new node();
									newnode->hash = hash;
									newnode->key = key;
									newnode->value = value;
								}
								void* expected = (void*)ptr;
								if (tbl->entries[rowpos + 1 + i].compare_exchange_weak(expected, newnode, std::memory_order_release, std::memory_order_relaxed)) {
									return;
								}
								ptr = (node*)expected;
							}
							if (comparer.Equals(key, ptr->key)) {
								ptr->value = value;
							}
						}

						table_t* next = static_cast<table_t*>(tbl->entries[rowpos].load());
						bshift += b;
						if (next) {
							tbl = next;
						}
						else {
							int numbts = (n * (c + 1)) << ptrshifter;
							table_t* newtbl = reinterpret_cast<table_t*>(MPool.Get(numbts));
							memset(newtbl, 0, numbts);
							if (!tbl) // avoid C28182
								throw NullReferenceException();
							for (int i = 0; i < c; i++) {
								node* nod = static_cast<node*>(tbl->entries[rowpos + 1 + i].load());
								int newrowpos = ((nod->hash >> bshift) & bmask) << cshift;
								newrowpos++;
								while (newtbl->entries[newrowpos].load()) { newrowpos++; }
								newtbl->entries[newrowpos].store(nod);
							}
							void* expected = NULL;
							if (tbl->entries[rowpos].compare_exchange_strong(expected, newtbl, std::memory_order_release, std::memory_order_relaxed)) {
								tbl = newtbl;
							}
							else {
								MPool.Put((byte*)newtbl);
								tbl = (table_t*)expected;
							}
						}
					} while (1);
				}

				bool Remove(const TKey& key) {
					throw NotImplementedException();
				}

				~WMorrissDictionaryD() {
					DeleteTable(MainTable);
				}
			};

			template<class TKey, class TValue> class System_API WMorrissDictionary : public virtual Object {
			public:
				WMorrissDictionary() {

				}

				WMorrissDictionary(const std::nullptr_t& n) {
					od = null;
				}

				WMorrissDictionary(int capacity) {
					WMorrissDictionaryD<TKey, TValue>* dd = new WMorrissDictionaryD<TKey, TValue>(capacity);
					od = dd;
				}

				WMorrissDictionary(int capacity, System::Collections::Generic::IEqualityComparer<TKey> comparer) {
					WMorrissDictionaryD<TKey, TValue>* dd = new WMorrissDictionaryD<TKey, TValue>(capacity, comparer);
					od = dd;
				}

				WMorrissDictionary(System::Collections::Generic::IEqualityComparer<TKey> comparer) {
					WMorrissDictionaryD<TKey, TValue>* dd = new WMorrissDictionaryD<TKey, TValue>(comparer);
					od = dd;
				}

				WMorrissDictionary(WMorrissDictionary* pValue) {
					od = pValue->od;
					if (!od) { // 'default explicit' construction
						WMorrissDictionaryD<TKey, TValue>* dd = new WMorrissDictionaryD<TKey, TValue>(0);
						od = dd;
					}
					MPool.Put((byte*)pValue);
				}



				WMorrissDictionary(const WMorrissDictionary& other) : Object(other) {}

				void Add(const TKey& key, const TValue& value) {
					WMorrissDictionaryD<TKey, TValue>* dd = (WMorrissDictionaryD<TKey, TValue>*)od;
					dd->Insert(key, value, true);
				}

				bool TryGetValue(const TKey& key, TValue& value) {
					WMorrissDictionaryD<TKey, TValue>* dd = (WMorrissDictionaryD<TKey, TValue>*)od;
					TValue* ptr = dd->TryGetValue(key);
					if (!ptr)
						return false;
					value = *ptr;
					return true;
				}

				bool Contains(const TKey& key) {
					TValue value;
					return TryGetValue(key, value);
				}

				bool Remove(const TKey& key) {
					WMorrissDictionaryD<TKey, TValue>* dd = (WMorrissDictionaryD<TKey, TValue>*)od;
					return dd->Remove(key);
				}

				TValue& operator[](const TKey& key)
				{
					WMorrissDictionaryD<TKey, TValue>* dd = (WMorrissDictionaryD<TKey, TValue>*)od;
					TValue* val = dd->TryGetValue(key);
					if (val)
						return *val;
					int index = dd->Insert(key, TValue(), true);
					return dd->entries[index].value;
				}


			};


			template<class TKey, class TValue>
			using ConcurrentDictionary = WMorrissDictionary<TKey, TValue>;

		}



	}


	class System_API IAsyncResult : public Object
	{
	private:


	public:
		bool Get_IsCompleted() const { return GOD()->Get_IsCompleted(); }
		bool Get_CompletedSynchronously() const { return GOD()->Get_CompletedSynchronously(); };
		Object Get_AsyncState() const { return GOD()->Get_AsyncState(); };
		System::Threading::WaitHandle Get_AsyncWaitHandle() const { return GOD()->Get_AsyncWaitHandle(); };

		class System_API ObjectData {
		public:
			ObjectData() {
			}

			~ObjectData() {
			}

			virtual bool Get_IsCompleted() const = 0;
			virtual bool Get_CompletedSynchronously() const = 0;
			virtual Object Get_AsyncState() const = 0;
			virtual System::Threading::WaitHandle Get_AsyncWaitHandle() const = 0;
		};

		PropGenGet<bool, IAsyncResult, &IAsyncResult::Get_IsCompleted> IsCompleted{ this };
		PropGenGet<bool, IAsyncResult, &IAsyncResult::Get_CompletedSynchronously> CompletedSynchronously{ this };
		PropGenGet<Object, IAsyncResult, &IAsyncResult::Get_AsyncState> AsyncState{ this };
		PropGenGet<System::Threading::WaitHandle, IAsyncResult, &IAsyncResult::Get_AsyncWaitHandle> AsyncWaitHandle{ this };

				ObjectData* id = nullptr;

		ObjectData* GOD() const { return this->id; };

		IAsyncResult(){}

		IAsyncResult(std::nullptr_t const & n) : System::Object (nullptr), id(nullptr) {
		}

		IAsyncResult(IAsyncResult const & other) : System::Object(other) { this->id = other.id; }

		IAsyncResult(IAsyncResult&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

		IAsyncResult(System::Object::ObjectData* baseod, ObjectData* other) : System::Object(baseod), id(other) {
		}

		IAsyncResult& operator=(IAsyncResult const & other) {
			System::Object::operator=(other);
			id = other.id;
			return *this;
		}

		IAsyncResult& operator=(IAsyncResult&& other) noexcept {
			System::Object::operator=(std::move(other));
			id = other.id;
			return *this;
		}

		IAsyncResult* operator->() {
			return this;
		}






	};

	namespace Threading {
#if _MSC_VER || __MINGW32__
		/// <summary>Represents a thread synchronization event that, when signaled, must be reset manually. This class cannot be inherited.</summary>
		class System_API ManualResetEventWindows : public Object {
		private:
		public:
			class System_API ObjectData : public Object::ObjectData {
			private:
				//		std::condition_variable condition;
				HANDLE evnt;

			public:
				ObjectData(bool initialState) {
					evnt = CreateEvent(NULL, true, initialState, NULL);
				}

				ObjectData() : ObjectData(false) {}

				~ObjectData() override {
					CloseHandle(evnt);
				}

				bool Set() {
					return SetEvent(evnt);
				}

				bool Reset() {
					return ResetEvent(evnt);
				}

				bool WaitOne() {
					DWORD err = WaitForSingleObject(evnt, INFINITE);
					return err == WAIT_OBJECT_0;
				}

				bool WaitOne(const System::TimeSpan& timeout) {
					return false;
				}

				bool WaitOne(int millisecondsTimeout) {
					return false;
				}
			};

			ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			ManualResetEventWindows(){}

			ManualResetEventWindows(std::nullptr_t const & n) : System::Object(n) {
			}

			ManualResetEventWindows(ManualResetEventWindows* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			ManualResetEventWindows(ManualResetEventWindows const & other) : System::Object(other) { }

			ManualResetEventWindows(ManualResetEventWindows&& other) noexcept : System::Object(std::move(other)) { }

			ManualResetEventWindows(Object::ObjectData* other) : System::Object(other) {
			}

			ManualResetEventWindows& operator=(ManualResetEventWindows const & other) {
				System::Object::operator=(other);
				return *this;
			}

			ManualResetEventWindows& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			ManualResetEventWindows& operator=(ManualResetEventWindows&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			ManualResetEventWindows& operator=(ManualResetEventWindows* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			ManualResetEventWindows* operator->() {
				return this;
			}



			/// <summary>Initializes a new instance of the ManualResetEvent class with a Boolean value indicating whether to set the initial state to signaled.</summary>
			/// <param name="initialState">true to set the initial state signaled; false to set the initial state to nonsignaled.</param>
			ManualResetEventWindows(bool initialState) {
				this->od = new ObjectData(initialState);
			}

			/// <summary>Sets the state of the event to signaled, allowing one or more waiting threads to proceed.</summary>
			/// <returns>true if the operation succeeds; otherwise, false.</returns>
			bool Set() const {
				return GOD()->Set();
			}

			/// <summary>Sets the state of the event to nonsignaled, causing threads to block.</summary>
			/// <returns>true if the operation succeeds; otherwise, false.</returns>
			bool Reset() const {
				return GOD()->Reset();
			}

			/// <summary>Blocks the current thread until the current instance receives a signal, using a TimeSpan to specify the time interval.</summary>
			/// <param name="timeout">A TimeSpan that represents the number of milliseconds to wait, or a TimeSpan that represents -1 milliseconds to wait indefinitely.</param>
			/// <returns>true if the current instance receives a signal; otherwise, false.</returns>
			bool WaitOne(TimeSpan const & timeout) const {
				return GOD()->WaitOne(timeout);
			}

			/// <summary>Blocks the current thread until the current WaitHandle receives a signal, using a 32-bit signed integer to specify the time interval in milliseconds.</summary>
			/// <param name="millisecondsTimeout">The number of milliseconds to wait, or Infinite (-1) to wait indefinitely.</param>
			/// <returns>true if the current instance receives a signal; otherwise, false.</returns>
			bool WaitOne(int millisecondsTimeout) const {
				return GOD()->WaitOne(millisecondsTimeout);
			}

			/// <summary>Blocks the current thread until the current WaitHandle receives a signal.</summary>
			/// <returns>true if the current instance receives a signal. If the current instance is never signaled, WaitOne() never returns.</returns>
			bool WaitOne() const {
				return GOD()->WaitOne();
			}
		};
#endif

		/// <summary>Represents a thread synchronization event that, when signaled, must be reset manually. This class cannot be inherited.</summary>
		class System_API ManualResetEventConditionVariable : public Object {
		private:
		public:
			class System_API ObjectData : public Object::ObjectData {
			private:
				std::condition_variable condition;
				std::mutex mut;
				std::atomic<bool> state;

			public:
				ObjectData(bool initialState) {
					state.store(initialState);
				}

				ObjectData() : ObjectData(false) {}

				bool Set() {
					std::unique_lock<std::mutex> lock(mut);
					state.store(true);
					lock.unlock();
					condition.notify_all();

					return true;
				}

				bool Reset() {
					std::unique_lock<std::mutex> lock(mut);
					state.store(false);
					return true;
				}

				bool WaitOne() {
					std::unique_lock<std::mutex> lock(mut);
					condition.wait(lock, [&]() {return state.load(); });

					return true;
				}

				bool WaitOne(System::TimeSpan const& timeout) {
					std::unique_lock<std::mutex> lock(mut);
					std::chrono::duration<long, std::milli> dur(timeout.TotalMilliseconds);
					return condition.wait_for(lock, dur, [&]() { return state.load(); });
				}

				bool WaitOne(int millisecondsTimeout) {
					if (millisecondsTimeout == -1)
						return WaitOne();
					std::unique_lock<std::mutex> lock(mut);
					std::chrono::duration<long, std::milli> dur(millisecondsTimeout);
					return condition.wait_for(lock, dur, [&]() { return state.load(); });
				}
			};

			ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			ManualResetEventConditionVariable(){}

			ManualResetEventConditionVariable(std::nullptr_t const & n) : System::Object(n) {
			}

			ManualResetEventConditionVariable(ManualResetEventConditionVariable* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			ManualResetEventConditionVariable(ManualResetEventConditionVariable const & other) : System::Object(other) { }

			ManualResetEventConditionVariable(ManualResetEventConditionVariable&& other) noexcept : System::Object(std::move(other)) { }

			ManualResetEventConditionVariable(Object::ObjectData* other) : System::Object(other) {
			}

			ManualResetEventConditionVariable& operator=(ManualResetEventConditionVariable const & other) {
				System::Object::operator=(other);
				return *this;
			}

			ManualResetEventConditionVariable& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			ManualResetEventConditionVariable& operator=(ManualResetEventConditionVariable&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			ManualResetEventConditionVariable& operator=(ManualResetEventConditionVariable* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			ManualResetEventConditionVariable* operator->() {
				return this;
			}



			/// <summary>Initializes a new instance of the ManualResetEvent class with a Boolean value indicating whether to set the initial state to signaled.</summary>
			/// <param name="initialState">true to set the initial state signaled; false to set the initial state to nonsignaled.</param>
			ManualResetEventConditionVariable(bool initialState) {
				this->od = new ObjectData(initialState);
			}

			/// <summary>Sets the state of the event to signaled, allowing one or more waiting threads to proceed.</summary>
			/// <returns>true if the operation succeeds; otherwise, false.</returns>
			bool Set() const {
				return GOD()->Set();
			}

			/// <summary>Sets the state of the event to nonsignaled, causing threads to block.</summary>
			/// <returns>true if the operation succeeds; otherwise, false.</returns>
			bool Reset() const {
				return GOD()->Reset();
			}

			/// <summary>Blocks the current thread until the current instance receives a signal, using a TimeSpan to specify the time interval.</summary>
			/// <param name="timeout">A TimeSpan that represents the number of milliseconds to wait, or a TimeSpan that represents -1 milliseconds to wait indefinitely.</param>
			/// <returns>true if the current instance receives a signal; otherwise, false.</returns>
			bool WaitOne(TimeSpan const& timeout) const {
				return GOD()->WaitOne(timeout);
			}

			/// <summary>Blocks the current thread until the current WaitHandle receives a signal, using a 32-bit signed integer to specify the time interval in milliseconds.</summary>
			/// <param name="millisecondsTimeout">The number of milliseconds to wait, or Infinite (-1) to wait indefinitely.</param>
			/// <returns>true if the current instance receives a signal; otherwise, false.</returns>
			bool WaitOne(int millisecondsTimeout) const {
				return GOD()->WaitOne(millisecondsTimeout);
			}

			/// <summary>Blocks the current thread until the current WaitHandle receives a signal.</summary>
			/// <returns>true if the current instance receives a signal. If the current instance is never signaled, WaitOne() never returns.</returns>
			bool WaitOne() const {
				return GOD()->WaitOne();
			}
		};

#if _MSC_VER || __MINGW32__
		using ManualResetEvent = ManualResetEventConditionVariable; // ManualResetEventWindows;
#else
		using ManualResetEvent = ManualResetEventConditionVariable;
#endif

	}
}


namespace System {
	namespace Threading {
		class System_API SimpleThreadPool : public Object
		{
		private:
			static int Get_ThreadCount() {
				return _Instance.GetThreadCount();
			}
		public:
			class System_API ObjectData : public Object::ObjectData {
			private:
			public:
				std::atomic<int> NumberOfThreads{};
				int MaxThreads = 8;

				class SimpleThread
				{
				private:
				public:
					Thread t;
					System::Collections::Concurrent::ConcurrentQueue<std::function<void()>> ActionQueue;

					void Run()
					{
						//int lastcnt = 0;
						std::function<void()> fptr;
						while (true)
						{
							//int cnt = ActionQueue.Count;
							//if (lastcnt != cnt) {
							//	lastcnt = cnt;
							//	Console::WriteLine("in thread " + string(lastcnt));
							//}
							if (ActionQueue.TryDequeue(out(fptr)))
							{
								fptr();
							}
							Thread::Sleep(10);
						}
					}
				};

				System::Collections::Concurrent::ConcurrentQueue<std::function<void()>> ActionQueue;

				ObjectData() : ActionQueue(new System::Collections::Concurrent::ConcurrentQueue<std::function<void()>>())
				{

				}

				void Execute(Action<> const& act)
				{
					ActionQueue.Enqueue(act);
					int numt = NumberOfThreads.load(std::memory_order_relaxed);
					while (numt < MaxThreads) {
						if (NumberOfThreads.compare_exchange_weak(numt, numt + 1)) {
							SimpleThread* nst = new SimpleThread();
							nst->ActionQueue = ActionQueue;
							nst->t = Thread([nst]() {
								nst->Run();
								});
							nst->t.IsBackground = true;
							nst->t.Start();
						}
					}
				}


			};

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			SimpleThreadPool(){}

			SimpleThreadPool(std::nullptr_t const & n) {
				this->od = nullptr;
			}

			SimpleThreadPool(SimpleThreadPool* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					this->od = dd;
				}
				else {
					this->od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			SimpleThreadPool(SimpleThreadPool const & other) : System::Object(other) { }

			SimpleThreadPool(SimpleThreadPool&& other) noexcept : System::Object(std::move(other)) { }

			SimpleThreadPool(ObjectData* other) {
				this->od = other;
				if (this->od)
					this->AddRef();
			}

			SimpleThreadPool& operator=(SimpleThreadPool const & other) {
				System::Object::operator=(other);
				return *this;
			}

			SimpleThreadPool& operator=(SimpleThreadPool* other) {
				if (this->od == other->od)
					return *this;
				this->Release();
				this->od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			SimpleThreadPool& operator=(SimpleThreadPool&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			SimpleThreadPool* operator->() {
				return this;
			}



			SimpleThreadPool(int NumberOfThreads)
			{
				ObjectData* dd = new ObjectData();
				dd->MaxThreads = NumberOfThreads;
				this->od = dd;
			}

			static SimpleThreadPool _Instance;
			static PropGenGetStatic<int, &SimpleThreadPool::Get_ThreadCount> ThreadCount;

			static void QueueUserWorkItem(Action<> const & act)
			{
				_Instance.Execute(act);
			}

			void Execute(Action<> const& act)
			{
				GOD()->Execute(act);
			}

			int GetThreadCount() {
				return GOD()->NumberOfThreads.load(std::memory_order_relaxed);
			}

			void GetMaxThreads(Out<int> workerThreads, Out<int> completionPortThreads) {
				workerThreads = GOD()->MaxThreads;
				completionPortThreads = 0; // TODO?
			}

		};

#ifndef SYSTEM_EXPORTS
#if _MSC_VER || __MINGW32__
		SimpleThreadPool SimpleThreadPool::_Instance{ 8 };
#elif ESP32
		SimpleThreadPool SimpleThreadPool::_Instance{ null };
#endif
		PropGenGetStatic<int, &SimpleThreadPool::Get_ThreadCount> SimpleThreadPool::ThreadCount{};
#endif



		using ThreadPool = SimpleThreadPool;
	}
}

namespace System {
	namespace Diagnostics {
		class System_API Stopwatch : public Object
		{
		private:

			TimeSpan GetElapsed() const
			{
				ObjectData* dd = GOD();
				return TimeSpan((dd->nanoend - dd->nanostart) / 100);
			}

			long GetElapsedMilliseconds() const
			{
				ObjectData* dd = GOD();
				return (dd->nanoend - dd->nanostart) / 1000000;
			}

			long GetElapsedTicks() const
			{
				ObjectData* dd = GOD();
				return (dd->nanoend - dd->nanostart) / 100;
			}


		public:
			class System_API ObjectData : public Object::ObjectData {
			private:
			public:
				long nanostart = 0;
				long nanoend = 0;
				bool isRunning = false;

				void Start() {
					if (!isRunning) {
						std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
						nanostart = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
						isRunning = true;
					}
				}

				void Stop()
				{
					if (isRunning) {
						std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
						nanoend = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
						isRunning = false;
					}
				}

				void Reset()
				{
					isRunning = false;
					nanoend = nanostart = 0;
				}

				void Restart() {
					Reset();
					Start();
				}
			};
			PropGenGet<TimeSpan, Stopwatch, &Stopwatch::GetElapsed> Elapsed{ this };
			PropGenGet<long, Stopwatch, &Stopwatch::GetElapsedMilliseconds> ElapsedMilliseconds{ this };
			PropGenGet<long, Stopwatch, &Stopwatch::GetElapsedTicks> ElapsedTicks{ this };

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			Stopwatch(){}

			Stopwatch(std::nullptr_t const & n) : System::Object(n) {
			}

			Stopwatch(Stopwatch* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			Stopwatch(Stopwatch const & other) : System::Object(other) { }

			Stopwatch(Stopwatch&& other) noexcept : System::Object(std::move(other)) { }

			Stopwatch(Object::ObjectData* other) : System::Object(other) {
			}

			Stopwatch& operator=(Stopwatch const & other) {
				System::Object::operator=(other);
				return *this;
			}

			Stopwatch& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			Stopwatch& operator=(Stopwatch&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			Stopwatch& operator=(Stopwatch* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			Stopwatch* operator->() {
				return this;
			}



				void Start() {
				ObjectData* dd = GOD();
				dd->Start();
			}

			void Stop()
			{
				ObjectData* dd = GOD();
				dd->Stop();
			}

			void Reset()
			{
				ObjectData* dd = GOD();
				dd->Reset();
			}

			void Restart() {
				ObjectData* dd = GOD();
				dd->Restart();
			}

			static Stopwatch StartNew() {
				Stopwatch s = new Stopwatch();
				s.Start();
				return s;
			}

			static long GetTimestamp() {
				std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
				long nano = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();

				return nano / 100;
			}
		};


		enum class ProcessWindowStyle {
			Normal = 0,
			Hidden = 1,
			Minimized = 2,
			Maximized = 3
		};

		class System_API ProcessStartInfoD : public Object::ObjectData {
		private:
		public:
			bool UseShellExecute = true; // The default is true on.NET Framework apps and false on.NET Core apps.
			bool CreateNoWindow = false;
			ProcessWindowStyle WindowStyle;
			System::String FileName;
			System::String Verb;
			System::String Arguments;
			System::String WorkingDirectory;

			ProcessStartInfoD() {
				FileName = "";
				Verb = "";
				Arguments = "";
				WorkingDirectory = "";
			}

			virtual ~ProcessStartInfoD() {

			}
		};

		class System_API ProcessStartInfo : public Object {
		private:
			bool& GetUseShellExecute() const {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				return dd->UseShellExecute;
			}

			void SetUseShellExecute(const bool& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->UseShellExecute = value;
			}

			void SetUseShellExecute(bool&& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->UseShellExecute = (bool&&)value;
			}

			bool& GetCreateNoWindow() const {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				return dd->CreateNoWindow;
			}

			void SetCreateNoWindow(const bool& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->CreateNoWindow = value;
			}

			void SetCreateNoWindow(bool&& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->CreateNoWindow = (bool&&)value;
			}

			ProcessWindowStyle& GetWindowStyle() const {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				return dd->WindowStyle;
			}

			void SetWindowStyle(const ProcessWindowStyle& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->WindowStyle = value;
			}

			void SetWindowStyle(ProcessWindowStyle&& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->WindowStyle = (ProcessWindowStyle&&)value;
			}

			System::String& GetFileName() const {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				return dd->FileName;
			}

			void SetFileName(const System::String& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->FileName = value;
			}

			void SetFileName(System::String&& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->FileName = (System::String&&)value;
			}

			System::String& GetVerb() const {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				return dd->Verb;
			}

			void SetVerb(const System::String& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->Verb = value;
			}

			void SetVerb(System::String&& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->Verb = (System::String&&)value;
			}

			System::String& GetArguments() const {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				return dd->Arguments;
			}

			void SetArguments(const System::String& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->Arguments = value;
			}

			void SetArguments(System::String&& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->Arguments = (System::String&&)value;
			}

			System::String& GetWorkingDirectory() const {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				return dd->WorkingDirectory;
			}

			void SetWorkingDirectory(const System::String& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->WorkingDirectory = value;
			}

			void SetWorkingDirectory(System::String&& value) {
				ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				dd->WorkingDirectory = (System::String&&)value;
			}

		public:
			PropGen<bool, bool, ProcessStartInfo, &ProcessStartInfo::GetUseShellExecute, &ProcessStartInfo::SetUseShellExecute, &ProcessStartInfo::SetUseShellExecute> UseShellExecute{ this };
			PropGen<bool, bool, ProcessStartInfo, &ProcessStartInfo::GetCreateNoWindow, &ProcessStartInfo::SetCreateNoWindow, &ProcessStartInfo::SetCreateNoWindow> CreateNoWindow{ this };
			PropGen<ProcessWindowStyle, ProcessWindowStyle, ProcessStartInfo, &ProcessStartInfo::GetWindowStyle, &ProcessStartInfo::SetWindowStyle, &ProcessStartInfo::SetWindowStyle> WindowStyle{ this };
			PropGen<System::String, System::String, ProcessStartInfo, &ProcessStartInfo::GetFileName, &ProcessStartInfo::SetFileName, &ProcessStartInfo::SetFileName> FileName{ this };
			PropGen<System::String, System::String, ProcessStartInfo, &ProcessStartInfo::GetVerb, &ProcessStartInfo::SetVerb, &ProcessStartInfo::SetVerb> Verb{ this };
			PropGen<System::String, System::String, ProcessStartInfo, &ProcessStartInfo::GetArguments, &ProcessStartInfo::SetArguments, &ProcessStartInfo::SetArguments> Arguments{ this };
			PropGen<System::String, System::String, ProcessStartInfo, &ProcessStartInfo::GetWorkingDirectory, &ProcessStartInfo::SetWorkingDirectory, &ProcessStartInfo::SetWorkingDirectory> WorkingDirectory{ this };

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			ProcessStartInfo(){}

			ProcessStartInfo(std::nullptr_t const & n) : System::Object(n) {
			}

			ProcessStartInfo(ProcessStartInfo* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			ProcessStartInfo(ProcessStartInfo const & other) : System::Object(other) { }

			ProcessStartInfo(ProcessStartInfo&& other) noexcept : System::Object(std::move(other)) { }

			ProcessStartInfo(Object::ObjectData* other) : System::Object(other) {
			}

			ProcessStartInfo& operator=(ProcessStartInfo const & other) {
				System::Object::operator=(other);
				return *this;
			}

			ProcessStartInfo& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			ProcessStartInfo& operator=(ProcessStartInfo&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			ProcessStartInfo& operator=(ProcessStartInfo* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			ProcessStartInfo* operator->() {
				return this;
			}




		};



		class System_API Process : public Object {
		private:
			ProcessStartInfo& GetStartInfo() const {
				ObjectData* dd = GOD();
				return dd->StartInfo;
			}

			void SetStartInfo(const ProcessStartInfo& value) {
				if (value == null)
					throw ArgumentNullException("value");
				ObjectData* dd = GOD();
				dd->StartInfo = value;
			}

			void SetStartInfo(ProcessStartInfo&& value) {
				ObjectData* dd = GOD();
				dd->StartInfo = (ProcessStartInfo&&)value;
			}

			System::String GetProcessName() const {
				ObjectData* dd = GOD();
				return dd->ProcessName;
			}

			void SetProcessName(System::String&& value) {
				if (value == null)
					throw ArgumentNullException("value");
				ObjectData* dd = GOD();
				dd->ProcessName = std::move(value);
			}

#if _MSC_VER
			void SetProcessHandle(HANDLE Handle) {
				ObjectData* dd = GOD();
				dd->Handle = Handle;
			}
#endif

			TimeSpan GetTotalProcessorTime() const {
#if _MSC_VER
				ObjectData* dd = GOD();
				FILETIME ftProcCreation, ftProcExit, ftProcKernel, ftProcUser;
				if (GetProcessTimes(dd->Handle, &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser)) {
					ulong tickskernel = ftProcKernel.dwLowDateTime + ((ulong)ftProcKernel.dwHighDateTime << 32);
					ulong ticksuser = ftProcUser.dwLowDateTime + ((ulong)ftProcUser.dwHighDateTime << 32);
					return TimeSpan((long)(tickskernel + ticksuser));
				}
#endif

				return TimeSpan::Zero;
			}



			bool StartWithShellExecute(const ProcessStartInfo& startInfo) {
#if _MSC_VER

				SHELLEXECUTEINFO shellExecuteInfo;
				memset((void*)&shellExecuteInfo, 0, sizeof(SHELLEXECUTEINFO));
				shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
				//if (startInfo.ErrorDialog) {
				//	shellExecuteInfo.hwnd = startInfo.ErrorDialogParentHandle;
				//}
				//else {
				shellExecuteInfo.fMask |= SEE_MASK_FLAG_NO_UI;
				//}

				switch (startInfo.WindowStyle) {
				case ProcessWindowStyle::Hidden:
					shellExecuteInfo.nShow = SW_HIDE;
					break;
				case ProcessWindowStyle::Minimized:
					shellExecuteInfo.nShow = SW_SHOWMINIMIZED;
					break;
				case ProcessWindowStyle::Maximized:
					shellExecuteInfo.nShow = SW_SHOWMAXIMIZED;
					break;
				default:
					shellExecuteInfo.nShow = SW_SHOWNORMAL;
					break;
				}
				if (startInfo.FileName->Length != 0)
					shellExecuteInfo.lpFile = (LPCWSTR)startInfo.FileName->ToCharArray();
				if (startInfo.Verb->Length != 0)
					shellExecuteInfo.lpVerb = (LPCWSTR)startInfo.Verb->ToCharArray();
				if (startInfo.Arguments->Length != 0)
					shellExecuteInfo.lpParameters = (LPCWSTR)startInfo.Arguments->ToCharArray();
				if (startInfo.WorkingDirectory->Length != 0)
					shellExecuteInfo.lpDirectory = (LPCWSTR)startInfo.WorkingDirectory->ToCharArray();

				shellExecuteInfo.fMask |= SEE_MASK_FLAG_DDEWAIT;
				bool okay = false;
				System::Threading::Thread executionThread = new System::Threading::Thread([&shellExecuteInfo, &okay]() {
					if (!ShellExecuteEx((SHELLEXECUTEINFO*)&shellExecuteInfo)) {
						DWORD error = GetLastError();
					}
					okay = true;
					});
				executionThread.SetApartmentState(System::Threading::ApartmentState::STA);
				executionThread.Start();
				executionThread.Join();

				if (okay) {
					ObjectData* dd = GOD();
					dd->Handle = (void*)shellExecuteInfo.hProcess;
				}

				return okay;
#else

				return false;
#endif
			}




		public:
			class System_API ObjectData : public Object::ObjectData {
			private:
			public:
				ProcessStartInfo StartInfo = null;
				System::String ProcessName = null;
				void* Handle = nullptr;

				ObjectData() : StartInfo(new ProcessStartInfo()) {
				}

				~ObjectData() override {
#if _MSC_VER
					if (Handle != nullptr) {
						CloseHandle(Handle);
					}

#endif	
				}

			};
						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			Process(){}

			Process(std::nullptr_t const & n) : System::Object(n) {
			}

			Process(Process* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			Process(Process const & other) : System::Object(other) { }

			Process(Process&& other) noexcept : System::Object(std::move(other)) { }

			Process(Object::ObjectData* other) : System::Object(other) {
			}

			Process& operator=(Process const & other) {
				System::Object::operator=(other);
				return *this;
			}

			Process& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			Process& operator=(Process&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			Process& operator=(Process* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			Process* operator->() {
				return this;
			}



				PropGen<ProcessStartInfo, ProcessStartInfo, Process, &Process::GetStartInfo, &Process::SetStartInfo, &Process::SetStartInfo> StartInfo{ this };
			PropGenGet<System::String, Process, &Process::GetProcessName> ProcessName{ this };
			PropGenGet<TimeSpan, Process, &Process::GetTotalProcessorTime> TotalProcessorTime{ this };


			static System::Collections::Generic::Array<Process> GetProcesses() {
#if _MSC_VER
				DWORD aProcesses[1024], cbNeeded;
				if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
					return null;
				}
				int numfound = cbNeeded / sizeof(DWORD);

				System::Collections::Generic::List<Process> lst = new System::Collections::Generic::List<Process>();


				for (int i = 0; i < numfound; i++) {
					DWORD processID = aProcesses[i];
					HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processID);
					if (hProcess != NULL) {
						HMODULE hMod;

						if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
							TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
							GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
							System::String pName = (char16_t*)szProcessName;
							Process proc = new Process();
							proc.SetProcessHandle(hProcess);
							proc.SetProcessName(std::move(pName));


							lst.Add(std::move(proc));
							//if (lst.Count == 6)
							//	break;
						}
					}
				}

				return lst.ToArray();
				//const int DefaultCachedBufferSize = 128 * 1024;
				//int bufferSize = DefaultCachedBufferSize;
				//long* buffer = new long[bufferSize];
				//int status = NtQuerySystemInformation();
#endif

				return null;
			}


			bool Start(const ProcessStartInfo& psi) {
#if _MSC_VER

				if (psi.UseShellExecute) {
					return StartWithShellExecute(psi);
				}


				//ProcessStartInfoD* dd = (ProcessStartInfoD*)this->od;
				// additional information
				STARTUPINFO si;
				PROCESS_INFORMATION pi;

				// set the size of the structures
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				ZeroMemory(&pi, sizeof(pi));


				// start the program up
				CreateProcess((LPCWSTR)(psi.FileName->ToCharArray()),   // the path
					(LPWSTR)null,//dd->Arguments.ToCharArray(),        // Command line
					NULL,           // Process handle not inheritable
					NULL,           // Thread handle not inheritable
					FALSE,          // Set handle inheritance to FALSE
					0,              // No creation flags
					NULL,           // Use parent's environment block
					NULL,           // Use parent's starting directory 
					&si,            // Pointer to STARTUPINFO structure
					&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
				);
				// Close process and thread handles. 
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				return true;

#else
				return true;
#endif


			}

			bool Start() {
				return Start(StartInfo);
			}

			bool WaitForExit(int milliseconds) {
				//if (od == nullptr)
				//	throw NullReferenceException();
				

#if _MSC_VER
				ObjectData* dd = static_cast<ObjectData*>(od);
				//if (dd->Handle == nullptr)
//	throw NullReferenceException();
				DWORD ret = WaitForSingleObject(dd->Handle, milliseconds);
				CloseHandle(dd->Handle);
				dd->Handle = nullptr;
				return ret == 0;
#endif

				return false;
			}

		};


	}




	namespace IO {

		class System_API IOException : public Exception {
		public:
			IOException() : Exception("IOException") {}

			IOException(System::String& str) : Exception(str) {

			}

			IOException(System::String&& str) : Exception((System::String&&)str) {

			}

		};

		class System_API Stream : public Object, public IDisposable {
		public:
			bool CanRead = false;
			bool CanWrite = false;
			long Length = 0;
			int ReadTimeout = 0;
			int WriteTimeout = 0;

			virtual void Dispose(bool disposing) {

			}

			virtual void Close() {
				Dispose(true);
			}

			virtual void Dispose() {
				Close();
			}

			virtual int Read(byte* buffer, int offset, int count) {
				return 0;
			}

			virtual void Write(byte* buffer, int offset, int count) {}

		};

		class Path {
		private:
		public:
#if _MSC_VER
			static const char16_t DirectorySeparatorChar = '\\';
			static const char16_t AltDirectorySeparatorChar = '/';
#else
			static const char16_t DirectorySeparatorChar = '/';
			static const char16_t AltDirectorySeparatorChar = '\\';
#endif
			static const char16_t VolumeSeparatorChar = ':';

			static System::String GetExtension(System::String path) {
				if (path == null)
					return null;
				int length = path.Length;
				for (int i = length; i-- > 0;) {
					char16_t ch = path[i];
					if (ch == u'.') {
						if (i != length - 1)
							return path.Substring(i, length - i);
						else
							return "";
					}
					if (ch == DirectorySeparatorChar || ch == AltDirectorySeparatorChar || ch == VolumeSeparatorChar)
						break;
				}

				return "";
			}

			static bool IsPathRooted(System::String path) {
				if (path == null)
					return false;
				int length = path.Length;
				return ((length >= 1 && (path[0] == DirectorySeparatorChar || path[0] == AltDirectorySeparatorChar)) || (length >= 2 && path[1] == VolumeSeparatorChar));
			}


			static System::String Combine(const std::initializer_list<System::String> paths) {
				std::initializer_list<System::String>::iterator it;  // same as: const int* it

				int finalSize = 0;
				int firstComponent = 0;
				int i = 0;
				for (i = 0, it = paths.begin(); it != paths.end(); ++it, i++) {
					System::String pth = *it;
					if (Path::IsPathRooted(pth)) {
						firstComponent = i;
						finalSize = pth.Length;
					}
					else {
						finalSize += pth.Length;
					}

					char16_t ch = pth[pth.Length - 1];
					if (ch != DirectorySeparatorChar && ch != AltDirectorySeparatorChar && ch != VolumeSeparatorChar)
						finalSize++;
				}

				// TODO: use StringBuilder
				System::String finalPath = "";
				for (i = 0, it = paths.begin(); it != paths.end(); ++it, i++) {
					if (i < firstComponent)
						continue;
					System::String pth = *it;
					if (pth.Length == 0)
						continue;
					if (finalPath.Length == 0) {
						finalPath = pth;
					}
					else {
						char16_t ch = finalPath[finalPath.Length - 1];
						if (ch != DirectorySeparatorChar && ch != AltDirectorySeparatorChar && ch != VolumeSeparatorChar)
							finalPath += DirectorySeparatorChar;
						finalPath += pth;
					}
				}

				return finalPath;
			}

			static System::String Combine(System::String path1, System::String path2) {
				if (path1 == null || path2 == null)
					throw ArgumentNullException();
				return Combine({ path1, path2 });
			}

			static System::String Combine(System::String path1, System::String path2, System::String path3) {
				if (path1 == null || path2 == null || path3 == null)
					throw ArgumentNullException();
				return Combine({ path1, path2, path3 });
			}

			static System::String Combine(System::String path1, System::String path2, System::String path3, System::String path4) {
				if (path1 == null || path2 == null || path3 == null || path4 == null)
					throw ArgumentNullException();
				return Combine({ path1, path2, path3, path4 });
			}

			static System::String Combine(System::String path1, System::String path2, System::String path3, System::String path4, System::String path5) {
				if (path1 == null || path2 == null || path3 == null || path4 == null || path5 == null)
					throw ArgumentNullException();
				return Combine({ path1, path2, path3, path4, path5 });
			}

#undef GetTempPath
			static System::String GetTempPath() {
#if _MSC_VER

				std::wstring TempPath;
				wchar_t wcharPath[MAX_PATH];
#ifdef UNICODE
				if (GetTempPathW(MAX_PATH, wcharPath))
#else
				if (GetTempPathA(MAX_PATH, wcharPath))
#endif
					TempPath = wcharPath;

				return (char16_t*)wcharPath;
#else

					
				return null;
#endif

			}

			//			static System::String GetTempPathA() {
			//#pragma push_macro("GetTempPath")
			//#undef GetTempPath
			//				return GetTempPath();
			//#pragma pop_macro("GetTempPath")
			//			}
			//
			//			static System::String GetTempPathW() {
			//#pragma push_macro("GetTempPath")
			//#undef GetTempPath
			//				return GetTempPath();
			//#pragma pop_macro("GetTempPath")
			//			}

		};


		class System_API File {

		public:
			static bool Exists(System::String path) {
				// the file is open with the ios::ate flag, which means that the get pointer will be positioned at the end of the file. This way, when we call to member tellg(), we will directly obtain the size of the file.
				// windows expects UTF16 file names in a wchar_t*, linux UTF8 in a char*
#if defined(_MSC_VER) //_MSC_VER
				std::ifstream file((wchar_t*)path.ToCharArray(), std::ios::in | std::ios::binary | std::ios::ate);
#else
				std::ifstream file((const char*)(const byte*)System::Text::Encoding::UTF8.GetBytes(path), std::ios::in | std::ios::binary | std::ios::ate);
#endif		
				if (!file.is_open())
					return false;
				file.close();

				return true;
			}

			static System::Collections::Generic::Array<byte> ReadAllBytes(System::String path) {
				// the file is open with the ios::ate flag, which means that the get pointer will be positioned at the end of the file. This way, when we call to member tellg(), we will directly obtain the size of the file.
				// windows expects UTF16 file names in a wchar_t*, linux UTF8 in a char*
#if defined(_MSC_VER) // _MSC_VER
				std::ifstream file((wchar_t*)path.ToCharArray(), std::ios::in | std::ios::binary | std::ios::ate);
#else
				std::ifstream file((const char*)(const byte*)System::Text::Encoding::UTF8.GetBytes(path), std::ios::in | std::ios::binary | std::ios::ate);
#endif		
				if (!file.is_open())
					throw IOException();

				std::streampos size = file.tellg();
				file.seekg(std::ios::beg);
				uint isize = (uint)size;

				byte* memblock = new byte[isize];

				file.read((char*)memblock, size);
				file.close();

				System::Collections::Generic::Array<byte> ret((byte*)memblock);
				return ret;
			}

			static String ReadAllText(String path) {
				System::Collections::Generic::Array<byte> bts = ReadAllBytes(path);

				String ret = System::Text::Encoding::UTF8.GetString(bts, 0, bts.Length);

				return ret;

				//	throw NotImplementedException();
			}


			static void WriteAllBytes(String Path, System::Collections::Generic::Array<byte>* bytes) {
				throw NotImplementedException();
			}
		};

		class FileInfoD : public Object::ObjectData {
		private:
		public:
			System::String path;
		};


		class FileInfo : public Object {
		private:
		public:

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			FileInfo(){}

			FileInfo(std::nullptr_t const & n) : System::Object(n) {
			}

			FileInfo(FileInfo* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			FileInfo(FileInfo const & other) : System::Object(other) { }

			FileInfo(FileInfo&& other) noexcept : System::Object(std::move(other)) { }

			FileInfo(Object::ObjectData* other) : System::Object(other) {
			}

			FileInfo& operator=(FileInfo const & other) {
				System::Object::operator=(other);
				return *this;
			}

			FileInfo& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			FileInfo& operator=(FileInfo&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			FileInfo& operator=(FileInfo* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			FileInfo* operator->() {
				return this;
			}



				FileInfo(System::String path) {
				FileInfoD* dd = new FileInfoD();
				dd->path = path;
				this->od = dd;
			}
		};



		class DirectoryInfoD : public Object::ObjectData {
		private:
		public:
			System::String path;


		};


		//template<typename T, typename C>
		//class Property {
		//public:
		//	using SetterType = void (C::*)(T);
		//	using GetterType = T(C::*)() const;

		//	Property(C* theObject, SetterType theSetter, GetterType theGetter)
		//		:itsObject(theObject),
		//		itsSetter(theSetter),
		//		itsGetter(theGetter)
		//	{ }

		//	operator T() const
		//	{
		//		return (itsObject->*itsGetter)();
		//	}

		//	C& operator = (T theValue) {
		//		(itsObject->*itsSetter)(theValue);
		//		return *itsObject;
		//	}

		//private:
		//	C* const itsObject;
		//	SetterType const itsSetter;
		//	GetterType const itsGetter;
		//};


		class DirectoryInfo : public Object {
		private:

			System::String mFullName() const {
				DirectoryInfoD* dd = (DirectoryInfoD*)this->od;
				return dd->path;
			}

			System::String mName() const {
				DirectoryInfoD* dd = (DirectoryInfoD*)this->od;
				System::String str = dd->path;
				int ilastpart = str.LastIndexOf(Path::DirectorySeparatorChar);
				return str.Substring(ilastpart + 1);
			}

			bool mExists() const {
#if _MSC_VER
				DirectoryInfoD* dd = (DirectoryInfoD*)this->od;


				WIN32_FIND_DATA FindFileData;
				HANDLE hFind;

				hFind = FindFirstFile((LPCWSTR)dd->path.ToCharArray(), &FindFileData);
				if (hFind == INVALID_HANDLE_VALUE) {
					return false;
				}



				FindClose(hFind);
				return true;

#else
				DirectoryInfoD* dd = (DirectoryInfoD*)this->od;
				DIR* pDir;
				bool bExists = false;

				pDir = opendir((const char*)(byte*)System::Text::Encoding::UTF8.GetBytes(dd->path));

				if (pDir != NULL)
				{
					bExists = true;
					closedir(pDir);
				}

				return bExists;
#endif
			}



			template <class T>
			System::Collections::Generic::Array<T> GetFilesOrDirs(bool dirs) {
				// there's std::is_same to check for types, but only in c++17, so we pass it as an argument
#if _MSC_VER
				DirectoryInfoD* dd = (DirectoryInfoD*)this->od;
				System::Collections::Generic::List<T> ret = new System::Collections::Generic::List<T>();
				WIN32_FIND_DATA ffd;
				HANDLE hFind;

				hFind = FindFirstFile((LPCWSTR)(dd->path + u"\\*").ToCharArray(), &ffd);
				if (hFind == INVALID_HANDLE_VALUE)
				{
					throw IOException();
				}
				else
				{
					do
					{
						char16_t* fname = (char16_t*)ffd.cFileName;

						if (fname[0] == u'.')
						{
							if (fname[1] == u'.') {
								if (fname[2] == 0)
									continue;
							}
							else if (fname[1] == 0) {
								continue;
							}
						}


						if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							if (dirs)
								ret.Add(T(dd->path + Path::DirectorySeparatorChar + fname));
						}
						else
						{
							if (!dirs)
								ret.Add(T(dd->path + Path::DirectorySeparatorChar + fname));
						}
					} while (FindNextFile(hFind, &ffd) != 0);

					FindClose(hFind);
				}

				return ret.ToArray();
#else
				DirectoryInfoD* dd = (DirectoryInfoD*)this->od;
				System::Collections::Generic::List<T> ret = new System::Collections::Generic::List<T>();

				struct dirent* entry;
				DIR* dp = nullptr;
				dp = opendir((const char*)(byte*)System::Text::Encoding::UTF8.GetBytes(dd->path));
				if (dp) {
					while ((entry = readdir(dp)) != NULL) {
						//std::cout << entry->d_name << std::endl;
						char* fname = entry->d_name;
						if (fname[0] == '.')
						{
							if (fname[1] == '.') {
								if (fname[2] == 0)
									continue;
							}
							else if (fname[1] == 0) {
								continue;
							}
						}

						ret.Add(T(dd->path + Path::DirectorySeparatorChar + entry->d_name));
					}
					closedir(dp);
				}

				return ret.ToArray();
#endif
			}


		public:
			PropGenGet<bool, DirectoryInfo, &DirectoryInfo::mExists> Exists{ this };

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			DirectoryInfo(){}

			DirectoryInfo(std::nullptr_t const & n) : System::Object(n) {
			}

			DirectoryInfo(DirectoryInfo* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			DirectoryInfo(DirectoryInfo const & other) : System::Object(other) { }

			DirectoryInfo(DirectoryInfo&& other) noexcept : System::Object(std::move(other)) { }

			DirectoryInfo(Object::ObjectData* other) : System::Object(other) {
			}

			DirectoryInfo& operator=(DirectoryInfo const & other) {
				System::Object::operator=(other);
				return *this;
			}

			DirectoryInfo& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			DirectoryInfo& operator=(DirectoryInfo&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			DirectoryInfo& operator=(DirectoryInfo* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			DirectoryInfo* operator->() {
				return this;
			}



				DirectoryInfo(System::String path) {
				DirectoryInfoD* dd = new DirectoryInfoD();
				dd->path = path;
				this->od = dd;
			}




			//PropGenGet<bool, DirectoryInfo, &DirectoryInfo::mExists> Exists = [&] {
			//	PropGenGet<bool, DirectoryInfo, &DirectoryInfo::mExists> ret(this);
			//	return ret;
			//}();

			PropGenGet<System::String, DirectoryInfo, &DirectoryInfo::mName> Name{ this };

			PropGenGet<System::String, DirectoryInfo, &DirectoryInfo::mFullName> FullName{ this };

			System::Collections::Generic::Array<FileInfo> GetFiles() {
				return GetFilesOrDirs<FileInfo>(false);
			}

			System::Collections::Generic::Array<DirectoryInfo> GetDirectories() {
				return GetFilesOrDirs<DirectoryInfo>(true);
			}

		};




		namespace Ports {
			enum class Parity {
				None = 0,
				Odd = 1,
				Even = 2,
				Mark = 3,
				Space = 4
			};

			enum class StopBits {
				None = 0,
				One = 1,
				Two = 2,
				OnePointFive = 3
			};

			enum class Handshake {
				None = 0,
				XOnXOff = 1,
				RequestToSend = 2,
				RequestToSendXOnXOff = 3
			};

#if _MSC_VER
			class System_API SerialStreamWinD : public Object::ObjectData {
			private:
			public:
				DCB dcb{};
				COMMTIMEOUTS commTimeouts{};
				COMSTAT comStat{};
				COMMPROP commProp{};

				bool DtrEnable = false;
				bool rtsEnable = false;
				bool RtsEnable = false;

				HANDLE h = nullptr;

				SerialStreamWinD() {}
			};

			class System_API SerialStreamWin : public Stream {
			private:
				//	byte parityReplace = '?';
			public:
				SerialStreamWin(const std::nullptr_t& n) {
					this->od = null;
				}

				SerialStreamWin(SerialStreamWin* pValue) {
					od = pValue->od;
					if (!od) { // 'default explicit' construction
						SerialStreamWinD* dd = new SerialStreamWinD();
						this->od = dd;
					}
					MPool.Put((byte*)pValue);
				}

				SerialStreamWin(SerialStreamWin& other) : Stream(other) {}

				SerialStreamWin(String portName, int baudRate, Parity parity, int dataBits, StopBits stopBits, int readTimeout, int writeTimeout, Handshake handshake,
					bool dtrEnable, bool rtsEnable, bool discardNull, byte parityReplace) {

					SerialStreamWinD* dd = new SerialStreamWinD();
					this->od = dd;

					dd->h = CreateFileW((wchar_t*)("\\\\.\\" + portName).ToCharArray(),
						GENERIC_READ | GENERIC_WRITE,
						0, // comm devices must be opened w/exclusive-access
						NULL, // no security attributes
						OPEN_EXISTING, // comm devices must use OPEN_EXISTING
						FILE_ATTRIBUTE_NORMAL, //FILE_FLAG_OVERLAPPED, // TODO: Async
						NULL); // hTemplate must be NULL for comm devices
					if (dd->h == INVALID_HANDLE_VALUE)
						throw Exception("Invalid handle");

					// COMMPROP can be used to get properties of the comport, like max baudrate
					COMMPROP commProp{};
					DWORD pinstatus = 0;
					if (!GetCommProperties(dd->h, &commProp) || !GetCommModemStatus(dd->h, &pinstatus)) {
						throw Exception("failed to get com properties");
					}
					if (commProp.dwMaxBaud != 0 && baudRate > (int)commProp.dwMaxBaud)
						throw Exception("invalid baudrate");


					// first get the current dcb structure setup
					GetCommState(dd->h, &dd->dcb);
					dd->dcb.DCBlength = sizeof(dd->dcb);
					dd->dcb.BaudRate = (DWORD)baudRate;
					dd->dcb.ByteSize = (byte)dataBits;
					switch (stopBits) {
					case StopBits::One:
						dd->dcb.StopBits = ONESTOPBIT;
						break;
					case StopBits::OnePointFive:
						dd->dcb.StopBits = ONE5STOPBITS;
						break;
					case StopBits::Two:
						dd->dcb.StopBits = TWOSTOPBITS;
						break;
					default:
						throw Exception("Invalid value for stopBits");
					}
					dd->dcb.Parity = (byte)parity;
					dd->dcb.fParity = parity == Parity::None ? 0 : 1;
					dd->dcb.fBinary = 1;
					dd->dcb.fOutxCtsFlow = (handshake == Handshake::RequestToSend || handshake == Handshake::RequestToSendXOnXOff) ? 1 : 0;
					dd->dcb.fOutxDsrFlow = 0;
					dd->dcb.fDtrControl = DTR_CONTROL_DISABLE;
					dd->dcb.fDsrSensitivity = 0;
					dd->dcb.fInX = (handshake == Handshake::XOnXOff || handshake == Handshake::RequestToSendXOnXOff) ? 1 : 0;
					dd->dcb.fOutX = (handshake == Handshake::XOnXOff || handshake == Handshake::RequestToSendXOnXOff) ? 1 : 0;

					if (parity != Parity::None) {
						dd->dcb.fErrorChar = parityReplace != '\0' ? 1 : 0;
						dd->dcb.ErrorChar = parityReplace;
					}
					else {
						dd->dcb.fErrorChar = 0; \
							dd->dcb.ErrorChar = '\0';
					}
					dd->dcb.fNull = discardNull ? 1 : 0;

					if (handshake == Handshake::RequestToSend || handshake == Handshake::RequestToSendXOnXOff) {
						dd->dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
					}
					else if (dd->dcb.fRtsControl == RTS_CONTROL_HANDSHAKE) {
						dd->dcb.fRtsControl = RTS_CONTROL_DISABLE;
					}

					dd->dcb.XonChar = (byte)17;
					dd->dcb.XoffChar = (byte)19;

					dd->dcb.XonLim = dd->dcb.XoffLim = (ushort)(commProp.dwCurrentRxQueue >> 2);

					dd->dcb.EofChar = (byte)26;

					//OLD MSCOMM: dcb.EvtChar = (byte) 0;
					// now changed to make use of RXFlag WaitCommEvent event => Eof WaitForCommEvent event
					dd->dcb.EvtChar = (byte)26;
					if (!SetCommState(dd->h, &dd->dcb))
						throw Exception("failed to set dcb");

					dd->DtrEnable = dtrEnable;

					// query and cache the initial RtsEnable value 
					// so that set_RtsEnable can do the (value != rtsEnable) optimization
					dd->rtsEnable = (dd->dcb.fRtsControl == RTS_CONTROL_ENABLE);

					// now set dd->RtsEnable to the specified value.
					// Handshake takes precedence, this will be a nop if 
					// handshake is either RequestToSend or RequestToSendXOnXOff 
					if ((handshake != Handshake::RequestToSend && handshake != Handshake::RequestToSendXOnXOff))
						dd->RtsEnable = rtsEnable;

					if (readTimeout == 0) {
						dd->commTimeouts.ReadTotalTimeoutConstant = 0;
						dd->commTimeouts.ReadTotalTimeoutMultiplier = 0;
						dd->commTimeouts.ReadIntervalTimeout = MAXDWORD;
					}
					else if (readTimeout == -1) {
						// SetCommTimeouts doesn't like a value of -1 for some reason, so
						// we'll use -2(infiniteTimeoutConst) to represent infinite. 
						dd->commTimeouts.ReadTotalTimeoutConstant = -2;
						dd->commTimeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
						dd->commTimeouts.ReadIntervalTimeout = MAXDWORD;
					}
					else {
						dd->commTimeouts.ReadTotalTimeoutConstant = readTimeout;
						dd->commTimeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
						dd->commTimeouts.ReadIntervalTimeout = MAXDWORD;
					}

					dd->commTimeouts.WriteTotalTimeoutMultiplier = 0;
					dd->commTimeouts.WriteTotalTimeoutConstant = (WriteTimeout == -1) ? 0 : WriteTimeout;

					if (!SetCommTimeouts(dd->h, &dd->commTimeouts)) {
						throw Exception("failed to set commtimeouts");
					}

					// TODO:
					//if (isAsync) {
					//	if (!ThreadPool.BindHandle(_handle))
					//	{
					//		throw new IOException(SR.GetString(SR.IO_BindHandleFailed));
					//	}
					//}
					//// monitor all events except TXEMPTY
					//UnsafeNativeMethods.SetCommMask(_handle, NativeMethods.ALL_EVENTS);

					//// prep. for starting event cycle.
					//eventRunner = new EventLoopRunner(this);
					//Thread eventLoopThread = LocalAppContextSwitches.DoNotCatchSerialStreamThreadExceptions
					//	? new Thread(new ThreadStart(eventRunner.WaitForCommEvent))
					//	: new Thread(new ThreadStart(eventRunner.SafelyWaitForCommEvent));

					//eventLoopThread.IsBackground = true;
					//eventLoopThread.Start();
				}

				virtual void Write(const byte* buffer, int offset, int count) {
					SerialStreamWinD* dd = (SerialStreamWinD*)this->od;

					DWORD numwritten = 0;
					bool succes = WriteFile(dd->h, buffer + offset, (DWORD)count, &numwritten, null);
					if (!succes) {
						//DWORD err = GetLastError();

						throw Exception();
					}

					if (numwritten != (DWORD)count) {
						throw TimeoutException();
					}

				}

				virtual int Read(byte* buffer, int offset, int count) {
					SerialStreamWinD* dd = (SerialStreamWinD*)this->od;
					DWORD cnt = 0;
					bool succes = ReadFile(dd->h, buffer + offset, (DWORD)count, &cnt, null);
					if (!succes) {
						//	DWORD err = GetLastError();
						throw Exception();
					}
					if (cnt == 0) {
						throw TimeoutException();
					}

					return (int)cnt;
				}



			};

			using SerialStream = SerialStreamWin;
#endif

		}
	}
}

namespace Microsoft {
#if _MSC_VER
	namespace Win32 {




		class System_API RegistryKey : public System::Object {
		private:
			REGSAM GetRegistryKeyAccess(bool isWritable) {
				REGSAM ret = ((STANDARD_RIGHTS_READ | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_NOTIFY) & ~SYNCHRONIZE);
				if (isWritable)
					ret |= ((STANDARD_RIGHTS_WRITE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY) & ~SYNCHRONIZE);

				return ret;
			}

			const int MaxKeyLength = 255;
			const int MaxValueLength = 16383;


		public:
			class System_API ObjectData : public System::Object::ObjectData {
			public:
				HKEY key = nullptr;
				bool writable = false;

				int InternalValueCount() {

					DWORD values = 0;
					DWORD junk = 0;
					int ret = RegQueryInfoKey(key, 0, 0, 0, &junk, 0, 0, &values, 0, 0, 0, 0);

					if (ret != 0)
					{
						throw System::Exception("RegQueryInfoKey");
					}
					return (int)values;
				}


			};


						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			RegistryKey(){}

			RegistryKey(std::nullptr_t const & n) : System::Object(n) {
			}

			RegistryKey(RegistryKey* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			RegistryKey(RegistryKey const & other) : System::Object(other) { }

			RegistryKey(RegistryKey&& other) noexcept : System::Object(std::move(other)) { }

			RegistryKey(Object::ObjectData* other) : System::Object(other) {
			}

			RegistryKey& operator=(RegistryKey const & other) {
				System::Object::operator=(other);
				return *this;
			}

			RegistryKey& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			RegistryKey& operator=(RegistryKey&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			RegistryKey& operator=(RegistryKey* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			RegistryKey* operator->() {
				return this;
			}



				RegistryKey(HKEY key, bool writable) {
				ObjectData* dd = new ObjectData();
				if (!dd)
					throw System::OutOfMemoryException();
				dd->key = key;
				dd->writable = writable;
				this->od = dd;
			}

			static RegistryKey GetBaseKey(HKEY key) {
				RegistryKey ret(key, false);

				return ret;
			}

			RegistryKey OpenSubKey(System::String name, bool writable) {
				ObjectData* dd = GOD();
				HKEY result;
				int ret = RegOpenKeyEx(dd->key, (LPCWSTR)name.ToCharArray(), 0, GetRegistryKeyAccess(writable), &result);
				if (ret == 0 && result != INVALID_HANDLE_VALUE) {
					return RegistryKey(result, writable);
				}

				return null;
			}

			System::Collections::Generic::Array<System::String> GetValueNames() {
				ObjectData* dd = GOD();

				int values = dd->InternalValueCount();
				System::Collections::Generic::Array<System::String> names(values);
				if (values > 0) {
					int namelen = MaxValueLength + 1;
					char16_t* namePtr = new char16_t[namelen];

					for (int i = 0; i < values; i++) {
						namelen = MaxValueLength + 1;
						int ret = RegEnumValueW(dd->key,
							i,
							(LPWSTR)namePtr,
							(LPDWORD)&namelen,
							NULL,
							NULL,
							NULL,
							NULL);

						if (ret != 0) {
							throw System::Exception("RegEnumValueW");
						}
						names[i] = System::String(namePtr);
					}

					delete[] namePtr;
				}


				return names;
			}

			System::String GetValue(System::String name) {
				ObjectData* dd = GOD();
				int type = 0;
				int datasize = 0;
				int ret = RegQueryValueEx(dd->key, (LPCWSTR)name.ToCharArray(), null, (LPDWORD)&type, NULL, (LPDWORD)&datasize);
				if (ret != 0 || datasize & 1)
					throw System::Exception("RegQueryValueEx");
				switch (type) {
				case REG_SZ:
				{
					char16_t* blob = new char16_t[datasize >> 1];

					ret = RegQueryValueEx(dd->key, (LPCWSTR)name.ToCharArray(), null, (LPDWORD)&type, (LPBYTE)blob, (LPDWORD)&datasize);
					if (datasize > 0 && blob[(datasize >> 1) - 1] == (char)0) {
						System::String data = System::String(blob);
						delete[] blob;
						return data;
					}
					else {
						// in the very unlikely case the data is missing null termination, 
						// pass in the whole char[] to prevent truncating a character
						//data = new String(blob);
						delete[] blob;
						throw System::Exception("RegQueryEx string missing null termination");
					}
					break;
				}
				}



				return "";

			}


		};

	}

#endif
}


namespace System {
	namespace IO {
		namespace Ports {



			class System_API SerialPort : public Object {
			private:
			public:
				class System_API ObjectData : public Object::ObjectData {

				};


							ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			SerialPort(){}

			SerialPort(std::nullptr_t const & n) : System::Object(n) {
			}

			SerialPort(SerialPort* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			SerialPort(SerialPort const & other) : System::Object(other) { }

			SerialPort(SerialPort&& other) noexcept : System::Object(std::move(other)) { }

			SerialPort(Object::ObjectData* other) : System::Object(other) {
			}

			SerialPort& operator=(SerialPort const & other) {
				System::Object::operator=(other);
				return *this;
			}

			SerialPort& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			SerialPort& operator=(SerialPort&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			SerialPort& operator=(SerialPort* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			SerialPort* operator->() {
				return this;
			}



					static System::Collections::Generic::Array<String> GetPortNames() {
					System::Collections::Generic::Array<String> portNames = null;
#if _MSC_VER
					Microsoft::Win32::RegistryKey basekey = Microsoft::Win32::RegistryKey::GetBaseKey(HKEY_LOCAL_MACHINE);
					Microsoft::Win32::RegistryKey serialKey = basekey.OpenSubKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false);

					if (serialKey != null) {
						System::Collections::Generic::Array<String> deviceNames = serialKey.GetValueNames();

						portNames = System::Collections::Generic::Array<String>(deviceNames.Length);
						for (int i = 0; i < deviceNames.Length; i++)
							portNames[i] = (string)serialKey.GetValue(deviceNames[i]);
					}
					if (portNames == null)
						portNames = System::Collections::Generic::Array<String>(0);

					return portNames;
#else


					return null;
#endif

				}

			};



		}
	}

	namespace Net {
		namespace Sockets {
			enum class AddressFamily {
				AppleTalk = 16,
				Atm = 22,
				Banyan = 21,
				Ccitt = 10,
				Chaos = 5,
				Cluster = 24,
				ControllerAreaNetwork = 65537,
				DataKit = 9,
				DecNet = 12,
				Ecma = 8,
				FireFox = 19,
				HyperChannel = 15,
				Ieee12844 = 25,
				ImpLink = 3,
				InterNetwork = 2,
				InterNetworkV6 = 23,
				Ipx = 6,
				Irda = 26,
				Iso = 7,
				Lat = 14,
				Max = 29,
				NetBios = 17,
				NetworkDesigners = 28,
				NS = 6,
				Osi = 7,
				Packet = 65536,
				Pup = 4,
				Sna = 11,
				Unix = 1,
				Unknown = -1,
				Unspecified = 0,
				VoiceView = 18
			};
		}

		class System_API IPAddress : public Object {
		private:
			static IPAddress InternalParse(System::String ipString, bool tryParse) {
				if (ipString == null) {
					if (tryParse) {
						return null;
					}
					throw ArgumentNullException();
				}

				if (ipString.IndexOf(':') != -1) {
					// IPV6
					throw NotImplementedException();
				}
				else {
					int end = ipString.Length;
					long result;
					char16_t* name = ipString.ToCharArray();
					result = IPv4AddressHelper::ParseNonCanonical(name, 0, ref(end), true);
					if (result == IPv4AddressHelper::Invalid || end != ipString.Length) {
						if (tryParse)
							return null;
						throw FormatException();
					}

					// IPv4AddressHelper always returns IP address in a format that we need to reverse.
					result = (((result & 0x000000FF) << 24) | (((result & 0x0000FF00) << 8)
						| (((result & 0x00FF0000) >> 8) | ((result & 0xFF000000) >> 24))));

					return IPAddress(result);
				}
			}

			Sockets::AddressFamily Get_AddressFamily() const {
				return GOD()->Get_AddressFamily();
			}

			long& Get_ScopeId() const { return GOD()->Get_ScopeId(); }

			void Set_ScopeId(long const & value) {
				GOD()->Set_ScopeId(value);
			}

			void Set_ScopeId(long&& value) {
				GOD()->Set_ScopeId(std::move(value));
			}

			long& Get_Address() const { return GOD()->Get_Address(); }

			void Set_Address(long const & value) {
				GOD()->Set_Address(value);
			}

			void Set_Address(long&& value) {
				GOD()->Set_Address(std::move(value));
			}

		public:
			static IPAddress Any;
			static IPAddress Loopback;
			static IPAddress Broadcast;

			class System_API ObjectData : public Object::ObjectData {
			private:
				static int const IPv4AddressBytes = 4;
				static int const IPv6AddressBytes = 16;
				static int const NumberOfLabels = IPv6AddressBytes / 2;
				Sockets::AddressFamily m_Family = Sockets::AddressFamily::InterNetwork;
				mutable long m_ScopeId = 0;
				mutable long m_Address = 0;
				ushort m_Numbers[NumberOfLabels];

			public:




				ObjectData() {
				}

				ObjectData(long newAddress) {
					m_Address = newAddress;
				}

				Sockets::AddressFamily Get_AddressFamily() const {
					return m_Family;
				}

				long& Get_ScopeId() const { return m_ScopeId; }

				void Set_ScopeId(long const & value) {
					m_ScopeId = value;
				}

				void Set_ScopeId(long&& value) {
					m_ScopeId = std::move(value);
				}

				long& Get_Address() const { return m_Address; }

				void Set_Address(long const & value) {
					m_Address = value;
				}

				void Set_Address(long&& value) {
					m_Address = std::move(value);
				}


				System::Collections::Generic::Array<byte> GetAddressBytes() const {
					if (m_Family == Sockets::AddressFamily::InterNetworkV6) {
						System::Collections::Generic::Array<byte> bytes(NumberOfLabels * 2);

						int j = 0;
						for (int i = 0; i < NumberOfLabels; i++) {
							bytes[j++] = (byte)((m_Numbers[i] >> 8) & 0xFF);
							bytes[j++] = (byte)((m_Numbers[i]) & 0xFF);
						}

						return bytes;
					}
					else {
						System::Collections::Generic::Array<byte> bytes(IPv4AddressBytes);
						bytes[0] = (byte)(m_Address);
						bytes[1] = (byte)(m_Address >> 8);
						bytes[2] = (byte)(m_Address >> 16);
						bytes[3] = (byte)(m_Address >> 24);

						return bytes;
					}
				}

				String ToString() const override {
					return string(m_Address & 0xFF) + "." + string((m_Address >> 8) & 0xFF) + "." + string((m_Address >> 16) & 0xFF) + "." + string((m_Address >> 24) & 0xFF);
				}

			};

			PropGen<long, long, IPAddress, &IPAddress::Get_ScopeId, &IPAddress::Set_ScopeId, &IPAddress::Set_ScopeId> ScopeId{ this };
			PropGen<long, long, IPAddress, &IPAddress::Get_Address, &IPAddress::Set_Address, &IPAddress::Set_Address> Address{ this };
			PropGenGet<Sockets::AddressFamily, IPAddress, &IPAddress::Get_AddressFamily> AddressFamily{ this };

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			IPAddress(){}

			IPAddress(std::nullptr_t const & n) : System::Object(n) {
			}

			IPAddress(IPAddress* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			IPAddress(IPAddress const & other) : System::Object(other) { }

			IPAddress(IPAddress&& other) noexcept : System::Object(std::move(other)) { }

			IPAddress(Object::ObjectData* other) : System::Object(other) {
			}

			IPAddress& operator=(IPAddress const & other) {
				System::Object::operator=(other);
				return *this;
			}

			IPAddress& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			IPAddress& operator=(IPAddress&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			IPAddress& operator=(IPAddress* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			IPAddress* operator->() {
				return this;
			}



				IPAddress(long newAddress) {
				//Console::WriteLine("IPAddress constructor");
				if (newAddress < 0 || newAddress > 0x00000000FFFFFFFF)
					throw ArgumentOutOfRangeException();
				//Console::WriteLine("IPAddress constructor2");
				ObjectData* dd = new ObjectData(newAddress);
				this->od = dd;
			}

			static bool TryParse(const System::String& ipString, Out<IPAddress> address) {
				address = InternalParse(ipString, true);
				return *address != null;
			}

			static IPAddress Parse(const System::String& ipString) {
				return InternalParse(ipString, false);
			}


			System::Collections::Generic::Array<byte> GetAddressBytes() const {
				return GOD()->GetAddressBytes();
			}

		};

#ifndef SYSTEM_EXPORTS
		IPAddress IPAddress::Any((long)0x0000000000000000);
		IPAddress IPAddress::Loopback(0x000000000100007F);
		IPAddress IPAddress::Broadcast(0x00000000FFFFFFFF);
#endif

		class System_API SocketAddress : public Object {
		private:
		public:
			static const int IPv6AddressSize = 28;
			static const int IPv4AddressSize = 16;

			class System_API ObjectData : public Object::ObjectData {
			private:
			public:

				int m_Size = 0;
				System::Collections::Generic::Array<byte> m_Buffer = nullptr;

				ObjectData() {

				}

				ObjectData(Sockets::AddressFamily const& family, int size) : m_Size(size), m_Buffer((size / ptrsize + 2) << ptrshifter) {
					int ifam = (int)family;
					m_Buffer[0] = (byte)ifam;
					m_Buffer[1] = (byte)(ifam >> 8);
				}

				ObjectData(IPAddress const& ipAddress) : ObjectData(ipAddress.AddressFamily, ipAddress.AddressFamily == Sockets::AddressFamily::InterNetwork ? IPv4AddressSize : IPv6AddressSize) {
					// No Port
					m_Buffer[2] = (byte)0;
					m_Buffer[3] = (byte)0;

					if (ipAddress.AddressFamily == Sockets::AddressFamily::InterNetworkV6) {
						// No handling for Flow Information
						m_Buffer[4] = (byte)0;
						m_Buffer[5] = (byte)0;
						m_Buffer[6] = (byte)0;
						m_Buffer[7] = (byte)0;

						// Scope serialization
						long scope = ipAddress.ScopeId;
						m_Buffer[24] = (byte)scope;
						m_Buffer[25] = (byte)(scope >> 8);
						m_Buffer[26] = (byte)(scope >> 16);
						m_Buffer[27] = (byte)(scope >> 24);

						// Address serialization
						System::Collections::Generic::Array<byte> addressBytes = ipAddress.GetAddressBytes();
						for (int i = 0; i < addressBytes.Length; i++) {
							m_Buffer[8 + i] = addressBytes[i];
						}
					}
					else {
						// IPv4 Address serialization
						long addr = ipAddress.Address;
						m_Buffer[4] = (byte)(addr);
						m_Buffer[5] = (byte)(addr >> 8);
						m_Buffer[6] = (byte)(addr >> 16);
						m_Buffer[7] = (byte)(addr >> 24);
					}
				}

				ObjectData(const IPAddress& ipAddress, int port) : ObjectData(ipAddress) {
					m_Buffer[2] = (byte)(port >> 8);
					m_Buffer[3] = (byte)port;
				}

				byte* GetRawBuffer() const {
					return (byte*)&m_Buffer[0];

				}



			};

						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			SocketAddress(){}

			SocketAddress(std::nullptr_t const & n) : System::Object(n) {
			}

			SocketAddress(SocketAddress* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			SocketAddress(SocketAddress const & other) : System::Object(other) { }

			SocketAddress(SocketAddress&& other) noexcept : System::Object(std::move(other)) { }

			SocketAddress(Object::ObjectData* other) : System::Object(other) {
			}

			SocketAddress& operator=(SocketAddress const & other) {
				System::Object::operator=(other);
				return *this;
			}

			SocketAddress& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			SocketAddress& operator=(SocketAddress&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			SocketAddress& operator=(SocketAddress* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			SocketAddress* operator->() {
				return this;
			}



				SocketAddress(const IPAddress& ipaddress, int port) {
				od = new ObjectData(ipaddress, port);
			}

			byte* GetRawBuffer() const {
				return GOD()->GetRawBuffer();
			}

			int GetSize() const {
				return GOD()->m_Size;
			}

		};

		class System_API EndPoint : public Object {
		private:
			Sockets::AddressFamily Get_AddressFamily() const {
				return GOD()->Get_AddressFamily();
			}
		public:
			class System_API ObjectData {
			public:
				virtual Sockets::AddressFamily Get_AddressFamily() const {
					throw NotImplementedException();
				}

				virtual SocketAddress Serialize() const {
					throw NotImplementedException();
				}

				virtual EndPoint Create(const SocketAddress& socketAddress) {
					throw NotImplementedException();
				}
			};

			PropGenGet<Sockets::AddressFamily, EndPoint, &EndPoint::Get_AddressFamily> AddressFamily{ this };

						ObjectData* id = nullptr;

			ObjectData* GOD() const { return this->id; };

			EndPoint(){}

			EndPoint(std::nullptr_t const & n) : System::Object (nullptr), id(nullptr) {
			}

			EndPoint(EndPoint const & other) : System::Object(other) { this->id = other.id; }

			EndPoint(EndPoint&& other) noexcept : System::Object(std::move(other)) { this->id = other.id; other.id = nullptr;  }

			EndPoint(System::Object::ObjectData* baseod, ObjectData* other) : System::Object(baseod), id(other) {
			}

			EndPoint& operator=(EndPoint const & other) {
				System::Object::operator=(other);
				id = other.id;
				return *this;
			}

			EndPoint& operator=(EndPoint&& other) noexcept {
				System::Object::operator=(std::move(other));
				id = other.id;
				return *this;
			}

			EndPoint* operator->() {
				return this;
			}



				SocketAddress Serialize() const {
				return GOD()->Serialize();
			}

			EndPoint Create(const SocketAddress& socketAddress) {
				return GOD()->Create(socketAddress);
			}
		};

		class System_API IPEndPoint : public Object {
		private:
			Sockets::AddressFamily Get_AddressFamily() const {
				return GOD()->Get_AddressFamily();
			}



			IPAddress& Get_Address() const { return GOD()->Get_Address(); }

			void Set_Address(const IPAddress& value) {
				GOD()->Set_Address(value);
			}

			void Set_Address(IPAddress&& value) {
				GOD()->Set_Address(std::move(value));
			}


		public:
			class System_API ObjectData : public Object::ObjectData, public EndPoint::ObjectData {
			public:
				mutable IPAddress Address = nullptr;
				int Port = 0;

				ObjectData() {}

				ObjectData(IPAddress const & address, int port) : Address(address), Port(port) {

				}

				Sockets::AddressFamily Get_AddressFamily() const override {
					return Address.AddressFamily;
				}


				IPAddress& Get_Address() const { return Address; }

				void Set_Address(const IPAddress& value) {
					Address = value;
				}

				void Set_Address(IPAddress&& value) {
					Address = std::move(value);
				}

				SocketAddress Serialize() const override {
					return SocketAddress(Address, Port);
				}

				EndPoint Create(SocketAddress const & socketAddress) override {
					throw NotImplementedException();
				}

				String ToString() const override {
					return Address.ToString() + ":" + string(Port);
				}

			};

			PropGenGet<Sockets::AddressFamily, IPEndPoint, &IPEndPoint::Get_AddressFamily> AddressFamily{ this };
			PropGen<IPAddress, IPAddress, IPEndPoint, &IPEndPoint::Get_Address, &IPEndPoint::Set_Address, &IPEndPoint::Set_Address> Address{ this };


						ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			IPEndPoint(){}

			IPEndPoint(std::nullptr_t const & n) : System::Object(n) {
			}

			IPEndPoint(IPEndPoint* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			IPEndPoint(IPEndPoint const & other) : System::Object(other) { }

			IPEndPoint(IPEndPoint&& other) noexcept : System::Object(std::move(other)) { }

			IPEndPoint(Object::ObjectData* other) : System::Object(other) {
			}

			IPEndPoint& operator=(IPEndPoint const & other) {
				System::Object::operator=(other);
				return *this;
			}

			IPEndPoint& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			IPEndPoint& operator=(IPEndPoint&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			IPEndPoint& operator=(IPEndPoint* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			IPEndPoint* operator->() {
				return this;
			}



			IPEndPoint(IPAddress const & address, int port) {
				ObjectData* dd = new ObjectData(address, port);
				this->od = dd;
			}

			operator EndPoint() const {
				return EndPoint(GOD(), GOD());
			}

			SocketAddress Serialize() const {
				return GOD()->Serialize();
			}

			EndPoint Create(SocketAddress const & socketAddress) const {
				return GOD()->Create(socketAddress);
			}


		};

		namespace Sockets {

			enum class SocketType {
				Dgram = 2,
				Raw = 3,
				Rdm = 4,
				Seqpacket = 5,
				Stream = 1,
				Unknown = -1
			};

			enum class ProtocolType {
				IP = 0,    // dummy for IP
				IPv6HopByHopOptions = 0,
				Icmp = 1,    // control message protocol
				Igmp = 2,    // group management protocol
				Ggp = 3,    // gateway^2 (deprecated)
				IPv4 = 4,
				Tcp = 6,    // tcp
				Pup = 12,   // pup
				Udp = 17,   // user datagram protocol
				Idp = 22,   // xns idp
				IPv6 = 41,   // IPv4
				IPv6RoutingHeader = 43,   // IPv6RoutingHeader
				IPv6FragmentHeader = 44,   // IPv6FragmentHeader
				IPSecEncapsulatingSecurityPayload = 50,   // IPSecEncapsulatingSecurityPayload
				IPSecAuthenticationHeader = 51,   // IPSecAuthenticationHeader
				IcmpV6 = 58,   // IcmpV6
				IPv6NoNextHeader = 59,   // IPv6NoNextHeader
				IPv6DestinationOptions = 60,   // IPv6DestinationOptions
				ND = 77,   // UNOFFICIAL net disk proto
				Raw = 255,  // raw IP packet
				Unspecified = 0,
				Ipx = 1000,
				Spx = 1256,
				SpxII = 1257,
				Unknown = -1,   // unknown protocol type
			};

			enum class SocketShutdown {
				Receive = 0,
				Send = 1,
				Both = 2
			};

			enum class SocketError {
				Success = 0,
				SocketError = (-1),
				//Socket error codes start at 10000. 64 is not a documented socket error code that WSAGetLastError() can return.However, Windows error code 64 is ERROR_NETNAME_DELETED("The specified network name is no longer available.").
				NetnameDeleted = 64,
				Interrupted = (10000 + 4),      //WSAEINTR
				AccessDenied = (10000 + 13),      //WSAEACCES
				Fault = (10000 + 14),        //WSAEFAULT
				InvalidArgument = (10000 + 22),    //WSAEINVAL
				TooManyOpenSockets = (10000 + 24),  //WSAEMFILE
				WouldBlock = (10000 + 35),   //WSAEWOULDBLOCK
				InProgress = (10000 + 36),  // WSAEINPROGRESS
				AlreadyInProgress = (10000 + 37),  //WSAEALREADY
				NotSocket = (10000 + 38),   //WSAENOTSOCK
				DestinationAddressRequired = (10000 + 39), //WSAEDESTADDRREQ
				MessageSize = (10000 + 40),  //WSAEMSGSIZE
				ProtocolType = (10000 + 41), //WSAEPROTOTYPE
				ProtocolOption = (10000 + 42), //WSAENOPROTOOPT
				ProtocolNotSupported = (10000 + 43), //WSAEPROTONOSUPPORT
				SocketNotSupported = (10000 + 44), //WSAESOCKTNOSUPPORT
				OperationNotSupported = (10000 + 45), //WSAEOPNOTSUPP
				ProtocolFamilyNotSupported = (10000 + 46), //WSAEPFNOSUPPORT
				AddressFamilyNotSupported = (10000 + 47), //WSAEAFNOSUPPORT
				AddressAlreadyInUse = (10000 + 48), // WSAEADDRINUSE
				AddressNotAvailable = (10000 + 49), //WSAEADDRNOTAVAIL
				NetworkDown = (10000 + 50), //WSAENETDOWN
				NetworkUnreachable = (10000 + 51), //WSAENETUNREACH
				NetworkReset = (10000 + 52), //WSAENETRESET
				ConnectionAborted = (10000 + 53), //WSAECONNABORTED
				ConnectionReset = (10000 + 54), //WSAECONNRESET
				NoBufferSpaceAvailable = (10000 + 55), //WSAENOBUFS
				IsConnected = (10000 + 56), //WSAEISCONN
				NotConnected = (10000 + 57), //WSAENOTCONN
				Shutdown = (10000 + 58), //WSAESHUTDOWN
				TimedOut = (10000 + 60), //WSAETIMEDOUT
				ConnectionRefused = (10000 + 61), //WSAECONNREFUSED
				HostDown = (10000 + 64), //WSAEHOSTDOWN
				HostUnreachable = (10000 + 65), //WSAEHOSTUNREACH
				ProcessLimit = (10000 + 67), //WSAEPROCLIM
				SystemNotReady = (10000 + 91), //WSASYSNOTREADY
				VersionNotSupported = (10000 + 92), //WSAVERNOTSUPPORTED
				NotInitialized = (10000 + 93), //WSANOTINITIALISED
				Disconnecting = (10000 + 101), //WSAEDISCON
				TypeNotFound = (10000 + 109), //WSATYPE_NOT_FOUND
				HostNotFound = (10000 + 1001), //WSAHOST_NOT_FOUND
				TryAgain = (10000 + 1002), //WSATRY_AGAIN
				NoRecovery = (10000 + 1003), //WSANO_RECOVERY
				NoData = (10000 + 1004), //WSANO_DATA
				IOPending = 997,
				OperationAborted = 995   // 995, WSA_OPERATION_ABORTED
			};


			class System_API SocketException : public Exception {
			public:
				class System_API ObjectData : public Exception::ObjectData {
				private:
				public:
					SocketError errorCode;

					String ToString() const override {
						String Message;
#if _MSC_VER
						LPSTR messageBuffer = nullptr;

						//Ask Win32 to give us the string version of that message ID.
						//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
						size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL, (DWORD)errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
						Message = String(messageBuffer, size);
						//Free the Win32's string's buffer.
						LocalFree(messageBuffer);
#elif ESP32
						Message = "SocketEXception: " + string((int)errorCode);
#endif

						return Message;
					}
				};
								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				SocketException(){}

				SocketException(std::nullptr_t const & n) : Exception(n) {
				}

				SocketException(SocketException* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				SocketException(SocketException const & other) : Exception(other) { }

				SocketException(SocketException&& other) noexcept : Exception(std::move(other)) { }

				SocketException(Object::ObjectData* other) : Exception(other) {
				}

				SocketException& operator=(SocketException const & other) {
					Exception::operator=(other);
					return *this;
				}

				SocketException& operator=(std::nullptr_t const & n) {
					Exception::operator=(n);
					return *this;
				}

				SocketException& operator=(SocketException&& other) noexcept {
					Exception::operator=(std::move(other));
					return *this;
				}

				SocketException& operator=(SocketException* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				SocketException* operator->() {
					return this;
				}



				SocketException(System::String const& str) : Exception(str) {

				}

				SocketException(SocketError errorCode) : Exception() {
					ObjectData* dd = new ObjectData();
					this->od = dd;
					dd->errorCode = errorCode;
				}

				SocketException(System::String&& str) noexcept : Exception((System::String&&)str) {

				}
			};


			enum class SocketFlags {
				None = 0x0000,

				OutOfBand = 0x0001,

				Peek = 0x0002,

				DontRoute = 0x0004,

				MaxIOVectorLength = 0x0010,

				Truncated = 0x0100,
				ControlDataTruncated = 0x0200,
				Broadcast = 0x0400,
				Multicast = 0x0800,


				Partial = 0x8000,
			};

#if _MSC_VER || __MINGW32__
			struct MYOVERLAPPED {
				WSAOVERLAPPED Overlapped;
				//int extra;
				byte* asyncResult;
			};
#endif

			class System_API OverlappedAsyncResult : public Object {
			private:

			public:

				class System_API ObjectData : public Object::ObjectData, public IAsyncResult::ObjectData {
				private:
					std::atomic<bool> completed;
					bool completedSynchronously = false;
					int errorCode = 0;
				public:
#if _MSC_VER || __MINGW32__

					MYOVERLAPPED Overlapped;
					WSABUF m_SingleBuffer;
#elif ESP32
					int rcvcnt;
					byte* rcvbuffer;
					int remoteport;
					ulong remoteaddress;
#else

#endif
					Object socket;
					Object asyncState;
					Action<IAsyncResult&> asyncCallback;
					SocketAddress m_SocketAddress;
					

					ObjectData() {}

					ObjectData(Object const & socket, Object const & asyncState, Action<IAsyncResult&> const & asyncCallback)
						: socket(socket), asyncState(asyncState), asyncCallback(asyncCallback) {
						completed.store(false);
					}
					
					~ObjectData() override {
						Console::WriteLine("Overlappedasyncresult destructor");
					}

					bool Get_IsCompleted() const override {
						return completed.load(std::memory_order_relaxed);
					}

					bool Get_CompletedSynchronously() const override {
						return completedSynchronously;
					}

					int Get_ErrorCode() { return errorCode; }

					void SetCompleted(bool completedSynchronously) {
						completed.store(true, std::memory_order_relaxed);
						this->completedSynchronously = completedSynchronously;
						OverlappedAsyncResult oar(this);
						IAsyncResult ar(oar);
						asyncCallback(ar);
					}

					void SetCompleted(bool completedSynchronously, int errorCode) {
						this->errorCode = errorCode;
						SetCompleted(completedSynchronously);
					}

					Object Get_AsyncState() const override {
						return null;
					}

					System::Threading::WaitHandle Get_AsyncWaitHandle() const override {
						return null;
					}



					void SetUnmanagedStructures(byte const * buffer, int offset, int size, SocketAddress const & socketAddress) {
						m_SocketAddress = socketAddress;
#if _MSC_VER || __MINGW32__
						m_SingleBuffer.len = size;
						m_SingleBuffer.buf = (CHAR*)(buffer + offset);

						// Make sure the SendOverlapped struct is zeroed out
						SecureZeroMemory((void*)&Overlapped, sizeof(WSAOVERLAPPED));
						
						Overlapped.asyncResult = (byte*)this;
#elif ESP32
						rcvbuffer = (byte*)buffer;
#else

#endif							

					}

					void SetUnmanagedStructures(byte const * buffer, int offset, int size) {
						SetUnmanagedStructures(buffer, offset, size, null);
					}

					byte* GetSocketAddressPtr() {
						return m_SocketAddress.GetRawBuffer();
					}



				};
								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				OverlappedAsyncResult(){}

				OverlappedAsyncResult(std::nullptr_t const & n) : System::Object(n) {
				}

				OverlappedAsyncResult(OverlappedAsyncResult* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				OverlappedAsyncResult(OverlappedAsyncResult const & other) : System::Object(other) { }

				OverlappedAsyncResult(OverlappedAsyncResult&& other) noexcept : System::Object(std::move(other)) { }

				OverlappedAsyncResult(Object::ObjectData* other) : System::Object(other) {
				}

				OverlappedAsyncResult& operator=(OverlappedAsyncResult const & other) {
					System::Object::operator=(other);
					return *this;
				}

				OverlappedAsyncResult& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				OverlappedAsyncResult& operator=(OverlappedAsyncResult&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				OverlappedAsyncResult& operator=(OverlappedAsyncResult* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				OverlappedAsyncResult* operator->() {
					return this;
				}



				OverlappedAsyncResult(Object const & socket, Object const & asyncState, Action<IAsyncResult&> const & asyncCallback) {
					ObjectData* dd = new ObjectData(socket, asyncState, asyncCallback);
					this->od = dd;
				}

				operator IAsyncResult() const {
					IAsyncResult ret(GOD(), GOD());
					return ret;
				}

				void SetUnmanagedStructures(byte const * buffer, int offset, int size, SocketAddress const & socketAddress) {
					GOD()->SetUnmanagedStructures(buffer, offset, size, socketAddress);
				}

				void SetUnmanagedStructures(byte const * buffer, int offset, int size) {
					GOD()->SetUnmanagedStructures(buffer, offset, size);
				}

				byte* GetSocketAddressPtr() {
					return GOD()->GetSocketAddressPtr();
				}

				Action<IAsyncResult&> GetCallback() {
					return GOD()->asyncCallback;
				}

				int GetReceiveCount() const {
#if _MSC_VER || __MINGW32__
					MYOVERLAPPED* mol = &GOD()->Overlapped;
					return (int)mol->Overlapped.InternalHigh;
#elif ESP32
					return GOD()->rcvcnt;
#else
					return 0;
#endif
				}

#if _MSC_VER || __MINGW32__
				WSABUF* GetSingleBuffer() const {
					return (WSABUF*)&GOD()->m_SingleBuffer;
				}

				MYOVERLAPPED* GetOverlapped() const {
					MYOVERLAPPED* ret = &GOD()->Overlapped;
					return ret;
				}
#elif ESP32
				void SetReceiveCount(int cnt) const {
					GOD()->rcvcnt = cnt;
				}
#else


#endif
			};









		}
	}

}



namespace System {
	namespace Net {
		namespace Sockets {

#if _MSC_VER || __MINGW32__ 

			class System_API IOCPThreadPool : public Object
			{
			private:
				static int Get_ThreadCount() {
					return _Instance.GetThreadCount();
				}
			public:
				class System_API ObjectData : public Object::ObjectData {
				private:
				public:
					int NumberOfThreads = 8;
					HANDLE hCompletionPort;

					class IOCPThread
					{
					private:
					public:
						System::Threading::Thread t;
						HANDLE hCompletionPort;

						void Run()
						{
							Console::WriteLine("Started IOCP thread");
							while (true)
							{
								OVERLAPPED* pOverlapped = NULL;
								DWORD dwBytesTransfered = 0;
								DWORD_PTR lpCompletionKey;
								BOOL bReturn = GetQueuedCompletionStatus(
									hCompletionPort,
									&dwBytesTransfered,
									&lpCompletionKey,
									&pOverlapped,
									INFINITE);
								MYOVERLAPPED* pMyOverlapped = (MYOVERLAPPED*)pOverlapped;
								if (bReturn) {
									//			Console::WriteLine("IOCP completed packet");
									OverlappedAsyncResult::ObjectData* oarod = (OverlappedAsyncResult::ObjectData*)pMyOverlapped->asyncResult;
									
									//oarod->asyncCallback(oar);
									Threading::ThreadPool::QueueUserWorkItem([oarod]() {
										OverlappedAsyncResult oar(oarod);
										oar.GOD()->SetCompleted(false);
										oar.Release();
										});
									
								}
								else {
									Console::WriteLine("IO error");
									// If *lpOverlapped is NULL, the function did not dequeue a completion packet from the completion port.In this case, the function does not store information in the variables pointed to by the lpNumberOfBytesand lpCompletionKey parameters, and their values are indeterminate.
									// If *lpOverlapped is not NULL and the function dequeues a completion packet for a failed I / O operation from the completion port, the function stores information about the failed operation in the variables pointed to by lpNumberOfBytes, lpCompletionKey, and lpOverlapped.To get extended error information, call GetLastError.
									if (pOverlapped != NULL) { // IO operation failed
										int errorCode = (int)GetLastError();

										OverlappedAsyncResult::ObjectData* oarod = (OverlappedAsyncResult::ObjectData*)pMyOverlapped->asyncResult;
										oarod->SetCompleted(false, errorCode);
									}
								}
							}
						}
					};

					ObjectData(int NumberOfThreads)
					{
						this->NumberOfThreads = NumberOfThreads;

						hCompletionPort = CreateIoCompletionPort(
							INVALID_HANDLE_VALUE, // _In_     HANDLE    FileHandle,
							NULL, // _In_opt_ HANDLE    ExistingCompletionPort,
							0, // _In_     ULONG_PTR CompletionKey,
							NumberOfThreads	// _In_     DWORD     NumberOfConcurrentThreads
						);

						for (int i = 0; i < NumberOfThreads; i++) {
							IOCPThread* nst = new IOCPThread();
							nst->hCompletionPort = hCompletionPort;
							nst->t = System::Threading::Thread([nst]() {
								nst->Run();
								});
							nst->t.IsBackground = true;
							nst->t.Start();
						}
						Console::WriteLine("Started IOCP");
					}

					ObjectData() : ObjectData(8) {}

					bool BindHandle(HANDLE hndl) {
						bool ret = true;

						CreateIoCompletionPort(
							hndl, // _In_     HANDLE    FileHandle,
							hCompletionPort, // _In_opt_ HANDLE    ExistingCompletionPort,
							0, // _In_     ULONG_PTR CompletionKey,
							NumberOfThreads	// _In_     DWORD     NumberOfConcurrentThreads
						);

						return ret;
					}
				};

							ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

			IOCPThreadPool(){}

			IOCPThreadPool(std::nullptr_t const & n) : System::Object(n) {
			}

			IOCPThreadPool(IOCPThreadPool* pValue) {
				if (!pValue->od) {
					ObjectData* dd = new ObjectData();
					od = dd;
				}
				else {
					od = pValue->od;
					pValue->od = nullptr;
				}
				delete pValue;
			}

			IOCPThreadPool(IOCPThreadPool const & other) : System::Object(other) { }

			IOCPThreadPool(IOCPThreadPool&& other) noexcept : System::Object(std::move(other)) { }

			IOCPThreadPool(Object::ObjectData* other) : System::Object(other) {
			}

			IOCPThreadPool& operator=(IOCPThreadPool const & other) {
				System::Object::operator=(other);
				return *this;
			}

			IOCPThreadPool& operator=(std::nullptr_t const & n) {
				System::Object::operator=(n);
				return *this;
			}

			IOCPThreadPool& operator=(IOCPThreadPool&& other) noexcept {
				System::Object::operator=(std::move(other));
				return *this;
			}

			IOCPThreadPool& operator=(IOCPThreadPool* other) {
				if (od == other->od)
					return *this;
				Release();
				od = other->od;
				::operator delete((void*)other);
				return *this;
			}

			IOCPThreadPool* operator->() {
				return this;
			}



					IOCPThreadPool(int NumberOfThreads)
				{
					ObjectData* dd = new ObjectData(NumberOfThreads);
					this->od = dd;
				}

				static IOCPThreadPool _Instance;
				static PropGenGetStatic<int, &IOCPThreadPool::Get_ThreadCount> ThreadCount;

				int GetThreadCount() {
					return GOD()->NumberOfThreads;
				}

				static bool BindHandle(HANDLE hndl) {
					return _Instance.GOD()->BindHandle(hndl);
				}

			};

#ifndef SYSTEM_EXPORTS
			IOCPThreadPool IOCPThreadPool::_Instance{ 1 };
			PropGenGetStatic<int, &IOCPThreadPool::Get_ThreadCount> IOCPThreadPool::ThreadCount{};
#endif
#endif



			class System_API Socket : public Object {
			private:

			public:
				class System_API ObjectData : public Object::ObjectData {
				private:
					AddressFamily addressFamily = AddressFamily::InterNetwork;
					SocketType socketType = SocketType::Stream;
					ProtocolType protocolType = ProtocolType::Tcp;

					static bool s_SupportsIPv4;
					static bool s_SupportsIPv6;
					static bool s_OSSupportsIPv6;

					bool m_IsConnected = false;
				//	bool m_IsDisconnected = false;

#if _MSC_VER || __MINGW32__
					SOCKET hndl;
#elif ESP32
					//AsyncUDP udp;
					udp_pcb* _pcb = NULL;
					tcp_pcb* pcb = NULL;
#else

#endif

					SocketAddress CallSerializeDnsEndPoint(EndPoint const & remoteEP) const {
						return remoteEP.Serialize();
					}

					SocketAddress CheckCacheRemote(EndPoint const & remoteEP, bool isOverwrite) const {
						//IPEndPoint ipSnapshot = remoteEP.as<IPEndPoint>();

						SocketAddress socketAddress = CallSerializeDnsEndPoint(remoteEP);

						return socketAddress;

					}

					void SetConnected(bool value) {
						m_IsConnected = value;
					}

					bool GetConnected() {
						return m_IsConnected;
					}

#if ESP32

					typedef struct {
						struct tcpip_api_call_data call;
						udp_pcb* pcb;
						const ip_addr_t* addr;
						uint16_t port;
						struct pbuf* pb;
						struct netif* netif;
						err_t err;
					} udp_api_call_t;

					typedef struct {
						struct tcpip_api_call_data call;
						tcp_pcb* pcb;
						int8_t closed_slot;
						int8_t err;
						union {
							struct {
								const char* data;
								size_t size;
								uint8_t apiflags;
							} write;
							size_t received;
							struct {
								ip_addr_t* addr;
								uint16_t port;
								tcp_connected_fn cb;
							} connect;
							struct {
								ip_addr_t* addr;
								uint16_t port;
							} bind;
							uint8_t backlog;
						};
					} tcp_api_call_t;

					static err_t _tcp_connect_api(struct tcpip_api_call_data* api_call_msg) {
						Console::WriteLine("_tcp_connect_api");
						tcp_api_call_t* msg = (tcp_api_call_t*)api_call_msg;
						msg->err = tcp_connect(msg->pcb, msg->connect.addr, msg->connect.port, msg->connect.cb);
						return msg->err;
					}



					static err_t _tcp_write_api(struct tcpip_api_call_data* api_call_msg) {
						tcp_api_call_t* msg = (tcp_api_call_t*)api_call_msg;
						msg->err = ERR_CONN;
						//if (msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
							msg->err = tcp_write(msg->pcb, msg->write.data, msg->write.size, msg->write.apiflags);
						//}
						return msg->err;
					}

					static esp_err_t _tcp_write(tcp_pcb* pcb, const char* data, size_t size, uint8_t apiflags) {
						if (!pcb) {
							return ERR_CONN;
						}
						tcp_api_call_t msg;
						msg.pcb = pcb;
						//msg.closed_slot = closed_slot;
						msg.write.data = data;
						msg.write.size = size;
						msg.write.apiflags = apiflags;
						tcpip_api_call(_tcp_write_api, (struct tcpip_api_call_data*)&msg);
						return msg.err;
					}

					static err_t _tcp_close_api(struct tcpip_api_call_data* api_call_msg) {
						tcp_api_call_t* msg = (tcp_api_call_t*)api_call_msg;
						msg->err = ERR_CONN;
					//	if (msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
							msg->err = tcp_close(msg->pcb);
					//	}
						return msg->err;
					}

					static esp_err_t _tcp_close(tcp_pcb* pcb) { // , int8_t closed_slot) {
						if (!pcb) {
							return ERR_CONN;
						}
						tcp_api_call_t msg;
						msg.pcb = pcb;
						//msg.closed_slot = closed_slot;
						tcpip_api_call(_tcp_close_api, (struct tcpip_api_call_data*)&msg);
						return msg.err;
					}

					static err_t _tcp_abort_api(struct tcpip_api_call_data* api_call_msg) {
						tcp_api_call_t* msg = (tcp_api_call_t*)api_call_msg;
						msg->err = ERR_CONN;
					//	if (msg->closed_slot == -1 || !_closed_slots[msg->closed_slot]) {
							tcp_abort(msg->pcb);
					//	}
						return msg->err;
					}

					static void _tcp_error(void* arg, int8_t err) {
						Console::WriteLine("TCP error : " + string(err));
					}

					static esp_err_t _tcp_abort(tcp_pcb* pcb) {//, int8_t closed_slot) {
						if (!pcb) {
							return ERR_CONN;
						}
						tcp_api_call_t msg;
						msg.pcb = pcb;
					//	msg.closed_slot = closed_slot;
						tcpip_api_call(_tcp_abort_api, (struct tcpip_api_call_data*)&msg);
						return msg.err;
					}

					static int8_t _tcp_connected(void* arg, tcp_pcb* pcb, int8_t err) {
						System::Console::WriteLine("tcp connected!");
						return ERR_OK;
					}

					System::Collections::Concurrent::ConcurrentQueue<pbuf*> rcvqueue = new System::Collections::Concurrent::ConcurrentQueue<pbuf*>{};
					
					std::atomic<OverlappedAsyncResult::ObjectData*> rcvarptr;


					void CompleteOverlappedAsyncResult(OverlappedAsyncResult & asyncResult, pbuf* pb, bool completedSync) {
					//	System::Console::WriteLine("CompleteOverlappedAsyncResult(OverlappedAsyncResult!!! " + string(completedSync) + string(rcvqueue.Count));
						if (completedSync) {
							memcpy(asyncResult.GOD()->rcvbuffer, pb->payload, pb->len);
							asyncResult.SetReceiveCount(pb->len);
						}
						else
						{
							memcpy(asyncResult.GOD()->rcvbuffer, pb->payload, pb->len);
							asyncResult.SetReceiveCount(pb->len);
						}

						pbuf_free(pb);
						asyncResult.GOD()->SetCompleted(completedSync);
						asyncResult.Release();
					}

					void _s_recv(pbuf* pb, ip_addr_t const * addr, uint16_t port) {
						//pbuf_free(pb);
						//return;
						rcvqueue.Enqueue(pb);
						OverlappedAsyncResult::ObjectData* tmprcvar = rcvarptr.load();

							if (tmprcvar && rcvarptr.compare_exchange_weak(tmprcvar, null, std::memory_order_release, std::memory_order_relaxed)) {
								
								pbuf* tmp;
								if (!rcvqueue.TryDequeue(out(tmp)))
									Console::WriteLine("failed to dequeue, shouldn't happen");
								
								//OverlappedAsyncResult oar(tmprcvar);
								tmprcvar->remoteaddress = (long)addr->u_addr.ip4.addr;
								tmprcvar->remoteport = port;
								//std::atomic_thread_fence(std::memory_order_release);
								//oar.Release();
								Threading::ThreadPool::QueueUserWorkItem([this, tmp, tmprcvar]() {
									OverlappedAsyncResult oar(tmprcvar);
									this->CompleteOverlappedAsyncResult(oar, tmp, false);
									});
								//OverlappedAsyncResult oar(tmprcvar);
								//this->CompleteOverlappedAsyncResult(oar, tmp, false);
								
								//pbuf_free(tmp);
							}
						//    else
							 //while (rcvqueue.TryDequeue(out(pb)))
						//		pbuf_free(pb);

						//OverlappedAsyncResult::ObjectData* tmprcvar = rcvar.load();
						//while (tmprcvar) {
						//	if (rcvar.compare_exchange_weak(tmprcvar, null, std::memory_order_release, std::memory_order_relaxed)) {
						//		pbuf* data = pb;
						//		if (rcvqueue.TryDequeue(out(data))) {
						//			rcvqueue.Enqueue(pb);
						//		}
						//		OverlappedAsyncResult ar(tmprcvar);
						//		ar.SetComplete(false);
						//		ar.Release();
						//		return;
						//		//tmprcvar = null;
						//	}
						//}
						//rcvqueue.Enqueue(pb);
						//tmprcvar = rcvar.load();
						//if (tmprcvar) {
						//	if (rcvar.compare_exchange_weak(tmprcvar, null, std::memory_order_release, std::memory_order_relaxed)) {
						//		if (rcvqueue.TryDequeue(out(pb))) {
						//		
						//		}
						//	}
						//}
					}

					static void _udp_recv(void* arg, udp_pcb* pcb, pbuf* pb, const ip_addr_t* addr, uint16_t port)
					{
						Socket::ObjectData* ptrsock = reinterpret_cast<Socket::ObjectData*>(arg);
						//System::Console::WriteLine("RECEIVED SMTH3!!!");
						while (pb != NULL) {
							pbuf* this_pb = pb;
							pb = pb->next;
							this_pb->next = NULL;
							ptrsock->_s_recv(this_pb, addr, port);
						}
					}

					static err_t _udp_connect_api(struct tcpip_api_call_data* api_call_msg) {
						//Console::WriteLine("RECEIVED SMTH0!!!");
						udp_api_call_t* msg = (udp_api_call_t*)api_call_msg;
						msg->err = udp_connect(msg->pcb, msg->addr, msg->port);
						//Console::WriteLine(string((uint)msg->err));
						return ((err_t)-1);//msg->err;
					}

					static err_t _udp_bind_api(struct tcpip_api_call_data* api_call_msg) {
						udp_api_call_t* msg = (udp_api_call_t*)api_call_msg;
						msg->err = udp_bind(msg->pcb, msg->addr, msg->port);
						return msg->err;
					}

					static err_t _udp_sendto_api(struct tcpip_api_call_data* api_call_msg) {
						//Console::WriteLine("RECEIVED SMTH1!!!" );
						udp_api_call_t* msg = (udp_api_call_t*)api_call_msg;
						msg->err = udp_sendto(msg->pcb, msg->pb, msg->addr, msg->port);
						//Console::WriteLine(string((uint)msg->err));
						return msg->err;
					}

					static err_t _udp_disconnect_api(struct tcpip_api_call_data* api_call_msg) {
						udp_api_call_t* msg = (udp_api_call_t*)api_call_msg;
						udp_disconnect(msg->pcb);
						return msg->err;
					}

					//static err_t _udp_sendto_if_api(struct tcpip_api_call_data* api_call_msg) {
					//	Console::WriteLine("RECEIVED SMTH2!!!");
					//	udp_api_call_t* msg = (udp_api_call_t*)api_call_msg;
					//	msg->err = udp_sendto_if(msg->pcb, msg->pb, msg->addr, msg->port, msg->netif);
					//	return ((err_t)-1);//msg->err;
					//}



#endif

					void DoConnect(EndPoint const & endPointSnapshot, SocketAddress const & socketAddress) {
						SocketError errorCode = SocketError::Fault;
#if _MSC_VER || __MINGW32__
						byte const * buf = socketAddress.GetRawBuffer();
						int size = socketAddress.GetSize();
						errorCode = (SocketError)WSAConnect(hndl, (sockaddr const *)buf, size, nullptr, nullptr, nullptr, nullptr);
						if (errorCode != SocketError::Success) {
							errorCode = (SocketError)WSAGetLastError();
						}
#elif ESP32
						byte* buf = socketAddress.GetRawBuffer();
						ip_addr_t addr;
						addr.type = IPADDR_TYPE_V4;
						addr.u_addr.ip4.addr = ((uint)buf[7] << 24) | (buf[6] << 16) | (buf[5] << 8) | buf[4];

						if (this->protocolType == ProtocolType::Tcp) {

							pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
							if (pcb) {
								tcp_arg(pcb, this);
								tcp_err(pcb, &_tcp_error);
								Console::WriteLine("Begin connect TCP");
								tcp_api_call_t msg;
								msg.pcb = pcb;
								msg.connect.addr = &addr;
								msg.connect.port = (buf[2] << 8) | buf[3];
								msg.connect.cb = (tcp_connected_fn)&_tcp_connected;
								tcpip_api_call(_tcp_connect_api, (struct tcpip_api_call_data*)&msg);

								err_t err = msg.err;
								// TODO: translate err_t to SocketError
								errorCode = (SocketError)err;
							}
						}
						else if (this->protocolType == ProtocolType::Udp) {
							if (GetConnected()) {
								errorCode = SocketError::Success;
								Console::WriteLine("already connected");
								return;
							}
							if (!_pcb) {
								_pcb = udp_new();
								if (_pcb) {
									
									udp_recv(_pcb, &_udp_recv, (void*) this);
								}
							}
							if (_pcb) {
								udp_api_call_t msg;
								msg.pcb = _pcb;
								msg.addr = &addr;
								msg.port = (buf[2] << 8) | buf[3];
								tcpip_api_call(_udp_connect_api, (struct tcpip_api_call_data*)&msg);
								err_t err = msg.err;
								// TODO: translate err_t to SocketError
								errorCode = (SocketError)err;
								Console::WriteLine("Connect returned " + string(err));
								//udp_recv(_pcb, &_udp_recv, (void*)this);
							}
						}
						
#else


#endif
						if (errorCode != SocketError::Success) {
							throw SocketException(errorCode);
						}

						SetConnected(errorCode == SocketError::Success);
					}

					void DoBind(EndPoint const& localEP) {
						
						SocketAddress sa = localEP.Serialize();

						SocketError errorCode = SocketError::Fault;
#if _MSC_VER || __MINGW32__
						byte const* buf = sa.GetRawBuffer();
						int size = sa.GetSize();
						errorCode = (SocketError) bind(hndl, (sockaddr const*)buf, size);
						if (errorCode !=  SocketError::Success) {
							errorCode = (SocketError)WSAGetLastError();
						}

#elif ESP32
						
						byte* buf = sa.GetRawBuffer();
						ip_addr_t addr;
						//addr.type = IPADDR_TYPE_V4;
						//addr.u_addr.ip4.addr = ((uint)buf[7] << 24) | (buf[6] << 16) | (buf[5] << 8) | buf[4];

						if (this->protocolType == ProtocolType::Tcp) {

						}
						else if (this->protocolType == ProtocolType::Udp) {
							if (GetConnected()) {
								errorCode = SocketError::Success;
								Console::WriteLine("already connected");
								return;
							}
							if (!_pcb) {
								_pcb = udp_new();
								if (_pcb) {

									udp_recv(_pcb, &_udp_recv, (void*)this);
								}
							}
							if (_pcb) {
								udp_api_call_t msg;
								msg.pcb = _pcb;
								msg.addr = IP_ANY_TYPE;
								msg.port = (buf[2] << 8) | buf[3];
								tcpip_api_call(_udp_bind_api, (struct tcpip_api_call_data*)&msg);
								err_t err = msg.err;
								// TODO: translate err_t to SocketError
								errorCode = (SocketError)err;
								Console::WriteLine("bind returned " + string(err));
								//udp_recv(_pcb, &_udp_recv, (void*)this);
							}
						}

#else


#endif
						if (errorCode != SocketError::Success) {
							throw SocketException(errorCode);
						}

						SetConnected(errorCode == SocketError::Success);
					}




					static std::atomic<bool> s_Initialized;
					static std::mutex m_mutex;

					static void InitializeSockets() {
						bool inited = s_Initialized.load(std::memory_order_relaxed);
						std::atomic_thread_fence(std::memory_order_acquire);
						if (inited)
							return;
						std::lock_guard<std::mutex> lock(m_mutex);

						//bool expected = false;
						//if (!s_Initialized.compare_exchange_strong(expected, true))
						//	return;
						inited = s_Initialized.load(std::memory_order_relaxed);
						if (inited)
							return;


#if _MSC_VER || __MINGW32__
						WSAData wsaData;
						SocketError errorCode = (SocketError) WSAStartup(0x0202, &wsaData);
						if (errorCode != SocketError::Success) {
							throw SocketException(errorCode);
						}
						bool ipv4 = true;
						bool ipv6 = true;


						SOCKET socketV4 = WSASocket((int)AddressFamily::InterNetwork, (int)SocketType::Dgram, (int)ProtocolType::IP, nullptr, 0, 0);
						if (socketV4 == INVALID_SOCKET) {
							errorCode = (SocketError)WSAGetLastError();
							if (errorCode == SocketError::AddressFamilyNotSupported) {
								ipv4 = false;
							}
						}
						closesocket(socketV4);

						SOCKET socketV6 = WSASocket((int)AddressFamily::InterNetworkV6, (int)SocketType::Dgram, (int)ProtocolType::IP, nullptr, 0, 0);
						if (socketV6 == INVALID_SOCKET) {
							errorCode = (SocketError) WSAGetLastError();
							if (errorCode == SocketError::AddressFamilyNotSupported) {
								ipv6 = false;
							}
						}
						closesocket(socketV6);

						s_SupportsIPv4 = ipv4;
						s_OSSupportsIPv6 = s_SupportsIPv6 = ipv6;

						//System::Threading::Thread t([]() {
						//	int cnt = 0;
						//	while (true) {
						//		Console::WriteLine("blabla");
						//		System::Threading::Thread::Sleep(1000);
						//	}
						//	});
						//t.IsBackground = true;
						//t.Start();

#else





#endif
						std::atomic_thread_fence(std::memory_order_release);
						s_Initialized.store(true);


					}

					SocketError DoBeginSend(byte const* buffer, int offset, int size, SocketFlags socketFlags, OverlappedAsyncResult& asyncResult) {
						SocketError errorCode = SocketError::Fault;
#if _MSC_VER || __MINGW32__
						asyncResult.SetUnmanagedStructures(buffer, offset, size);

						int bytesTransferred;
						errorCode = (SocketError)WSASend(hndl, asyncResult.GetSingleBuffer(), 1, (LPDWORD)&bytesTransferred, (DWORD)socketFlags, (WSAOVERLAPPED*)asyncResult.GetOverlapped(), NULL);
						if (errorCode == SocketError::Success) {
							// completed synchronously!
						}
						else {
							errorCode = (SocketError)WSAGetLastError();
						}

						//if (errorCode != SocketError::Success) {
							// TODO: throw in callback
						//	throw SocketException(errorCode);
						//}
#elif ESP32
						tcp_api_call_t msg;
						msg.pcb = pcb;
						//msg.closed_slot = closed_slot;
						msg.write.data = (char const *)buffer + offset;
						msg.write.size = size;
						msg.write.apiflags = TCP_WRITE_FLAG_COPY;
						tcpip_api_call(_tcp_write_api, (struct tcpip_api_call_data*)&msg);
						err_t err = msg.err;
						// TODO: translate err_t to SocketError
						errorCode = (SocketError)err;
						//Console::WriteLine("SokcetError = " + string(err));

#else

#endif

						return errorCode;
					}

					SocketError DoBeginReceive(byte const* buffer, int offset, int size, SocketFlags socketFlags, OverlappedAsyncResult& asyncResult) {
						SocketError errorCode = SocketError::Fault;
#if _MSC_VER || __MINGW32__
						asyncResult.SetUnmanagedStructures(buffer, offset, size);
						int bytesTransferred;

						asyncResult->AddRef();
						errorCode = (SocketError)WSARecv(hndl, asyncResult.GetSingleBuffer(), 1, (LPDWORD)&bytesTransferred, (LPDWORD)&socketFlags, (WSAOVERLAPPED*)asyncResult.GetOverlapped(), NULL);

						if (errorCode == SocketError::Success) {
							// completed synchronously!
							//asyncResult.GOD()->asyncCallback(asyncResult);
					//		asyncResult->Release();
					//		asyncResult.GOD()->SetCompleted(true);
						}
						else {
							errorCode = (SocketError)WSAGetLastError();
							if (errorCode == SocketError::IOPending) {
								//ulong cnt = asyncResult->AddRef(); // completed asynchronously, AddRef to make sure the OverlappedAsyncResult doesn't get destroyed
								//if (cnt != 2) {
							//		Console::WriteLine("Async cnt = " + ((string)cnt));
								//}
							}
							else {
								asyncResult->Release();
								asyncResult.GOD()->SetCompleted(true, (int)errorCode);
							}
						}

						//if (errorCode != SocketError::Success && errorCode != SocketError::IOPending) {
						//	throw SocketException(errorCode);
						//}
#endif
						return errorCode;
					}

					SocketError DoBeginSendTo(byte const* buffer, int offset, int size, SocketFlags socketFlags, EndPoint const& remoteEP, OverlappedAsyncResult& asyncResult) {
						SocketError errorCode = SocketError::Fault;
						SocketAddress sa = remoteEP.Serialize();
#if _MSC_VER || __MINGW32__
						
						asyncResult.SetUnmanagedStructures(buffer, offset, size, sa);

						int bytesTransferred;
						asyncResult->AddRef();
						errorCode = (SocketError)WSASendTo(hndl,
							asyncResult.GetSingleBuffer(),
							1,
							(LPDWORD)&bytesTransferred,
							(DWORD)socketFlags,
							(sockaddr*)asyncResult.GetSocketAddressPtr(),
							sa.GetSize(),
							(WSAOVERLAPPED*)asyncResult.GetOverlapped(),
							NULL);
						if (errorCode == SocketError::Success) {
							// completed synchronously!
						}
						else {
							errorCode = (SocketError)WSAGetLastError();
							asyncResult->Release();
							asyncResult.GOD()->SetCompleted(true, (int)errorCode);
						}

						//if (errorCode != SocketError::Success) {
						//	throw SocketException(errorCode);
						//}
#elif ESP32
						Console::WriteLine("DoSend");
						if (!_pcb) {
							DoConnect(remoteEP, sa);
						}
						
						byte* buf = sa.GetRawBuffer();
						ip_addr_t addr;
						addr.type = IPADDR_TYPE_V4;
						addr.u_addr.ip4.addr = ((uint)buf[7] << 24) | (buf[6] << 16) | (buf[5] << 8) | buf[4];
						Console::WriteLine("DoSend2");
						pbuf* pbt = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
						byte* dst = reinterpret_cast<byte*>(pbt->payload);
						memcpy(dst, buffer + offset, size);
						udp_api_call_t msg;
						msg.pcb = _pcb;
						msg.addr = &addr;
						msg.port = (buf[2] << 8) | buf[3];
						msg.pb = pbt;
						Console::WriteLine("DoSend3");
						tcpip_api_call(_udp_sendto_api, (struct tcpip_api_call_data*)&msg);
						pbuf_free(pbt);
						err_t err = msg.err;
						// TODO: translate err_t to SocketError
						errorCode = (SocketError)err;
						Console::WriteLine("SokcetError = " + string(err) + "addr = " + string((uint)addr.u_addr.ip4.addr));
						
#endif

						return errorCode;
					}

					SocketError DoBeginReceiveFrom(byte * buffer, int offset, int size, SocketFlags socketFlags, EndPoint const& remoteEP, OverlappedAsyncResult& asyncResult) {
						SocketError errorCode = SocketError::Fault;
#if _MSC_VER || __MINGW32__
						SocketAddress sa = remoteEP.Serialize();
						asyncResult.SetUnmanagedStructures(buffer, offset, size, sa);
						int bytesTransferred;

						//int WSAAPI WSARecvFrom(
						//	[in]      SOCKET                             s,
						//	[in, out] LPWSABUF                           lpBuffers,
						//	[in]      DWORD                              dwBufferCount,
						//	[out]     LPDWORD                            lpNumberOfBytesRecvd,
						//	[in, out] LPDWORD                            lpFlags,
						//	[out]     sockaddr * lpFrom,
						//	[in, out] LPINT                              lpFromlen,
						//	[in]      LPWSAOVERLAPPED                    lpOverlapped,
						//	[in]      LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
						//);

						asyncResult->AddRef();
						int sasize = sa.GetSize();
						errorCode = (SocketError)WSARecvFrom(
							hndl,
							asyncResult.GetSingleBuffer(),
							1,
							(LPDWORD)&bytesTransferred,
							(LPDWORD)&socketFlags,
							(sockaddr*)asyncResult.GetSocketAddressPtr(),
							&sasize,
							(WSAOVERLAPPED*)asyncResult.GetOverlapped(),
							NULL);

						if (errorCode == SocketError::Success) {
							// completed synchronously!
							//asyncResult.GOD()->asyncCallback(asyncResult);
					//		asyncResult->Release();
					//		asyncResult.GOD()->SetCompleted(true);
						}
						else {
							errorCode = (SocketError)WSAGetLastError();
							if (errorCode == SocketError::IOPending) {
								//ulong cnt = asyncResult->AddRef(); // completed asynchronously, AddRef to make sure the OverlappedAsyncResult doesn't get destroyed
								//if (cnt != 2) {
							//		Console::WriteLine("Async cnt = " + ((string)cnt));
								//}
							}
							else {
								asyncResult->Release();
								asyncResult.GOD()->SetCompleted(true, (int)errorCode);
							}
						}

						//if (errorCode != SocketError::Success && errorCode != SocketError::IOPending) {
						//	throw SocketException(errorCode);
						//}
#elif ESP32
						
						asyncResult.AddRef();
						//rcvar = asyncResult;
						OverlappedAsyncResult::ObjectData* tmprcvar = asyncResult.GOD();
						tmprcvar->rcvbuffer = buffer + offset;
						rcvarptr.store(tmprcvar);

						if (!rcvqueue.IsEmpty) {
								if (rcvarptr.compare_exchange_weak(tmprcvar, null, std::memory_order_release, std::memory_order_relaxed)) {
									pbuf* pb;
									if(!rcvqueue.TryDequeue(out(pb)))
										Console::WriteLine("failed to dequeue sync, shouldn't happen");
									CompleteOverlappedAsyncResult(asyncResult, pb, true);
								}
						}
						errorCode = SocketError::Success;

#endif

						return errorCode;
					}


				public:
					ObjectData() {}

					ObjectData(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) {
						this->addressFamily = addressFamily;
						this->socketType = socketType;
						this->protocolType = protocolType;
						InitializeSockets();
#if _MSC_VER || __MINGW32__
						hndl = WSASocket((int)addressFamily, (int)socketType, (int)protocolType, nullptr, 0, WSA_FLAG_OVERLAPPED);
						if (hndl == INVALID_SOCKET) {
							SocketError errorCode = (SocketError) WSAGetLastError();
							throw SocketException(errorCode);
						}
						//DWORD optval;
						//int optlen = sizeof(optval);
						//getsockopt(hndl, SOL_SOCKET, SO_KEEPALIVE, (char*)&optval, &optlen);


						//int optval = 1;
						//if (setsockopt(hndl, SOL_SOCKET, SO_KEEPALIVE, (char*)&optval, sizeof(optval)) == -1)
						//	throw SocketException();

						IOCPThreadPool::BindHandle((HANDLE)hndl);
#elif ESP32
						rcvarptr.store(null);
#endif

					}

					~ObjectData() override {
#if ESP32
						Console::WriteLine("Socket destructor");
						Close();
						if (pcb) {
					//		Console::WriteLine("PCB destructor");
							tcp_arg(pcb, NULL);
							tcp_sent(pcb, NULL);
							tcp_recv(pcb, NULL);
							tcp_err(pcb, NULL);
							tcp_poll(pcb, NULL, 0);
							int err = _tcp_close(pcb);
							if (err != ERR_OK) {
								err = _tcp_abort(pcb);;
							}
							pcb = NULL;
						}

						if (_pcb) {
							//		Console::WriteLine("_PCB destructor");
							udp_recv(_pcb, NULL, NULL);
							udp_remove(_pcb);
							

							pbuf* pb;
							while (rcvqueue.TryDequeue(out(pb))) {
								pbuf_free(pb);
							}
							OverlappedAsyncResult::ObjectData* oarod = rcvarptr.load();
							if (oarod) {
								OverlappedAsyncResult ar(oarod);
								Console::WriteLine("ar.Release()");
								ar.Release();
							}

							_pcb = NULL;
						}

#endif
					}

					void Connect(EndPoint const& remoteEP) {

						DoConnect(remoteEP, CheckCacheRemote(remoteEP, true));
					}

					void Connect(IPAddress const& address, int port) {
						IPEndPoint remoteEP(address, port);
						Connect(remoteEP);
					}

					void Bind(EndPoint const& localEP) {
						DoBind(localEP);
					}

					int Send(byte const* buffer, int offset, int size, SocketFlags socketFlags, Out<SocketError> errorCode) {
						int bytesTransferred = 0;
						errorCode = SocketError::Success;

#if _MSC_VER || __MINGW32__
						if (size == 0)
							bytesTransferred = send(hndl, NULL, 0, (int)socketFlags);
						else
							bytesTransferred = send(hndl, ((const char*)buffer) + offset, size, (int)socketFlags);

						if ((SocketError)bytesTransferred == SocketError::SocketError) {
							errorCode = (SocketError)WSAGetLastError();
						}
#else



#endif
						return bytesTransferred;
					}

					int Send(byte const* buffer, int offset, int size, SocketFlags socketFlags) {
						SocketError errorCode;
						int bytesTransferred = Send(buffer, offset, size, socketFlags, out(errorCode));
						if (errorCode != SocketError::Success) {
							throw SocketException(errorCode);
						}

						return bytesTransferred;
					}

					IAsyncResult BeginSend(byte const* buffer, int offset, int size, SocketFlags socketFlags, Out<SocketError> errorCode, Action<IAsyncResult&> const& callback, Object const& state) {
						OverlappedAsyncResult asyncResult = new OverlappedAsyncResult(Socket(this), state, callback);

						errorCode = DoBeginSend(buffer, offset, size, socketFlags, asyncResult);

						return asyncResult;
					}

					IAsyncResult BeginSend(byte const* buffer, int offset, int size, SocketFlags socketFlags, Action<IAsyncResult&> const& callback, Object const& state) {
						SocketError errorCode;
						IAsyncResult result = BeginSend(buffer, offset, size, socketFlags, out(errorCode), callback, state);
						if (errorCode != SocketError::Success) {
							throw SocketException(errorCode);
						}
						return result;
					}

					IAsyncResult BeginReceive(byte * buffer, int offset, int size, SocketFlags socketFlags, Out<SocketError> errorCode, Action<IAsyncResult&> const& callback, Object const& state) {
						OverlappedAsyncResult asyncResult = new OverlappedAsyncResult(Socket(this), state, callback);
						errorCode = DoBeginReceive(buffer, offset, size, socketFlags, asyncResult);

						return asyncResult;
					}

					IAsyncResult BeginReceive(byte * buffer, int offset, int size, SocketFlags socketFlags, Action<IAsyncResult&> const& callback, Object const& state) {
						SocketError errorCode;
						IAsyncResult result = BeginReceive(buffer, offset, size, socketFlags, out(errorCode), callback, state);
						if (errorCode != SocketError::Success && errorCode != SocketError::IOPending) {
							throw SocketException(errorCode);
						}
						return result;
					}

					int EndReceive(IAsyncResult const& asyncResult, Out<SocketError> errorCode) {
						int ret = 0;
						OverlappedAsyncResult::ObjectData* oarod = static_cast<OverlappedAsyncResult::ObjectData*>(asyncResult.GOD());
						while (!oarod->Get_IsCompleted())
							System::Threading::Thread::Sleep(15);
						errorCode = (SocketError)oarod->Get_ErrorCode();
						if (errorCode == SocketError::Success) {
#if _MSC_VER || __MINGW32__
							OverlappedAsyncResult oar(oarod);
							MYOVERLAPPED* mol = oar.GetOverlapped();
							ret = (int)mol->Overlapped.InternalHigh;
#endif
						}

						return ret;
					}

					IAsyncResult BeginSendTo(byte const* buffer, int offset, int size, SocketFlags socketFlags, EndPoint const& remoteEP, Action<IAsyncResult&> const& callback, Object const& state) {
						OverlappedAsyncResult asyncResult = new OverlappedAsyncResult(Socket(this), state, callback);
						SocketError errorCode = DoBeginSendTo(buffer, offset, size, socketFlags, remoteEP, asyncResult);
						if (errorCode != SocketError::Success) {
							throw SocketException(errorCode);
						}

						return asyncResult;
					}

					IAsyncResult BeginReceiveFrom(byte * buffer, int offset, int size, SocketFlags socketFlags, EndPoint const& remoteEP, Action<IAsyncResult&> const& callback, Object const& state) {
						OverlappedAsyncResult asyncResult = new OverlappedAsyncResult(Socket(this), state, callback);
						SocketError errorCode = DoBeginReceiveFrom(buffer, offset, size, socketFlags, remoteEP, asyncResult);
						if (errorCode != SocketError::Success && errorCode != SocketError::IOPending) {
							throw SocketException(errorCode);
						}

						return asyncResult;
					}

					int EndReceiveFrom(IAsyncResult const& asyncResult, Out<SocketError> errorCode, Out<System::Net::EndPoint> endPoint) {
						int ret = 0;
						//Console::WriteLine("endreceive0");
						OverlappedAsyncResult::ObjectData* oarod = static_cast<OverlappedAsyncResult::ObjectData*>(asyncResult.GOD());
						Console::WriteLine("endreceive1");
						while (!oarod->Get_IsCompleted())
							System::Threading::Thread::Sleep(15);
						Console::WriteLine("endreceive2");
						errorCode = (SocketError)oarod->Get_ErrorCode();
						if (errorCode == SocketError::Success) {
							//Console::WriteLine("endrceive3");
#if _MSC_VER || __MINGW32__
							OverlappedAsyncResult oar(oarod);
							ret = oar.GetReceiveCount();
							//MYOVERLAPPED* mol = oar.GetOverlapped();
							//ret = (int)mol->Overlapped.InternalHigh;
							//int namelen = 0;
							//sockaddr name;
							//errorCode = (SocketError)getpeername(hndl, 
							//	&name, // [out]     sockaddr * name,
							//	&namelen //[in, out] int* namelen
							//);
							//if (errorCode == SocketError::Success) {
								byte* buf = (byte*)oar.GetSocketAddressPtr();
								int remport = (buf[2] << 8) | buf[3];
								long remaddr = ((uint)buf[7] << 24) | (buf[6] << 16) | (buf[5] << 8) | buf[4];
								System::Net::IPAddress ipa(remaddr);
								System::Net::IPEndPoint ipe(ipa, remport);
								endPoint = ipe;
							//}
							//else {
							//	errorCode = (SocketError)WSAGetLastError();
							//}

#elif ESP32
							//try {
								//Console::WriteLine("endrceive3.01");
								long remaddr = (long)oarod->remoteaddress;
								//Console::WriteLine("endrceive3.02");
								long remport = oarod->remoteport;

								System::Net::IPAddress ipa(remaddr);

								//Console::WriteLine("rmoteport = " + string(remport) + ", address = " + ipa.ToString());


								//Console::WriteLine("endreceive3.1");
								System::Net::IPEndPoint ipe(ipa, remport);
								//Console::WriteLine("endreceive4");
								endPoint = ipe;
								//Console::WriteLine("endreceive5");
							//}
							//catch (Exception& ex) {
							//	Console::WriteLine("endreceive3.0");
							//}
								ret = oarod->rcvcnt; // oar.GetReceiveCount();
#endif
						}

						return ret;
					}

					void Shutdown(SocketShutdown how) {
#if _MSC_VER || __MINGW32__
						shutdown(hndl, (int)how);
#endif
					}

					void Close() {
						//Console::WriteLine("socket close");
#if _MSC_VER || __MINGW32__
						if (hndl) {
							closesocket(hndl);
						}
#elif ESP32
						if (GetConnected()) {
							udp_api_call_t msg;
							msg.pcb = _pcb;
							tcpip_api_call(_udp_disconnect_api, (struct tcpip_api_call_data*)&msg);
							pbuf* pb;
							while (rcvqueue.TryDequeue(out(pb))) {
								pbuf_free(pb);
							}
							OverlappedAsyncResult::ObjectData* oarod = rcvarptr.load();
							if (oarod) {
								OverlappedAsyncResult ar(oarod);
								Console::WriteLine("ar.Release()");
								ar.Release();
							}

							SetConnected(false);
						}
#endif


					}



				};


								ObjectData* GOD() const { return static_cast<ObjectData*>(this->od); };

				Socket(){}

				Socket(std::nullptr_t const & n) : System::Object(n) {
				}

				Socket(Socket* pValue) {
					if (!pValue->od) {
						ObjectData* dd = new ObjectData();
						od = dd;
					}
					else {
						od = pValue->od;
						pValue->od = nullptr;
					}
					delete pValue;
				}

				Socket(Socket const & other) : System::Object(other) { }

				Socket(Socket&& other) noexcept : System::Object(std::move(other)) { }

				Socket(Object::ObjectData* other) : System::Object(other) {
				}

				Socket& operator=(Socket const & other) {
					System::Object::operator=(other);
					return *this;
				}

				Socket& operator=(std::nullptr_t const & n) {
					System::Object::operator=(n);
					return *this;
				}

				Socket& operator=(Socket&& other) noexcept {
					System::Object::operator=(std::move(other));
					return *this;
				}

				Socket& operator=(Socket* other) {
					if (od == other->od)
						return *this;
					Release();
					od = other->od;
					::operator delete((void*)other);
					return *this;
				}

				Socket* operator->() {
					return this;
				}




					Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) {
					this->od = new ObjectData(addressFamily, socketType, protocolType);
				}

				Socket(SocketType socketType, ProtocolType protocolType) : Socket(AddressFamily::InterNetworkV6, socketType, protocolType) {
				}

				void Connect(EndPoint const& remoteEP) const {
					GOD()->Connect(remoteEP);
				}

				void Connect(IPAddress const& address, int port) const {
					GOD()->Connect(address, port);
				}

				void Bind(EndPoint const& localEP) const {
					GOD()->Bind(localEP);
				}

				int Send(byte const* buffer, int offset, int size, SocketFlags socketFlags, Out<SocketError> errorCode) const {
					return GOD()->Send(buffer, offset, size, socketFlags, errorCode);
				}

				int Send(byte const* buffer, int offset, int size, SocketFlags socketFlags) const {
					return GOD()->Send(buffer, offset, size, socketFlags);
				}

				int Send(byte const* buffer, int size, SocketFlags socketFlags) const {
					return GOD()->Send(buffer, 0, size, socketFlags);
				}

				int Send(System::Collections::Generic::Array<byte> const& buffer, SocketFlags socketFlags) const {
					return GOD()->Send(buffer, 0, buffer != null ? (int)buffer.Length : 0, socketFlags);
				}

				int Send(System::Collections::Generic::Array<byte> const& buffer) const {
					return GOD()->Send(buffer, 0, buffer != null ? (int)buffer.Length : 0, SocketFlags::None);
				}

				IAsyncResult BeginSend(byte const* buffer, int offset, int size, SocketFlags socketFlags, Action<IAsyncResult&> const& callback, Object const& state) const {
					return GOD()->BeginSend(buffer, offset, size, socketFlags, callback, state);
				}

				IAsyncResult BeginReceive(byte * buffer, int offset, int size, SocketFlags socketFlags, Action<IAsyncResult&> const& callback, Object const& state) const {
					return GOD()->BeginReceive(buffer, offset, size, socketFlags, callback, state);
				}

				int EndReceive(IAsyncResult const& asyncResult, Out<SocketError> errorCode) const {
					return GOD()->EndReceive(asyncResult, errorCode);
				}

				IAsyncResult BeginSendTo(byte const* buffer, int offset, int size, SocketFlags socketFlags, EndPoint const& remoteEP, Action<IAsyncResult&> const& callback, Object const& state) const {
					return GOD()->BeginSendTo(buffer, offset, size, socketFlags, remoteEP, callback, state);
				}

				IAsyncResult BeginReceiveFrom(byte * buffer, int offset, int size, SocketFlags socketFlags, EndPoint const& remoteEP, Action<IAsyncResult&> const& callback, Object const& state) const {
					return GOD()->BeginReceiveFrom(buffer, offset, size, socketFlags, remoteEP, callback, state);
				}

				int EndReceiveFrom(IAsyncResult const& asyncResult, Out<System::Net::EndPoint> endPoint) const {
					SocketError errorCode;
					int ret = GOD()->EndReceiveFrom(asyncResult, out(errorCode), out(endPoint));
					if (errorCode != SocketError::Success) {
						throw SocketException(errorCode);
					}


					return ret;
				}

				void Shutdown(SocketShutdown how) const {
					GOD()->Shutdown(how);
				}

				void Close() const {
					return GOD()->Close();
				}



				


			};

#ifndef SYSTEM_EXPORTS
			std::atomic<bool> Socket::ObjectData::s_Initialized{ false };
			std::mutex Socket::ObjectData::m_mutex{};

			bool Socket::ObjectData::s_SupportsIPv4{ false };
			bool Socket::ObjectData::s_SupportsIPv6{ false };
			bool Socket::ObjectData::s_OSSupportsIPv6{ false };
#endif


		}
	}
}

namespace System{

	void begin() {
#if ESP32
		UseMemoryPool = true;

		System::Threading::SimpleThreadPool::_Instance = new System::Threading::SimpleThreadPool(1);
#endif
	}
}



#endif // include guard