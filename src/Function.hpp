#include <algorithm>	// max
#include <utility>		// exchange, forward, move
#include <type_traits>	// aligned_storage, decay, reference_wrapper

template<typename T> struct Function;

template<typename ReturnType, typename...Args>
class Function<ReturnType(Args...)>
{
	// A function pointer or a reference_wrapper should require no extra memory
	static constexpr size_t StorageSize = std::max(sizeof(void(*)()), sizeof(std::reference_wrapper<char>));
	
	using storage_t = typename std::aligned_storage<StorageSize>::type;

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
			std::exchange(other.m_callable, nullptr)->~callable();
		}
		else if (other.m_callable)
		{
			// callable is large; transfer the pointer to our m_callable, leaving the source empty
			m_callable = std::exchange(other.m_callable, nullptr);
		}
	}

	ReturnType operator()(Args&&...args)
	{
		return m_callable->invoke(std::forward<Args>(args)...);
	}

private:
	void* storage() const { return m_storage; }

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

		callable_base* clone(storage_t& storage) override
		{
			return std::make_unique<callable>(m_t);
		}

		callable_base* move(storage_t& storage) override
		{

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

	// 
	storage_t m_storage;
};

