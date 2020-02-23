#include <algorithm>
#include <chrono>
#include <cmath>
#include <codecvt>
#include <cstdarg>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <type_traits>
#include <vector>

template<typename T> size_t bufLen_() noexcept { return 0; }
template<> size_t bufLen_<char16_t>() noexcept { return 1; }

struct Ref_;
struct Class_;
template<typename T> struct Array_;
template<typename T> struct List_;
template<typename T> struct Stack_;
template<typename T> struct Queue_;
template<typename T1, typename T2> struct Dict_;
template<typename T1, typename T2> struct dictImpl_;

struct Ref_ {
	Ref_() noexcept : R(0LL) {}
	bool EqAddr(const Ref_* t) noexcept { return this == t; }
	int64_t R;
};
struct Class_ : public Ref_ {
	Class_() noexcept : Ref_(), Y(0LL) {}
	int64_t Y;
};
template<typename T> struct Array_ : public Ref_ {
	Array_() noexcept : Ref_(), L(), B() {}
	explicit Array_(int64_t n, ...) noexcept : Ref_() {
		L = n;
		B = new T[static_cast<size_t>(n + bufLen_<T>())];
		va_list l;
		va_start(l, n);
		for (int64_t i = 0; i < n; i++)
			B[i] = va_arg(l, T);
		va_end(l);
		if (bufLen_<T>() > 0)
			B[n] = 0;
	}
	Array_<T>* Cat(const Array_<T>* t) noexcept {
		Array_<T>* r = new Array_<T>();
		r->B = new T[static_cast<size_t>(L + t->L + bufLen_<T>())];
		memcpy(r->B, B, sizeof(T) * static_cast<size_t>(L));
		memcpy(r->B + L, t->B, sizeof(T) * static_cast<size_t>(t->L + bufLen_<T>()));
		r->L = L + t->L;
		return r;
	}
	int64_t Len() noexcept { return L; }
	T& At(int64_t n) noexcept { return B[n]; }
	int64_t L;
	T* B;
};
template<>
Array_<char16_t>::Array_(int64_t n, ...) noexcept : Ref_() {
	L = n;
	B = new char16_t[static_cast<size_t>(n + bufLen_<char16_t>())];
	va_list l;
	va_start(l, n);
	for (int64_t i = 0; i < n; i++)
		B[i] = va_arg(l, int);
	va_end(l);
	if (bufLen_<char16_t>() > 0)
		B[n] = 0;
}
template<typename T> struct List_ : public Ref_ {
	List_() noexcept : Ref_(), B(), I(B.end()) {}
	int64_t Len() noexcept { return static_cast<int64_t>(B.size()); }
	std::list<T> B;
	typename std::list<T>::iterator I;
};
template<typename T> struct Stack_ : public Ref_ {
	Stack_() noexcept : Ref_(), B() {}
	int64_t Len() noexcept { return static_cast<int64_t>(B.size()); }
	std::stack<T> B;
};
template<typename T> struct Queue_ : public Ref_ {
	Queue_() noexcept : Ref_(), B() {}
	int64_t Len() noexcept { return static_cast<int64_t>(B.size()); }
	std::queue<T> B;
};
template<typename T1, typename T2> dictImpl_<T1, T2>* dictAdd_(dictImpl_<T1, T2>* r, T1 k, T2 v, bool* a) noexcept;
template<typename T1, typename T2> dictImpl_<T1, T2>* dictCopyRec_(dictImpl_<T1, T2>* n) noexcept;
template<typename T1, typename T2> void dictToBinRec_(Array_<uint8_t>* a, dictImpl_<T1, T2>* d) noexcept;
template<typename T1, typename T2> struct Dict_ : public Ref_ {
	Dict_() noexcept : Ref_(), L(0LL), B(nullptr) {}
	int64_t Len() noexcept { return L; }
	void Add(T1 k, T2 v) noexcept {
		bool a;
		B = dictAdd_<T1, T2>(B, k, v, &a);
		if (a)
			L++;
	}
	int64_t L;
	dictImpl_<T1, T2>* B;
};
template<typename T1, typename T2> struct dictImpl_ {
	dictImpl_(T1 k, T2 v) noexcept : K(k), V(v), CL(nullptr), CR(nullptr), R(true) {}
	T1 K;
	T2 V;
	dictImpl_* CL;
	dictImpl_* CR;
	bool R;
};

#if _MSC_VER >= 1900 && _MSC_VER < 1922
static std::string utf16ToUtf8_(const std::u16string& s) noexcept {
	const int16_t* p = reinterpret_cast<const int16_t*>(s.data());
	return std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t>{}.to_bytes(p, p + s.size());
}
static std::u16string utf8ToUtf16_(const std::string& s) noexcept {
	const auto t = std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t>{}.from_bytes(s);
	return std::u16string(reinterpret_cast<const char16_t*>(t.data()), t.size());
}
#else
static std::string utf16ToUtf8_(const std::u16string& s) noexcept {
	return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(s);
}
static std::u16string utf8ToUtf16_(const std::string& s) noexcept {
	return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(s);
}
#endif

#if defined(_WIN32)

#define STRICT
#define _WIN32_DCOM
#include <Windows.h>

const char newLine_[] = { '\r', '\n' };

static bool setCurDir_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::SetCurrentDirectoryA(t.c_str()) != 0;
}
static bool makeDir_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::CreateDirectoryA(t.c_str(), 0) != 0;
}
static bool delDir_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::RemoveDirectoryA(t.c_str()) != 0;
}
static bool delFile_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::DeleteFileA(t.c_str()) != 0;
}
static bool copyDir_(const char16_t* d, const char16_t* s) noexcept {
	std::u16string s1 = d;
	const std::string& t1 = utf16ToUtf8_(s1);
	std::u16string s2 = s;
	const std::string& t2 = utf16ToUtf8_(s2);
	return ::CreateDirectoryExA(t2.c_str(), t1.c_str(), 0) != 0;
}
static bool copyFile_(const char16_t* d, const char16_t* s) noexcept {
	std::u16string s1 = d;
	const std::string& t1 = utf16ToUtf8_(s1);
	std::u16string s2 = s;
	const std::string& t2 = utf16ToUtf8_(s2);
	return ::CopyFileA(t2.c_str(), t1.c_str(), false) != 0;
}
static bool moveFile_(const char16_t* d, const char16_t* s) noexcept {
	std::u16string s1 = d;
	const std::string& t1 = utf16ToUtf8_(s1);
	std::u16string s2 = s;
	const std::string& t2 = utf16ToUtf8_(s2);
	return ::MoveFileExA(t2.c_str(), t1.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != 0;
}

#else

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

const char newLine_[] = { '\n' };

static bool setCurDir_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::chdir(t.c_str()) == 0;
}
static bool makeDir_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::mkdir(t.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
}
static bool delDir_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::rmdir(t.c_str()) == 0;
}
static bool delFile_(const char16_t* p) noexcept {
	std::u16string s = p;
	const std::string& t = utf16ToUtf8_(s);
	return ::unlink(t.c_str()) == 0;
}
static bool copyDir_(const char16_t* d, const char16_t* s) noexcept {
	std::u16string s1 = d;
	const std::string& t1 = utf16ToUtf8_(s1);
	std::u16string s2 = s;
	const std::string& t2 = utf16ToUtf8_(s2);
	struct stat f;
	return !(::stat(t2.c_str(), &f) != 0 || ::mkdir(t1.c_str(), f.st_mode) != 0);
}
static bool copyFile_(const char16_t* d, const char16_t* s) noexcept {
	std::u16string s1 = d;
	const std::string& t1 = utf16ToUtf8_(s1);
	std::u16string s2 = s;
	const std::string& t2 = utf16ToUtf8_(s2);
	int i = ::open(t2.c_str(), O_RDONLY);
	if (i < 0)
		return false;
	struct stat f;
	if (::stat(t2.c_str(), &f) != 0)
	{
		::close(i);
		return false;
	}
	int o = ::open(t1.c_str(), O_CREAT | O_WRONLY | O_TRUNC, f.st_mode);
	if (o < 0)
	{
		::close(i);
		return false;
	}
	ssize_t r = 1;
	char* buf = new char[65536];
	while (r > 0 && (r = ::read(i, buf, 65536)) > 0)
	{
		ssize_t z, w = 0;
		do
		{
			if ((z = ::write(o, buf + w, r - w)) < 0)
			{
				r = z;
				break;
			}
			w += z;
		} while (w < r);
	}
	if (::close(i) < 0)
		r = -1;
	if (::close(o) < 0)
		r = -1;
	delete[] buf;
	return r >= 0;
}
static bool moveFile_(const char16_t* d, const char16_t* s) noexcept {
	std::u16string s1 = d;
	const std::string& t1 = utf16ToUtf8_(s1);
	std::u16string s2 = s;
	const std::string& t2 = utf16ToUtf8_(s2);
	return ::rename(t2.c_str(), t1.c_str()) == 0;
}

#   define BOOST_COPY_FILE(F,T,FailIfExistsBool)
#   define BOOST_MOVE_FILE(OLD,NEW)()
#endif

template<typename T> void* newArrayRec_(int64_t n, int64_t x, const int64_t* b) noexcept {
	if (x == n - 1)
	{
		Array_<T>* r = new Array_<T>();
		r->L = b[x];
		size_t s = static_cast<size_t>(b[x] + bufLen_<T>());
		r->B = new T[s];
		memset(r->B, 0, sizeof(T) * s);
		return r;
	}
	{
		Array_<void*>* r = new Array_<void*>();
		r->L = b[x];
		r->B = new void*[static_cast<size_t>(b[x])];
		for (int64_t i = 0; i < b[x]; i++)
			r->B[i] = newArrayRec_<T>(n, x + 1, b);
		return r;
	}
}
template<typename T, typename R> R newArray_(int64_t n, ...) noexcept {
	if (n > 64)
		return nullptr;
	int64_t b[64];
	va_list l;
	va_start(l, n);
	for (int64_t i = 0; i < n; i++)
		b[i] = va_arg(l, int64_t);
	va_end(l);
	return static_cast<R>(newArrayRec_<T>(n, 0, b));
}

template<typename T> Array_<T>* toArray_(List_<T>* l) noexcept {
	Array_<T>* a = new Array_<T>();
	a->L = l->Len();
	a->B = new T[static_cast<size_t>(a->L) + bufLen_<T>()];
	int64_t i = 0;
	for (auto& e : l->B)
	{
		a->B[i] = e;
		i++;
	}
	if (bufLen_<T>() > 0)
		a->B[a->L] = 0;
	return a;
}

template<typename T> struct copy_ {};
template<typename T> struct copy_<Array_<T>*> { Array_<T>* operator()(Array_<T>* t) noexcept {
	if (t == nullptr)
		return nullptr;
	Array_<T>* r = new Array_<T>();
	r->L = t->L;
	r->B = new T[static_cast<size_t>(t->L) + bufLen_<T>()];
	for (int64_t i = 0; i < t->L; i++)
		r->B[i] = copy_<T>()(t->B[i]);
	if (bufLen_<T>() > 0)
		r->B[r->L] = 0;
	return r;
}};
template<typename T> struct copy_<List_<T>*> { List_<T>* operator()(List_<T>* t) noexcept {
	if (t == nullptr)
		return nullptr;
	List_<T>* r = new List_<T>();
	for (auto i = t->B.begin(); i != t->B.end(); ++i)
	{
		r->B.push_back(copy_<T>()(*i));
		if (i == t->I)
		{
			auto e = r->B.end();
			--e;
			r->I = e;
		}
	}
	return r;
}};
template<typename T> struct copy_<Stack_<T>*> { Stack_<T>* operator()(Stack_<T>* t) noexcept {
	if (t == nullptr)
		return nullptr;
	Stack_<T>* r = new Stack_<T>();
	std::stack<T> b;
	while (!t->B.empty())
	{
		b.push(t->B.top());
		t->B.pop();
	}
	while (!b.empty())
	{
		r->B.push(copy_<T>()(b.top()));
		t->B.push(b.top());
		b.pop();
	}
	return r;
}};
template<typename T> struct copy_<Queue_<T>*> { Queue_<T>* operator()(Queue_<T>* t) noexcept {
	if (t == nullptr)
		return nullptr;
	Queue_<T>* r = new Queue_<T>();
	std::queue<T> b;
	while (!t->B.empty())
	{
		b.push(t->B.front());
		t->B.pop();
	}
	while (!b.empty())
	{
		r->B.push(copy_<T>()(b.front()));
		t->B.push(b.front());
		b.pop();
	}
	return r;
}};
template<typename T1, typename T2> struct copy_<Dict_<T1, T2>*> { Dict_<T1, T2>* operator()(Dict_<T1, T2>* t) noexcept {
	if (t == nullptr)
		return nullptr;
	Dict_<T1, T2>* r = new Dict_<T1, T2>();
	r->L = t->L;
	r->B = dictCopyRec_<T1, T2>(t->B);
	return r;
}};
template<typename T> struct copy_<T*> { T* operator()(T* t) noexcept {
	if (std::is_class<T>::value)
	{
		if (t == nullptr)
			return nullptr;
		return reinterpret_cast<T*>(reinterpret_cast<Class_*(*)(Class_*)>(classTable_[t->Y + 4])(t));
	}
	else
		return t;
}};
template<> struct copy_ <int64_t> { int64_t operator()(int64_t t) noexcept { return t; } };
template<> struct copy_ <char16_t> { char16_t operator()(char16_t t) noexcept { return t; } };
template<> struct copy_ <bool> { bool operator()(bool t) noexcept { return t; } };
template<> struct copy_ <double> { double operator()(double t) noexcept { return t; } };
template<> struct copy_ <uint8_t> { uint8_t operator()(uint8_t t) noexcept { return t; } };
template<> struct copy_ <uint16_t> { uint16_t operator()(uint16_t t) noexcept { return t; } };
template<> struct copy_ <uint32_t> { uint32_t operator()(uint32_t t) noexcept { return t; } };
template<> struct copy_ <uint64_t> { uint64_t operator()(uint64_t t) noexcept { return t; } };

static Array_<char16_t>* toStr_(int64_t v) noexcept {
	std::stringstream s;
	s << v;
	const std::string& t = s.str();
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(t.size());
	r->B = new char16_t[t.size() + 1];
	int64_t p = 0;
	for (char c : t)
		r->B[p++] = c;
	r->B[t.size()] = 0;
	return r;
}
static Array_<char16_t>* toStr_(char16_t v) noexcept {
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(1);
	r->B = new char16_t[2];
	r->B[0] = v;
	r->B[1] = 0;
	return r;
}
static Array_<char16_t>* toStr_(bool v) noexcept {
	Array_<char16_t>* r = new Array_<char16_t>();
	if (v)
	{
		r->L = static_cast<int64_t>(4);
		r->B = new char16_t[5];
		r->B[0] = 't';
		r->B[1] = 'r';
		r->B[2] = 'u';
		r->B[3] = 'e';
		r->B[4] = 0;
	}
	else
	{
		r->L = static_cast<int64_t>(5);
		r->B = new char16_t[6];
		r->B[0] = 'f';
		r->B[1] = 'a';
		r->B[2] = 'l';
		r->B[3] = 's';
		r->B[4] = 'e';
		r->B[5] = 0;
	}
	return r;
}
static Array_<char16_t>* toStr_(double v) noexcept {
	std::stringstream s;
	s << v;
	const std::string& t = s.str();
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(t.size());
	r->B = new char16_t[t.size() + 1];
	int64_t p = 0;
	for (char c : t)
		r->B[p++] = c;
	r->B[t.size()] = 0;
	return r;
}
static Array_<char16_t>* toStr_(uint8_t v) noexcept {
	std::stringstream s;
	s << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(v);
	const std::string& t = s.str();
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(t.size());
	r->B = new char16_t[t.size() + 1];
	int64_t p = 0;
	for (char c : t)
		r->B[p++] = c;
	r->B[t.size()] = 0;
	return r;
}
static Array_<char16_t>* toStr_(uint16_t v) noexcept {
	std::stringstream s;
	s << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << v;
	const std::string& t = s.str();
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(t.size());
	r->B = new char16_t[t.size() + 1];
	int64_t p = 0;
	for (char c : t)
		r->B[p++] = c;
	r->B[t.size()] = 0;
	return r;
}
static Array_<char16_t>* toStr_(uint32_t v) noexcept {
	std::stringstream s;
	s << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << v;
	const std::string& t = s.str();
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(t.size());
	r->B = new char16_t[t.size() + 1];
	int64_t p = 0;
	for (char c : t)
		r->B[p++] = c;
	r->B[t.size()] = 0;
	return r;
}
static Array_<char16_t>* toStr_(uint64_t v) noexcept {
	std::stringstream s;
	s << "0x" << std::uppercase << std::setfill('0') << std::setw(16) << std::hex << v;
	const std::string& t = s.str();
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(t.size());
	r->B = new char16_t[t.size() + 1];
	int64_t p = 0;
	for (char c : t)
		r->B[p++] = c;
	r->B[t.size()] = 0;
	return r;
}
static Array_<char16_t>* toStr_(Array_<char16_t>* v) noexcept {
	std::u16string s = v->B;
	const std::string& t = utf16ToUtf8_(s);
	Array_<char16_t>* r = new Array_<char16_t>();
	r->L = static_cast<int64_t>(t.size());
	r->B = new char16_t[t.size() + 1];
	int64_t p = 0;
	for (char c : t)
		r->B[p++] = c;
	r->B[t.size()] = 0;
	return r;
}

static int64_t cmp_(Array_<char16_t>* a, Array_<char16_t>* b) noexcept {
	int64_t p = 0;
	while (p < a->L && p < b->L)
	{
		if (a->B[p] != b->B[p])
			return a->B[p] > b->B[p] ? 1 : (a->B[p] < b->B[p] ? -1 : 0);
		p++;
	}
	return a->L > b->L ? 1 : (a->L < b->L ? -1 : 0);
}
static int64_t cmp_(Class_* a, Class_* b) noexcept { return reinterpret_cast<int64_t(*)(Class_*, Class_*)>(classTable_[a->Y + 3])(a, b); }
static int64_t cmp_(int64_t a, int64_t b) noexcept { return a - b; }
static int64_t cmp_(char16_t a, char16_t b) noexcept { return static_cast<int64_t>(a) - static_cast<int64_t>(b); }
static int64_t cmp_(double a, double b) noexcept { return a > b ? 1LL : (a < b ? -1LL : 0LL); }
static int64_t cmp_(uint8_t a, uint8_t b) noexcept { return static_cast<int64_t>(a) - static_cast<int64_t>(b); }
static int64_t cmp_(uint16_t a, uint16_t b) noexcept { return static_cast<int64_t>(a) - static_cast<int64_t>(b); }
static int64_t cmp_(uint32_t a, uint32_t b) noexcept { return static_cast<int64_t>(a) - static_cast<int64_t>(b); }
static int64_t cmp_(uint64_t a, uint64_t b) noexcept { return a > b ? 1LL : (a < b ? -1LL : 0LL); }

static Array_<uint8_t>* makeBin_(const void* v, size_t s) noexcept {
	Array_<uint8_t>* r = new Array_<uint8_t>();
	r->L = s;
	r->B = new uint8_t[s];
	memcpy(r->B, v, s);
	return r;
}
static void mergeBin_(Array_<uint8_t>* a, const Array_<uint8_t>* b) noexcept {
	int64_t l = a->L + b->L;
	uint8_t* d = new uint8_t[static_cast<size_t>(l)];
	memcpy(d, a->B, static_cast<size_t>(a->L));
	memcpy(d + a->L, b->B, static_cast<size_t>(b->L));
	a->L = l;
	a->B = d;
}
template<typename T> struct toBin_ {};
template<typename T> struct toBin_<Array_<T>*> { Array_<uint8_t>* operator()(Array_<T>* v) noexcept {
	if (v == nullptr) { int64_t p = -1; return makeBin_(&p, sizeof(p)); }
	Array_<uint8_t>* r = makeBin_(&v->L, sizeof(int64_t));
	for (int64_t i = 0; i < v->L; i++)
		mergeBin_(r, toBin_<T>()(v->B[i]));
	return r;
}};
template<typename T> struct toBin_<List_<T>*> { Array_<uint8_t>* operator()(List_<T>* v) noexcept {
	if (v == nullptr) { int64_t p = -1; return makeBin_(&p, sizeof(p)); }
	int64_t s = static_cast<int64_t>(v->B.size());
	Array_<uint8_t>* r = makeBin_(&s, sizeof(int64_t));
	int64_t c = 0, d = -1;
	for (auto i = v->B.begin(); i != v->B.end(); ++i, ++c) if (i == v->I) { d = c; break; }
	mergeBin_(r, makeBin_(&d, sizeof(int64_t)));
	for (auto& n : v->B)
		mergeBin_(r, toBin_<T>()(n));
	return r;
}};
template<typename T> struct toBin_<Stack_<T>*> { Array_<uint8_t>* operator()(Stack_<T>* v) noexcept {
	if (v == nullptr) { int64_t p = -1; return makeBin_(&p, sizeof(p)); }
	int64_t s = static_cast<int64_t>(v->B.size());
	std::stack<T> b;
	Array_<uint8_t>* r = makeBin_(&s, sizeof(int64_t));
	while (!v->B.empty())
	{
		b.push(v->B.top());
		v->B.pop();
	}
	while (!b.empty())
	{
		T n = b.top();
		mergeBin_(r, toBin_<T>()(n));
		v->B.push(n);
		b.pop();
	}
	return r;
}};
template<typename T> struct toBin_<Queue_<T>*> { Array_<uint8_t>* operator()(Queue_<T>* v) noexcept {
	if (v == nullptr) { int64_t p = -1; return makeBin_(&p, sizeof(p)); }
	int64_t s = static_cast<int64_t>(v->B.size());
	std::queue<T> b;
	Array_<uint8_t>* r = makeBin_(&s, sizeof(int64_t));
	while (!v->B.empty())
	{
		T n = v->B.front();
		b.push(n);
		mergeBin_(r, toBin_<T>()(n));
		v->B.pop();
	}
	while (!b.empty())
	{
		v->B.push(b.front());
		b.pop();
	}
	return r;
}};
template<typename T1, typename T2> struct toBin_<Dict_<T1, T2>*> { Array_<uint8_t>* operator()(Dict_<T1, T2>* v) noexcept {
	if (v == nullptr) { int64_t p = -1; return makeBin_(&p, sizeof(p)); }
	Array_<uint8_t>* r = makeBin_(&v->L, sizeof(int64_t));
	dictToBinRec_<T1, T2>(r, v->B);
	return r;
}};
template<typename T> struct toBin_<T*> { Array_<uint8_t>* operator()(T* v) noexcept {
	if (std::is_class<T>::value)
	{
		if (v == nullptr) { int64_t p = -1; return makeBin_(&p, sizeof(p)); }
		Array_<uint8_t>* r = makeBin_(&v->Y, sizeof(int64_t));
		mergeBin_(r, reinterpret_cast<Array_<uint8_t>*(*)(Class_*)>(classTable_[v->Y + 5])(v));
		return r;
	}
	else
	{
		int64_t p = -1;
		return makeBin_(&p, sizeof(p));
	}
}};
template<> struct toBin_<int64_t> { Array_<uint8_t>* operator()(int64_t v) noexcept { return makeBin_(&v, sizeof(v)); } };
template<> struct toBin_<double> { Array_<uint8_t>* operator()(double v) noexcept { return makeBin_(&v, sizeof(v)); } };
template<> struct toBin_<char16_t> { Array_<uint8_t>* operator()(char16_t v) noexcept { return makeBin_(&v, sizeof(v)); } };
template<> struct toBin_<bool> { Array_<uint8_t>* operator()(bool v) noexcept { return makeBin_(&v, sizeof(v)); } };
template<> struct toBin_<uint8_t> { Array_<uint8_t>* operator()(uint8_t v) noexcept { return makeBin_(&v, sizeof(v)); } };
template<> struct toBin_<uint16_t> { Array_<uint8_t>* operator()(uint16_t v) noexcept { return makeBin_(&v, sizeof(v)); } };
template<> struct toBin_<uint32_t> { Array_<uint8_t>* operator()(uint32_t v) noexcept { return makeBin_(&v, sizeof(v)); } };
template<> struct toBin_<uint64_t> { Array_<uint8_t>* operator()(uint64_t v) noexcept { return makeBin_(&v, sizeof(v)); } };

template<typename T> struct fromBin_ {};

template<typename T> struct fromBin_<Array_<T>*> { Array_<T>* operator()(Array_<uint8_t>* b, int64_t& o) noexcept {
	int64_t l = *reinterpret_cast<int64_t*>(b->B + o);
	o += sizeof(int64_t);
	if (l == -1) return nullptr;
	Array_<T>* r = new Array_<T>();
	r->L = l;
	r->B = new T[static_cast<size_t>(l) + bufLen_<T>()];
	for (int64_t i = 0; i < l; i++)
		r->B[i] = fromBin_<T>()(b, o);
	if (bufLen_<T>() > 0)
		r->B[l] = 0;
	return r;
}};
template<typename T> struct fromBin_<List_<T>*> { List_<T>* operator()(Array_<uint8_t>* b, int64_t& o) noexcept {
	int64_t l = *reinterpret_cast<int64_t*>(b->B + o);
	o += sizeof(int64_t);
	if (l == -1) return nullptr;
	List_<T>* r = new List_<T>();
	int64_t d = *reinterpret_cast<int64_t*>(b->B + o);
	o += sizeof(int64_t);
	for (int64_t i = 0; i < l; i++)
	{
		r->B.push_back(fromBin_<T>()(b, o));
		if (i == d)
		{
			auto e = r->B.end();
			--e;
			r->I = e;
		}
	}
	return r;
}};
template<typename T> struct fromBin_<Stack_<T>*> { Stack_<T>* operator()(Array_<uint8_t>* b, int64_t& o) noexcept {
	int64_t l = *reinterpret_cast<int64_t*>(b->B + o);
	o += sizeof(int64_t);
	if (l == -1) return nullptr;
	Stack_<T>* r = new Stack_<T>();
	for (int64_t i = 0; i < l; i++)
		r->B.push(fromBin_<T>()(b, o));
	return r;
}};
template<typename T> struct fromBin_<Queue_<T>*> { Queue_<T>* operator()(Array_<uint8_t>* b, int64_t& o) noexcept {
	int64_t l = *reinterpret_cast<int64_t*>(b->B + o);
	o += sizeof(int64_t);
	if (l == -1) return nullptr;
	Queue_<T>* r = new Queue_<T>();
	for (int64_t i = 0; i < l; i++)
		r->B.push(fromBin_<T>()(b, o));
	return r;
}};
template<typename T1, typename T2> struct fromBin_<Dict_<T1, T2>*> { Dict_<T1, T2>* operator()(Array_<uint8_t>* b, int64_t& o) noexcept {
	int64_t l = *reinterpret_cast<int64_t*>(b->B + o);
	o += sizeof(int64_t);
	if (l == -1) return nullptr;
	Dict_<T1, T2>* r = new Dict_<T1, T2>();
	r->L = l;
	for (int64_t i = 0; i < l; i++)
	{
		T1 k = fromBin_<T1>()(b, o);
		T2 v = fromBin_<T2>()(b, o);
		r->Add(k, v);
	}
	return r;
}};
template<typename T> struct fromBin_<T*> { T* operator()(Array_<uint8_t>* b, int64_t& o) noexcept {
	if (std::is_class<T>::value)
	{
		int64_t y = *reinterpret_cast<int64_t*>(b->B + o);
		o += sizeof(int64_t);
		if (y == -1) return nullptr;
		return reinterpret_cast<T*>(reinterpret_cast<Class_*(*)(Class_*, Array_<uint8_t>*, int64_t*)>(classTable_[y + 6])(nullptr, b, &o));
	}
	else
	{
		o += sizeof(void*);
		return nullptr;
	}
}};
template<> struct fromBin_<int64_t> { int64_t operator()(Array_<uint8_t>* b, int64_t& o) noexcept { int64_t r = *reinterpret_cast<int64_t*>(b->B + o); o += sizeof(int64_t); return r; } };
template<> struct fromBin_<double> { double operator()(Array_<uint8_t>* b, int64_t& o) noexcept { double r = *reinterpret_cast<double*>(b->B + o); o += sizeof(double); return r; } };
template<> struct fromBin_<char16_t> { char16_t operator()(Array_<uint8_t>* b, int64_t& o) noexcept { char16_t r = *reinterpret_cast<char16_t*>(b->B + o); o += sizeof(char16_t); return r; } };
template<> struct fromBin_<bool> { bool operator()(Array_<uint8_t>* b, int64_t& o) noexcept { bool r = *reinterpret_cast<bool*>(b->B + o); o += sizeof(bool); return r; } };
template<> struct fromBin_<uint8_t> { uint8_t operator()(Array_<uint8_t>* b, int64_t& o) noexcept { uint8_t r = *reinterpret_cast<uint8_t*>(b->B + o); o += sizeof(uint8_t); return r; } };
template<> struct fromBin_<uint16_t> { uint16_t operator()(Array_<uint8_t>* b, int64_t& o) noexcept { uint16_t r = *reinterpret_cast<uint16_t*>(b->B + o); o += sizeof(uint16_t); return r; } };
template<> struct fromBin_<uint32_t> { uint32_t operator()(Array_<uint8_t>* b, int64_t& o) noexcept { uint32_t r = *reinterpret_cast<uint32_t*>(b->B + o); o += sizeof(uint32_t); return r; } };
template<> struct fromBin_<uint64_t> { uint64_t operator()(Array_<uint8_t>* b, int64_t& o) noexcept { uint64_t r = *reinterpret_cast<uint64_t*>(b->B + o); o += sizeof(uint64_t); return r; } };

template<typename T> Array_<T>* sub_(Array_<T>* a, int64_t start, int64_t len) noexcept {
	if (len == -1)
		len = a->L - start;
	if (start < 0 || len < 0 || start + len > a->L)
		return nullptr;
	Array_<T>* r = new Array_<T>();
	r->L = len;
	r->B = new T[static_cast<size_t>(len + bufLen_<T>())];
	for (int64_t i = 0; i < len; i++)
		r->B[i] = a->B[start + i];
	if (bufLen_<T>() > 0)
		r->B[len] = 0;
	return r;
}

template<typename T> T* as_(const int64_t* y, Class_* c, int64_t o) noexcept {
	if (c == nullptr)
		return nullptr;
	int64_t m = c->Y;
	for (; ; )
	{
		if (m == o)
			return reinterpret_cast<T*>(c);
		if (m == 0)
			return nullptr;
		m = y[m];
	}
}

static bool is_(const int64_t* y, Class_* c, int64_t o) noexcept {
	int64_t m = c->Y;
	for (; ; )
	{
		if (m == o)
			return true;
		if (m == 0)
			return false;
		m = y[m];
	}
}

template<typename T1, typename T2> dictImpl_<T1, T2>* dictAddRec_(dictImpl_<T1, T2>* n, T1 k, T2 v, bool* a) noexcept {
	if (n == nullptr)
	{
		*a = true;
		return new dictImpl_<T1, T2>(k, v);
	}
	{
		int64_t c = cmp_(k, n->K);
		if (c == 0)
		{
			n->V = v;
			return n;
		}
		if (c < 0)
			n->CL = dictAddRec_<T1, T2>(n->CL, k, v, a);
		else
			n->CR = dictAddRec_<T1, T2>(n->CR, k, v, a);
	}
	if (n->CR != nullptr && n->CR->R)
	{
		dictImpl_<T1, T2>* r = n->CR;
		n->CR = r->CL;
		r->CL = n;
		r->R = n->R;
		n->R = true;
		n = r;
	}
	if (n->CL != nullptr && n->CL->R && n->CL->CL != nullptr && n->CL->CL->R)
	{
		dictImpl_<T1, T2>* l = n->CL;
		n->CL = l->CR;
		l->CR = n;
		l->R = n->R;
		n->R = true;
		n = l;
		n->R = true;
		n->CL->R = false;
		n->CR->R = false;
	}
	return n;
}
template<typename T1, typename T2> dictImpl_<T1, T2>* dictAdd_(dictImpl_<T1, T2>* r, T1 k, T2 v, bool* a) noexcept {
	*a = false;
	dictImpl_<T1, T2>* n = dictAddRec_(r, k, v, a);
	n->R = false;
	return n;
}
template<typename T1, typename T2> dictImpl_<T1, T2>* dictCopyRec_(dictImpl_<T1, T2>* n) noexcept {
	if (n == nullptr)
		return nullptr;
	dictImpl_<T1, T2>* r = new dictImpl_<T1, T2>(copy_<T1>()(n->K), copy_<T2>()(n->V));
	r->R = n->R;
	r->CL = dictCopyRec_(n->CL);
	r->CR = dictCopyRec_(n->CR);
	return r;
}
template<typename T1, typename T2> void dictToBinRec_(Array_<uint8_t>* a, dictImpl_<T1, T2>* d) noexcept {
	if (d->CL != nullptr)
		dictToBinRec_(a, d->CL);
	mergeBin_(a, toBin_<T1>()(d->K));
	mergeBin_(a, toBin_<T2>()(d->V));
	if (d->CR != nullptr)
		dictToBinRec_(a, d->CR);
}
template<typename T1, typename T2> T2 dictSearch_(dictImpl_<T1, T2>* r, T1 k, bool* f) noexcept {
	dictImpl_<T1, T2>* n = r;
	while (n != nullptr)
	{
		int64_t c = cmp_(k, n->K);
		if (c == 0)
		{
			*f = true;
			return n->V;
		}
		if (c < 0)
			n = n->CL;
		else
			n = n->CR;
	}
	*f = false;
	return (T2)0;
}
template<typename T1, typename T2> bool dictForEach_(dictImpl_<T1, T2>* r, bool(*f)(T1, T2, Class_*), Class_* p) noexcept {
	if (r == nullptr)
		return true;
	if (!dictForEach_<T1, T2>(r->CL, f, p))
		return false;
	if (!f(r->K, r->V, p))
		return false;
	if (!dictForEach_<T1, T2>(r->CR, f, p))
		return false;
	return true;
}

static bool eqAddr_(const Ref_* a, const Ref_* b) noexcept { return a == b; }

static uint32_t rX_, rY_, rZ_, rW_;
static uint32_t xs128_() noexcept {
	uint32_t t = rX_ ^ (rX_ << 11);
	rX_ = rY_;
	rY_ = rZ_;
	rZ_ = rW_;
	return rW_ = (rW_ ^ (rW_ >> 19)) ^ (t ^ (t >> 8));
}
static int64_t rnd_(int64_t a, int64_t b) noexcept {
	uint64_t n = (uint64_t)(b - a + 1);
	uint64_t m = 0 - ((0 - n) % n);
	uint64_t r;
	if (m == 0)
		r = (static_cast<uint64_t>(xs128_()) << 32) | static_cast<uint64_t>(xs128_());
	else
	{
		do
		{
			r = (static_cast<uint64_t>(xs128_()) << 32) | static_cast<uint64_t>(xs128_());
		} while (m <= r);
	}
	return static_cast<int64_t>(r % n) + a;
}

static double rndFloat_(double a, double b) noexcept {
	return (double)((static_cast<uint64_t>(xs128_()) << 32) | static_cast<uint64_t>(xs128_())) / 18446744073709551616.0 * (b - a) + a;
}

static int64_t powI_(int64_t a, int64_t b) noexcept {
	switch (b)
	{
	case 0LL:
		return 1LL;
	case 1LL:
		return a;
	case 2LL:
		return a * a;
	}
	if (a == 1LL)
		return 1LL;
	if (a == -1LL)
		return std::abs(b) % 2LL == 0LL ? 1LL : -1LL;
	if (b < 0LL)
		return 0LL;
	int64_t r = 1LL;
	for (; ; )
	{
		if ((b & 1LL) == 1LL)
			r *= a;
		b >>= 1LL;
		if (b == 0LL)
			break;
		a *= a;
	}
	return r;
}

template<typename T> void reverse_(Array_<T>* me) { std::reverse<T*>(me->B, me->B + me->L); }
template<typename T> bool sortCmp_(const T& a, const T& b) { return cmp_(a, b) < 0; }
template<typename T> void sort_(Array_<T>* me) { std::sort<T*, bool(*)(const T&, const T&)>(me->B, me->B + me->L, sortCmp_<T>); }
static uint8_t sar_(uint8_t me_, int64_t n) { return static_cast<uint8_t>(static_cast<int8_t>(me_) >> n); }
static uint16_t sar_(uint16_t me_, int64_t n) { return static_cast<uint16_t>(static_cast<int16_t>(me_) >> n); }
static uint32_t sar_(uint32_t me_, int64_t n) { return static_cast<uint32_t>(static_cast<int32_t>(me_) >> n); }
static uint64_t sar_(uint64_t me_, int64_t n) { return static_cast<uint64_t>(static_cast<int64_t>(me_) >> n); }
static uint8_t endian_(uint8_t me_) { return me_; }
static uint16_t endian_(uint16_t me_) { return ((me_ & 0x00ff) << 8) | ((me_ & 0xff00) >> 8); }
static uint32_t endian_(uint32_t me_)
{
	me_ = ((me_ & 0x00ff00ff) << 8) | ((me_ & 0xff00ff00) >> 8);
	return ((me_ & 0x0000ffff) << 16) | ((me_ & 0xffff0000) >> 16);
}
static uint64_t endian_(uint64_t me_)
{
	me_ = ((me_ & 0x00ff00ff00ff00ff) << 8) | ((me_ & 0xff00ff00ff00ff00) >> 8);
	me_ = ((me_ & 0x0000ffff0000ffff) << 16) | ((me_ & 0xffff0000ffff0000) >> 16);
	return ((me_ & 0x00000000ffffffff) << 32) | ((me_ & 0xffffffff00000000) >> 32);
}

struct reader_ {
	reader_() noexcept : F(new std::ifstream()) {}
	std::ifstream* F;
};

struct writer_ {
	writer_() noexcept : F(new std::ofstream()) {}
	std::ofstream* F;
};

static char16_t readUtf8_(std::ifstream* f) {
	char c;
	int64_t l;
	uint64_t u;
	if (!f->get(c))
		return 0xffff;
	if ((c & 0xc0) == 0x80)
		return 0xffff;
	if ((c & 0x80) == 0x00)
		l = 0;
	else if ((c & 0xe0) == 0xc0)
		l = 1, c &= 0x1f;
	else if ((c & 0xf0) == 0xe0)
		l = 2, c &= 0x0f;
	else if ((c & 0xf8) == 0xf0)
		l = 3, c &= 0x07;
	else if ((c & 0xfc) == 0xf8)
		l = 4, c &= 0x03;
	else if ((c & 0xfe) == 0xfc)
		l = 5, c &= 0x01;
	else
		return 0xffff;
	u = static_cast<uint64_t>(c);
	for (int64_t i = 0; i < l; i++)
	{
		if (!f->get(c) || (c & 0xc0) != 0x80)
			return 0xffff;
		u = (u << 6) | static_cast<uint64_t>(c & 0x3f);
	}
	if (0x00010000 <= u && u <= 0x0010ffff)
		u = 0x20;
	return static_cast<char16_t>(u);
}

static void writeUtf8_(std::ofstream* f, char16_t c) {
	uint64_t u;
	size_t size;
	if ((c >> 7) == 0)
		u = c, size = 1;
	else
	{
		u = static_cast<uint64_t>(0x80 | (c & 0x3f)) << 8;
		c >>= 6;
		if ((c >> 5) == 0)
			u |= 0xc0 | c, size = 2;
		else
		{
			u = (u | 0x80 | (c & 0x3f)) << 8;
			c >>= 6;
			if ((c >> 4) == 0)
				u |= 0xe0 | c, size = 3;
			else
			{
				u = (u | 0x80 | (c & 0x3f)) << 8;
				c >>= 6;
				if ((c >> 3) == 0)
					u |= 0xf0 | c, size = 4;
				else
				{
					u = (u | 0x80 | (c & 0x3f)) << 8;
					c >>= 6;
					if ((c >> 2) == 0)
						u |= 0xf8 | c, size = 5;
					else
					{
						u = (u | 0x80 | (c & 0x3f)) << 8;
						c >>= 6;
						if ((c >> 1) == 0)
							u |= 0xfc | c, size = 6;
						else
							return;
					}
				}
			}
		}
	}
	if (size == 1 && u == 0x0a)
		f->write(newLine_, static_cast<std::streamsize>(sizeof(newLine_)));
	else
		f->write(reinterpret_cast<char*>(&u), static_cast<std::streamsize>(size));
}

static void init_() {
	uint32_t t = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	rX_ = 123456789;
	rY_ = 362436069;
	rZ_ = 521288629 * t;
	rW_ = 88675123 * (rZ_ >> 1);
}

Array_<char16_t> delimiter_(3,',',' ','\n');
