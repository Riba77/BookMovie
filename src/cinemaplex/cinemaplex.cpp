
#include "cinemaplex.h"

namespace cpx {

class PCtor {};  // Private constructor

MovieId::MovieId(const std::string &id) : Id(id) {}

MovieDetails::MovieDetails(const std::string &title,
                           const std::string &description)
    : Title(title), Description(description) {}

}  // namespace cpx