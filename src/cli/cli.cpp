
#include <iostream>
#include "cinemaplex.h"

int main(int argc, char const* argv[]) {
  // Main function
  CTheater theater("01", 10);  // Call the constructor
  theater.AddShow(SShowParams(1, "01", "10:00", "Theater1"));  // Add a show
  theater.AddShow(SShowParams(2, "02", "12:00", "Theater2"));  // Add a show
  theater.AddShow(SShowParams(3, "03", "14:00", "Theater3"));  // Add a show
  std::cout << "Theater ID: " << theater.Id
            << std::endl;            // Print the theater ID
  auto result = theater.GetShow(2);  // Get the show
  if (result.has_value()) {
    CShow& show = result.value();  // Get the show
    std::cout << "Show Time: " << show.ShowParams.Time
              << std::endl;  // Print the show ID
    std::cout << "Movie Id: " << show.ShowParams.MovieId
              << std::endl;  // Print the show time
    std::cout << ""
                 "Theater Id: "
              << show.ShowParams.TheaterId << std::endl;  // Print the movie ID
  } else {
    std::cout << "Show not found" << std::endl;  // Print show not found
  }
  return 0;
}
