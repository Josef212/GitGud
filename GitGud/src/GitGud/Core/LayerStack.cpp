#include "ggpch.h"
#include "LayerStack.h"

namespace GitGud
{
	LayerStack::LayerStack() : _layerInsertIndex(0)
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : _layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		_layers.emplace(_layers.begin() + _layerInsertIndex, layer);
		_layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(_layers.begin(), _layers.end(), layer);
		if (it != end())
		{
			layer->OnDetach();
			_layers.erase(it);
			_layerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(_layers.begin(), _layers.end(), overlay);
		if (it != end())
		{
			overlay->OnDetach();
			_layers.erase(it);
		}
	}
}