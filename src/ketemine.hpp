/**
 * @file ketemine.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#if !defined(KETEMINE_SRC_KETEMINE_HPP_)
#define KETEMINE_SRC_KETEMINE_HPP_

#include "types.hpp"

namespace ktp { namespace keteMine {
  
// main functions
void init();
void run();
// logging functions
void contextInfo();
void versionInfo();
// mouse functions
void captureMouse();
void releaseMouse();

extern bool show_gui;
extern GameState* state;
extern GLFWwindow* window;
extern Size2D window_size;

} } // namespace keteMine / ktp

#endif // KETEMINE_SRC_KETEMINE_HPP_
