#include "cinemaplex.h"
#include <gtest/gtest.h>

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


