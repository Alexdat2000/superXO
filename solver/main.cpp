#define CROW_MAIN

#include <iostream>

#include "board_fast.hpp"
#include "crow_all.h"
#include "monte_carlo_tree_simple.cpp"

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/calculate")
      .methods(crow::HTTPMethod::GET)([&](const crow::request& req) {
        const char* paramValue = req.url_params.get("moves");
        std::string moves = (paramValue != nullptr) ? paramValue : "";
        auto b = BoardFast(moves);
      return GetBestMove(b, 10'000);
      });

  app.port(18080).multithreaded().run();
  return 0;
}
