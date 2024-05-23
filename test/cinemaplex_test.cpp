#include "cinemaplex.h"
#include <gtest/gtest.h>

// Not all tests added.

TEST(CCinemaplexTest, CreateTheater_Success) {
  CCinemaplex cineplex("Cinemaplex company");
  std::string theaterId = "Theater1";
  size_t nseats = 100;

  EXPECT_NO_THROW({
    std::string result = cineplex.CreateTheater(theaterId, nseats);
    EXPECT_EQ(result, theaterId);
  });
}

TEST(CCinemaplexTest, CreateTheater_AlreadyExists) {
  CCinemaplex cineplex("Cinemaplex company");
  std::string theaterId = "Theater1";
  size_t nseats = 100;

  cineplex.CreateTheater(theaterId, nseats);  // Create theater first time

  EXPECT_THROW(
      {
        cineplex.CreateTheater(theaterId,
                               nseats);  // Attempt to create theater again
      },
      std::runtime_error);
}

TEST(CCinemaplexTest, AddShow_Success) {
  CCinemaplex cineplex("Cinemaplex company");
  std::string theaterId = "Theater1";
  std::string movieId = "Movie1";
  STime time;  // Assume STime is properly defined and instantiated

  cineplex.CreateTheater(theaterId,
                         100);  // Assuming CreateTheater method exists
  cineplex.AddMovie(
      SMovieParams(movieId,
                   "Sample Movie"));  // Assuming AddMovie method exists

  auto result = cineplex.AddShow(theaterId, movieId, time);

  EXPECT_TRUE(result);
  EXPECT_EQ(
      result.value(),
      1);  // Assuming this is the first show and ShowIdCounter starts at 0
}

TEST(CCinemaplexTest, AddShow_TheaterNotFound) {
  CCinemaplex cineplex("Cinemaplex company");
  std::string theaterId = "NonExistentTheater";
  std::string movieId = "Movie1";
  STime time;

  cineplex.AddMovie(
      SMovieParams(movieId,
                   "Sample Movie"));  // Assuming AddMovie method exists

  auto result = cineplex.AddShow(theaterId, movieId, time);

  EXPECT_FALSE(result);
  EXPECT_EQ(result.error(), "Theater with ID " + theaterId + " not found.");
}

TEST(CCinemaplexTest, AddShow_MovieNotFound) {
  CCinemaplex cineplex("Cinemaplex company");
  std::string theaterId = "Theater1";
  std::string movieId = "NonExistentMovie";
  STime time;

  cineplex.CreateTheater(theaterId,
                         100);  // Assuming CreateTheater method exists

  auto result = cineplex.AddShow(theaterId, movieId, time);

  EXPECT_EQ(result.error(), "Movie with ID " + movieId + " not found.");
}

TEST(CinemaplexTest, BuyTickets1) {
  CCinemaplex cinemaplex{"Cinemaplex limited company"};
  const size_t nseats = 20;
  // create theater
  auto theater = cinemaplex.CreateTheater("Best Cinema", nseats);

  // create movie
  const std::string movieTitle = "Fast and Forious";
  const std::string movieDescription = "Fast and Forious description.";

  auto movie1 = cinemaplex.AddMovie(SMovieParams(movieTitle, movieDescription));

  // add show
  auto show = cinemaplex.AddShow(theater, movie1, STime(12, 0));

  // get movies
  auto movies = cinemaplex.ListMovies();
  EXPECT_EQ(movies.size(), 1);

  // check movie details
  auto movieDetails = cinemaplex.GetMovieDetails(movie1);
  EXPECT_EQ(movieDetails.value().Title, movieTitle);
  EXPECT_EQ(movieDetails.value().Description, movieDescription);

  // get theaters for movie
  auto theaters = cinemaplex.ListTheaters();
  EXPECT_EQ(theaters.size(), 1);

  // get shows for movie
  auto shows = cinemaplex.ListShows(movie1);
  EXPECT_EQ(shows.size(), 1);

  // get available seats
  auto availableSeats = cinemaplex.GetAvailableSeats(show.value());
  // check available seats
  EXPECT_EQ(availableSeats.value().size(), nseats);

  // buy tickets
  std::vector<std::string> seats1{"02", "03", "05"};
  auto buyTickets = cinemaplex.BuyTickets(show.value(), seats1);
  EXPECT_TRUE(buyTickets);

  // get available seats
  availableSeats = cinemaplex.GetAvailableSeats(show.value());
  EXPECT_EQ(availableSeats.value().size(), nseats - seats1.size());

  // try to buy reserved seats
  std::vector<std::string> seats2{"03", "04"};
  buyTickets = cinemaplex.BuyTickets(show.value(), seats2);
  EXPECT_FALSE(buyTickets);

  // get available seats
  availableSeats = cinemaplex.GetAvailableSeats(show.value());
  EXPECT_TRUE(availableSeats);
  EXPECT_EQ(availableSeats.value().size(), nseats - seats1.size());

  // buy more tickets
  std::vector<std::string> seats3{"01", "04"};
  buyTickets = cinemaplex.BuyTickets(show.value(), seats3);

  // get remaining available seats
  availableSeats = cinemaplex.GetAvailableSeats(show.value());
  EXPECT_EQ(availableSeats.value().size(),
            nseats - seats1.size() - seats3.size());
}
