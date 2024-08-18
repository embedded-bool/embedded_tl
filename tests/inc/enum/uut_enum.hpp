/**
 * @file uut_enum.hpp
 * @date 2024-08-10
 * @author Robert Morley
 *
 * @brief 
 *
 * @version v0.1.0
 * @copyright Copyright (c) 2024
 */
#ifndef EMBEDDED_TEMPLATE_LIBRARY_UUT_ENUM_HPP
#define EMBEDDED_TEMPLATE_LIBRARY_UUT_ENUM_HPP

#include <embedded_types.hpp>

CATCH_REGISTER_ENUM(embtl::STATUS,
  embtl::STATUS::ERROR,
  embtl::STATUS::NOT_IMPLEMENTED,
  embtl::STATUS::NOT_AVAILABLE,
  embtl::STATUS::BUFFER_OVERFLOW,
  embtl::STATUS::INVALID_PARAMETER,
  embtl::STATUS::OUT_OF_RANGE,
  embtl::STATUS::UNINITIALIZED,
  embtl::STATUS::OK,
  embtl::STATUS::BUSY,
  embtl::STATUS::INITIALIZED
)

#endif //EMBEDDED_TEMPLATE_LIBRARY_UUT_ENUM_HPP
