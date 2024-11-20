#ifndef CG_IMPORTER_HPP
#define CG_IMPORTER_HPP

#include "model.hpp"

#include <optional>
#include <string>

auto import_model(const std::string& obj_path) -> std::optional<Model>;

#endif