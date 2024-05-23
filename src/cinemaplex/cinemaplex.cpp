
#include "cinemaplex.h"
#include <stdexcept>

CShow::CShow(const SShowParams &showParams, size_t nseats)
    : ShowParams(showParams), nSeats(nseats) {
  // calculate number of digits in nseats
  size_t ts = nseats + 1;
  size_t digits = 0;
  while (ts) {
    ts /= 10;
    digits++;
  }
  for (size_t i = 0; i < nseats; i++) {
    const auto seat_name =
        std::to_string(i + 1);  // seat name is just the seat number
    // pad with zeros
    // this is to make sure that seats are sorted in the numerical order
    std::string padded_seat_name = std::string(digits - seat_name.size(), '0') +
                                   seat_name;  // pad with zeros
    this->Seats[padded_seat_name] = SeatStatus::AVAILABLE;
  }
}

std::expected<void, std::string> CShow::BuySeats(
    std::vector<std::string> seats) {
  // check all seats are available
  std::set<std::string> uniqueSeats;
  for (const auto &seat : seats) {
    if (this->Seats.find(seat) == this->Seats.end()) {
      return std::unexpected("Seat " + seat + " does not exist");
    }
    if (this->Seats[seat] == SeatStatus::SOLD) {
      return std::unexpected("Seat " + seat + " is already sold");
    }
    if (uniqueSeats.find(seat) != uniqueSeats.end()) {
      return std::unexpected("Seat " + seat + " is duplicated");
    }
    uniqueSeats.insert(seat);
  }
  // make dedicated seats marked as sold
  for (auto &seat : seats) {
    this->Seats[seat] = SeatStatus::SOLD;
  }
  return std::expected<void, std::string>();
}

std::vector<std::string> CShow::AvailableSeats() const {
  std::vector<std::string> availableSeats;
  for (auto const &[seat, status] : this->Seats) {
    if (status == SeatStatus::AVAILABLE) {
      availableSeats.push_back(seat);
    }
  }
  return availableSeats;
}

CTheater::CTheater(const std::string &id, size_t nseats)
    : Id(id), nSeats(nseats) {}

std::expected<void, std::string> CTheater::AddShow(
    const SShowParams &showParam) {
  if (this->Shows.find(showParam.Id) != this->Shows.end()) {
    return std::unexpected("Show with id " + std::to_string(showParam.Id) +
                           " already exists");  // Return error message as
                                                // string in case of failure
  }
  this->Shows.emplace(showParam.Id, CShow(showParam, this->nSeats));
  return std::expected<void, std::string>();  // Return void in case of success
}

std::expected<std::reference_wrapper<CShow>, std::string> CTheater::GetShow(
    const uint32_t showId) {
  const auto show = this->Shows.find(showId);
  if (this->Shows.find(showId) == this->Shows.end()) {
    return std::unexpected("Show with id " + std::to_string(showId) +
                           " does not exist");  // Return error message as
                                                // string in case of failure
  }
  return std::expected<std::reference_wrapper<CShow>, std::string>(
      show->second);  // Return reference to show in case of
                      // success
}

std::expected<std::reference_wrapper<const CShow>, std::string>
CTheater::GetCShow(const uint32_t showId) const {
  const auto show = this->Shows.find(showId);
  if (show == this->Shows.end()) {
    return std::unexpected("Show with ID " + std::to_string(showId) +
                           " not found.)");
  }
  return std::expected<std::reference_wrapper<const CShow>, std::string>(
      show->second);
}

/*
 * CCinemaplex
 */

CCinemaplex::CCinemaplex(const std::string &name) : name(name) {}

std::string CCinemaplex::AddMovie(const SMovieParams &movieParams) {
  std::string movieId(movieParams.Title);

  if (Movies.find(movieId) != Movies.end()) {
    throw std::runtime_error("Movie with ID " + movieId + " not found.");
  }
  Movies.emplace(movieId, movieParams);
  return movieId;
}
std::string CCinemaplex::CreateTheater(const std::string &theaterId,
                                       size_t nseats) {
  if (Theaters.find(theaterId) != Theaters.end()) {
    throw std::runtime_error("Theater with ID " + theaterId +
                             " already exist.");
  }
  Theaters.emplace(theaterId, CTheater(theaterId, nseats));
  return theaterId;
}

std::expected<int32_t, std::string> CCinemaplex::AddShow(
    const std::string &theaterId, const std::string &movieId,
    const STime &time) {
  const auto theater = Theaters.find(theaterId);
  if (theater == Theaters.end()) {
    return std::unexpected("Theater with ID " + theaterId + " not found.");
  }
  const auto movie = Movies.find(movieId);
  if (movie == Movies.end()) {
    return std::unexpected("Movie with ID " + movieId + " not found.");
  }
  if (ShowIdCounter > MaxShowId) {
    return std::unexpected("Maximum number of shows reached.");
  }
  ShowIdCounter++;
  auto shid = ShowIdCounter;
  SShowParams showParams(shid, movieId, time, theaterId);
  return theater->second.AddShow(showParams)
      .and_then([this, &shid, &movieId, &showParams]() {
        Shows.insert({movieId, shid});
        ShowParams.insert({shid, showParams});
        return std::expected<int32_t, std::string>(shid);
      });
}

std::vector<std::string> CCinemaplex::ListMovies() const {
  std::vector<std::string> movieIds;
  for (auto const &[mid, _] : Movies) {
    movieIds.push_back(mid);
  }
  return movieIds;
}

std::vector<std::string> CCinemaplex::ListTheaters() const {
  std::vector<std::string> theaterIds;
  for (auto const &[tid, _] : Theaters) {
    theaterIds.push_back(tid);
  }
  return theaterIds;
}

std::vector<int32_t> CCinemaplex::ListShows(const std::string &movieId) const {
  std::vector<int32_t> showIds;
  for (auto const &[mid, sid] : Shows) {
    if (mid == movieId) {
      showIds.push_back(sid);
    }
  }
  return showIds;
}

std::expected<SMovieParams, std::string> CCinemaplex::GetMovieDetails(
    const std::string &movieId) const {
  if (Movies.find(movieId) == Movies.end()) {
    return std::unexpected("Movie does not exist");
  }
  return std::expected<SMovieParams, std::string>(Movies.at(movieId));
}

std::expected<std::vector<std::string>, std::string>
CCinemaplex::GetAvailableSeats(const int32_t showId) const {
  const auto show = ShowParams.find(showId);
  if (show == ShowParams.end()) {
    return std::unexpected("Show with ID " + std::to_string(showId) +
                           " not found)");
  }

  const auto theater = Theaters.find(show->second.TheaterId);
  if (theater == Theaters.end()) {
    return std::unexpected("Theater does not exist");
  }

  return theater->second.GetCShow(showId).transform(
      [](const CShow &show) { return show.AvailableSeats(); });
}

std::expected<SShowParams, std::string> CCinemaplex::GetShowParams(
    const int32_t showId) const {
  const auto show = ShowParams.find(showId);
  if (show == ShowParams.end()) {
    return std::unexpected("Show with ID " + std::to_string(showId) +
                           " not found)");
  }
  return std::expected<SShowParams, std::string>(show->second);
}

std::expected<void, std::string> CCinemaplex::BuyTickets(
    const int32_t showId, std::vector<std::string> seats) {
  std::lock_guard<std::mutex> lock(Mutex);  // Lock the mutex
  const auto show = ShowParams.find(showId);
  if (show == ShowParams.end()) {
    return std::unexpected("Show with ID " + std::to_string(showId) +
                           " not found)");
  }

  const auto theater = Theaters.find(show->second.TheaterId);
  if (theater == Theaters.end()) {
    return std::unexpected("Theater with ID " + show->second.TheaterId +
                           " not found)");
  }

  return theater->second.GetShow(showId).and_then(
      [&seats](CShow &show) { return show.BuySeats(seats); });
}