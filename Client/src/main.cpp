#include <string>
#include <vector>

#include <DungeonRoyale/Globals.hpp>
#include <DungeonRoyale/Utils.hpp>
#include <DungeonRoyale/Tile.hpp>
#include <DungeonRoyale/Projectile.hpp>
#include <DungeonRoyale/Timer.hpp>
#include <DungeonRoyale/Player.hpp>
#include <DungeonRoyale/Texture.hpp>
#include <DungeonRoyale/Physics.hpp>
#include <DungeonRoyale/Game.hpp>

int main(int argc, char* argv[]) {
  Game::init();

  SDL_Texture* atlas = Texture::load("./assets/atlas2.png");

  bool quit = false;
  SDL_Event e;

  Player player(
    SDL_Rect {
      (int)(globals.SCREEN_WIDTH / 2.0f - 6),
      (int)(globals.SCREEN_HEIGHT / 2.0f - 7.5f),
      24,
      30
    },
    atlas,
    SDL_Rect { 0, 320, 12, 15 }
  );

 std::vector<Tile> tiles;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      tiles.push_back(Tile{ i + 7, j + 5, i == 0 || j == 0 || i == 9 || j == 9 });
    }
  }

  std::vector<Projectile> projectiles;

  while (!quit) {
    Utils::updateDeltaTime();

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x / globals.GRID_SIZE;
        int y = e.button.y / globals.GRID_SIZE;

        if (e.button.button == SDL_BUTTON_LEFT) {
          bool found = false;

          for (auto tile : tiles) {
            if (tile.x == x && tile.y == y && tile.type == 1)
            {
              found = true;
              break;
            }
          }

          if (!found) {
            tiles.push_back(Tile { x, y, 1 });
          }
        } else if (e.button.button == SDL_BUTTON_RIGHT) {
          for (int i = 0; i < tiles.size(); i++) {
            Tile& tile = tiles[i];
            if (tile.x == x && tile.y == y && tile.type == 1)
            {
              tiles.erase(tiles.begin() + i);

              break;
            }
          }
        }
      }

      player.tick();

      if (currentKeyStates[SDL_SCANCODE_SPACE]) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int endx = mouseX;
        int endy = mouseY;

        int startx = player.rect.x;
        int starty = player.rect.y;

        float vx = endx - startx;
        float vy = endy - starty;

        float length = sqrt(vx * vx + vy * vy);
        if (length > 0) {
          vx /= length;
          vy /= length;
        }

        projectiles.push_back(Projectile{ (float)startx, (float)starty, vx, vy });
      }
    }

    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
      quit = true;
    }

    player.running = currentKeyStates[SDL_SCANCODE_D] ||
      currentKeyStates[SDL_SCANCODE_A] ||
      currentKeyStates[SDL_SCANCODE_S] ||
      currentKeyStates[SDL_SCANCODE_W];

    int oldX = player.rect.x;
    int oldY = player.rect.y;

    player.rect.x += (currentKeyStates[SDL_SCANCODE_D] - currentKeyStates[SDL_SCANCODE_A]) * player.speed * Utils::getDeltaTime();
    player.rect.y += (currentKeyStates[SDL_SCANCODE_S] - currentKeyStates[SDL_SCANCODE_W]) * player.speed * Utils::getDeltaTime();

    for (auto& tile : tiles) {
      tile.colliding = false;

      if (tile.type == 1 &&
        doAABBsIntersect(
          player.rect.x,
          player.rect.y,
          player.rect.w,
          player.rect.h,
          tile.x * globals.GRID_SIZE,
          tile.y *globals.GRID_SIZE,
          32,
          32
        )
       )
      {
        float aMidX = player.rect.x + player.rect.w / 2;
        float aMidY = player.rect.y + player.rect.h / 2;
        float bMidX = tile.x * globals.GRID_SIZE + 32 / 2;
        float bMidY = tile.y * globals.GRID_SIZE + 32 / 2;

        float dx = aMidX - bMidX;
        float dy = aMidY - bMidY;

        float overlapX = (player.rect.w / 2 + 32 / 2) - abs(dx);
        float overlapY = (player.rect.h / 2 + 32 / 2) - abs(dy);

        if (overlapX < overlapY) {
          if (dx > 0) {
            player.rect.x += overlapX;
          }
          else {
            player.rect.x -= overlapX;
          }
        }
        else {
          if (dy > 0) {
            player.rect.y += overlapY;
          }
          else {
            player.rect.y -= overlapY;
          }
        }
      }

      for (auto& projectile : projectiles) {
        if (tile.type == 1 &&
          doAABBsIntersect(
            projectile.x,
            projectile.y,
            12,
            12,
            tile.x * globals.GRID_SIZE,
            tile.y * globals.GRID_SIZE,
            32,
            32
           )
          )
        {
          float aMidX = projectile.x + 6 / 2;
          float aMidY = projectile.y + 6 / 2;
          float bMidX = tile.x * globals.GRID_SIZE + 32 / 2;
          float bMidY = tile.y * globals.GRID_SIZE + 32 / 2;

          float dx = aMidX - bMidX;
          float dy = aMidY - bMidY;

          float overlapX = (6 / 2 + 32 / 2) - abs(dx);
          float overlapY = (6 / 2 + 32 / 2) - abs(dy);

          if (overlapX < overlapY) {
            projectile.vx *= -1;
          }
          else {
            projectile.vy *= -1;
          }
        }
      }
    }

    for (int i = 0; i < projectiles.size(); i++) {
      auto& projectile = projectiles.at(i);
      projectile.x += projectile.vx * projectile.speed * Utils::getDeltaTime();
      projectile.y += projectile.vy * projectile.speed * Utils::getDeltaTime();

      if (projectile.x + 6 > globals.SCREEN_WIDTH || projectile.x < 0) {
        projectile.vx *= -1;
      }
      else if (projectile.y + 6 > globals.SCREEN_HEIGHT || projectile.y < 0) {
        projectile.vy *= -1;
      }

      projectile.scale -= 0.1f * Utils::getDeltaTime();

      if (projectile.scale < 0) {
        projectiles.erase(projectiles.begin() + i);
      }
    }

    globals.camera.x = player.rect.x;
    globals.camera.y = player.rect.y;

    Utils::updateFPS();
    std::string fps = "FPS: " + std::to_string(Utils::getFPS());
    SDL_SetWindowTitle(globals.window, fps.c_str());


    player.flipped = currentKeyStates[SDL_SCANCODE_A];

    SDL_SetRenderDrawColor(globals.renderer, 0x12, 0x12, 0x1b, 255);
    SDL_RenderClear(globals.renderer);
    SDL_SetRenderDrawColor(globals.renderer, 0, 0, 0, 255);

    if (globals.DRAW_GRID) {
      for (int x = 0; x < 800; x += globals.GRID_SIZE)
      {
        SDL_RenderDrawLine(globals.renderer, x, 0, x, 600);
      }

      for (int y = 0; y < 600; y += globals.GRID_SIZE)
      {
        SDL_RenderDrawLine(globals.renderer, 0, y, 800, y);
      }
    }

    for (auto tile : tiles) {
      SDL_Rect srcRect = { 16, 0, 16, 16 };
      if (tile.type == 1) {
        srcRect.x = 0;
        srcRect.y = 16;
      }
      SDL_Rect destRect = {
        tile.x * globals.GRID_SIZE - globals.camera.x + globals.SCREEN_WIDTH / 2 - 16,
        tile.y * globals.GRID_SIZE - globals.camera.y + globals.SCREEN_HEIGHT / 2 - 16,
        32,
        32
      };
      SDL_RenderCopy(globals.renderer, atlas, &srcRect, &destRect);
    }

    for (auto tile : tiles) {
      if (tile.colliding) {
        SDL_SetRenderDrawColor(globals.renderer, 0, 255, 255, 255);
        SDL_RenderDrawLine(globals.renderer, tile.x * globals.GRID_SIZE, tile.y * globals.GRID_SIZE, tile.x * globals.GRID_SIZE + 32, tile.y * globals.GRID_SIZE);
        SDL_RenderDrawLine(globals.renderer, tile.x * globals.GRID_SIZE + 32, tile.y * globals.GRID_SIZE, tile.x * globals.GRID_SIZE + 32, tile.y * globals.GRID_SIZE + 32);
        SDL_RenderDrawLine(globals.renderer, tile.x * globals.GRID_SIZE + 32, tile.y * globals.GRID_SIZE + 32, tile.x* globals.GRID_SIZE, tile.y * globals.GRID_SIZE + 32);
        SDL_RenderDrawLine(globals.renderer, tile.x * globals.GRID_SIZE, tile.y * globals.GRID_SIZE, tile.x * globals.GRID_SIZE, tile.y * globals.GRID_SIZE + 32);
      }
    }

    for (const auto& projectile : projectiles) {
      SDL_Rect srcRect = { 355, 307, 6, 6 };
      SDL_Rect destRect = { projectile.x, projectile.y, 12 * projectile.scale, 12 * projectile.scale };
      SDL_RenderCopy(globals.renderer, atlas, &srcRect, &destRect);
    }

    player.draw();

    SDL_RenderPresent(globals.renderer);
  }

  Game::cleanUp(atlas);

  return 0;
}
