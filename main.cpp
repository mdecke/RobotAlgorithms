#include <cstdio>
#include <cstdlib>

#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>

namespace {

mjModel* model = nullptr;
mjData* data = nullptr;

mjvCamera camera;
mjvOption option;
mjvScene scene;
mjrContext context;

void KeyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

}  // namespace

int main() {
    char error[1000] = "Could not load scene.xml";
    model = mj_loadXML(SCENE_PATH, nullptr, error, sizeof(error));
    if (!model) {
        std::fprintf(stderr, "%s\n", error);
        return EXIT_FAILURE;
    }
    data = mj_makeData(model);

    if (!glfwInit()) {
        std::fprintf(stderr, "Could not initialize GLFW\n");
        return EXIT_FAILURE;
    }

    GLFWwindow* window = glfwCreateWindow(1200, 900, "MuJoCo Drone", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Could not create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, KeyCallback);

    mjv_defaultCamera(&camera);
    camera.azimuth = 120;
    camera.elevation = -20;
    camera.distance = 3;

    mjv_defaultOption(&option);
    mjv_defaultScene(&scene);
    mjr_defaultContext(&context);

    mjv_makeScene(model, &scene, 2000);
    mjr_makeContext(model, &context, mjFONTSCALE_150);

    while (!glfwWindowShouldClose(window)) {
        mjtNum simstart = data->time;
        while (data->time - simstart < 1.0 / 60.0) {
            mj_step(model, data);
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        mjrRect viewport = {0, 0, width, height};

        mjv_updateScene(model, data, &option, nullptr, &camera, mjCAT_ALL, &scene);
        mjr_render(viewport, &scene, &context);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    mjr_freeContext(&context);
    mjv_freeScene(&scene);
    mj_deleteData(data);
    mj_deleteModel(model);

    glfwTerminate();
    return EXIT_SUCCESS;
}
