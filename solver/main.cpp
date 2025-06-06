#define CROW_MAIN

#include "board/board_fast.hpp"
#include "bot1.cpp"
#include "bot2.cpp"
#include "bot3.cpp"
#include "crow_all.h"

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/health").methods(crow::HTTPMethod::GET)([]() {
    return crow::response(200, "OK");
  });

  CROW_ROUTE(app, "/calculate")
      .methods(crow::HTTPMethod::GET)([&](const crow::request& req) {
        const char* moves_param = req.url_params.get("moves");
        const char* bot_level_param = req.url_params.get("bot");
        std::string moves = (moves_param != nullptr) ? moves_param : "";
        std::string bot_level =
            (bot_level_param != nullptr) ? bot_level_param : "3";
        if (bot_level == "1") {
          return Bot1Move(moves);
        } else if (bot_level == "2") {
          return Bot2Move(moves);
        } else {
          return Bot3Move(moves);
        }
      });

  app.port(18080).multithreaded().run();
  return 0;
}
