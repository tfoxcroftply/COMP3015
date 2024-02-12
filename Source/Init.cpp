#include "helper/scene.h"
#include "helper/scenerunner.h"
#include "ShowcaseScene.h"

int main(int argc, char* argv[]) {
	SceneRunner runner("COMP3015 Assignment by Tobey Foxcroft");
	std::unique_ptr<Scene> scene;
	scene = std::unique_ptr<Scene>(new ShowcaseScene());
	return runner.run(*scene);
}