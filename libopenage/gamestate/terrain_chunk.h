// Copyright 2018-2024 the openage authors. See copying.md for legal info.

#pragma once

#include <vector>

#include "coord/tile.h"
#include "gamestate/terrain_tile.h"
#include "renderer/stages/terrain/render_entity.h"
#include "time/time.h"
#include "util/vector.h"


namespace openage::gamestate {

const size_t MAX_CHUNK_WIDTH = 16;
const size_t MAX_CHUNK_HEIGHT = 16;


/**
 * Subdivision of the main terrain entity.
 */
class TerrainChunk {
public:
	TerrainChunk(const util::Vector2s size,
	             const coord::tile_delta offset,
	             const std::vector<TerrainTile> &&tiles);
	~TerrainChunk() = default;

	/**
	 * Set the current render entity of the terrain.
	 *
	 * @param entity New render entity.
	 */
	void set_render_entity(const std::shared_ptr<renderer::terrain::RenderEntity> &entity);

	/**
	 * Get the size of this terrain chunk.
	 *
	 * @return Size of the terrain chunk (in tiles).
	 */
	const util::Vector2s &get_size() const;

	/**
	 * Get the offset of this terrain chunk to the terrain origin.
	 *
	 * @return Offset of the terrain chunk (in tiles).
	 */
	const coord::tile_delta &get_offset() const;

	/**
	 * Update the render entity.
	 *
	 * @param time Simulation time of the update.
	 */
	void render_update(const time::time_t &time);

private:
	/**
	 * Size of the terrain chunk.
	 * Origin is the left corner.
	 * x = top left edge; y = top right edge.
	 */
	util::Vector2s size;

	/**
	 * Offset of the terrain chunk to the origin.
	 */
	coord::tile_delta offset;

	/**
	 * Terrain tile info of the terrain chunk.
	 *
	 * Layout is row-major.
	 */
	std::vector<TerrainTile> tiles;

	/**
	 * Render entity for pushing updates to the renderer. Can be \p nullptr.
	 */
	std::shared_ptr<renderer::terrain::RenderEntity> render_entity;
};

} // namespace openage::gamestate
