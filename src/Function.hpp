#include <algorithm>	// max
#include <utility>		// exchange, forward, move
#include <type_traits>	// aligned_storage, decay, reference_wrapper

#include <iostream>

namespace
{
	// Dummy is a prototype for class callable, which contains:
	// - a vtable-pointer,
	// - a function pointer as a field.
	template<typename T> struct Dummy { virtual ~Dummy(); T _val; };

	static inline constexpr size_t StorageSize = std::max(sizeof(Dummy<void(*)()>), sizeof(std::reference_wrapper<char>));
}

template<typename T> struct Function;

template<typename ReturnType, typename...Args>
class Function<ReturnType(Args...)>
{
	using storage_t = typename std::aligned_storage_t<StorageSize>;

public:
	Function() = default;

	template<typename Func> Function(Func&& t)
	{
		if constexpr (sizeof(*new callable(t)) <= StorageSize)
		{
			// small enough to be put into m_storage by placement-new
			m_callable = new(storage()) callable<std::decay_t<Func>>(std::forward<Func>(t));
		}
		else
		{
			// quite large, needs to be heap-allocated
			m_callable = new callable<std::decay_t<Func>>(std::forward<Func>(t));
		}
	}

	Function(const Function& other)
	{
		if (other.m_callable)
		{
			// try to clone other.m_callable to our m_storage
			m_callable = other.m_callable->clone(m_storage);
		}
	}

	Function(Function&& other)
	{
		if (other.m_callable == other.storage())
		{
			// callable is small; move it to our m_storage and destruct the source leaving it empty
			m_callable = other.m_callable->move(m_storage);
			std::exchange(other.m_callable, nullptr)->~callable_base();
		}
		else if (other.m_callable)
		{
			// callable is large; transfer the pointer to our m_callable, leaving the source empty
			m_callable = std::exchange(other.m_callable, nullptr);
		}
	}

	~Function()
	{
		if (m_callable == storage())
			m_callable->~callable_base();
		else
			delete m_callable;
	}

	ReturnType operator()(Args&&...args)
	{
		return m_callable->invoke(std::forward<Args>(args)...);
	}

private:
	void* storage() { return std::addressof(m_storage); }

private:
	struct callable_base
	{
		callable_base() = default;
		virtual ~callable_base() = default;

		virtual callable_base* clone(storage_t&) = 0;
		virtual callable_base* move(storage_t&) = 0;

		virtual ReturnType invoke(Args...args) = 0;
	};

	template <typename Function> struct callable : callable_base
	{
		callable(const Function& t) : m_t(t) {}
		callable(Function&& t) : m_t(std::move(t)) {}
		~callable() override = default;

		callable_base* clone(storage_t& storage) override
		{
			if constexpr (sizeof(*this) <= sizeof(storage))
				return new(std::addressof(storage)) callable(m_t);
			else
				return new callable(m_t);
		}

		callable_base* move(storage_t& storage) override
		{
			if constexpr (sizeof(*this) <= sizeof(storage))
				return new(std::addressof(storage)) callable(std::move(m_t));
			else
				return nullptr;		// should not be called
		}

		ReturnType invoke(Args...args) override
		{
			return m_t(std::forward<Args>(args)...);
		}

		Function m_t;
	};

private:
	// Points either to a heap-allocated callable<T> (if large) or to a callable<T> in m_storage (if small)
	callable_base* m_callable = nullptr;

	// A function pointer or a reference_wrapper should require no extra memory
	storage_t m_storage;
};

