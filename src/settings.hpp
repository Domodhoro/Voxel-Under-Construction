#ifndef SETTINGS_HPP
#define SETTINGS_HPP

namespace settings {

constexpr auto WINDOW_WIDTH       {1200};
constexpr auto WINDOW_HEIGHT      {600};
constexpr auto WINDOW_TITLE       {"Voxel-Engine"};
constexpr auto FPS                {60};
constexpr auto CHUNK_SIZE_X       {16};
constexpr auto CHUNK_SIZE_Y       {64};
constexpr auto CHUNK_SIZE_Z       {CHUNK_SIZE_X};
constexpr auto ASPECT             {static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT)};
constexpr auto CAMERA_SENSITIVITY {0.1f};
constexpr auto CAMERA_SPEED       {0.5f};
constexpr auto CAMERA_FOV         {60.0f};

}

#endif
