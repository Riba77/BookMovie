#include <cstdint>
#include <string>
#include <vector>
#include <expected>
#include <map>
#include <set>

class CMovieDetails {
 public:
  CMovieDetails(const std::string &title, const std::string &description);

  const std::string Title;
  const std::string Description;
};

enum class SeatStatus { AVAILABLE, SOLD };

// Represents a movie playing at a specific time in a theater
class CShowTime {
 public:
  CShowTime(const int32_t &showId, const std::string &movieId,
            const std::string &time, const std::string &theaterId)
      : Id(showId), MovieId(movieId), Time(time), TheaterId(theaterId) {}
  const int32_t Id;
  const std::string MovieId;
  const std::string Time;
  const std::string TheaterId;
};

class CShow {
 public:
  CShow(const CShowTime &showTime, size_t nseats);
  std::expected<void, std::string> BuySeats(std::vector<std::string> seats);
  std::vector<std::string> AvailableSeats() const;

  const CShowTime ShowTime;
  const size_t nSeats;

 private:
  std::map<std::string, SeatStatus> Seats;  // Map of seat name to status
};

class CTheater {
 public:
  CTheater(const std::string &id, size_t a_nseats);

  std::expected<void, std::string> addShow(const CShowTime &showtime);

  std::vector<int32_t> listShowTimes() const;

  std::expected<CShowTime, std::string> getShowTimes(
      const int32_t &showId) const;

  std::expected<std::reference_wrapper<CShow>, std::string> getShow(
      const int32_t &showId);
  std::expected<std::reference_wrapper<const CShow>, std::string> getCShow(
      const int32_t &showId) const;

  const std::string Id;
  const size_t nSeats = 0;

 private:
  std::map<int32_t, CShow> Shows;
};
