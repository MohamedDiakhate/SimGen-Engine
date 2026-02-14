#include <iostream>
#include "nodes/randomizers/color_randomizer.h"
#include "fabric/scene_database.h"

ColorRandomizer::ColorRandomizer(std::string name, int ID,
                                 const ColorConfig &cfg)
    : RandomizerNode(std::move(name), ID, cfg.seed),
      col_conf(cfg)

{
}

void ColorRandomizer::execute(Scene &scene)
{

  for (auto &entity : scene.getAllEntities())
  {
    if (entity != nullptr && !entity->isStatic)
    {
      float r_val = getRandomFloat(col_conf.minColor.r(), col_conf.maxColor.r());
      float b_val = getRandomFloat(col_conf.minColor.r(), col_conf.maxColor.r());
      float g_val = getRandomFloat(col_conf.minColor.r(), col_conf.maxColor.r());

      vec3 newColor = vec3(r_val, b_val, g_val);

      entity->setColor(newColor);
    }
  }
};