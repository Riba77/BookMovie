#include <cstdint>
#include <string>
#include <vector>
#include <expected>
#include <map>
#include <set>

struct SMovieParams {
  const std::string Title;
  const std::string Description;
};

enum class SeatStatus { AVAILABLE, SOLD };

// Represents a movie playing at a specific time in a theater
struct SShowParams {
  const int32_t Id;
  const std::string MovieId;
  const std::string Time;
  const std::string TheaterId;
};

class CShow {
 public:
  CShow(const SShowParams &showTime, size_t nseats);
  std::expected<void, std::string> BuySeats(std::vector<std::string> seats);
  std::vector<std::string> AvailableSeats() const;

  const SShowParams ShowParams;
  const size_t nSeats;

 private:
  std::map<std::string, SeatStatus> Seats;  // Map of seat name to status
};

class CTheater {
 public:
  CTheater(const std::string &id, size_t nseats);

  std::expected<void, std::string> AddShow(const SShowParams &showtime);

  std::expected<std::reference_wrapper<CShow>, std::string> GetShow(
      const uint32_t showId);
  std::expected<std::reference_wrapper<const CShow>, std::string> GetCShow(
      const uint32_t showId) const;

  const std::string Id;
  const size_t nSeats = 0;

 private:
  std::map<int32_t, CShow> Shows;
};

class Cinemaplex {
 public:
  Cinemaplex(const std::string &name);
  const std::string name = "";

  std::string AddMovie(const SMovieParams &movieParams);
  std::string CreateTheater(const std::string &showName, size_t nseats);

  std::expected<int32_t, std::string> AddShow(const std::string &theaterId,
                                              const std::string &movieId,
                                              const std::string &time);
  std::vector<std::string> ListMovies() const;
  std::vector<std::string> ListTheaters(const std::string &movieId) const;
  std::vector<int32_t> ListShows(const std::string &movieId) const;

  std::expected<SMovieParams, std::string> GetMovieDetails(
      const std::string &movieId) const;

  std::expected<std::vector<std::string>, std::string> GetAvailableSeats(
      const int32_t showId) const;

  std::expected<SShowParams, std::string> GetShowParams(
      const int32_t showId) const;

  std::expected<void, std::string> BuyTickets(const int32_t showId,
                                              std::vector<std::string> seats);

 private:
  std::map<std::string, CTheater> Theaters;
  std::map<std::string, SMovieParams> Movies;
  std::multimap<std::string, int32_t> Shows;
  std::map<int32_t, SShowParams> ShowParams;
  int32_t ShowIdCounter = 0;
  const int32_t MaxShowId = 100;
};
