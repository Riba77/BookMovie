
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "cinemaplex.h"

void printMenu() {
  std::cout << "1. List Movies\n";
  std::cout << "2. List Theaters\n";
  std::cout << "3. List Shows\n";
  std::cout << "4. View Available Seats\n";
  std::cout << "5. Book Tickets\n";
  std::cout << "6. Exit\n";
  std::cout << "Choose an option: ";
}

int main(int argc, char const* argv[]) {
  // Make some movies
  CCinemaplex cinemaplex("Cinemaplex company");
  std::string movie1, movie2, movie3;
  std::string theater1, theater2, theater3;

  try {
    // Add some movies
    movie1 = cinemaplex.AddMovie(SMovieParams(
        "Fast and Forious", "Description of the movie Fast and Forious"));
    movie2 = cinemaplex.AddMovie(
        SMovieParams("Matrix", "Description of the movie Matrix"));
    movie3 = cinemaplex.AddMovie(SMovieParams(
        "The Intouchables", "Description of the movie The Intouchebles"));
  } catch (const std::exception& e) {
    std::cerr << "Error adding some movies: " << e.what() << std::endl;
  }
  try {
    // Make some theaters
    theater1 = cinemaplex.CreateTheater("Ljubljana Cinema", 20);
    theater2 = cinemaplex.CreateTheater("Maribor Cinema", 15);
    theater3 = cinemaplex.CreateTheater("Celje Cinema", 10);
  } catch (const std::exception& e) {
    std::cerr << "Error creating some theaters: " << e.what() << std::endl;
  }

  // Add some shows
  auto show1 = cinemaplex.AddShow(theater1, movie1, STime({12, 0}));
  auto show2 = cinemaplex.AddShow(theater1, movie1, STime({16, 45}));
  auto show3 = cinemaplex.AddShow(theater2, movie2, STime({13, 30}));
  auto show4 = cinemaplex.AddShow(theater2, movie3, STime({18, 45}));
  auto show5 = cinemaplex.AddShow(theater3, movie2, STime({9, 0}));
  auto show6 = cinemaplex.AddShow(theater3, movie1, STime({19, 0}));
  auto show7 = cinemaplex.AddShow(theater3, movie3, STime({15, 0}));

  // Run the CLI application
  int choice;
  std::string movieId, theaterId;
  std::vector<std::string> seats;
  do {
    printMenu();
    std::cin >> choice;
    switch (choice) {
      case 1: {
        auto movies = cinemaplex.ListMovies();
        std::cout << "Movies:\n";
        for (uint32_t i = 0; i < movies.size(); i++) {
          std::cout << i << ". " << movies[i] << std::endl;
        }
        std::cout << std::endl;
        break;
      }
      case 2: {
        std::cout << "Enter movie ID to list theaters: ";
        auto theaters = cinemaplex.ListTheaters();
        std::cout << "Theaters:\n";
        for (const auto& tid : theaters) {
          std::cout << tid << "\n";
        }
        std::cout << std::endl;
        break;
      }
      case 3: {
        auto movies = cinemaplex.ListMovies();
        for (uint32_t i = 0; i < movies.size(); i++) {
          std::cout << i << ". " << movies[i] << std::endl;
        }
        std::cout << "Enter number of movie:" << std::endl;
        std::cin >> movieId;
        uint32_t movieIdNum = 0;

        // Check if the input is a number
        try {
          movieIdNum = std::stoi(movieId);
        } catch (const std::exception& e) {
          std::cerr << "Invalid movie ID, please enter correct ID." << e.what()
                    << std::endl;
          break;
        }
        // Check if the input is a valid movie ID
        if (movieIdNum >= movies.size()) {
          std::cerr << "Invalid movie ID, please enter correct ID."
                    << std::endl;
          break;
        }

        auto movie = movies[movieIdNum];
        std::cout << "Shows for movie " << movie << ":\n";
        std::vector<int32_t> shows = cinemaplex.ListShows(movie);
        if (shows.empty()) {
          std::cout << "No shows available for this movie." << std::endl;
          break;
        }

        for (auto show : shows) {
          auto show_e = cinemaplex.GetShowParams(show);
          if (!show_e) {
            std::cerr << "Error: " << show_e.error() << std::endl;
            break;
          }
          std::cout << " " << show << " : " << show_e.value().TheaterId
                    << " at " << std::setw(2) << std::setfill('0')
                    << show_e.value().Time.Hour << "h" << ":" << std::setw(2)
                    << std::setfill('0') << show_e.value().Time.Minute << "m."
                    << std::endl;

          std::cout << std::endl;
        }
        break;
      }
      case 4: {
        std::string showId;
        std::cout << "Enter show ID to view available seats: ";
        std::cin >> showId;

        uint32_t showIdNum = 0;
        // Check if the input is a number
        try {
          showIdNum = std::stoi(showId);
        } catch (const std::exception& e) {
          std::cerr << "Invalid show ID, please enter correct ID." << e.what()
                    << std::endl;
          break;
        }

        auto seats = cinemaplex.GetAvailableSeats(showIdNum);
        if (seats) {
          std::cout << "Available seats:\n";
          for (const auto& seat : *seats) {
            std::cout << seat << " ";
          }
          std::cout << "\n";
        } else {
          std::cerr << "Error: " << seats.error() << std::endl;
        }
        break;
      }
      case 5: {
        std::string showId;
        uint32_t showIdNum = 0;
        std::cout << "Enter show ID to book tickets: ";
        std::cin >> showId;
        // Check if the input is a number
        try {
          showIdNum = std::stoi(showId);
        } catch (const std::exception& e) {
          std::cerr << "Invalid show ID, please enter correct ID." << e.what()
                    << std::endl;
          break;
        }
        std::string seat;
        std::cout << "Enter seat numbers (type 'done' to finish): ";
        while (true) {
          std::cin >> seat;
          if (seat == "done") break;
          seats.push_back(seat);
        }
        auto result = cinemaplex.BuyTickets(showIdNum, seats);
        if (result) {
          std::cout << "Tickets booked successfully.\n";
        } else {
          std::cerr << "Error: " << result.error() << std::endl;
        }
        seats.clear();  // Clear the seats vector for future use
        break;
      }
      case 6:
        std::cout << "Exiting...\n";
        break;
      default:
        std::cout << "Invalid option. Please try again." << std::endl;
    }
  } while (choice != 6);

  return 0;
}
