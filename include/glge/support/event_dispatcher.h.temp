// TODO find a home for this code

#pragma once

#include <glge/common.h>
#include <glge/util/util.h>

#include <any>
#include <forward_list>
#include <typeindex>
#include <unordered_map>

namespace glge::event
{
	template<typename Event>
	class Handler
	{
	public:
		virtual void raise(const Event & e) = 0;
		virtual ~Handler() { };
	};

	class EventDispatcher
	{
	private:
		std::unordered_multimap<std::type_index, std::any> listener_map;
	public:
		EventDispatcher();

		template<typename Event>
		void raise(const Event & e)
		{
			auto[begin, end] = listener_map.equal_range(std::type_index(typeid(e)));

			auto raise_handler = [&](std::pair<const std::type_index, std::any> & pair)
			{
				try
				{
					std::any_cast<Handler<Event> *>(pair.second)->raise(e);
				}
				catch (const std::bad_any_cast &)
				{
					std::throw_with_nested(std::logic_error(EXC_MSG("Registered handler was not able to be cast to a Handler")));
				}
			};

			std::for_each(begin, end, raise_handler);
		}

		template<typename Event>
		void register_handler(Handler<Event> & handler)
		{
			listener_map.emplace(std::type_index(typeid(Event)), std::any(&handler));
		}
	};
}
