#include <string>
#include <vector>
#include <iostream>

#include <DungeonRoyale/Globals.hpp>
#include <DungeonRoyale/Utils.hpp>
#include <DungeonRoyale/Tile.hpp>
#include <DungeonRoyale/Projectile.hpp>
#include <DungeonRoyale/Timer.hpp>
#include <DungeonRoyale/Player2.hpp>
#include <DungeonRoyale/Texture.hpp>
#include <DungeonRoyale/Physics.hpp>
#include <DungeonRoyale/Game.hpp>
#include <DungeonRoyale/Network.hpp>

struct PlayerInputStates {
  bool up = false;
  bool left = false;
  bool down = false;
  bool right = false;
};

int main(int argc, char* argv[]) {
  Game::init();

  SDL_Texture* atlas = Texture::load("./assets/atlas2.png");

  bool quit = false;
  SDL_Event e;

  Player2 player(
    globals.SCREEN_WIDTH / 2.0f - 6,
    globals.SCREEN_HEIGHT / 2.0f - 7.5f,
    24,
    30,
    atlas,
    SDL_Rect { 0, 320, 12, 15 }
  );

  PlayerInputStates inputs;

  Network::startReceiveThread();

  while (!quit) {
    Utils::updateDeltaTime();
    Utils::updateFPS();

    std::string fps = "FPS: " + std::to_string(Utils::getFPS());
    SDL_SetWindowTitle(globals.window, fps.c_str());

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
      quit = true;
    }

    player.running = currentKeyStates[SDL_SCANCODE_D] ||
      currentKeyStates[SDL_SCANCODE_A] ||
      currentKeyStates[SDL_SCANCODE_S] ||
      currentKeyStates[SDL_SCANCODE_W];

    PlayerInputStates inputsNew;

    inputsNew.up = currentKeyStates[SDL_SCANCODE_W];
    inputsNew.left = currentKeyStates[SDL_SCANCODE_A];
    inputsNew.down = currentKeyStates[SDL_SCANCODE_S];
    inputsNew.right = currentKeyStates[SDL_SCANCODE_D];

    PlayerAction action = PlayerAction::IDLE;
    if (!inputs.right && inputsNew.right) {
      action = PlayerAction::MOVE_RIGHT_START;
    }
    else if (inputs.right && !inputsNew.right) {
      action = PlayerAction::MOVE_RIGHT_END;
    } else if (!inputs.left && inputsNew.left) {
      action = PlayerAction::MOVE_LEFT_START;
    }
    else if (inputs.left && !inputsNew.left) {
      action = PlayerAction::MOVE_LEFT_END;
    } else if (!inputs.up && inputsNew.up) {
      action = PlayerAction::MOVE_UP_START;
    }
    else if (inputs.up && !inputsNew.up) {
      action = PlayerAction::MOVE_UP_END;
    } else if (!inputs.down && inputsNew.down) {
      action = PlayerAction::MOVE_DOWN_START;
    }
    else if (inputs.down && !inputsNew.down) {
      action = PlayerAction::MOVE_DOWN_END;
    }

    if (action != PlayerAction::IDLE) {
      PlayerPacket packet = PlayerPacket{ action };
      Network::sendToServer(packet);

      std::cout << "sent packet" << "\n";
    }

    inputs.up = inputsNew.up;
    inputs.left = inputsNew.left;
    inputs.down = inputsNew.down;
    inputs.right = inputsNew.right;

    player.tick();

   /* player.x += (currentKeyStates[SDL_SCANCODE_D] - currentKeyStates[SDL_SCANCODE_A]) * player.speed * Utils::getDeltaTime();
    player.y += (currentKeyStates[SDL_SCANCODE_S] - currentKeyStates[SDL_SCANCODE_W]) * player.speed * Utils::getDeltaTime();*/

    player.x = globals.playerPos.x;
    player.y = globals.playerPos.y;


    //globals.camera.x = player.rect.x;
    //globals.camera.y = player.rect.y;

    player.flipped = currentKeyStates[SDL_SCANCODE_A];

    SDL_SetRenderDrawColor(globals.renderer, 0x12, 0x12, 0x1b, 255);
    SDL_RenderClear(globals.renderer);
    SDL_SetRenderDrawColor(globals.renderer, 0, 0, 0, 255);

    player.draw();

    SDL_RenderPresent(globals.renderer);
  }

  Network::stopReceiveThread();

  Game::cleanUp(atlas);

  return 0;
}
