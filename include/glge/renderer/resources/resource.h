#pragma once

#include <glge/common.h>
#include <glge/Renderer/Resources/ResourceManager.h>

namespace glge::renderer::resource
{
	template<typename ResT>
	class Resource
	{
		ResourceManager & manager;
		std::function<void()> load;
		std::function<void()> unload;
		id_type res_id;
	public:
		Resource(ResourceManager & manager, uptr<ResT> && resource) : 
			manager(manager), load([] {}), unload([] {})
		{

		}

		template<typename DataT>
		Resource(ResourceManager & manager, const DataT & data) : 
			manager(manager)
		{

		}

		operator ResT()
		{
			return manager.get<ResT>(res_id);
		}

		void signal_load()
		{
			load();
		}

		void signal_unload()
		{
			unload();
		}
	};
}