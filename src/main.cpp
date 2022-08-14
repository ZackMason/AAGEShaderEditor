
#define WIN32_MEAN_AND_LEAN
#define NOMINMAX

#include "core.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Engine/window.hpp"
#include "Engine/asset_loader.hpp"

#include "Math/transform.hpp"

#include "Graphics/shader.hpp"
#include "Graphics/screen.hpp"

#include "Util/string.hpp"

struct editor_t {
    enum struct mode_e {
        SCREEN, MODEL, ANIMATION, INVALID
    } mode = mode_e::SCREEN;

    asset_loader_t asset_loader;
    screen_t screen;
    resource_handle_t<shader_t> shader;
    std::vector<uniform_variable_t> uniform_variables;
};


int main(int argc, char** argv) {
    window_t window;
    window.width = 640;
    window.height = 480;

    window.open_window();
    window.set_title("AAGE Shader Editor");
    
    editor_t editor;

    if (argc < 2) {
        editor.asset_loader.asset_dir = GAME_ASSETS_PATH;
    } else {
        editor.asset_loader.asset_dir = argv[1];
        logger_t::info(fmt::format("Asset Path set to \'{}\'", editor.asset_loader.asset_dir));
    }

    window.set_event_callback(
        [&editor, &window](event_i& event) {
            event_handler_t handler;

            handler.dispatch<const key_event_t> (event, 
                [&](const key_event_t& event) {
                    switch(event.key) {
                        case GLFW_KEY_R:
                            editor.asset_loader.reload<shader_t>();
                            if (editor.shader.valid()) {
                                editor.uniform_variables = editor.shader.get().get_uniform_variables();
                            }
                            else {
                                editor.uniform_variables.clear();
                            }
                            break;
                    }
                    return true;
            });

            handler.dispatch<const file_dropped_event_t>(event, 
                [&](const file_dropped_event_t& event) {
                    for (const auto file: event.files) {
                        if (has_extension(file, "fs")) {
                            editor.shader = editor.asset_loader.get_shader_vs_fs(remove_path(file), "shaders/screen.vs", "shaders/" + remove_path(file));
                            if (editor.shader.valid()) {
                                editor.uniform_variables = editor.shader.get().get_uniform_variables();
                            } else {
                                editor.uniform_variables.clear();
                            }
                        }
                        else {
                            logger_t::info(fmt::format("Dropped file: {} - Extension \'{}\' Not handled", file, get_extension(file)));
                        }
                    }
                    return true;
            });
    });
    
    while(!window.should_close()) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /////////////////////////////

        /////////////////////////////
        // Update

        /////////////////////////////
        // Render

        if (editor.shader.valid()) {
            editor.shader.get().bind();
            editor.shader.get().set_uniforms(editor.uniform_variables);
        } else {
            resource_handle_t<shader_t> error_shader = editor.asset_loader.get_shader("error", {"shaders/screen.vs", "shaders/error.fs"});
            editor.shader = error_shader;
        }

        switch(editor.mode) {
            case editor_t::mode_e::SCREEN:
                editor.screen.draw();
                break;
            case editor_t::mode_e::MODEL:
            case editor_t::mode_e::ANIMATION:
            default:
                logger_t::warn("Mode Not Implemented");
                break;
            case editor_t::mode_e::INVALID:
                logger_t::warn("Mode Invalid");
                editor.mode = editor_t::mode_e::SCREEN;
                break;
        }


        /////////////////////////////
        // UI

   

        if (editor.shader.valid()) {
            ImGui::Begin("Shader Editor");

                if (editor.shader.get().name == "error") {
                    ImGui::Text("Drag a fragment shader onto\nthe window to load it");                    
                } else {
                    ImGui::Text(fmt::format("Shader name: {}", editor.shader.get().name).c_str());

                    ImGui::SameLine();
                    if (ImGui::Button("Reload shader")) {
                        // kind of a hack
                        key_event_t event{GLFW_KEY_R, 0, 0};
                        event.key = GLFW_KEY_R;
                        window.event_callback(event);
                    }
                    
                    ImGui::Text("Uniforms");
                    ImGui::Separator();
                    editor.shader.get().uniform_edit(editor.uniform_variables);

                }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        /////////////////////////////
        window.swap_buffers();
        window.poll_events();
    }

    return 0;
}