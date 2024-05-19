
#include "cinemaplex.h"

CMovieDetails::CMovieDetails(const std::string &title,
                             const std::string &description)
    : Title(title), Description(description) {}

CShow::CShow(const CShowTime &showtime, size_t nseats)
    : ShowTime(showtime), nSeats(nseats) {
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
  std::set<std::string> UniqueSeats;
  for (const auto &seat : seats) {
    if (this->Seats.find(seat) == this->Seats.end()) {
      return std::unexpected("Seat " + seat + " does not exist");
    }
    if (this->Seats[seat] == SeatStatus::SOLD) {
      return std::unexpected("Seat " + seat + " is already sold");
    }
    if (UniqueSeats.find(seat) != UniqueSeats.end()) {
      return std::unexpected("Seat " + seat + " is duplicated");
    }
    UniqueSeats.insert(seat);
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
