/**
 * @file gui.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief GUI for the app.
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_GUI_GUI_HPP_)
#define KETEMINE_SRC_GUI_GUI_HPP_

#include "../types.hpp"

namespace ktp { namespace gui {

void clean();
void draw();
void init(GLFWwindow* window);

void mainWindow();
void shaders();
void textures();

} } // namespace gui/ktp

#endif // KETEMINE_SRC_GUI_GUI_HPP_
