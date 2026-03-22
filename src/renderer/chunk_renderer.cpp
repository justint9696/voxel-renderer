#include "renderer/chunk_renderer.hpp"
#include "logger.hpp"

void ChunkRenderer::remesh(ChunkSection& section) {
    ASSERT(section.blocks.size() > 0);

    // clear out the previous meshes
    section.solid.clear();
    section.transparent.clear();

    size_t n = 0;
    for (auto idx : section.indices) {
        auto block_type = section.blocks.at(idx);
        if (block_type == BlockType::Air)
            continue;

        // only mesh the topmost water block in bodies of water
        auto position = section.position_from_index(idx);
        if (block_type == BlockType::Water
                && position.y < CHUNK_WATER_HEIGHT - 1)
            continue;

        const auto& block =
            BLOCK_TABLE[static_cast<uint32_t>(block_type) - 1];
        auto& mesh = ((block.flags & BLOCK_TRANSPARENT) ?
                       section.transparent : section.solid);

        for (size_t i = 0; i < BLOCK_FACES; i++) {
            auto normal = BLOCK_NORMALS[i];

            // only mesh the top face of water blocks
            if (block_type == BlockType::Water
                    && normal.y < 1.0f)
                continue;

            if (!section.is_visible(position, normal))
                continue;

            auto other = chunk.section_from_position(position + normal);
            if (other && !other->is_visible(position, normal))
                continue;

            auto uv = this->texture.uv_coords(
                static_cast<uint32_t>(
                    (fabs(normal.y) < 1.0f) ? block.front : block.top));

            section.mesh_block_face(
                    mesh, position - section.position,
                    normal, uv, block.opacity, i, n++);
        }
    }

    section.solid.allocate(false);
    section.solid.submit();

    section.transparent.allocate(true);
    section.transparent.submit();

    section.flags &= ~CHUNK_DIRTY;
}

void ChunkRenderer::prepare() {
    // process render queue
    while (!this->chunk.queue.empty()) {
        if (this->cmd++ >= MESH_PER_FRAME)
            break;

        auto idx = this->chunk.queue.front();
        auto& section = this->chunk.sections.at(idx);
        this->chunk.queue.pop_front();

        if (section.flags & CHUNK_REGEN) {
            this->chunk.regenerate(section);
        } else if (section.flags & CHUNK_DIRTY) {
            section.regen_position = section.position;
            this->remesh(section);
        }
    }
}

void ChunkRenderer::render(const Camera& camera) {
    // sort the chunks
    if (this->chunk.flags & CHUNK_SORT) {
        this->chunk.sort(camera);
    }

    auto& shader = this->shader;

    shader.use();
    this->texture.bind();

    shader.set<glm::mat4>("u_view", camera.view);
    shader.set<glm::mat4>("u_projection", camera.projection);
    shader.set<glm::vec4>("u_color", glm::vec4(1.0f));

    for (auto idx : this->chunk.indices) {
        auto& section = this->chunk.sections.at(idx);
        auto model = glm::translate(glm::mat4(1.0f), section.regen_position);
        shader.set<glm::mat4>("u_model", model);

        section.solid.render();
        section.transparent.render();
    }

    this->cmd = 0;
}
