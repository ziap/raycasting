#pragma once

namespace GameState {

void Init();
void Update(float);

extern int level[];

extern float player_x, player_y, player_rot_sin, player_rot_cos, player_pitch;
}  // namespace GameState
