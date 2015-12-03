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
 *
 * \file SMURFParser.cpp
 * \author Kai von Szadkowski
 */

#include "SMURFParser.h"
#include <urdf_parser/urdf_parser.h>
#include <boost/filesystem.hpp>

namespace smurf_parser {

  
  /**Replaces relative paths inside the urdf model with absolut paths. */
  void relativeToAbsolut(boost::shared_ptr<urdf::ModelInterface> model,
                         const std::string& urdfPathStr)
  {
    assert(urdfPathStr.size() > 0);
    assert(model.get() != NULL);
    
    //strip filename from urdf path (if there is one)
    boost::filesystem::path urdfPath(urdfPathStr);
    urdfPath.remove_filename(); 
     
    std::map<std::string, boost::shared_ptr<urdf::Link> >::iterator it;
    for(it = model->links_.begin(); it != model->links_.end(); ++it)
    {
      std::vector<boost::shared_ptr<urdf::Visual> >& visuals = it->second->visual_array;
      std::vector<boost::shared_ptr<urdf::Visual> >::iterator visIt;
      for(visIt = visuals.begin(); visIt != visuals.end(); ++visIt)
      {
        boost::shared_ptr<urdf::Geometry> geometry = (*visIt)->geometry;
        if(geometry->type == urdf::Geometry::MESH)
        {
          boost::shared_ptr<urdf::Mesh> mesh = boost::dynamic_pointer_cast<urdf::Mesh>(geometry);
          assert(mesh != NULL); //otherwise implementation error in parser
          boost::filesystem::path path(mesh->filename);
          if(!path.is_absolute())
          {
            boost::filesystem::path p(urdfPath);
            p /= path; //append path to urdfPath
            p = boost::filesystem::canonical(p); //remove symlinks and ".." 
            mesh->filename = p.generic_string();
          }
        }
      }
    }
  }
  
  
  boost::shared_ptr<urdf::ModelInterface> parseFile(configmaps::ConfigMap* map,
          std::string path, std::string smurffilename, bool expandURIs) {

    path+="/"; // secure that path and file are combined correctly

    // parse yaml data and write to provided map, identify path to URDF file
    map->append(configmaps::ConfigMap::fromYamlFile(path+smurffilename, expandURIs));
    //map->toYamlFile("smurfparserdebug.yml");
    std::string urdfpath = "";
    configmaps::ConfigVector::iterator it;
    for(it = (*map)["files"].begin(); it!=(*map)["files"].end(); ++it) {
      boost::filesystem::path filepath((std::string)(*it));
      if(filepath.extension().generic_string() == ".urdf") {
        urdfpath = path + filepath.generic_string();
      }
      else if(filepath.extension() == ".yml") {
        configmaps::ConfigMap tmpconfig =
            configmaps::ConfigMap::fromYamlFile(path+filepath.generic_string());
        (*map).append(tmpconfig);
      }
    }

    // parse URDF model and return
    fprintf(stderr, "  ...loading urdf data from %s.\n", urdfpath.c_str());
    boost::shared_ptr<urdf::ModelInterface> model = urdf::parseURDFFile(urdfpath);
    
    if (!model) {
      return boost::shared_ptr<urdf::ModelInterface>();
    }
    
    //there might be relative paths inside the model. Since we do not return the urdf path 
    //there is no easy way for the caller to know to which dir the paths are relativ. 
    //Therefore they are converted to absolut paths.
    relativeToAbsolut(model, urdfpath);
    return model;
  }
}
