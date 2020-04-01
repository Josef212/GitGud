#include <GitGud.h>

class SandboxApp : public GitGud::Application
{
public:
	SandboxApp() {}
	~SandboxApp() {}
};

GitGud::Application* GitGud::CreateApplication()
{
	return new SandboxApp();
}
