#ifndef SETTINGS_HPP
#define SETTINGS_HPP

namespace settings {

auto WORLD_SEED         {1007};
auto WINDOW_WIDTH       {1200};
auto WINDOW_HEIGHT      {600};
auto WINDOW_TITLE       {"Voxel-Engine"};
auto CAMERA_SPEED       {0.5f};
auto CAMERA_FOV         {60.0f};
auto CAMERA_SENSITIVITY {0.1f};

constexpr auto FPS          {60};
constexpr auto NOISE_MAX    {3};
constexpr auto CHUNK_SIZE_X {16};
constexpr auto CHUNK_SIZE_Y {64};
constexpr auto CHUNK_SIZE_Z {CHUNK_SIZE_X};

}

#endif
