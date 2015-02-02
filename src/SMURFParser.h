/*
 *  Copyright 2015, DFKI GmbH Robotics Innovation Center
 *
 *  This file is part of the MARS simulation framework.
 *
 *  MARS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  MARS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with MARS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \file SMURFParser.h
 * \author Kai von Szadkowski
 */
#ifndef SMURF_PARSER_H
#define SMURF_PARSER_H

#ifdef _PRINT_HEADER_
  #warning "SMURFParser.h"
#endif

#include <boost/function.hpp>
#include <urdf_model/model.h>
#include <configmaps/ConfigData.h>

namespace smurf_parser {

  boost::shared_ptr<urdf::ModelInterface> parseFile(configmaps::ConfigMap* map,
      std::string path, std::string smurffilename, bool expandURIs);

} // end of namespace smurf_parser

#endif  // SMURF_PARSER_H
