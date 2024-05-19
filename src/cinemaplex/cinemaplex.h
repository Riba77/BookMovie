#include <cstdint>
#include <string>

namespace cpx {
class PCtor;  // private constructor tag

class TheaterId {
 public:
  TheaterId(const std::string &id) : id(id) {}
  const std::string id;
};

class Time {
 public:
  Time(const std::string &time) : xTime(time) {}
  const std::string xTime;
};

class MovieId {
 public:
  MovieId(const std::string &title);
  const std::string Id = "";
};

class MovieDetails {
 public:
  MovieDetails(const std::string &title, const std::string &description);

  const std::string Title;
  const std::string Description;
};

enum class SeatStatus { AVAILABLE, SOLD };

}  // namespace cpx