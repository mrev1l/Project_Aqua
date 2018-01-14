#pragma once

namespace aqua
{
namespace ecs
{

class BaseComponent
{
public:
	BaseComponent() = default;
	virtual ~BaseComponent() = default;

	virtual bool InitializeComponent() = 0;
};

}
}