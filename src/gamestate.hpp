#pragma once

namespace GameState {

void Init();
void Update(float);

extern bool level[];

extern float player_x, player_y, player_rot;
}  // namespace GameState
