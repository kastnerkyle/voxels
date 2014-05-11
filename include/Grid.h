// Copyright (c) 2013-2014, Stoyan Nikolov
// All rights reserved.
// Voxels Library, please see LICENSE for licensing details.
#pragma once

#include "Declarations.h"
#include "Structs.h"

namespace Voxels
{

class VoxelSurface;
class VoxelGrid;

/// Indicates the type of surface injection
///
enum InjectionType
{
	IT_Add,
	IT_SubtractAddInner,
	IT_Subtract,
};

typedef unsigned char MaterialId;
typedef unsigned char BlendFactor;

/// A Voxel grid. Voxels inside are kept compressed
/// **Note:** Grids use a coordinate system where Z is *up*
class VOXELS_API Grid
{
public:
	/// A Voxel grid packed for serilization
	///
	struct PackedGrid
	{
		/// Destroys the pack
		///
		virtual void Destroy() = 0;
		/// Returns the size in bytes of the packed grid
		/// @return size in bytes
		virtual unsigned GetSize() const = 0;
		/// Returns the pointer to the data in the packed grid
		/// @return pointer to the pack
		virtual const char* GetData() const = 0;
	};

	/// Creates a voxel grid generated by a surface
	/// @param w width of the grid in voxels
	/// @param d depth of the grid in voxels
	/// @param h height of the grid in voxels
	/// @param startX starting point on the X axis in the coords of the surface
	/// @param startY starting point on the Y axis in the coords of the surface
	/// @param startZ starting point on the Z axis in the coords of the surface
	/// @param step coordinate step on the surface for the next voxel coordinate
	/// @param surface a surface that generates the Voxels
	/// @return the resultant voxel grid
	static Grid* Create(unsigned w, unsigned d, unsigned h,
		float startX, float startY, float startZ, float step,
		VoxelSurface* surface);

	/// Creates an empty voxel grid with the specified dimensions
	/// @param w width of the grid in voxels
	/// @param d depth of the grid in voxels
	/// @param h height of the grid in voxels
	/// @return the resultant voxel grid
	static Grid* Create(unsigned w, unsigned d, unsigned h);

	/// Creates a voxel grid from heighmap values
	/// @param w param used for all dimensions of the grid, the result will be a w*w*w grid
	/// @param heightmap the heightmap used to generate the grid, it should contain w*w*w values
	static Grid* Create(unsigned w, const char* heightmap);

	/// Loads a grid from packed data
	/// @param blob pointer to the packed grid
	/// @param size of the memoty blob
	/// @return the newly created grid
	static Grid* Load(const char* blob, unsigned size);

	/// Destroys the voxel grid
	///
	void Destroy();

	/// Generates a packed grid that can be serialized to disk
	/// @return the packed grid
	PackedGrid* PackForSave() const;

	/// Gets the width of the grid
	/// @return the width
	unsigned GetWidth() const;
	/// Gets the depth of the grid
	/// @return the depth
	unsigned GetDepth() const;
	/// Gets the height of the grid
	/// @return the height
	unsigned GetHeight() const;

	/// Injects a surface in the grid
	/// @param position point in the grid where to apply the surface
	/// @param extents the extents of the modification on all axes
	/// @param surface the surface that will generate the new voxels
	/// @param type the type of the injection operation
	/// @return a pair of coordinates that indicate a box where the modifications took place
	float3pair InjectSurface(const float3& position,
							const float3& extents,
							VoxelSurface* surface,
							InjectionType type);
	
	/// Injects a material in the grid
	/// @param position point in the grid where to apply the material
	/// @param extents the extents of the modification on all axes
	/// @param material the material to inject
	/// @param addSubtractBlend if to add or subtract the material blend
	/// @return a pair of coordinates that indicate a box where the modifications took place
	float3pair InjectMaterial(const float3& position,
							const float3& extents,
							MaterialId material,
							bool addSubtractBlend);
	
	/// The block extents in voxels. Blocks are always cubes
	/// @return the extent in voxels on all axes
	unsigned GetBlockExtent() const;
	
	/// Gets the distance data of a block
	/// @param coords the coordinates of a block
	/// @param output the memory where to copy the data - should be block_extent^3 * sizeof(char) bytes
	/// @return if the data is valid
	bool GetBlockDistanceData(const float3& coords, char* output) const;

	/// Modifies the distance data of a block
	/// @param coords the coordinates of a block
	/// @param distances the memory from where to copy the data - should be block_extent^3 * sizeof(char) bytes
	void ModifyBlockDistanceData(const float3& coords, const char* distances);

	/// Gets the material data of a block
	/// @param coords the coordinates of a block
	/// @param materials the memory where to copy the material data - should be block_extent^3 * sizeof(MaterialId) bytes
	/// @param blends the memory where to copy the material blending data - should be block_extent^3 * sizeof(BlendFactor) bytes
	/// @return if the data is valid
	bool GetBlockMaterialData(const float3& coords, MaterialId* materials, BlendFactor* blends) const;

	/// Modifies the material data of a block
	/// @param coords the coordinates of a block
	/// @param materials the memory from where to copy the material data - should be block_extent^3 * sizeof(MaterialId) bytes
	/// @param blends the memory from where to copy the material blending data - should be block_extent^3 * sizeof(BlendFactor) bytes
	void ModifyBlockMaterialData(const float3& coords, const MaterialId* materials, const BlendFactor* blends);

	/// Returns the amount of memory used for the compressed blocks in the grid
	/// @return memory size in bytes
	unsigned GetGridBlocksMemorySize();

	VoxelGrid* GetInternalRepresentation() const;

private:
	~Grid();
	Grid(VoxelGrid*);
	
	Grid(const Grid&);
	Grid& operator=(const Grid&);

	VoxelGrid* m_InternalGrid;
};

}

