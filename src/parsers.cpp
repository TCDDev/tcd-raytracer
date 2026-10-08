#include <raytracer/parsers.hpp>

#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <vector>
#include <string>

#include <raytracer/vec3.hpp>
#include <raytracer/scene.hpp>
#include <raytracer/camera.hpp>
#include <raytracer/material.hpp>
#include <raytracer/triangle.hpp>
#include <raytracer/sphere.hpp>


namespace raytracer {

    namespace {
    
        using MaterialMap = std::unordered_map<std::string, Material>;
        using Object = std::variant<Sphere, Triangle>;
        using Objects = std::vector<Object>;

        Vec3 parse_vec3 (const nlohmann::json& value) {
            return Vec3{
                value.at(0).get<double>(),
                value.at(1).get<double>(),
                value.at(2).get<double>()
            };
        }

        Material parse_material(const nlohmann::json& material_data) {
            return {
                .diffuse = parse_vec3(material_data.at("diffuse")),
                .specular = parse_vec3(material_data.at("specular")),
                .shininess = material_data.at("shininess").get<double>(),
                .reflectivity =  material_data.at("reflectivity").get<double>()
            };
        } 
        
        MaterialMap parse_materials(const nlohmann::json& materials_data) {
            MaterialMap materials;

            for (const auto& [name, material_data] : materials_data.items()) {
                materials.emplace(name, parse_material(material_data));
            }

            return materials;
        }

        Sphere parse_sphere (const nlohmann::json& sphere_data, const Material& material) {
            return {
            parse_vec3(sphere_data.at("center")),
            sphere_data.at("radius").get<double>(),
            material
            };  
        }

        Triangle parse_triangle (const nlohmann::json& triangle_data, const Material& material) {
            return {
                parse_vec3(triangle_data.at("vertices").at(0)),
                parse_vec3(triangle_data.at("vertices").at(1)),
                parse_vec3(triangle_data.at("vertices").at(2)),
                material
            };
        }

        Objects parse_objects (const nlohmann::json& data, const MaterialMap& materials) {
            Objects objects;
            
            for (const auto& object_data : data) {
                const std::string type = object_data.at("type").get<std::string>();
                const std::string material_name = object_data.at("material").get<std::string>();
                const Material& material = materials.at(material_name);

                if (type == "sphere") {
                    objects.emplace_back(parse_sphere(object_data, material));
                }

                else if (type == "triangle") {
                    objects.emplace_back(parse_triangle(object_data, material));
                }

                else {
                    throw(std::runtime_error) ("unknown object type: " + type);
                }
            }

            return objects;
        }

        
    }

    ParsedScene parse_scene (const std::string& filename) {
        std::ifstream file (filename);

        if (!file) {
            throw std::runtime_error ("could not open scene file: " + filename);
        }

        nlohmann::json data;
        file >> data;

        ParsedScene result;

        const nlohmann::json image_data = data.at("image");

        result.width = image_data.at("width").get<int>();
        result.height = image_data.at("height").get<int>();

        const nlohmann::json camera_data = data.at("camera");

        result.camera = {
            .eye = parse_vec3(camera_data.at("eye")),
            .look_at = parse_vec3(camera_data.at("look_at")),
            .up = parse_vec3(camera_data.at("up")),
            .fov = camera_data.at("fov").get<double>()
        };

        result.scene.set_background(parse_vec3(data.at("background")));
        result.scene.set_ambient(parse_vec3(data.at("ambient")));

        for (const auto& light_data : data.at("lights")) {
            result.scene.add(
                Light {
                    .position = parse_vec3(light_data.at("position")),
                    .color = parse_vec3 (light_data.at("color"))
                }
            );
        }

        const MaterialMap materials = parse_materials(data.at("materials"));
        const Objects objects = parse_objects(data.at("objects"), materials);

        for (const Object& object : objects) {
            std::visit(
                [&result](const auto& geometry) {
                    result.scene.add(geometry);
                }, object
            );
        }

        result.scene.create_bvh();

        return result;
        }
   
}
