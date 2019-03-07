#pragma once

#include <glge/renderer/resources/resource_data.h>
#include <glge/renderer/primitives/model.h>
#include <glge/renderer/primitives/texture.h>
#include <glge/renderer/primitives/cubemap.h>

#include <glge/common.h>
#include <glge/util/util.h>

#include <functional>
#include <future>
#include <unordered_map>
#include <variant>

namespace glge::renderer::resource
{
	using id_type = unsigned int;

	template<bool Async, typename T>
	using MaybeFuture = std::conditional_t<Async, std::future<T>, T>;

	template<typename T>
	struct is_loadable_to_resource
	{
		static const bool value = false;
	};

	template<typename T>
	constexpr bool is_loadable_to_resource_v = is_loadable_to_resource<T>::value;

	template<typename T>
	struct is_loadable_to_file_info
	{
		static const bool value = false;
	};

	template<typename T>
	constexpr bool is_loadable_to_file_info_v = is_loadable_to_file_info<T>::value;

	template<typename T>
	struct is_async_loadable
	{
		static const bool value = false;
	};

	template<typename T>
	constexpr bool is_async_loadable_v = is_async_loadable<T>::value;

	template<>
	struct is_loadable_to_resource<ModelData>
	{
		static const bool value = true;
	};

	template<>
	struct is_loadable_to_resource<TextureData>
	{
		static const bool value = true;
	};

	template<>
	struct is_loadable_to_resource<TextureFileInfo>
	{
		static const bool value = true;
	};

	template<>
	struct is_loadable_to_resource<CubemapFileInfo>
	{
		static const bool value = true;
	};

	template<>
	struct is_loadable_to_resource<CubemapData>
	{
		static const bool value = true;
	};

	template<>
	struct is_loadable_to_file_info<ModelFileInfo>
	{
		static const bool value = true;
	};

	template<>
	struct is_loadable_to_file_info<TextureFileInfo>
	{
		// TODO hmm
		static const bool value = false;
	};

	template<>
	struct is_loadable_to_file_info<CubemapFileInfo>
	{
		// TODO necessary?
		static const bool value = false;
	};

	template<>
	struct is_async_loadable<ModelData>
	{
		static const bool value = true;
	};

	template<>
	struct is_async_loadable<TextureData>
	{
		static const bool value = true;
	};

	template<>
	struct is_async_loadable<CubemapData>
	{
		static const bool value = true;
	};

	template<typename T>
	using is_loadable = std::disjunction<
		std::bool_constant<is_loadable_to_file_info_v<T>>,
		std::bool_constant<is_loadable_to_resource_v<T>>
	>;

	template<typename T>
	constexpr bool is_loadable_v = is_loadable<T>::value;

	class ResourceManager
	{
	private:
		static const bool enable_async_load = false;

		Map<id_type, uptr<primitive::Model>> models;
		Map<id_type, uptr<primitive::Texture>> textures;
		Map<id_type, uptr<primitive::Cubemap>> cubemaps;

		template<typename T>
		constexpr Map<id_type, uptr<typename std::decay_t<T>>> &
			getMapFor()
		{
			using Resource = typename std::decay_t<T>;

			return const_cast<Map<id_type, uptr<Resource>> &>(static_cast<const ResourceManager *>(this)->getMapFor<T>());
		}

		template<typename T>
		constexpr const Map<id_type, uptr<typename std::decay_t<T>>> &
			getMapFor() const
		{
			using Resource = typename std::decay_t<T>;

			if constexpr (std::is_same_v<Resource, Model>)
			{
				return models;
			}
			else if constexpr (std::is_same_v<Resource, Texture>)
			{
				return textures;
			}
			else if constexpr (std::is_same_v<Resource, Cubemap>)
			{
				return cubemaps;
			}
		}

		template<typename T>
		uptr<typename T::manifest_type> _load_to_resource(const T & info)
		{
			using Resource = typename T::manifest_type;
			try
			{
				return std::move(std::make_unique<Resource>(info));
			}
			catch (...)
			{
				std::throw_with_nested(std::runtime_error(EXC_MSG("Failed to load resource")));
			}
		}

		template<typename T>
		uptr<typename T::manifest_type> _load_to_resource(std::future<T> & f)
		{
			try
			{
				return _load_to_resource(f.get());
			}
			catch (...)
			{
				std::throw_with_nested(std::runtime_error(EXC_MSG("Failed to load resource (async)")));
			}
		}

		template<bool Async, typename ResourceFileInfo>
		static typename MaybeFuture<Async, typename ResourceFileInfo::data_type>
			_load_file_to_info(const ResourceFileInfo & fileInfo)
		{
			using ResourceInfo = typename ResourceFileInfo::data_type;

			if constexpr (Async)
			{
				return std::async(std::launch::async, &_load_file_to_info<false, ResourceFileInfo>, fileInfo);
			}
			else
			{
				return ResourceInfo(fileInfo);
			}
		}
	public:

		ResourceManager();
		ResourceManager(const ResourceManager &) = delete;

		ResourceManager & operator=(const ResourceManager &) = delete;

		template<typename T>
		typename std::enable_if_t<is_loadable_to_resource_v<T>, void>
			load(const T & info)
		{
			using Resource = typename T::manifest_type;

			auto ptr = _load_to_resource(info);
			auto id = info.get_res_id();

			getMapFor<Resource>()[id] = std::move(ptr);
		}

		template<typename T>
		void load(std::future<T> & f)
		{
			try
			{
				load<T>(f.get());
			}
			catch (...)
			{
				std::throw_with_nested(std::runtime_error(EXC_MSG("Failed to load resource (async)")));
			}
		}

		template<typename T>
		void load(std::vector<std::future<T>> & futureInfos)
		{
			using ResourceData = T;
			using Resource = typename ResourceData::manifest_type *;

			auto loader = [&](std::future<ResourceData> & info) { this->load(info); };

			std::for_each(futureInfos.begin(), futureInfos.end(), loader);
		}

		template<typename T>
		typename std::enable_if_t<is_loadable_v<T>, void>
			load(const Vector<T> & infos)
		{
			using Resource = typename T::manifest_type *;
			// Parse file
			if constexpr (is_loadable_to_file_info<T>::value)
			{
				using ResourceFileInfo = T;
				using ResourceData = typename ResourceFileInfo::data_type;
				constexpr bool async = enable_async_load && is_async_loadable_v<ResourceData>;
				using Transformed = typename std::conditional_t<async, std::future<ResourceData>, ResourceData>;

				std::vector<Transformed> new_infos(infos.size());

				std::transform(infos.cbegin(), infos.cend(), new_infos.begin(), &_load_file_to_info<async, ResourceFileInfo>);

				load(new_infos);
			}
			// Load directly to resource
			else
			{
				using ResourceData = T;

				auto loader = [&](const ResourceData & info) { this->load(info); };

				std::for_each(infos.cbegin(), infos.cend(), loader);
			}
		}

		template<typename T>
		T & get(const id_type id)
		{
			return const_cast<T &>(static_cast<const ResourceManager *>(this)->get<T>(id));
		}

		template<typename T>
		const T & get(const id_type id) const
		{
			return *getMapFor<T>().at(id);
		}

		~ResourceManager();
	};
}
