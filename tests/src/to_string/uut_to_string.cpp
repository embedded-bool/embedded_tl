/**
 * @file uut_to_string.cpp
 * @date 2024-10-05
 * @author Robert Morley
 *
 * @brief
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#include <to_string/uut_to_string.hpp>
#include <string>

std::ostream& embtl::operator<<(std::ostream& os, const STATUS& status) noexcept {
  std::string status_str { "?" };

  switch (status) {
    case STATUS::ERROR              : status_str = "ERROR"; break;
    case STATUS::NOT_IMPLEMENTED    : status_str = "NOT_IMPLEMENTED"; break;
    case STATUS::NOT_AVAILABLE      : status_str = "NOT_AVAILABLE"; break;
    case STATUS::BUFFER_OVERFLOW    : status_str = "BUFFER_OVERFLOW"; break;
    case STATUS::INVALID_PARAMETER  : status_str = "INVALID_PARAMETER"; break;
    case STATUS::OUT_OF_RANGE       : status_str = "OUT_OF_RANGE"; break;
    case STATUS::UNINITIALIZED      : status_str = "UNINITIALIZED"; break;
    case STATUS::OK                 : status_str = "OK"; break;
    case STATUS::BUSY               : status_str = "BUSY"; break;
    case STATUS::INITIALIZED        : status_str = "INITIALIZED"; break;
    default: break;
  }

  os << status_str;

  return os;
}