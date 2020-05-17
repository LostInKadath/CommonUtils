#include <memory>

template<typename T> struct Function;

template<typename ReturnType, typename...Args>
struct Function<ReturnType(Args...)>
{
	Function() = default;

	template<typename Func>
	Function(Func&& t)
		: m_callable(new callable<std::decay_t<Func>>(std::forward<Func>(t)))
	{}

	Function(const Function& other)
		: m_callable(other.m_callable ? other.m_callable->clone() : nullptr)
	{}

	Function(Function&&) = default;

	ReturnType operator()(Args&&...args)
	{
		return m_callable->invoke(std::forward<Args>(args)...);
	}

private:
	struct callable_base
	{
		callable_base() = default;
		virtual ~callable_base() = default;
		virtual std::unique_ptr<callable_base> clone() = 0;

		virtual ReturnType invoke(Args...args) = 0;
	};

	template <typename Function> struct callable : callable_base
	{
		callable(const Function& t) : m_t(t) {}
		callable(Function&& t) : m_t(std::move(t)) {}

		std::unique_ptr<callable_base> clone() override
		{
			return std::make_unique<callable>(m_t);
		}

		ReturnType invoke(Args...args) override
		{
			return m_t(std::forward<Args>(args)...);
		}

		Function m_t;
	};

	std::unique_ptr<callable_base> m_callable;
};

