/// <summary>Support for 3D models.</summary>
///
/// Contains class for using 3D models.
///
/// \file model.h

#pragma once

#include <glge/common.h>

#include <glge/renderer/primitives/renderable.h>
#include <glge/renderer/primitives/primitive_data.h>

namespace glge::renderer::primitive
{
  /// <summary>
  /// Class representing a 3D model.
  /// </summary>
	class Model : public Renderable
	{
	public:
		Model() = default;

		virtual ~Model() = default;

    /// <summary>
    /// Load a model from a file on disk.
    /// </summary>
    /// <param name="file_info">Descriptor for the model file.</param>
    /// <returns>Pointer to created model.</returns>
		static unique_ptr<Model> from_file(const ModelFileInfo & file_info);

    /// <summary>
    /// Load a model from a set of model data. Copies the supplied data; may
    /// be slower than moving.
    /// </summary>
    /// <param name="model_data">Set of model data to load from.</param>
    /// <returns>Pointer to created model.</returns>
		static unique_ptr<Model> from_data(const ModelData & model_data);

    /// <summary>
    /// Load a model from a set of model data. Takes ownership of the supplied
    /// data.
    /// </summary>
    /// <param name="model_data">Set of model data to load from.</param>
    /// <returns>Pointer to created model.</returns>
		static unique_ptr<Model> from_data(ModelData && model_data);

    /// <summary>
    /// Load a model from a set of model data transformed to be used in an EBO. 
    /// Data is not copied.
    /// </summary>
    /// <param name="model_data">Set of model data to load from.</param>
    /// <returns>Pointer to created model.</returns>
		static unique_ptr<Model> from_data(const EBOModelData & model_data);
	};
}
