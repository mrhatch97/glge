#include "glge/Renderer/SceneGraph/Scene.h"

#include <glge/Util/Math.h>
#include <glge/Renderer/SceneGraph/SceneCamera.h>

// TODO delete this
#include <iostream>

namespace eng::render::scene
{
	SceneSettings::SceneSettings(const SceneCamera * active_camera,
		bool draw_bounding_spheres,
		bool enable_VF_culling) :
		active_camera(active_camera),
		draw_bounding_spheres(draw_bounding_spheres),
		enable_VF_culling(enable_VF_culling)
	{ }

	Scene::Scene(uptr<Node> && root) :
		root(std::move(root)),
		settings(nullptr, false, false)
	{
	}

	void Scene::draw() const
	{
		static const mat4 identity(1.0f);

		//size_t numModels = 0;
		using time_res = std::chrono::milliseconds;
		//util::bench_clock::duration gpuTime(0);
		//auto frameTime = util::bench_clock::now();

		//auto modelCountCallback = [&numModels] { numModels++; };
		//auto gpuTimeCallback = [&gpuTime](const util::bench_clock::duration & time) { gpuTime += time; };

		SceneSettings scene_params = settings;

		//root->draw(identity, scene_params);

		//time_res frameTimeTicks = std::chrono::duration_cast<time_res>(util::bench_clock::now() - frameTime);
		//time_res gpuTimeTicks = std::chrono::duration_cast<time_res>(gpuTime);

		//std::clog << "Drew " << numModels << " models" << std::endl;
		//std::clog << "GPU time: " << gpuTimeTicks.count() << " ms" << std::endl;
		//std::clog << "Frame time: " << frameTimeTicks.count() << " ms" << std::endl;
		//std::clog << std::endl;
	}

	void Scene::view_resized(int viewWidth, int viewHeight)
	{
		if (viewHeight > 0)
		{
			auto set_ratio = [=](SceneCamera * cam) 
			{
				cam->camera_intrinsics.set_ratio(viewWidth, viewHeight);
			};
			std::for_each(cameras.begin(), cameras.end(), set_ratio);
		}
	}

	Node & Scene::get_root()
	{
		return *root;
	}

	const Node & Scene::get_root() const
	{
		return *root;
	}

	void Scene::add_camera(SceneCamera & cam)
	{
		cameras.push_back(&cam);
	}

	Scene::~Scene() = default;
}